//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem DigitalOutput
 Module
 Description    Subsystem constructor
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
#include <limits.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#ifdef LOCALSERVER_SELECT
#include "UnitTests/LocalServer/webserver_main.h"
#endif
#include "RTOS/Interface/rtos.h"
#include "System/Interface/microcontroller.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"

#include "T_DATA_OBJ/Interface/t_data_obj_units.h"
#include "T_DATA_OBJ/Interface/t_data_obj_floatunits.h"
#include "T_DATA_OBJ/Interface/t_data_obj_floatfloatunits.h"
#include "T_DATA_OBJ/Interface/t_data_obj_float_fixedunit.h"
#include "T_DATA_OBJ/Interface/t_data_obj_floatrange_fixedunit.h"


#include "DigitalOutput/Interface/DigitalOutput.h"
#include "digitaloutput_hardware.h"
#include "digitalOutput_logic.h"
#include "digitaloutput_pulse.h"
#include "digitalOutput_types.h"
#include "digitalOutput_attributes.h"
#include "DigitalOutput_Frequency.h"
#include "digitalOutput_execute.h"

#define TIMERS_RUNNING      (0U)
#define RESTART_TIMERS      (1U)
#define STOP_TIMERS         (2U)

#define LOWEST_FREQ_NUM   100.0

const TUSIGN16 FREQ_DIV_PRECISION_BIG = 500;//
const TUSIGN16 FREQ_DIV_PRECISION_SMALL = 10;//
const TUSIGN16 FREQ_DIV_BASE = 50;//every 1/50 s a the freq is out as we want.

TFrequencyOutputData gFrequencyOutput; // Frequency Output working data


//-------------------------------------------------------------------------------------------------
/*!
 \brief     Check if "x" is odd or even. 
 \author    Kevin Lu
 \date      2010-6-22
 \param     x 
 \return    eTRUE / eFALSE
 \warning
 \test
 \n test-date: 2010-12-11
 \n by: Kevin Lu
 \n environment:    Embedded Workbench 5.4
 \n intention:  instruction coverage
 \n result module test: OK
 \n result Lint check: OK
 \bug		
*/
//-------------------------------------------------------------------------------------------------
TBOOL ISODD(TUSIGN32 x)
{
    if ((x%2) != 0)
    {
      return eTRUE;
    }
    else
    {
      return eFALSE;
    }
}

//-------------------------------------------------------------------------------------------------
/*!
 \brief Convert percent to frequent.
 \author Paul Li
 \date	2011-09-26
 \param connectedupdatevalue, *ptrFreqValue
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
static TUSIGN16  ConvertDVInPercentToFrequency( TFLOAT connectedupdatevalue, TFLOAT *ptrFreqValue )
{  
    TUSIGN16 returnCode = (TUSIGN16) OK;     // setting to OK condition
    TFLOAT tempFreqValue = 0.0f;
    if( digitalOutputDynamic.simuEnabled == DIGITALOUTPUT_SIMU_ENABLED)
    {
        tempFreqValue = digitalOutputDynamic.simuFrequency;
    }
    else
    {
        tempFreqValue =( connectedupdatevalue * (plantData.maxFrequency.value - plantData.minFrequency.value) ) + plantData.minFrequency.value;
    }
    
    if( tempFreqValue < DIGITALOUTPUT_OUTPUTFREQ_MIN ) // 0.25Hz
    {
        tempFreqValue = DIGITALOUTPUT_OUTPUTFREQ_MIN; // 0.0Hz
        returnCode = (TUSIGN16) LESSTHAN_RANGEMIN;
    }
    
    if( tempFreqValue > DIGITALOUTPUT_OUTPUTFREQ_MAX ) // 10500.0Hz
    {
        tempFreqValue = DIGITALOUTPUT_OUTPUTFREQ_MAX;   // 10500.0Hz
        returnCode = (TUSIGN16) GREATERTHAN_RANGEMAX;
    }
    
    *ptrFreqValue = tempFreqValue;
    
    return returnCode;
}


//-------------------------------------------------------------------------------------------------
/*!
 \brief Calc Timer registor through frequence.
        also the justfy timer 
        the assignment should occur once 
 \author Paul Li
 \date	2011-09-26
 \param Freq_FrequencyOutput
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
static void  CalculateTimerContentForFrequencyOutput( TFLOAT Freq_FrequencyOutput )
{     
        TFLOAT      MarkLength;
        TFLOAT      PeriodLength;
        TFLOAT      TotalCounts;
        TUSIGN16    quotient;
        TUSIGN16    remainder;
        TUSIGN16    FREQ_DIV_PRECISION;
        TUSIGN16    FREQ_DIV_SPACE;
        TUSIGN16    NumCounterSpace = 0;
        TUSIGN16    NumCounterMark = 0;
        TUSIGN16    baseFreqCounter = 0;    // Counter value for Base frequency [in Timer Mode]
        TUSIGN16    NumCounter = 0;		// Number of Counter [in Eventer Counter Mode]
        TUSIGN8     needreinit = TIMERS_RUNNING;
        TUSIGN8     NeedChangeReg = 0;
        TUSIGN32    halfCounts = 0L;

        static TFLOAT clocktimer = TIMER_F1F2_CLOCK;

        #define HD_SUPPORT_MIN_FREQ 0.25f  //clocktimer/(65536*2)=0.000429

        //static TFLOAT PeriodLength_old = 1.0;
        
        if (Freq_FrequencyOutput >= HD_SUPPORT_MIN_FREQ)
        {
            if (gFrequencyOutput.needreinit == STOP_TIMERS)
            {
                needreinit = RESTART_TIMERS; //restart timer
            }
            else { }
            

            FREQ_DIV_PRECISION = (TUSIGN16)(Freq_FrequencyOutput/FREQ_DIV_BASE);//
            if(FREQ_DIV_PRECISION < FREQ_DIV_PRECISION_SMALL)
            {
                FREQ_DIV_PRECISION = FREQ_DIV_PRECISION_SMALL;
            }
            else if(FREQ_DIV_PRECISION>FREQ_DIV_PRECISION_BIG)
            {
                FREQ_DIV_PRECISION = FREQ_DIV_PRECISION_BIG;
            }
            else
            {}
            PeriodLength = 1/Freq_FrequencyOutput;
            TotalCounts = clocktimer / Freq_FrequencyOutput;  // Required total counts for one period
            //if the update time is too long, i do not know if we should stop the old output to out put new value or not 
            /*
            if(PeriodLength_old > (UPDATE_INTERVAL*SECONDS_PER_MILLISECOND) && PeriodLength < (UPDATE_INTERVAL*SECONDS_PER_MILLISECOND))
            {
                needreinit = STOP_TIMERS;
            }
            else
            {
                needreinit = TIMERS_RUNNING;
            }
            
            */
            //PeriodLength_old = PeriodLength;
            if( TotalCounts > (65535*2) )
            {
            
                MarkLength = PeriodLength/2 ; // - 0.0000002
                quotient = (TUSIGN16)(((MarkLength * clocktimer )/ (ONE_TIMER_COUNT_LIMIT+1))); // 
                remainder = (TUSIGN16)( (TUSIGN32)(MarkLength * clocktimer) %( ONE_TIMER_COUNT_LIMIT+1) );
                
                if(remainder > 0)
                {
                    baseFreqCounter = (TUSIGN16)(((MarkLength * clocktimer)/(quotient+1))+0.5)-1;
                    NumCounter = quotient;
                }
                else
                {
                    baseFreqCounter = ONE_TIMER_COUNT_LIMIT;
                    NumCounter = quotient-1;
                }
        
            }
            else    // TA4 in Programmable Output Mode
            {
                baseFreqCounter = (TUSIGN16)(TotalCounts/2)-1;
                NumCounter =0 ;
                
                if(Freq_FrequencyOutput >= LOWEST_FREQ_NUM )
                {          
                    NeedChangeReg = 1;
                    halfCounts = (TUSIGN32)(TotalCounts/2);
                    FREQ_DIV_SPACE = (TUSIGN16)(0.5 + (TFLOAT)(FREQ_DIV_PRECISION * 2.0 * ((TotalCounts/2.0 )- (TFLOAT)halfCounts)));
                    NumCounterSpace = FREQ_DIV_SPACE;
                    NumCounterMark =(FREQ_DIV_PRECISION*2) - FREQ_DIV_SPACE;
                    //check if too low, then the justify timer should stopped
                    if(NumCounterSpace < 1)
                    {
                        NeedChangeReg = 0;
                    }
                    else
                    {
                        //else the reg should add -1 to assign to the reg
                        NumCounterSpace -= 1;
                    }
                     //check if too low, then the justify timer should stopped
                    if(NumCounterMark < 1)
                    {
                        NeedChangeReg = 0;
                        baseFreqCounter = baseFreqCounter+1;
                        //the base counter should increase;
                    }
                    else
                    {
                      //else the reg should add -1 to assign to the reg
                        NumCounterMark -= 1;
                    }                   
                }
            }
            
        }
        else
        {
        // Stop all Timers
            needreinit = STOP_TIMERS; 
        }
        //the assignment should occur once because the timer is use it as source
        SuspendAll_RTOS_TASK(); 
        gFrequencyOutput.NeedChangeReg = NeedChangeReg;
        gFrequencyOutput.NumCounterSpace = NumCounterSpace;
        gFrequencyOutput.NumCounterMark = NumCounterMark;
        gFrequencyOutput.baseFreqCounter = baseFreqCounter;
        gFrequencyOutput.NumCounter = NumCounter;
        gFrequencyOutput.needreinit = needreinit;
        ResumeAll_RTOS_TASK();  
}

//-------------------------------------------------------------------------------------------------
/*!
 \brief Init frequence by init all timer.
 \author Paul Li
 \date	2011-09-26
 \param -
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
void InitFrequencyOutput(void)
{
#if 0
	output_mode = DIGITALOUTPUT_DO1_FREQUENCY;
    tb2s = 0;
    tb2 = gFrequencyOutput.baseFreqCounter; // write i to TB2
    tb2mr = 0x00;
    ta4s = 0;    
    
    ta4mr = 0x05; // ta4mr Event Counter Mode
    ta4ud = 0; // Set Timer A4 to count down
            // select timer B2 as trigger source
    SELECT_B2_TO_A4();
    ta4 = gFrequencyOutput.NumCounter;// write n to TA4
    ta4ic = 0x00; // ta4ic Disable the interrupt of TA4
#ifdef IO_REVERT_PIN8_0
	pofs4 = 1; // high-level
#else
	pofs4 = 0; // low-level
#endif	
    ta4s = 1;   //ta4s Start Timer A4  
    tb2s = 1;   //tb2s Start Timer B2 
    gFrequencyOutput.OldTA0Status = 0;
#endif
}

//-------------------------------------------------------------------------------------------------
/*!
 \brief Change Freq Output, only change the registor.
 \author Paul Li
 \date	2011-09-26
 \param -
 \warning -
 \test
 \n test-date: 2011-09-27
 \n by: Paul Li
 \n environment: IAR Embedded Workbench 3.5
 \n intention:
 \bug		
*/
//-------------------------------------------------------------------------------------------------
void UpdateFrequency(void)
{ 
#if 0
    ta4ic = 0;
    if(gFrequencyOutput.NeedChangeReg != 0)
    {
        if(gFrequencyOutput.OldTA0Status != 1)
        {
            ta0ic = 0;
            ta0s = 0;
            InitFreqWidthTimer();
            gFrequencyOutput.OldTA0Status = 1;
        }        
    }
    else
    {
        tb2 = gFrequencyOutput.baseFreqCounter; // write i to TB2
        ta4 = gFrequencyOutput.NumCounter;// write n to TA4
        gFrequencyOutput.OldTA0Status = 0;
        ta0ic = 0;
        ta0s = 0;
    }
#endif
}

//-------------------------------------------------------------------------------------------------
/*!
 \brief Update freq Output.
 \author Paul Li
 \date	2011-09-26
 \param -
 \return void.
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
static void UpdateFrequencyOutput(void)
{ 
#if 0
	switch (digitalOutputDynamic.oldOutputMode)
	{
		case DIGITALOUTPUT_DO1_NONE:
		case DIGITALOUTPUT_DO1_LOGIC:
        case DIGITALOUTPUT_DO1_PULSE:
            InitFrequencyOutput();
		break;
		case DIGITALOUTPUT_DO1_FREQUENCY:
            if(gFrequencyOutput.needreinit == STOP_TIMERS)
            {
                tb2s = 0; //stop tb2
                ta4s = 0;//no freq out; stop ta4
            }
            else if(gFrequencyOutput.needreinit == RESTART_TIMERS)
            {
                InitFrequencyOutput();//time div too long need reset timer;
            }
            else
            {
                UpdateFrequency();
            }
		break;
        default:
		break;
	}
    digitalOutputDynamic.oldOutputMode = digitalOutputDynamic.outputMode;
#endif
}


//-------------------------------------------------------------------------------------------------
/*!
 \brief Update Freq Output.
 \author Paul Li
 \date	2011-09-26
 \param -
 \return returnCode
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
TUSIGN16 UpdateFrequency_DIGITALOUTPUT(TFLOAT flowPercentage)
{
    TUSIGN16 returnCode = (TUSIGN16) OK;     // setting to OK condition 
    TFLOAT  connectedupdatevalue = 0.0f;    
    TFLOAT  Freq_FrequencyOutput = (TFLOAT) ZERO_FREQUENCY;     // intermediate result

    
    connectedupdatevalue=flowPercentage;
    if((connectedupdatevalue >1.05) || (connectedupdatevalue <-1.05))
    {
        connectedupdatevalue = 1.05;
    }
    else if(connectedupdatevalue <0)
    {
        connectedupdatevalue = -connectedupdatevalue;
    }        
    else
    {
    }

    // Convert connectedupdatevalue to frequency.     
    returnCode = ConvertDVInPercentToFrequency( connectedupdatevalue, &Freq_FrequencyOutput);
    digitalOutputDynamic.frequencyValue = Freq_FrequencyOutput;

    CalculateTimerContentForFrequencyOutput(Freq_FrequencyOutput);
    UpdateFrequencyOutput();

    return returnCode;
}