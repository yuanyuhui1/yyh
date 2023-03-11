//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Frame
 Module
 Description    Declaration of subsystem attributes
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before Frame_attributes.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before Frame_attributes.h
#endif

#ifndef __T_UNIT_H__
  #error t_unit.h must be included before Frame_attributes.h
#endif

#ifndef __FRAME_TYPES_H__
  #error Frame_types.h must be included before Frame_attributes.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __FRAME_ATTRIBUTES_H__
  #error Frame_attributes.h included more than once
#endif

#define __FRAME_ATTRIBUTES_H__

//-------------------------------------------------------------------------------------------------
//! unit global variables
extern FAST TUSIGN16 frameState; // state of the SubSystem
extern FAST T_DATA_SEMAPHORE frameDataSemaphore;

//@SubGen put@ <DATACLASS>






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
extern const SLOW T_FRAME_TEMPLATE frameTemplate;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS CONSTANT>



//@SubGen template start@<DATACLASS DYNAMIC>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
extern FAST T_FRAME_TEMPLATE frameTemplate;
extern const SLOW T_FRAME_TEMPLATE frameTemplateInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS DYNAMIC>



//@SubGen template start@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>
//-------------------------------------------------------------------------------------------------
//! definition of Template
#ifdef WIN32
  #pragma bss_seg(".AAA$FRAME_DATA")
    extern const FAST T_FRAME_TEMPLATE frameTemplate;
    extern const FAST T_FRAME_TEMPLATE frameTemplateDefault;
  #pragma bss_seg()
#else
  #pragma location="FRAME_TEMPLATE" 
    extern const FAST __no_init T_FRAME_TEMPLATE frameTemplate;
  #pragma location="FRAME_TEMPLATE_DEFAULT" 
    extern const FAST __no_init T_FRAME_TEMPLATE frameTemplateDefault;
#endif
extern const SLOW T_FRAME_TEMPLATE frameTemplateInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>


//this template is for all other standard storage classes:

//@SubGen template start@<DATACLASS>
//-------------------------------------------------------------------------------------------------
//! definition of Template
#ifdef WIN32
  #pragma bss_seg(".AAA$FRAME_DATA")
  extern const FAST T_FRAME_TEMPLATE frameTemplate;
  #pragma bss_seg()
#else
  #pragma location="FRAME_TEMPLATE"
  extern const FAST __no_init T_FRAME_TEMPLATE frameTemplate;
#endif
extern const SLOW T_FRAME_TEMPLATE frameTemplateInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS>


*/
