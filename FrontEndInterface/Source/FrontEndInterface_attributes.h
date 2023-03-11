//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem FrontEndInterface
 Module
 Description    Declaration of subsystem attributes
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before FrontEndInterface_attributes.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before FrontEndInterface_attributes.h
#endif

#ifndef __T_UNIT_H__
  #error t_unit.h must be included before FrontEndInterface_attributes.h
#endif

#ifndef __FRONT_END_INTERFACE_TYPES_H__
  #error FrontEndInterface_types.h must be included before FrontEndInterface_attributes.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __FRONT_END_INTERFACE_ATTRIBUTES_H__
  #error FrontEndInterface_attributes.h included more than once
#endif

#define __FRONT_END_INTERFACE_ATTRIBUTES_H__

//-------------------------------------------------------------------------------------------------
//! unit global variables
extern FAST TUSIGN16 frontEndInterfaceState; // state of the SubSystem
extern FAST T_DATA_SEMAPHORE frontEndInterfaceDataSemaphore;


//@SubGen start@<DATACLASS DYNAMIC_DUPLICATED>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
extern FAST T_FRONT_END_INTERFACE_DYNAMIC_DUPLICATED frontEndInterfaceDynamicDuplicated;
extern const SLOW T_FRONT_END_INTERFACE_DYNAMIC_DUPLICATED frontEndInterfaceDynamicDuplicatedInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS DYNAMIC_DUPLICATED>









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
extern const SLOW T_FRONT_END_INTERFACE_TEMPLATE frontEndInterfaceTemplate;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS CONSTANT>



//@SubGen template start@<DATACLASS DYNAMIC>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
extern FAST T_FRONT_END_INTERFACE_TEMPLATE frontEndInterfaceTemplate;
extern const SLOW T_FRONT_END_INTERFACE_TEMPLATE frontEndInterfaceTemplateInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS DYNAMIC>



//@SubGen template start@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>
//-------------------------------------------------------------------------------------------------
//! definition of Template
#ifdef WIN32
  #pragma bss_seg(".AAA$FRONT_END_INTERFACE_DATA")
    extern const FAST T_FRONT_END_INTERFACE_TEMPLATE frontEndInterfaceTemplate;
    extern const FAST T_FRONT_END_INTERFACE_TEMPLATE frontEndInterfaceTemplateDefault;
  #pragma bss_seg()
#else
  #pragma location="FRONT_END_INTERFACE_TEMPLATE" 
    extern const FAST __no_init T_FRONT_END_INTERFACE_TEMPLATE frontEndInterfaceTemplate;
  #pragma location="FRONT_END_INTERFACE_TEMPLATE_DEFAULT" 
    extern const FAST __no_init T_FRONT_END_INTERFACE_TEMPLATE frontEndInterfaceTemplateDefault;
#endif
extern const SLOW T_FRONT_END_INTERFACE_TEMPLATE frontEndInterfaceTemplateInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>


//this template is for all other standard storage classes:

//@SubGen template start@<DATACLASS>
//-------------------------------------------------------------------------------------------------
//! definition of Template
#ifdef WIN32
  #pragma bss_seg(".AAA$FRONT_END_INTERFACE_DATA")
  extern const FAST T_FRONT_END_INTERFACE_TEMPLATE frontEndInterfaceTemplate;
  #pragma bss_seg()
#else
  #pragma location="FRONT_END_INTERFACE_TEMPLATE"
  extern const FAST __no_init T_FRONT_END_INTERFACE_TEMPLATE frontEndInterfaceTemplate;
#endif
extern const SLOW T_FRONT_END_INTERFACE_TEMPLATE frontEndInterfaceTemplateInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS>


*/

