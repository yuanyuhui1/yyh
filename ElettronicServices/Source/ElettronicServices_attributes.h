//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem ElettronicServices
 Module
 Description    Declaration of subsystem attributes
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before ElettronicServices_attributes.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before ElettronicServices_attributes.h
#endif

#ifndef __T_UNIT_H__
  #error t_unit.h must be included before ElettronicServices_attributes.h
#endif

#ifndef __ELETTRONIC_SERVICES_TYPES_H__
  #error ElettronicServices_types.h must be included before ElettronicServices_attributes.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __ELETTRONIC_SERVICES_ATTRIBUTES_H__
  #error ElettronicServices_attributes.h included more than once
#endif

#define __ELETTRONIC_SERVICES_ATTRIBUTES_H__

//-------------------------------------------------------------------------------------------------
//! unit global variables
extern FAST TUSIGN16 elettronicServicesState; // state of the SubSystem
extern FAST T_DATA_SEMAPHORE elettronicServicesDataSemaphore;

//@SubGen put@ <DATACLASS>
//@SubGen start@<DATACLASS CONFIGURE>
//-------------------------------------------------------------------------------------------------
//! definition of Configure
#ifdef WIN32
  #pragma bss_seg(".AAA$ELETTRONIC_SERVICES_DATA")
    extern const FAST T_ELETTRONIC_SERVICES_CONFIGURE elettronicServicesConfigure;
    extern const FAST T_ELETTRONIC_SERVICES_CONFIGURE elettronicServicesConfigureDefault;
  #pragma bss_seg()
#else
  #pragma location="ELETTRONIC_SERVICES_CONFIGURE" 
    extern const FAST __no_init T_ELETTRONIC_SERVICES_CONFIGURE elettronicServicesConfigure;
  #pragma location="ELETTRONIC_SERVICES_CONFIGURE_DEFAULT" 
    extern const FAST __no_init T_ELETTRONIC_SERVICES_CONFIGURE elettronicServicesConfigureDefault;
#endif
extern const SLOW T_ELETTRONIC_SERVICES_CONFIGURE elettronicServicesConfigureInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS CONFIGURE>



//@SubGen start@<DATACLASS CONSTANT>
//-------------------------------------------------------------------------------------------------
//! definition of constant block
extern const SLOW T_ELETTRONIC_SERVICES_CONSTANT elettronicServicesConstant;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS CONSTANT>



//@SubGen start@<DATACLASS PROTECTED>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
extern FAST T_ELETTRONIC_SERVICES_PROTECTED elettronicServicesProtected;
extern const SLOW T_ELETTRONIC_SERVICES_PROTECTED elettronicServicesProtectedInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS PROTECTED>



//@SubGen start@<DATACLASS REPLACE_STATIC_RARE>
//-------------------------------------------------------------------------------------------------
//! definition of ReplaceStaticRare
#ifdef WIN32
  #pragma bss_seg(".AAA$ELETTRONIC_SERVICES_DATA")
  extern const FAST T_ELETTRONIC_SERVICES_REPLACE_STATIC_RARE elettronicServicesReplaceStaticRare;
  #pragma bss_seg()
#else
  #pragma location="ELETTRONIC_SERVICES_REPLACE_STATIC_RARE"
  extern const FAST __no_init T_ELETTRONIC_SERVICES_REPLACE_STATIC_RARE elettronicServicesReplaceStaticRare;
#endif
extern const SLOW T_ELETTRONIC_SERVICES_REPLACE_STATIC_RARE elettronicServicesReplaceStaticRareInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS REPLACE_STATIC_RARE>



//@SubGen start@<DATACLASS UNPROTECTED>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
extern FAST T_ELETTRONIC_SERVICES_UNPROTECTED elettronicServicesUnprotected;
extern const SLOW T_ELETTRONIC_SERVICES_UNPROTECTED elettronicServicesUnprotectedInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS UNPROTECTED>


//@SubGen start@<DATACLASS DYNAMIC_DUPLICATED>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
extern FAST T_ELETTRONIC_SERVICES_DYNAMIC_DUPLICATED elettronicServicesDynamicDuplicated;
extern const SLOW T_ELETTRONIC_SERVICES_DYNAMIC_DUPLICATED elettronicServicesDynamicDuplicatedInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS DYNAMIC_DUPLICATED>



//@SubGen start@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>
//-------------------------------------------------------------------------------------------------
//! definition of StaticFrequentStaticDefault
#ifdef WIN32
  #pragma bss_seg(".AAA$ELETTRONIC_SERVICES_DATA")
    extern const FAST T_ELETTRONIC_SERVICES_STATIC_FREQUENT_STATIC_DEFAULT elettronicServicesStaticFrequentStaticDefault;
    extern const FAST T_ELETTRONIC_SERVICES_STATIC_FREQUENT_STATIC_DEFAULT elettronicServicesStaticFrequentStaticDefaultDefault;
  #pragma bss_seg()
#else
  #pragma location="ELETTRONIC_SERVICES_STATIC_FREQUENT_STATIC_DEFAULT" 
    extern const FAST __no_init T_ELETTRONIC_SERVICES_STATIC_FREQUENT_STATIC_DEFAULT elettronicServicesStaticFrequentStaticDefault;
  #pragma location="ELETTRONIC_SERVICES_STATIC_FREQUENT_STATIC_DEFAULT_DEFAULT"
    extern const FAST __no_init T_ELETTRONIC_SERVICES_STATIC_FREQUENT_STATIC_DEFAULT elettronicServicesStaticFrequentStaticDefaultDefault;
#endif
extern const SLOW T_ELETTRONIC_SERVICES_STATIC_FREQUENT_STATIC_DEFAULT elettronicServicesStaticFrequentStaticDefaultInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>







//@SubGen start@<DATACLASS REPLACE_STATIC_FREQUENT_NO_DEFAULT>
//-------------------------------------------------------------------------------------------------
//! definition of ReplaceStaticFrequentNoDefault
#ifdef WIN32
  #pragma bss_seg(".AAA$ELETTRONIC_SERVICES_DATA")
  extern const FAST T_ELETTRONIC_SERVICES_REPLACE_STATIC_FREQUENT_NO_DEFAULT elettronicServicesReplaceStaticFrequentNoDefault;
  #pragma bss_seg()
#else
  #pragma location="ELETTRONIC_SERVICES_REPLACE_STATIC_FREQUENT_NO_DEFAULT"
  extern const FAST __no_init T_ELETTRONIC_SERVICES_REPLACE_STATIC_FREQUENT_NO_DEFAULT elettronicServicesReplaceStaticFrequentNoDefault;
#endif
extern const SLOW T_ELETTRONIC_SERVICES_REPLACE_STATIC_FREQUENT_NO_DEFAULT elettronicServicesReplaceStaticFrequentNoDefaultInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS REPLACE_STATIC_FREQUENT_NO_DEFAULT>


//@SubGen start@<DATACLASS STATIC_RARE>
//-------------------------------------------------------------------------------------------------
//! definition of StaticRare
#ifdef WIN32
  #pragma bss_seg(".AAA$ELETTRONIC_SERVICES_DATA")
  extern const FAST T_ELETTRONIC_SERVICES_STATIC_RARE elettronicServicesStaticRare;
  #pragma bss_seg()
#else
  #pragma location="ELETTRONIC_SERVICES_STATIC_RARE"
  extern const FAST __no_init T_ELETTRONIC_SERVICES_STATIC_RARE elettronicServicesStaticRare;
#endif
extern const SLOW T_ELETTRONIC_SERVICES_STATIC_RARE elettronicServicesStaticRareInitializer;
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
extern const SLOW T_ELETTRONIC_SERVICES_TEMPLATE elettronicServicesTemplate;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS CONSTANT>



//@SubGen template start@<DATACLASS DYNAMIC>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
extern FAST T_ELETTRONIC_SERVICES_TEMPLATE elettronicServicesTemplate;
extern const SLOW T_ELETTRONIC_SERVICES_TEMPLATE elettronicServicesTemplateInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS DYNAMIC>



//@SubGen template start@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>
//-------------------------------------------------------------------------------------------------
//! definition of Template
#ifdef WIN32
  #pragma bss_seg(".AAA$ELETTRONIC_SERVICES_DATA")
    extern const FAST T_ELETTRONIC_SERVICES_TEMPLATE elettronicServicesTemplate;
    extern const FAST T_ELETTRONIC_SERVICES_TEMPLATE elettronicServicesTemplateDefault;
  #pragma bss_seg()
#else
  #pragma location="ELETTRONIC_SERVICES_TEMPLATE" 
    extern const FAST __no_init T_ELETTRONIC_SERVICES_TEMPLATE elettronicServicesTemplate;
  #pragma location="ELETTRONIC_SERVICES_TEMPLATE_DEFAULT" 
    extern const FAST __no_init T_ELETTRONIC_SERVICES_TEMPLATE elettronicServicesTemplateDefault;
#endif
extern const SLOW T_ELETTRONIC_SERVICES_TEMPLATE elettronicServicesTemplateInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>


//this template is for all other standard storage classes:

//@SubGen template start@<DATACLASS>
//-------------------------------------------------------------------------------------------------
//! definition of Template
#ifdef WIN32
  #pragma bss_seg(".AAA$ELETTRONIC_SERVICES_DATA")
  extern const FAST T_ELETTRONIC_SERVICES_TEMPLATE elettronicServicesTemplate;
  #pragma bss_seg()
#else
  #pragma location="ELETTRONIC_SERVICES_TEMPLATE"
  extern const FAST __no_init T_ELETTRONIC_SERVICES_TEMPLATE elettronicServicesTemplate;
#endif
extern const SLOW T_ELETTRONIC_SERVICES_TEMPLATE elettronicServicesTemplateInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS>


*/

