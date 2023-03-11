//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem MVMeasurement
 Module
 Description    Declaration of subsystem attributes
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before MVMeasurement_attributes.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before MVMeasurement_attributes.h
#endif

#ifndef __T_UNIT_H__
  #error t_unit.h must be included before MVMeasurement_attributes.h
#endif

#ifndef __MVMEASUREMENT_TYPES_H__
  #error MVMeasurement_types.h must be included before MVMeasurement_attributes.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __MVMEASUREMENT_ATTRIBUTES_H__
  #error MVMeasurement_attributes.h included more than once
#endif

#define __MVMEASUREMENT_ATTRIBUTES_H__

//-------------------------------------------------------------------------------------------------
//! unit global variables
extern FAST TUSIGN16 mVMeasurementState; // state of the SubSystem
extern FAST T_DATA_SEMAPHORE mVMeasurementDataSemaphore;

//@SubGen put@ <DATACLASS>
//@SubGen start@<DATACLASS REPLACE_STATIC_FREQUENT_STATIC_DEFAULT>
//-------------------------------------------------------------------------------------------------
//! definition of REPLACE_STATIC_FREQUENT_STATIC_DEFAULT
#ifdef WIN32
  #pragma bss_seg(".AAA$MVMEASUREMENT_DATA")
    extern const FAST T_MVMEASUREMENT_REPLACE_STATIC_FREQUENT_STATIC_DEFAULT mVMeasurementREPLACE_STATIC_FREQUENT_STATIC_DEFAULT;
    extern const FAST T_MVMEASUREMENT_REPLACE_STATIC_FREQUENT_STATIC_DEFAULT mVMeasurementREPLACE_STATIC_FREQUENT_STATIC_DEFAULTDefault;
  #pragma bss_seg()
#else
  #pragma location="MVMEASUREMENT_REPLACE_STATIC_FREQUENT_STATIC_DEFAULT" 
    extern const FAST __no_init T_MVMEASUREMENT_REPLACE_STATIC_FREQUENT_STATIC_DEFAULT mVMeasurementREPLACE_STATIC_FREQUENT_STATIC_DEFAULT;
  #pragma location="MVMEASUREMENT_REPLACE_STATIC_FREQUENT_STATIC_DEFAULT_DEFAULT"
    extern const FAST __no_init T_MVMEASUREMENT_REPLACE_STATIC_FREQUENT_STATIC_DEFAULT mVMeasurementREPLACE_STATIC_FREQUENT_STATIC_DEFAULTDefault;
#endif
extern const SLOW T_MVMEASUREMENT_REPLACE_STATIC_FREQUENT_STATIC_DEFAULT mVMeasurementREPLACE_STATIC_FREQUENT_STATIC_DEFAULTInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS REPLACE_STATIC_FREQUENT_STATIC_DEFAULT>


//@SubGen start@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>
//-------------------------------------------------------------------------------------------------
//! definition of StaticFrequentStaticDefault
#ifdef WIN32
  #pragma bss_seg(".AAA$MVMEASUREMENT_DATA")
    extern const FAST T_MVMEASUREMENT_STATIC_FREQUENT_STATIC_DEFAULT mVMeasurementStaticFrequentStaticDefault;
    extern const FAST T_MVMEASUREMENT_STATIC_FREQUENT_STATIC_DEFAULT mVMeasurementStaticFrequentStaticDefaultDefault;
  #pragma bss_seg()
#else
  #pragma location="MVMEASUREMENT_STATIC_FREQUENT_STATIC_DEFAULT" 
    extern const FAST __no_init T_MVMEASUREMENT_STATIC_FREQUENT_STATIC_DEFAULT mVMeasurementStaticFrequentStaticDefault;
  #pragma location="MVMEASUREMENT_STATIC_FREQUENT_STATIC_DEFAULT_DEFAULT"
    extern const FAST __no_init T_MVMEASUREMENT_STATIC_FREQUENT_STATIC_DEFAULT mVMeasurementStaticFrequentStaticDefaultDefault;
#endif
extern const SLOW T_MVMEASUREMENT_STATIC_FREQUENT_STATIC_DEFAULT mVMeasurementStaticFrequentStaticDefaultInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>


//@SubGen start@<DATACLASS CONSTANT>
//-------------------------------------------------------------------------------------------------
//! definition of constant block
extern const SLOW T_MVMEASUREMENT_CONSTANT mVMeasurementConstant;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS CONSTANT>



//@SubGen start@<DATACLASS PROTECTED>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
extern FAST T_MVMEASUREMENT_PROTECTED mVMeasurementProtected;
extern const SLOW T_MVMEASUREMENT_PROTECTED mVMeasurementProtectedInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS PROTECTED>



//@SubGen start@<DATACLASS UNPROTECTED>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
extern FAST T_MVMEASUREMENT_UNPROTECTED mVMeasurementUnprotected;
extern const SLOW T_MVMEASUREMENT_UNPROTECTED mVMeasurementUnprotectedInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS UNPROTECTED>


//@SubGen start@<DATACLASS DYNAMIC_DUPLICATED>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
extern FAST T_MVMEASUREMENT_DYNAMIC_DUPLICATED mVMeasurementDynamicDuplicated;
extern const SLOW T_MVMEASUREMENT_DYNAMIC_DUPLICATED mVMeasurementDynamicDuplicatedInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS DYNAMIC_DUPLICATED>


//@SubGen start@<DATACLASS STATIC_RARE>
//-------------------------------------------------------------------------------------------------
//! definition of StaticRare
#ifdef WIN32
  #pragma bss_seg(".AAA$MVMEASUREMENT_DATA")
  extern const FAST T_MVMEASUREMENT_STATIC_RARE mVMeasurementStaticRare;
  #pragma bss_seg()
#else
  #pragma location="MVMEASUREMENT_STATIC_RARE"
  extern const FAST __no_init T_MVMEASUREMENT_STATIC_RARE mVMeasurementStaticRare;
#endif
extern const SLOW T_MVMEASUREMENT_STATIC_RARE mVMeasurementStaticRareInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS STATIC_RARE>









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
extern const SLOW T_MVMEASUREMENT_TEMPLATE mVMeasurementTemplate;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS CONSTANT>



//@SubGen template start@<DATACLASS DYNAMIC>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
extern FAST T_MVMEASUREMENT_TEMPLATE mVMeasurementTemplate;
extern const SLOW T_MVMEASUREMENT_TEMPLATE mVMeasurementTemplateInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS DYNAMIC>



//@SubGen template start@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>
//-------------------------------------------------------------------------------------------------
//! definition of Template
#ifdef WIN32
  #pragma bss_seg(".AAA$MVMEASUREMENT_DATA")
    extern const FAST T_MVMEASUREMENT_TEMPLATE mVMeasurementTemplate;
    extern const FAST T_MVMEASUREMENT_TEMPLATE mVMeasurementTemplateDefault;
  #pragma bss_seg()
#else
  #pragma location="MVMEASUREMENT_TEMPLATE" 
    extern const FAST __no_init T_MVMEASUREMENT_TEMPLATE mVMeasurementTemplate;
  #pragma location="MVMEASUREMENT_TEMPLATE_DEFAULT" 
    extern const FAST __no_init T_MVMEASUREMENT_TEMPLATE mVMeasurementTemplateDefault;
#endif
extern const SLOW T_MVMEASUREMENT_TEMPLATE mVMeasurementTemplateInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>


//this template is for all other standard storage classes:

//@SubGen template start@<DATACLASS>
//-------------------------------------------------------------------------------------------------
//! definition of Template
#ifdef WIN32
  #pragma bss_seg(".AAA$MVMEASUREMENT_DATA")
  extern const FAST T_MVMEASUREMENT_TEMPLATE mVMeasurementTemplate;
  #pragma bss_seg()
#else
  #pragma location="MVMEASUREMENT_TEMPLATE"
  extern const FAST __no_init T_MVMEASUREMENT_TEMPLATE mVMeasurementTemplate;
#endif
extern const SLOW T_MVMEASUREMENT_TEMPLATE mVMeasurementTemplateInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS>


*/