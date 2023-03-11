//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Coordinator
 Module
 Description    Declaration of subsystem attributes
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before Coordinator_attributes.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before Coordinator_attributes.h
#endif

#ifndef __T_UNIT_H__
  #error t_unit.h must be included before Coordinator_attributes.h
#endif

#ifndef __COORDINATOR_TYPES_H__
  #error Coordinator_types.h must be included before Coordinator_attributes.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __COORDINATOR_ATTRIBUTES_H__
  #error Coordinator_attributes.h included more than once
#endif

#define __COORDINATOR_ATTRIBUTES_H__

//-------------------------------------------------------------------------------------------------
//! unit global variables
extern FAST TUSIGN16 coordinatorState; // state of the SubSystem
extern FAST T_DATA_SEMAPHORE coordinatorDataSemaphore;
//@SubGen put@ <DATACLASS>
//@SubGen start@<DATACLASS FE_OUTPUT_FILTER_PARA>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
extern FAST T_COORDINATOR_FE_OUTPUT_FILTER_PARA coordinatorFE_OUTPUT_FILTER_PARA;
extern const SLOW T_COORDINATOR_FE_OUTPUT_FILTER_PARA coordinatorFE_OUTPUT_FILTER_PARAInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS FE_OUTPUT_FILTER_PARA>



//@SubGen start@<DATACLASS REPLACE_STATIC_FREQ_STATIC_DEFAULT>
//-------------------------------------------------------------------------------------------------
//! definition of ReplaceStaticFreqStaticDefault
#ifdef WIN32
  #pragma bss_seg(".AAA$COORDINATOR_DATA")
    extern const FAST T_COORDINATOR_REPLACE_STATIC_FREQ_STATIC_DEFAULT coordinatorReplaceStaticFreqStaticDefault;
    extern const FAST T_COORDINATOR_REPLACE_STATIC_FREQ_STATIC_DEFAULT coordinatorReplaceStaticFreqStaticDefaultDefault;
  #pragma bss_seg()
#else
  #pragma location="COORDINATOR_REPLACE_STATIC_FREQ_STATIC_DEFAULT" 
    extern const FAST __no_init T_COORDINATOR_REPLACE_STATIC_FREQ_STATIC_DEFAULT coordinatorReplaceStaticFreqStaticDefault;
  #pragma location="COORDINATOR_REPLACE_STATIC_FREQ_STATIC_DEFAULT_DEFAULT"
    extern const FAST __no_init T_COORDINATOR_REPLACE_STATIC_FREQ_STATIC_DEFAULT coordinatorReplaceStaticFreqStaticDefaultDefault;
#endif
extern const SLOW T_COORDINATOR_REPLACE_STATIC_FREQ_STATIC_DEFAULT coordinatorReplaceStaticFreqStaticDefaultInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS REPLACE_STATIC_FREQ_STATIC_DEFAULT>



//@SubGen start@<DATACLASS REPLACE_STATIC_RARE_CALIBRATION>
//-------------------------------------------------------------------------------------------------
//! definition of ReplaceStaticRare_Calibration
#ifdef WIN32
  #pragma bss_seg(".AAA$COORDINATOR_DATA")
  extern const FAST T_COORDINATOR_REPLACE_STATIC_RARE_CALIBRATION coordinatorReplaceStaticRare_Calibration;
  #pragma bss_seg()
#else
  #pragma location="COORDINATOR_REPLACE_STATIC_RARE_CALIBRATION"
  extern const FAST __no_init T_COORDINATOR_REPLACE_STATIC_RARE_CALIBRATION coordinatorReplaceStaticRare_Calibration;
#endif
extern const SLOW T_COORDINATOR_REPLACE_STATIC_RARE_CALIBRATION coordinatorReplaceStaticRare_CalibrationInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS REPLACE_STATIC_RARE_CALIBRATION>



//@SubGen start@<DATACLASS STATIC_FREQUENT_CONSTANT_DEFAULT>
//-------------------------------------------------------------------------------------------------
//! definition of StaticFrequentConstantDefault
#ifdef WIN32
  #pragma bss_seg(".AAA$COORDINATOR_DATA")
  extern const FAST T_COORDINATOR_STATIC_FREQUENT_CONSTANT_DEFAULT coordinatorStaticFrequentConstantDefault;
  #pragma bss_seg()
#else
  #pragma location="COORDINATOR_STATIC_FREQUENT_CONSTANT_DEFAULT"
  extern const FAST __no_init T_COORDINATOR_STATIC_FREQUENT_CONSTANT_DEFAULT coordinatorStaticFrequentConstantDefault;
#endif
extern const SLOW T_COORDINATOR_STATIC_FREQUENT_CONSTANT_DEFAULT coordinatorStaticFrequentConstantDefaultInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS STATIC_FREQUENT_CONSTANT_DEFAULT>



//@SubGen start@<DATACLASS UNPROTECTED>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
extern FAST T_COORDINATOR_UNPROTECTED coordinatorUnprotected;
extern const SLOW T_COORDINATOR_UNPROTECTED coordinatorUnprotectedInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS UNPROTECTED>



//@SubGen start@<DATACLASS REPLACE_STATIC_RARE>
//-------------------------------------------------------------------------------------------------
//! definition of ReplaceStaticRare
#ifdef WIN32
  #pragma bss_seg(".AAA$COORDINATOR_DATA")
  extern const FAST T_COORDINATOR_REPLACE_STATIC_RARE coordinatorReplaceStaticRare;
  #pragma bss_seg()
#else
  #pragma location="COORDINATOR_REPLACE_STATIC_RARE"
  extern const FAST __no_init T_COORDINATOR_REPLACE_STATIC_RARE coordinatorReplaceStaticRare;
#endif
extern const SLOW T_COORDINATOR_REPLACE_STATIC_RARE coordinatorReplaceStaticRareInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS REPLACE_STATIC_RARE>


//@SubGen start@<DATACLASS DYNAMIC_DUPLICATED>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
extern FAST T_COORDINATOR_DYNAMIC_DUPLICATED coordinatorDynamicDuplicated;
extern const SLOW T_COORDINATOR_DYNAMIC_DUPLICATED coordinatorDynamicDuplicatedInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS DYNAMIC_DUPLICATED>


//@SubGen start@<DATACLASS PROTECTED>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
extern FAST T_COORDINATOR_PROTECTED coordinatorProtected;
extern const SLOW T_COORDINATOR_PROTECTED coordinatorProtectedInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS PROTECTED>





//@SubGen start@<DATACLASS STATIC_RARE>
//-------------------------------------------------------------------------------------------------
//! definition of StaticRare
#ifdef WIN32
  #pragma bss_seg(".AAA$COORDINATOR_DATA")
  extern const FAST T_COORDINATOR_STATIC_RARE coordinatorStaticRare;
  #pragma bss_seg()
#else
  #pragma location="COORDINATOR_STATIC_RARE"
  extern const FAST __no_init T_COORDINATOR_STATIC_RARE coordinatorStaticRare;
#endif
extern const SLOW T_COORDINATOR_STATIC_RARE coordinatorStaticRareInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS STATIC_RARE>



//@SubGen start@<DATACLASS CONSTANT>
//-------------------------------------------------------------------------------------------------
//! definition of constant block
extern const SLOW T_COORDINATOR_CONSTANT coordinatorConstant;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS CONSTANT>



//@SubGen start@<DATACLASS STATIC_CYCLIC>
//-------------------------------------------------------------------------------------------------
//! definition of StaticCyclic
#ifdef WIN32
  #pragma bss_seg(".AAA$COORDINATOR_DATA")
  extern const FAST T_COORDINATOR_STATIC_CYCLIC coordinatorStaticCyclic;
  #pragma bss_seg()
#else
  #pragma location="COORDINATOR_STATIC_CYCLIC"
  extern const FAST __no_init T_COORDINATOR_STATIC_CYCLIC coordinatorStaticCyclic;
#endif
extern const SLOW T_COORDINATOR_STATIC_CYCLIC coordinatorStaticCyclicInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS STATIC_CYCLIC>




/*

This section defines templates for the SubSystem CodeGenerator.
Do not modify or remove this section

//@SubGen template start@ <NEW DATACLASS>
//@SubGen start@<DATACLASS TEMPLATE>
//@SubGen end@<DATACLASS TEMPLATE>
//@SubGen template end@ <NEW DATACLASS>


//special templates for specific standard storage classes:
//@SubGen template start@<DATACLASS CONSTANT>
//-------------------------------------------------------------------------------------------------
//! definition of constant block
extern const SLOW T_COORDINATOR_TEMPLATE coordinatorTemplate;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS CONSTANT>



//@SubGen template start@<DATACLASS DYNAMIC>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
extern FAST T_COORDINATOR_TEMPLATE coordinatorTemplate;
extern const SLOW T_COORDINATOR_TEMPLATE coordinatorTemplateInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS DYNAMIC>



//@SubGen template start@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>
//-------------------------------------------------------------------------------------------------
//! definition of Template
#ifdef WIN32
  #pragma bss_seg(".AAA$COORDINATOR_DATA")
    extern const FAST T_COORDINATOR_TEMPLATE coordinatorTemplate;
    extern const FAST T_COORDINATOR_TEMPLATE coordinatorTemplateDefault;
  #pragma bss_seg()
#else
  #pragma location="COORDINATOR_TEMPLATE" 
    extern const FAST __no_init T_COORDINATOR_TEMPLATE coordinatorTemplate;
  #pragma location="COORDINATOR_TEMPLATE_DEFAULT" 
    extern const FAST __no_init T_COORDINATOR_TEMPLATE coordinatorTemplateDefault;
#endif
extern const SLOW T_COORDINATOR_TEMPLATE coordinatorTemplateInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>


//this template is for all other standard storage classes:

//@SubGen template start@<DATACLASS>
//-------------------------------------------------------------------------------------------------
//! definition of Template
#ifdef WIN32
  #pragma bss_seg(".AAA$COORDINATOR_DATA")
  extern const FAST T_COORDINATOR_TEMPLATE coordinatorTemplate;
  #pragma bss_seg()
#else
  #pragma location="COORDINATOR_TEMPLATE"
  extern const FAST __no_init T_COORDINATOR_TEMPLATE coordinatorTemplate;
#endif
extern const SLOW T_COORDINATOR_TEMPLATE coordinatorTemplateInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS>


*/

