//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem FrontEndInterface
 Module
 Description    Subsystem constructor
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_DATA_OBJ/Interface/t_data_obj_simple.h"

#include "T_Unit/Interface/t_unit.h"
#include "T_Unit/Interface/T_DATACLASSES.H"

#include "FrontEndInterface/Interface/FrontEndInterface.h"
#include "frontEndInterface_types.h"
#include "frontEndInterface_attributes.h"
#include "frontEndInterface_overload.h"
#include "frontEndInterface_execute.h"

#include "Diagnosis/interface/t_data_obj_alarm.h"

//-------------------------------------------------------------------------------------------------
//! unit global attributes
FAST TUSIGN16 frontEndInterfaceState;     // Subsystem state
FAST T_DATA_SEMAPHORE frontEndInterfaceDataSemaphore;



const SLOW T_DATACLASS frontEndInterfaceDataClassList[]=
{
    //lint -e{651} Initializers are OK, the warning pop up with the first 708
    //@SubGen start@    <DATACLASSLIST>
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_DYNAMIC(frontEndInterfaceDynamicDuplicated,frontEndInterfaceDynamicDuplicatedInitializer)
    //@SubGen end@    <DATACLASSLIST>
};


//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
FAST T_FRONT_END_INTERFACE_DYNAMIC_DUPLICATED frontEndInterfaceDynamicDuplicated;
const SLOW T_FRONT_END_INTERFACE_DYNAMIC_DUPLICATED frontEndInterfaceDynamicDuplicatedInitializer=
{
    //@SubGen start@    <INITIALIZER DYNAMIC_DUPLICATED>
    0,    // SIMPLE_U8 frontendDiagnosis
    0,    // T_DIAGNOSIS_ALARM frontendAlarmSimulation
    0,    // SIMPLE_U8 frontendDiagnosisDuplicated
    0    // T_DIAGNOSIS_ALARM frontendAlarmSimulationDuplicated
    //@SubGen end@    <INITIALIZER DYNAMIC_DUPLICATED>
};
//-------------------------------------------------------------------------------------------------





//-------------------------------------------------------------------------------------------------
//! object check and PutGet descriptions
//@SubGen start@<DESCRIPTORS>
const SLOW TUSIGN8 _FE_DIAGNOSIS_ALARM_TABLE[] =
// Table referenced by: frontendAlarmSimulation, frontendAlarmSimulationDuplicated
{
  FE_BOARD_COMM_ERROR
};

const SLOW T_RANGE_E8 frontendAlarmSimulationDescriptor =
{
  _FE_DIAGNOSIS_ALARM_TABLE,  // TUSIGN8* ptrCodeTab
  FE_FE_DIAG_ALARM_SIZE  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 frontendAlarmSimulationDuplicatedDescriptor =
{
  _FE_DIAGNOSIS_ALARM_TABLE,  // TUSIGN8* ptrCodeTab
  sizeof(_FE_DIAGNOSIS_ALARM_TABLE)  // TUSIGN8 numberOfCodes
};
//@SubGen end@<DESCRIPTORS>


//-------------------------------------------------------------------------------------------------
//! objectList
const SLOW T_DATA_OBJ frontEndInterfaceObjectList[] =
{
    //lint -e605 All non-volatile data are const for T_UNIT
    //           and the typecast (void FAST*) does not increase the readability
    //@SubGen start@    <OBJECTS>
    //frontendDiagnosis
    CONSTRUCT_SIMPLE_U8(
        &frontEndInterfaceDynamicDuplicated.frontendDiagnosis,
        RAM),

    //frontendAlarmSimulation
    CONSTRUCT_ALARM(
        &frontEndInterfaceDynamicDuplicated.frontendAlarmSimulation,
        sizeof(T_DIAGNOSIS_ALARM),
        RAM,
        &frontendAlarmSimulationDescriptor),

    //frontendDiagnosisDuplicated
    CONSTRUCT_SIMPLE_U8(
        &frontEndInterfaceDynamicDuplicated.frontendDiagnosisDuplicated,
        RAM),

    //frontendAlarmSimulationDuplicated
    CONSTRUCT_ALARM(
        &frontEndInterfaceDynamicDuplicated.frontendAlarmSimulationDuplicated,
        sizeof(T_DIAGNOSIS_ALARM),
        RAM,
        &frontendAlarmSimulationDuplicatedDescriptor)
    //@SubGen end@    <OBJECTS>
    //lint +e605 warning on for the rest of the code
};



//-------------------------------------------------------------------------------------------------
//! define the FRONT_END_INTERFACE-Class object
SLOW const T_FRONT_END_INTERFACE frontEndInterface =
{
    {
        &frontEndInterfaceState,
        &frontEndInterfaceDataSemaphore,

        frontEndInterfaceObjectList,
        //lint -e{778} -e{961}
        sizeof(frontEndInterfaceObjectList) /sizeof(frontEndInterfaceObjectList[0])-1,

        frontEndInterfaceDataClassList,
        sizeof(frontEndInterfaceDataClassList)/sizeof(frontEndInterfaceDataClassList[0]),

        ChangeState_T_UNIT,
        GetState_T_UNIT,
        Initialize_FRONT_END_INTERFACE, // will be overloaded
        LoadRomDefaults_T_UNIT,
        ResetToDefault_T_UNIT,
        SaveAsDefault_T_UNIT,
        Get_T_UNIT,
        Put_FRONT_END_INTERFACE,    //overload
        Check_T_UNIT,
        GetAttributeDescription_T_UNIT,
        GetObjectDescription_T_UNIT
    }

    //subsystem EXE and SRV-methods
    //@SubGen start@    <METHODS>
    ,UpdateDiagnosis_FE
    ,SetDiagnosis_FE
    ,ClearDiagnosis_FE
    ,InterpreterCmd_FE
    ,ProcessComData_FE
    ,ReadComCmd_FE
    ,WriteComCmd_FE
    ,ProcessCmd_FE
    ,PutCommand_FE
    ,SendCmd_FE
    ,UpdateDiagData_FE
    ,CheckFEreadySRV_FE
    //@SubGen end@    <METHODS>
};




/*

This section defines templates for the SubSystem CodeGenerator.
Do not modify or remove this section

see also: frontEndInterface_attributes.h


//special templates for specific standard storage classes:
//@SubGen template start@<DATACLASS CONSTANT>
//-------------------------------------------------------------------------------------------------
//! definition of constant block
const SLOW T_FRONT_END_INTERFACE_TEMPLATE frontEndInterfaceTemplate=
{
    //@SubGen start@    <INITIALIZER TEMPLATE>
    //@SubGen end@    <INITIALIZER TEMPLATE>
};
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS CONSTANT>



//@SubGen template start@<DATACLASS DYNAMIC>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
FAST T_FRONT_END_INTERFACE_TEMPLATE frontEndInterfaceTemplate;
const SLOW T_FRONT_END_INTERFACE_TEMPLATE frontEndInterfaceTemplateInitializer=
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
  #pragma bss_seg(".AAA$FRONT_END_INTERFACE_DATA")
    const FAST T_FRONT_END_INTERFACE_TEMPLATE frontEndInterfaceTemplate;
    const FAST T_FRONT_END_INTERFACE_TEMPLATE frontEndInterfaceTemplateDefault;
  #pragma bss_seg()
#else
  #pragma location="FRONT_END_INTERFACE_TEMPLATE"
    const FAST __no_init T_FRONT_END_INTERFACE_TEMPLATE frontEndInterfaceTemplate;
  #pragma location="FRONT_END_INTERFACE_TEMPLATE_DEFAULT"
    const FAST __no_init T_FRONT_END_INTERFACE_TEMPLATE frontEndInterfaceTemplateDefault;
#endif
const SLOW T_FRONT_END_INTERFACE_TEMPLATE frontEndInterfaceTemplateInitializer=
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
  #pragma bss_seg(".AAA$FRONT_END_INTERFACE_DATA")
    const FAST T_FRONT_END_INTERFACE_TEMPLATE frontEndInterfaceTemplate;
  #pragma bss_seg()
#else
  #pragma location="FRONT_END_INTERFACE_TEMPLATE"
    const FAST __no_init T_FRONT_END_INTERFACE_TEMPLATE frontEndInterfaceTemplate;
#endif
const SLOW T_FRONT_END_INTERFACE_TEMPLATE frontEndInterfaceTemplateInitializer=
{
    //@SubGen start@    <INITIALIZER TEMPLATE>
    //@SubGen end@    <INITIALIZER TEMPLATE>
};
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS>

//@SubGen template start@ <DATACLASSNAME>
frontEndInterfaceTemplate
//@SubGen template end@ <DATACLASSNAME>


//@SubGen template start@ <DATACLASSLIST DYNAMIC>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_DYNAMIC(frontEndInterfaceTemplate,frontEndInterfaceTemplateInitializer)
//@SubGen template end@ <DATACLASSLIST DYNAMIC>

//@SubGen template start@ <DATACLASSLIST CONSTANT>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_CONSTANT(frontEndInterfaceTemplate)
//@SubGen template end@ <DATACLASSLIST CONSTANT>

//@SubGen template start@ <DATACLASSLIST>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_STATIC_NODEF(frontEndInterfaceTemplate,frontEndInterfaceTemplateInitializer)
//@SubGen template end@ <DATACLASSLIST>

//@SubGen template start@ <DATACLASSLIST STATIC_FREQUENT_CONSTANT_DEFAULT>
//lint -e{708} initialization is OK
CONSTRUCTOR_DC_STATIC_CONSTDEF(frontEndInterfaceTemplate,frontEndInterfaceTemplateInitializer)
//@SubGen template end@ <DATACLASSLIST STATIC_FREQUENT_CONSTANT_DEFAULT>

//@SubGen template start@ <DATACLASSLIST STATIC_FREQUENT_STATIC_DEFAULT>
//lint -e{708} initialization is OK
CONSTRUCTOR_DC_STATIC_STATDEF(frontEndInterfaceTemplate,frontEndInterfaceTemplateInitializer,frontEndInterfaceTemplateDefault)
//@SubGen template end@ <DATACLASSLIST STATIC_FREQUENT_STATIC_DEFAULT>

*/

