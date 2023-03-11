// M Ashford - Suppress MISRA Diagnostics
// MISRA Rule 31
// Braces shall be used to indicate and match the structure in the non-zero
// initialization of arrays and structures.
#pragma diag_suppress=Pm022

//----------------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2005.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Hmi
 Module
 Description    subsystem constructor
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//----------------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>
#include <intrinsics.h>
#include "IncludeList_Implementation.h"

#include "System/Interface/system.h"

#if(HOST_FRAMEWORK_VERSION < 0x020300)
#include "../../common/interface/common_type.h"
#include "../../common/interface/assert.h"
#else
#include "System/Interface/common_type.h"
#include "System/Interface/assert.h"
#endif
#include "RTOS/Interface/rtos.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
// include the data_obj-classes you need, e.g. T_DATA_OBJ::INT16
#include "T_DATA_OBJ/Interface/t_data_obj_int16.h"
#include "T_DATA_OBJ/Interface/t_data_obj_tabenum8.h"
#include "T_DATA_OBJ/Interface/t_data_obj_usign8.h"
#include "T_DATA_OBJ/Interface/t_data_obj_simple.h"
#include "T_DATA_OBJ/Interface/t_data_obj_action.h"


#include "T_Unit/Interface/t_unit.h"
#include "T_Unit/Interface/T_DATACLASSES.H"
#include "Hmi/Interface/HMI.h"
#include "hmi_types.h"
#include "hmi_attributes.h"
#include "hmi_overload.h"
#include "hmi_execute.h"

#include "IncludeList_Constructor.h"

//----------------------------------------------------------------------------------------------------------
//! unit global attributes
FAST TUSIGN16 hmiState;     // Subsystem state
FAST T_DATA_SEMAPHORE hmiDataSemaphore;



const SLOW T_DATACLASS hmiDataClassList[]=
{
/* Save SubGen templates for Entry tool, actually, the DataClass is depend on HMIGen Defines */
/*
    //lint -e{651} Initializer are OK, the warning pop up with the first 708
    //@SubGen start@    <DATACLASSLIST>
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_STATIC_NODEF(hmi__SFD_SIMPLE_data,hmi__SFD_SIMPLE_initializer),
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_STATIC_NODEF(hmi__SFD_MULTI_OP_data,hmi__SFD_MULTI_OP_initializer),
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_STATIC_NODEF(hmi__SFD_CUSTOM_DP_data,hmi__SFD_CUSTOM_DP_initializer),
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_STATIC_NODEF(hmi__SFD_SIMPLE_CTW_data,hmi__SFD_SIMPLE_CTW_initializer),
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_STATIC_NODEF(hmi__SFD_MULTI_OP_CTW_data,hmi__SFD_MULTI_OP_CTW_initializer),
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_DYNAMIC(hmi_dynamic_data,hmi_dynamic_initializer)
    //@SubGen end@    <DATACLASSLIST>
*/
  
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_DYNAMIC(hmi_dynamic_data,hmi_dynamic_initializer)
      
    #ifdef SFCD

        #ifdef SFD_SPLIT

            //lint -e{708} initialization is OK
            CONSTRUCTOR_DC_STATIC_CONSTDEF(hmi_static_frequent_constant_default_simple_data,
                                           hmi_static_frequent_constant_default_simple_initializer),
            //lint -e{708} initialization is OK
            CONSTRUCTOR_DC_STATIC_CONSTDEF(hmi_static_frequent_constant_default_multiple_operator_pages_data,
                                           hmi_static_frequent_constant_default_multiple_operator_pages_initializer),
            //lint -e{708} initialization is OK
            CONSTRUCTOR_DC_STATIC_CONSTDEF(hmi_static_frequent_constant_default_custom_DP_data,
                                           hmi_static_frequent_constant_default_custom_DP_initializer),

          #ifdef COMMON_TOP_WORKS_NEW_FEATURES
            //lint -e{708} initialization is OK
            CONSTRUCTOR_DC_STATIC_CONSTDEF(hmi_static_frequent_constant_default_simple_CTW_data,
                                           hmi_static_frequent_constant_default_simple_CTW_initializer),
            //lint -e{708} initialization is OK
            CONSTRUCTOR_DC_STATIC_CONSTDEF(hmi_static_frequent_constant_default_multiple_operator_pages_CTW_data,
                                           hmi_static_frequent_constant_default_multiple_operator_pages_CTW_initializer),
          #endif

        #else // else condition for #ifdef SFD_SPLIT

            //lint -e{708} initialization is OK
            CONSTRUCTOR_DC_STATIC_CONSTDEF(hmi_static_frequent_constant_default_data,
                                           hmi_static_frequent_constant_default_initializer),

          #ifdef COMMON_TOP_WORKS_NEW_FEATURES
            //lint -e{708} initialization is OK
            CONSTRUCTOR_DC_STATIC_CONSTDEF(hmi_static_frequent_constant_default_CTW_data,
                                           hmi_static_frequent_constant_default_CTW_initializer),
          #endif

        #endif // end of else condition for #ifdef SFD_SPLIT

    #else // else condition for #ifdef SFCD
        #ifdef SFD_SPLIT

            //lint -e{708} initialization is OK
            CONSTRUCTOR_DC_STATIC_STATDEF(hmi_static_frequent_static_default_simple_data,
                                          hmi_static_frequent_static_default_simple_initializer,
                                          hmi_static_frequent_static_default_simple_default),
            //lint -e{708} initialization is OK
            CONSTRUCTOR_DC_STATIC_STATDEF(hmi_static_frequent_static_default_multiple_operator_pages_data,
                                          hmi_static_frequent_static_default_multiple_operator_pages_initializer,
                                          hmi_static_frequent_static_default_multiple_operator_pages_default),
            //lint -e{708} initialization is OK
            CONSTRUCTOR_DC_STATIC_STATDEF(hmi_static_frequent_static_default_custom_DP_data,
                                          hmi_static_frequent_static_default_custom_DP_initializer,
                                          hmi_static_frequent_static_default_custom_DP_default),

          #ifdef COMMON_TOP_WORKS_NEW_FEATURES
            //lint -e{708} initialization is OK
            CONSTRUCTOR_DC_STATIC_STATDEF(hmi_static_frequent_static_default_simple_CTW_data,
                                          hmi_static_frequent_static_default_simple_CTW_initializer,
                                          hmi_static_frequent_static_default_simple_CTW_default),
            //lint -e{708} initialization is OK
            CONSTRUCTOR_DC_STATIC_STATDEF(hmi_static_frequent_static_default_multiple_operator_pages_CTW_data,
                                          hmi_static_frequent_static_default_multiple_operator_pages_CTW_initializer,
                                          hmi_static_frequent_static_default_multiple_operator_pages_CTW_default),
          #endif

        #else // else condition for #ifdef SFD_SPLIT

            //lint -e{708} initialization is OK
            ,CONSTRUCTOR_DC_STATIC_STATDEF(hmi_static_frequent_static_default_data,
                                          hmi_static_frequent_static_default_initializer,
                                          hmi_static_frequent_static_default_default),
          #ifdef COMMON_TOP_WORKS_NEW_FEATURES
            //lint -e{708} initialization is OK
            CONSTRUCTOR_DC_STATIC_STATDEF(hmi_static_frequent_static_default_CTW_data,
                                          hmi_static_frequent_static_default_CTW_initializer,
                                          hmi_static_frequent_static_default_CTW_default),
          #endif

        #endif // end of else condition for #ifdef SFD_SPLIT

    #endif  // end of else condition for #ifdef SFCD

};

/*
//@SubGen put@<DATACLASS>
//----------------------------------------------------------------------------------------------------------
//! definition of _SFD_MULTI_OP
#ifdef WIN32
  #pragma bss_seg(".AAA$HMI_DATA")
    const FAST T_HMI__SFD_MULTI_OP hmi__SFD_MULTI_OP_data;
  #pragma bss_seg()
#else
  #pragma location="HMI__SFD_MULTI_OP"
    const FAST __no_init T_HMI__SFD_MULTI_OP hmi__SFD_MULTI_OP_data;
#endif
const SLOW T_HMI__SFD_MULTI_OP hmi__SFD_MULTI_OP_initializer=
{
    //@SubGen start@    <INITIALIZER _SFD_MULTI_OP>
    PAGE_2_DEFAULT_LINE_1,    // TABENUM8 mainOperatorView_2_1
    PAGE_2_DEFAULT_LINE_2,    // TABENUM8 mainOperatorView_2_2
    PAGE_2_DEFAULT_LINE_3,    // TABENUM8 mainOperatorView_2_3
    PAGE_2_DEFAULT_LINE_B,    // TABENUM8 mainOperatorView_2_bargraph
    PAGE_3_DEFAULT_LINE_1,    // TABENUM8 mainOperatorView_3_1
    PAGE_3_DEFAULT_LINE_2,    // TABENUM8 mainOperatorView_3_2
    PAGE_3_DEFAULT_LINE_3,    // TABENUM8 mainOperatorView_3_3
    PAGE_3_DEFAULT_LINE_B,    // TABENUM8 mainOperatorView_3_bargraph
    PAGE_4_DEFAULT_LINE_1,    // TABENUM8 mainOperatorView_4_1
    PAGE_4_DEFAULT_LINE_2,    // TABENUM8 mainOperatorView_4_2
    PAGE_4_DEFAULT_LINE_3,    // TABENUM8 mainOperatorView_4_3
    PAGE_4_DEFAULT_LINE_B,    // TABENUM8 mainOperatorView_4_bargraph
    PAGE_2_DEFAULT_FORMAT,    // TABENUM8 displayMode_2
    PAGE_3_DEFAULT_FORMAT,    // TABENUM8 displayMode_3
    PAGE_4_DEFAULT_FORMAT,    // TABENUM8 displayMode_4
    DEFAULT_AUTOSCROLL_ENABLE    // TABENUM8 autoscrollEnable
    //@SubGen end@    <INITIALIZER _SFD_MULTI_OP>
};
//----------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------
//! definition of _SFD_CUSTOM_DP
#ifdef WIN32
  #pragma bss_seg(".AAA$HMI_DATA")
    const FAST T_HMI__SFD_CUSTOM_DP hmi__SFD_CUSTOM_DP_data;
  #pragma bss_seg()
#else
  #pragma location="HMI__SFD_CUSTOM_DP"
    const FAST __no_init T_HMI__SFD_CUSTOM_DP hmi__SFD_CUSTOM_DP_data;
#endif
const SLOW T_HMI__SFD_CUSTOM_DP hmi__SFD_CUSTOM_DP_initializer=
{
    //@SubGen start@    <INITIALIZER _SFD_CUSTOM_DP>
    DEFAULT_CUSTOM_DP_1,    // TABENUM8 customDP_1
    DEFAULT_CUSTOM_DP_2,    // TABENUM8 customDP_2
    DEFAULT_CUSTOM_DP_3,    // TABENUM8 customDP_3
    DEFAULT_CUSTOM_DP_4,    // TABENUM8 customDP_4
    DEFAULT_CUSTOM_DP_5,    // TABENUM8 customDP_5
    DEFAULT_CUSTOM_DP_6,    // TABENUM8 customDP_6
    DEFAULT_CUSTOM_DP_7,    // TABENUM8 customDP_7
    DEFAULT_CUSTOM_DP_8,    // TABENUM8 customDP_8
    DEFAULT_CUSTOM_DP_9,    // TABENUM8 customDP_9
    DEFAULT_CUSTOM_DP_10,    // TABENUM8 customDP_10
    DEFAULT_CUSTOM_DP_11    // TABENUM8 customDP_11
    //@SubGen end@    <INITIALIZER _SFD_CUSTOM_DP>
};
//----------------------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------------------
//! definition of _SFD_SIMPLE
#ifdef WIN32
  #pragma bss_seg(".AAA$HMI_DATA")
    const FAST T_HMI__SFD_SIMPLE hmi__SFD_SIMPLE_data;
  #pragma bss_seg()
#else
  #pragma location="HMI__SFD_SIMPLE"
    const FAST __no_init T_HMI__SFD_SIMPLE hmi__SFD_SIMPLE_data;
#endif
const SLOW T_HMI__SFD_SIMPLE hmi__SFD_SIMPLE_initializer=
{
    //@SubGen start@    <INITIALIZER _SFD_SIMPLE>
    DEFAULT_LANGUAGE,    // TABENUM8 language
    PAGE_1_DEFAULT_LINE_1,    // TABENUM8 mainOperatorView_1_1
    PAGE_1_DEFAULT_LINE_2,    // TABENUM8 mainOperatorView_1_2
    PAGE_1_DEFAULT_LINE_3,    // TABENUM8 mainOperatorView_1_3
    PAGE_1_DEFAULT_LINE_B,    // TABENUM8 mainOperatorView_1_bargraph
    50,    // USIGN8 contrast
    DEFAULT_LOCAL_OP_ENABLE,    // TABENUM8 localOperationEnable
    PAGE_1_DEFAULT_FORMAT,    // TABENUM8 displayMode_1
    HMI_DATE_FORMAT_YYYY_MM_DD    // TABENUM8 DateFormat
    //@SubGen end@    <INITIALIZER _SFD_SIMPLE>
};
//----------------------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------------------
//! definition of _SFD_SIMPLE_CTW
#ifdef WIN32
  #pragma bss_seg(".AAA$HMI_DATA")
    const FAST T_HMI__SFD_SIMPLE_CTW hmi__SFD_SIMPLE_CTW_data;
  #pragma bss_seg()
#else
  #pragma location="HMI__SFD_SIMPLE_CTW"
    const FAST __no_init T_HMI__SFD_SIMPLE_CTW hmi__SFD_SIMPLE_CTW_data;
#endif
const SLOW T_HMI__SFD_SIMPLE_CTW hmi__SFD_SIMPLE_CTW_initializer=
{
    //@SubGen start@    <INITIALIZER _SFD_SIMPLE_CTW>
    HMI_ROTATION_0,    // TABENUM8 Rotation
    ITEMS_PER_MENU,    // TABENUM8 MenuFormat
    PAGE_1_DEFAULT_LINE_4    // TABENUM8 mainOperatorView_1_4
    //@SubGen end@    <INITIALIZER _SFD_SIMPLE_CTW>
};
//----------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------
//! definition of _SFD_MULTI_OP_CTW
#ifdef WIN32
  #pragma bss_seg(".AAA$HMI_DATA")
    const FAST T_HMI__SFD_MULTI_OP_CTW hmi__SFD_MULTI_OP_CTW_data;
  #pragma bss_seg()
#else
  #pragma location="HMI__SFD_MULTI_OP_CTW"
    const FAST __no_init T_HMI__SFD_MULTI_OP_CTW hmi__SFD_MULTI_OP_CTW_data;
#endif
const SLOW T_HMI__SFD_MULTI_OP_CTW hmi__SFD_MULTI_OP_CTW_initializer=
{
    //@SubGen start@    <INITIALIZER _SFD_MULTI_OP_CTW>
    PAGE_5_DEFAULT_FORMAT,    // TABENUM8 displayMode_5
    PAGE_6_DEFAULT_FORMAT,    // TABENUM8 displayMode_6
    PAGE_5_DEFAULT_LINE_1,    // TABENUM8 mainOperatorView_5_1
    PAGE_5_DEFAULT_LINE_2,    // TABENUM8 mainOperatorView_5_2
    PAGE_5_DEFAULT_LINE_3,    // TABENUM8 mainOperatorView_5_3
    PAGE_5_DEFAULT_LINE_B,    // TABENUM8 mainOperatorView_5_bargraph
    PAGE_6_DEFAULT_LINE_1,    // TABENUM8 mainOperatorView_6_1
    PAGE_6_DEFAULT_LINE_2,    // TABENUM8 mainOperatorView_6_2
    PAGE_6_DEFAULT_LINE_3,    // TABENUM8 mainOperatorView_6_3
    PAGE_6_DEFAULT_LINE_B,    // TABENUM8 mainOperatorView_6_bargraph
    PAGE_2_DEFAULT_LINE_4,    // TABENUM8 mainOperatorView_2_4
    PAGE_3_DEFAULT_LINE_4,    // TABENUM8 mainOperatorView_3_4
    PAGE_4_DEFAULT_LINE_4,    // TABENUM8 mainOperatorView_4_4
    PAGE_5_DEFAULT_LINE_4,    // TABENUM8 mainOperatorView_5_4
    PAGE_6_DEFAULT_LINE_4    // TABENUM8 mainOperatorView_6_4
    //@SubGen end@    <INITIALIZER _SFD_MULTI_OP_CTW>
};
//----------------------------------------------------------------------------------------------------------
*/


//----------------------------------------------------------------------------------------------------------
//! definition of dynamic data
FAST T_HMI_DYNAMIC hmi_dynamic_data;
const SLOW T_HMI_DYNAMIC hmi_dynamic_initializer=
{
    //@SubGen start@    <INITIALIZER DYNAMIC>
    0,    // USIGN8 lowLevelSoftwareRevision
    50,    // USIGN8 defaultContrast
    DEFAULT_DIAGNOSIS_VIEW,    // TABENUM8 diagnosisView
    {0x00,0x00,0x00,0x00,0x00,0x00},    // SIMPLE_U8 Password[6]
    {0x00,0x00,0x00,0x00,0x00,0x00},    // SIMPLE_U8 SetPassword_StandardLevel[6]
    {0x00,0x00,0x00,0x00,0x00,0x00},    // SIMPLE_U8 SetPassword_AdvancedLevel[6]
    {0x00,0x00,0x00,0x00,0x00,0x00},    // SIMPLE_U8 SetPassword_ServiceLevel[6]
    {0x00,0x00,0x00,0x00,0x00,0x00},    // SIMPLE_U8 SetPassword_StandardLevelDefault[6]
    {0x00,0x00,0x00,0x00,0x00,0x00},    // SIMPLE_U8 SetPassword_AdvancedLevelDefault[6]
    {0x00,0x00,0x00,0x00,0x00,0x00},    // SIMPLE_U8 SetPassword_ServiceLevelDefault[6]
    0,    // SIMPLE_U8 displayTest
#ifdef COMMON_TOP_WORKS_NEW_FEATURES
    HMI_LANGUAGE_NOT_DEFINED    // TABENUM8 Customlanguage
#endif
    //@SubGen end@    <INITIALIZER DYNAMIC>
};
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
#ifdef SFCD
#ifdef SFD_SPLIT
//----------------------------------------------------------------------------------------------------------
//! definition of STATIC_FREQUENT_CONSTANT_DEFAULT_SIMPLE
#ifdef WIN32
    #pragma bss_seg(".AAA$HMI_DATA")
    const FAST              T_HMI_STATIC_FREQUENT_SIMPLE                        hmi_static_frequent_constant_default_simple_data;
    #pragma bss_seg()
#else
    #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT_SIMPLE"
    const FAST __no_init    T_HMI_STATIC_FREQUENT_SIMPLE                        hmi_static_frequent_constant_default_simple_data;
#endif
    const SLOW              T_HMI_STATIC_FREQUENT_SIMPLE                        hmi_static_frequent_constant_default_simple_initializer=
    {
         DEFAULT_LANGUAGE                   // TABENUM8 language
        ,PAGE_1_DEFAULT_LINE_1              // TABENUM8 mainOperatorView_1_1
        ,PAGE_1_DEFAULT_LINE_2              // TABENUM8 mainOperatorView_1_2
        ,PAGE_1_DEFAULT_LINE_3              // TABENUM8 mainOperatorView_1_3
        ,PAGE_1_DEFAULT_LINE_B              // TABENUM8 mainOperatorView_1_bargraph
        ,50                                 // USIGN8 contrast
        ,DEFAULT_LOCAL_OP_ENABLE            // TABENUM8 localOperationEnable
        ,PAGE_1_DEFAULT_FORMAT              // TABENUM8 displayMode_1
        ,HMI_DATE_FORMAT_YYYY_MM_DD         // TABENUM8 DateFormat
    };
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//! definition of STATIC_FREQUENT_CONSTANT_DEFAULT_SIMPLE_CTW
#ifdef WIN32
    #pragma bss_seg(".AAA$HMI_DATA")
    const FAST              T_HMI_STATIC_FREQUENT_SIMPLE_CTW                    hmi_static_frequent_constant_default_simple_CTW_data;
    #pragma bss_seg()
#else
    #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT_SIMPLE_CTW"
    const FAST __no_init    T_HMI_STATIC_FREQUENT_SIMPLE_CTW                    hmi_static_frequent_constant_default_simple_CTW_data;
#endif
    const SLOW              T_HMI_STATIC_FREQUENT_SIMPLE_CTW                    hmi_static_frequent_constant_default_simple_CTW_initializer=
    {
         HMI_ROTATION_0                     // TABENUM8 Rotation;
        ,ITEMS_PER_MENU                     // TABENUM8 MenuFormat;
        ,PAGE_1_DEFAULT_LINE_4              // TABENUM8 mainOperatorView_1_4
    };
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//! definition of STATIC_FREQUENT_CONSTANT_DEFAULT_MULTIPLE_OPERATOR_PAGES
#ifdef WIN32
    #pragma bss_seg(".AAA$HMI_DATA")
    const FAST              T_HMI_STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES       hmi_static_frequent_constant_default_multiple_operator_pages_data;
    #pragma bss_seg()
#else
    #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT_MULTIPLE_OPERATOR_PAGES"
    const FAST __no_init    T_HMI_STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES       hmi_static_frequent_constant_default_multiple_operator_pages_data;
#endif
    const SLOW              T_HMI_STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES       hmi_static_frequent_constant_default_multiple_operator_pages_initializer=
    {
         PAGE_2_DEFAULT_LINE_1              // TABENUM8 mainOperatorView_2_1
        ,PAGE_2_DEFAULT_LINE_2              // TABENUM8 mainOperatorView_2_2
        ,PAGE_2_DEFAULT_LINE_3              // TABENUM8 mainOperatorView_2_3
        ,PAGE_2_DEFAULT_LINE_B              // TABENUM8 mainOperatorView_2_bargraph
        ,PAGE_3_DEFAULT_LINE_1              // TABENUM8 mainOperatorView_3_1
        ,PAGE_3_DEFAULT_LINE_2              // TABENUM8 mainOperatorView_3_2
        ,PAGE_3_DEFAULT_LINE_3              // TABENUM8 mainOperatorView_3_3
        ,PAGE_3_DEFAULT_LINE_B              // TABENUM8 mainOperatorView_3_bargraph
        ,PAGE_4_DEFAULT_LINE_1              // TABENUM8 mainOperatorView_4_1
        ,PAGE_4_DEFAULT_LINE_2              // TABENUM8 mainOperatorView_4_2
        ,PAGE_4_DEFAULT_LINE_3              // TABENUM8 mainOperatorView_4_3
        ,PAGE_4_DEFAULT_LINE_B              // TABENUM8 mainOperatorView_4_bargraph
        ,PAGE_2_DEFAULT_FORMAT              // TABENUM8 displayMode_2
        ,PAGE_3_DEFAULT_FORMAT              // TABENUM8 displayMode_3
        ,PAGE_4_DEFAULT_FORMAT              // TABENUM8 displayMode_4
        ,DEFAULT_AUTOSCROLL_ENABLE          // TABENUM8 autoscrollEnable
    };
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//! definition of STATIC_FREQUENT_CONSTANT_DEFAULT_MULTIPLE_OPERATOR_PAGES_CTW
#ifdef WIN32
  #pragma bss_seg(".AAA$HMI_DATA")
    const FAST              T_HMI_STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES_CTW   hmi_static_frequent_constant_default_multiple_operator_pages_CTW_data;
  #pragma bss_seg()
#else
    #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT_MULTIPLE_OPERATOR_PAGES_CTW"
  const FAST __no_init    T_HMI_STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES_CTW   hmi_static_frequent_constant_default_multiple_operator_pages_CTW_data;
#endif
    const SLOW              T_HMI_STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES_CTW   hmi_static_frequent_constant_default_multiple_operator_pages_CTW_initializer=
    {
         PAGE_5_DEFAULT_FORMAT              // TABENUM8 displayMode_5;
        ,PAGE_6_DEFAULT_FORMAT              // TABENUM8 displayMode_6;
        ,PAGE_5_DEFAULT_LINE_1              // TABENUM8 mainOperatorView_5_1;
        ,PAGE_5_DEFAULT_LINE_2              // TABENUM8 mainOperatorView_5_2;
        ,PAGE_5_DEFAULT_LINE_3              // TABENUM8 mainOperatorView_5_3;
        ,PAGE_5_DEFAULT_LINE_B              // TABENUM8 mainOperatorView_5_bargraph;
        ,PAGE_6_DEFAULT_LINE_1              // TABENUM8 mainOperatorView_6_1;
        ,PAGE_6_DEFAULT_LINE_2              // TABENUM8 mainOperatorView_6_2;
        ,PAGE_6_DEFAULT_LINE_3              // TABENUM8 mainOperatorView_6_3;
        ,PAGE_6_DEFAULT_LINE_B              // TABENUM8 mainOperatorView_6_bargraph;
        ,PAGE_2_DEFAULT_LINE_4              // TABENUM8 mainOperatorView_2_4;
        ,PAGE_3_DEFAULT_LINE_4              // TABENUM8 mainOperatorView_3_4;
        ,PAGE_4_DEFAULT_LINE_4              // TABENUM8 mainOperatorView_4_4;
        ,PAGE_5_DEFAULT_LINE_4              // TABENUM8 mainOperatorView_5_4;
        ,PAGE_6_DEFAULT_LINE_4              // TABENUM8 mainOperatorView_6_4;
    };
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//! definition of STATIC_FREQUENT_CONSTANT_DEFAULT_CUSTOM_DP
#ifdef WIN32
    #pragma bss_seg(".AAA$HMI_DATA")
    const FAST              T_HMI_STATIC_FREQUENT_CUSTOM_DP                     hmi_static_frequent_constant_default_custom_DP_data;
    #pragma bss_seg()
#else
    #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT_CUSTOM_DP"
    const FAST __no_init    T_HMI_STATIC_FREQUENT_CUSTOM_DP                     hmi_static_frequent_constant_default_custom_DP_data;
#endif
    extern const SLOW       T_HMI_STATIC_FREQUENT_CUSTOM_DP                     hmi_static_frequent_constant_default_custom_DP_initializer;
    {
        DEFAULT_CUSTOM_DP_1,                // TABENUM8 customDP_1
        DEFAULT_CUSTOM_DP_2,                // TABENUM8 customDP_2
        DEFAULT_CUSTOM_DP_3,                // TABENUM8 customDP_3
        DEFAULT_CUSTOM_DP_4,                // TABENUM8 customDP_4
        DEFAULT_CUSTOM_DP_5,                // TABENUM8 customDP_5
        DEFAULT_CUSTOM_DP_6,                // TABENUM8 customDP_6
        DEFAULT_CUSTOM_DP_7,                // TABENUM8 customDP_7
        DEFAULT_CUSTOM_DP_8,                // TABENUM8 customDP_8
        DEFAULT_CUSTOM_DP_9,                // TABENUM8 customDP_9
        DEFAULT_CUSTOM_DP_10,               // TABENUM8 customDP_10
        DEFAULT_CUSTOM_DP_11                // TABENUM8 customDP_11
    };
//----------------------------------------------------------------------------------------------------------

#else // else condition for #ifdef SFD_SPLIT

//----------------------------------------------------------------------------------------------------------
//! definition of StaticFrequentConstantDefault
#ifdef WIN32
    #pragma bss_seg(".AAA$HMI_DATA")
    const FAST              T_HMI_STATIC_FREQUENT                               hmi_static_frequent_constant_default_data;
    #pragma bss_seg()
#else
    #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT"
    const FAST __no_init    T_HMI_STATIC_FREQUENT                               hmi_static_frequent_constant_default_data;
#endif
    const SLOW              T_HMI_STATIC_FREQUENT                               hmi_static_frequent_constant_default_initializer=
    {
         DEFAULT_LANGUAGE                   // TABENUM8 language
        ,PAGE_1_DEFAULT_LINE_1              // TABENUM8 mainOperatorView_1_1
        ,PAGE_1_DEFAULT_LINE_2              // TABENUM8 mainOperatorView_1_2
        ,PAGE_1_DEFAULT_LINE_3              // TABENUM8 mainOperatorView_1_3
        ,PAGE_1_DEFAULT_LINE_B              // TABENUM8 mainOperatorView_1_bargraph
        ,PAGE_2_DEFAULT_LINE_1              // TABENUM8 mainOperatorView_2_1
        ,PAGE_2_DEFAULT_LINE_2              // TABENUM8 mainOperatorView_2_2
        ,PAGE_2_DEFAULT_LINE_3              // TABENUM8 mainOperatorView_2_3
        ,PAGE_2_DEFAULT_LINE_B              // TABENUM8 mainOperatorView_2_bargraph
        ,PAGE_3_DEFAULT_LINE_1              // TABENUM8 mainOperatorView_3_1
        ,PAGE_3_DEFAULT_LINE_2              // TABENUM8 mainOperatorView_3_2
        ,PAGE_3_DEFAULT_LINE_3              // TABENUM8 mainOperatorView_3_3
        ,PAGE_3_DEFAULT_LINE_B              // TABENUM8 mainOperatorView_3_bargraph
        ,PAGE_4_DEFAULT_LINE_1              // TABENUM8 mainOperatorView_4_1
        ,PAGE_4_DEFAULT_LINE_2              // TABENUM8 mainOperatorView_4_2
        ,PAGE_4_DEFAULT_LINE_3              // TABENUM8 mainOperatorView_4_3
        ,PAGE_4_DEFAULT_LINE_B              // TABENUM8 mainOperatorView_4_bargraph
        ,50                                 // USIGN8 contrast
        ,DEFAULT_LOCAL_OP_ENABLE            // TABENUM8 localOperationEnable
        ,PAGE_1_DEFAULT_FORMAT              // TABENUM8 displayMode_1
        ,PAGE_2_DEFAULT_FORMAT              // TABENUM8 displayMode_2
        ,PAGE_3_DEFAULT_FORMAT              // TABENUM8 displayMode_3
        ,PAGE_4_DEFAULT_FORMAT              // TABENUM8 displayMode_4
        ,DEFAULT_AUTOSCROLL_ENABLE          // TABENUM8 autoscrollEnable
        ,DEFAULT_CUSTOM_DP_1                // TABENUM8 customDP_1
        ,DEFAULT_CUSTOM_DP_2                // TABENUM8 customDP_2
        ,DEFAULT_CUSTOM_DP_3                // TABENUM8 customDP_3
        ,DEFAULT_CUSTOM_DP_4                // TABENUM8 customDP_4
        ,DEFAULT_CUSTOM_DP_5                // TABENUM8 customDP_5
        ,DEFAULT_CUSTOM_DP_6                // TABENUM8 customDP_6
        ,DEFAULT_CUSTOM_DP_7                // TABENUM8 customDP_7
        ,DEFAULT_CUSTOM_DP_8                // TABENUM8 customDP_8
        ,DEFAULT_CUSTOM_DP_9                // TABENUM8 customDP_9
        ,DEFAULT_CUSTOM_DP_10               // TABENUM8 customDP_10
        ,DEFAULT_CUSTOM_DP_11               // TABENUM8 customDP_11
        ,HMI_DATE_FORMAT_YYYY_MM_DD          // TABENUM8 DateFormat
    };
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//! definition of StaticFrequentConstantDefault_CTW
#ifdef WIN32
    #pragma bss_seg(".AAA$HMI_DATA")
    const FAST              T_HMI_STATIC_FREQUENT_CTW                           hmi_static_frequent_constant_default_CTW_data;
    #pragma bss_seg()
#else
    #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT_CTW"
    const FAST __no_init    T_HMI_STATIC_FREQUENT_CTW                           hmi_static_frequent_constant_default_CTW_data;
#endif
    const SLOW              T_HMI_STATIC_FREQUENT_CTW                           hmi_static_frequent_constant_default_CTW_initializer=
    {
         HMI_ROTATION_0                     // TABENUM8 Rotation;
        ,ITEMS_PER_MENU                     // TABENUM8 MenuFormat;
        ,PAGE_5_DEFAULT_FORMAT              // TABENUM8 displayMode_5;
        ,PAGE_6_DEFAULT_FORMAT              // TABENUM8 displayMode_6;
        ,PAGE_5_DEFAULT_LINE_1              // TABENUM8 mainOperatorView_5_1;
        ,PAGE_5_DEFAULT_LINE_2              // TABENUM8 mainOperatorView_5_2;
        ,PAGE_5_DEFAULT_LINE_3              // TABENUM8 mainOperatorView_5_3;
        ,PAGE_5_DEFAULT_LINE_B              // TABENUM8 mainOperatorView_5_bargraph;
        ,PAGE_6_DEFAULT_LINE_1              // TABENUM8 mainOperatorView_6_1;
        ,PAGE_6_DEFAULT_LINE_2              // TABENUM8 mainOperatorView_6_2;
        ,PAGE_6_DEFAULT_LINE_3              // TABENUM8 mainOperatorView_6_3;
        ,PAGE_6_DEFAULT_LINE_B              // TABENUM8 mainOperatorView_6_bargraph;
        ,PAGE_1_DEFAULT_LINE_4              // TABENUM8 mainOperatorView_1_4;
        ,PAGE_2_DEFAULT_LINE_4              // TABENUM8 mainOperatorView_2_4;
        ,PAGE_3_DEFAULT_LINE_4              // TABENUM8 mainOperatorView_3_4;
        ,PAGE_4_DEFAULT_LINE_4              // TABENUM8 mainOperatorView_4_4;
        ,PAGE_5_DEFAULT_LINE_4              // TABENUM8 mainOperatorView_5_4;
        ,PAGE_6_DEFAULT_LINE_4              // TABENUM8 mainOperatorView_6_4;
    };
//----------------------------------------------------------------------------------------------------------
#endif  // end of else condition for #ifdef SFD_SPLIT

#else // else condition for #ifdef SFCD

#ifdef SFD_SPLIT
//----------------------------------------------------------------------------------------------------------
//! definition of STATIC_FREQUENT_STATIC_DEFAULT_SIMPLE
#ifdef WIN32
    #pragma bss_seg(".AAA$HMI_DATA")
    const FAST               T_HMI_STATIC_FREQUENT_SIMPLE                        hmi_static_frequent_static_default_simple_data;
    const FAST               T_HMI_STATIC_FREQUENT_SIMPLE                        hmi_static_frequent_static_default_simple_default;
    #pragma bss_seg()
#else
    #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT_SIMPLE"
    const FAST __no_init     T_HMI_STATIC_FREQUENT_SIMPLE                        hmi_static_frequent_static_default_simple_data;
    #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT_SIMPLE"
    const FAST __no_init     T_HMI_STATIC_FREQUENT_SIMPLE                        hmi_static_frequent_static_default_simple_default;
#endif
    const SLOW               T_HMI_STATIC_FREQUENT_SIMPLE                        hmi_static_frequent_static_default_simple_initializer=
    {
         DEFAULT_LANGUAGE                   // TABENUM8 language
        ,PAGE_1_DEFAULT_LINE_1              // TABENUM8 mainOperatorView_1_1
        ,PAGE_1_DEFAULT_LINE_2              // TABENUM8 mainOperatorView_1_2
        ,PAGE_1_DEFAULT_LINE_3              // TABENUM8 mainOperatorView_1_3
        ,PAGE_1_DEFAULT_LINE_B              // TABENUM8 mainOperatorView_1_bargraph
        ,50                                 // USIGN8 contrast
        ,DEFAULT_LOCAL_OP_ENABLE            // TABENUM8 localOperationEnable
        ,PAGE_1_DEFAULT_FORMAT              // TABENUM8 displayMode_1
        ,HMI_DATE_FORMAT_YYYY_MM_DD         // TABENUM8 DateFormat
    };
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//! definition of STATIC_FREQUENT_STATIC_DEFAULT_SIMPLE_CTW
#ifdef WIN32
    #pragma bss_seg(".AAA$HMI_DATA")
    const FAST               T_HMI_STATIC_FREQUENT_SIMPLE_CTW                    hmi_static_frequent_static_default_simple_CTW_data;
    const FAST               T_HMI_STATIC_FREQUENT_SIMPLE_CTW                    hmi_static_frequent_static_default_simple_CTW_default;
    #pragma bss_seg()
#else
    #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT_SIMPLE_CTW"
    const FAST __no_init     T_HMI_STATIC_FREQUENT_SIMPLE_CTW                    hmi_static_frequent_static_default_simple_CTW_data;
    #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT_SIMPLE_CTW"
    const FAST __no_init     T_HMI_STATIC_FREQUENT_SIMPLE_CTW                    hmi_static_frequent_static_default_simple_CTW_default;
#endif
    const SLOW               T_HMI_STATIC_FREQUENT_SIMPLE_CTW                    hmi_static_frequent_static_default_simple_CTW_initializer=
    {
         HMI_ROTATION_0                     // TABENUM8 Rotation;
        ,ITEMS_PER_MENU                     // TABENUM8 MenuFormat;
        ,PAGE_1_DEFAULT_LINE_4              // TABENUM8 mainOperatorView_1_4
    };
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//! definition of STATIC_FREQUENT_STATIC_DEFAULT_MULTIPLE_OPERATOR_PAGES
#ifdef WIN32
  #pragma bss_seg(".AAA$HMI_DATA")
    const FAST               T_HMI_STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES       hmi_static_frequent_static_default_multiple_operator_pages_data;
    const FAST               T_HMI_STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES       hmi_static_frequent_static_default_multiple_operator_pages_default;
  #pragma bss_seg()
#else
    #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT_MULTIPLE_OPERATOR_PAGES"
    const FAST __no_init     T_HMI_STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES       hmi_static_frequent_static_default_multiple_operator_pages_data;
    #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT_MULTIPLE_OPERATOR_PAGES"
    const FAST __no_init     T_HMI_STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES       hmi_static_frequent_static_default_multiple_operator_pages_default;
#endif
    const SLOW               T_HMI_STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES       hmi_static_frequent_static_default_multiple_operator_pages_initializer=
    {
         PAGE_2_DEFAULT_LINE_1              // TABENUM8 mainOperatorView_2_1
        ,PAGE_2_DEFAULT_LINE_2              // TABENUM8 mainOperatorView_2_2
        ,PAGE_2_DEFAULT_LINE_3              // TABENUM8 mainOperatorView_2_3
        ,PAGE_2_DEFAULT_LINE_B              // TABENUM8 mainOperatorView_2_bargraph
        ,PAGE_3_DEFAULT_LINE_1              // TABENUM8 mainOperatorView_3_1
        ,PAGE_3_DEFAULT_LINE_2              // TABENUM8 mainOperatorView_3_2
        ,PAGE_3_DEFAULT_LINE_3              // TABENUM8 mainOperatorView_3_3
        ,PAGE_3_DEFAULT_LINE_B              // TABENUM8 mainOperatorView_3_bargraph
        ,PAGE_4_DEFAULT_LINE_1              // TABENUM8 mainOperatorView_4_1
        ,PAGE_4_DEFAULT_LINE_2              // TABENUM8 mainOperatorView_4_2
        ,PAGE_4_DEFAULT_LINE_3              // TABENUM8 mainOperatorView_4_3
        ,PAGE_4_DEFAULT_LINE_B              // TABENUM8 mainOperatorView_4_bargraph
        ,PAGE_2_DEFAULT_FORMAT              // TABENUM8 displayMode_2
        ,PAGE_3_DEFAULT_FORMAT              // TABENUM8 displayMode_3
        ,PAGE_4_DEFAULT_FORMAT              // TABENUM8 displayMode_4
        ,DEFAULT_AUTOSCROLL_ENABLE          // TABENUM8 autoscrollEnable
    };
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//! definition of STATIC_FREQUENT_STATIC_DEFAULT_MULTIPLE_OPERATOR_PAGES_CTW
#ifdef WIN32
  #pragma bss_seg(".AAA$HMI_DATA")
    const FAST               T_HMI_STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES_CTW   hmi_static_frequent_static_default_multiple_operator_pages_CTW_data;
    const FAST               T_HMI_STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES_CTW   hmi_static_frequent_static_default_multiple_operator_pages_CTW_default;
  #pragma bss_seg()
#else
    #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT_MULTIPLE_OPERATOR_PAGES_CTW"
    const FAST __no_init     T_HMI_STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES_CTW   hmi_static_frequent_static_default_multiple_operator_pages_CTW_data;
    #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT_MULTIPLE_OPERATOR_PAGES_CTW"
    const FAST __no_init     T_HMI_STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES_CTW   hmi_static_frequent_static_default_multiple_operator_pages_CTW_default;
#endif
    const SLOW               T_HMI_STATIC_FREQUENT_MULTIPLE_OPERATOR_PAGES_CTW   hmi_static_frequent_static_default_multiple_operator_pages_CTW_initializer=
    {
         PAGE_5_DEFAULT_FORMAT              // TABENUM8 displayMode_5;
        ,PAGE_6_DEFAULT_FORMAT              // TABENUM8 displayMode_6;
        ,PAGE_5_DEFAULT_LINE_1              // TABENUM8 mainOperatorView_5_1;
        ,PAGE_5_DEFAULT_LINE_2              // TABENUM8 mainOperatorView_5_2;
        ,PAGE_5_DEFAULT_LINE_3              // TABENUM8 mainOperatorView_5_3;
        ,PAGE_5_DEFAULT_LINE_B              // TABENUM8 mainOperatorView_5_bargraph;
        ,PAGE_6_DEFAULT_LINE_1              // TABENUM8 mainOperatorView_6_1;
        ,PAGE_6_DEFAULT_LINE_2              // TABENUM8 mainOperatorView_6_2;
        ,PAGE_6_DEFAULT_LINE_3              // TABENUM8 mainOperatorView_6_3;
        ,PAGE_6_DEFAULT_LINE_B              // TABENUM8 mainOperatorView_6_bargraph;
        ,PAGE_2_DEFAULT_LINE_4              // TABENUM8 mainOperatorView_2_4;
        ,PAGE_3_DEFAULT_LINE_4              // TABENUM8 mainOperatorView_3_4;
        ,PAGE_4_DEFAULT_LINE_4              // TABENUM8 mainOperatorView_4_4;
        ,PAGE_5_DEFAULT_LINE_4              // TABENUM8 mainOperatorView_5_4;
        ,PAGE_6_DEFAULT_LINE_4              // TABENUM8 mainOperatorView_6_4;
    };
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//! definition of STATIC_FREQUENT_STATIC_DEFAULT_CUSTOM_DP
#ifdef WIN32
  #pragma bss_seg(".AAA$HMI_DATA")
    const FAST               T_HMI_STATIC_FREQUENT_CUSTOM_DP                     hmi_static_frequent_static_default_custom_DP_data;
    const FAST               T_HMI_STATIC_FREQUENT_CUSTOM_DP                     hmi_static_frequent_static_default_custom_DP_default;
  #pragma bss_seg()
#else
    #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT_CUSTOM_DP"
    const FAST __no_init     T_HMI_STATIC_FREQUENT_CUSTOM_DP                     hmi_static_frequent_static_default_custom_DP_data;
    #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT_CUSTOM_DP"
    const FAST __no_init     T_HMI_STATIC_FREQUENT_CUSTOM_DP                     hmi_static_frequent_static_default_custom_DP_default;
#endif
    const SLOW               T_HMI_STATIC_FREQUENT_CUSTOM_DP                     hmi_static_frequent_static_default_custom_DP_initializer=
    {
        DEFAULT_CUSTOM_DP_1,                // TABENUM8 customDP_1
        DEFAULT_CUSTOM_DP_2,                // TABENUM8 customDP_2
        DEFAULT_CUSTOM_DP_3,                // TABENUM8 customDP_3
        DEFAULT_CUSTOM_DP_4,                // TABENUM8 customDP_4
        DEFAULT_CUSTOM_DP_5,                // TABENUM8 customDP_5
        DEFAULT_CUSTOM_DP_6,                // TABENUM8 customDP_6
        DEFAULT_CUSTOM_DP_7,                // TABENUM8 customDP_7
        DEFAULT_CUSTOM_DP_8,                // TABENUM8 customDP_8
        DEFAULT_CUSTOM_DP_9,                // TABENUM8 customDP_9
        DEFAULT_CUSTOM_DP_10,               // TABENUM8 customDP_10
        DEFAULT_CUSTOM_DP_11                // TABENUM8 customDP_11
    };
//----------------------------------------------------------------------------------------------------------

#else // else condition for #ifdef SFD_SPLIT

//----------------------------------------------------------------------------------------------------------
//! definition of StaticFrequentStaticDefault
#ifdef WIN32
  #pragma bss_seg(".AAA$HMI_DATA")
    const FAST               T_HMI_STATIC_FREQUENT                               hmi_static_frequent_static_default_data;
    const FAST               T_HMI_STATIC_FREQUENT                               hmi_static_frequent_static_default_default;
  #pragma bss_seg()
#else
  #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT"
    const FAST __no_init     T_HMI_STATIC_FREQUENT                               hmi_static_frequent_static_default_data;
  #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT"
    const FAST __no_init     T_HMI_STATIC_FREQUENT                               hmi_static_frequent_static_default_default;
#endif
    const SLOW               T_HMI_STATIC_FREQUENT                               hmi_static_frequent_static_default_initializer=
    {
        DEFAULT_LANGUAGE,    // TABENUM8 language
        PAGE_1_DEFAULT_LINE_1,    // TABENUM8 mainOperatorView_1_1
        PAGE_1_DEFAULT_LINE_2,    // TABENUM8 mainOperatorView_1_2
        PAGE_1_DEFAULT_LINE_3,    // TABENUM8 mainOperatorView_1_3
        PAGE_1_DEFAULT_LINE_B,    // TABENUM8 mainOperatorView_1_bargraph
        PAGE_2_DEFAULT_LINE_1,    // TABENUM8 mainOperatorView_2_1
        PAGE_2_DEFAULT_LINE_2,    // TABENUM8 mainOperatorView_2_2
        PAGE_2_DEFAULT_LINE_3,    // TABENUM8 mainOperatorView_2_3
        PAGE_2_DEFAULT_LINE_B,    // TABENUM8 mainOperatorView_2_bargraph
        PAGE_3_DEFAULT_LINE_1,    // TABENUM8 mainOperatorView_3_1
        PAGE_3_DEFAULT_LINE_2,    // TABENUM8 mainOperatorView_3_2
        PAGE_3_DEFAULT_LINE_3,    // TABENUM8 mainOperatorView_3_3
        PAGE_3_DEFAULT_LINE_B,    // TABENUM8 mainOperatorView_3_bargraph
        PAGE_4_DEFAULT_LINE_1,    // TABENUM8 mainOperatorView_4_1
        PAGE_4_DEFAULT_LINE_2,    // TABENUM8 mainOperatorView_4_2
        PAGE_4_DEFAULT_LINE_3,    // TABENUM8 mainOperatorView_4_3
        PAGE_4_DEFAULT_LINE_B,    // TABENUM8 mainOperatorView_4_bargraph
        50,    // USIGN8 contrast
        DEFAULT_LOCAL_OP_ENABLE,    // TABENUM8 localOperationEnable
        PAGE_1_DEFAULT_FORMAT,    // TABENUM8 displayMode_1
        PAGE_2_DEFAULT_FORMAT,    // TABENUM8 displayMode_2
        PAGE_3_DEFAULT_FORMAT,    // TABENUM8 displayMode_3
        PAGE_4_DEFAULT_FORMAT,    // TABENUM8 displayMode_4
        DEFAULT_AUTOSCROLL_ENABLE,    // TABENUM8 autoscrollEnable
        DEFAULT_CUSTOM_DP_1,    // TABENUM8 customDP_1
        DEFAULT_CUSTOM_DP_2,    // TABENUM8 customDP_2
        DEFAULT_CUSTOM_DP_3,    // TABENUM8 customDP_3
        DEFAULT_CUSTOM_DP_4,    // TABENUM8 customDP_4
        DEFAULT_CUSTOM_DP_5,    // TABENUM8 customDP_5
        DEFAULT_CUSTOM_DP_6,    // TABENUM8 customDP_6
        DEFAULT_CUSTOM_DP_7,    // TABENUM8 customDP_7
        DEFAULT_CUSTOM_DP_8,    // TABENUM8 customDP_8
        DEFAULT_CUSTOM_DP_9,    // TABENUM8 customDP_9
        DEFAULT_CUSTOM_DP_10,    // TABENUM8 customDP_10
        DEFAULT_CUSTOM_DP_11,    // TABENUM8 customDP_11
        HMI_DATE_FORMAT_YYYY_MM_DD    // TABENUM8 DateFormat
    };
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//! definition of StaticFrequentStaticDefault_CTW
#ifdef WIN32
  #pragma bss_seg(".AAA$HMI_DATA")
    const FAST               T_HMI_STATIC_FREQUENT_CTW                           hmi_static_frequent_static_default_CTW_data;
    const FAST               T_HMI_STATIC_FREQUENT_CTW                           hmi_static_frequent_static_default_CTW_default;
  #pragma bss_seg()
#else
    #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT_CTW"
    const FAST __no_init     T_HMI_STATIC_FREQUENT_CTW                           hmi_static_frequent_static_default_CTW_data;
    #pragma location="HMI_STATIC_FREQUENT_STATIC_DEFAULT_CTW"
    const FAST __no_init     T_HMI_STATIC_FREQUENT_CTW                           hmi_static_frequent_static_default_CTW_default;
#endif
    const SLOW               T_HMI_STATIC_FREQUENT_CTW                           hmi_static_frequent_static_default_CTW_initializer=
    {
         HMI_ROTATION_0                     // TABENUM8 Rotation;
        ,ITEMS_PER_MENU                     // TABENUM8 MenuFormat;
        ,PAGE_5_DEFAULT_FORMAT              // TABENUM8 displayMode_5;
        ,PAGE_6_DEFAULT_FORMAT              // TABENUM8 displayMode_6;
        ,PAGE_5_DEFAULT_LINE_1              // TABENUM8 mainOperatorView_5_1;
        ,PAGE_5_DEFAULT_LINE_2              // TABENUM8 mainOperatorView_5_2;
        ,PAGE_5_DEFAULT_LINE_3              // TABENUM8 mainOperatorView_5_3;
        ,PAGE_5_DEFAULT_LINE_B              // TABENUM8 mainOperatorView_5_bargraph;
        ,PAGE_6_DEFAULT_LINE_1              // TABENUM8 mainOperatorView_6_1;
        ,PAGE_6_DEFAULT_LINE_2              // TABENUM8 mainOperatorView_6_2;
        ,PAGE_6_DEFAULT_LINE_3              // TABENUM8 mainOperatorView_6_3;
        ,PAGE_6_DEFAULT_LINE_B              // TABENUM8 mainOperatorView_6_bargraph;
        ,PAGE_1_DEFAULT_LINE_4              // TABENUM8 mainOperatorView_1_4;
        ,PAGE_2_DEFAULT_LINE_4              // TABENUM8 mainOperatorView_2_4;
        ,PAGE_3_DEFAULT_LINE_4              // TABENUM8 mainOperatorView_3_4;
        ,PAGE_4_DEFAULT_LINE_4              // TABENUM8 mainOperatorView_4_4;
        ,PAGE_5_DEFAULT_LINE_4              // TABENUM8 mainOperatorView_5_4;
        ,PAGE_6_DEFAULT_LINE_4              // TABENUM8 mainOperatorView_6_4;
    };

//----------------------------------------------------------------------------------------------------------

    #endif // end of else condition for #ifdef SFD_SPLIT

#endif  // end of else condition for #ifdef SFCD



//----------------------------------------------------------------------------------------------------------
//! object check and PutGet descriptions
//@SubGen start@<DESCRIPTORS>
const SLOW TUSIGN8 _LOCAL_OP[] =
// Table referenced by: localOperationEnable
{
  HMI_LOCAL_OP_DISABLED,
  HMI_LOCAL_OP_ENABLED
};

const SLOW TUSIGN8 _AUTOSCROLL[] =
// Table referenced by: autoscrollEnable
{
  HMI_AUTOSCROLL_DISABLED,
  HMI_AUTOSCROLL_ENABLED
};

const SLOW TUSIGN8 _DATE_FORMAT[] =
// Table referenced by: DateFormat
{
  HMI_DATE_FORMAT_DD_MM_YYYY,
  HMI_DATE_FORMAT_MM_DD_YYYY,
  HMI_DATE_FORMAT_YYYY_MM_DD
};

const SLOW TUSIGN8 _ROTATION[] =
// Table referenced by: Rotation
{
  HMI_ROTATION_0,
  HMI_ROTATION_180
};

const SLOW TUSIGN8 _MENU_FORMAT[] =
// Table referenced by: MenuFormat
{
  HMI_MENU_FORMAT_3_LINES,
  HMI_MENU_FORMAT_4_LINES,
  HMI_MENU_FORMAT_5_LINES,
  HMI_MENU_FORMAT_6_LINES
};

const SLOW TUSIGN8 LANGUAGE_OPTIONS[] =
// Table referenced by: language
{
  HMI__LANGUAGE_OPTIONS_LIST
};

const SLOW TUSIGN8 CUSTOM_LANGUAGE_OPTIONS[] =
// Table referenced by: Customlanguage
{
  HMI__CUSTOM_LANGUAGE_OPTIONS_LIST
};

const SLOW TUSIGN8 PAGE_1_FORMAT_OPTIONS[] =
// Table referenced by: displayMode_1
{
  HMI__PAGE_1_FORMAT_OPTIONS_LIST
};

const SLOW TUSIGN8 PAGE_1_LINE_1_OPTIONS[] =
// Table referenced by: mainOperatorView_1_1
{
  HMI__PAGE_1_LINE_1_OPTIONS_LIST
};

const SLOW TUSIGN8 PAGE_1_LINE_2_OPTIONS[] =
// Table referenced by: mainOperatorView_1_2
{
  HMI__PAGE_1_LINE_2_OPTIONS_LIST
};

const SLOW TUSIGN8 PAGE_1_LINE_3_OPTIONS[] =
// Table referenced by: mainOperatorView_1_3
{
  HMI__PAGE_1_LINE_3_OPTIONS_LIST
};

const SLOW TUSIGN8 PAGE_1_LINE_4_OPTIONS[] =
// Table referenced by: mainOperatorView_1_4
{
  HMI__PAGE_1_LINE_4_OPTIONS_LIST
};

const SLOW TUSIGN8 PAGE_1_LINE_B_OPTIONS[] =
// Table referenced by: mainOperatorView_1_bargraph
{
  HMI__PAGE_1_LINE_B_OPTIONS_LIST
};

const SLOW TUSIGN8 PAGE_2_FORMAT_OPTIONS[] =
// Table referenced by: displayMode_2
{
  HMI__PAGE_2_FORMAT_OPTIONS_LIST
};

const SLOW TUSIGN8 PAGE_2_LINE_1_OPTIONS[] =
// Table referenced by: mainOperatorView_2_1
{
  HMI__PAGE_2_LINE_1_OPTIONS_LIST
};

const SLOW TUSIGN8 PAGE_2_LINE_2_OPTIONS[] =
// Table referenced by: mainOperatorView_2_2
{
  HMI__PAGE_2_LINE_2_OPTIONS_LIST
};

const SLOW TUSIGN8 PAGE_2_LINE_3_OPTIONS[] =
// Table referenced by: mainOperatorView_2_3
{
  HMI__PAGE_2_LINE_3_OPTIONS_LIST
};

const SLOW TUSIGN8 PAGE_2_LINE_4_OPTIONS[] =
// Table referenced by: mainOperatorView_2_4
{
  HMI__PAGE_2_LINE_4_OPTIONS_LIST
};

const SLOW TUSIGN8 PAGE_2_LINE_B_OPTIONS[] =
// Table referenced by: mainOperatorView_2_bargraph
{
  HMI__PAGE_2_LINE_B_OPTIONS_LIST
};

const SLOW TUSIGN8 PAGE_3_FORMAT_OPTIONS[] =
// Table referenced by: displayMode_3
{
  HMI__PAGE_3_FORMAT_OPTIONS_LIST
};

const SLOW TUSIGN8 PAGE_3_LINE_1_OPTIONS[] =
// Table referenced by: mainOperatorView_3_1
{
  HMI__PAGE_3_LINE_1_OPTIONS_LIST
};

const SLOW TUSIGN8 PAGE_3_LINE_2_OPTIONS[] =
// Table referenced by: mainOperatorView_3_2
{
  HMI__PAGE_3_LINE_2_OPTIONS_LIST
};

const SLOW TUSIGN8 PAGE_3_LINE_3_OPTIONS[] =
// Table referenced by: mainOperatorView_3_3
{
  HMI__PAGE_3_LINE_3_OPTIONS_LIST
};

const SLOW TUSIGN8 PAGE_3_LINE_4_OPTIONS[] =
// Table referenced by: mainOperatorView_3_4
{
  HMI__PAGE_3_LINE_4_OPTIONS_LIST
};

const SLOW TUSIGN8 PAGE_3_LINE_B_OPTIONS[] =
// Table referenced by: mainOperatorView_3_bargraph
{
  HMI__PAGE_3_LINE_B_OPTIONS_LIST
};

const SLOW TUSIGN8 PAGE_4_FORMAT_OPTIONS[] =
// Table referenced by: displayMode_4
{
  HMI__PAGE_4_FORMAT_OPTIONS_LIST
};

const SLOW TUSIGN8 PAGE_4_LINE_1_OPTIONS[] =
// Table referenced by: mainOperatorView_4_1
{
  HMI__PAGE_4_LINE_1_OPTIONS_LIST
};

const SLOW TUSIGN8 PAGE_4_LINE_2_OPTIONS[] =
// Table referenced by: mainOperatorView_4_2
{
  HMI__PAGE_4_LINE_2_OPTIONS_LIST
};

const SLOW TUSIGN8 PAGE_4_LINE_3_OPTIONS[] =
// Table referenced by: mainOperatorView_4_3
{
  HMI__PAGE_4_LINE_3_OPTIONS_LIST
};

const SLOW TUSIGN8 PAGE_4_LINE_4_OPTIONS[] =
// Table referenced by: mainOperatorView_4_4
{
  HMI__PAGE_4_LINE_4_OPTIONS_LIST
};

const SLOW TUSIGN8 PAGE_4_LINE_B_OPTIONS[] =
// Table referenced by: mainOperatorView_4_bargraph
{
  HMI__PAGE_4_LINE_B_OPTIONS_LIST
};

const SLOW TUSIGN8 PAGE_5_FORMAT_OPTIONS[] =
// Table referenced by: displayMode_5
{
  HMI__PAGE_5_FORMAT_OPTIONS_LIST
};

const SLOW TUSIGN8 PAGE_5_LINE_1_OPTIONS[] =
// Table referenced by: mainOperatorView_5_1
{
  HMI__PAGE_5_LINE_1_OPTIONS_LIST
};

const SLOW TUSIGN8 PAGE_5_LINE_2_OPTIONS[] =
// Table referenced by: mainOperatorView_5_2
{
  HMI__PAGE_5_LINE_2_OPTIONS_LIST
};

const SLOW TUSIGN8 PAGE_5_LINE_3_OPTIONS[] =
// Table referenced by: mainOperatorView_5_3
{
  HMI__PAGE_5_LINE_3_OPTIONS_LIST
};

const SLOW TUSIGN8 PAGE_5_LINE_4_OPTIONS[] =
// Table referenced by: mainOperatorView_5_4
{
  HMI__PAGE_5_LINE_4_OPTIONS_LIST
};

const SLOW TUSIGN8 PAGE_5_LINE_B_OPTIONS[] =
// Table referenced by: mainOperatorView_5_bargraph
{
  HMI__PAGE_5_LINE_B_OPTIONS_LIST
};

const SLOW TUSIGN8 PAGE_6_FORMAT_OPTIONS[] =
// Table referenced by: displayMode_6
{
  HMI__PAGE_6_FORMAT_OPTIONS_LIST
};

const SLOW TUSIGN8 PAGE_6_LINE_1_OPTIONS[] =
// Table referenced by: mainOperatorView_6_1
{
  HMI__PAGE_6_LINE_1_OPTIONS_LIST
};

const SLOW TUSIGN8 PAGE_6_LINE_2_OPTIONS[] =
// Table referenced by: mainOperatorView_6_2
{
  HMI__PAGE_6_LINE_2_OPTIONS_LIST
};

const SLOW TUSIGN8 PAGE_6_LINE_3_OPTIONS[] =
// Table referenced by: mainOperatorView_6_3
{
  HMI__PAGE_6_LINE_3_OPTIONS_LIST
};

const SLOW TUSIGN8 PAGE_6_LINE_4_OPTIONS[] =
// Table referenced by: mainOperatorView_6_4
{
  HMI__PAGE_6_LINE_4_OPTIONS_LIST
};

const SLOW TUSIGN8 PAGE_6_LINE_B_OPTIONS[] =
// Table referenced by: mainOperatorView_6_bargraph
{
  HMI__PAGE_6_LINE_B_OPTIONS_LIST
};

const SLOW TUSIGN8 DIAGNOSIS_VIEW_OPTIONS[] =
// Table referenced by: diagnosisView
{
  HMI__DIAGNOSIS_VIEW_OPTIONS_LIST
};

const SLOW TUSIGN8 CUSTOM_DP_1_OPTIONS[] =
// Table referenced by: customDP_1
{
  HMI__CUSTOM_DP_1_OPTIONS_LIST
};

const SLOW TUSIGN8 CUSTOM_DP_2_OPTIONS[] =
// Table referenced by: customDP_2
{
  HMI__CUSTOM_DP_2_OPTIONS_LIST
};

const SLOW TUSIGN8 CUSTOM_DP_3_OPTIONS[] =
// Table referenced by: customDP_3
{
  HMI__CUSTOM_DP_3_OPTIONS_LIST
};

const SLOW TUSIGN8 CUSTOM_DP_4_OPTIONS[] =
// Table referenced by: customDP_4
{
  HMI__CUSTOM_DP_4_OPTIONS_LIST
};

const SLOW TUSIGN8 CUSTOM_DP_5_OPTIONS[] =
// Table referenced by: customDP_5
{
  HMI__CUSTOM_DP_5_OPTIONS_LIST
};

const SLOW TUSIGN8 CUSTOM_DP_6_OPTIONS[] =
// Table referenced by: customDP_6
{
  HMI__CUSTOM_DP_6_OPTIONS_LIST
};

const SLOW TUSIGN8 CUSTOM_DP_7_OPTIONS[] =
// Table referenced by: customDP_7
{
  HMI__CUSTOM_DP_7_OPTIONS_LIST
};

const SLOW TUSIGN8 CUSTOM_DP_8_OPTIONS[] =
// Table referenced by: customDP_8
{
  HMI__CUSTOM_DP_8_OPTIONS_LIST
};

const SLOW TUSIGN8 CUSTOM_DP_9_OPTIONS[] =
// Table referenced by: customDP_9
{
  HMI__CUSTOM_DP_9_OPTIONS_LIST
};

const SLOW TUSIGN8 CUSTOM_DP_10_OPTIONS[] =
// Table referenced by: customDP_10
{
  HMI__CUSTOM_DP_10_OPTIONS_LIST
};

const SLOW TUSIGN8 CUSTOM_DP_11_OPTIONS[] =
// Table referenced by: customDP_11
{
  HMI__CUSTOM_DP_11_OPTIONS_LIST
};

static const SLOW T_RANGE_E8 languageDescriptor =
{
  LANGUAGE_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__LANGUAGE_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 mainOperatorView_1_1Descriptor =
{
  PAGE_1_LINE_1_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__PAGE_1_LINE_1_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 mainOperatorView_1_2Descriptor =
{
  PAGE_1_LINE_2_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__PAGE_1_LINE_2_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 mainOperatorView_1_3Descriptor =
{
  PAGE_1_LINE_3_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__PAGE_1_LINE_3_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 mainOperatorView_1_bargraphDescriptor =
{
  PAGE_1_LINE_B_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__PAGE_1_LINE_B_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 mainOperatorView_2_1Descriptor =
{
  PAGE_2_LINE_1_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__PAGE_2_LINE_1_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 mainOperatorView_2_2Descriptor =
{
  PAGE_2_LINE_2_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__PAGE_2_LINE_2_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 mainOperatorView_2_3Descriptor =
{
  PAGE_2_LINE_3_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__PAGE_2_LINE_3_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 mainOperatorView_2_bargraphDescriptor =
{
  PAGE_2_LINE_B_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__PAGE_2_LINE_B_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 mainOperatorView_3_1Descriptor =
{
  PAGE_3_LINE_1_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__PAGE_3_LINE_1_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 mainOperatorView_3_2Descriptor =
{
  PAGE_3_LINE_2_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__PAGE_3_LINE_2_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 mainOperatorView_3_3Descriptor =
{
  PAGE_3_LINE_3_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__PAGE_3_LINE_3_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 mainOperatorView_3_bargraphDescriptor =
{
  PAGE_3_LINE_B_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__PAGE_3_LINE_B_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 mainOperatorView_4_1Descriptor =
{
  PAGE_4_LINE_1_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__PAGE_4_LINE_1_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 mainOperatorView_4_2Descriptor =
{
  PAGE_4_LINE_2_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__PAGE_4_LINE_2_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 mainOperatorView_4_3Descriptor =
{
  PAGE_4_LINE_3_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__PAGE_4_LINE_3_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 mainOperatorView_4_bargraphDescriptor =
{
  PAGE_4_LINE_B_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__PAGE_4_LINE_B_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_U8 sharedTRangeU8Descriptor0 =
// Descriptor referenced by: contrast, defaultContrast
{
  0,  // TUSIGN8 min
  100,  // TUSIGN8 max
  1  // TUSIGN8 increment
};

static const SLOW T_RANGE_E8 localOperationEnableDescriptor =
{
  _LOCAL_OP,  // TUSIGN8* ptrCodeTab
  2  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 displayMode_1Descriptor =
{
  PAGE_1_FORMAT_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__PAGE_1_FORMAT_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 displayMode_2Descriptor =
{
  PAGE_2_FORMAT_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__PAGE_2_FORMAT_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 displayMode_3Descriptor =
{
  PAGE_3_FORMAT_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__PAGE_3_FORMAT_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 displayMode_4Descriptor =
{
  PAGE_4_FORMAT_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__PAGE_4_FORMAT_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 autoscrollEnableDescriptor =
{
  _AUTOSCROLL,  // TUSIGN8* ptrCodeTab
  2  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_U8 lowLevelSoftwareRevisionDescriptor =
{
  0,  // TUSIGN8 min
  255,  // TUSIGN8 max
  1  // TUSIGN8 increment
};

static const SLOW T_RANGE_E8 diagnosisViewDescriptor =
{
  DIAGNOSIS_VIEW_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__DIAGNOSIS_VIEW_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 customDP_1Descriptor =
{
  CUSTOM_DP_1_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__CUSTOM_DP_1_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 customDP_2Descriptor =
{
  CUSTOM_DP_2_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__CUSTOM_DP_2_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 customDP_3Descriptor =
{
  CUSTOM_DP_3_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__CUSTOM_DP_3_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 customDP_4Descriptor =
{
  CUSTOM_DP_4_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__CUSTOM_DP_4_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 customDP_5Descriptor =
{
  CUSTOM_DP_5_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__CUSTOM_DP_5_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 customDP_6Descriptor =
{
  CUSTOM_DP_6_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__CUSTOM_DP_6_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 customDP_7Descriptor =
{
  CUSTOM_DP_7_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__CUSTOM_DP_7_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 customDP_8Descriptor =
{
  CUSTOM_DP_8_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__CUSTOM_DP_8_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 customDP_9Descriptor =
{
  CUSTOM_DP_9_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__CUSTOM_DP_9_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 customDP_10Descriptor =
{
  CUSTOM_DP_10_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__CUSTOM_DP_10_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 customDP_11Descriptor =
{
  CUSTOM_DP_11_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__CUSTOM_DP_11_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 DateFormatDescriptor =
{
  _DATE_FORMAT,  // TUSIGN8* ptrCodeTab
  3  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 RotationDescriptor =
{
  _ROTATION,  // TUSIGN8* ptrCodeTab
  HMI_ROTATION_N_CODES  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 displayMode_5Descriptor =
{
  PAGE_5_FORMAT_OPTIONS,  // TUSIGN8* ptrCodeTab
  PAGE_5_FORMAT_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 displayMode_6Descriptor =
{
  PAGE_6_FORMAT_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__PAGE_6_FORMAT_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 mainOperatorView_5_1Descriptor =
{
  PAGE_5_LINE_1_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__PAGE_5_LINE_1_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 mainOperatorView_5_2Descriptor =
{
  PAGE_5_LINE_2_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__PAGE_5_LINE_2_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 mainOperatorView_5_3Descriptor =
{
  PAGE_5_LINE_3_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__PAGE_5_LINE_3_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 mainOperatorView_5_bargraphDescriptor =
{
  PAGE_5_LINE_B_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__PAGE_5_LINE_B_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 mainOperatorView_6_1Descriptor =
{
  PAGE_6_LINE_1_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__PAGE_6_LINE_1_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 mainOperatorView_6_2Descriptor =
{
  PAGE_6_LINE_2_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__PAGE_6_LINE_2_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 mainOperatorView_6_3Descriptor =
{
  PAGE_6_LINE_3_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__PAGE_6_LINE_3_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 mainOperatorView_6_bargraphDescriptor =
{
  PAGE_6_LINE_B_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__PAGE_6_LINE_B_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 MenuFormatDescriptor =
{
  _MENU_FORMAT,  // TUSIGN8* ptrCodeTab
  HMI_MENU_FORMAT_N_CODES  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 CustomlanguageDescriptor =
{
  CUSTOM_LANGUAGE_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__CUSTOM_LANGUAGE_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 mainOperatorView_1_4Descriptor =
{
  PAGE_1_LINE_4_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__PAGE_1_LINE_4_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 mainOperatorView_2_4Descriptor =
{
  PAGE_2_LINE_4_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__PAGE_2_LINE_4_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 mainOperatorView_3_4Descriptor =
{
  PAGE_3_LINE_4_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__PAGE_3_LINE_4_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 mainOperatorView_4_4Descriptor =
{
  PAGE_4_LINE_4_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__PAGE_4_LINE_4_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 mainOperatorView_5_4Descriptor =
{
  PAGE_5_LINE_4_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__PAGE_5_LINE_4_N_OPTIONS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 mainOperatorView_6_4Descriptor =
{
  PAGE_6_LINE_4_OPTIONS,  // TUSIGN8* ptrCodeTab
  HMI__PAGE_6_LINE_4_N_OPTIONS  // TUSIGN8 numberOfCodes
};
//@SubGen end@<DESCRIPTORS>


//----------------------------------------------------------------------------------------------------------
//! objectList
const SLOW T_DATA_OBJ hmiObjectList[] =
{
    //lint -e605 All non-volatile data are const for T_UNIT
    //           and the typecast (void FAST*) does not increase the readability
    //@SubGen start@    <OBJECTS>
    //language
    CONSTRUCT_TABENUM8(
        &hmi__SFD_SIMPLE.language,
        NON_VOLATILE,
        &languageDescriptor),

    //mainOperatorView_1_1
    CONSTRUCT_TABENUM8(
        &hmi__SFD_SIMPLE.mainOperatorView_1_1,
        NON_VOLATILE,
        &mainOperatorView_1_1Descriptor),

    //mainOperatorView_1_2
    CONSTRUCT_TABENUM8(
        &hmi__SFD_SIMPLE.mainOperatorView_1_2,
        NON_VOLATILE,
        &mainOperatorView_1_2Descriptor),

    //mainOperatorView_1_3
    CONSTRUCT_TABENUM8(
        &hmi__SFD_SIMPLE.mainOperatorView_1_3,
        NON_VOLATILE,
        &mainOperatorView_1_3Descriptor),

    //mainOperatorView_1_bargraph
    CONSTRUCT_TABENUM8(
        &hmi__SFD_SIMPLE.mainOperatorView_1_bargraph,
        NON_VOLATILE,
        &mainOperatorView_1_bargraphDescriptor),

    //mainOperatorView_2_1
    CONSTRUCT_TABENUM8(
        &hmi__SFD_MULTI_OP.mainOperatorView_2_1,
        NON_VOLATILE,
        &mainOperatorView_2_1Descriptor),

    //mainOperatorView_2_2
    CONSTRUCT_TABENUM8(
        &hmi__SFD_MULTI_OP.mainOperatorView_2_2,
        NON_VOLATILE,
        &mainOperatorView_2_2Descriptor),

    //mainOperatorView_2_3
    CONSTRUCT_TABENUM8(
        &hmi__SFD_MULTI_OP.mainOperatorView_2_3,
        NON_VOLATILE,
        &mainOperatorView_2_3Descriptor),

    //mainOperatorView_2_bargraph
    CONSTRUCT_TABENUM8(
        &hmi__SFD_MULTI_OP.mainOperatorView_2_bargraph,
        NON_VOLATILE,
        &mainOperatorView_2_bargraphDescriptor),

    //mainOperatorView_3_1
    CONSTRUCT_TABENUM8(
        &hmi__SFD_MULTI_OP.mainOperatorView_3_1,
        NON_VOLATILE,
        &mainOperatorView_3_1Descriptor),

    //mainOperatorView_3_2
    CONSTRUCT_TABENUM8(
        &hmi__SFD_MULTI_OP.mainOperatorView_3_2,
        NON_VOLATILE,
        &mainOperatorView_3_2Descriptor),

    //mainOperatorView_3_3
    CONSTRUCT_TABENUM8(
        &hmi__SFD_MULTI_OP.mainOperatorView_3_3,
        NON_VOLATILE,
        &mainOperatorView_3_3Descriptor),

    //mainOperatorView_3_bargraph
    CONSTRUCT_TABENUM8(
        &hmi__SFD_MULTI_OP.mainOperatorView_3_bargraph,
        NON_VOLATILE,
        &mainOperatorView_3_bargraphDescriptor),

    //mainOperatorView_4_1
    CONSTRUCT_TABENUM8(
        &hmi__SFD_MULTI_OP.mainOperatorView_4_1,
        NON_VOLATILE,
        &mainOperatorView_4_1Descriptor),

    //mainOperatorView_4_2
    CONSTRUCT_TABENUM8(
        &hmi__SFD_MULTI_OP.mainOperatorView_4_2,
        NON_VOLATILE,
        &mainOperatorView_4_2Descriptor),

    //mainOperatorView_4_3
    CONSTRUCT_TABENUM8(
        &hmi__SFD_MULTI_OP.mainOperatorView_4_3,
        NON_VOLATILE,
        &mainOperatorView_4_3Descriptor),

    //mainOperatorView_4_bargraph
    CONSTRUCT_TABENUM8(
        &hmi__SFD_MULTI_OP.mainOperatorView_4_bargraph,
        NON_VOLATILE,
        &mainOperatorView_4_bargraphDescriptor),

    //contrast
    CONSTRUCT_USIGN8(
        &hmi__SFD_SIMPLE.contrast,
        NON_VOLATILE,
        &sharedTRangeU8Descriptor0),

    //localOperationEnable
    CONSTRUCT_TABENUM8(
        &hmi__SFD_SIMPLE.localOperationEnable,
        NON_VOLATILE,
        &localOperationEnableDescriptor),

    //displayMode_1
    CONSTRUCT_TABENUM8(
        &hmi__SFD_SIMPLE.displayMode_1,
        NON_VOLATILE,
        &displayMode_1Descriptor),

    //displayMode_2
    CONSTRUCT_TABENUM8(
        &hmi__SFD_MULTI_OP.displayMode_2,
        NON_VOLATILE,
        &displayMode_2Descriptor),

    //displayMode_3
    CONSTRUCT_TABENUM8(
        &hmi__SFD_MULTI_OP.displayMode_3,
        NON_VOLATILE,
        &displayMode_3Descriptor),

    //displayMode_4
    CONSTRUCT_TABENUM8(
        &hmi__SFD_MULTI_OP.displayMode_4,
        NON_VOLATILE,
        &displayMode_4Descriptor),

    //autoscrollEnable
    CONSTRUCT_TABENUM8(
        &hmi__SFD_MULTI_OP.autoscrollEnable,
        NON_VOLATILE,
        &autoscrollEnableDescriptor),

    //lowLevelSoftwareRevision
    CONSTRUCT_USIGN8(
        &hmi_dynamic.lowLevelSoftwareRevision,
        RAM,
        &lowLevelSoftwareRevisionDescriptor),

    //defaultContrast
    CONSTRUCT_USIGN8(
        &hmi_dynamic.defaultContrast,
        RAM,
        &sharedTRangeU8Descriptor0),

    //diagnosisView
    CONSTRUCT_TABENUM8(
        &hmi_dynamic.diagnosisView,
        RAM,
        &diagnosisViewDescriptor),

    //customDP_1
    CONSTRUCT_TABENUM8(
        &hmi__SFD_CUSTOM_DP.customDP_1,
        NON_VOLATILE,
        &customDP_1Descriptor),

    //customDP_2
    CONSTRUCT_TABENUM8(
        &hmi__SFD_CUSTOM_DP.customDP_2,
        NON_VOLATILE,
        &customDP_2Descriptor),

    //customDP_3
    CONSTRUCT_TABENUM8(
        &hmi__SFD_CUSTOM_DP.customDP_3,
        NON_VOLATILE,
        &customDP_3Descriptor),

    //customDP_4
    CONSTRUCT_TABENUM8(
        &hmi__SFD_CUSTOM_DP.customDP_4,
        NON_VOLATILE,
        &customDP_4Descriptor),

    //customDP_5
    CONSTRUCT_TABENUM8(
        &hmi__SFD_CUSTOM_DP.customDP_5,
        NON_VOLATILE,
        &customDP_5Descriptor),

    //customDP_6
    CONSTRUCT_TABENUM8(
        &hmi__SFD_CUSTOM_DP.customDP_6,
        NON_VOLATILE,
        &customDP_6Descriptor),

    //customDP_7
    CONSTRUCT_TABENUM8(
        &hmi__SFD_CUSTOM_DP.customDP_7,
        NON_VOLATILE,
        &customDP_7Descriptor),

    //customDP_8
    CONSTRUCT_TABENUM8(
        &hmi__SFD_CUSTOM_DP.customDP_8,
        NON_VOLATILE,
        &customDP_8Descriptor),

    //customDP_9
    CONSTRUCT_TABENUM8(
        &hmi__SFD_CUSTOM_DP.customDP_9,
        NON_VOLATILE,
        &customDP_9Descriptor),

    //customDP_10
    CONSTRUCT_TABENUM8(
        &hmi__SFD_CUSTOM_DP.customDP_10,
        NON_VOLATILE,
        &customDP_10Descriptor),

    //customDP_11
    CONSTRUCT_TABENUM8(
        &hmi__SFD_CUSTOM_DP.customDP_11,
        NON_VOLATILE,
        &customDP_11Descriptor),

    //Password
    CONSTRUCT_ARRAY_SIMPLE_U8(
        hmi_dynamic.Password,
        6,
        RAM),

    //SetPassword_StandardLevel
    CONSTRUCT_ARRAY_SIMPLE_U8(
        hmi_dynamic.SetPassword_StandardLevel,
        6,
        RAM),

    //SetPassword_AdvancedLevel
    CONSTRUCT_ARRAY_SIMPLE_U8(
        hmi_dynamic.SetPassword_AdvancedLevel,
        6,
        RAM),

    //SetPassword_ServiceLevel
    CONSTRUCT_ARRAY_SIMPLE_U8(
        hmi_dynamic.SetPassword_ServiceLevel,
        6,
        RAM),

    //SetPassword_StandardLevelDefault
    CONSTRUCT_ARRAY_SIMPLE_U8(
        hmi_dynamic.SetPassword_StandardLevelDefault,
        6,
        RAM),

    //SetPassword_AdvancedLevelDefault
    CONSTRUCT_ARRAY_SIMPLE_U8(
        hmi_dynamic.SetPassword_AdvancedLevelDefault,
        6,
        RAM),

    //SetPassword_ServiceLevelDefault
    CONSTRUCT_ARRAY_SIMPLE_U8(
        hmi_dynamic.SetPassword_ServiceLevelDefault,
        6,
        RAM),

    //DateFormat
    CONSTRUCT_TABENUM8(
        &hmi__SFD_SIMPLE.DateFormat,
        NON_VOLATILE,
        &DateFormatDescriptor),

    //displayTest
    CONSTRUCT_SIMPLE_U8(
        &hmi_dynamic.displayTest,
        RAM),
    
#ifdef COMMON_TOP_WORKS_NEW_FEATURES
    //Rotation
    CONSTRUCT_TABENUM8(
        &hmi__SFD_SIMPLE_CTW.Rotation,
        NON_VOLATILE,
        &RotationDescriptor),

    //MenuFormat
    CONSTRUCT_TABENUM8(
        &hmi__SFD_SIMPLE_CTW.MenuFormat,
        NON_VOLATILE,
        &MenuFormatDescriptor),

    //displayMode_5
    CONSTRUCT_TABENUM8(
        &hmi__SFD_MULTI_OP_CTW.displayMode_5,
        NON_VOLATILE,
        &displayMode_5Descriptor),

    //displayMode_6
    CONSTRUCT_TABENUM8(
        &hmi__SFD_MULTI_OP_CTW.displayMode_6,
        NON_VOLATILE,
        &displayMode_6Descriptor),

    //mainOperatorView_5_1
    CONSTRUCT_TABENUM8(
        &hmi__SFD_MULTI_OP_CTW.mainOperatorView_5_1,
        NON_VOLATILE,
        &mainOperatorView_5_1Descriptor),

    //mainOperatorView_5_2
    CONSTRUCT_TABENUM8(
        &hmi__SFD_MULTI_OP_CTW.mainOperatorView_5_2,
        NON_VOLATILE,
        &mainOperatorView_5_2Descriptor),

    //mainOperatorView_5_3
    CONSTRUCT_TABENUM8(
        &hmi__SFD_MULTI_OP_CTW.mainOperatorView_5_3,
        NON_VOLATILE,
        &mainOperatorView_5_3Descriptor),

    //mainOperatorView_5_bargraph
    CONSTRUCT_TABENUM8(
        &hmi__SFD_MULTI_OP_CTW.mainOperatorView_5_bargraph,
        NON_VOLATILE,
        &mainOperatorView_5_bargraphDescriptor),

    //mainOperatorView_6_1
    CONSTRUCT_TABENUM8(
        &hmi__SFD_MULTI_OP_CTW.mainOperatorView_6_1,
        NON_VOLATILE,
        &mainOperatorView_6_1Descriptor),

    //mainOperatorView_6_2
    CONSTRUCT_TABENUM8(
        &hmi__SFD_MULTI_OP_CTW.mainOperatorView_6_2,
        NON_VOLATILE,
        &mainOperatorView_6_2Descriptor),

    //mainOperatorView_6_3
    CONSTRUCT_TABENUM8(
        &hmi__SFD_MULTI_OP_CTW.mainOperatorView_6_3,
        NON_VOLATILE,
        &mainOperatorView_6_3Descriptor),

    //mainOperatorView_6_bargraph
    CONSTRUCT_TABENUM8(
        &hmi__SFD_MULTI_OP_CTW.mainOperatorView_6_bargraph,
        NON_VOLATILE,
        &mainOperatorView_6_bargraphDescriptor),

    //Customlanguage
    CONSTRUCT_TABENUM8(
        &hmi_dynamic.Customlanguage,
        RAM,
        &CustomlanguageDescriptor),

    //mainOperatorView_1_4
    CONSTRUCT_TABENUM8(
        &hmi__SFD_SIMPLE_CTW.mainOperatorView_1_4,
        NON_VOLATILE,
        &mainOperatorView_1_4Descriptor),

    //mainOperatorView_2_4
    CONSTRUCT_TABENUM8(
        &hmi__SFD_MULTI_OP_CTW.mainOperatorView_2_4,
        NON_VOLATILE,
        &mainOperatorView_2_4Descriptor),

    //mainOperatorView_3_4
    CONSTRUCT_TABENUM8(
        &hmi__SFD_MULTI_OP_CTW.mainOperatorView_3_4,
        NON_VOLATILE,
        &mainOperatorView_3_4Descriptor),

    //mainOperatorView_4_4
    CONSTRUCT_TABENUM8(
        &hmi__SFD_MULTI_OP_CTW.mainOperatorView_4_4,
        NON_VOLATILE,
        &mainOperatorView_4_4Descriptor),

    //mainOperatorView_5_4
    CONSTRUCT_TABENUM8(
        &hmi__SFD_MULTI_OP_CTW.mainOperatorView_5_4,
        NON_VOLATILE,
        &mainOperatorView_5_4Descriptor),

    //mainOperatorView_6_4
    CONSTRUCT_TABENUM8(
        &hmi__SFD_MULTI_OP_CTW.mainOperatorView_6_4,
        NON_VOLATILE,
        &mainOperatorView_6_4Descriptor)
    //@SubGen end@    <OBJECTS>
#endif
    //lint +e605 warning on for the rest of the code
};



//----------------------------------------------------------------------------------------------------------
//! define the HMI-Class object
SLOW const T_HMI hmi =
{
    {
        &hmiState,
        &hmiDataSemaphore,

        hmiObjectList,
        //lint -e{778}
        sizeof(hmiObjectList)/sizeof(hmiObjectList[0])-1,

        hmiDataClassList,
        sizeof(hmiDataClassList)/sizeof(hmiDataClassList[0]),

        ChangeState_T_UNIT,
        GetState_T_UNIT,
        Initialize_HMI, // will be overloaded
        LoadRomDefaults_T_UNIT,
        ResetToDefault_T_UNIT,
        SaveAsDefault_T_UNIT,

      #if(HOST_FRAMEWORK_VERSION < 0x020300)
        LoadAll_T_UNIT,
        SaveAll_T_UNIT,
      #endif

        Get_T_UNIT,
        Put_T_UNIT,
        Check_T_UNIT,

      #if defined(HMI_SUPPORTS_SIGNALS_VALIDITY_TEST) && defined(ENABLE_LINE_SOURCE_VALIDITY_TESTS)
        GetAttributeDescription_HMI,
      #else
        GetAttributeDescription_T_UNIT,
      #endif

        GetObjectDescription_T_UNIT
    }

    //subsystem EXE and SRV-methods
    //@SubGen start@    <METHODS>
    ,HMI_EXE_HMI
    ,HMI_ForceDisplayTimeoutSRV_HMI
    ,HMI_PowerOnSRV_HMI
    ,HMI_AllowIOaccessSRV_HMI
    ,HMI_DenyIOaccessSRV_HMI
    ,HMI_TransmitInterruptSRV_HMI
    ,HMI_ReceiveInterruptSRV_HMI
    ,HMI_IsIOActiveSRV_HMI
    ,HMI_NV_InitCompleteSRV_HMI
    ,HMI_GetKeypressSRV_HMI
    ,HMI_IsOperatorViewActive_HMI
    ,HMI_Get_LLHardwareType_SRV_HMI
    ,HMI_GetAutoscrollState_SRV_HMI
    ,HMI_GetOperatorPageNumber_SRV_HMI
    ,HMI_resetConfigurationTimeout_HMI
    ,HMI_SetStartupFrame_SRV_HMI
    ,HMI_SetOperatorIcon_SRV_HMI
    ,HMI_SetGraphPointer_SRV_HMI
    ,HMI_ToggleBacklight_SRV_HMI
    ,HMI_SetFunctionKeyLock_SRV_HMI
    ,HMI_SetParameterDumpFont_SRV_HMI
    ,HMI_SetOperatorInfo_SRV_HMI
    ,HMI_SetGraphTimebase_SRV_HMI
    ,HMI_AllowReInit_SRV_HMI
    ,HMI_GetLanguageIndex_SRV_HMI
    ,HMI_InitialiseGlobalConfigChangeCounter_SRV_HMI
    ,HMI_ReInitialiseLowLevelDetection_SRV_HMI
    ,HMI_PutPacketBuffer_SRV_HMI
    //@SubGen end@    <METHODS>
};

/*

This section defines templates for the SubSystem CodeGenerator.
Do not modify or remove this section

see also: hmi_attributes.h


//special templates for specific standard storage classes:
//@SubGen template start@<DATACLASS CONSTANT>
//----------------------------------------------------------------------------------------------------------
//! definition of constant block
const SLOW T_HMI_TEMPLATE hmi_template_data=
{
    //@SubGen start@    <INITIALIZER TEMPLATE>
    //@SubGen end@    <INITIALIZER TEMPLATE>
};
//----------------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS CONSTANT>



//@SubGen template start@<DATACLASS DYNAMIC>
//----------------------------------------------------------------------------------------------------------
//! definition of dynamic data
FAST T_HMI_DYNAMIC hmi_dynamic_data;
const SLOW T_HMI_TEMPLATE hmi_template_initializer=
{
    //@SubGen start@    <INITIALIZER TEMPLATE>
    //@SubGen end@    <INITIALIZER TEMPLATE>
};
//----------------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS DYNAMIC>



//@SubGen template start@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>
//----------------------------------------------------------------------------------------------------------
//! definition of Template
#ifdef WIN32
  #pragma bss_seg(".AAA$HMI_DATA")
    const FAST T_HMI_TEMPLATE hmi_template_data;
    const FAST T_HMI_TEMPLATE hmi_template_default;
  #pragma bss_seg()
#else
  #pragma location="HMI_TEMPLATE"
    const FAST __no_init T_HMI_TEMPLATE hmi_template_data;
  #pragma location="HMI_TEMPLATE_DEFAULT"
    const FAST __no_init T_HMI_TEMPLATE hmi_template_default;
#endif
const SLOW T_HMI_TEMPLATE hmi_template_initializer=
{
    //@SubGen start@    <INITIALIZER TEMPLATE>
    //@SubGen end@    <INITIALIZER TEMPLATE>
};
//----------------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>


//this template is for all other standard storage classes:

//@SubGen template start@<DATACLASS>
//----------------------------------------------------------------------------------------------------------
//! definition of Template
#ifdef WIN32
  #pragma bss_seg(".AAA$HMI_DATA")
    const FAST T_HMI_TEMPLATE hmi_template_data;
  #pragma bss_seg()
#else
  #pragma location="HMI_TEMPLATE"
    const FAST __no_init T_HMI_TEMPLATE hmi_template_data;
#endif
const SLOW T_HMI_TEMPLATE hmi_template_initializer=
{
    //@SubGen start@    <INITIALIZER TEMPLATE>
    //@SubGen end@    <INITIALIZER TEMPLATE>
};
//----------------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS>

//@SubGen template start@ <DATACLASSNAME>
hmi_template
//@SubGen template end@ <DATACLASSNAME>


//@SubGen template start@ <DATACLASSLIST DYNAMIC>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_DYNAMIC(hmi_template_data,hmi_template_initializer)
//@SubGen template end@ <DATACLASSLIST DYNAMIC>

//@SubGen template start@ <DATACLASSLIST CONSTANT>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_CONSTANT(hmi_template_data)
//@SubGen template end@ <DATACLASSLIST CONSTANT>

//@SubGen template start@ <DATACLASSLIST>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_STATIC_NODEF(hmi_template_data,hmi_template_initializer)
//@SubGen template end@ <DATACLASSLIST>

//@SubGen template start@ <DATACLASSLIST STATIC_FREQUENT_CONSTANT_DEFAULT>
//lint -e{708} initialization is OK
CONSTRUCTOR_DC_STATIC_CONSTDEF(hmi_template_data,hmi_template_initializer)
//@SubGen template end@ <DATACLASSLIST STATIC_FREQUENT_CONSTANT_DEFAULT>

//@SubGen template start@ <DATACLASSLIST STATIC_FREQUENT_STATIC_DEFAULT>
//lint -e{708} initialization is OK
CONSTRUCTOR_DC_STATIC_STATDEF(hmi_template_data,hmi_template_initializer,hmi_template_default)
//@SubGen template end@ <DATACLASSLIST STATIC_FREQUENT_STATIC_DEFAULT>

*/



// M. Ashford - Enable MISRA Diagnostics
#pragma diag_default=Pm022
