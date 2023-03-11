//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Mapper
 Module
 Description    Declaration of subsystem attributes
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before Mapper_attributes.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before Mapper_attributes.h
#endif

#ifndef __T_UNIT_H__
  #error t_unit.h must be included before Mapper_attributes.h
#endif

#ifndef __MAPPER_TYPES_H__
  #error Mapper_types.h must be included before Mapper_attributes.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __MAPPER_ATTRIBUTES_H__
  #error Mapper_attributes.h included more than once
#endif

#define __MAPPER_ATTRIBUTES_H__

//-------------------------------------------------------------------------------------------------
//! unit global variables
extern FAST TUSIGN16 mapperState; // state of the SubSystem
extern FAST T_DATA_SEMAPHORE mapperDataSemaphore;

//@SubGen put@ <DATACLASS>
//@SubGen start@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>
//-------------------------------------------------------------------------------------------------
//! definition of StaticFrequentStaticDefault
#ifdef WIN32
  #pragma bss_seg(".AAA$MAPPER_DATA")
    extern const FAST T_MAPPER_STATIC_FREQUENT_STATIC_DEFAULT mapperStaticFrequentStaticDefault;
    extern const FAST T_MAPPER_STATIC_FREQUENT_STATIC_DEFAULT mapperStaticFrequentStaticDefaultDefault;
  #pragma bss_seg()
#else
  #pragma location="MAPPER_STATIC_FREQUENT_STATIC_DEFAULT" 
    extern const FAST __no_init T_MAPPER_STATIC_FREQUENT_STATIC_DEFAULT mapperStaticFrequentStaticDefault;
  #pragma location="MAPPER_STATIC_FREQUENT_STATIC_DEFAULT_DEFAULT" 
    extern const FAST __no_init T_MAPPER_STATIC_FREQUENT_STATIC_DEFAULT mapperStaticFrequentStaticDefaultDefault;
#endif
extern const SLOW T_MAPPER_STATIC_FREQUENT_STATIC_DEFAULT mapperStaticFrequentStaticDefaultInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>



//@SubGen start@<DATACLASS PROTECTED>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
extern FAST T_MAPPER_PROTECTED mapperProtected;
extern const SLOW T_MAPPER_PROTECTED mapperProtectedInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS PROTECTED>



//@SubGen start@<DATACLASS UNPROTECTED>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
extern FAST T_MAPPER_UNPROTECTED mapperUnprotected;
extern const SLOW T_MAPPER_UNPROTECTED mapperUnprotectedInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS UNPROTECTED>


//@SubGen start@<DATACLASS DYNAMIC_DUPLICATED>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
extern FAST T_MAPPER_DYNAMIC_DUPLICATED mapperDynamicDuplicated;
extern const SLOW T_MAPPER_DYNAMIC_DUPLICATED mapperDynamicDuplicatedInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS DYNAMIC_DUPLICATED>


//@SubGen start@<DATACLASS CONSTANT>
//-------------------------------------------------------------------------------------------------
//! definition of constant block
extern const SLOW T_MAPPER_CONSTANT mapperConstant;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS CONSTANT>


//@SubGen start@<DATACLASS STATIC_RARE>
//-------------------------------------------------------------------------------------------------
//! definition of StaticRare
#ifdef WIN32
  #pragma bss_seg(".AAA$MAPPER_DATA")
  extern const FAST T_MAPPER_STATIC_RARE mapperStaticRare;
  #pragma bss_seg()
#else
  #pragma location="MAPPER_STATIC_RARE"
  extern const FAST __no_init T_MAPPER_STATIC_RARE mapperStaticRare;
#endif
extern const SLOW T_MAPPER_STATIC_RARE mapperStaticRareInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS STATIC_RARE>


//@SubGen start@<DATACLASS STATIC_FREQUENT_CONSTANT_DEFAULT>
//-------------------------------------------------------------------------------------------------
//! definition of StaticFrequentConstantDefault
#ifdef WIN32
  #pragma bss_seg(".AAA$MAPPER_DATA")
  extern const FAST T_MAPPER_STATIC_FREQUENT_CONSTANT_DEFAULT mapperStaticFrequentConstantDefault;
  #pragma bss_seg()
#else
  #pragma location="MAPPER_STATIC_FREQUENT_CONSTANT_DEFAULT"
  extern const FAST __no_init T_MAPPER_STATIC_FREQUENT_CONSTANT_DEFAULT mapperStaticFrequentConstantDefault;
#endif
extern const SLOW T_MAPPER_STATIC_FREQUENT_CONSTANT_DEFAULT mapperStaticFrequentConstantDefaultInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS STATIC_FREQUENT_CONSTANT_DEFAULT>









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
extern const SLOW T_MAPPER_TEMPLATE mapperTemplate;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS CONSTANT>



//@SubGen template start@<DATACLASS DYNAMIC>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
extern FAST T_MAPPER_TEMPLATE mapperTemplate;
extern const SLOW T_MAPPER_TEMPLATE mapperTemplateInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS DYNAMIC>



//@SubGen template start@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>
//-------------------------------------------------------------------------------------------------
//! definition of Template
#ifdef WIN32
  #pragma bss_seg(".AAA$MAPPER_DATA")
    extern const FAST T_MAPPER_TEMPLATE mapperTemplate;
    extern const FAST T_MAPPER_TEMPLATE mapperTemplateDefault;
  #pragma bss_seg()
#else
  #pragma location="MAPPER_TEMPLATE" 
    extern const FAST __no_init T_MAPPER_TEMPLATE mapperTemplate;
  #pragma location="MAPPER_TEMPLATE_DEFAULT" 
    extern const FAST __no_init T_MAPPER_TEMPLATE mapperTemplateDefault;
#endif
extern const SLOW T_MAPPER_TEMPLATE mapperTemplateInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>


//this template is for all other standard storage classes:

//@SubGen template start@<DATACLASS>
//-------------------------------------------------------------------------------------------------
//! definition of Template
#ifdef WIN32
  #pragma bss_seg(".AAA$MAPPER_DATA")
  extern const FAST T_MAPPER_TEMPLATE mapperTemplate;
  #pragma bss_seg()
#else
  #pragma location="MAPPER_TEMPLATE"
  extern const FAST __no_init T_MAPPER_TEMPLATE mapperTemplate;
#endif
extern const SLOW T_MAPPER_TEMPLATE mapperTemplateInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS>


*/

