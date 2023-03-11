//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2011.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Hart
 Module
 Description    Declaration of subsystem attributes
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before Hart_attributes.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before Hart_attributes.h
#endif

#ifndef __T_UNIT_H__
  #error t_unit.h must be included before Hart_attributes.h
#endif

#ifndef __HART_TYPES_H__
  #error Hart_types.h must be included before Hart_attributes.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __HART_ATTRIBUTES_H__
  #error Hart_attributes.h included more than once
#endif

#define __HART_ATTRIBUTES_H__

//-------------------------------------------------------------------------------------------------
//! unit global variables
extern FAST TUSIGN16 hartState; // state of the SubSystem
extern FAST T_DATA_SEMAPHORE hartDataSemaphore;

//@SubGen put@ <DATACLASS>
//@SubGen start@<DATACLASS FREQ_STATIC>
//-------------------------------------------------------------------------------------------------
//! definition of FreqStatic

#ifdef WIN32
  #pragma bss_seg(".AAA$HART_DATA")
    extern const FAST T_HART_FREQ_STATIC hartFreqStatic;
    extern const FAST T_HART_FREQ_STATIC hartFreqStaticDefault;
  #pragma bss_seg()
#else
  #pragma location="HART_FREQ_STATIC"
    extern const FAST __no_init T_HART_FREQ_STATIC hartFreqStatic;
  #pragma location="HART_FREQ_STATIC_DEFAULT"
    extern const FAST __no_init T_HART_FREQ_STATIC hartFreqStaticDefault;
#endif
extern const SLOW T_HART_FREQ_STATIC hartFreqStaticInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS FREQ_STATIC>



//@SubGen start@<DATACLASS CONSTANT>
//-------------------------------------------------------------------------------------------------
//! definition of constant block
extern const SLOW T_HART_CONSTANT hartConstant;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS CONSTANT>



//@SubGen start@<DATACLASS STATIC_RARE>
//-------------------------------------------------------------------------------------------------
//! definition of StaticRare
#ifdef WIN32
  #pragma bss_seg(".AAA$HART_DATA")
    extern const FAST T_HART_STATIC_RARE hartStaticRare;
  #pragma bss_seg()
#else
  #pragma location="HART_STATIC_RARE"
    extern const FAST __no_init T_HART_STATIC_RARE hartStaticRare;
#endif
extern const SLOW T_HART_STATIC_RARE hartStaticRareInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS STATIC_RARE>



//@SubGen start@<DATACLASS DYNAMIC>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
extern FAST T_HART_DYNAMIC hartDynamic;
extern const SLOW T_HART_DYNAMIC hartDynamicInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS DYNAMIC>



//@SubGen start@<DATACLASS FREQ_NO>
//-------------------------------------------------------------------------------------------------
//! definition of FreqNo
#ifdef WIN32
  #pragma bss_seg(".AAA$HART_DATA")
    extern const FAST T_HART_FREQ_NO hartFreqNo;
  #pragma bss_seg()
#else
  #pragma location="HART_STATIC_FREQUENT_NO_DEFAULT"
    extern const FAST __no_init T_HART_FREQ_NO hartFreqNo;
#endif
extern const SLOW T_HART_FREQ_NO hartFreqNoInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS FREQ_NO>



//@SubGen start@<DATACLASS DYN>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
extern FAST T_HART_DYN hartDyn;
extern const SLOW T_HART_DYN hartDynInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS DYN>


//@SubGen start@<DATACLASS RARE>
//-------------------------------------------------------------------------------------------------
//! definition of Rare
#ifdef WIN32
  #pragma bss_seg(".AAA$HART_DATA")
    extern const FAST T_HART_RARE hartRare;
  #pragma bss_seg()
#else
  #pragma location="HART_STATIC_RARE"
    extern const FAST __no_init T_HART_RARE hartRare;
#endif
extern const SLOW T_HART_RARE hartRareInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS RARE>


//@SubGen start@<DATACLASS FREQ_CST>
//-------------------------------------------------------------------------------------------------
//! definition of FreqCst
#ifdef WIN32
  #pragma bss_seg(".AAA$HART_DATA")
    extern const FAST T_HART_FREQ_CST hartFreqCst;
  #pragma bss_seg()
#else
  #pragma location="HART_STATIC_FREQUENT_STATIC_DEFAULT_DEFAULT"
    extern const FAST __no_init T_HART_FREQ_CST hartFreqCst;
#endif
extern const SLOW T_HART_FREQ_CST hartFreqCstInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS FREQ_CST>


//@SubGen start@<DATACLASS CST>
//-------------------------------------------------------------------------------------------------
//! definition of constant block
extern const SLOW T_HART_CST hartCst;
//-------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS CST>









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
extern const SLOW T_HART_TEMPLATE hartTemplate;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS CONSTANT>



//@SubGen template start@<DATACLASS DYNAMIC>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
extern FAST T_HART_TEMPLATE hartTemplate;
extern const SLOW T_HART_TEMPLATE hartTemplateInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS DYNAMIC>



//@SubGen template start@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>
//-------------------------------------------------------------------------------------------------
//! definition of Template
#ifdef WIN32
  #pragma bss_seg(".AAA$HART_DATA")
    extern const FAST T_HART_TEMPLATE hartTemplate;
    extern const FAST T_HART_TEMPLATE hartTemplateDefault;
  #pragma bss_seg()
#else
  #pragma location="HART_TEMPLATE"
    extern const FAST __no_init T_HART_TEMPLATE hartTemplate;
  #pragma location="HART_TEMPLATE_DEFAULT"
    extern const FAST __no_init T_HART_TEMPLATE hartTemplateDefault;
#endif
extern const SLOW T_HART_TEMPLATE hartTemplateInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>


//this template is for all other standard storage classes:

//@SubGen template start@<DATACLASS>
//-------------------------------------------------------------------------------------------------
//! definition of Template
#ifdef WIN32
  #pragma bss_seg(".AAA$HART_DATA")
    extern const FAST T_HART_TEMPLATE hartTemplate;
  #pragma bss_seg()
#else
  #pragma location="HART_TEMPLATE"
    extern const FAST __no_init T_HART_TEMPLATE hartTemplate;
#endif
extern const SLOW T_HART_TEMPLATE hartTemplateInitializer;
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS>


*/

