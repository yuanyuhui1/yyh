//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem DigitalOutput
 Module
 Description    Implementation of overloaded methods
                for Subsystems based on T_UNIT.
 
 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>
#include <intrinsics.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "System/Interface/microcontroller.h"
#include "System/Interface/assert.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"

#include "T_DATA_OBJ/Interface/t_data_obj_units.h"
#include "T_DATA_OBJ/Interface/t_data_obj_floatunits.h"
#include "T_DATA_OBJ/Interface/t_data_obj_floatfloatunits.h"
#include "T_DATA_OBJ/Interface/t_data_obj_float_fixedunit.h"
#include "T_DATA_OBJ/Interface/t_data_obj_floatrange_fixedunit.h"
#include "UnitsOfMeasure/Interface/UnitsOfMeasure.h"

#include "DigitalOutput/Interface/DigitalOutput.h"
#include "DigitalOutput/Interface/DigitalOutput_idx.h"
#include "digitaloutput_hardware.h"
#include "digitalOutput_logic.h"
#include "digitaloutput_pulse.h"
#include "digitalOutput_types.h"
#include "digitalOutput_attributes.h"
#include "digitalOutput_overload.h"


#if( TIMER_F1F2_CLOCK == OSZILLATOR_CLOCK )
const TFLOAT PULSE_WIDTH_PRESCALAR_TIME = (1000.0*16*ONE_TIMER_COUNT_LIMIT)/ TIMER_F1F2_CLOCK;//  max. for f32, TB could trigger one cycle(two pulse width)
#elif( UART_F1F2_CLOCK == (OSZILLATOR_CLOCK/2) )
const TFLOAT PULSE_WIDTH_PRESCALAR_TIME = (1000.0*8*ONE_TIMER_COUNT_LIMIT)/ TIMER_F1F2_CLOCK;//  max. for f32, TB could trigger one cycle (two pulse width)
#else
#error invalid definition for UART_F1F2_CLOCK in system.h
#endif




/*-------------------------------------------------------------------------------------------------
  Local functions that address Hardware
-------------------------------------------------------------------------------------------------*/

//-------------------------------------------------------------------------------------------------
/*!
 \brief	This method sets the Pulse-Width Timer registers according to the subsystem configuration.
        Might consider, instead, to invoke from UpdateEXE_DIGITALOUTPUT(), because there it can be
        done under the protection of the interrupt-mask, hence the prototype in the header file.
 \author	Paul Li
 \date   2011-09-07

 \param[in]	pulseWidth TFLOAT pulse width (ms) that the user has configured

 \return :      error-code: 0 = OK
                            1 = pulse-width too small (limited to min configurable pulse-width)
                            2 = pulse-width too big (limited to ceiling value for a U16)
 \warning: none
 \test:
 \n by: Paul Li
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//-------------------------------------------------------------------------------------------------
TUSIGN16 SetPulseWidthTimer(TFLOAT pulseWidth)
{
    // Declare local variables
    TUSIGN32 totalCounts = 0;
    TUSIGN16 pulseWidthFreqCount = 0;
    TUSIGN16 returnCode = (TUSIGN16) ILLEGAL_STATE;     // setting to FAIL_SAFE_ERROR condition
    TFLOAT pulseWidthCountTarget = 0.0f;

    // Ensure that what we set is within the agreed limits, even if the Put checks have failed
    // to spot

    if (pulseWidth < DIGITALOUTPUT_PULSE_WIDTH_MIN)
    {
        // Less than the "prescale" threshold means enabling the one-shot output
        // For pulse-widths less than the min allowed, load the one-shot pulse width timer with the
        // min allowed count value
        pulseWidth = DIGITALOUTPUT_PULSE_WIDTH_MIN;
        // pulse-width was too small, report a warning to the caller
        returnCode = (TUSIGN16) LESSTHAN_RANGEMIN;
    }
    else if (pulseWidth > PULSE_WIDTH_PRESCALAR_TIME)
    {
        // Greater than the "prescale" threshold means disabling the one-shot output
        if (pulseWidth > DIGITALOUTPUT_PULSE_WIDTH_MAX)
        {
            // cap the pulse width at max value
            pulseWidth = DIGITALOUTPUT_PULSE_WIDTH_MAX;
            // pulse-width was too large, report a warning to the caller
            returnCode = (TUSIGN16) GREATERTHAN_RANGEMAX;
        }
        else
        {
            returnCode = (TUSIGN16) OK;
        }
        // Calculate the total number of counts for a timer with clock f1 selected.
        // From this, evenly distribute the total counts in chunks smaller than 2^16.
        // So pulseWidthCountTarget holds the number of times a timer loaded with
        // pulseWidthCountTarget has to expire, to generate the given pulse width.
        totalCounts = (TUSIGN32) ((TIMER_F1F2_CLOCK * pulseWidth * SECONDS_PER_MILLISECOND) + 0.5);
        pulseWidthCountTarget = (totalCounts >> 16) + 1;
        pulseWidthFreqCount = (TUSIGN16) (totalCounts / pulseWidthCountTarget);

        
        if((digitalOutputDynamic.oldOutputMode == DIGITALOUTPUT_DO1_NONE)||
                (digitalOutputDynamic.oldOutputMode == DIGITALOUTPUT_DO1_LOGIC)
                  ||(digitalOutputDynamic.oldOutputMode == DIGITALOUTPUT_DO1_FREQUENCY))
        {
            InitTimers_LongPulse();
            gPulseOutput.pulseType = LONGPULSE;
        }
        if(gPulseOutput.pulseType == SHORTPULSE)
        {
            InitTimers_LongPulse();
            gPulseOutput.pulseType = LONGPULSE;  
        }
    }
    else
    {
        // Less than the "prescale" threshold means enabling the one-shot output
        returnCode = (TUSIGN16) OK;
        
    }
    // Convert the count value to U16 before transferring to the count register for the
    // pulse-width timer
    if (pulseWidth <= PULSE_WIDTH_PRESCALAR_TIME)
    {
          //p4_0=~p4_0;
          if((gPulseOutput.pulseType == LONGPULSE) || (digitalOutputDynamic.oldOutputMode == DIGITALOUTPUT_DO1_NONE)||
                (digitalOutputDynamic.oldOutputMode == DIGITALOUTPUT_DO1_LOGIC)
                  ||(digitalOutputDynamic.oldOutputMode == DIGITALOUTPUT_DO1_FREQUENCY))
        {
            InitTimers_ShortPulse();
            gPulseOutput.pulseType = SHORTPULSE;
        }		
        InitFwdPulseWidthTimer(pulseWidth);  // Timer TA4
    }

    gPulseOutput.pulseWidthCountTarget = (TUSIGN16) pulseWidthCountTarget;
    gPulseOutput.pulseWidthFreqCount = pulseWidthFreqCount;
    gPulseOutput.pulseWidth = pulseWidth;
    gPulseOutput.pulseWidthChange = 0;

    return returnCode;

}  // end SetPulseWidthTimer()




//-------------------------------------------------------------------------------------------------
/*!
 \brief change the min freq.
 \author Paul Li
 \date	2011-09-26
 \param *ptrValue
 \return port value.
 \warning
 \test
 \n test-date: 2011-09-27
 \n by: Paul Li
 \n environment: IAR Embedded Workbench 3.5
 \n intention:
 \n result module test: OK
 \n result Lint check: OK
 \bug		
*/
//-------------------------------------------------------------------------------------------------
void changingMinfreq(const TFLOAT* ptrValue)
{
    // Declare local variables
    TFLOAT maxfreq = *ptrValue;

    if (maxfreq > DIGITALOUTPUT_OUTPUTFREQ_MAX)
    {
        maxfreq = DIGITALOUTPUT_OUTPUTFREQ_MAX;
    }

    if (maxfreq < DIGITALOUTPUT_OUTPUTFREQ_MIN)
    {
        maxfreq = DIGITALOUTPUT_OUTPUTFREQ_MIN;
    }

    (void)digitalOutput_Put((TUSIGN16)DIGITALOUTPUT_IDX_maxFrequencyRange, (TINT16)FLOATFLOATUNITS_min, &maxfreq);

    if (plantData.maxFrequency.value < maxfreq)
    {
        (void)digitalOutput_Put((TUSIGN16)DIGITALOUTPUT_IDX_maxFrequency, (TINT16)FLOAT_FIXEDUNIT_value, &maxfreq);
    }		

}

//-------------------------------------------------------------------------------------------------
/*!
 \brief Change the max freq.
 \author Paul Li
 \date	2011-09-26
 \param *ptrValue
 \return port value.
 \warning
 \test
 \n test-date: 2011-09-27
 \n by: Paul Li
 \n environment: IAR Embedded Workbench 3.5
 \n intention:
 \n result module test: OK
 \n result Lint check: OK
 \bug		
*/
//-------------------------------------------------------------------------------------------------
void changingMaxfreq(const TFLOAT* ptrValue)
{
  // Declare local variables
    TFLOAT maxfreq = *ptrValue;
    TUSIGN16 errorCode = (TUSIGN16)FATAL_ERROR;

    if (maxfreq > DIGITALOUTPUT_OUTPUTFREQ_MAX)
    {
        maxfreq = DIGITALOUTPUT_OUTPUTFREQ_MAX;
    }

    if (maxfreq < DIGITALOUTPUT_OUTPUTFREQ_MIN)
    {
        maxfreq = DIGITALOUTPUT_OUTPUTFREQ_MIN;
    }

    errorCode = digitalOutput_Put((TUSIGN16)DIGITALOUTPUT_IDX_minFrequencyRange, (TINT16)FLOATFLOATUNITS_max, &maxfreq);

    if( (plantData.minFrequency.value > maxfreq) && ( errorCode== (TUSIGN16)OK))
    {
        errorCode = digitalOutput_Put((TUSIGN16)DIGITALOUTPUT_IDX_minFrequency, (TINT16)FLOAT_FIXEDUNIT_value, &maxfreq);
    }
}


//-------------------------------------------------------------------------------------------------
/*!
 \brief  Specific configuration item check: back-calculate the fullscale frequency since the pulse
         factor has changed. Set/clear specific Diagnostic Alarm(s).
 \author Kevin Lu
 \date
 \param  -
 \return errorCode
 \warning: -
 \test:
 \n environment: IAR Embedded Workbench 3.5
 \n by: Kevin Lu
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//-------------------------------------------------------------------------------------------------
void pulseFactorChange()
{
    // Declare local variables
    TUSIGN16 errorCode = (TUSIGN16)FATAL_ERROR;
    TFLOAT pulseWidthRangeMax = 0.0f;



    // (2) Calculate new pulse width range max.
    //
    //		 pulse width range max = 1000 / (2 x pulse factor(p/totUnit)  Qmax(totUnit/s) )
    //
    if(digitalOutputDynamic.gMaxData != 0.0)
    {
        pulseWidthRangeMax = ((MILLISECONDS_PER_SECOND * digitalOutputDynamic.timebaseforcalc)/ (2.0f * plantData.pulseFactor.value 
                            * digitalOutputDynamic.gMaxData))/DIGITALOUTPUT_PULSE_FACTOR_MULTI_WIDTH;
    }
    
    
    // (3) Make sure the calculated upper limit fits into the static ranges
    if (pulseWidthRangeMax > DIGITALOUTPUT_PULSE_WIDTH_MAX)
    {
        pulseWidthRangeMax = DIGITALOUTPUT_PULSE_WIDTH_MAX;
    }

    if (pulseWidthRangeMax < DIGITALOUTPUT_PULSE_WIDTH_MIN)
    {
        pulseWidthRangeMax = DIGITALOUTPUT_PULSE_WIDTH_MIN;
    }

    // (4) Put pulseWidth Range max into NV-MEM.
    errorCode = digitalOutput_Put((TUSIGN16)DIGITALOUTPUT_IDX_pulseWidthRange, (TINT16)FLOATRANGE_FIXEDUNIT_max, &pulseWidthRangeMax);

    if(errorCode == (TUSIGN16)OK)
    {
        // (5) Check if pulse width is larger than pulse width range max.
        if (plantData.pulseWidth.value > pulseWidthRangeMax)
        {
            // (5.1) if yes, Adjust pulse width to be pulse width range max, and Put it into NV-MEM.
            errorCode = digitalOutput_Put((TUSIGN16)DIGITALOUTPUT_IDX_pulseWidth, (TINT16)FLOAT_FIXEDUNIT_value, &pulseWidthRangeMax);
            // send warning to diagnosis
            ENABLE_DATA_PROTECTION(*(digitalOutput.unit.ptrDataSemaphore));   // lock semaphore 
            gPulseOutput.pulseWidthChange = 1;
            digitalOutputDynamic.alarmState_DO |= DIGITALOUTPUT_ALARM_BEHAVIOUR_FAIL_LOGIC_OUT_1;
            DISABLE_DATA_PROTECTION(*(digitalOutput.unit.ptrDataSemaphore)); // release semaphore
        }
        else
        {
            ENABLE_DATA_PROTECTION(*(digitalOutput.unit.ptrDataSemaphore));   // lock semaphore  
            digitalOutputDynamic.alarmState_DO &= (~DIGITALOUTPUT_ALARM_BEHAVIOUR_FAIL_LOGIC_OUT_1);
            DISABLE_DATA_PROTECTION(*(digitalOutput.unit.ptrDataSemaphore)); // release semaphore
        }
        
    }
}


//-------------------------------------------------------------------------------------------------
/*!
 \brief  Specific configuration item check: back-calculate the fullscale frequency since the pulse
         width has changed. Set/clear specific Diagnostic Alarm(s).
 \author Paul Li
 \date
 \param  -
 \return errorCode
 \warning: none
 \test:
 \n environment: IAR Embedded Workbench 3.5
 \n by: Kevin Lu
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//-------------------------------------------------------------------------------------------------
void pulseWidthChange(void)
{
    // Declare local variables
    TFLOAT pulseFactorRangeMax = 0.0f;

    if(digitalOutputDynamic.gMaxData != 0.0)
    {
        pulseFactorRangeMax = ((MILLISECONDS_PER_SECOND * digitalOutputDynamic.timebaseforcalc) / (2.0f * plantData.pulseWidth.value \
                               * digitalOutputDynamic.gMaxData)) / DIGITALOUTPUT_PULSE_FACTOR_MULTI_WIDTH;
    }

    if (pulseFactorRangeMax > DIGITALOUTPUT_PULSE_FACTOR_MAX)
    {
        pulseFactorRangeMax = DIGITALOUTPUT_PULSE_FACTOR_MAX;
    }

    if (pulseFactorRangeMax < DIGITALOUTPUT_PULSE_FACTOR_MIN)
    {
        pulseFactorRangeMax = DIGITALOUTPUT_PULSE_FACTOR_MIN;
    }

    if (plantData.pulseFactor.value > pulseFactorRangeMax)
    {
        digitalOutputDynamic.alarmState_DO |= DIGITALOUTPUT_ALARM_BEHAVIOUR_FAIL_LOGIC_OUT_1;
        digitalOutputDynamic.do1Available = eFALSE;
    }
    else
    {
        digitalOutputDynamic.alarmState_DO &=(~DIGITALOUTPUT_ALARM_BEHAVIOUR_FAIL_LOGIC_OUT_1);
        digitalOutputDynamic.do1Available = eTRUE;
    }
}

//-------------------------------------------------------------------------------------------------
/*!
 \brief qmax changed, the factor and width should be recacled.
 \author Paul Li
 \date	2011-09-26
 \param -
 \return -
 \warning
 \test
 \n test-date: 2011-09-27
 \n by: Paul Li
 \n environment: IAR Embedded Workbench 3.5
 \n intention:
 \n result module test: OK
 \n result Lint check: OK
 \bug		
*/
//-------------------------------------------------------------------------------------------------
void QmaxChange(void)
{
    TFLOAT connectedupdatevalue = 0.0f;
    TBOOL convert;

    //change m3/h To the internal unit.
    convert = unitsOfMeasure.ConvertToBaseTime(digitalOutputDynamic.unitType,digitalOutputDynamic.unitCode, &connectedupdatevalue);
     //convert |= unitsOfMeasure.ConvertFromBaseUnit(digitalOutputDynamic.unitType,digitalOutputDynamic.unitCode, &digitalOutputDynamic.gMaxData);
    if(convert)
    {
    }
    else
    {
        connectedupdatevalue = 1.0f;
    }

    digitalOutputDynamic.timebaseforcalc = connectedupdatevalue;
    pulseFactorChange();
    pulseWidthChange();
}

//-------------------------------------------------------------------------------------------------
/*!
 \brief  initialize hardware ports used in Digital Output.
 \author Paul li
 \date
 \return void
 \param  -
 \test
 \warning: none
 \test:
 \n environment: IAR Embedded Workbench 3.5
 \n by: Peter Vince
 \n result module test: 2005-12-16 OK
 \n result Lint Level 3: free
 \n by: Peter vince
 \bug
*/
//-------------------------------------------------------------------------------------------------
void InitDigitalOutputPorts(void)
{
#if 0
    LOGIC_OP1_PORT_DR = OUTPUT_PORT;    // OP1 port direction set for O/P
    LOGIC_OP1_PORT    = OUTPUT_LOW;     //     and O/P set low initially
#endif
}


//-------------------------------------------------------------------------------------------------
/*!
 \brief  initialize the Digital Output Data requirements
 \author  Kevin Lu
 \date   2010-11-12
 \param  -
 \return void
 \warning: none
 \test: 
 \n environment: IAR Embedded Workbench 3.5
 \n by: Kevin Lu
 \n result module test: OK
 \n result Lint Level 3: free
 \bug
*/
//-------------------------------------------------------------------------------------------------
void InitDigitalOutputData(void)
{
    digitalOutputDynamic.gMaxData = 0.0f;

    gPulseOutput.pulseType = SHORTPULSE;

    digitalOutputDynamic.oldOutputMode = DIGITALOUTPUT_DO1_NONE;
} 
//-------------------------------------------------------------------------------------------------
/*!
 \brief  initialize the subsystem; (hardware, tasks, data, calculation)
 \author 
 \date   
 \param  *me,typeOfStartUp
 \return error-code
 \test
 \n by: Paul Li
 \n environment: IAR Embedded Workbench 3.5
 \n intention: 
 \n result module test:
 \n result Lint Level 3: 
 \bug
*/
//-------------------------------------------------------------------------------------------------
TUSIGN16 Initialize_DIGITAL_OUTPUT(const T_UNIT SLOW*me, TUSIGN8 typeOfStartUp)
{
    TUSIGN16 result = (TUSIGN16)FATAL_ERROR;
    // to help manage initialization
    static TBOOL hwInitialized = eFALSE;
    static TBOOL timerInitialized = eFALSE;
    static TBOOL dataInitialized = eFALSE;

    //lint -e{746,730} 
    VIP_ASSERT(me==DIGITAL_OUTPUT_ME);

    result = Initialize_T_UNIT(me,typeOfStartUp);

    //lint -e{730} //VIP_ASSERT Bool
    if( result==(TUSIGN16)OK )
    {
        if( (typeOfStartUp & (TUSIGN16)INIT_HARDWARE) != 0)
        {               
            if (!hwInitialized) // if not already done
            {
                InitDigitalOutputPorts();    // address the basic hardware requirements
                InitDigitalOutputTimers();
                hwInitialized = eTRUE;
            }
        }

        if( (typeOfStartUp & (TUSIGN16)INIT_DATA) != 0)
        {
            if (!dataInitialized)       // if not already done
            {
                // Initialize subsystem variables held in the dynamic data class.
                // (All others will be loaded up by the NV subsystem.)
                (void)me->LoadRomDefaults(me, (TINT16)DIGITALOUTPUT_DC_IDX_DYNAMIC);
                dataInitialized = eTRUE;
            }
        }

        if( (typeOfStartUp & (TUSIGN16)INIT_TASKS) != 0)
        {
            if (!timerInitialized)      // if not already done
            {
                timerInitialized = eTRUE;
            }
        }

        // if the caller didn't ask for up-to-and-including initialization of data
        // then we shouldn't declare this subsystem as initialized:
        if (dataInitialized == (TBOOL)eFALSE)
        {   // so...
            hwInitialized = eFALSE;     // force reinitialization of h/w
            timerInitialized = eFALSE;  // force reinitialization of timers
            // initialization must start from scratch again...
            digitalOutputState = (TUSIGN16) NOT_INITIALIZED;
        }
    }
    return result;
}


//-------------------------------------------------------------------------------------------------
/*!
 \brief Put method overloaded in Digital Output subsystem because of restrictions to writing
        certain objects. Put the attribute attributeIndex of the data-object objectIndex; only in
        "INITIALIZED".

 \author Kevin Lu
 \date   2010-11-12
 \param[in] me is a pointer to the T_DIGITALOUTPUT object
 \param[in] objectIndex: index of the data object
 \param[in] attributeIndex: index of the data objects' attribute ; <0 means put whole object
 \param[in] ptrValue pointer to object/attribute-value and return-value
 \return error-code:

   OK                      operation was successful
   LESSTHAN_RANGEMIN       at least one value is less than its minimum value
   GREATERTHAN_RANGEMAX    at least one value is greater than its maximum
   SPAN_TO_SMALL           then span between two values is to small
   NOT_ON_GRID             at least one value is not a multiple of its increment
   RULE_VIOLATION          this shows a violation of one or more businessrules
   DECLINE                 DEBUG, operation isn't permitted in the aktual unit-state
   ILLEGAL_OBJ_IDX         DEBUG, unknown Object DEBUG, unknown Attribute

\warning: none
\test
\n by:	Kevin Lu
\n environment: IAR Embedded Workbench 3.5
\n intention:  1. complete code covarage; 2. equivalence partitioning 3. Boundary Value Analysis (parameter)
\n result module test:
\n result Lint Level 3: free
 */
//-------------------------------------------------------------------------------------------------
TUSIGN16 Put_DIGITAL_OUTPUT(const T_UNIT SLOW * me, TUSIGN16 objectIndex, 
                                    TINT16 attributeIndex, void FAST * ptrValue)
{
    // Declare local variables
    const T_DATA_OBJ SLOW *ptrObj = 0;
    TUSIGN16 result = (TUSIGN16) ILLEGAL_STATE; // setting to FAIL_SAFE_ERROR condition

    // check for zeropointer
    VIP_ASSERT(me);
    VIP_ASSERT(ptrValue);
    VIP_ASSERT(me->ptrObjectList);

    // exception if not initialized
    VIP_ASSERT(*(me->ptrState) >= (TUSIGN16) INITIALIZED);

    // unknown object
    if (objectIndex > me->maxIdx)
    {
        result = (TUSIGN16) ILLEGAL_OBJ_IDX;
    }
    else
    {
        // handmade code optimizer
        ptrObj = &(me->ptrObjectList[objectIndex]);

        // Digital Output specific parameter checks
        switch (objectIndex)
        {
            //case DIGITALOUTPUT_IDX_output1Freq:    // ..."display-only" output frequency
            case DIGITALOUTPUT_IDX_logicValue:   // ..."display-only" logic output state
                result = (TUSIGN16) READ_ONLY_ERR;
                break;
            default:
                //result= Put_T_UNIT(me,objectIndex,attributeIndex,ptrValue);
                result = ptrObj->pClass->Put(ptrObj, ptrValue, attributeIndex, me->ptrDataSemaphore);
                break;
        }

        // Digital Output specific parameter checks
             
        if (result == (TUSIGN16) OK)//&&(plantData.outputMode_1 == (TUSIGN8) DIGITALOUTPUT_DO1_PULSE)
        {
            switch (objectIndex)
            {
              case DIGITALOUTPUT_IDX_pulseWidth:
                // Re-set pulse generating Timer.
                ENABLE_DATA_PROTECTION(*(me->ptrDataSemaphore));   // lock semaphore 
                gPulseOutput.pulseWidthChange = 1;
                digitalOutputDynamic.alarmState_DO = DIGITALOUTPUT_ALARM_BEHAVIOUR_OK;
                pulseWidthChange();
                DISABLE_DATA_PROTECTION(*(me->ptrDataSemaphore)); // release semaphore
                break;
              case DIGITALOUTPUT_IDX_pulseFactor:
                // Need to re-calculate pulse width range and pulse width.
                ENABLE_DATA_PROTECTION(*(me->ptrDataSemaphore));   // lock semaphore 
                digitalOutputDynamic.alarmState_DO = DIGITALOUTPUT_ALARM_BEHAVIOUR_OK;
                DISABLE_DATA_PROTECTION(*(me->ptrDataSemaphore)); // release semaphore
                pulseFactorChange();
                break;
              case DIGITALOUTPUT_IDX_maxFrequency:
                changingMaxfreq(ptrValue);
                break;
              case DIGITALOUTPUT_IDX_minFrequency:
                changingMinfreq(ptrValue);
                break;
              default:
                // Do nothing.
                break;
            }
        }

        // --Digital Output specific parameter checks
    }
    return result;
}       // end Put_DIGITALOUTPUT
