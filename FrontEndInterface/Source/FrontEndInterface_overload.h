//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem FrontEndInterface
 Module
 Description    Prototypes of overloaded methods
                for Subsystems based on T_UNIT.
 
 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before FrontEndInterface_overload.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before FrontEndInterface_overload.h
#endif

#ifndef __T_UNIT_H__
  #error  t_unit.h must be included before FrontEndInterface_overload.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __FRONT_END_INTERFACE_OVERLOAD_H__
  #error FrontEndInterface_overload.h included more than once
#endif

#define __FRONT_END_INTERFACE_OVERLOAD_H__

//-------------------------------------------------------------------------------------------------
// Overloaded methods for this subsystem
// Note: add these to FRONT_END_INTERFACE-Class in FrontEndInterface_constructor.c 
TUSIGN16 Initialize_FRONT_END_INTERFACE(const T_UNIT SLOW*me, TUSIGN8 typeOfStartUp);
TUSIGN16 Put_FRONT_END_INTERFACE(const T_UNIT SLOW *me, TUSIGN16 objIdx, TINT16 attribIdx, void FAST *ptrValue);


