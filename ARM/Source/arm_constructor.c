// M Ashford - Suppress MISRA Diagnostics
// MISRA Rule 31
// Braces shall be used to indicate and match the structure in the non-zero
// initialization of arrays and structures.
#pragma diag_suppress=Pm023

// M Ashford - Suppress MISRA Diagnostics
// MISRA Rule 31
// Braces shall be used to indicate and match the structure in the non-zero
// initialization of arrays and structures.
#pragma diag_suppress=Pm022
//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2005.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Arm
 Module
 Description    Subsystem constructor
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#include "System/Interface/system.h"

#ifdef FRAMEWORK_VERSION_2_1_0
  #include "../../common/interface/common_type.h"
#endif //FRAMEWORK_VERSION_2_1_0
#ifdef FRAMEWORK_VERSION_2_2_0
  #include "../../common/interface/common_type.h"
#endif //FRAMEWORK_VERSION_2_2_0
#ifdef FRAMEWORK_VERSION_2_3_1
  #include "System/Interface/common_type.h"
#endif //FRAMEWORK_VERSION_2_3_1
#include "RTOS/Interface/rtos.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_DATA_OBJ/Interface/t_data_obj_simple.h"
#include "T_DATA_OBJ/Interface/t_data_obj_tabenum8.h"

#include "T_Unit/Interface/t_unit.h"
#include "T_Unit/Interface/T_DATACLASSES.H"
#include "Coordinator/Interface/subsystem_idx.h"

// Additional T_Data_Obj includes
#include "T_DATA_OBJ/Interface/t_data_obj_action.h"
#include "ARM/Interface/arm_bespoke.h"
#include "ARM/Interface/arm.h"
#include "arm_types.h"
#include "arm_attributes.h"
#include "arm_overload.h"
#include "arm_execute.h"

// Action functions
#include "ARM/Source/arm_action_fct.h"

//-------------------------------------------------------------------------------------------------
//! unit global attributes
FAST TUSIGN16 armState;     // Subsystem state
FAST T_DATA_SEMAPHORE armDataSemaphore;



const SLOW T_DATACLASS armDataClassList[]=
{
    //lint -e{651} Initializers are OK, the warning pop up with the first 708
    //@SubGen start@    <DATACLASSLIST>
    //lint -e{708} initialization is OK
    CONSTRUCTOR_DC_STATIC_CONSTDEF(armStaticFrequentConstantDefault_2,armStaticFrequentConstantDefault_2Initializer),
    //lint -e{708} initialization is OK
    CONSTRUCTOR_DC_STATIC_CONSTDEF(armStaticFrequentConstantDefault_Password,armStaticFrequentConstantDefault_PasswordInitializer),
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_DYNAMIC(armDynamic,armDynamicInitializer),
    //lint -e{708} initialization is OK
    CONSTRUCTOR_DC_STATIC_CONSTDEF(armStaticFrequentConstantDefault,armStaticFrequentConstantDefaultInitializer)
    //@SubGen end@    <DATACLASSLIST>
};

//@SubGen put@<DATACLASS>
//-------------------------------------------------------------------------------------------------
//! definition of StaticFrequentConstantDefault_Password data
#ifdef WIN32
  #pragma bss_seg(".AAA$ARM_DATA")
    const FAST T_ARM_STATIC_FREQUENT_CONSTANT_DEFAULT_PASSWORD armStaticFrequentConstantDefault_Password;
  #pragma bss_seg()
#else
  #pragma location="ARM_STATIC_FREQUENT_CONSTANT_DEFAULT_PASSWORD"
    const FAST __no_init T_ARM_STATIC_FREQUENT_CONSTANT_DEFAULT_PASSWORD armStaticFrequentConstantDefault_Password;
#endif
const SLOW T_ARM_STATIC_FREQUENT_CONSTANT_DEFAULT_PASSWORD armStaticFrequentConstantDefault_PasswordInitializer=
{
    //@SubGen start@    <INITIALIZER STATIC_FREQUENT_CONSTANT_DEFAULT_PASSWORD>
    ARM_CALIBRATION_LOAD,    // SIMPLE_U8 hardwareCalibrationLoad
    ARM_CUSTODY_SWITCH_ACTIVE,    // TABENUM8 serviceAccountSwitch
    {ARM_STANDARD_DEFAULT_PASSWORD_1,ARM_STANDARD_DEFAULT_PASSWORD_2,ARM_STANDARD_DEFAULT_PASSWORD_3,ARM_STANDARD_DEFAULT_PASSWORD_4,ARM_STANDARD_DEFAULT_PASSWORD_5,ARM_STANDARD_DEFAULT_PASSWORD_6,ARM_STANDARD_DEFAULT_PASSWORD_7,ARM_STANDARD_DEFAULT_PASSWORD_8},    // SIMPLE_U32 stdDefaultPassword[8]
    {ARM_ADVANCED_DEFAULT_PASSWORD_1,ARM_ADVANCED_DEFAULT_PASSWORD_2,ARM_ADVANCED_DEFAULT_PASSWORD_3,ARM_ADVANCED_DEFAULT_PASSWORD_4,ARM_ADVANCED_DEFAULT_PASSWORD_5,ARM_ADVANCED_DEFAULT_PASSWORD_6,ARM_ADVANCED_DEFAULT_PASSWORD_7,ARM_ADVANCED_DEFAULT_PASSWORD_8},    // SIMPLE_U32 advDefaultPassword[8]
    {ARM_SERVICE_DEFAULT_PASSWORD_1,ARM_SERVICE_DEFAULT_PASSWORD_2,ARM_SERVICE_DEFAULT_PASSWORD_3,ARM_SERVICE_DEFAULT_PASSWORD_4,ARM_SERVICE_DEFAULT_PASSWORD_5,ARM_SERVICE_DEFAULT_PASSWORD_6,ARM_SERVICE_DEFAULT_PASSWORD_7,ARM_SERVICE_DEFAULT_PASSWORD_8},    // SIMPLE_U32 svcDefaultPassword[8]
    {ARM_STANDARD_DEFAULT_PASSWORD_1,ARM_STANDARD_DEFAULT_PASSWORD_2,ARM_STANDARD_DEFAULT_PASSWORD_3,ARM_STANDARD_DEFAULT_PASSWORD_4,ARM_STANDARD_DEFAULT_PASSWORD_5,ARM_STANDARD_DEFAULT_PASSWORD_6,ARM_STANDARD_DEFAULT_PASSWORD_7,ARM_STANDARD_DEFAULT_PASSWORD_8},    // SIMPLE_U32 stdUserPassword[8]
    {ARM_ADVANCED_DEFAULT_PASSWORD_1,ARM_ADVANCED_DEFAULT_PASSWORD_2,ARM_ADVANCED_DEFAULT_PASSWORD_3,ARM_ADVANCED_DEFAULT_PASSWORD_4,ARM_ADVANCED_DEFAULT_PASSWORD_5,ARM_ADVANCED_DEFAULT_PASSWORD_6,ARM_ADVANCED_DEFAULT_PASSWORD_7,ARM_ADVANCED_DEFAULT_PASSWORD_8},    // SIMPLE_U32 advUserPassword[8]
    {ARM_SERVICE_DEFAULT_PASSWORD_1,ARM_SERVICE_DEFAULT_PASSWORD_2,ARM_SERVICE_DEFAULT_PASSWORD_3,ARM_SERVICE_DEFAULT_PASSWORD_4,ARM_SERVICE_DEFAULT_PASSWORD_5,ARM_SERVICE_DEFAULT_PASSWORD_6,ARM_SERVICE_DEFAULT_PASSWORD_7,ARM_SERVICE_DEFAULT_PASSWORD_8}    // SIMPLE_U32 svcUserPassword[8]
    //@SubGen end@    <INITIALIZER STATIC_FREQUENT_CONSTANT_DEFAULT_PASSWORD>
};
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//! definition of StaticFrequentConstantDefault data
#ifdef WIN32
  #pragma bss_seg(".AAA$ARM_DATA")
    const FAST T_ARM_STATIC_FREQUENT_CONSTANT_DEFAULT armStaticFrequentConstantDefault;
  #pragma bss_seg()
#else
  #pragma location="ARM_STATIC_FREQUENT_CONSTANT_DEFAULT"
    const FAST __no_init T_ARM_STATIC_FREQUENT_CONSTANT_DEFAULT armStaticFrequentConstantDefault;
#endif
const SLOW T_ARM_STATIC_FREQUENT_CONSTANT_DEFAULT armStaticFrequentConstantDefaultInitializer=
{
    //@SubGen start@    <INITIALIZER STATIC_FREQUENT_CONSTANT_DEFAULT>
    ARM_STANDARD_DEFAULT_PASSWORD,    // SIMPLE_U32 stdDefaultPassword_dummy
    ARM_ADVANCED_DEFAULT_PASSWORD,    // SIMPLE_U32 advDefaultPassword_dummy
    ARM_SERVICE_DEFAULT_PASSWORD,    // SIMPLE_U32 svcDefaultPassword_dummy
    ARM_STANDARD_DEFAULT_PASSWORD,    // SIMPLE_U32 stdUserPassword_dummy
    ARM_ADVANCED_DEFAULT_PASSWORD,    // SIMPLE_U32 advUserPassword_dummy
    ARM_SERVICE_DEFAULT_PASSWORD,    // SIMPLE_U32 svcUserPassword_dummy
    1,    // SIMPLE_U16 adpin
    ARM_CLIENT_ENABLE_DEFAULT    // TABENUM8 clientEnableState
    //@SubGen end@    <INITIALIZER STATIC_FREQUENT_CONSTANT_DEFAULT>
};
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//! definition of StaticFrequentConstantDefault_2 data
#ifdef WIN32
  #pragma bss_seg(".AAA$ARM_DATA")
    const FAST T_ARM_STATIC_FREQUENT_CONSTANT_DEFAULT_2 armStaticFrequentConstantDefault_2;
    const FAST T_ARM_STATIC_FREQUENT_CONSTANT_DEFAULT_2 armStaticFrequentConstantDefault_2Default;
  #pragma bss_seg()
#else
  #pragma location="ARM_STATIC_FREQUENT_CONSTANT_DEFAULT_2"
    const FAST __no_init T_ARM_STATIC_FREQUENT_CONSTANT_DEFAULT_2 armStaticFrequentConstantDefault_2;
  #pragma location="ARM_STATIC_FREQUENT_CONSTANT_DEFAULT_2_DEFAULT"
    const FAST __no_init T_ARM_STATIC_FREQUENT_CONSTANT_DEFAULT_2 armStaticFrequentConstantDefault_2Default;
#endif
const SLOW T_ARM_STATIC_FREQUENT_CONSTANT_DEFAULT_2 armStaticFrequentConstantDefault_2Initializer=
{
    //@SubGen start@    <INITIALIZER STATIC_FREQUENT_CONSTANT_DEFAULT_2>
    HIGH_SECURITY_DEFAULT    // TABENUM8 hiSecAccessState
    //@SubGen end@    <INITIALIZER STATIC_FREQUENT_CONSTANT_DEFAULT_2>
};
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
FAST T_ARM_DYNAMIC armDynamic;
const SLOW T_ARM_DYNAMIC armDynamicInitializer=
{
    //@SubGen start@    <INITIALIZER DYNAMIC>
    ARM_NO_USER,    // TABENUM8 hmiLoginState
    ARM_NO_USER,    // TABENUM8 hartLoginState
    {'?',0,0,0,0,0,0,0},    // SIMPLE_U8 transmitterIdStringForm[8]
    ARM_READ_ONLY_SWITCH_INACTIVE,    // TABENUM8 readOnlySwitchStatus
    ARM_CUSTODY_SWITCH_INACTIVE    // TABENUM8 custodySwitchStatus
    //@SubGen end@    <INITIALIZER DYNAMIC>
};
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//! object check and PutGet descriptions
//@SubGen start@<DESCRIPTORS>
const SLOW TUSIGN8 THighSecurityEnableFormats[] =
// Table referenced by: hiSecAccessState
{
  ARM_HIGH_SECURITY_DISABLED,
  ARM_HIGH_SECURITY_ENABLED
};

const SLOW TUSIGN8 TClientEnableFormats[] =
// Table referenced by: clientEnableState
{
  ARM_HART_ENABLED,
  ARM_HMI_ENABLED,
  ARM_HART_AND_HMI_ENABLED
};

const SLOW TUSIGN8 TClientLoginFormats[] =
// Table referenced by: hartLoginState, hmiLoginState
{
  ARM_NO_USER,
  ARM_STANDARD_USER,
  ARM_ADVANCED_USER,
  ARM_SERVICE_USER,
  ARM_CUSTODY_USER
};

const SLOW TUSIGN8 TCustodySwitchActiveFormats[] =
// Table referenced by: custodySwitchStatus, serviceAccountSwitch
{
  ARM_CUSTODY_SWITCH_INACTIVE,
  ARM_CUSTODY_SWITCH_ACTIVE
};

const SLOW TUSIGN8 TReadOnlySwitchActiveFormats[] =
// Table referenced by: readOnlySwitchStatus
{
  ARM_READ_ONLY_SWITCH_INACTIVE,
  ARM_READ_ONLY_SWITCH_ACTIVE
};

static const SLOW T_RANGE_E8 sharedTRangeE8Descriptor0 =
// Descriptor referenced by: hartLoginState, hmiLoginState
{
  TClientLoginFormats,  // TUSIGN8* ptrCodeTab
  ARM_NUMBER_OF_USER_LOGGED_IN_STATES  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 clientEnableStateDescriptor =
{
  TClientEnableFormats,  // TUSIGN8* ptrCodeTab
  ARM_NUMBER_OF_CLIENT_ENABLE_STATES  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 hiSecAccessStateDescriptor =
{
  THighSecurityEnableFormats,  // TUSIGN8* ptrCodeTab
  ARM_NUMBER_OF_HIGH_SECURITY_ENABLE_STATES  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 readOnlySwitchStatusDescriptor =
{
  TReadOnlySwitchActiveFormats,  // TUSIGN8* ptrCodeTab
  ARM_NUMBER_OF_READ_ONLY_SWITCH_STATES  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 custodySwitchStatusDescriptor =
{
  TCustodySwitchActiveFormats,  // TUSIGN8* ptrCodeTab
  ARM_NUMBER_OF_CUSTODY_SWITCH_STATES  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 serviceAccountSwitchDescriptor =
{
  TCustodySwitchActiveFormats,  // TUSIGN8* ptrCodeTab
  sizeof(TCustodySwitchActiveFormats)  // TUSIGN8 numberOfCodes
};
//@SubGen end@<DESCRIPTORS>


//-------------------------------------------------------------------------------------------------
//! objectList
const SLOW T_DATA_OBJ armObjectList[] =
{
    //lint -e605 All non-volatile data are const for T_UNIT
    //           and the typecast (void FAST*) does not increase the readability
    //@SubGen start@    <OBJECTS>
    //stdDefaultPassword_dummy
    CONSTRUCT_SIMPLE_U32(
        &armStaticFrequentConstantDefault.stdDefaultPassword_dummy,
        NON_VOLATILE),

    //advDefaultPassword_dummy
    CONSTRUCT_SIMPLE_U32(
        &armStaticFrequentConstantDefault.advDefaultPassword_dummy,
        NON_VOLATILE),

    //svcDefaultPassword_dummy
    CONSTRUCT_SIMPLE_U32(
        &armStaticFrequentConstantDefault.svcDefaultPassword_dummy,
        NON_VOLATILE),

    //stdUserPassword_dummy
    CONSTRUCT_SIMPLE_U32(
        &armStaticFrequentConstantDefault.stdUserPassword_dummy,
        NON_VOLATILE),

    //advUserPassword_dummy
    CONSTRUCT_SIMPLE_U32(
        &armStaticFrequentConstantDefault.advUserPassword_dummy,
        NON_VOLATILE),

    //svcUserPassword_dummy
    CONSTRUCT_SIMPLE_U32(
        &armStaticFrequentConstantDefault.svcUserPassword_dummy,
        NON_VOLATILE),

    //adpin
    CONSTRUCT_SIMPLE_U16(
        &armStaticFrequentConstantDefault.adpin,
        NON_VOLATILE),

    //clientEnableState
    CONSTRUCT_TABENUM8(
        &armStaticFrequentConstantDefault.clientEnableState,
        NON_VOLATILE,
        &clientEnableStateDescriptor),

    //hiSecAccessState
    CONSTRUCT_TABENUM8(
        &armStaticFrequentConstantDefault_2.hiSecAccessState,
        NON_VOLATILE,
        &hiSecAccessStateDescriptor),

    //hmiLoginState
    CONSTRUCT_TABENUM8(
        &armDynamic.hmiLoginState,
        RAM,
        &sharedTRangeE8Descriptor0),

    //hartLoginState
    CONSTRUCT_TABENUM8(
        &armDynamic.hartLoginState,
        RAM,
        &sharedTRangeE8Descriptor0),

    //transmitterIdStringForm
    CONSTRUCT_ARRAY_SIMPLE_U8(
        armDynamic.transmitterIdStringForm,
        8,
        RAM),

    //readOnlySwitchStatus
    CONSTRUCT_TABENUM8(
        &armDynamic.readOnlySwitchStatus,
        RAM,
        &readOnlySwitchStatusDescriptor),

    //custodySwitchStatus
    CONSTRUCT_TABENUM8(
        &armDynamic.custodySwitchStatus,
        RAM,
        &custodySwitchStatusDescriptor),

    //enableHighSecurityMode
    CONSTRUCT_ACTION(
        //lint -e{611} cast is OK for T_ACTION
        enableHighSecurityMode_ARM),

    //disableHighSecurityMode
    CONSTRUCT_ACTION(
        //lint -e{611} cast is OK for T_ACTION
        disableHighSecurityMode_ARM),

    //hardwareCalibrationLoad
    CONSTRUCT_SIMPLE_U8(
        &armStaticFrequentConstantDefault_Password.hardwareCalibrationLoad,
        NON_VOLATILE),

    //serviceAccountSwitch
    CONSTRUCT_TABENUM8(
        &armStaticFrequentConstantDefault_Password.serviceAccountSwitch,
        NON_VOLATILE,
        &serviceAccountSwitchDescriptor),

    //stdDefaultPassword
    CONSTRUCT_ARRAY_SIMPLE_U32(
        armStaticFrequentConstantDefault_Password.stdDefaultPassword,
        8,
        NON_VOLATILE),

    //advDefaultPassword
    CONSTRUCT_ARRAY_SIMPLE_U32(
        armStaticFrequentConstantDefault_Password.advDefaultPassword,
        8,
        NON_VOLATILE),

    //svcDefaultPassword
    CONSTRUCT_ARRAY_SIMPLE_U32(
        armStaticFrequentConstantDefault_Password.svcDefaultPassword,
        8,
        NON_VOLATILE),

    //stdUserPassword
    CONSTRUCT_ARRAY_SIMPLE_U32(
        armStaticFrequentConstantDefault_Password.stdUserPassword,
        8,
        NON_VOLATILE),

    //advUserPassword
    CONSTRUCT_ARRAY_SIMPLE_U32(
        armStaticFrequentConstantDefault_Password.advUserPassword,
        8,
        NON_VOLATILE),

    //svcUserPassword
    CONSTRUCT_ARRAY_SIMPLE_U32(
        armStaticFrequentConstantDefault_Password.svcUserPassword,
        8,
        NON_VOLATILE)
    //@SubGen end@    <OBJECTS>
    //lint +e605 warning on for the rest of the code
};



//-------------------------------------------------------------------------------------------------
//! define the ARM-Class object
SLOW const T_ARM arm =
{
    {
        &armState,
        &armDataSemaphore,

        armObjectList,
        //lint -e{778}
        sizeof(armObjectList)/sizeof(armObjectList[0])-1,

        armDataClassList,
        sizeof(armDataClassList)/sizeof(armDataClassList[0]),

        ChangeState_T_UNIT,
        GetState_T_UNIT,
        Initialize_ARM, // will be overloaded
        LoadRomDefaults_T_UNIT,
        ResetToDefault_T_UNIT,
        SaveAsDefault_T_UNIT,
		#ifdef FRAMEWORK_VERSION_2_1_0
        LoadAll_T_UNIT,
        SaveAll_T_UNIT,
		#endif // FRAMEWORK_VERSION_2_1_0
		#ifdef FRAMEWORK_VERSION_2_2_0
        LoadAll_T_UNIT,
        SaveAll_T_UNIT,
		#endif // FRAMEWORK_VERSION_2_2_0
        Get_ARM,	// overloaded to allow HMI to read transmitter Id string form
        Put_ARM,   // overloaded, to block access via Put() method to externals
        Check_T_UNIT,
        GetAttributeDescription_T_UNIT,
        GetObjectDescription_T_UNIT
    }

    //subsystem EXE and SRV-methods
    //@SubGen start@    <METHODS>
    ,LogInEXE_ARM
    ,LogOffEXE_ARM
    ,CheckAccessRightsSRV_ARM
    ,ResetPasswordsEXE_ARM
    ,ChangePasswordEXE_ARM
    //@SubGen end@    <METHODS>
};




/*

This section defines templates for the SubSystem CodeGenerator.
Do not modify or remove this section

see also: arm_attributes.h


//special templates for specific standard storage classes:
//@SubGen template start@<DATACLASS CONSTANT>
//-------------------------------------------------------------------------------------------------
//! definition of constant block
const SLOW T_ARM_TEMPLATE armTemplate=
{
    //@SubGen start@    <INITIALIZER TEMPLATE>
    //@SubGen end@    <INITIALIZER TEMPLATE>
};
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS CONSTANT>



//@SubGen template start@<DATACLASS DYNAMIC>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
FAST T_ARM_TEMPLATE armTemplate;
const SLOW T_ARM_TEMPLATE armTemplateInitializer=
{
    //@SubGen start@    <INITIALIZER TEMPLATE>
    //@SubGen end@    <INITIALIZER TEMPLATE>
};
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS DYNAMIC>



//@SubGen template start@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>
//-------------------------------------------------------------------------------------------------
//! definition of Template data
#ifdef WIN32
  #pragma bss_seg(".AAA$ARM_DATA")
    const FAST T_ARM_TEMPLATE armTemplate;
    const FAST T_ARM_TEMPLATE armTemplateDefault;
  #pragma bss_seg()
#else
  #pragma location="ARM_TEMPLATE"
    const FAST __no_init T_ARM_TEMPLATE armTemplate;
  #pragma location="ARM_TEMPLATE_DEFAULT"
    const FAST __no_init T_ARM_TEMPLATE armTemplateDefault;
#endif
const SLOW T_ARM_TEMPLATE armTemplateInitializer=
{
    //@SubGen start@    <INITIALIZER TEMPLATE>
    //@SubGen end@    <INITIALIZER TEMPLATE>
};
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS STATIC_FREQUENT_STATIC_DEFAULT>


//this template is for all other standard storage classes:

//@SubGen template start@<DATACLASS>
//-------------------------------------------------------------------------------------------------
//! definition of Template data
#ifdef WIN32
  #pragma bss_seg(".AAA$ARM_DATA")
    const FAST T_ARM_TEMPLATE armTemplate;
  #pragma bss_seg()
#else
  #pragma location="ARM_TEMPLATE"
    const FAST __no_init T_ARM_TEMPLATE armTemplate;
#endif
const SLOW T_ARM_TEMPLATE armTemplateInitializer=
{
    //@SubGen start@    <INITIALIZER TEMPLATE>
    //@SubGen end@    <INITIALIZER TEMPLATE>
};
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS>

//@SubGen template start@ <DATACLASSNAME>
armTemplate
//@SubGen template end@ <DATACLASSNAME>


//@SubGen template start@ <DATACLASSLIST DYNAMIC>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_DYNAMIC(armTemplate,armTemplateInitializer)
//@SubGen template end@ <DATACLASSLIST DYNAMIC>

//@SubGen template start@ <DATACLASSLIST CONSTANT>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_CONSTANT(armTemplate)
//@SubGen template end@ <DATACLASSLIST CONSTANT>

//@SubGen template start@ <DATACLASSLIST>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_STATIC_NODEF(armTemplate,armTemplateInitializer)
//@SubGen template end@ <DATACLASSLIST>

//@SubGen template start@ <DATACLASSLIST STATIC_FREQUENT_CONSTANT_DEFAULT>
//lint -e{708} initialization is OK
CONSTRUCTOR_DC_STATIC_CONSTDEF(armTemplate,armTemplateInitializer)
//@SubGen template end@ <DATACLASSLIST STATIC_FREQUENT_CONSTANT_DEFAULT>

//@SubGen template start@ <DATACLASSLIST STATIC_FREQUENT_STATIC_DEFAULT>
//lint -e{708} initialization is OK
CONSTRUCTOR_DC_STATIC_STATDEF(armTemplate,armTemplateInitializer,armTemplateDefault)
//@SubGen template end@ <DATACLASSLIST STATIC_FREQUENT_STATIC_DEFAULT>

*/


// M. Ashford - MISRA defaults
#pragma diag_default=Pm022


// M. Ashford - Enable MISRA Diagnostics
#pragma diag_default=Pm023
