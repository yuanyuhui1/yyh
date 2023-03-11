//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem DigitalOutput
 Module
 Description    Prototypes of overloaded methods
                for Subsystems based on T_UNIT.
 
 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before DigitalOutput_overload.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before DigitalOutput_overload.h
#endif

#ifndef __T_UNIT_H__
  #error  t_unit.h must be included before DigitalOutput_overload.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __DIGITAL_OUTPUT_OVERLOAD_H__
  #error DigitalOutput_overload.h included more than once
#endif

#define __DIGITAL_OUTPUT_OVERLOAD_H__
//-------------------------------------------------------------------------------------------------
// Overloaded methods for this subsystem
// Note: add these to DIGITAL_OUTPUT-Class in DigitalOutput_constructor.c 
extern TUSIGN16 Initialize_DIGITAL_OUTPUT(const T_UNIT SLOW*me, TUSIGN8 typeOfStartUp);
extern TUSIGN16 Put_DIGITAL_OUTPUT(const T_UNIT SLOW*me,
                           TUSIGN16 objectIndex,
                           TINT16 attributeIndex,
                           void FAST* ptrValue);
extern void pulseFactorChange(void);
