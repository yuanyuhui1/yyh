//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Mapper
 Module
 Description    Prototypes of overloaded methods
                for Subsystems based on T_UNIT.
 
 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before Mapper_overload.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before Mapper_overload.h
#endif

#ifndef __T_UNIT_H__
  #error  t_unit.h must be included before Mapper_overload.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __MAPPER_OVERLOAD_H__
  #error Mapper_overload.h included more than once
#endif

#define __MAPPER_OVERLOAD_H__

#define DIAG_SIMUTYPE_VALUEOFFSET 4

#define MAPPER_NO_SELECTION 255 //NO SELECTION

//-------------------------------------------------------------------------------------------------

// Overloaded methods for this subsystem
// Note: add these to MAPPER-Class in Mapper_constructor.c 
TUSIGN16 Initialize_MAPPER(const T_UNIT SLOW*me, TUSIGN8 typeOfStartUp);
TUSIGN16 Put_T_MAPPER(const T_UNIT SLOW*me,TUSIGN16 objectIndex, TINT16 attributeIndex, void FAST* ptrValue);
TUSIGN16 Get_T_MAPPER(const T_UNIT SLOW*me,TUSIGN16 objectIndex, TINT16 attributeIndex, void FAST* ptrValue);

TUSIGN16 GetAttributeDescription_T_MAPPER(const struct _T_UNIT SLOW*me,
                                        TUSIGN16 objectIndex,
                                        TUSIGN16 attributeIndex,
                                        T_DO_ATTRIB_RANGE FAST* ptrDescriptor);



TUSIGN16 Put_DiagnosisSimulationType(const void FAST* ptrValue);//add for VT5 HMI use

TUSIGN16 GetUnit(const TUSIGN8 label, TINT16 attributeIndex, void FAST* ptrValue);