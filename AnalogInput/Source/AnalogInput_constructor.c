//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem AnalogInput
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
// include the data_obj-classes you need, e.g. T_DATA_OBJ::INT16
//#include "..\..\t_data_obj\interface\t_data_obj_int16.h"
#include "T_DATA_OBJ/Interface/t_data_obj_tabenum8.h"
#include "T_DATA_OBJ/Interface/t_data_obj_float.h"
#include "T_DATA_OBJ/Interface/t_data_obj_simple.h"
#include "T_Unit/Interface/t_unit.h"
#include "T_Unit/Interface/T_DATACLASSES.H"
#include "AnalogInput/Interface/AnalogInput.h"
#include "analogInput_types.h"
#include "analogInput_attributes.h"
#include "analogInput_overload.h"
#include "analogInput_execute.h"

#include "Diagnosis/interface/t_data_obj_alarm.h"

extern const SLOW T_DATA_OBJ analogInputObjectList[];

//-------------------------------------------------------------------------------------------------
//! unit global attributes
FAST TUSIGN16 analogInputState;     // Subsystem state
FAST T_DATA_SEMAPHORE analogInputDataSemaphore;



const SLOW T_DATACLASS analogInputDataClassList[]=
{
    //lint -e{651} Initializers are OK, the warning pop up with the first 708
    //@SubGen start@    <DATACLASSLIST>
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_DYNAMIC(analogInputUnprotected,analogInputUnprotectedInitializer),
    //lint -e{708} initialization is OK
    CONSTRUCTOR_DC_STATIC_STATDEF(analogInputStaticFreqStaticDefalutCalibration,analogInputStaticFreqStaticDefalutCalibrationInitializer,analogInputStaticFreqStaticDefalutCalibrationDefault),
    //lint -e{708} initialization is OK
    CONSTRUCTOR_DC_STATIC_STATDEF(analogInputStaticFrequentStaticDefault,analogInputStaticFrequentStaticDefaultInitializer,analogInputStaticFrequentStaticDefaultDefault)
    //@SubGen end@    <DATACLASSLIST>
};

//@SubGen put@<DATACLASS>
//-------------------------------------------------------------------------------------------------
//! definition of StaticFreqStaticDefalutCalibration data
#ifdef WIN32
  #pragma bss_seg(".AAA$ANALOG_INPUT_DATA")
    const FAST T_ANALOG_INPUT_STATIC_FREQ_STATIC_DEFALUT_CALIBRATION analogInputStaticFreqStaticDefalutCalibration;
    const FAST T_ANALOG_INPUT_STATIC_FREQ_STATIC_DEFALUT_CALIBRATION analogInputStaticFreqStaticDefalutCalibrationDefault;
  #pragma bss_seg()
#else
  #pragma location="ANALOG_INPUT_STATIC_FREQ_STATIC_DEFALUT_CALIBRATION"
    const FAST __no_init T_ANALOG_INPUT_STATIC_FREQ_STATIC_DEFALUT_CALIBRATION analogInputStaticFreqStaticDefalutCalibration;
  #pragma location="ANALOG_INPUT_STATIC_FREQ_STATIC_DEFALUT_CALIBRATION_DEFAULT"
    const FAST __no_init T_ANALOG_INPUT_STATIC_FREQ_STATIC_DEFALUT_CALIBRATION analogInputStaticFreqStaticDefalutCalibrationDefault;
#endif
const SLOW T_ANALOG_INPUT_STATIC_FREQ_STATIC_DEFALUT_CALIBRATION analogInputStaticFreqStaticDefalutCalibrationInitializer=
{
    //@SubGen start@    <INITIALIZER STATIC_FREQ_STATIC_DEFALUT_CALIBRATION>
    {1.0f,0.0f}    // SIMPLE_FLOAT baseCalibration[2]
    //@SubGen end@    <INITIALIZER STATIC_FREQ_STATIC_DEFALUT_CALIBRATION>
};
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
FAST T_ANALOG_INPUT_UNPROTECTED analogInputUnprotected;
const SLOW T_ANALOG_INPUT_UNPROTECTED analogInputUnprotectedInitializer=
{
    //@SubGen start@    <INITIALIZER UNPROTECTED>
    0.0f,    // FLOAT aiInValue
    0,    // T_DIAGNOSIS_ALARM aiAlarmSimulation
    0,    // SIMPLE_U8 aiDiagnosis
    4.0f,    // FLOAT aiSimulationData
    4.01f,    // FLOAT aiTempCurr
    4.0f,    // SIMPLE_FLOAT trimAiLowerEndpoint
    20.0f,    // SIMPLE_FLOAT trimAiUpperEndpoint
    AI_CALIBRATION_OFF,    // TABENUM8 calibrationState
    4.01f,    // FLOAT aiRawCurr
    AI_EXTERNAL_UNCUT_OFF    // TABENUM8 aiExternalCutOffStatus
    //@SubGen end@    <INITIALIZER UNPROTECTED>
};
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//! definition of StaticFrequentStaticDefault data
#ifdef WIN32
  #pragma bss_seg(".AAA$ANALOG_INPUT_DATA")
    const FAST T_ANALOG_INPUT_STATIC_FREQUENT_STATIC_DEFAULT analogInputStaticFrequentStaticDefault;
    const FAST T_ANALOG_INPUT_STATIC_FREQUENT_STATIC_DEFAULT analogInputStaticFrequentStaticDefaultDefault;
  #pragma bss_seg()
#else
  #pragma location="ANALOG_INPUT_STATIC_FREQUENT_STATIC_DEFAULT"
    const FAST __no_init T_ANALOG_INPUT_STATIC_FREQUENT_STATIC_DEFAULT analogInputStaticFrequentStaticDefault;
  #pragma location="ANALOG_INPUT_STATIC_FREQUENT_STATIC_DEFAULT_DEFAULT"
    const FAST __no_init T_ANALOG_INPUT_STATIC_FREQUENT_STATIC_DEFAULT analogInputStaticFrequentStaticDefaultDefault;
#endif
const SLOW T_ANALOG_INPUT_STATIC_FREQUENT_STATIC_DEFAULT analogInputStaticFrequentStaticDefaultInitializer=
{
    //@SubGen start@    <INITIALIZER STATIC_FREQUENT_STATIC_DEFAULT>
    AI_WITHOUT,    // TABENUM8 aiSelection
    AI_EXTERNAL_CUT_OFF_4MA    // TABENUM8 aiExternalCutOffOption
    //@SubGen end@    <INITIALIZER STATIC_FREQUENT_STATIC_DEFAULT>
};
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//! object check and PutGet descriptions
//@SubGen start@<DESCRIPTORS>
const SLOW TUSIGN8 _AI_SELECT[] =
// Table referenced by: aiSelection
{
  AI_EXT_TEMPERATURE,
  AI_PRESSURE,
  AI_PRESSURE_ABS,
  AI_DENSITY,
  AI_GAS_CONTENT,
  AI_EXTERNAL_OUTPUT_CUTOFF,
  AI_WITHOUT,
  AI_INTER_TEMPERATURE
};

const SLOW TUSIGN8 _AI_DIAGNOSIS_ALARM_TABLE[] =
// Table referenced by: aiAlarmSimulation
{
  AI_ALM_SPICOM_FAIL,
  AI_ALM_OUT_OF_RANGE,
  AI_ALM_CUT_OFF
};

const SLOW TUSIGN8 _CALIBRATION_STATE[] =
// Table referenced by: calibrationState
{
  AI_CALIBRATION_OFF,
  AI_CALIBRATION_START,
  AI_CALIBRATION_STOP
};

const SLOW TUSIGN8 _EXTERNAL_CUT_OFF_STATUS[] =
// Table referenced by: aiExternalCutOffStatus
{
  AI_EXTERNAL_UNCUT_OFF,
  AI_EXTERNAL_CUT_OFF
};

const SLOW TUSIGN8 _EXTERNAL_CUT_OFF_OPTION[] =
// Table referenced by: aiExternalCutOffOption
{
  AI_EXTERNAL_CUT_OFF_4MA,
  AI_EXTERNAL_CUT_OFF_8MA,
  AI_EXTERNAL_CUT_OFF_12MA
};

static const SLOW T_RANGE_E8 aiSelectionDescriptor =
{
  _AI_SELECT,  // TUSIGN8* ptrCodeTab
  sizeof(_AI_SELECT)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_FLOAT aiInValueDescriptor =
{
  -500000.0f,  // TFLOAT min
  500000.0f,  // TFLOAT max
  0.0001f  // TFLOAT increment
};

const SLOW T_RANGE_E8 aiAlarmSimulationDescriptor =
{
  _AI_DIAGNOSIS_ALARM_TABLE,  // TUSIGN8* ptrCodeTab
  sizeof(_AI_DIAGNOSIS_ALARM_TABLE)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_FLOAT aiSimulationDataDescriptor =
{
  0.0f,  // TFLOAT min
  30.0f,  // TFLOAT max
  0.001f  // TFLOAT increment
};

static const SLOW T_RANGE_FLOAT aiTempCurrDescriptor =
{
  0.0f,  // TFLOAT min
  26.0f,  // TFLOAT max
  0.001f  // TFLOAT increment
};

static const SLOW T_RANGE_E8 calibrationStateDescriptor =
{
  _CALIBRATION_STATE,  // TUSIGN8* ptrCodeTab
  sizeof(_CALIBRATION_STATE)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_FLOAT aiRawCurrDescriptor =
{
  1.0f,  // TFLOAT min
  26.0f,  // TFLOAT max
  0.001f  // TFLOAT increment
};

static const SLOW T_RANGE_E8 aiExternalCutOffStatusDescriptor =
{
  _EXTERNAL_CUT_OFF_STATUS,  // TUSIGN8* ptrCodeTab
  sizeof(_EXTERNAL_CUT_OFF_STATUS)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 aiExternalCutOffOptionDescriptor =
{
  _EXTERNAL_CUT_OFF_OPTION,  // TUSIGN8* ptrCodeTab
  sizeof(_EXTERNAL_CUT_OFF_OPTION)  // TUSIGN8 numberOfCodes
};
//@SubGen end@<DESCRIPTORS>


//-------------------------------------------------------------------------------------------------
//! objectList
const SLOW T_DATA_OBJ analogInputObjectList[] =
{
    //lint -e605 All non-volatile data are const for T_UNIT
    //           and the typecast (void FAST*) does not increase the readability
    //@SubGen start@    <OBJECTS>
    //aiSelection
    CONSTRUCT_TABENUM8(
        &analogInputStaticFrequentStaticDefault.aiSelection,
        NON_VOLATILE,
        &aiSelectionDescriptor),

    //aiInValue
    CONSTRUCT_FLOAT(
        &analogInputUnprotected.aiInValue,
        RAM,
        &aiInValueDescriptor),

    //aiAlarmSimulation
    CONSTRUCT_ALARM(
        &analogInputUnprotected.aiAlarmSimulation,
        sizeof(T_DIAGNOSIS_ALARM),
        RAM,
        &aiAlarmSimulationDescriptor),

    //aiDiagnosis
    CONSTRUCT_SIMPLE_U8(
        &analogInputUnprotected.aiDiagnosis,
        RAM),

    //aiSimulationData
    CONSTRUCT_FLOAT(
        &analogInputUnprotected.aiSimulationData,
        RAM,
        &aiSimulationDataDescriptor),

    //aiTempCurr
    CONSTRUCT_FLOAT(
        &analogInputUnprotected.aiTempCurr,
        RAM,
        &aiTempCurrDescriptor),

    //baseCalibration
    CONSTRUCT_ARRAY_SIMPLE_FLOAT(
        analogInputStaticFreqStaticDefalutCalibration.baseCalibration,
        2,
        NON_VOLATILE),

    //trimAiLowerEndpoint
    CONSTRUCT_SIMPLE_FLOAT(
        &analogInputUnprotected.trimAiLowerEndpoint,
        RAM),

    //trimAiUpperEndpoint
    CONSTRUCT_SIMPLE_FLOAT(
        &analogInputUnprotected.trimAiUpperEndpoint,
        RAM),

    //calibrationState
    CONSTRUCT_TABENUM8(
        &analogInputUnprotected.calibrationState,
        RAM,
        &calibrationStateDescriptor),

    //aiRawCurr
    CONSTRUCT_FLOAT(
        &analogInputUnprotected.aiRawCurr,
        RAM,
        &aiRawCurrDescriptor),

    //aiExternalCutOffStatus
    CONSTRUCT_TABENUM8(
        &analogInputUnprotected.aiExternalCutOffStatus,
        RAM,
        &aiExternalCutOffStatusDescriptor),

    //aiExternalCutOffOption
    CONSTRUCT_TABENUM8(
        &analogInputStaticFrequentStaticDefault.aiExternalCutOffOption,
        NON_VOLATILE,
        &aiExternalCutOffOptionDescriptor)
    //@SubGen end@    <OBJECTS>
    //lint +e605 warning on for the rest of the code
};



//-------------------------------------------------------------------------------------------------
//! define the ANALOG_INPUT-Class object
SLOW const T_ANALOG_INPUT analogInput =
{
    {
        &analogInputState,
        &analogInputDataSemaphore,

        analogInputObjectList,
        //lint -e{961} 
        sizeof(analogInputObjectList)/sizeof(analogInputObjectList[0])-1,

        analogInputDataClassList,
        sizeof(analogInputDataClassList)/sizeof(analogInputDataClassList[0]),

        ChangeState_T_UNIT,
        GetState_T_UNIT,
        Initialize_ANALOG_INPUT, // will be overloaded
        LoadRomDefaults_AI,      // will be overloaded
        ResetToDefault_T_UNIT,
        SaveAsDefault_T_UNIT,
        Get_T_UNIT,
        Put_T_AI,//overload
        Check_T_UNIT,
        GetAttributeDescription_T_UNIT,
        GetObjectDescription_T_UNIT
    }

    //subsystem EXE and SRV-methods
    //@SubGen start@    <METHODS>
    ,UpdateAnalogInput_AI
    ,UpdateDiagnosis_AI
    ,IsExternalCutOff_AI
    //@SubGen end@    <METHODS>
};




/*

This section defines templates for the SubSystem CodeGenerator.
Do not modify or remove this section

see also: analogInput_attributes.h


//special templates for specific standard storage classes:
//@SubGen template start@<DATACLASS CONSTANT>
//-------------------------------------------------------------------------------------------------
//! definition of constant block
const SLOW T_ANALOG_INPUT_TEMPLATE analogInputTemplate=
{
    //@SubGen start@    <INITIALIZER TEMPLATE>
    //@SubGen end@    <INITIALIZER TEMPLATE>
};
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS CONSTANT>



//@SubGen template start@<DATACLASS DYNAMIC>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
FAST T_ANALOG_INPUT_TEMPLATE analogInputTemplate;
const SLOW T_ANALOG_INPUT_TEMPLATE analogInputTemplateInitializer=
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
  #pragma bss_seg(".AAA$ANALOG_INPUT_DATA")
    const FAST T_ANALOG_INPUT_TEMPLATE analogInputTemplate;
    const FAST T_ANALOG_INPUT_TEMPLATE analogInputTemplateDefault;
  #pragma bss_seg()
#else
  #pragma location="ANALOG_INPUT_TEMPLATE"
    const FAST __no_init T_ANALOG_INPUT_TEMPLATE analogInputTemplate;
  #pragma location="ANALOG_INPUT_TEMPLATE_DEFAULT"
    const FAST __no_init T_ANALOG_INPUT_TEMPLATE analogInputTemplateDefault;
#endif
const SLOW T_ANALOG_INPUT_TEMPLATE analogInputTemplateInitializer=
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
  #pragma bss_seg(".AAA$ANALOG_INPUT_DATA")
    const FAST T_ANALOG_INPUT_TEMPLATE analogInputTemplate;
  #pragma bss_seg()
#else
  #pragma location="ANALOG_INPUT_TEMPLATE"
    const FAST __no_init T_ANALOG_INPUT_TEMPLATE analogInputTemplate;
#endif
const SLOW T_ANALOG_INPUT_TEMPLATE analogInputTemplateInitializer=
{
    //@SubGen start@    <INITIALIZER TEMPLATE>
    //@SubGen end@    <INITIALIZER TEMPLATE>
};
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS>

//@SubGen template start@ <DATACLASSNAME>
analogInputTemplate
//@SubGen template end@ <DATACLASSNAME>


//@SubGen template start@ <DATACLASSLIST DYNAMIC>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_DYNAMIC(analogInputTemplate,analogInputTemplateInitializer)
//@SubGen template end@ <DATACLASSLIST DYNAMIC>

//@SubGen template start@ <DATACLASSLIST CONSTANT>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_CONSTANT(analogInputTemplate)
//@SubGen template end@ <DATACLASSLIST CONSTANT>

//@SubGen template start@ <DATACLASSLIST>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_STATIC_NODEF(analogInputTemplate,analogInputTemplateInitializer)
//@SubGen template end@ <DATACLASSLIST>

//@SubGen template start@ <DATACLASSLIST STATIC_FREQUENT_CONSTANT_DEFAULT>
//lint -e{708} initialization is OK
CONSTRUCTOR_DC_STATIC_CONSTDEF(analogInputTemplate,analogInputTemplateInitializer)
//@SubGen template end@ <DATACLASSLIST STATIC_FREQUENT_CONSTANT_DEFAULT>

//@SubGen template start@ <DATACLASSLIST STATIC_FREQUENT_STATIC_DEFAULT>
//lint -e{708} initialization is OK
CONSTRUCTOR_DC_STATIC_STATDEF(analogInputTemplate,analogInputTemplateInitializer,analogInputTemplateDefault)
//@SubGen template end@ <DATACLASSLIST STATIC_FREQUENT_STATIC_DEFAULT>

*/
