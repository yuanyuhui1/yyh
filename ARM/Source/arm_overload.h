//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2005.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Arm
 Module
 Description    Prototypes of overloaded methods
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before Arm_overload.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before Arm_overload.h
#endif

#ifndef __T_UNIT_H__
  #error  t_unit.h must be included before Arm_overload.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __ARM_OVERLOAD_H__
  #error Arm_overload.h included more than once
#endif

#define __ARM_OVERLOAD_H__

//-------------------------------------------------------------------------------------------------
// Overloaded methods for this subsystem
// Note: add these to ARM-Class in Arm_constructor.c
TUSIGN16 Initialize_ARM(const T_UNIT SLOW*me, TUSIGN8 typeOfStartUp);

// Put method overloaded in ARM subsystem because of restrictions to writing certain objects.
TUSIGN16 Put_ARM(const T_UNIT SLOW*me, TUSIGN16 objectIndex,
                 TINT16 attributeIndex, void FAST* ptrValue);

// Get method overloaded in ARM subsystem because of restrictions to reading certain objects.
TUSIGN16 Get_ARM(const T_UNIT SLOW*me,
                    TUSIGN16 objectIndex, TINT16 attributeIndex, void FAST* ptrValue);
