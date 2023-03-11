// M Ashford - Suppress MISRA Diagnostics
// MISRA Rule 11
// (Identifiers (internal and external) shall not rely on significance of more than 31
// characters. Furthermore, the compiler/linker shall be checked to ensure that 31 character
// significance and case sensitivity are supported for external identifiers.
#pragma diag_suppress=Pm009

//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Modbus
 Module
 Description    Declaration of subsystem attributes
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before MODBUS_attributes.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before MODBUS_attributes.h
#endif

#ifndef __T_UNIT_H__
  #error t_unit.h must be included before MODBUS_attributes.h
#endif

#ifndef __MODBUS_TYPES_H__
  #error MODBUS_types.h must be included before MODBUS_attributes.h
#endif

//............................................
//!  extend include list if necessary

#ifdef __MODBUS_ATTRIBUTES_H__
  #error MODBUS_attributes.h included more than once
#endif

#define __MODBUS_ATTRIBUTES_H__

//-------------------------------------------------------------------------------------------------
//! unit global variables
extern TUSIGN16 modbusState; // state of the SubSystem
extern T_DATA_SEMAPHORE modbusDataSemaphore;


//@SubGen put@ <DATACLASS>
//@SubGen start@<DATACLASS STATIC_FREQUENT_NO_DEFAULT>
//-------------------------------------------------------------------------------------------------
//! definition of StaticFrequentNoDefault
#ifdef WIN32
  #pragma bss_seg(".AAA$MODBUS_DATA")
    extern const T_MODBUS_STATIC_FREQUENT_NO_DEFAULT modbusStaticFrequentNoDefault;
  #pragma bss_seg()
#else
  #pragma location="MODBUS_STATIC_FREQUENT_NO_DEFAULT"
    extern const __no_init T_MODBUS_STATIC_FREQUENT_NO_DEFAULT modbusStaticFrequentNoDefault;
#endif
extern const T_MODBUS_STATIC_FREQUENT_NO_DEFAULT modbusStaticFrequentNoDefaultInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS STATIC_FREQUENT_NO_DEFAULT>


//@SubGen start@<DATACLASS STATIC_FREQUENT_CONSTANT_DEFAULT>
//-------------------------------------------------------------------------------------------------
//! definition of StaticFrequentConstantDefault
#ifdef WIN32
  #pragma bss_seg(".AAA$MODBUS_DATA")
    extern const T_MODBUS_STATIC_FREQUENT_CONSTANT_DEFAULT modbusStaticFrequentConstantDefault;
  #pragma bss_seg()
#else
  #pragma location="MODBUS_STATIC_FREQUENT_CONSTANT_DEFAULT"
    extern const __no_init T_MODBUS_STATIC_FREQUENT_CONSTANT_DEFAULT modbusStaticFrequentConstantDefault;
#endif
extern const T_MODBUS_STATIC_FREQUENT_CONSTANT_DEFAULT modbusStaticFrequentConstantDefaultInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS STATIC_FREQUENT_CONSTANT_DEFAULT>



//@SubGen start@<DATACLASS CONSTANT>
//-------------------------------------------------------------------------------------------------
//! definition of constant block
extern const T_MODBUS_CONSTANT modbusConstant;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS CONSTANT>








//@SubGen start@<DATACLASS DYNAMIC>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
extern T_MODBUS_DYNAMIC modbusDynamic;
extern const T_MODBUS_DYNAMIC modbusDynamicInitializer;
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
extern const T_MODBUS_TEMPLATE modbusTemplate;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS CONSTANT>



//@SubGen template start@<DATACLASS DYNAMIC>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
extern T_MODBUS_TEMPLATE modbusTemplate;
extern const T_MODBUS_TEMPLATE modbusTemplateInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS DYNAMIC>



//@SubGen template start@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>
//-------------------------------------------------------------------------------------------------
//! definition of Template
#ifdef WIN32
  #pragma bss_seg(".AAA$MODBUS_DATA")
    extern const T_MODBUS_TEMPLATE modbusTemplate;
    extern const T_MODBUS_TEMPLATE modbusTemplateDefault;
  #pragma bss_seg()
#else
  #pragma location="MODBUS_TEMPLATE"
    extern const __no_init T_MODBUS_TEMPLATE modbusTemplate;
  #pragma location="ETHERNET_TEMPLATE_DEFAULT"
    extern const __no_init T_MODBUS_TEMPLATE modbusTemplateDefault;
#endif
extern const T_MODBUS_TEMPLATE modbusTemplateInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>


//this template is for all other standard storage classes:

//@SubGen template start@<DATACLASS>
//-------------------------------------------------------------------------------------------------
//! definition of Template
#ifdef WIN32
  #pragma bss_seg(".AAA$MODBUS_DATA")
    extern const T_MODBUS_TEMPLATE modbusTemplate;
  #pragma bss_seg()
#else
  #pragma location="MODBUS_TEMPLATE"
    extern const __no_init T_MODBUS_TEMPLATE modbusTemplate;
#endif
extern const T_MODBUS_TEMPLATE modbusTemplateInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS>


*/
