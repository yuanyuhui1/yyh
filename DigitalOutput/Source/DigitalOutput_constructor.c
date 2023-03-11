//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem DigitalOutput
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
#include "T_DATA_OBJ/Interface/t_data_obj_units.h"
#include "T_DATA_OBJ/Interface/t_data_obj_floatunits.h"
#include "T_DATA_OBJ/Interface/t_data_obj_floatfloatunits.h"
#include "T_DATA_OBJ/Interface/t_data_obj_float_fixedunit.h"
#include "T_DATA_OBJ/Interface/t_data_obj_floatrange_fixedunit.h"
#include "T_DATA_OBJ/Interface/t_data_obj_int16.h"
#include "T_DATA_OBJ/Interface/t_data_obj_simple.h"
#include "T_DATA_OBJ/Interface/t_data_obj_tabenum8.h"
#include "T_DATA_OBJ/Interface/t_data_obj_float.h"
#include "Diagnosis/interface/t_data_obj_alarm.h"
#include "T_Unit/Interface/t_unit.h"
#include "UnitsOfMeasure/Interface/UnitsOfMeasure.h"
#include "T_Unit/Interface/T_DATACLASSES.H"
#include "DigitalOutput/Interface/DigitalOutput.h"
#include "DigitalOutput/Interface/DigitalOutput_idx.h"
#include "digitalOutput_logic.h"
#include "digitaloutput_pulse.h"
#include "digitalOutput_types.h"
#include "digitalOutput_attributes.h"
#include "digitalOutput_overload.h"
#include "digitalOutput_execute.h"




// Forward declaration ObjectList
extern const SLOW T_DATA_OBJ digitalOutputObjectList[];

//-------------------------------------------------------------------------------------------------
//! unit global attributes
FAST TUSIGN16 digitalOutputState;     // Subsystem state
FAST T_DATA_SEMAPHORE digitalOutputDataSemaphore;



const SLOW T_DATACLASS digitalOutputDataClassList[]=
{
    //lint -e{651} Initializers are OK, the warning pop up with the first 708
    //@SubGen start@    <DATACLASSLIST>
    //lint -e{708} initialization is OK
    CONSTRUCTOR_DC_STATIC_CONSTDEF(digitalOutputStaticFrequentConstantDefault,digitalOutputStaticFrequentConstantDefaultInitializer),
    //lint -e{708} initialization is OK
    CONSTRUCTOR_DC_STATIC_STATDEF(digitalOutputREPLACE_STATIC_FREQUENT_STATIC_DEFAULT,digitalOutputREPLACE_STATIC_FREQUENT_STATIC_DEFAULTInitializer,digitalOutputREPLACE_STATIC_FREQUENT_STATIC_DEFAULTDefault),
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_DYNAMIC(digitalOutputDynamic,digitalOutputDynamicInitializer)
    //@SubGen end@    <DATACLASSLIST>
};

//@SubGen put@<DATACLASS>
//-------------------------------------------------------------------------------------------------
//! definition of REPLACE_STATIC_FREQUENT_STATIC_DEFAULT data
#ifdef WIN32
  #pragma bss_seg(".AAA$DIGITAL_OUTPUT_DATA")
    const FAST T_DIGITAL_OUTPUT_REPLACE_STATIC_FREQUENT_STATIC_DEFAULT digitalOutputREPLACE_STATIC_FREQUENT_STATIC_DEFAULT;
    const FAST T_DIGITAL_OUTPUT_REPLACE_STATIC_FREQUENT_STATIC_DEFAULT digitalOutputREPLACE_STATIC_FREQUENT_STATIC_DEFAULTDefault;
  #pragma bss_seg()
#else
  #pragma location="DIGITAL_OUTPUT_REPLACE_STATIC_FREQUENT_STATIC_DEFAULT"
    const FAST __no_init T_DIGITAL_OUTPUT_REPLACE_STATIC_FREQUENT_STATIC_DEFAULT digitalOutputREPLACE_STATIC_FREQUENT_STATIC_DEFAULT;
  #pragma location="DIGITAL_OUTPUT_REPLACE_STATIC_FREQUENT_STATIC_DEFAULT_DEFAULT"
    const FAST __no_init T_DIGITAL_OUTPUT_REPLACE_STATIC_FREQUENT_STATIC_DEFAULT digitalOutputREPLACE_STATIC_FREQUENT_STATIC_DEFAULTDefault;
#endif
const SLOW T_DIGITAL_OUTPUT_REPLACE_STATIC_FREQUENT_STATIC_DEFAULT digitalOutputREPLACE_STATIC_FREQUENT_STATIC_DEFAULTInitializer=
{
    //@SubGen start@    <INITIALIZER REPLACE_STATIC_FREQUENT_STATIC_DEFAULT>
    DIGITALOUTPUT_DO1_NONE,    // TABENUM8 outputMode
    DIGITALOUTPUT_NO_LOGIC_SOURCE,    // TABENUM8 lgcSgnlSrc
    DIGITALOUTPUT_ACTIVE_HIGH,    // TABENUM8 lgcActState
    DIGITALOUTPUT_COMMAND_OFF,    // TABENUM8 lgcSimCmd
    {DIGITALOUTPUT_PULSE_WIDTH_DEFAULT,UM_MILLISECOND,UM_T_TIME},    // FLOAT_FIXEDUNIT pulseWidth {value,unitsCode,unitsType}
    {DIGITALOUTPUT_PULSE_FACTOR_DEFAULT,0,0},    // FLOAT_FIXEDUNIT pulseFactor {value,unitsCode,unitsType}
    {DIGITALOUTPUT_OUTPUTFREQ_MIN,0,0},    // FLOAT_FIXEDUNIT minFrequency {value,unitsCode,unitsType}
    {DIGITALOUTPUT_OUTPUTFREQ_MAX,0,0}    // FLOAT_FIXEDUNIT maxFrequency {value,unitsCode,unitsType}
    //@SubGen end@    <INITIALIZER REPLACE_STATIC_FREQUENT_STATIC_DEFAULT>
};
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//! definition of StaticFrequentConstantDefault data
#ifdef WIN32
  #pragma bss_seg(".AAA$DIGITAL_OUTPUT_DATA")
    const FAST T_DIGITAL_OUTPUT_STATIC_FREQUENT_CONSTANT_DEFAULT digitalOutputStaticFrequentConstantDefault;
  #pragma bss_seg()
#else
  #pragma location="DIGITAL_OUTPUT_STATIC_FREQUENT_CONSTANT_DEFAULT"
    const FAST __no_init T_DIGITAL_OUTPUT_STATIC_FREQUENT_CONSTANT_DEFAULT digitalOutputStaticFrequentConstantDefault;
#endif
const SLOW T_DIGITAL_OUTPUT_STATIC_FREQUENT_CONSTANT_DEFAULT digitalOutputStaticFrequentConstantDefaultInitializer=
{
    //@SubGen start@    <INITIALIZER STATIC_FREQUENT_CONSTANT_DEFAULT>
    {DIGITALOUTPUT_PULSE_FACTOR_MAX,DIGITALOUTPUT_PULSE_FACTOR_MIN,0,0},    // FLOATRANGE_FIXEDUNIT pulseFactorRange {max,min,unitsCode,unitsType}
    {DIGITALOUTPUT_PULSE_WIDTH_MAX,DIGITALOUTPUT_PULSE_WIDTH_MIN,UM_MILLISECOND,UM_T_TIME},    // FLOATRANGE_FIXEDUNIT pulseWidthRange {max,min,unitsCode,unitsType}
    {DIGITALOUTPUT_OUTPUTFREQ_MAX,DIGITALOUTPUT_OUTPUTFREQ_MIN,0,0},    // FLOATRANGE_FIXEDUNIT minFrequencyRange {max,min,unitsCode,unitsType}
    {DIGITALOUTPUT_OUTPUTFREQ_MAX,DIGITALOUTPUT_OUTPUTFREQ_MIN,0,0},    // FLOATRANGE_FIXEDUNIT maxFrequencyRange {max,min,unitsCode,unitsType}
    DIGITALOUTPUT_ALARM_DISABLE,    // TABENUM8 generalAlarm
    DIGITALOUTPUT_ALARM_DISABLE,    // TABENUM8 minFlowrateAlarm
    DIGITALOUTPUT_ALARM_DISABLE,    // TABENUM8 maxFlowrateAlarm
    DIGITALOUTPUT_ALARM_DISABLE,    // TABENUM8 maxTemperatureAlarm
    DIGITALOUTPUT_ALARM_DISABLE,    // TABENUM8 lowFlowCutOff
    DIGITALOUTPUT_ALARM_DISABLE    // TABENUM8 minTemperatureAlarm
    //@SubGen end@    <INITIALIZER STATIC_FREQUENT_CONSTANT_DEFAULT>
};


//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
FAST T_DIGITAL_OUTPUT_DYNAMIC digitalOutputDynamic;
const SLOW T_DIGITAL_OUTPUT_DYNAMIC digitalOutputDynamicInitializer=
{

    CONSTRUCT_TLOGIC(&digitalOutputDynamic.simuEnabled
                    ,&plantData.lgcSimCmd
                    ,&plantData.lgcSgnlSrc
                    ,&plantData.lgcActState
                    ,&digitalOutputDynamic.logicValue
                    ,&digitalOutputDynamic.do1Available),//TLogic	logic_DO1;
    eTRUE,//TBOOL	do1Available;
    DIGITALOUTPUT_DO1_NONE,//TUSIGN8	oldOutputMode;
    0,//TUSIGN8	oldMeasurement;
    0,//TUSIGN8 onecycle;
    0.0f,//TFLOAT  Qv_Display;
    1.0f,//TFLOAT  DoPercentage;
    0,//TUSIGN8 unitType;
    0,//TUSIGN8 unitCode;
    0.0f,//TFLOAT   gMaxData;
    0,//TUSIGN8  outputMode;
    0L,//TUSIGN32 sysTime;
    0L,//TUSIGN32 elapsedTime;
    0L,//TUSIGN32 oldelapsedTime;
    1.0f,//TFLOAT   timebaseforcalc;
    //@SubGen start@    <INITIALIZER DYNAMIC>
    DIGITALOUTPUT_ALARM_BEHAVIOUR_OK,    // TABENUM8 alarmState_DO
    DIGITALOUTPUT_SIMU_DISABLED,    // TABENUM8 simuEnabled
    10000.0f,    // FLOAT simuFrequency
    0,    // T_DIAGNOSIS_ALARM alarmSimulation_DO
    100,    // INT16 simuPulsesNum
    DIGITALOUTPUT_LOGIC_LOW,    // TABENUM8 logicValue
    0,    // SIMPLE_U32 pulseNum
    0.0f,    // SIMPLE_FLOAT frequencyValue
    DIGITALOUTPUT_FUNCTION_ENABLE    // TABENUM8 functionEnable
    //@SubGen end@    <INITIALIZER DYNAMIC>
};
//-------------------------------------------------------------------------------------------------





//-------------------------------------------------------------------------------------------------
//! object check and PutGet descriptions
//@SubGen start@<DESCRIPTORS>
const SLOW TUSIGN8 TOutputMode1Formats[] =
// Table referenced by: outputMode
{
  DIGITALOUTPUT_DO1_NONE,
  DIGITALOUTPUT_DO1_LOGIC,
  DIGITALOUTPUT_DO1_PULSE,
  DIGITALOUTPUT_DO1_FREQUENCY
};

const SLOW TUSIGN8 TSimulationEnabledFormat[] =
// Table referenced by: simuEnabled
{
  DIGITALOUTPUT_SIMU_DISABLED,
  DIGITALOUTPUT_SIMU_ENABLED
};

const SLOW TUSIGN8 TLogicSignalSourceFormats[] =
// Table referenced by: lgcSgnlSrc
{
  DIGITALOUTPUT_NO_LOGIC_SOURCE,
  DIGITALOUTPUT_DIGITAL_OUTPUT_ALARM
};

const SLOW TUSIGN8 TLogicActiveStateFormats[] =
// Table referenced by: lgcActState
{
  DIGITALOUTPUT_ACTIVE_HIGH,
  DIGITALOUTPUT_ACTIVE_LOW
};

const SLOW TUSIGN8 TOutputLogicFormat[] =
// Table referenced by: logicValue
{
  DIGITALOUTPUT_LOGIC_LOW,
  DIGITALOUTPUT_LOGIC_HIGH
};

const SLOW TUSIGN8 TLogicSimulationCommandFormats[] =
// Table referenced by: lgcSimCmd
{
  DIGITALOUTPUT_COMMAND_OFF,
  DIGITALOUTPUT_COMMAND_ON
};

const SLOW TUSIGN8 E_DIG_OUT_ALARM_BEHAVIOUR[] =
// Table referenced by: alarmState_DO
{
  DIGITALOUTPUT_ALARM_BEHAVIOUR_OK,
  DIGITALOUTPUT_ALARM_BEHAVIOUR_FAIL_LOGIC_OUT_1
};

const SLOW TUSIGN8 DO_ALARM[] =
// Table referenced by: alarmSimulation_DO
{
  DIGITALOUTPUT_PULSENUMMAXALARM
};

const SLOW TUSIGN8 T_ALARM_STATUS[] =
// Table referenced by: generalAlarm, minFlowrateAlarm, maxFlowrateAlarm, maxTemperatureAlarm, lowFlowCutOff, minTemperatureAlarm
{
  DIGITALOUTPUT_ALARM_DISABLE,
  DIGITALOUTPUT_ALARM_ENABLE
};

const SLOW TUSIGN8 T_FUNCTION_STATUS[] =
// Table referenced by: functionEnable
{
  DIGITALOUTPUT_FUNCTION_ENABLE,
  DIGITALOUTPUT_FUNCTION_DISABLE
};

static const SLOW T_RANGE_E8 outputModeDescriptor =
{
  TOutputMode1Formats,  // TUSIGN8* ptrCodeTab
  DIGITALOUTPUT_NUMBER_OF_OUTPUT1_MODES  // TUSIGN8 numberOfCodes
};

const SLOW T_RANGE_E8 alarmState_DODescriptor =
{
  E_DIG_OUT_ALARM_BEHAVIOUR,  // TUSIGN8* ptrCodeTab
  DIGITALOUTPUT_NUMBER_OF_ALM_BEHAV_ELEMENTS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 simuEnabledDescriptor =
{
  TSimulationEnabledFormat,  // TUSIGN8* ptrCodeTab
  DIGITALOUTPUT_NUMBER_OF_SIMULATION_ENABLED  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 lgcSgnlSrcDescriptor =
{
  TLogicSignalSourceFormats,  // TUSIGN8* ptrCodeTab
  DIGITALOUTPUT_NUMBER_OF_LOGIC_SIGNAL_SOURCES  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 lgcActStateDescriptor =
{
  TLogicActiveStateFormats,  // TUSIGN8* ptrCodeTab
  DIGITALOUTPUT_NUMBER_OF_LOGIC_ACTIVE_STATES  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 lgcSimCmdDescriptor =
{
  TLogicSimulationCommandFormats,  // TUSIGN8* ptrCodeTab
  DIGITALOUTPUT_NUMBER_OF_LOGIC_SIM_COMMANDS  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_FLOAT simuFrequencyDescriptor =
{
  DIGITALOUTPUT_FULLSCALE_FREQ_MIN,  // TFLOAT min
  DIGITALOUTPUT_FULLSCALE_FREQ_MAX,  // TFLOAT max
  DIGITALOUTPUT_FULLSCALE_FREQ_DEFAULT  // TFLOAT increment
};

static const SLOW T_FLOAT_FIXEDUNIT_DESCRIPTOR pulseWidthDescriptor =
{
  (const T_DATA_OBJ SLOW*)NULL,  // const T_DATA_OBJ SLOW* publicUnitsDatObj
  &digitalOutputObjectList[DIGITALOUTPUT_IDX_pulseWidthRange],  // const T_DATA_OBJ SLOW* publicRangeDatObj
  (const T_DATA_OBJ SLOW*)NULL,  // const T_DATA_OBJ SLOW* customUnitsDatObj
  DIGITALOUTPUT_PULSE_WIDTH_MIN,  // TFLOAT min
  DIGITALOUTPUT_PULSE_WIDTH_MAX,  // TFLOAT max
  UM_MILLISECOND,  // TUSIGN8 unitsCode
  UM_T_TIME  // TUSIGN8 unitsType
};

static const SLOW T_FLOAT_FIXEDUNIT_DESCRIPTOR pulseFactorDescriptor =
{
  (const T_DATA_OBJ SLOW*)NULL,  // const T_DATA_OBJ SLOW* publicUnitsDatObj
  &digitalOutputObjectList[DIGITALOUTPUT_IDX_pulseFactorRange],  // const T_DATA_OBJ SLOW* publicRangeDatObj
  (const T_DATA_OBJ SLOW*)NULL,  // const T_DATA_OBJ SLOW* customUnitsDatObj
  DIGITALOUTPUT_PULSE_FACTOR_MIN,  // TFLOAT min
  DIGITALOUTPUT_PULSE_FACTOR_MAX,  // TFLOAT max
  0,  // TUSIGN8 unitsCode
  0  // TUSIGN8 unitsType
};

static const SLOW T_FLOATRANGE_FIXEDUNIT_DESCRIPTOR pulseFactorRangeDescriptor =
{
  (const T_DATA_OBJ SLOW*)NULL,  // const T_DATA_OBJ SLOW* publicUnitsDatObj
  (const T_DATA_OBJ SLOW*)NULL,  // const T_DATA_OBJ SLOW* publicRangeDatObj
  (const T_DATA_OBJ SLOW*)NULL,  // const T_DATA_OBJ SLOW* customUnitsDatObj
  DIGITALOUTPUT_PULSE_FACTOR_MIN,  // TFLOAT min
  DIGITALOUTPUT_PULSE_FACTOR_MAX,  // TFLOAT max
  0,  // TUSIGN8 unitsCode
  0  // TUSIGN8 unitsType
};

static const SLOW T_FLOATRANGE_FIXEDUNIT_DESCRIPTOR pulseWidthRangeDescriptor =
{
  (const T_DATA_OBJ SLOW*)NULL,  // const T_DATA_OBJ SLOW* publicUnitsDatObj
  (const T_DATA_OBJ SLOW*)NULL,  // const T_DATA_OBJ SLOW* publicRangeDatObj
  (const T_DATA_OBJ SLOW*)NULL,  // const T_DATA_OBJ SLOW* customUnitsDatObj
  DIGITALOUTPUT_PULSE_WIDTH_MIN,  // TFLOAT min
  DIGITALOUTPUT_PULSE_WIDTH_MAX,  // TFLOAT max
  UM_MILLISECOND,  // TUSIGN8 unitsCode
  UM_T_TIME  // TUSIGN8 unitsType
};

static const SLOW T_RANGE_E8 sharedTRangeE8Descriptor0 =
// Descriptor referenced by: generalAlarm, minFlowrateAlarm, maxFlowrateAlarm, maxTemperatureAlarm, lowFlowCutOff, minTemperatureAlarm
{
  T_ALARM_STATUS,  // TUSIGN8* ptrCodeTab
  DIGITALOUTPUT_T_ALARM_DTABLE_SIZE  // TUSIGN8 numberOfCodes
};

static const SLOW T_FLOATRANGE_FIXEDUNIT_DESCRIPTOR minFrequencyRangeDescriptor =
{
  (const T_DATA_OBJ SLOW*)NULL,  // const T_DATA_OBJ SLOW* publicUnitsDatObj
  (const T_DATA_OBJ SLOW*)NULL,  // const T_DATA_OBJ SLOW* publicRangeDatObj
  (const T_DATA_OBJ SLOW*)NULL,  // const T_DATA_OBJ SLOW* customUnitsDatObj
  DIGITALOUTPUT_OUTPUTFREQ_MIN,  // TFLOAT min
  DIGITALOUTPUT_OUTPUTFREQ_MAX,  // TFLOAT max
  0,  // TUSIGN8 unitsCode
  0  // TUSIGN8 unitsType
};

static const SLOW T_FLOATRANGE_FIXEDUNIT_DESCRIPTOR maxFrequencyRangeDescriptor =
{
  (const T_DATA_OBJ SLOW*)NULL,  // const T_DATA_OBJ SLOW* publicUnitsDatObj
  (const T_DATA_OBJ SLOW*)NULL,  // const T_DATA_OBJ SLOW* publicRangeDatObj
  (const T_DATA_OBJ SLOW*)NULL,  // const T_DATA_OBJ SLOW* customUnitsDatObj
  DIGITALOUTPUT_FULLSCALE_FREQ_MIN,  // TFLOAT min
  DIGITALOUTPUT_FULLSCALE_FREQ_MAX,  // TFLOAT max
  0,  // TUSIGN8 unitsCode
  0  // TUSIGN8 unitsType
};

static const SLOW T_FLOAT_FIXEDUNIT_DESCRIPTOR minFrequencyDescriptor =
{
  (const T_DATA_OBJ SLOW*)NULL,  // const T_DATA_OBJ SLOW* publicUnitsDatObj
  &digitalOutputObjectList[DIGITALOUTPUT_IDX_minFrequencyRange],  // const T_DATA_OBJ SLOW* publicRangeDatObj
  (const T_DATA_OBJ SLOW*)NULL,  // const T_DATA_OBJ SLOW* customUnitsDatObj
  DIGITALOUTPUT_OUTPUTFREQ_MIN,  // TFLOAT min
  DIGITALOUTPUT_OUTPUTFREQ_MAX,  // TFLOAT max
  0,  // TUSIGN8 unitsCode
  0  // TUSIGN8 unitsType
};

static const SLOW T_FLOAT_FIXEDUNIT_DESCRIPTOR maxFrequencyDescriptor =
{
  (const T_DATA_OBJ SLOW*)NULL,  // const T_DATA_OBJ SLOW* publicUnitsDatObj
  &digitalOutputObjectList[DIGITALOUTPUT_IDX_maxFrequencyRange],  // const T_DATA_OBJ SLOW* publicRangeDatObj
  (const T_DATA_OBJ SLOW*)NULL,  // const T_DATA_OBJ SLOW* customUnitsDatObj
  DIGITALOUTPUT_OUTPUTFREQ_MIN,  // TFLOAT min
  DIGITALOUTPUT_OUTPUTFREQ_MAX,  // TFLOAT max
  0,  // TUSIGN8 unitsCode
  0  // TUSIGN8 unitsType
};

static const SLOW T_RANGE_I16 simuPulsesNumDescriptor =
{
  0,  // TUSIGN16 min
  10000,  // TUSIGN16 max
  1  // TUSIGN16 increment
};

static const SLOW T_RANGE_E8 logicValueDescriptor =
{
  TOutputLogicFormat,  // TUSIGN8* ptrCodeTab
  DIGITALOUTPUT_NUMBER_OF_OUTPUT_LOGICS  // TUSIGN8 numberOfCodes
};

const SLOW T_RANGE_E8 alarmSimulation_DODescriptor =
{
  DO_ALARM,  // TUSIGN8* ptrCodeTab
  DIGITALOUTPUT_DO_ALARM_SIZE  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 functionEnableDescriptor =
{
  T_FUNCTION_STATUS,  // TUSIGN8* ptrCodeTab
  sizeof(T_FUNCTION_STATUS)  // TUSIGN8 numberOfCodes
};


//@SubGen end@<DESCRIPTORS>


//-------------------------------------------------------------------------------------------------
//! objectList
const SLOW T_DATA_OBJ digitalOutputObjectList[] =
{
    //lint -e605 All non-volatile data are const for T_UNIT
    //           and the typecast (void FAST*) does not increase the readability
    //@SubGen start@    <OBJECTS>
    //alarmState_DO
    CONSTRUCT_TABENUM8(
        &digitalOutputDynamic.alarmState_DO,
        READONLY_RAM,
        &alarmState_DODescriptor),

    //outputMode
    CONSTRUCT_TABENUM8(
        &digitalOutputREPLACE_STATIC_FREQUENT_STATIC_DEFAULT.outputMode,
        NON_VOLATILE,
        &outputModeDescriptor),

    //simuEnabled
    CONSTRUCT_TABENUM8(
        &digitalOutputDynamic.simuEnabled,
        RAM,
        &simuEnabledDescriptor),

    //lgcSgnlSrc
    CONSTRUCT_TABENUM8(
        &digitalOutputREPLACE_STATIC_FREQUENT_STATIC_DEFAULT.lgcSgnlSrc,
        NON_VOLATILE,
        &lgcSgnlSrcDescriptor),

    //lgcActState
    CONSTRUCT_TABENUM8(
        &digitalOutputREPLACE_STATIC_FREQUENT_STATIC_DEFAULT.lgcActState,
        NON_VOLATILE,
        &lgcActStateDescriptor),

    //lgcSimCmd
    CONSTRUCT_TABENUM8(
        &digitalOutputREPLACE_STATIC_FREQUENT_STATIC_DEFAULT.lgcSimCmd,
        NON_VOLATILE,
        &lgcSimCmdDescriptor),

    //pulseWidth
    CONSTRUCT_STRUCT_FLOAT_FIXEDUNIT(
        &digitalOutputREPLACE_STATIC_FREQUENT_STATIC_DEFAULT.pulseWidth,
        NON_VOLATILE,
        &pulseWidthDescriptor),

    //pulseFactor
    CONSTRUCT_STRUCT_FLOAT_FIXEDUNIT(
        &digitalOutputREPLACE_STATIC_FREQUENT_STATIC_DEFAULT.pulseFactor,
        NON_VOLATILE,
        &pulseFactorDescriptor),

    //pulseFactorRange
    CONSTRUCT_STRUCT_FLOATRANGE_FIXEDUNIT(
        &digitalOutputStaticFrequentConstantDefault.pulseFactorRange,
        NON_VOLATILE,
        &pulseFactorRangeDescriptor),

    //pulseWidthRange
    CONSTRUCT_STRUCT_FLOATRANGE_FIXEDUNIT(
        &digitalOutputStaticFrequentConstantDefault.pulseWidthRange,
        NON_VOLATILE,
        &pulseWidthRangeDescriptor),

    //simuFrequency
    CONSTRUCT_FLOAT(
        &digitalOutputDynamic.simuFrequency,
        RAM,
        &simuFrequencyDescriptor),

    //minFrequency
    CONSTRUCT_STRUCT_FLOAT_FIXEDUNIT(
        &digitalOutputREPLACE_STATIC_FREQUENT_STATIC_DEFAULT.minFrequency,
        NON_VOLATILE,
        &minFrequencyDescriptor),

    //maxFrequency
    CONSTRUCT_STRUCT_FLOAT_FIXEDUNIT(
        &digitalOutputREPLACE_STATIC_FREQUENT_STATIC_DEFAULT.maxFrequency,
        NON_VOLATILE,
        &maxFrequencyDescriptor),

    //minFrequencyRange
    CONSTRUCT_STRUCT_FLOATRANGE_FIXEDUNIT(
        &digitalOutputStaticFrequentConstantDefault.minFrequencyRange,
        NON_VOLATILE,
        &minFrequencyRangeDescriptor),

    //maxFrequencyRange
    CONSTRUCT_STRUCT_FLOATRANGE_FIXEDUNIT(
        &digitalOutputStaticFrequentConstantDefault.maxFrequencyRange,
        NON_VOLATILE,
        &maxFrequencyRangeDescriptor),

    //alarmSimulation_DO
    CONSTRUCT_ALARM(
        &digitalOutputDynamic.alarmSimulation_DO,
        sizeof(T_DIAGNOSIS_ALARM),
        RAM,
        &alarmSimulation_DODescriptor),

    //generalAlarm
    CONSTRUCT_TABENUM8(
        &digitalOutputStaticFrequentConstantDefault.generalAlarm,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor0),

    //minFlowrateAlarm
    CONSTRUCT_TABENUM8(
        &digitalOutputStaticFrequentConstantDefault.minFlowrateAlarm,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor0),

    //maxFlowrateAlarm
    CONSTRUCT_TABENUM8(
        &digitalOutputStaticFrequentConstantDefault.maxFlowrateAlarm,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor0),

    //maxTemperatureAlarm
    CONSTRUCT_TABENUM8(
        &digitalOutputStaticFrequentConstantDefault.maxTemperatureAlarm,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor0),

    //lowFlowCutOff
    CONSTRUCT_TABENUM8(
        &digitalOutputStaticFrequentConstantDefault.lowFlowCutOff,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor0),

    //simuPulsesNum
    CONSTRUCT_INT16(
        &digitalOutputDynamic.simuPulsesNum,
        RAM,
        &simuPulsesNumDescriptor),

    //logicValue
    CONSTRUCT_TABENUM8(
        &digitalOutputDynamic.logicValue,
        READONLY_RAM,
        &logicValueDescriptor),

    //pulseNum
    CONSTRUCT_SIMPLE_U32(
        &digitalOutputDynamic.pulseNum,
        READONLY_RAM),

    //frequencyValue
    CONSTRUCT_SIMPLE_FLOAT(
        &digitalOutputDynamic.frequencyValue,
        READONLY_RAM),

    //functionEnable
    CONSTRUCT_TABENUM8(
        &digitalOutputDynamic.functionEnable,
        RAM,
        &functionEnableDescriptor),

    //minTemperatureAlarm
    CONSTRUCT_TABENUM8(
        &digitalOutputStaticFrequentConstantDefault.minTemperatureAlarm,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor0)
    //@SubGen end@    <OBJECTS>
    //lint +e605 warning on for the rest of the code
};



//-------------------------------------------------------------------------------------------------
//! define the DIGITAL_OUTPUT-Class object
SLOW const T_DIGITAL_OUTPUT digitalOutput =
{
    {
        &digitalOutputState,
        &digitalOutputDataSemaphore,

        digitalOutputObjectList,
        (sizeof(digitalOutputObjectList)/sizeof(digitalOutputObjectList[0]))-1,

        digitalOutputDataClassList,
        sizeof(digitalOutputDataClassList)/sizeof(digitalOutputDataClassList[0]),

        ChangeState_T_UNIT,
        GetState_T_UNIT,
        Initialize_DIGITAL_OUTPUT, // will be overloaded
        LoadRomDefaults_T_UNIT,
        ResetToDefault_T_UNIT,
        SaveAsDefault_T_UNIT,
        Get_T_UNIT,
        //Put_T_UNIT,
        Put_DIGITAL_OUTPUT,
        Check_T_UNIT,
        GetAttributeDescription_T_UNIT,
        GetObjectDescription_T_UNIT
    }

    //subsystem EXE and SRV-methods
    //@SubGen start@    <METHODS>
    ,UpdateEXE_DIGITALOUTPUT
    ,UpdateDiagnosis_DIGITALOUTPUT
    //@SubGen end@    <METHODS>
};




/*

This section defines templates for the SubSystem CodeGenerator.
Do not modify or remove this section

see also: digitalOutput_attributes.h


//special templates for specific standard storage classes:
//@SubGen template start@<DATACLASS CONSTANT>
//-------------------------------------------------------------------------------------------------
//! definition of constant block
const SLOW T_DIGITAL_OUTPUT_TEMPLATE digitalOutputTemplate=
{
    //@SubGen start@    <INITIALIZER TEMPLATE>
    //@SubGen end@    <INITIALIZER TEMPLATE>
};
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS CONSTANT>



//@SubGen template start@<DATACLASS DYNAMIC>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
FAST T_DIGITAL_OUTPUT_TEMPLATE digitalOutputTemplate;
const SLOW T_DIGITAL_OUTPUT_TEMPLATE digitalOutputTemplateInitializer=
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
  #pragma bss_seg(".AAA$DIGITAL_OUTPUT_DATA")
    const FAST T_DIGITAL_OUTPUT_TEMPLATE digitalOutputTemplate;
    const FAST T_DIGITAL_OUTPUT_TEMPLATE digitalOutputTemplateDefault;
  #pragma bss_seg()
#else
  #pragma location="DIGITAL_OUTPUT_TEMPLATE"
    const FAST __no_init T_DIGITAL_OUTPUT_TEMPLATE digitalOutputTemplate;
  #pragma location="DIGITAL_OUTPUT_TEMPLATE_DEFAULT"
    const FAST __no_init T_DIGITAL_OUTPUT_TEMPLATE digitalOutputTemplateDefault;
#endif
const SLOW T_DIGITAL_OUTPUT_TEMPLATE digitalOutputTemplateInitializer=
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
  #pragma bss_seg(".AAA$DIGITAL_OUTPUT_DATA")
    const FAST T_DIGITAL_OUTPUT_TEMPLATE digitalOutputTemplate;
  #pragma bss_seg()
#else
  #pragma location="DIGITAL_OUTPUT_TEMPLATE"
    const FAST __no_init T_DIGITAL_OUTPUT_TEMPLATE digitalOutputTemplate;
#endif
const SLOW T_DIGITAL_OUTPUT_TEMPLATE digitalOutputTemplateInitializer=
{
    //@SubGen start@    <INITIALIZER TEMPLATE>
    //@SubGen end@    <INITIALIZER TEMPLATE>
};
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS>

//@SubGen template start@ <DATACLASSNAME>
digitalOutputTemplate
//@SubGen template end@ <DATACLASSNAME>


//@SubGen template start@ <DATACLASSLIST DYNAMIC>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_DYNAMIC(digitalOutputTemplate,digitalOutputTemplateInitializer)
//@SubGen template end@ <DATACLASSLIST DYNAMIC>

//@SubGen template start@ <DATACLASSLIST CONSTANT>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_CONSTANT(digitalOutputTemplate)
//@SubGen template end@ <DATACLASSLIST CONSTANT>

//@SubGen template start@ <DATACLASSLIST>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_STATIC_NODEF(digitalOutputTemplate,digitalOutputTemplateInitializer)
//@SubGen template end@ <DATACLASSLIST>

//@SubGen template start@ <DATACLASSLIST STATIC_FREQUENT_CONSTANT_DEFAULT>
//lint -e{708} initialization is OK
CONSTRUCTOR_DC_STATIC_CONSTDEF(digitalOutputTemplate,digitalOutputTemplateInitializer)
//@SubGen template end@ <DATACLASSLIST STATIC_FREQUENT_CONSTANT_DEFAULT>

//@SubGen template start@ <DATACLASSLIST STATIC_FREQUENT_STATIC_DEFAULT>
//lint -e{708} initialization is OK
CONSTRUCTOR_DC_STATIC_STATDEF(digitalOutputTemplate,digitalOutputTemplateInitializer,digitalOutputTemplateDefault)
//@SubGen template end@ <DATACLASSLIST STATIC_FREQUENT_STATIC_DEFAULT>

*/