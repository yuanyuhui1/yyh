//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2005.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Arm
 Module
 Description    Declaration of subsystem attributes
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before Arm_attributes.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before Arm_attributes.h
#endif

#ifndef __T_UNIT_H__
  #error t_unit.h must be included before Arm_attributes.h
#endif

#ifndef __ARM_TYPES_H__
  #error Arm_types.h must be included before Arm_attributes.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __ARM_ATTRIBUTES_H__
  #error Arm_attributes.h included more than once
#endif

#define __ARM_ATTRIBUTES_H__

//-------------------------------------------------------------------------------------------------
//! unit global variables
extern FAST TUSIGN16 armState; // state of the SubSystem
extern FAST T_DATA_SEMAPHORE armDataSemaphore;

//@SubGen put@ <DATACLASS>
//@SubGen start@<DATACLASS STATIC_FREQUENT_CONSTANT_DEFAULT_PASSWORD>
//-------------------------------------------------------------------------------------------------
//! definition of StaticFrequentConstantDefault_Password
#ifdef WIN32
  #pragma bss_seg(".AAA$ARM_DATA")
  extern const FAST T_ARM_STATIC_FREQUENT_CONSTANT_DEFAULT_PASSWORD armStaticFrequentConstantDefault_Password;
  #pragma bss_seg()
#else
  #pragma location="ARM_STATIC_FREQUENT_CONSTANT_DEFAULT_PASSWORD"
  extern const FAST __no_init T_ARM_STATIC_FREQUENT_CONSTANT_DEFAULT_PASSWORD armStaticFrequentConstantDefault_Password;
#endif
extern const SLOW T_ARM_STATIC_FREQUENT_CONSTANT_DEFAULT_PASSWORD armStaticFrequentConstantDefault_PasswordInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS STATIC_FREQUENT_CONSTANT_DEFAULT_PASSWORD>



//@SubGen start@<DATACLASS STATIC_FREQUENT_CONSTANT_DEFAULT>
//-------------------------------------------------------------------------------------------------
//! definition of StaticFrequentConstantDefault
#ifdef WIN32
  #pragma bss_seg(".AAA$ARM_DATA")
  extern const FAST T_ARM_STATIC_FREQUENT_CONSTANT_DEFAULT armStaticFrequentConstantDefault;
  #pragma bss_seg()
#else
  #pragma location="ARM_STATIC_FREQUENT_CONSTANT_DEFAULT"
  extern const FAST __no_init T_ARM_STATIC_FREQUENT_CONSTANT_DEFAULT armStaticFrequentConstantDefault;
#endif
extern const SLOW T_ARM_STATIC_FREQUENT_CONSTANT_DEFAULT armStaticFrequentConstantDefaultInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS STATIC_FREQUENT_CONSTANT_DEFAULT>



//@SubGen start@<DATACLASS STATIC_FREQUENT_CONSTANT_DEFAULT_2>
//-------------------------------------------------------------------------------------------------
//! definition of StaticFrequentConstantDefault_2
#ifdef WIN32
  #pragma bss_seg(".AAA$ARM_DATA")
  extern const FAST T_ARM_STATIC_FREQUENT_CONSTANT_DEFAULT_2 armStaticFrequentConstantDefault_2;
  #pragma bss_seg()
#else
  #pragma location="ARM_STATIC_FREQUENT_CONSTANT_DEFAULT_2"
  extern const FAST __no_init T_ARM_STATIC_FREQUENT_CONSTANT_DEFAULT_2 armStaticFrequentConstantDefault_2;
#endif
extern const SLOW T_ARM_STATIC_FREQUENT_CONSTANT_DEFAULT_2 armStaticFrequentConstantDefault_2Initializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS STATIC_FREQUENT_CONSTANT_DEFAULT_2>



//@SubGen start@<DATACLASS DYNAMIC>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
extern FAST T_ARM_DYNAMIC armDynamic;
extern const SLOW T_ARM_DYNAMIC armDynamicInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS DYNAMIC>




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
extern const SLOW T_ARM_TEMPLATE armTemplate;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS CONSTANT>



//@SubGen template start@<DATACLASS DYNAMIC>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
extern FAST T_ARM_TEMPLATE armTemplate;
extern const SLOW T_ARM_TEMPLATE armTemplateInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS DYNAMIC>



//@SubGen template start@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>
//-------------------------------------------------------------------------------------------------
//! definition of Template
#ifdef WIN32
  #pragma bss_seg(".AAA$ARM_DATA")
    extern const FAST T_ARM_TEMPLATE armTemplate;
    extern const FAST T_ARM_TEMPLATE armTemplateDefault;
  #pragma bss_seg()
#else
  #pragma location="ARM_TEMPLATE"
    extern const FAST __no_init T_ARM_TEMPLATE armTemplate;
  #pragma location="ARM_TEMPLATE_DEFAULT"
    extern const FAST __no_init T_ARM_TEMPLATE armTemplateDefault;
#endif
extern const SLOW T_ARM_TEMPLATE armTemplateInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>


//this template is for all other standard storage classes:

//@SubGen template start@<DATACLASS>
//-------------------------------------------------------------------------------------------------
//! definition of Template
#ifdef WIN32
  #pragma bss_seg(".AAA$ARM_DATA")
  extern const FAST T_ARM_TEMPLATE armTemplate;
  #pragma bss_seg()
#else
  #pragma location="ARM_TEMPLATE"
  extern const FAST __no_init T_ARM_TEMPLATE armTemplate;
#endif
extern const SLOW T_ARM_TEMPLATE armTemplateInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS>


*/
