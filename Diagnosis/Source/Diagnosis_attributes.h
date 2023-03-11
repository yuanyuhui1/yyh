//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright      Copyright ABB, 2005.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Subsystem Diagnosis
 Module
 Description    Declaration of subsystem attributes
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
#ifndef __SYSTEM_H__
  #error system.h must be included before Diagnosis_attributes.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before Diagnosis_attributes.h
#endif

#ifndef __T_UNIT_H__
  #error t_unit.h must be included before Diagnosis_attributes.h
#endif

#ifndef __DIAGNOSIS_TYPES_H__
  #error Diagnosis_types.h must be included before Diagnosis_attributes.h
#endif

#ifdef __DIAGNOSIS_ATTRIBUTES_H__
  #error Diagnosis_attributes.h included more than once
#endif

#define __DIAGNOSIS_ATTRIBUTES_H__

//-------------------------------------------------------------------------------------------------
//! unit global variables
extern FAST TUSIGN16 diagnosisState; // state of the SubSystem
extern FAST T_DATA_SEMAPHORE diagnosisDataSemaphore;

//@CodeGen start@ <ATTRIBUTE DATACLASS>
extern FAST T_DIAGNOSIS_DYNAMIC diagnosisDynamic;
extern const SLOW T_DIAGNOSIS_DYNAMIC diagnosisDynamicInitializer;
//@CodeGen end@ <ATTRIBUTE DATACLASS>


#ifdef WIN32
  #pragma bss_seg(".AAA$DIAGNOSIS_DATA")
    extern const FAST T_DIAGNOSIS_STATIC_FREQUENT_CONSTANT_DEFAULT diagnosisStaticFrequentConstantDefault;
  #pragma bss_seg()
#else
  #pragma location="DIAGNOSIS_STATIC_FREQUENT_CONSTANT_DEFAULT"
    extern const FAST __no_init T_DIAGNOSIS_STATIC_FREQUENT_CONSTANT_DEFAULT diagnosisStaticFrequentConstantDefault;
#endif
extern const SLOW T_DIAGNOSIS_STATIC_FREQUENT_CONSTANT_DEFAULT diagnosisStaticFrequentConstantDefaultInitializer;

#ifdef WIN32
  #pragma bss_seg(".AAA$DIAGNOSIS_DATA")
    extern const FAST T_DIAGNOSIS_STATIC_CYCLIC diagnosisStaticCyclic;
  #pragma bss_seg()
#else
  #pragma location="DIAGNOSIS_STATIC_CYCLIC"
    extern const FAST __no_init T_DIAGNOSIS_STATIC_CYCLIC diagnosisStaticCyclic;
#endif
extern const SLOW T_DIAGNOSIS_STATIC_CYCLIC diagnosisStaticCyclicInitializer;

