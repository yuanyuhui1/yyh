//----------------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2005.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Hmi
 Module
 Description    declaration of subsystem attributes
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//----------------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before hmi_attributes.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be include before hmi_attributes.h
#endif


#ifndef __T_UNIT_H__
  #error t_unit.h must be include before hmi.h
#endif

#ifndef __HMI_TYPES_H__
  #error hmi_types.h must be included before hmi_attributes.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __HMI_ATTRIBUTES_H__
  #error hmi_attributes.h included more than once
#endif

#define __HMI_ATTRIBUTES_H__

//----------------------------------------------------------------------------------------------------------
//! unit global variables
extern FAST TUSIGN16 hmiState; // state of the SubSystem
extern FAST T_DATA_SEMAPHORE hmiDataSemaphore;

//----------------------------------------------------------------------------------------------------------
//! definition of dynamic data
extern FAST T_HMI_DYNAMIC hmi_dynamic_data;
extern const SLOW T_HMI_DYNAMIC hmi_dynamic_initializer;

#ifdef SFCD

#ifdef SFD_SPLIT
//----------------------------------------------------------------------------------------------------------
//! definition of STATIC_FREQUENT_CONSTANT_DEFAULT_SIMPLE
#ifdef WIN32
  #pragma bss_seg(".AAA$HMI_DATA")
    extern const FAST               T_HMI_STATIC_FREQUENT_SIMPLE                        hmi_static_frequent_constant_default_simple_data;
  #pragma bss_seg()
#else
    #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT_SIMPLE"
    extern const FAST __no_init     T_HMI_STATIC_FREQUENT_SIMPLE                        hmi_static_frequent_constant_default_simple_data;
#endif
    extern const SLOW               T_HMI_STATIC_FREQUENT_SIMPLE                        hmi_static_frequent_constant_default_simple_initializer;
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
//! definition of STATIC_FREQUENT_CONSTANT_DEFAULT_SIMPLE_CTW
#ifdef WIN32
  #pragma bss_seg(".AAA$HMI_DATA")
    extern const FAST               T_HMI_STATIC_FREQUENT_SIMPLE_CTW                    hmi_static_frequent_constant_default_simple_CTW_data;
  #pragma bss_seg()
#else
  #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT_SIMPLE_CTW"
    extern const FAST __no_init     T_HMI_STATIC_FREQUENT_SIMPLE_CTW                    hmi_static_frequent_constant_default_simple_CTW_data;
#endif
    extern const SLOW               T_HMI_STATIC_FREQUENT_SIMPLE_CTW                    hmi_static_frequent_constant_default_simple_CTW_initializer;
//----------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------
//! definition of STATIC_FREQUENT_CONSTANT_DEFAULT_MULTIPLE_OPERATOR_PAGES
#ifdef WIN32
  #pragma bss_seg(".AAA$HMI_DATA")
    extern const FAST               T_HMI_STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES       hmi_static_frequent_constant_default_multiple_operator_pages_data;
  #pragma bss_seg()
#else
  #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT_MULTIPLE_OPERATOR_PAGES"
    extern const FAST __no_init     T_HMI_STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES       hmi_static_frequent_constant_default_multiple_operator_pages_data;
#endif

    extern const SLOW               T_HMI_STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES       hmi_static_frequent_constant_default_multiple_operator_pages_initializer;
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
//! definition of STATIC_FREQUENT_CONSTANT_DEFAULT_MULTIPLE_OPERATOR_PAGES_CTW
#ifdef WIN32
  #pragma bss_seg(".AAA$HMI_DATA")
    extern const FAST               T_HMI_STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES_CTW   hmi_static_frequent_constant_default_multiple_operator_pages_CTW_data;
  #pragma bss_seg()
#else
  #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT_MULTIPLE_OPERATOR_PAGES_CTW"
    extern const FAST __no_init     T_HMI_STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES_CTW   hmi_static_frequent_constant_default_multiple_operator_pages_CTW_data;
#endif
    extern const SLOW               T_HMI_STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES_CTW   hmi_static_frequent_constant_default_multiple_operator_pages_CTW_initializer;
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
//! definition of STATIC_FREQUENT_CONSTANT_DEFAULT_CUSTOM_DP
#ifdef WIN32
  #pragma bss_seg(".AAA$HMI_DATA")
    extern const FAST               T_HMI_STATIC_FREQUENT_CUSTOM_DP                     hmi_static_frequent_constant_default_custom_DP_data;
  #pragma bss_seg()
#else
  #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT_CUSTOM_DP"
    extern const FAST __no_init     T_HMI_STATIC_FREQUENT_CUSTOM_DP                     hmi_static_frequent_constant_default_custom_DP_data;
#endif
    extern const SLOW               T_HMI_STATIC_FREQUENT_CUSTOM_DP                     hmi_static_frequent_constant_default_custom_DP_initializer;
//----------------------------------------------------------------------------------------------------------

#else // else condition for #ifdef SFD_SPLIT

//----------------------------------------------------------------------------------------------------------
//! definition of StaticFrequentConstantDefault
#ifdef WIN32
  #pragma bss_seg(".AAA$HMI_DATA")
    extern const FAST               T_HMI_STATIC_FREQUENT                               hmi_static_frequent_constant_default_data;
  #pragma bss_seg()
#else
  #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT"
    extern const FAST __no_init     T_HMI_STATIC_FREQUENT                               hmi_static_frequent_constant_default_data;
#endif
    extern const SLOW               T_HMI_STATIC_FREQUENT                               hmi_static_frequent_constant_default_initializer;
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
//! definition of StaticFrequentConstantDefault_CTW
#ifdef WIN32
  #pragma bss_seg(".AAA$HMI_DATA")
    extern const FAST               T_HMI_STATIC_FREQUENT_CTW                           hmi_static_frequent_constant_default_CTW_data;
  #pragma bss_seg()
#else
  #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT_CTW"
    extern const FAST __no_init     T_HMI_STATIC_FREQUENT_CTW                           hmi_static_frequent_constant_default_CTW_data;
#endif
    extern const SLOW               T_HMI_STATIC_FREQUENT_CTW                           hmi_static_frequent_constant_default_CTW_initializer;
//----------------------------------------------------------------------------------------------------------

#endif // end of else condition for #ifdef SFD_SPLIT

#else // else condition for #ifdef SFCD

#ifdef SFD_SPLIT
//----------------------------------------------------------------------------------------------------------
//! definition of STATIC_FREQUENT_STATIC_DEFAULT_SIMPLE
#ifdef WIN32
  #pragma bss_seg(".AAA$HMI_DATA")
    extern const FAST               T_HMI_STATIC_FREQUENT_SIMPLE                        hmi_static_frequent_static_default_simple_data;
    extern const FAST               T_HMI_STATIC_FREQUENT_SIMPLE                        hmi_static_frequent_static_default_simple_default;
  #pragma bss_seg()
#else
  #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT_SIMPLE"
    extern const FAST __no_init     T_HMI_STATIC_FREQUENT_SIMPLE                        hmi_static_frequent_static_default_simple_data;
  #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT_SIMPLE"
    extern const FAST __no_init     T_HMI_STATIC_FREQUENT_SIMPLE                        hmi_static_frequent_static_default_simple_default;
#endif
    extern const SLOW               T_HMI_STATIC_FREQUENT_SIMPLE                        hmi_static_frequent_static_default_simple_initializer;
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
//! definition of STATIC_FREQUENT_STATIC_DEFAULT_SIMPLE_CTW
#ifdef WIN32
  #pragma bss_seg(".AAA$HMI_DATA")
    extern const FAST               T_HMI_STATIC_FREQUENT_SIMPLE_CTW                    hmi_static_frequent_static_default_simple_CTW_data;
    extern const FAST               T_HMI_STATIC_FREQUENT_SIMPLE_CTW                    hmi_static_frequent_static_default_simple_CTW_default;
  #pragma bss_seg()
#else
  #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT_SIMPLE_CTW"
    extern const FAST __no_init     T_HMI_STATIC_FREQUENT_SIMPLE_CTW                    hmi_static_frequent_static_default_simple_CTW_data;
  #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT_SIMPLE_CTW"
    extern const FAST __no_init     T_HMI_STATIC_FREQUENT_SIMPLE_CTW                    hmi_static_frequent_static_default_simple_CTW_default;
#endif
    extern const SLOW               T_HMI_STATIC_FREQUENT_SIMPLE_CTW                    hmi_static_frequent_static_default_simple_CTW_initializer;
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
//! definition of STATIC_FREQUENT_STATIC_DEFAULT_MULTIPLE_OPERATOR_PAGES
#ifdef WIN32
  #pragma bss_seg(".AAA$HMI_DATA")
    extern const FAST               T_HMI_STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES       hmi_static_frequent_static_default_multiple_operator_pages_data;
    extern const FAST               T_HMI_STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES       hmi_static_frequent_static_default_multiple_operator_pages_default;
  #pragma bss_seg()
#else
  #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT_MULTIPLE_OPERATOR_PAGES"
    extern const FAST __no_init     T_HMI_STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES       hmi_static_frequent_static_default_multiple_operator_pages_data;
  #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT_MULTIPLE_OPERATOR_PAGES"
    extern const FAST __no_init     T_HMI_STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES       hmi_static_frequent_static_default_multiple_operator_pages_default;
#endif
    extern const SLOW               T_HMI_STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES       hmi_static_frequent_static_default_multiple_operator_pages_initializer;
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
//! definition of STATIC_FREQUENT_STATIC_DEFAULT_MULTIPLE_OPERATOR_PAGES_CTW
#ifdef WIN32
  #pragma bss_seg(".AAA$HMI_DATA")
    extern const FAST               T_HMI_STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES_CTW   hmi_static_frequent_static_default_multiple_operator_pages_CTW_data;
    extern const FAST               T_HMI_STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES_CTW   hmi_static_frequent_static_default_multiple_operator_pages_CTW_default;
  #pragma bss_seg()
#else
  #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT_MULTIPLE_OPERATOR_PAGES_CTW"
    extern const FAST __no_init     T_HMI_STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES_CTW   hmi_static_frequent_static_default_multiple_operator_pages_CTW_data;
  #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT_MULTIPLE_OPERATOR_PAGES_CTW"
    extern const FAST __no_init     T_HMI_STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES_CTW   hmi_static_frequent_static_default_multiple_operator_pages_CTW_default;
#endif
    extern const SLOW               T_HMI_STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES_CTW   hmi_static_frequent_static_default_multiple_operator_pages_CTW_initializer;
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
//! definition of STATIC_FREQUENT_STATIC_DEFAULT_CUSTOM_DP
#ifdef WIN32
  #pragma bss_seg(".AAA$HMI_DATA")
    extern const FAST               T_HMI_STATIC_FREQUENT_CUSTOM_DP                     hmi_static_frequent_static_default_custom_DP_data;
    extern const FAST               T_HMI_STATIC_FREQUENT_CUSTOM_DP                     hmi_static_frequent_static_default_custom_DP_default;
  #pragma bss_seg()
#else
  #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT_CUSTOM_DP"
    extern const FAST __no_init     T_HMI_STATIC_FREQUENT_CUSTOM_DP                     hmi_static_frequent_static_default_custom_DP_data;
  #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT_CUSTOM_DP"
    extern const FAST __no_init     T_HMI_STATIC_FREQUENT_CUSTOM_DP                     hmi_static_frequent_static_default_custom_DP_default;
#endif
    extern const SLOW               T_HMI_STATIC_FREQUENT_CUSTOM_DP                     hmi_static_frequent_static_default_custom_DP_initializer;
//----------------------------------------------------------------------------------------------------------

#else // else condition for #ifdef SFD_SPLIT

//----------------------------------------------------------------------------------------------------------
//! definition of StaticFrequentStaticDefault
#ifdef WIN32
  #pragma bss_seg(".AAA$HMI_DATA")
    extern const FAST               T_HMI_STATIC_FREQUENT                               hmi_static_frequent_static_default_data;
    extern const FAST               T_HMI_STATIC_FREQUENT                               hmi_static_frequent_static_default_default;
  #pragma bss_seg()
#else
  #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT"
    extern const FAST __no_init     T_HMI_STATIC_FREQUENT                               hmi_static_frequent_static_default_data;
  #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT"
    extern const FAST __no_init     T_HMI_STATIC_FREQUENT                               hmi_static_frequent_static_default_default;
#endif
    extern const SLOW               T_HMI_STATIC_FREQUENT                               hmi_static_frequent_static_default_initializer;
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
//! definition of StaticFrequentStaticDefault_CTW
#ifdef WIN32
  #pragma bss_seg(".AAA$HMI_DATA")
    extern const FAST               T_HMI_STATIC_FREQUENT_CTW                           hmi_static_frequent_static_default_CTW_data;
    extern const FAST               T_HMI_STATIC_FREQUENT_CTW                           hmi_static_frequent_static_default_CTW_default;
  #pragma bss_seg()
#else
  #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT_CTW"
    extern const FAST __no_init     T_HMI_STATIC_FREQUENT_CTW                           hmi_static_frequent_static_default_CTW_data;
  #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT_CTW"
    extern const FAST __no_init     T_HMI_STATIC_FREQUENT_CTW                           hmi_static_frequent_static_default_CTW_default;
#endif
    extern const SLOW               T_HMI_STATIC_FREQUENT_CTW                           hmi_static_frequent_static_default_CTW_initializer;
//----------------------------------------------------------------------------------------------------------

#endif // end of else condition for #ifdef SFD_SPLIT

#endif  // end of else condition for #ifdef SFCD

/* Save SubGen templates for Entry tool, actually, the DataClass is depend on HMIGen Defines */
/*
//@SubGen put@ <DATACLASS>    
//@SubGen start@<DATACLASS _SFD_MULTI_OP>
//----------------------------------------------------------------------------------------------------------
//! definition of _SFD_MULTI_OP
#ifdef WIN32
  #pragma bss_seg(".AAA$HMI_DATA")
  extern const FAST T_HMI__SFD_MULTI_OP hmi__SFD_MULTI_OP_data;
  #pragma bss_seg()
#else
  #pragma location="HMI__SFD_MULTI_OP"
  extern const FAST __no_init T_HMI__SFD_MULTI_OP hmi__SFD_MULTI_OP_data;
#endif
extern const SLOW T_HMI__SFD_MULTI_OP hmi__SFD_MULTI_OP_initializer;
//----------------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS _SFD_MULTI_OP>


//@SubGen start@<DATACLASS _SFD_CUSTOM_DP>
//----------------------------------------------------------------------------------------------------------
//! definition of _SFD_CUSTOM_DP
#ifdef WIN32
  #pragma bss_seg(".AAA$HMI_DATA")
  extern const FAST T_HMI__SFD_CUSTOM_DP hmi__SFD_CUSTOM_DP_data;
  #pragma bss_seg()
#else
  #pragma location="HMI__SFD_CUSTOM_DP"
  extern const FAST __no_init T_HMI__SFD_CUSTOM_DP hmi__SFD_CUSTOM_DP_data;
#endif
extern const SLOW T_HMI__SFD_CUSTOM_DP hmi__SFD_CUSTOM_DP_initializer;
//----------------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS _SFD_CUSTOM_DP>



//@SubGen start@<DATACLASS _SFD_SIMPLE>
//----------------------------------------------------------------------------------------------------------
//! definition of _SFD_SIMPLE
#ifdef WIN32
  #pragma bss_seg(".AAA$HMI_DATA")
  extern const FAST T_HMI__SFD_SIMPLE hmi__SFD_SIMPLE_data;
  #pragma bss_seg()
#else
  #pragma location="HMI__SFD_SIMPLE"
  extern const FAST __no_init T_HMI__SFD_SIMPLE hmi__SFD_SIMPLE_data;
#endif
extern const SLOW T_HMI__SFD_SIMPLE hmi__SFD_SIMPLE_initializer;
//----------------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS _SFD_SIMPLE>



//@SubGen start@<DATACLASS _SFD_SIMPLE_CTW>
//----------------------------------------------------------------------------------------------------------
//! definition of _SFD_SIMPLE_CTW
#ifdef WIN32
  #pragma bss_seg(".AAA$HMI_DATA")
  extern const FAST T_HMI__SFD_SIMPLE_CTW hmi__SFD_SIMPLE_CTW_data;
  #pragma bss_seg()
#else
  #pragma location="HMI__SFD_SIMPLE_CTW"
  extern const FAST __no_init T_HMI__SFD_SIMPLE_CTW hmi__SFD_SIMPLE_CTW_data;
#endif
extern const SLOW T_HMI__SFD_SIMPLE_CTW hmi__SFD_SIMPLE_CTW_initializer;
//----------------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS _SFD_SIMPLE_CTW>


//@SubGen start@<DATACLASS _SFD_MULTI_OP_CTW>
//----------------------------------------------------------------------------------------------------------
//! definition of _SFD_MULTI_OP_CTW
#ifdef WIN32
  #pragma bss_seg(".AAA$HMI_DATA")
  extern const FAST T_HMI__SFD_MULTI_OP_CTW hmi__SFD_MULTI_OP_CTW_data;
  #pragma bss_seg()
#else
  #pragma location="HMI__SFD_MULTI_OP_CTW"
  extern const FAST __no_init T_HMI__SFD_MULTI_OP_CTW hmi__SFD_MULTI_OP_CTW_data;
#endif
extern const SLOW T_HMI__SFD_MULTI_OP_CTW hmi__SFD_MULTI_OP_CTW_initializer;
//----------------------------------------------------------------------------------------------------------
//@SubGen end@<DATACLASS _SFD_MULTI_OP_CTW>
*/

    
//@SubGen start@<DATACLASS DYNAMIC>
//----------------------------------------------------------------------------------------------------------
//! definition of dynamic data
extern FAST T_HMI_DYNAMIC hmi_dynamic_data;
extern const SLOW T_HMI_DYNAMIC hmi_dynamic_initializer;
//----------------------------------------------------------------------------------------------------------
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
//----------------------------------------------------------------------------------------------------------
//! definition of constant block
extern const SLOW T_HMI_CONSTANT hmi_constant_data;
//----------------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS CONSTANT>



//@SubGen template start@<DATACLASS DYNAMIC>
//----------------------------------------------------------------------------------------------------------
//! definition of dynamic data
extern FAST T_HMI_DYNAMIC hmi_dynamic_data;
extern const SLOW T_HMI_DYNAMIC hmi_dynamic_initializer;
//----------------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS DYNAMIC>



//@SubGen template start@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>
//----------------------------------------------------------------------------------------------------------
//! definition of Template
#ifdef WIN32
  #pragma bss_seg(".AAA$HMI_DATA")
    extern const FAST T_HMI_TEMPLATE hmi_template_data;
    extern const FAST T_HMI_TEMPLATE hmi_template_default;
  #pragma bss_seg()
#else
  #pragma location="HMI_TEMPLATE"
    extern const FAST __no_init T_HMI_TEMPLATE hmi_template_data;
  #pragma location="HMI_TEMPLATE_DEFAULT"
    extern const FAST __no_init T_HMI_TEMPLATE hmi_template_default;
#endif
extern const SLOW T_HMI_TEMPLATE hmi_template_initializer;
//----------------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>


//this template is for all other standard storage classes:

//@SubGen template start@<DATACLASS>
//----------------------------------------------------------------------------------------------------------
//! definition of Template
#ifdef WIN32
  #pragma bss_seg(".AAA$HMI_DATA")
  extern const FAST T_HMI_TEMPLATE hmi_template_data;
  #pragma bss_seg()
#else
  #pragma location="HMI_TEMPLATE"
  extern const FAST __no_init T_HMI_TEMPLATE hmi_template_data;
#endif
extern const SLOW T_HMI_TEMPLATE hmi_template_initializer;
//----------------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS>


*/
