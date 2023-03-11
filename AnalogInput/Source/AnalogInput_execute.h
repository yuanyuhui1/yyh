//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem AnalogInput
 Module
 Description    Prototypes of execute methods
                for Subsystems based on T_UNIT

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before AnalogInput_execute.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before AnalogInput_execute.h
#endif

#ifndef __ANALOG_INPUT_H__
  #error  AnalogInput.h must be included before AnalogInput_execute.h
#endif


//............................................
//!  extend include list if neccessary

#ifdef __ANALOG_INPUT_EXECUTE_H__
  #error AnalogInput_execute.h included more than once
#endif

#define __ANALOG_INPUT_EXECUTE_H__

#define ADC_RESOLUTION              0.0000625f    // 2.048f / 2^15 = 0.0000625f
#define SAMPLE_R_VALUE              82            // 82 ohm
#define MAXCUR                      25.0f         // 25mA   2.048V    //0x7fff
#define MINCUR                      0.0f          // 0mA    0V        //0x0
#define HILIMCUR                    20.5f         // meet the NUMOR standard
#define LOLIMCUR                    3.8f          // (3.8~2.5) measure information
#define HIRNGCUR                    20.0f         // 20mA,   1.640V   //0x6680
#define LORNGCUR                    4.0f          // 4mA,    0.328V   //0x1480

#define AI_SET_DIAG(name)           SetDiagnosis_AI(AI_ALM_##name##)
#define AI_CLR_DIAG(name)           ClearDiagnosis_AI(AI_ALM_##name##)

#define AI_OBJECT_LOCAL_DIAGNOSIS    analogInputDynamic.aiDiagnosis


//@SubGen start@<METHODS>
void UpdateAnalogInput_AI(void);
void UpdateDiagnosis_AI(void);
TUSIGN8 IsExternalCutOff_AI(void);
//@SubGen end@<METHODS>