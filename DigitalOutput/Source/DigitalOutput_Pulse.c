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
#include <intrinsics.h>
#include <limits.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#ifdef LOCALSERVER_SELECT
#include "UnitTests/LocalServer/webserver_main.h"
#endif
#include "RTOS/Interface/rtos.h"
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
#include "digitalOutput_execute.h"
#include "digitalOutput_overload.h"



TPulseOutputData gPulseOutput; // Pulse Output working data

// The static gapTime variable is used to keep track of the time span that the actual
// pulse generation takes longer than the 100 ms window. This time will be waited for
// before the new pulses of the following window are generated. This method assures
// that the mark-space ratio is always 1:1 regardless of the pulse width setting.
TFLOAT gapTime = MIN_GAP_TIME;

volatile TPulseOutputAction pulseOutputAction = NO_PULSES;

// Pulse counters to correct extra pulses that might be generated due to interrupt delay
volatile TUSIGN16 ExtraPulses = 0;

//-------------------------------------------------------------------------------------------------
/*!
 \brief  This function is called by InitDigitalOutputTimer to initialize the timer that generates
         the base frequency for the pulse generation. The update timer is used as the counting
         source for the pulse counter timer and to trigger the pulse width timer.
 \author Georg Horst
 \date   2007-05-04

 \param  -

 \return : void
 \warning: none
 \test
 \n by: Paul Li
 \n environment: IAR Embedded Workbench 3.5
 \n result module test: OK
 \n result Lint Level 3: OK
 \bug

*/
//-------------------------------------------------------------------------------------------------
void InitUpdateTimer(void)
{
#if 0
    //***************
    // TB2 Timer Init
    //***************
    
    tb2s = 0;
    // timer mode and clock f1 selected
    tb2mr = 0x00;  // XXXX XXXX: Setup value 0000 0000
                                        // |||| ||++- Selection of timer mode
                                        // |||| ||          00 Timer mode
                                        // |||| ||          01 Event counter mode
                                        // |||| ||          10 Pulse period/width measurement mode
                                        // |||| ||          11 Do not set to this value
                                        // |||| ++--- IN TIMER MODE: Do not effect (Can bet set to "0")
                                        // ||++------ IN TIMER MODE: When write set to "0"
                                        // ++-------- Count Source Select Bit
                                        //                  00 f1 or f2
                                        //                  01 f8
                                        //                  10 f32
                                        //                  11 fC32
    
    // Disable interrupt for update timer
    tb2ic = 0x00;
    
    // Disable update timer initially
    tb2s = 0;
#endif
}


//-------------------------------------------------------------------------------------------------
/*!
 \brief  This function is called by InitDigitalOutputTimer to initialize the timer that counts
         down the number of pulses to be generated. The counting source for this timer is the
         update timer. If an underrun of this timer occurs, an interrupt routine is called to
         disable the pulse output.
 \author Georg Horst
 \date   2007-05-04
 \param  -
 \test
 \n by: Paul Li
 \n environment: IAR Embedded Workbench 3.5
 \n result module test: OK
 \n result Lint Level 3: OK
 \bug

*/
//-------------------------------------------------------------------------------------------------
void InitPulseCountTimer(void)
{
#if 0
    //***************
    // TA0 Timer Init
    //***************
    ta0ic = 0;
    output_mode = DIGITALOUTPUT_DO1_PULSE;
    ta0s = 0;
    //EventCounter Mode and Free-run type selected
    ta0mr =                           0x41; // XXXX XXXX: Setup value 0100 0001
                                            // |||| ||++- Selection of timer mode
                                            // |||| ||          00 Timer mode
                                            // |||| ||          01 Event counter mode
                                            // |||| ||          10 One-shot timer mode
                                            // |||| ||          11 Pulse width modulation(PWM mode)
                                            // |||| |+--- IN EVENT COUNTER MODE:
                                            // |||| |       Pulse Output Function Selected Bit:
                                            // |||| |           0 Pulse is not output
                                            // |||| |           1 Pulse is output
                                            // |||| +---- IN EVENT COUNTER MODE:
                                            // ||||         Count Polarity Selected Bit
                                            // ||||             0 Counts falling edge of external signal
                                            // ||||             1 Counts rising edge of external signal
                                            // |||+------ IN EVENT COUNTER MODE:
                                            // |||          Up/Down Switching Cause Select Bit
                                            // |||              0 UDF register
                                            // |||              1 Input signal to TAiOUT pin
                                            // ||+------  IN EVENT COUNTER MODE: Set to "0"
                                            // |+-------  IN EVENT COUNTER MODE:
                                            // |            Count Operation Type Select Bit
                                            // |                0 Reload type
                                            // |                1 Free-run type
                                            // +--------  IN EVENT COUNTER MODE:
                                            //              Can be "0" or "1"

    // Set Timer A0 to count down
    ta0ud = 0;
    
    // Set trigger for Timer A0 to Timer B2
    SELECT_B2_TO_A0();
    
    // Init counter for Timer A0
    ta0 = 0xFFFF;
    
    ta0ic = 0x07; //interrupt level 7
    // Enable Timer TA0
    ta0s = 1;
#endif
}

//-------------------------------------------------------------------------------------------------
/*!
 \brief  Initialize Timer A4 and B2 for Long Pulse output.
 \author Kevin Lu
 \date   2010-12-11
 \param  -
 \warning: none
 \test
 \n by: Paul Li
 \n environment: IAR Embedded Workbench 3.5
 \n result module test: OK
 \n result Lint Level 3: OK
 \bug

*/
//-------------------------------------------------------------------------------------------------
// TB2 is TA4 counter source, TA4 in Eventer Counter mode.
void InitTimers_LongPulse(void)
{
#if 0
    InitUpdateTimer();
    Init_TimerA0(0x00, 0x0000, 0x00);
    //***************
    // TA4 Timer Init in Event Counter mode
    //***************
    // Stop Timer TA4
    // Stop Timer TA4
    ta4s = 0;
    
    //EventCounter Mode and Free-run type selected
    ta4mr =                           0x01; // XXXX XXXX: Setup value 0100 0001
                                            // |||| ||++- Selection of timer mode
                                            // |||| ||          00 Timer mode
                                            // |||| ||          01 Event counter mode
                                            // |||| ||          10 One-shot timer mode
                                            // |||| ||          11 Pulse width modulation(PWM mode)
                                            // |||| |+--- IN EVENT COUNTER MODE:
                                            // |||| |       Pulse Output Function Selected Bit:
                                            // |||| |           0 Pulse is not output
                                            // |||| |           1 Pulse is output
                                            // |||| +---- IN EVENT COUNTER MODE:
                                            // ||||         Count Polarity Selected Bit
                                            // ||||             0 Counts falling edge of external signal
                                            // ||||             1 Counts rising edge of external signal
                                            // |||+------ IN EVENT COUNTER MODE:
                                            // |||          Up/Down Switching Cause Select Bit
                                            // |||              0 UDF register
                                            // |||              1 Input signal to TAiOUT pin
                                            // ||+------  IN EVENT COUNTER MODE: Set to "0"
                                            // |+-------  IN EVENT COUNTER MODE:
                                            // |            Count Operation Type Select Bit
                                            // |                0 Reload type
                                            // |                1 Free-run type
                                            // +--------  IN EVENT COUNTER MODE:
                                            //              Can be "0" or "1"

    // Set Timer A4 to count down
    ta4ud = 0;

    // Set trigger for Timer A4 to Timer B2
    SELECT_B2_TO_A4();
    
    // Set highest interrupt level for pulse count timer
    ta4ic = 0x01;
    
    // Init counter for Timer A4
    ta4 = 0xFFFF;
    
    // Enable Timer TA4
    ta4s = 0;
#endif
}


//-------------------------------------------------------------------------------------------------
/*!
 \brief  This function is called by InitDigitalOutputTimer to initialize the timer that generates
         the pulse width for the flow pulses. The timer is triggered by the update timer,
         operates in one-shot mode and generates the actual pulse signal at the port pin.
 \author Paul Li
 \date   2011-10-08

 \param  pulseWidth_ms

 \return : void
 \warning: none
 \test
 \n by: Paul Li
 \n environment: IAR Embedded Workbench 3.5
 \n result module test: OK
 \n result Lint Level 3: OK
 \bug

*/
//-------------------------------------------------------------------------------------------------
void InitFwdPulseWidthTimer(TFLOAT pulseWidth_ms)
{
#if 0
    TUSIGN32 numCounts = (TUSIGN32)(TIMER_F1F2_CLOCK * pulseWidth_ms * SECONDS_PER_MILLISECOND);
    //***************
    // TA4 Timer Init
    //***************
    ta4s = 0; //TA4
    

    //One-Shot timer Mode: TB" trigger selected, Pulse output selected 10110
    ta4mr = 0x16;                           // XXXX XXXX: Setup value 0001 0110
                                            // |||| ||++- Selection of timer mode
                                            // |||| ||          00 Timer mode
                                            // |||| ||          01 Event counter mode
                                            // |||| ||          10 One-shot timer mode
                                            // |||| ||          11 Pulse width modulation(PWM mode)
                                            // |||| |+--- IN ONE-SHOT TIMER MODE:
                                            // |||| |       Pulse Output Function Selected Bit:
                                            // |||| |           0 Pulse is not output
                                            // |||| |           1 Pulse is output
                                            // |||| +---- IN ONE-SHOT TIMER MODE:
                                            // ||||         External Trigger Select Bit
                                            // ||||             0 Falling edge of input signal to TAiIN pin
                                            // ||||             1 Rising edge of input signal to TAiIN pin
                                            // |||+------ IN ONE-SHOT TIMER MODE:
                                            // |||          Trigger Select Bit (TB2 SELCTED through TA4TGL & TA4TGH)
                                            // |||              0 TAiOS bit is enabled
                                            // |||              1 Selected by TAiTGH to TAiTGL bits
                                            // ||+------  IN ONE-SHOT TIMER MODE: : Set to "0"
                                            // ++-------  IN ONE-SHOT TIMER MODE:
                                            //              Count Source Select Bit
                                            //                  00 f1 or f2
                                            //                  01 f8
                                            //                  10 f32
                                            //                  11 fC32
    

    if (numCounts > 65535)
    {
        // Select clock f8 for forward pulse width timer
        ta4mr = 0x56;       // 0101 0110
        #if( TIMER_F1F2_CLOCK == OSZILLATOR_CLOCK )
        numCounts >>= 3;
        #elif( UART_F1F2_CLOCK == (OSZILLATOR_CLOCK/2) )
        numCounts >>= 2;
        #else
        #error invalid definition for UART_F1F2_CLOCK in system.h
        #endif
        //OIML_TIMER_MODE_REGISTER = 0x52; //OIML A1 timer, no pulse 
        
        if (numCounts > 65535)
        {
            // Select clock f32 for forward pulse width timer
            ta4mr = 0x96;   // 1001 0110
            //OIML_TIMER_MODE_REGISTER = 0x92; //OIML A1 timer, no pulse
            numCounts >>= 2;
        }
    }
    
    
    // Set timer to count down mode
    ta4ud = 0;
    
    // Select trigger source for this timer (typically Timer B2)
    SELECT_B2_TO_A4();
    
    // Disable interrupt for forward pulse width timer
    ta4ic = 0;
    
    // Set Port to Output
    LOGIC_OP1_PORT_DR = OUTPUT_PORT;

#ifdef IO_REVERT_PIN8_0
    pofs4 = 1; // high-level
#else
    pofs4 = 0; // low-level
#endif
    
    // Set initial pulse width
    ta4 = (TUSIGN16) numCounts;
    
    // Disable forward pulse width timer
    ta4s = 0;
    
    // reset gapTime after Pulse width change
    gapTime = MIN_GAP_TIME;


#endif
}


//-------------------------------------------------------------------------------------------------
/*!
 \brief  Initialize Timers as Short Pulse output.
 \author Kevin Lu
 \date   2010-12-11
 \param  -
 \return : void
 \warning: none
 \test
 \n by: Paul Li
 \n environment: IAR Embedded Workbench 3.5
 \n result module test: OK
 \n result Lint Level 3: OK
 \bug

*/
//-------------------------------------------------------------------------------------------------
// 

void InitTimers_ShortPulse(void)
{
    InitUpdateTimer(); // Timer TB2
    InitPulseCountTimer(); // Timer TA0
    output_mode = DIGITALOUTPUT_DO1_PULSE;

}




//-------------------------------------------------------------------------------------------------
/*!
 \brief  This function configures the timers to generate a given number of pulses for the
         flow at the corresponding port pin. (Not for Long Pulses)
 \author Georg Horst
 \date   2007-05-04
 \param[in]  numPulses : the number of pulses to be generated during the update interval (100 ms)
 \return : void
 \warning: none
 \n by: Paul Li
 \n environment: IAR Embedded Workbench 3.5
 \n result module test: OK
 \n result Lint Level 3: OK
 \bug
 
*/
//-------------------------------------------------------------------------------------------------
void SetDigitalOutputPulses(TUSIGN16 numPulses)
{
#if 0
    TFLOAT nextGapTime = MIN_GAP_TIME;
    TUSIGN16 numPulses_ = numPulses;
    // Stop frequency and pulse count timers
    tb2s = 0;
    ta0s = 0;

    if (numPulses_)
    {
        TUSIGN32 numCounts, gapCounts;
        TFLOAT minPulseOutTime = numPulses_ * (2.0f * gPulseOutput.pulseWidth);
        TFLOAT pulseWindow = UPDATE_INTERVAL;

        if(digitalOutputDynamic.onecycle > 0)//have at least one cycle been pasted
        {
            pulseWindow = UPDATE_INTERVAL - gapTime;//the time leave for output. 
        }
        else
        {
            pulseWindow = minPulseOutTime;
        }
        
        if (minPulseOutTime > pulseWindow)
        {
            // The next gap time is the previous gap time plus the difference of the time
            // it takes to output the given number of pulses at the selected pulse width
            // and the update interval.
            pulseWindow = minPulseOutTime;
            //could not output all pulses, then should set alarm to diagnosis
            // digitalOutputDynamic.DO_alarmState = DIGITALOUTPUT_ALARM_BEHAVIOUR_FAIL_LOGIC_OUT_1;
        }

        nextGapTime = pulseWindow + gapTime;
        gapTime = (2.0f * gPulseOutput.pulseWidth);//should have the last pulse and min gap outpout time 
        numCounts = (TUSIGN32) (TIMER_F1F2_CLOCK *
        pulseWindow * SECONDS_PER_MILLISECOND) / numPulses_;
        
        gapCounts = (TUSIGN32) (TIMER_F1F2_CLOCK * gapTime * SECONDS_PER_MILLISECOND);
        
        // Select clock f1 for frequency timer
        tb2mr = 0x00;

        // Select appropiate frequency such that numCounts and gapCounts fit into 16 bit
        if ((numCounts > 65535) || (gapCounts > 65535))
        {
            // Select clock f8 for frequency timer
            tb2mr = 0x40;
            #if( TIMER_F1F2_CLOCK == OSZILLATOR_CLOCK )
            numCounts >>= 3;
            gapCounts >>= 3;
            #elif( UART_F1F2_CLOCK == (OSZILLATOR_CLOCK/2) )
            numCounts >>= 2;
            gapCounts >>= 2;
            #else
            #error invalid definition for UART_F1F2_CLOCK in system.h
            #endif
            
            if ((numCounts > 65535) || (gapCounts > 65535))
            {
                // Select clock f32 for frequency timer
                tb2mr = 0x80;
                numCounts >>= 2;
                gapCounts >>= 2;
            }
        }

        // Set number of pulses for pulse count timer (counter triggers at underflow)
        // TA0
        ta0 = numPulses - 1;
        // Set initial counter value for frequency timer TB2
        if(gapCounts > 65535)
        {
          gapCounts = 65535;
        }
        if(numCounts > 65535)
        {
          numCounts = 65535;
        }
        tb2 = (TUSIGN16) gapCounts;
        // Enable pulse width timer
           ta4s = 1; //TA4

        // Start both timers (frequency  and pulse count)
        ta0s = 1; //TA0
        tb2s = 1; //TB2
        ta0ic = 0x07;
        tb2 = (TUSIGN16) numCounts;
                // Set reload counter value for frequency timer
    }

    gapTime = nextGapTime;
#endif
}

//-------------------------------------------------------------------------------------------------
/*!
 \brief  This function configures the timers to generate one long pulse for
         the flow at the corresponding port pin.
 \author Georg Horst
 \date   2007-05-04
 \param  -
 \return : void
 \warning: none
 \test
 \n by: Paul Li
 \n environment: IAR Embedded Workbench 3.5
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//-------------------------------------------------------------------------------------------------
void SetDigitalOutputLongPulse(void)
{
#if 0
    TFLOAT minPulseOutTime = 2.0f * gPulseOutput.pulseWidth;
    TFLOAT nextGapTime = gapTime +minPulseOutTime;
    gapTime = nextGapTime;

    // Stop frequency and pulse count timers
    tb2s = 0;
    ta4s = 0; //TA4
    ta4 = gPulseOutput.pulseWidthCountTarget - 1;
    
    // Set counter value for frequency timer // TB2
    tb2 = gPulseOutput.pulseWidthFreqCount;
    // Start both timers (frequency  and pulse count)
    //ta0s = 1; // TABSR_bit.TA0S
    ta4s = 1; // TABSR_bit.TA4S
    tb2s = 1; // TABSR_bit.TB2S

#endif
}

//-------------------------------------------------------------------------------------------------
/*!
 \brief  Calculate number of pulses and update roll over and addition counters
 \author  Paul Li
 \date   2011-09-04
 \param  -
 \return numPulses
 \warning 
 \test
 \n by: Paul Li
 \n environment: IAR Embedded Workbench 3.5
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//-------------------------------------------------------------------------------------------------
TUSIGN16 GetFwdPulses(void)
{
    TUSIGN32 newPulseCountLow,newPulseCountHigh;
    TUSIGN16 numPulses;

    //add 64 bit addition counter
    newPulseCountLow = gPulseOutput.pulseFwdCountLow;
    newPulseCountLow += gPulseOutput.pulseAddCountLow;
    newPulseCountHigh = gPulseOutput.pulseFwdCountHigh;
    newPulseCountHigh += gPulseOutput.pulseAddCountHigh;
    // add carry to the upper 32 bits if overflow occurred
    if (newPulseCountLow < gPulseOutput.pulseFwdCountLow)
    {
        newPulseCountHigh++;
    }

    // The number of pulses to be generated is the difference of the upper 32 bits
    numPulses = (TUSIGN16)(newPulseCountHigh - gPulseOutput.pulseFwdCountHigh);
    // Correct pulse count in case some extra pulses were generated due to interrupt delay
    
    if (numPulses > ExtraPulses)
    {
        numPulses -= ExtraPulses;
        ExtraPulses = 0;
    }

    /*
    if the pulse num is too big that it will cost MAX_PULSE_TIME ms to send all the pulse, then the alarm should be generated.
    and the pulse out is stopped.
    else it will output all the pulses. 
    todo:
        how to clear the alarm? 
    because the DO alarm is only one, then the alarm is cleared if the user change the pulse width and factor to the right ones, then it will be cleared. 
    */
    if((MAX_PULSE_TIME /(2*gPulseOutput.pulseWidth) )< numPulses)
    {
        numPulses = 0;
        digitalOutputDynamic.alarmState_DO = DIGITALOUTPUT_ALARM_BEHAVIOUR_FAIL_LOGIC_OUT_1;
    }
    
    if((LONGPULSE == gPulseOutput.pulseType) && (numPulses >1))
    {
        gPulseOutput.pulseFwdCountLow = newPulseCountLow;
        gPulseOutput.pulseFwdCountHigh = newPulseCountHigh-(numPulses-1);
        gPulseOutput.pulseAddCountLow = 0;
        gPulseOutput.pulseAddCountHigh = (numPulses-1);
    }
    else
    {
        // update the roll over counter and addition counter
        gPulseOutput.pulseFwdCountLow = newPulseCountLow;
        gPulseOutput.pulseFwdCountHigh = newPulseCountHigh;
        gPulseOutput.pulseAddCountLow = 0;
        gPulseOutput.pulseAddCountHigh = 0;
    }
        // in case the pulse number tot too high.
    if(gPulseOutput.pulseFwdCountHigh > (PULSE_OUTPUT_COUNT_LIMIT/2))
    {
        gPulseOutput.pulseFwdCountHigh = 0;
    }
    
    digitalOutputDynamic.pulseNum = gPulseOutput.pulseFwdCountHigh;
    return numPulses;
}


//-------------------------------------------------------------------------------------------------
/*!
 \brief  Calculate Pulsen counters
 \author  Paul Li
 \date   2011-09-04
 \param  -
 \return numPulses
 \warning 
 \test
 \n by: Paul Li
 \n environment: IAR Embedded Workbench 3.5
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//-------------------------------------------------------------------------------------------------
void CalculatePulsecount(TFLOAT deviceValue)
{  
    TFLOAT   connectedupdatevalue = 0.0f;     
    TDOUBLE  pulseOutputValue = (TDOUBLE) ZERO_PULSE_OUTPUT;     // intermediate result
    TDOUBLE  floatPulseOutputCount = (TDOUBLE) ZERO_PULSE_OUTPUT;        // final result (TFLOAT)
    TUSIGN32 unsignedPulseOutputCountLow = ZERO_PULSE_OUTPUT;   // final result (unsigned)
    TUSIGN32 unsignedPulseOutputCountHigh = ZERO_PULSE_OUTPUT;  // final result (unsigned)
    TBOOL convert;
    TINT16 SimuNum = 0;
    TFLOAT maxdat = digitalOutputDynamic.gMaxData;
    if(maxdat < 0.0f)
    {
        maxdat = -maxdat;
    }

    connectedupdatevalue = deviceValue;
    convert = unitsOfMeasure.ConvertFromBaseUnit(digitalOutputDynamic.unitType,digitalOutputDynamic.unitCode, &connectedupdatevalue); //change base unit To the internal unit.

    if(convert)
    {
    }
    else
    {
        connectedupdatevalue=0.0f;
    }

    if((connectedupdatevalue > PULSE_RANGE_PERCENT * maxdat) || (connectedupdatevalue < -(PULSE_RANGE_PERCENT * maxdat)))
    {
        connectedupdatevalue = maxdat;
    }

    // p/s = tot/s * p/tot
    pulseOutputValue = (connectedupdatevalue * plantData.pulseFactor.value)/(digitalOutputDynamic.timebaseforcalc);

    if(digitalOutputDynamic.simuEnabled == DIGITALOUTPUT_SIMU_ENABLED)
    {
        SuspendAll_RTOS_TASK();
        SimuNum = digitalOutputDynamic.simuPulsesNum;
        digitalOutputDynamic.simuPulsesNum = 0;
        ResumeAll_RTOS_TASK();
        
        if(SimuNum!=0)
        {
            gPulseOutput.pulseAddCountHigh += (TUSIGN16)SimuNum;
        }
       
        return;  
    }
    // Continue if we were initialized prior to calling this function
    else if (pulseOutputValue)       // if there's something to calculate...
    {
        if(!digitalOutputDynamic.do1Available)
        {
            InitDigitalOutputTimers();
            return ;
        }
        // PROPORTION_OF_MAX_FREQUENCY is the pre-calculated ratio of:
        //   (2^32 -1) x (100e^-3), the latter being the callback interval:
        if (pulseOutputValue >= 0)
        {
            floatPulseOutputCount = (TDOUBLE)(digitalOutputDynamic.elapsedTime)* (PULSE_OUTPUT_COUNT_LIMIT+1.0) * SECONDS_PER_MILLISECOND * pulseOutputValue;
        }
        else // this is where the count is made +ve value only...
        {
            floatPulseOutputCount = (TDOUBLE)(digitalOutputDynamic.elapsedTime) *(PULSE_OUTPUT_COUNT_LIMIT+1.0) * SECONDS_PER_MILLISECOND * pulseOutputValue* -1;
        }
        
        // calculate low and high DWORD of the 64 bit pulse addition count
        //(..+0,5 to get sure that the value should always rounded up)
        unsignedPulseOutputCountHigh = (TUSIGN32)
                ((floatPulseOutputCount + 0.5f) / (PULSE_OUTPUT_COUNT_LIMIT + 1.0f));
        
        unsignedPulseOutputCountLow = (TUSIGN32) ((floatPulseOutputCount + 0.5f) -
                 (unsignedPulseOutputCountHigh * (PULSE_OUTPUT_COUNT_LIMIT + 1.0f)));
        
        // ...write the pulse addition count,
        // Pulse Simualtion only at Digital Output 1 posssible
        unsignedPulseOutputCountLow += gPulseOutput.pulseAddCountLow;
        unsignedPulseOutputCountHigh += gPulseOutput.pulseAddCountHigh;
        if(unsignedPulseOutputCountLow< gPulseOutput.pulseAddCountLow)
        {
            unsignedPulseOutputCountHigh += 1;
        }
        
        gPulseOutput.pulseAddCountLow = unsignedPulseOutputCountLow;
        gPulseOutput.pulseAddCountHigh = unsignedPulseOutputCountHigh;
        digitalOutputDynamic.oldelapsedTime +=(digitalOutputDynamic.elapsedTime);

    }// no else
    else{}
}



//-------------------------------------------------------------------------------------------------
/*!
 \brief  This function checks if new pulses need to be output for either forward or reverse flow.
          The number of pulses to be generated are calculated from the gPulseOutput structure that
          is filled cyclically by the coordinator.
 \author Georg Horst
 \date   2007-05-04
 \param  -
 \return : none
 \warning: none
 \test
 \n by: Paul Li
 \n environment: IAR Embedded Workbench 3.5
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//-------------------------------------------------------------------------------------------------

void UpdatePulseOutput(void)
{  
    TUSIGN16 numPulses = 0;
    // check if normal or long pulses need to be generated (pulseWidthCountTarget is 0 for normal pulses)
    if((pulseOutputAction == NO_PULSES) && (gPulseOutput.pulseWidthChange == 1))
    {
        (void)SetPulseWidthTimer(plantData.pulseWidth.value);
    }

    if(pulseOutputAction == NO_PULSES)
    {
        gapTime -= digitalOutputDynamic.oldelapsedTime;
        if(gapTime<0)
        {
            gapTime = 0;
        }
        if(digitalOutputDynamic.oldelapsedTime >0)
        {
          digitalOutputDynamic.onecycle = 1;
        }
        digitalOutputDynamic.oldelapsedTime = 0;
    }
    
    if ( gPulseOutput.pulseWidthCountTarget == 0)
    {
        // check if new normal pulses have to be generated and previous pulse output is completed
        if (pulseOutputAction == NO_PULSES)
        {
            if((gPulseOutput.pulseAddCountLow > 0) || (gPulseOutput.pulseAddCountHigh > 0))
            {
                // the current flow direction is forward
                numPulses = GetFwdPulses();
                if (numPulses > 0)
                {
                    if((digitalOutputDynamic.oldOutputMode == DIGITALOUTPUT_DO1_PULSE))
                    {
                        // set flag to indicate that pulse generation is on the way
                        pulseOutputAction = NORMAL_PULSES;
                        SetDigitalOutputPulses(numPulses);
                    }
                }                
            }

        }
    }
    else
    {
        // check if a new long pulse has to be generated and previous pulse output is completed
        if ((pulseOutputAction == NO_PULSES) &&
            ((gPulseOutput.pulseAddCountLow > 0) || (gPulseOutput.pulseAddCountHigh > 0)))
        {
            // the current flow direction is forward
            numPulses = GetFwdPulses();//only be 1 pulse
            
            if (numPulses > 0)
            {
                  if((digitalOutputDynamic.oldOutputMode == DIGITALOUTPUT_DO1_PULSE))
                {
                    // set flag to indicate that pulse generation is on the way
                    pulseOutputAction = LONG_GAP;
                    SetDigitalOutputLongPulse();
                }
            }
        }
    }
}



//-------------------------------------------------------------------------------------------------
/*!
 \brief Update pulse cyclically.
 \author   Paul Li
 \date  2011-09-12
 \param -
 \return errorCode
 \warning
 \test
 \n test-date: 2011-09-15
 \n by: Paul Li
 \n environment: IAR Embedded Workbench 3.5
 \n intention:
 \n result module test: OK
 \n result Lint check: OK
 \bug
*/
//-------------------------------------------------------------------------------------------------
TUSIGN16 UpdatePulse_DIGITALOUTPUT(TFLOAT deviceValue)
{ 
    TUSIGN16 returnCode = (TUSIGN16) eTRUE; 
    
// calculate the number of pulse
    CalculatePulsecount(deviceValue);

    // Now call function to update the pulse output
    UpdatePulseOutput();  //digitaloutput_isr.c
     
    return returnCode;
}