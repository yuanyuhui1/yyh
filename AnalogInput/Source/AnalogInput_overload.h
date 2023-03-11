//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem AnalogInput
 Module
 Description    Prototypes of overloaded methods
                for Subsystems based on T_UNIT.
 
 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before AnalogInput_overload.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before AnalogInput_overload.h
#endif

#ifndef __T_UNIT_H__
  #error  t_unit.h must be included before AnalogInput_overload.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __ANALOG_INPUT_OVERLOAD_H__
  #error AnalogInput_overload.h included more than once
#endif

#define __ANALOG_INPUT_OVERLOAD_H__

//-------------------------------------------------------------------------------------------------
// Overloaded methods for this subsystem
// Note: add these to ANALOG_INPUT-Class in AnalogInput_constructor.c 
TUSIGN16 Initialize_ANALOG_INPUT(const T_UNIT SLOW*me, TUSIGN8 typeOfStartUp);
TUSIGN16 Put_T_AI(const T_UNIT SLOW*me,TUSIGN16 objectIndex, TINT16 attributeIndex, void FAST* ptrValue);
TUSIGN16 LoadRomDefaults_AI(const T_UNIT SLOW *me, TINT16 dataClassIndex);