//----------------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2005.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Hmi
 Module
 Description    type defintions for subsystem attributes
                for Subsystems based on T_UNIT.
 Remarks
 </pre>
*/
//----------------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before hmi_types.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be include before hmi_types.h
#endif

#ifndef _COMPILETIMEDEFINITIONS
  #error compileTimeDefinitions.h must be included
//#include "compileTimeDefinitions.h"

#endif

//............................................
//!  extend include list if neccessary

#ifdef __HMI_TYPES_H__
  #error hmi_types.h included more than once
#endif

#define __HMI_TYPES_H__

// implement the required storage-classes
/*
//@SubGen put@ <DATACLASS>
 //----------------------------------------------------------------------------------------------------------
 //! structure of _SFD_MULTI_OP block
 typedef struct _T_HMI__SFD_MULTI_OP
 {
     //@SubGen start@    <DATACLASS _SFD_MULTI_OP>
    TUSIGN8 mainOperatorView_2_1;
    TUSIGN8 mainOperatorView_2_2;
    TUSIGN8 mainOperatorView_2_3;
    TUSIGN8 mainOperatorView_2_bargraph;
    TUSIGN8 mainOperatorView_3_1;
    TUSIGN8 mainOperatorView_3_2;
    TUSIGN8 mainOperatorView_3_3;
    TUSIGN8 mainOperatorView_3_bargraph;
    TUSIGN8 mainOperatorView_4_1;
    TUSIGN8 mainOperatorView_4_2;
    TUSIGN8 mainOperatorView_4_3;
    TUSIGN8 mainOperatorView_4_bargraph;
    TUSIGN8 displayMode_2;
    TUSIGN8 displayMode_3;
    TUSIGN8 displayMode_4;
    TUSIGN8 autoscrollEnable;
     //@SubGen end@    <DATACLASS _SFD_MULTI_OP>
 } T_HMI__SFD_MULTI_OP;


 //----------------------------------------------------------------------------------------------------------
 //! structure of _SFD_CUSTOM_DP block
 typedef struct _T_HMI__SFD_CUSTOM_DP
 {
     //@SubGen start@    <DATACLASS _SFD_CUSTOM_DP>
    TUSIGN8 customDP_1;
    TUSIGN8 customDP_2;
    TUSIGN8 customDP_3;
    TUSIGN8 customDP_4;
    TUSIGN8 customDP_5;
    TUSIGN8 customDP_6;
    TUSIGN8 customDP_7;
    TUSIGN8 customDP_8;
    TUSIGN8 customDP_9;
    TUSIGN8 customDP_10;
    TUSIGN8 customDP_11;
     //@SubGen end@    <DATACLASS _SFD_CUSTOM_DP>
 } T_HMI__SFD_CUSTOM_DP;



 //----------------------------------------------------------------------------------------------------------
 //! structure of _SFD_SIMPLE block
 typedef struct _T_HMI__SFD_SIMPLE
 {
     //@SubGen start@    <DATACLASS _SFD_SIMPLE>
    TUSIGN8 language;
    TUSIGN8 mainOperatorView_1_1;
    TUSIGN8 mainOperatorView_1_2;
    TUSIGN8 mainOperatorView_1_3;
    TUSIGN8 mainOperatorView_1_bargraph;
    TUSIGN8 contrast;
    TUSIGN8 localOperationEnable;
    TUSIGN8 displayMode_1;
    TUSIGN8 DateFormat;
     //@SubGen end@    <DATACLASS _SFD_SIMPLE>
 } T_HMI__SFD_SIMPLE;



 //----------------------------------------------------------------------------------------------------------
 //! structure of _SFD_SIMPLE_CTW block
 typedef struct _T_HMI__SFD_SIMPLE_CTW
 {
     //@SubGen start@    <DATACLASS _SFD_SIMPLE_CTW>
    TUSIGN8 Rotation;
    TUSIGN8 MenuFormat;
    TUSIGN8 mainOperatorView_1_4;
     //@SubGen end@    <DATACLASS _SFD_SIMPLE_CTW>
 } T_HMI__SFD_SIMPLE_CTW;


 //----------------------------------------------------------------------------------------------------------
 //! structure of _SFD_MULTI_OP_CTW block
 typedef struct _T_HMI__SFD_MULTI_OP_CTW
 {
     //@SubGen start@    <DATACLASS _SFD_MULTI_OP_CTW>
    TUSIGN8 displayMode_5;
    TUSIGN8 displayMode_6;
    TUSIGN8 mainOperatorView_5_1;
    TUSIGN8 mainOperatorView_5_2;
    TUSIGN8 mainOperatorView_5_3;
    TUSIGN8 mainOperatorView_5_bargraph;
    TUSIGN8 mainOperatorView_6_1;
    TUSIGN8 mainOperatorView_6_2;
    TUSIGN8 mainOperatorView_6_3;
    TUSIGN8 mainOperatorView_6_bargraph;
    TUSIGN8 mainOperatorView_2_4;
    TUSIGN8 mainOperatorView_3_4;
    TUSIGN8 mainOperatorView_4_4;
    TUSIGN8 mainOperatorView_5_4;
    TUSIGN8 mainOperatorView_6_4;
     //@SubGen end@    <DATACLASS _SFD_MULTI_OP_CTW>
 } T_HMI__SFD_MULTI_OP_CTW;
*/


#ifdef SFD_SPLIT
 //----------------------------------------------------------------------------------------------------------
 //! structure of STATIC_FREQUENT_SIMPLE block
 typedef struct _T_HMI_STATIC_FREQUENT_SIMPLE
 {
     //@SubGen start@    <DATACLASS _T_HMI_STATIC_FREQUENT_SIMPLE>
    TUSIGN8 language;
    TUSIGN8 mainOperatorView_1_1;
    TUSIGN8 mainOperatorView_1_2;
    TUSIGN8 mainOperatorView_1_3;
    TUSIGN8 mainOperatorView_1_bargraph;
    TUSIGN8 contrast;
    TUSIGN8 localOperationEnable;
    TUSIGN8 displayMode_1;
    TUSIGN8 DateFormat;
     //@SubGen end@    <DATACLASS _T_HMI_STATIC_FREQUENT_SIMPLE>
 } T_HMI_STATIC_FREQUENT_SIMPLE;

 //----------------------------------------------------------------------------------------------------------
 //! structure of STATIC_FREQUENT_SIMPLE_CTW block
 typedef struct _T_HMI_STATIC_FREQUENT_SIMPLE_CTW
 {
     //@SubGen start@    <DATACLASS STATIC_FREQUENT_SIMPLE_CTW>
    TUSIGN8 Rotation;
    TUSIGN8 MenuFormat;
    TUSIGN8 mainOperatorView_1_4;
     //@SubGen end@    <DATACLASS STATIC_FREQUENT_SIMPLE_CTW>
 } T_HMI_STATIC_FREQUENT_SIMPLE_CTW;

 //----------------------------------------------------------------------------------------------------------
 //! structure of STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES block
 typedef struct _T_HMI_STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES
 {
     //@SubGen start@    <DATACLASS STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES>
    TUSIGN8 mainOperatorView_2_1;
    TUSIGN8 mainOperatorView_2_2;
    TUSIGN8 mainOperatorView_2_3;
    TUSIGN8 mainOperatorView_2_bargraph;
    TUSIGN8 mainOperatorView_3_1;
    TUSIGN8 mainOperatorView_3_2;
    TUSIGN8 mainOperatorView_3_3;
    TUSIGN8 mainOperatorView_3_bargraph;
    TUSIGN8 mainOperatorView_4_1;
    TUSIGN8 mainOperatorView_4_2;
    TUSIGN8 mainOperatorView_4_3;
    TUSIGN8 mainOperatorView_4_bargraph;
    TUSIGN8 displayMode_2;
    TUSIGN8 displayMode_3;
    TUSIGN8 displayMode_4;
    TUSIGN8 autoscrollEnable;
     //@SubGen end@    <DATACLASS STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES>
 } T_HMI_STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES;

 //----------------------------------------------------------------------------------------------------------
 //! structure of STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES_CTW block
 typedef struct _T_HMI_STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES_CTW
 {
     //@SubGen start@    <DATACLASS STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES_CTW>
    TUSIGN8 displayMode_5;
    TUSIGN8 displayMode_6;
    TUSIGN8 mainOperatorView_5_1;
    TUSIGN8 mainOperatorView_5_2;
    TUSIGN8 mainOperatorView_5_3;
    TUSIGN8 mainOperatorView_5_bargraph;
    TUSIGN8 mainOperatorView_6_1;
    TUSIGN8 mainOperatorView_6_2;
    TUSIGN8 mainOperatorView_6_3;
    TUSIGN8 mainOperatorView_6_bargraph;
    TUSIGN8 mainOperatorView_2_4;
    TUSIGN8 mainOperatorView_3_4;
    TUSIGN8 mainOperatorView_4_4;
    TUSIGN8 mainOperatorView_5_4;
    TUSIGN8 mainOperatorView_6_4;
     //@SubGen end@    <DATACLASS STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES_CTW>
 } T_HMI_STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES_CTW;

 //----------------------------------------------------------------------------------------------------------
 //! structure of STATIC_FREQUENT_CUSTOM_DP block
 typedef struct _T_HMI_STATIC_FREQUENT_CUSTOM_DP
 {
     //@SubGen start@    <DATACLASS STATIC_FREQUENT_CUSTOM_DP>
    TUSIGN8 customDP_1;
    TUSIGN8 customDP_2;
    TUSIGN8 customDP_3;
    TUSIGN8 customDP_4;
    TUSIGN8 customDP_5;
    TUSIGN8 customDP_6;
    TUSIGN8 customDP_7;
    TUSIGN8 customDP_8;
    TUSIGN8 customDP_9;
    TUSIGN8 customDP_10;
    TUSIGN8 customDP_11;
     //@SubGen end@    <DATACLASS STATIC_FREQUENT_CUSTOM_DP>
 } T_HMI_STATIC_FREQUENT_CUSTOM_DP;

#else // else condition for #ifdef SFD_SPLIT

 //----------------------------------------------------------------------------------------------------------
 //! structure of STATIC_FREQUENT block
 typedef struct _T_HMI_STATIC_FREQUENT
 {
     //@SubGen start@    <DATACLASS STATIC_FREQUENT>
    TUSIGN8 language;
    TUSIGN8 mainOperatorView_1_1;
    TUSIGN8 mainOperatorView_1_2;
    TUSIGN8 mainOperatorView_1_3;
    TUSIGN8 mainOperatorView_1_bargraph;
    TUSIGN8 mainOperatorView_2_1;
    TUSIGN8 mainOperatorView_2_2;
    TUSIGN8 mainOperatorView_2_3;
    TUSIGN8 mainOperatorView_2_bargraph;
    TUSIGN8 mainOperatorView_3_1;
    TUSIGN8 mainOperatorView_3_2;
    TUSIGN8 mainOperatorView_3_3;
    TUSIGN8 mainOperatorView_3_bargraph;
    TUSIGN8 mainOperatorView_4_1;
    TUSIGN8 mainOperatorView_4_2;
    TUSIGN8 mainOperatorView_4_3;
    TUSIGN8 mainOperatorView_4_bargraph;
    TUSIGN8 contrast;
    TUSIGN8 localOperationEnable;
    TUSIGN8 displayMode_1;
    TUSIGN8 displayMode_2;
    TUSIGN8 displayMode_3;
    TUSIGN8 displayMode_4;
    TUSIGN8 autoscrollEnable;
    TUSIGN8 customDP_1;
    TUSIGN8 customDP_2;
    TUSIGN8 customDP_3;
    TUSIGN8 customDP_4;
    TUSIGN8 customDP_5;
    TUSIGN8 customDP_6;
    TUSIGN8 customDP_7;
    TUSIGN8 customDP_8;
    TUSIGN8 customDP_9;
    TUSIGN8 customDP_10;
    TUSIGN8 customDP_11;
    TUSIGN8 DateFormat;
    //@SubGen end@    <DATACLASS STATIC_FREQUENT>
 } T_HMI_STATIC_FREQUENT;

  //----------------------------------------------------------------------------------------------------------
 //! structure of STATIC_FREQUENT_CTW block
 typedef struct _T_HMI_STATIC_FREQUENT_CTW
 {
     //@SubGen start@    <DATACLASS STATIC_FREQUENT_CTW>
    TUSIGN8 Rotation;
    TUSIGN8 MenuFormat;
    TUSIGN8 displayMode_5;
    TUSIGN8 displayMode_6;
    TUSIGN8 mainOperatorView_5_1;
    TUSIGN8 mainOperatorView_5_2;
    TUSIGN8 mainOperatorView_5_3;
    TUSIGN8 mainOperatorView_5_bargraph;
    TUSIGN8 mainOperatorView_6_1;
    TUSIGN8 mainOperatorView_6_2;
    TUSIGN8 mainOperatorView_6_3;
    TUSIGN8 mainOperatorView_6_bargraph;
    TUSIGN8 mainOperatorView_1_4;
    TUSIGN8 mainOperatorView_2_4;
    TUSIGN8 mainOperatorView_3_4;
    TUSIGN8 mainOperatorView_4_4;
    TUSIGN8 mainOperatorView_5_4;
    TUSIGN8 mainOperatorView_6_4;

    //@SubGen end@    <DATACLASS STATIC_FREQUENT_CTW>
 } T_HMI_STATIC_FREQUENT_CTW;

#endif // end of else condition for #ifdef SFD_SPLIT


 //----------------------------------------------------------------------------------------------------------
 //! structure of DYNAMIC block
 typedef struct _T_HMI_DYNAMIC
 {
     //@SubGen start@    <DATACLASS DYNAMIC>
    TUSIGN8 lowLevelSoftwareRevision;
    TUSIGN8 defaultContrast;
    TUSIGN8 diagnosisView;
    TUSIGN8 Password[6];
    TUSIGN8 SetPassword_StandardLevel[6];
    TUSIGN8 SetPassword_AdvancedLevel[6];
    TUSIGN8 SetPassword_ServiceLevel[6];
    TUSIGN8 SetPassword_StandardLevelDefault[6];
    TUSIGN8 SetPassword_AdvancedLevelDefault[6];
    TUSIGN8 SetPassword_ServiceLevelDefault[6];
    TUSIGN8 displayTest;
#ifdef COMMON_TOP_WORKS_NEW_FEATURES
    TUSIGN8 Customlanguage;
#endif
     //@SubGen end@    <DATACLASS DYNAMIC>
 } T_HMI_DYNAMIC;





/*

This section defines templates for the SubSystem CodeGenerator.
Do not modify or remove this section

//@SubGen template start@ <DATACLASS>
//----------------------------------------------------------------------------------------------------------
//! structure of TEMPLATE block
typedef struct _T_HMI_TEMPLATE
{
    //@SubGen start@    <DATACLASS TEMPLATE>
    //@SubGen end@    <DATACLASS TEMPLATE>
} T_HMI_TEMPLATE;
//@SubGen template end@ <DATACLASS>

*/

#ifdef SFCD

    #ifdef SFD_SPLIT

        #define SFD_SIMPLE          hmi_static_frequent_constant_default_simple_data
        #define SFD_MULTI_OP        hmi_static_frequent_constant_default_multiple_operator_pages_data
        #define SFD_CUSTOM_DP       hmi_static_frequent_constant_default_custom_DP_data
        #ifdef COMMON_TOP_WORKS_NEW_FEATURES
        #define SFD_SIMPLE_CTW      hmi_static_frequent_constant_default_simple_CTW_data
        #define SFD_MULTI_OP_CTW    hmi_static_frequent_constant_default_multiple_operator_pages_CTW_data
        #endif

    #else   // else condition for #ifdef SFD_SPLIT

        #define SFD_SIMPLE          hmi_static_frequent_constant_default_data
        #define SFD_MULTI_OP        hmi_static_frequent_constant_default_data
        #define SFD_CUSTOM_DP       hmi_static_frequent_constant_default_data
        #ifdef COMMON_TOP_WORKS_NEW_FEATURES
        #define SFD_SIMPLE_CTW      hmi_static_frequent_constant_default_CTW_data
        #define SFD_MULTI_OP_CTW    hmi_static_frequent_constant_default_CTW_data
        #endif

    #endif  // end of else condition for #ifdef SFD_SPLIT

#else // else condition for #ifdef SFCD

    #ifdef SFD_SPLIT

        #define SFD_SIMPLE          hmi_static_frequent_static_default_simple_data
        #define SFD_MULTI_OP        hmi_static_frequent_static_default_multiple_operator_pages_data
        #define SFD_CUSTOM_DP       hmi_static_frequent_static_default_custom_DP_data
        #ifdef COMMON_TOP_WORKS_NEW_FEATURES
        #define SFD_SIMPLE_CTW      hmi_static_frequent_static_default_simple_CTW_data
        #define SFD_MULTI_OP_CTW    hmi_static_frequent_static_default_multiple_operator_pages_CTW_data
        #endif

    #else   // else condition for #ifdef SFD_SPLIT

        #define SFD_SIMPLE          hmi_static_frequent_static_default_data
        #define SFD_MULTI_OP        hmi_static_frequent_static_default_data
        #define SFD_CUSTOM_DP       hmi_static_frequent_static_default_data
        #ifdef COMMON_TOP_WORKS_NEW_FEATURES
        #define SFD_SIMPLE_CTW      hmi_static_frequent_static_default_CTW_data
        #define SFD_MULTI_OP_CTW    hmi_static_frequent_static_default_CTW_data
        #endif

    #endif  // end of else condition for #ifdef SFD_SPLIT

#endif  // end of else condition for #ifdef SFCD

#define hmi__SFD_SIMPLE                 SFD_SIMPLE
#define hmi__SFD_MULTI_OP               SFD_MULTI_OP
#define hmi__SFD_CUSTOM_DP              SFD_CUSTOM_DP
#define hmi__SFD_SIMPLE_CTW             SFD_SIMPLE_CTW
#define hmi__SFD_MULTI_OP_CTW           SFD_MULTI_OP_CTW
#define hmi_dynamic                     hmi_dynamic_data
 
