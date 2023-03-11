//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem DigitalOutput
 Module
 Description    Declaration of subsystem attributes
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before DigitalOutput_attributes.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before DigitalOutput_attributes.h
#endif

#ifndef __T_UNIT_H__
  #error t_unit.h must be included before DigitalOutput_attributes.h
#endif

#ifndef __DIGITAL_OUTPUT_TYPES_H__
  #error DigitalOutput_types.h must be included before DigitalOutput_attributes.h
#endif


//@ToDO include specific files for different projects
//#include "..\..\MVMeasurement\Interface\MeasurementLabel.h"
//#include "..\..\MVMeasurement\Source\MeasureClass.h"
//@End TODO


//............................................
//!  extend include list if neccessary

#ifdef __DIGITAL_OUTPUT_ATTRIBUTES_H__
  #error DigitalOutput_attributes.h included more than once
#endif

#define __DIGITAL_OUTPUT_ATTRIBUTES_H__



//-------------------------------------------------------------------------------------------------
//! unit global variables
extern FAST TUSIGN16 digitalOutputState; // state of the SubSystem
extern FAST T_DATA_SEMAPHORE digitalOutputDataSemaphore;

//@SubGen put@ <DATACLASS>
//@SubGen start@<DATACLASS REPLACE_STATIC_FREQUENT_STATIC_DEFAULT>
//-------------------------------------------------------------------------------------------------
//! definition of REPLACE_STATIC_FREQUENT_STATIC_DEFAULT
#ifdef WIN32
  #pragma bss_seg(".AAA$DIGITAL_OUTPUT_DATA")
    extern const FAST T_DIGITAL_OUTPUT_REPLACE_STATIC_FREQUENT_STATIC_DEFAULT digitalOutputREPLACE_STATIC_FREQUENT_STATIC_DEFAULT;
    extern const FAST T_DIGITAL_OUTPUT_REPLACE_STATIC_FREQUENT_STATIC_DEFAULT digitalOutputREPLACE_STATIC_FREQUENT_STATIC_DEFAULTDefault;
  #pragma bss_seg()
#else
  #pragma location="DIGITAL_OUTPUT_REPLACE_STATIC_FREQUENT_STATIC_DEFAULT"
    extern const FAST __no_init T_DIGITAL_OUTPUT_REPLACE_STATIC_FREQUENT_STATIC_DEFAULT digitalOutputREPLACE_STATIC_FREQUENT_STATIC_DEFAULT;
  #pragma location="DIGITAL_OUTPUT_REPLACE_STATIC_FREQUENT_STATIC_DEFAULT_DEFAULT"
    extern const FAST __no_init T_DIGITAL_OUTPUT_REPLACE_STATIC_FREQUENT_STATIC_DEFAULT digitalOutputREPLACE_STATIC_FREQUENT_STATIC_DEFAULTDefault;
#endif
extern const SLOW T_DIGITAL_OUTPUT_REPLACE_STATIC_FREQUENT_STATIC_DEFAULT digitalOutputREPLACE_STATIC_FREQUENT_STATIC_DEFAULTInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS REPLACE_STATIC_FREQUENT_STATIC_DEFAULT>




//@SubGen start@<DATACLASS STATIC_FREQUENT_CONSTANT_DEFAULT>
//-------------------------------------------------------------------------------------------------
//! definition of StaticFrequentConstantDefault
#ifdef WIN32
  #pragma bss_seg(".AAA$DIGITAL_OUTPUT_DATA")
  extern const FAST T_DIGITAL_OUTPUT_STATIC_FREQUENT_CONSTANT_DEFAULT digitalOutputStaticFrequentConstantDefault;
  #pragma bss_seg()
#else
  #pragma location="DIGITAL_OUTPUT_STATIC_FREQUENT_CONSTANT_DEFAULT"
  extern const FAST __no_init T_DIGITAL_OUTPUT_STATIC_FREQUENT_CONSTANT_DEFAULT digitalOutputStaticFrequentConstantDefault;
#endif
extern const SLOW T_DIGITAL_OUTPUT_STATIC_FREQUENT_CONSTANT_DEFAULT digitalOutputStaticFrequentConstantDefaultInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS STATIC_FREQUENT_CONSTANT_DEFAULT>




//@SubGen start@<DATACLASS DYNAMIC>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
extern FAST T_DIGITAL_OUTPUT_DYNAMIC digitalOutputDynamic;
extern const SLOW T_DIGITAL_OUTPUT_DYNAMIC digitalOutputDynamicInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS DYNAMIC>


#define plantData digitalOutputREPLACE_STATIC_FREQUENT_STATIC_DEFAULT


typedef struct 
{
  TUSIGN8 LinkLabel;
  TUSIGN8 MvmLabelValue;
}DIGI_MAP_TABLE;

#define DO_GROUP_SIZE   0x0D

extern DIGI_MAP_TABLE do_maptables[DO_GROUP_SIZE];
extern DIGI_MAP_TABLE   do_temptable;
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
extern const SLOW T_DIGITAL_OUTPUT_TEMPLATE digitalOutputTemplate;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS CONSTANT>



//@SubGen template start@<DATACLASS DYNAMIC>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
extern FAST T_DIGITAL_OUTPUT_TEMPLATE digitalOutputTemplate;
extern const SLOW T_DIGITAL_OUTPUT_TEMPLATE digitalOutputTemplateInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS DYNAMIC>



//@SubGen template start@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>
//-------------------------------------------------------------------------------------------------
//! definition of Template
#ifdef WIN32
  #pragma bss_seg(".AAA$DIGITAL_OUTPUT_DATA")
    extern const FAST T_DIGITAL_OUTPUT_TEMPLATE digitalOutputTemplate;
    extern const FAST T_DIGITAL_OUTPUT_TEMPLATE digitalOutputTemplateDefault;
  #pragma bss_seg()
#else
  #pragma location="DIGITAL_OUTPUT_TEMPLATE" 
    extern const FAST __no_init T_DIGITAL_OUTPUT_TEMPLATE digitalOutputTemplate;
  #pragma location="DIGITAL_OUTPUT_TEMPLATE_DEFAULT" 
    extern const FAST __no_init T_DIGITAL_OUTPUT_TEMPLATE digitalOutputTemplateDefault;
#endif
extern const SLOW T_DIGITAL_OUTPUT_TEMPLATE digitalOutputTemplateInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>


//this template is for all other standard storage classes:

//@SubGen template start@<DATACLASS>
//-------------------------------------------------------------------------------------------------
//! definition of Template
#ifdef WIN32
  #pragma bss_seg(".AAA$DIGITAL_OUTPUT_DATA")
  extern const FAST T_DIGITAL_OUTPUT_TEMPLATE digitalOutputTemplate;
  #pragma bss_seg()
#else
  #pragma location="DIGITAL_OUTPUT_TEMPLATE"
  extern const FAST __no_init T_DIGITAL_OUTPUT_TEMPLATE digitalOutputTemplate;
#endif
extern const SLOW T_DIGITAL_OUTPUT_TEMPLATE digitalOutputTemplateInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS>


*/