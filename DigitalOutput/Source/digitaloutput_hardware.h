//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.


 System         Digital Output subsystem
 Module         digitaloutput_hardware.h
 Description    Digital Output subsystem hardware definitions

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
#ifndef __DIGITAL_OUTPUT_HARDWARE_H__
#define __DIGITAL_OUTPUT_HARDWARE_H__



// Timer Operation Mode
typedef enum
{
  TIMERMODE,
  EVENTCOUNTERMODE,
  ONESHOTMODE,
  PROGRAMMABLEOUTPUTMODE
} T_TIMER_MODE;


// Hardware definitions predominantly used by asm source files...
#define LOGIC_OP1_PORT_DR           (PD8_bit.PD8_0)  // Logic Output 1 Port Direction Register
#define LOGIC_OP1_PORT              (P8_bit.P8_0)    // Logic Output 1 pin20


// Hardware definitions predominantly used by c source files...
#define INPUT_PORT                      (0)         // general I/P definition for HW initialisation
#define OUTPUT_PORT                     (1)         // general O/P definition for HW initialisation
#define OUTPUT_LOW                      (0)         // general O/P high definition
#define OUTPUT_HIGH                     (1)         // general O/P low definition


//***************************************************************************************************
//                              Definitions for  the PULSE_WIDTH_TIMER
//***************************************************************************************************
#define SELECT_B2_TO_A4()        TRGSR_bit.TA4TGL = 1; TRGSR_bit.TA4TGH = 0;

//***************************************************************************************************
//                              Definitions for  the PULSE_COUNT_TIMER
//***************************************************************************************************
// select timer B2 as trigger source
#define SELECT_B2_TO_A0()      ONSF_bit.TA0TGL = 1; ONSF_bit.TA0TGH = 0;


//***************************************************************************************************
//                              Generell Definitions for PulseCalculation
//***************************************************************************************************
// "Long" pulse width threshold, expressed in ms.
// 100 ms between update-pulse interrupts, expressed in ms
#define TIMER_RESTART_TIME                  (0.5)               // estimated time to restart

#define ONE_TIMER_COUNT_LIMIT 65535 //  2^16 - 1

//void UpdatePulseOutput(void);
void InitFwdPulseWidthTimer(TFLOAT pulseWidth_ms);
extern void InitDigitalOutputTimers(void);
void InitDigitalOutputHardware(void);

extern void Init_TimerB2(TUSIGN8 mode, TUSIGN16 content, TUSIGN8 interr);
extern void Init_TimerA0(TUSIGN8 mode, TUSIGN16 content, TUSIGN8 interr);
extern void Init_TimerA4(TUSIGN8 mode, TUSIGN16 content, TUSIGN8 interr);

extern void InitUpdateTimer(void);
extern void InitTimers_ShortPulse(void);
extern void InitTimers_LongPulse(void);
extern void SetDigitalOutputPulses(TUSIGN16 numPulses);
extern void SetDigitalOutputLongPulse(void);

void InitFreqWidthTimer(void);

#endif