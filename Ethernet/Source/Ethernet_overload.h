//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Ethernet
 Module
 Description    Prototypes of overloaded methods
                for Subsystems based on T_UNIT.
 
 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before Ethernet_overload.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before Ethernet_overload.h
#endif

#ifndef __T_UNIT_H__
  #error  t_unit.h must be included before Ethernet_overload.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __ETHERNET_OVERLOAD_H__
  #error Ethernet_overload.h included more than once
#endif

#define __ETHERNET_OVERLOAD_H__

//-------------------------------------------------------------------------------------------------
// Overloaded methods for this subsystem
// Note: add these to ETHERNET-Class in Ethernet_constructor.c 
TUSIGN16 Initialize_ETHERNET(const T_UNIT SLOW*me, TUSIGN8 typeOfStartUp);
TUSIGN16 Get_ETHERNET(const T_UNIT SLOW*me, TUSIGN16 objectIndex, TINT16 attributeIndex, void FAST* ptrValue);
TUSIGN16 Check_ETHERNET(const T_UNIT SLOW*me, TUSIGN16 objectIndex, TINT16 attributeIndex, void FAST* ptrValue);
TUSIGN16 Put_ETHERNET(const T_UNIT SLOW *me, TUSIGN16 objectIndex, TINT16 attributeIndex, void FAST *ptrValue);
TUSIGN16 GetAttributeDescription_ETHERNET(const struct _T_UNIT SLOW*me,
                                        TUSIGN16 objectIndex,
                                        TUSIGN16 attributeIndex,
                                        T_DO_ATTRIB_RANGE FAST* ptrDescriptor);
TUSIGN16 SaveAsDefault_ETHERNET(const T_UNIT SLOW*me, TINT16 dataClassIndex);
TUSIGN16 ResetToDefault_ETHERNET(const T_UNIT SLOW*me, TINT16 dataClassIndex);
