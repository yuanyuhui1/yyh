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
#include "RTOS/Interface/rtos.h"
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
#include "DigitalOutput_execute.h"



extern volatile TUSIGN16 ExtraPulses;


//-------------------------------------------------------------------------------------------------
/*!
 \brief  This function can be used to initialize timer B2mode, contentm and interrupt priority.
 \author Kevin Lu
 \date   2010-12-11
 \param  mode timer mode
 \param  content timer reload register content
 \param  interr interrupt priority
 \return : void
 \warning: none
 \test
 \n by: Kevin Lu
 \n environment: IAR Embedded Workbench 3.5
 \n result module test: OK
 \n result Lint Level 3: OK
 \bug
*/
//-------------------------------------------------------------------------------------------------
void Init_TimerB2(TUSIGN8 mode, TUSIGN16 content, TUSIGN8 interr)
{
#if 0
    // Stop Timer
    TABSR_bit.TB2S = 0;

    // Set Timer mode
    TB2MR = mode;

    // Set timer content.
    TB2 = content;

    // Enable / Disable interrupt and its priority
    tb2ic = interr;
#endif
}


//-------------------------------------------------------------------------------------------------
/*!
 \brief	 This function can be used to initialize timer A4 mode, contentm and interrupt priority.
 \author Kevin Lu
 \date   2010-12-11
 \param  mode timer mode
 \param  content timer reload register content
 \param  interr interrupt priority
 \return : void
 \warning: none
 \test
 \n by: Kevin Lu
 \n environment: IAR Embedded Workbench 3.5
 \n result module test: OK
 \n result Lint Level 3: OK
 \bug
*/
//-------------------------------------------------------------------------------------------------
void Init_TimerA4(TUSIGN8 mode, TUSIGN16 content, TUSIGN8 interr)
{
#if 0
    // Stop Timer
    TABSR_bit.TA4S = 0;

    // Set Timer mode
    TA4MR = mode;

    // Set timer content.
    TA4 = content;

    // Enable / Disable interrupt and its priority
    TA4IC = interr;
#endif
}


//-------------------------------------------------------------------------------------------------
/*!
 \brief	 This function can be used to initialize timer A0 mode, contentm and interrupt priority.
 \author Kevin Lu
 \date   2010-12-11
 \param  mode timer mode
 \param  content timer reload register content
 \param  interr interrupt priority
 \return : void
 \warning: none
 \test
 \n by: Kevin Lu
 \n environment: IAR Embedded Workbench 3.5
 \n result module test: OK
 \n result Lint Level 3: OK
 \bug
*/
//-------------------------------------------------------------------------------------------------
void Init_TimerA0(TUSIGN8 mode, TUSIGN16 content, TUSIGN8 interr)
{
#if 0
    // Stop Timer
    TABSR_bit.TA0S = 0;

    // Set Timer mode
    TA0MR = mode;

    // Set timer content.
    TA0 = content;

    // Enable / Disable interrupt and its priority
    ta0ic = interr;
#endif
}





//-------------------------------------------------------------------------------------------------
/*!
 \brief	 Initialize Timers.
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
void InitDigitalOutputTimers(void)
{
    Init_TimerB2(0x00, 0x00, 0x00);
    Init_TimerA4(0x00, 0x00, 0x00);
    Init_TimerA0(0x00, 0x00, 0x00);
}

//-------------------------------------------------------------------------------------------------
/*!
 \brief	 Interrupt function that handles short pulse. 
 \		when mode is changed, it should adjust timer according to the new output mode.
 \author Paul Li
 \date   2011-09-11
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
//#pragma vector = 21 //TA0
void IRQ_TimerA0(void)
{
#if 0
    TUSIGN16 extraPulses;
    if(output_mode == DIGITALOUTPUT_DO1_PULSE)
    {
        extraPulses = 0xffff - TA0;
        tb2s = 0;	// Stop Timer B2
        ta0s = 0;	// Stop Timer A0
        //ta4s = 0;	// Stop Timer A4
        
        // Read out TA0 register to check if the correct number of pulses have been
        // generated (should contain 0xffff, if the interrupt was triggered in time).
        // Add up extra pulses to compensate by following pulses generation.
        ExtraPulses += extraPulses;
        // Reset output active flag to indicate the timer has elapsed, before calling update
        pulseOutputAction = NO_PULSES;
    
        switch (digitalOutputDynamic.outputMode)
        {
            case DIGITALOUTPUT_DO1_NONE:    
            case DIGITALOUTPUT_DO1_LOGIC:    
                *digitalOutputDynamic.logic_DO1.pAvailable = (TBOOL)eTRUE;
            break;
    
            case DIGITALOUTPUT_DO1_PULSE:  
                UpdatePulseOutput();
            break;
    
            case DIGITALOUTPUT_DO1_FREQUENCY:    
                InitFrequencyOutput();
            break;
            default:
            break;
        }
        digitalOutputDynamic.oldOutputMode = digitalOutputDynamic.outputMode;
    }
    else if(DIGITALOUTPUT_DO1_FREQUENCY ==  output_mode)
    {
        if(gFrequencyOutput.SpaceNow==1)
        {
            gFrequencyOutput.SpaceNow = 0;
            tb2 = gFrequencyOutput.baseFreqCounter; // write i to TB2
            //next interrupt registor
            ta0 = (TUSIGN16)(gFrequencyOutput.NumCounterSpace);
        }
        else
        {
            gFrequencyOutput.SpaceNow = 1;
            tb2 = gFrequencyOutput.baseFreqCounter+1; // write i to TB2
            //next interrupt registor
            ta0 = (TUSIGN16)(gFrequencyOutput.NumCounterMark);
        }	
    }
    else{}
#endif  
}


//-------------------------------------------------------------------------------------------------
/*!
 \brief	 Interrupt function that handles long pulse or frequency output. 
 \		when mode is changed, it should adjust timer according to the new output mode.
 \author Paul Li
 \date   2011-09-11
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
//#pragma vector = 25 //Timer A4
void IRQ_TimerA4(void)
{
#if 0
    switch (digitalOutputDynamic.outputMode)
    {
        case DIGITALOUTPUT_DO1_NONE:	
        case DIGITALOUTPUT_DO1_LOGIC:	
            if(pulseOutputAction == NO_PULSES)
            {
                //	Stop Timer B2 and Timer A4
                Init_TimerB2(0x00, 0x0000, 0x00);
                Init_TimerA4(0x00, 0x0000, 0x00);
                Init_TimerA0(0x00, 0x0000, 0x00);
                digitalOutputDynamic.oldOutputMode = digitalOutputDynamic.outputMode;
            }
        break;
        case DIGITALOUTPUT_DO1_FREQUENCY:	// Freq -> Freq
            UpdateFrequency();
        break;
        case DIGITALOUTPUT_DO1_PULSE:	// PULSE -> PULSE
                    // Stop update and pulse count timer and thus the pulse output
            tb2s = 0; //TB2
            ta4s = 0; 
            if (pulseOutputAction == LONG_GAP)
            {
                // Set number of cycles for pulse count timer // TA4
                ta4 = gPulseOutput.pulseWidthCountTarget - 1;
                
                // Set counter value for frequency timer // TB2
                tb2 = gPulseOutput.pulseWidthFreqCount;
        
                // Set port pin to high for forward flow
#ifdef IO_REVERT_PIN8_0
                    LOGIC_OP1_PORT    = OUTPUT_LOW;     //     and O/P set low initially
#else
                    LOGIC_OP1_PORT    = OUTPUT_HIGH;     //     and O/P set low initially
#endif	

                pulseOutputAction = LONG_PULSE;
                
                // Restart the update and pulse count timers
                tb2s = 1; // TABSR_bit.TB2S
                ta4s = 1; // TABSR_bit.TA4S
            }
            else if(pulseOutputAction == LONG_PULSE)
            {
                // Set port pin to low for forward flow
#ifdef IO_REVERT_PIN8_0
                LOGIC_OP1_PORT    = OUTPUT_HIGH;     //     and O/P set low initially
#else
                LOGIC_OP1_PORT    = OUTPUT_LOW;     //     and O/P set low initially
#endif	
                    // Reset output active flag to indicate the timer has elapsed
                pulseOutputAction = NO_PULSES;
                  UpdatePulseOutput();  //digitaloutput_isr.c
            }	
            else
            {
            }
        break;
        default:
        break;
    }
#endif
}


//-------------------------------------------------------------------------------------------------
/*!
 \brief	 This function is to Init TA1 to begin the just of freq out reg.
 \author Paul Li
 \date   2011-10-08

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
void InitFreqWidthTimer()
{
#if 0
    gFrequencyOutput.SpaceNow = 0; 
    ta4ic = 0;
    tb2 = gFrequencyOutput.baseFreqCounter; // write i to TB2
    ta4 = gFrequencyOutput.NumCounter;// write n to TA4
    
    //---------------
    // TA0 Timer Init
    //---------------
    ta0s = 0; //TA0
    

    ta0mr = 0x01;                           // XXXX XXXX: Setup value 0001 0110
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
     SELECT_B2_TO_A0();  //A0 event triger is B2

    
    // Set timer to count down mode
    ta0ud = 0;
    
    
    ta0 = ( gFrequencyOutput.NumCounterMark );
    ta0s = 1;
    ta0ic = 7;
    
    // Disable forward pulse width timer
    ta0 =  ( gFrequencyOutput.NumCounterSpace);
#endif
}