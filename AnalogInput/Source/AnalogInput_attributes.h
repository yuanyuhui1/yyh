//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem AnalogInput
 Module
 Description    Declaration of subsystem attributes
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before AnalogInput_attributes.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before AnalogInput_attributes.h
#endif

#ifndef __T_UNIT_H__
  #error t_unit.h must be included before AnalogInput_attributes.h
#endif

#ifndef __ANALOG_INPUT_TYPES_H__
  #error AnalogInput_types.h must be included before AnalogInput_attributes.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __ANALOG_INPUT_ATTRIBUTES_H__
  #error AnalogInput_attributes.h included more than once
#endif

#define __ANALOG_INPUT_ATTRIBUTES_H__

//-------------------------------------------------------------------------------------------------
//! unit global variables
extern FAST TUSIGN16 analogInputState; // state of the SubSystem
extern FAST T_DATA_SEMAPHORE analogInputDataSemaphore;

//@SubGen put@ <DATACLASS>
//@SubGen start@<DATACLASS STATIC_FREQ_STATIC_DEFALUT_CALIBRATION>
//-------------------------------------------------------------------------------------------------
//! definition of StaticFreqStaticDefalutCalibration
#ifdef WIN32
  #pragma bss_seg(".AAA$ANALOG_INPUT_DATA")
    extern const FAST T_ANALOG_INPUT_STATIC_FREQ_STATIC_DEFALUT_CALIBRATION analogInputStaticFreqStaticDefalutCalibration;
    extern const FAST T_ANALOG_INPUT_STATIC_FREQ_STATIC_DEFALUT_CALIBRATION analogInputStaticFreqStaticDefalutCalibrationDefault;
  #pragma bss_seg()
#else
  #pragma location="ANALOG_INPUT_STATIC_FREQ_STATIC_DEFALUT_CALIBRATION" 
    extern const FAST __no_init T_ANALOG_INPUT_STATIC_FREQ_STATIC_DEFALUT_CALIBRATION analogInputStaticFreqStaticDefalutCalibration;
  #pragma location="ANALOG_INPUT_STATIC_FREQ_STATIC_DEFALUT_CALIBRATION_DEFAULT" 
    extern const FAST __no_init T_ANALOG_INPUT_STATIC_FREQ_STATIC_DEFALUT_CALIBRATION analogInputStaticFreqStaticDefalutCalibrationDefault;
#endif
extern const SLOW T_ANALOG_INPUT_STATIC_FREQ_STATIC_DEFALUT_CALIBRATION analogInputStaticFreqStaticDefalutCalibrationInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS STATIC_FREQ_STATIC_DEFALUT_CALIBRATION>



//@SubGen start@<DATACLASS UNPROTECTED>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
extern FAST T_ANALOG_INPUT_UNPROTECTED analogInputUnprotected;
extern const SLOW T_ANALOG_INPUT_UNPROTECTED analogInputUnprotectedInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS UNPROTECTED>



//@SubGen start@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>
//-------------------------------------------------------------------------------------------------
//! definition of StaticFrequentStaticDefault
#ifdef WIN32
  #pragma bss_seg(".AAA$ANALOG_INPUT_DATA")
    extern const FAST T_ANALOG_INPUT_STATIC_FREQUENT_STATIC_DEFAULT analogInputStaticFrequentStaticDefault;
    extern const FAST T_ANALOG_INPUT_STATIC_FREQUENT_STATIC_DEFAULT analogInputStaticFrequentStaticDefaultDefault;
  #pragma bss_seg()
#else
  #pragma location="ANALOG_INPUT_STATIC_FREQUENT_STATIC_DEFAULT" 
    extern const FAST __no_init T_ANALOG_INPUT_STATIC_FREQUENT_STATIC_DEFAULT analogInputStaticFrequentStaticDefault;
  #pragma location="ANALOG_INPUT_STATIC_FREQUENT_STATIC_DEFAULT_DEFAULT" 
    extern const FAST __no_init T_ANALOG_INPUT_STATIC_FREQUENT_STATIC_DEFAULT analogInputStaticFrequentStaticDefaultDefault;
#endif
extern const SLOW T_ANALOG_INPUT_STATIC_FREQUENT_STATIC_DEFAULT analogInputStaticFrequentStaticDefaultInitializer;
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
extern const SLOW T_ANALOG_INPUT_TEMPLATE analogInputTemplate;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS CONSTANT>



//@SubGen template start@<DATACLASS DYNAMIC>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
extern FAST T_ANALOG_INPUT_TEMPLATE analogInputTemplate;
extern const SLOW T_ANALOG_INPUT_TEMPLATE analogInputTemplateInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS DYNAMIC>



//@SubGen template start@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>
//-------------------------------------------------------------------------------------------------
//! definition of Template
#ifdef WIN32
  #pragma bss_seg(".AAA$ANALOG_INPUT_DATA")
    extern const FAST T_ANALOG_INPUT_TEMPLATE analogInputTemplate;
    extern const FAST T_ANALOG_INPUT_TEMPLATE analogInputTemplateDefault;
  #pragma bss_seg()
#else
  #pragma location="ANALOG_INPUT_TEMPLATE" 
    extern const FAST __no_init T_ANALOG_INPUT_TEMPLATE analogInputTemplate;
  #pragma location="ANALOG_INPUT_TEMPLATE_DEFAULT" 
    extern const FAST __no_init T_ANALOG_INPUT_TEMPLATE analogInputTemplateDefault;
#endif
extern const SLOW T_ANALOG_INPUT_TEMPLATE analogInputTemplateInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>


//this template is for all other standard storage classes:

//@SubGen template start@<DATACLASS>
//-------------------------------------------------------------------------------------------------
//! definition of Template
#ifdef WIN32
  #pragma bss_seg(".AAA$ANALOG_INPUT_DATA")
  extern const FAST T_ANALOG_INPUT_TEMPLATE analogInputTemplate;
  #pragma bss_seg()
#else
  #pragma location="ANALOG_INPUT_TEMPLATE"
  extern const FAST __no_init T_ANALOG_INPUT_TEMPLATE analogInputTemplate;
#endif
extern const SLOW T_ANALOG_INPUT_TEMPLATE analogInputTemplateInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS>


*/

