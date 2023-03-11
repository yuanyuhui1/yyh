//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Digital Output subsystem
 Module         digitaloutput_hardware.h
 Description    Digital Output subsystem timer definitions
 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
#ifndef __DIGITAL_OUTPUT_TIMERS_H__
#define __DIGITAL_OUTPUT_TIMERS_H__

    //***************************************************************************************************
    //                              Definitions for  the PULSE_WIDTH_TIMER
    //***************************************************************************************************
    #define FWD_WIDTH_TIMER_IC                      (TA4IC)           // TA4
    #define FWD_WIDTH_TIMER_START_BIT               (TABSR_bit.TA4S)
    #define FWD_WIDTH_TIMER                         (TA4)
    #define FWD_WIDTH_TIMER_REGISTER                (FWD_WIDTH_TIMER) // used by overload.c
    #define FWD_WIDTH_TIMER_MODE_REGISTER           (TA4MR)
    #define FWD_WIDTH_TIMER_PORT_PIN_STATE          (TA4MR_bit.MR0)   //Port Pin 8.0: TA4 Timer Output State
    #define FWD_WIDTH_TIMER_COUNT_DIRECTION_BIT     (UDF_bit.TA4UD)
    // select timer B2 as trigger source
    #define SELECT_FWD_WIDTH_TIMER_TRIGGER()        TRGSR_bit.TA4TGL = 1; TRGSR_bit.TA4TGH = 0;
  
    //***************************************************************************************************
    //                              Definitions for  the PULSE_COUNT_TIMER
    //***************************************************************************************************
    #define PULSE_COUNT_TIMER_IC                    (TA0IC)           // TA0
    #define PULSE_COUNT_TIMER_START_BIT             (TABSR_bit.TA0S)
    #define PULSE_COUNT_TIMER                       (TA0)
    #define PULSE_COUNT_TIMER_REGISTER              (PULSE_COUNT_TIMER)// used by overload.c
    #define PULSE_COUNT_TIMER_MODE_REGISTER         (TA0MR)
    #define PULSE_COUNT_TIMER_COUNT_DIRECTION_BIT   (UDF_bit.TA0UD)
    // select timer B2 as trigger source
    #define SELECT_PULSE_COUNT_TIMER_TRIGGER()      ONSF_bit.TA0TGL = 1; ONSF_bit.TA0TGH = 0;

    //***************************************************************************************************
    //                              Definitions for  the BASE_TIMER
    //***************************************************************************************************
    #define BASE_TIMER_IC                         (TB2IC)           // TB2
    #define BASE_TIMER_START_BIT                  (TABSR_bit.TB2S)
    #define BASE_TIMER_REGISTER                   (TB2)
    #define BASE_TIMER_MODE_REGISTER              (TB2MR)

    //***************************************************************************************************
    //                              Definitions for  the OIML TIMER
    //***************************************************************************************************
#define OIML_TIMER_IC						(TA1IC) 		  // TA1
#define OIML_TIMER_START_BIT				(TABSR_bit.TA1S)
#define OIML_TIMER 						(TA1)
#define OIML_TIMER_REGISTER				(OIML_TIMER) // used by overload.c
#define OIML_TIMER_MODE_REGISTER			(TA1MR)
#define OIML_TIMER_PORT_PIN_STATE			(TA1MR_bit.MR0)   //Port Pin 7.2: TA1 Timer Output State
#define OIML_TIMER_COUNT_DIRECTION_BIT 	(UDF_bit.TA1UD)
		// select timer B2 as trigger source
#define SELECT_OIML_TIMER_TRIGGER()		TRGSR_bit.TA1TGL = 1; TRGSR_bit.TA1TGH = 0;




     //***************************************************************************************************
    //                              Generell Definitions for PulseCalculation
    //***************************************************************************************************
    // "Long" pulse width threshold, expressed in ms.
    #define PULSE_WIDTH_PRESCALAR_TIME          (85.0)              // NB: 87.38 ms max. for f32
    // 100 ms between update-pulse interrupts, expressed in ms
    #define UPDATE_INTERVAL                     (100.0) 
    #define TIMER_RESTART_TIME                  (0.5)               // estimated time to restart
       
#endif
