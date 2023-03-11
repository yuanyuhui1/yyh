//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Ethernet
 Module
 Description    Declaration of subsystem attributes
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before Ethernet_attributes.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before Ethernet_attributes.h
#endif

#ifndef __T_UNIT_H__
  #error t_unit.h must be included before Ethernet_attributes.h
#endif

#ifndef __ETHERNET_TYPES_H__
  #error Ethernet_types.h must be included before Ethernet_attributes.h
#endif

//............................................
//!  extend include list if necessary

#ifdef __ETHERNET_ATTRIBUTES_H__
  #error Ethernet_attributes.h included more than once
#endif

#define __ETHERNET_ATTRIBUTES_H__

//-------------------------------------------------------------------------------------------------
//! unit global variables
extern TUSIGN16 ethernetState; // state of the SubSystem
extern T_DATA_SEMAPHORE ethernetDataSemaphore;
extern TUSIGN8 ethernet_ControlFactoryDefaults;

//@SubGen put@ <DATACLASS>
//@SubGen start@<DATACLASS CONSTANT>
//-------------------------------------------------------------------------------------------------
//! definition of constant block
extern const T_ETHERNET_CONSTANT ethernetConstant;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS CONSTANT>


//@SubGen start@<DATACLASS DYNAMIC>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
extern T_ETHERNET_DYNAMIC ethernetDynamic;
extern const T_ETHERNET_DYNAMIC ethernetDynamicInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS DYNAMIC>


//@SubGen start@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>
//-------------------------------------------------------------------------------------------------
//! definition of StaticFrequentStaticDefault
#ifdef WIN32
  #pragma bss_seg(".AAA$ETHERNET_DATA")
    extern const T_ETHERNET_STATIC_FREQUENT_STATIC_DEFAULT ethernetStaticFrequentStaticDefault;
    extern const T_ETHERNET_STATIC_FREQUENT_STATIC_DEFAULT ethernetStaticFrequentStaticDefaultDefault;
  #pragma bss_seg()
#else
  #pragma location="ETHERNET_STATIC_FREQUENT_STATIC_DEFAULT"
    extern const __no_init T_ETHERNET_STATIC_FREQUENT_STATIC_DEFAULT ethernetStaticFrequentStaticDefault;
  #pragma location="ETHERNET_STATIC_FREQUENT_STATIC_DEFAULT_DEFAULT"
    extern const __no_init T_ETHERNET_STATIC_FREQUENT_STATIC_DEFAULT ethernetStaticFrequentStaticDefaultDefault;
#endif
extern const T_ETHERNET_STATIC_FREQUENT_STATIC_DEFAULT ethernetStaticFrequentStaticDefaultInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>



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
extern const T_ETHERNET_TEMPLATE ethernetTemplate;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS CONSTANT>



//@SubGen template start@<DATACLASS DYNAMIC>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
extern T_ETHERNET_TEMPLATE ethernetTemplate;
extern const T_ETHERNET_TEMPLATE ethernetTemplateInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS DYNAMIC>



//@SubGen template start@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>
//-------------------------------------------------------------------------------------------------
//! definition of Template
#ifdef WIN32
  #pragma bss_seg(".AAA$ETHERNET_DATA")
    extern const T_ETHERNET_TEMPLATE ethernetTemplate;
    extern const T_ETHERNET_TEMPLATE ethernetTemplateDefault;
  #pragma bss_seg()
#else
  #pragma location="ETHERNET_TEMPLATE"
    extern const __no_init T_ETHERNET_TEMPLATE ethernetTemplate;
  #pragma location="ETHERNET_TEMPLATE_DEFAULT"
    extern const __no_init T_ETHERNET_TEMPLATE ethernetTemplateDefault;
#endif
extern const T_ETHERNET_TEMPLATE ethernetTemplateInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>


//this template is for all other standard storage classes:

//@SubGen template start@<DATACLASS>
//-------------------------------------------------------------------------------------------------
//! definition of Template
#ifdef WIN32
  #pragma bss_seg(".AAA$ETHERNET_DATA")
    extern const T_ETHERNET_TEMPLATE ethernetTemplate;
  #pragma bss_seg()
#else
  #pragma location="ETHERNET_TEMPLATE"
    extern const __no_init T_ETHERNET_TEMPLATE ethernetTemplate;
#endif
extern const T_ETHERNET_TEMPLATE ethernetTemplateInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS>


*/

