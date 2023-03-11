//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Coordinator
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

#include "T_DATA_OBJ/Interface/t_data_obj_simple.h"
#include "T_DATA_OBJ/Interface/t_data_obj_float.h"
#include "T_DATA_OBJ/Interface/t_data_obj_int16.h"
#include "T_DATA_OBJ/Interface/t_data_obj_usign8.h"
#include "T_DATA_OBJ/Interface/t_data_obj_tabenum8.h"
//#include "..\..\t_data_obj\interface\t_data_obj_tabenum16.h"
#include "T_DATA_OBJ/Interface/t_data_obj_uint32.h"
#include "T_DATA_OBJ/Interface/t_data_obj_action.h"
#include "T_DATA_OBJ/Interface/t_data_obj_const.h"
#include "T_DATA_OBJ/Interface/t_data_obj_uom_pf.h"
#include "T_DATA_OBJ/Interface/t_data_obj_rngflt.h"
#include "Diagnosis/interface/t_data_obj_alarm.h"
#include "T_DATA_OBJ/Interface/t_data_obj_dev_rngflt.h"


#include "T_Unit/Interface/t_unit.h"
#include "T_Unit/Interface/T_DATACLASSES.H"
#include "Coordinator/Interface/Coordinator.h"
#include "Coordinator/Interface/Coordinator_idx.h"
#include "coordinator_action_fct.h"

#include "coordinator_types.h"
#include "coordinator_attributes.h"
#include "coordinator_overload.h"
#include "coordinator_execute.h"


//-------------------------------------------------------------------------------------------------
//! unit global attributes
FAST TUSIGN16 coordinatorState;     // Subsystem state
FAST T_DATA_SEMAPHORE coordinatorDataSemaphore;
// Forward declaration ObjectList
extern const SLOW T_DATA_OBJ coordinatorObjectList[];

const SLOW T_DATACLASS coordinatorDataClassList[]=
{
    //lint -e{651} Initializers are OK, the warning pop up with the first 708
    //@SubGen start@    <DATACLASSLIST>
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_STATIC_NODEF(coordinatorReplaceStaticRare,coordinatorReplaceStaticRareInitializer),
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_DYNAMIC(coordinatorDynamicDuplicated,coordinatorDynamicDuplicatedInitializer),
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_DYNAMIC(coordinatorProtected,coordinatorProtectedInitializer),
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_DYNAMIC(coordinatorUnprotected,coordinatorUnprotectedInitializer),
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_STATIC_NODEF(coordinatorReplaceStaticRare_Calibration,coordinatorReplaceStaticRare_CalibrationInitializer),
    //lint -e{708} initialization is OK
    CONSTRUCTOR_DC_STATIC_STATDEF(coordinatorReplaceStaticFreqStaticDefault,coordinatorReplaceStaticFreqStaticDefaultInitializer,coordinatorReplaceStaticFreqStaticDefaultDefault),
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_DYNAMIC(coordinatorFE_OUTPUT_FILTER_PARA,coordinatorFE_OUTPUT_FILTER_PARAInitializer),
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_CONSTANT(coordinatorConstant),
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_STATIC_NODEF(coordinatorStaticRare,coordinatorStaticRareInitializer),
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_STATIC_NODEF(coordinatorStaticCyclic,coordinatorStaticCyclicInitializer),
    //lint -e{708} initialization is OK
    CONSTRUCTOR_DC_STATIC_CONSTDEF(coordinatorStaticFrequentConstantDefault,coordinatorStaticFrequentConstantDefaultInitializer)
    //@SubGen end@    <DATACLASSLIST>
};

//@SubGen put@<DATACLASS>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
FAST T_COORDINATOR_FE_OUTPUT_FILTER_PARA coordinatorFE_OUTPUT_FILTER_PARA;
const SLOW T_COORDINATOR_FE_OUTPUT_FILTER_PARA coordinatorFE_OUTPUT_FILTER_PARAInitializer=
{
    //@SubGen start@    <INITIALIZER FE_OUTPUT_FILTER_PARA>
    CDT_EXT_ALG_NO_FILTER,    // TABENUM8 ext_algSelection
    CDT_INT_ALG_USE_NONE,    // TABENUM8 int_algSelection
    {0,0,0,0,0,0,0,0},    // SIMPLE_U16 ss_paras[8]
    {0,0,0,0}    // SIMPLE_U16 sf_paras[4]
    //@SubGen end@    <INITIALIZER FE_OUTPUT_FILTER_PARA>
};
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//! definition of ReplaceStaticFreqStaticDefault data
#ifdef WIN32
  #pragma bss_seg(".AAA$COORDINATOR_DATA")
    const FAST T_COORDINATOR_REPLACE_STATIC_FREQ_STATIC_DEFAULT coordinatorReplaceStaticFreqStaticDefault;
    const FAST T_COORDINATOR_REPLACE_STATIC_FREQ_STATIC_DEFAULT coordinatorReplaceStaticFreqStaticDefaultDefault;
  #pragma bss_seg()
#else
  #pragma location="COORDINATOR_REPLACE_STATIC_FREQ_STATIC_DEFAULT"
    const FAST __no_init T_COORDINATOR_REPLACE_STATIC_FREQ_STATIC_DEFAULT coordinatorReplaceStaticFreqStaticDefault;
  #pragma location="COORDINATOR_REPLACE_STATIC_FREQ_STATIC_DEFAULT_DEFAULT"
    const FAST __no_init T_COORDINATOR_REPLACE_STATIC_FREQ_STATIC_DEFAULT coordinatorReplaceStaticFreqStaticDefaultDefault;
#endif
const SLOW T_COORDINATOR_REPLACE_STATIC_FREQ_STATIC_DEFAULT coordinatorReplaceStaticFreqStaticDefaultInitializer=
{
    //@SubGen start@    <INITIALIZER REPLACE_STATIC_FREQ_STATIC_DEFAULT>
    CDT_LIQUID_ACTUAL_VOLUME,    // TABENUM8 operatingMode
    0.2,    // FLOAT temperature_DampingTime
    CDT_CELSIUS,    // TABENUM8 temperature_UnitCode
    {500.0f,-200.0f,700.0f},    // DEV_RNGFLT temperature_range {hiLim,loLim,span}
    20.0f,    // FLOAT temperaturePreset
    CDT_EXT_DENSITY    // TABENUM8 actualDensitySelection
    //@SubGen end@    <INITIALIZER REPLACE_STATIC_FREQ_STATIC_DEFAULT>
};
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//! definition of ReplaceStaticRare_Calibration data
#ifdef WIN32
  #pragma bss_seg(".AAA$COORDINATOR_DATA")
    const FAST T_COORDINATOR_REPLACE_STATIC_RARE_CALIBRATION coordinatorReplaceStaticRare_Calibration;
  #pragma bss_seg()
#else
  #pragma location="COORDINATOR_REPLACE_STATIC_RARE_CALIBRATION"
    const FAST __no_init T_COORDINATOR_REPLACE_STATIC_RARE_CALIBRATION coordinatorReplaceStaticRare_Calibration;
#endif
const SLOW T_COORDINATOR_REPLACE_STATIC_RARE_CALIBRATION coordinatorReplaceStaticRare_CalibrationInitializer=
{
    //@SubGen start@    <INITIALIZER REPLACE_STATIC_RARE_CALIBRATION>
    CDT_SENSOR_IS_UNCALIBRATED,    // TABENUM8 calibrationStatus
    {'*','*','*','*','*','*'},    // SIMPLE_CHAR internalRTDPassword[6]
    {'*','*','*','*','*','*'},    // SIMPLE_CHAR analogInputPassword[6]
    {'*','*','*','*','*','*'}    // SIMPLE_CHAR energyFlowPassword[6]
    //@SubGen end@    <INITIALIZER REPLACE_STATIC_RARE_CALIBRATION>
};
//-------------------------------------------------------------------------------------------------






//-------------------------------------------------------------------------------------------------
//! definition of StaticFrequentConstantDefault data
#ifdef WIN32
  #pragma bss_seg(".AAA$COORDINATOR_DATA")
    const FAST T_COORDINATOR_STATIC_FREQUENT_CONSTANT_DEFAULT coordinatorStaticFrequentConstantDefault;
  #pragma bss_seg()
#else
  #pragma location="COORDINATOR_STATIC_FREQUENT_CONSTANT_DEFAULT"
    const FAST __no_init T_COORDINATOR_STATIC_FREQUENT_CONSTANT_DEFAULT coordinatorStaticFrequentConstantDefault;
#endif
const SLOW T_COORDINATOR_STATIC_FREQUENT_CONSTANT_DEFAULT coordinatorStaticFrequentConstantDefaultInitializer=
{
    //@SubGen start@    <INITIALIZER STATIC_FREQUENT_CONSTANT_DEFAULT>
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}    // SIMPLE_U8 page1[30]
    //@SubGen end@    <INITIALIZER STATIC_FREQUENT_CONSTANT_DEFAULT>
};
//-------------------------------------------------------------------------------------------------




//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
FAST T_COORDINATOR_UNPROTECTED coordinatorUnprotected;
const SLOW T_COORDINATOR_UNPROTECTED coordinatorUnprotectedInitializer=
{
    //@SubGen start@    <INITIALIZER UNPROTECTED>
    0,    // SIMPLE_U32 workingTimeMSec
    0,    // SIMPLE_U32 partialWorkingTimeHour
    0.0f,    // FLOAT frequency_Sim
    3000.0f,    // SIMPLE_FLOAT frequency_MaxDN
    0.7f,    // SIMPLE_FLOAT frequency_MinDN
    2999.3f,    // SIMPLE_FLOAT frequency_Span
    0.0,    // SIMPLE_FLOAT frequency_Percentage
    0xC0,    // SIMPLE_U8 frequency_Status
    0.0f,    // FLOAT temperature_Sim
    CDT_CELSIUS,    // TABENUM8 simTemperature_UnitCode
    500.0f,    // SIMPLE_FLOAT temperature_MaxDN
    -200.0f,    // SIMPLE_FLOAT temperature_MinDN
    700.0f,    // SIMPLE_FLOAT temperature_Span
    0xC0,    // SIMPLE_U8 temperature_Status
    CDT_T_PRESET,    // TABENUM8 temperature_Selection
    0.0,    // FLOAT cpuLoad
    255,    // SIMPLE_U8 frontEndConfParamWr
    255,    // SIMPLE_U8 frontEndConfParamRd
    255,    // TABENUM8 frontEndTempTypeRd
    100,    // SIMPLE_U32 updateTimeFast
    250,    // SIMPLE_U32 updateTimeSlow
    0,    // SIMPLE_U8 vortexDeviceStatus
    0,    // SIMPLE_U8 additionalVortexDeviceStatus
    0.0f,    // FLOAT snr
    0,    // SIMPLE_U32 frontendOperationHour
    CDT_DEFAULT_MAINTENANCE_CYCLE_HOURS,    // USIGN32 frontendMaintenance
    CDT_STATUS_OFF,    // TABENUM8 vbrCorrectionOn
    33,    // INT16 vbrFilStep
    15,    // USIGN8 vbrFilNumCoef
    CDT_STATUS_OFF,    // TABENUM8 fcdOn
    0,    // USIGN8 fcdShift
    10,    // USIGN8 fcdDelay
    10,    // USIGN8 fcdNumCoef
    1,    // USIGN8 lockMinIntPeriod
    4,    // USIGN8 lockMaxIntPeriod
    10,    // USIGN8 lockEstimation
    1,    // USIGN8 accuracyHigh
    1,    // USIGN8 accuracyMiddle
    1,    // USIGN8 accuracyLow
    CDT_GAIN_CONTROL_1TIMES,    // TABENUM8 analogGain
    CDT_STATUS_OFF,    // TABENUM8 gainCtrl
    2048,    // INT16 gainLowThresh
    20480,    // INT16 gainHighThresh
    660,    // INT16 gainMarginFreq
    0,    // USIGN8 gainMaxShift
    CDT_STATUS_OFF,    // TABENUM8 lowFlowThreshOn
    7,    // INT16 lowFlowThresh
    5,    // INT16 lowFlowThreshCoef
    4,    // USIGN8 noiseSampSize
    0,    // USIGN8 staticDecimation
    1,    // USIGN8 middleBufDecimation
    3000.0f,    // FLOAT maxFlowFreq
    0.0f,    // FLOAT minFlowFreq
    0,    // USIGN8 freqMinMarginPer
    0,    // USIGN8 freqMaxMarginPer
    0.0f,    // FLOAT temperatureOffsetCorrectDisp
    0,    // SIMPLE_U32 timeStamp
    0,    // SIMPLE_U8 tPresetChangeFlag
    0,    // SIMPLE_U16 protectedDataCrc
    32767,    // INT16 spanUpperRange
    0,    // SIMPLE_U16 sigAmplitude
    0,    // SIMPLE_U16 vbrAmplitude
    0,    // SIMPLE_U16 sigMagnitude
    0.0f,    // FLOAT temperatureOffsetCorrectUserDisp
    CDT_DEFAULT_MAINTENANCE_CYCLE_HOURS,    // USIGN32 BothMaintenanceCycle
    0.0f    // SIMPLE_FLOAT rawTemperatureSensor
    //@SubGen end@    <INITIALIZER UNPROTECTED>
};
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//! definition of ReplaceStaticRare data
#ifdef WIN32
  #pragma bss_seg(".AAA$COORDINATOR_DATA")
    const FAST T_COORDINATOR_REPLACE_STATIC_RARE coordinatorReplaceStaticRare;
  #pragma bss_seg()
#else
  #pragma location="COORDINATOR_REPLACE_STATIC_RARE"
    const FAST __no_init T_COORDINATOR_REPLACE_STATIC_RARE coordinatorReplaceStaticRare;
#endif
const SLOW T_COORDINATOR_REPLACE_STATIC_RARE coordinatorReplaceStaticRareInitializer=
{
    //@SubGen start@    <INITIALIZER REPLACE_STATIC_RARE>
    0.2,    // FLOAT frequency_DampingTime
    CDT_TEMP_NONE,    // TABENUM8 intRTDType
    -200.0f,    // FLOAT temperatureMaxRecord
    {500.00,-200.00},    // RANGEFLT temperatureAlmRange {hiLim,loLim}
    CDT_VORTEX,    // TABENUM8 converterType
    CDT_VotrexD50mm,    // TABENUM8 vortexmeterSize
    CDT_Swirl50mm,    // TABENUM8 swirlmeterSize
    0.0f    // FLOAT temperatureOffsetCorrect
    //@SubGen end@    <INITIALIZER REPLACE_STATIC_RARE>
};
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
FAST T_COORDINATOR_DYNAMIC_DUPLICATED coordinatorDynamicDuplicated;
const SLOW T_COORDINATOR_DYNAMIC_DUPLICATED coordinatorDynamicDuplicatedInitializer=
{
    //@SubGen start@    <INITIALIZER DYNAMIC_DUPLICATED>
    0.0f,    // FLOAT frequency
    0.0f,    // FLOAT frequency_Damped
    0.0f,    // SIMPLE_FLOAT temperature
    0.0f,    // FLOAT temperature_Damped
    0.0f,    // SIMPLE_FLOAT temperature_Percentage
    0.0f,    // FLOAT frequency_Duplicated
    0.0f,    // FLOAT frequency_Damped_Duplictated
    0.0f,    // SIMPLE_FLOAT temperature_Duplicated
    0.0f,    // FLOAT temperature_Damped_Duplicated
    {0,0,0,0},    // SIMPLE_U8 cdtDiagnosis[4]
    {0,0,0,0},    // SIMPLE_U8 cdtDiagnosis_Duplicated[4]
    0.0f,    // SIMPLE_FLOAT temperature_Percentage_Duplicated
    0.0f,    // SIMPLE_FLOAT temperatureSensor
    0.0f,    // SIMPLE_FLOAT temperatureSensor_Duplicated
    CDT_STATUS_ON,    // TABENUM8 autoselfcheck_Enable
    CDT_STATUS_ON    // TABENUM8 autoselfcheck_Enable_Duplicated
    //@SubGen end@    <INITIALIZER DYNAMIC_DUPLICATED>
};
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
FAST T_COORDINATOR_PROTECTED coordinatorProtected;
const SLOW T_COORDINATOR_PROTECTED coordinatorProtectedInitializer=
{
    //@SubGen start@    <INITIALIZER PROTECTED>
    {0,0,0,0},    // T_DIAGNOSIS_ALARM coodinatorAlarmSimulation[4]
    CDT_SIMULATION_CB_OFF,    // TABENUM8 simulationMode
    CDT_SIMULATION_DISABLE,    // TABENUM8 frequencySim_Enable
    CDT_SIMULATION_DISABLE,    // TABENUM8 temperatureSim_Enable
    0x00,    // SIMPLE_U8 passwordEnable
    CDT_UNPROTECTED    // TABENUM8 protectedDataStatus
    //@SubGen end@    <INITIALIZER PROTECTED>
};
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//! definition of StaticRare data
#ifdef WIN32
  #pragma bss_seg(".AAA$COORDINATOR_DATA")
    const FAST T_COORDINATOR_STATIC_RARE coordinatorStaticRare;
  #pragma bss_seg()
#else
  #pragma location="COORDINATOR_STATIC_RARE"
    const FAST __no_init T_COORDINATOR_STATIC_RARE coordinatorStaticRare;
#endif
const SLOW T_COORDINATOR_STATIC_RARE coordinatorStaticRareInitializer=
{
    //@SubGen start@    <INITIALIZER STATIC_RARE>
    {3000.0f,0.7f,2999.3f},    // DEV_RNGFLT frequency_range {hiLim,loLim,span}
    CDT_HERTZ,    // TABENUM8 frequency_UnitCode
    CDT_HERTZ,    // TABENUM8 simFrequency_UnitCode
    CDT_FORCE_REPLACE_NONE,    // TABENUM8 forceReplace
    0.0f,    // FLOAT temperatureOffsetCorrectUser
    CDT_PRESET    // TABENUM8 reaction_InputFail
    //@SubGen end@    <INITIALIZER STATIC_RARE>
};
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//! definition of constant block
const SLOW T_COORDINATOR_CONSTANT coordinatorConstant=
{
    //@SubGen start@    <INITIALIZER CONSTANT>
    CDT_T_FREQUENCY,    // CONST_U8 frequency_UnitType
    CDT_T_FREQUENCY,    // CONST_U8 frequency_Classification
    CDT_T_TEMPERATURE,    // CONST_U8 temperature_UnitType
    CDT_T_TEMPERATURE,    // CONST_U8 temperature_Classification
    5    // CONST_U8 familyRevision
    //@SubGen end@    <INITIALIZER CONSTANT>
};
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//! definition of StaticCyclic data
#ifdef WIN32
  #pragma bss_seg(".AAA$COORDINATOR_DATA")
    const FAST T_COORDINATOR_STATIC_CYCLIC coordinatorStaticCyclic;
  #pragma bss_seg()
#else
  #pragma location="COORDINATOR_STATIC_CYCLIC"
    const FAST __no_init T_COORDINATOR_STATIC_CYCLIC coordinatorStaticCyclic;
#endif
const SLOW T_COORDINATOR_STATIC_CYCLIC coordinatorStaticCyclicInitializer=
{
    //@SubGen start@    <INITIALIZER STATIC_CYCLIC>
    0,    // SIMPLE_U32 totalWorkingTimeHour
    CDT_DEFAULT_MAINTENANCE_CYCLE_HOURS    // USIGN32 tranMaintenanceCyc
    //@SubGen end@    <INITIALIZER STATIC_CYCLIC>
};
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//! object check and PutGet descriptions
//@SubGen start@<DESCRIPTORS>
const SLOW TUSIGN8 _CDT_DIAGNOSIS_ALARM_TABLE[] =
// Table referenced by: coodinatorAlarmSimulation
{
  CDT_ALARM_CB_SIMUDATA,
  CDT_ALARM_CB_ALARM,
  CDT_ALARM_CB_NV_ERROR,
  CDT_ALARM_CB_MAINTENANCE,
  CDT_ALARM_CB_UNCALIBRATED,
  CDT_ALRAM_T_MAX,
  CDT_ALRAM_T_MIN,
  CDT_VORTEX_SENSOR_CIRCUIT_FAILURE,
  CDT_VORTEX_SENSOR_OPEN,
  CDT_VORTEX_SENSOR_SHORT,
  CDT_VORTEX_SENSOR_DRIFT,
  CDT_VIBRAT_SENSOR_CIRCUIT_FAILURE,
  CDT_VIBRAT_SENSOR_OPEN,
  CDT_VIBRAT_SENSOR_SHORT,
  CDT_VIBRAT_SENSOR_DRIFT,
  CDT_SYNC_MISSING,
  CDT_SNR_FAILURE,
  CDT_TEMP_SENSOR_UPPER_LIMIT,
  CDT_TEMP_SENSOR_LOWER_LIMIT,
  CDT_FE_RAM_FAILURE,
  CDT_FE_NV_FAILURE,
  CDT_FE_MAINTENANCE_MATURE,
  CDT_ALARM_REPLACE_FE_TO_CB_BLOCKED,
  CDT_ALARM_REPLACE_REQUIRED_FE_TO_CB_BLOCKED,
  CDT_ALARM_REPLACE_REQUIRED_BOTH_DIRECTION_ENABLED
};

const SLOW TUSIGN8 _CDT_SIMULATION_MODE[] =
// Table referenced by: simulationMode
{
  CDT_SIMULATION_CB_OFF,
  CDT_SIMULATION_CB_QV_VALUE,
  CDT_SIMULATION_CB_QV_PERCENTAGE,
  CDT_SIMULATION_CB_QM_VALUE,
  CDT_SIMULATION_CB_QM_PERCENTAGE,
  CDT_SIMULATION_CB_MEDIUM_TEMP,
  CDT_SIMULATION_CB_AMBIENT_TEMP,
  CDT_SIMULATION_CB_DO_FREQ,
  CDT_SIMULATION_CB_DO_LOGIC,
  CDT_SIMULATION_CB_DO_PULSE,
  CDT_SIMULATION_CB_AI_VALUE,
  CDT_SIMULATION_FE_SENSOR_FREQ
};

const SLOW TUSIGN8 _CDT_FORCE_REPLACE[] =
// Table referenced by: forceReplace
{
  CDT_FORCE_REPLACE_NONE,
  CDT_FORCE_REPLACE_FE_TO_CB_COMMON_ONLY,
  CDT_FORCE_REPLACE_FE_TO_CB,
  CDT_FORCE_REPLACE_CB_TO_FE
};

const SLOW TUSIGN8 _SENSOR_CALIBRATION_STATUS[] =
// Table referenced by: calibrationStatus
{
  CDT_SENSOR_IS_CALIBRATED,
  CDT_SENSOR_IS_UNCALIBRATED
};

const SLOW TUSIGN8 _TEMP_SENSOR_TYPE[] =
// Table referenced by: frontEndTempTypeRd, intRTDType
{
  CDT_TEMP_PT100,
  CDT_TEMP_PT1000,
  CDT_TEMP_NONE
};

const SLOW TUSIGN8 _SIMULATION_STATUS[] =
// Table referenced by: frequencySim_Enable, temperatureSim_Enable
{
  CDT_SIMULATION_DISABLE,
  CDT_SIMULATION_ENABLE
};

const SLOW TUSIGN8 _T_FREQUENCY[] =
// Table referenced by: frequency_UnitCode, simFrequency_UnitCode
{
  CDT_HERTZ
};

const SLOW TUSIGN8 _T_TEMPERATURE[] =
// Table referenced by: temperature_UnitCode, simTemperature_UnitCode
{
  CDT_KELVIN,
  CDT_CELSIUS,
  CDT_FAHRENHEIT
};

const SLOW TUSIGN8 _TSELECTION[] =
// Table referenced by: temperature_Selection
{
  CDT_T_PRESET,
  CDT_T_SENSOR,
  CDT_T_AI,
  CDT_T_HART
};

const SLOW TUSIGN8 _OPERATING_MODE[] =
// Table referenced by: operatingMode
{
  CDT_LIQUID_ACTUAL_VOLUME,
  CDT_LIQUID_NORMAL_VOLUME,
  CDT_LIQUID_MASS,
  CDT_LIQUID_POWER,
  CDT_GAS_ACTUAL_VOLUME,
  CDT_GAS_STANDARD_VOLUME,
  CDT_GAS_MASS,
  CDT_GAS_POWER,
  CDT_BIOGAS_ACTUAL_VOLUME,
  CDT_BIOGAS_STANDARD_VOLUME,
  CDT_STEAM_ACTUAL_VOLUME,
  CDT_STEAM_MASS,
  CDT_STEAM_POWER
};

const SLOW TUSIGN8 _CONVERTER_TYPE[] =
// Table referenced by: converterType
{
  CDT_SWIREL,
  CDT_VORTEX
};

const SLOW TUSIGN8 _SWIRL_SIZE[] =
// Table referenced by: swirlmeterSize
{
  CDT_Swirl15mm,
  CDT_Swirl20mm,
  CDT_Swirl25mm,
  CDT_Swirl32mm,
  CDT_Swirl40mm,
  CDT_Swirl50mm,
  CDT_Swirl80mm,
  CDT_Swirl100mm,
  CDT_Swirl150mm,
  CDT_Swirl200mm,
  CDT_Swirl300mm,
  CDT_Swirl400mm
};

const SLOW TUSIGN8 _VORTEX_SIZE[] =
// Table referenced by: vortexmeterSize
{
  CDT_VotrexD15mm,
  CDT_VotrexD25mm,
  CDT_VotrexD40mm,
  CDT_VotrexD50mm,
  CDT_VotrexD80mm,
  CDT_VotrexD100mm,
  CDT_VotrexD150mm,
  CDT_VotrexD200mm,
  CDT_VotrexD250mm,
  CDT_VotrexD300mm,
  CDT_VotrexA15mm,
  CDT_VotrexA25mm,
  CDT_VotrexA40mm,
  CDT_VotrexA50mm,
  CDT_VotrexA80mm,
  CDT_VotrexA100mm,
  CDT_VotrexA150mm,
  CDT_VotrexA200mm,
  CDT_VotrexA250mm,
  CDT_VotrexA300mm
};

const SLOW TUSIGN8 _STATUS[] =
// Table referenced by: vbrCorrectionOn, fcdOn, gainCtrl, lowFlowThreshOn, autoselfcheck_Enable, autoselfcheck_Enable_Duplicated
{
  CDT_STATUS_OFF,
  CDT_STATUS_ON
};

const SLOW TUSIGN8 DENSITY_SELECTION_TABLE[] =
// Table referenced by: actualDensitySelection
{
  CDT_EXT_DENSITY,
  CDT_CALCULATION_FROM_TP,
  CDT_CALCULATION_FROM_T,
  CDT_CALCULATION_FROM_P
};

const SLOW TUSIGN8 GAIN_CONTROL_TAB[] =
// Table referenced by: analogGain
{
  CDT_GAIN_CONTROL_1TIMES,
  CDT_GAIN_CONTROL_2TIMES,
  CDT_GAIN_CONTROL_4TIMES,
  CDT_GAIN_CONTROL_8TIMES
};

const SLOW TUSIGN8 _CDT_STATUS_TABLE[] =
// Table referenced by: protectedDataStatus
{
  CDT_UNPROTECTED,
  CDT_CALCULATE,
  CDT_PROTECTED
};

const SLOW TUSIGN8 REACTION_INPUT_FAIL[] =
// Table referenced by: reaction_InputFail
{
  CDT_PRESET,
  CDT_LAST_GOOD_VALUE
};

const SLOW TUSIGN8 _FE_OUT_FILTER_EXT_ALG_TABLE[] =
// Table referenced by: ext_algSelection
{
  CDT_EXT_ALG_NO_FILTER,
  CDT_EXT_ALG_PRESET_1,
  CDT_EXT_ALG_PRESET_2,
  CDT_EXT_ALG_SELF_CUSTOM
};

const SLOW TUSIGN8 _FE_OUT_FILTER_INT_ALG_TABLE[] =
// Table referenced by: int_algSelection
{
  CDT_INT_ALG_USE_NONE,
  CDT_INT_ALG_USE_SELF_STUDY,
  CDT_INT_ALG_USE_SMOOTH_FILTER,
  CDT_INT_ALG_USE_SS_AND_SF
};

 const SLOW T_RANGE_E8 coodinatorAlarmSimulationDescriptor =
{
  _CDT_DIAGNOSIS_ALARM_TABLE,  // TUSIGN8* ptrCodeTab
  sizeof(_CDT_DIAGNOSIS_ALARM_TABLE)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 simulationModeDescriptor =
{
  _CDT_SIMULATION_MODE,  // TUSIGN8* ptrCodeTab
  sizeof(_CDT_SIMULATION_MODE)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_FLOAT cpuLoadDescriptor =
{
  0.0f,  // TFLOAT min
  100.0f,  // TFLOAT max
  0.01f  // TFLOAT increment
};

static const SLOW T_RANGE_E8 forceReplaceDescriptor =
{
  _CDT_FORCE_REPLACE,  // TUSIGN8* ptrCodeTab
  sizeof(_CDT_FORCE_REPLACE)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 calibrationStatusDescriptor =
{
  _SENSOR_CALIBRATION_STATUS,  // TUSIGN8* ptrCodeTab
  sizeof(_SENSOR_CALIBRATION_STATUS)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 sharedTRangeE8Descriptor0 =
// Descriptor referenced by: frontEndTempTypeRd, intRTDType
{
  _TEMP_SENSOR_TYPE,  // TUSIGN8* ptrCodeTab
  sizeof(_TEMP_SENSOR_TYPE)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_FLOAT sharedTRangeFloatDescriptor1 =
// Descriptor referenced by: frequency, frequency_Damped, frequency_Duplicated, frequency_Damped_Duplictated
{
  0.3f,  // TFLOAT min
  3000.0f,  // TFLOAT max
  0.1f  // TFLOAT increment
};

static const SLOW T_RANGE_E8 sharedTRangeE8Descriptor2 =
// Descriptor referenced by: frequencySim_Enable, temperatureSim_Enable
{
  _SIMULATION_STATUS,  // TUSIGN8* ptrCodeTab
  sizeof(_SIMULATION_STATUS)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_FLOAT frequency_SimDescriptor =
{
  0.0f,  // TFLOAT min
  3000.0f,  // TFLOAT max
  0.1f  // TFLOAT increment
};

static const SLOW T_RANGE_FLOAT temperature_SimDescriptor =
{
  -200.0,  // TFLOAT min
  500.0,  // TFLOAT max
  0.1f  // TFLOAT increment
};

static const SLOW T_RANGE_FLOAT sharedTRangeFloatDescriptor3 =
// Descriptor referenced by: temperature_Damped, temperaturePreset, temperature_Damped_Duplicated
{
  -200.0f,  // TFLOAT min
  500.0f,  // TFLOAT max
  0.1f  // TFLOAT increment
};

static const SLOW T_RANGE_E8 sharedTRangeE8Descriptor4 =
// Descriptor referenced by: frequency_UnitCode, simFrequency_UnitCode
{
  _T_FREQUENCY,  // TUSIGN8* ptrCodeTab
  sizeof(_T_FREQUENCY)  // TUSIGN8 numberOfCodes
};

static const SLOW T_UOM_PF_DESCRIPTOR frequency_SimObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(simFrequency_UnitCode, coordinator, CDT),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(frequency_Sim, coordinator, CDT),  // void * pObject_value
  CDT_T_FREQUENCY  // TUSIGN8 unitsType
};

static const SLOW T_RANGE_FLOAT sharedTRangeFloatDescriptor5 =
// Descriptor referenced by: frequency_DampingTime, temperature_DampingTime
{
  0.0f,  // TFLOAT min
  100.0f,  // TFLOAT max
  0.1f  // TFLOAT increment
};

static const SLOW T_RANGE_E8 sharedTRangeE8Descriptor6 =
// Descriptor referenced by: temperature_UnitCode, simTemperature_UnitCode
{
  _T_TEMPERATURE,  // TUSIGN8* ptrCodeTab
  sizeof(_T_TEMPERATURE)  // TUSIGN8 numberOfCodes
};

static const SLOW T_UOM_PF_DESCRIPTOR sharedTUomPfDescriptorDescriptor7 =
// Descriptor referenced by: temperature_Obj, temperature_Damped_Obj
{
  SUBSYS_OBJPTR_IDX_(temperature_UnitCode, coordinator, CDT),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(temperature_Damped, coordinator, CDT),  // void * pObject_value
  CDT_T_TEMPERATURE  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR temperatureSim_ObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(simTemperature_UnitCode, coordinator, CDT),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(temperature_Sim, coordinator, CDT),  // void * pObject_value
  CDT_T_TEMPERATURE  // TUSIGN8 unitsType
};

static const SLOW T_RANGEFLT_DESCRIPTOR temperatureAlmRangeDescriptor =
{
  500.00,  // TFLOAT maxHi
  -200.00  // TFLOAT maxLo
};

static const SLOW T_RANGE_E8 converterTypeDescriptor =
{
  _CONVERTER_TYPE,  // TUSIGN8* ptrCodeTab
  sizeof(_CONVERTER_TYPE)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 vortexmeterSizeDescriptor =
{
  _VORTEX_SIZE,  // TUSIGN8* ptrCodeTab
  sizeof(_VORTEX_SIZE)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 swirlmeterSizeDescriptor =
{
  _SWIRL_SIZE,  // TUSIGN8* ptrCodeTab
  sizeof(_SWIRL_SIZE)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_FLOAT snrDescriptor =
{
  0.0f,  // TFLOAT min
  1.0f,  // TFLOAT max
  0.01f  // TFLOAT increment
};

static const SLOW T_RANGE_FLOAT maxFlowFreqDescriptor =
{
  19.0f,  // TFLOAT min
  3000.0f,  // TFLOAT max
  0.1f  // TFLOAT increment
};

static const SLOW T_RANGE_FLOAT minFlowFreqDescriptor =
{
  0.0f,  // TFLOAT min
  1000.0f,  // TFLOAT max
  0.1f  // TFLOAT increment
};

static const SLOW T_RANGE_I16 vbrFilStepDescriptor =
{
  33,  // TUSIGN16 min
  1310,  // TUSIGN16 max
  1  // TUSIGN16 increment
};

static const SLOW T_RANGE_U8 sharedTRangeU8Descriptor8 =
// Descriptor referenced by: vbrFilNumCoef, fcdNumCoef
{
  10,  // TUSIGN8 min
  50,  // TUSIGN8 max
  1  // TUSIGN8 increment
};

static const SLOW T_RANGE_E8 sharedTRangeE8Descriptor9 =
// Descriptor referenced by: vbrCorrectionOn, fcdOn, gainCtrl, lowFlowThreshOn, autoselfcheck_Enable, autoselfcheck_Enable_Duplicated
{
  _STATUS,  // TUSIGN8* ptrCodeTab
  sizeof(_STATUS)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_U8 sharedTRangeU8Descriptor10 =
// Descriptor referenced by: fcdShift, staticDecimation
{
  0,  // TUSIGN8 min
  5,  // TUSIGN8 max
  1  // TUSIGN8 increment
};

static const SLOW T_RANGE_U8 fcdDelayDescriptor =
{
  10,  // TUSIGN8 min
  25,  // TUSIGN8 max
  1  // TUSIGN8 increment
};

static const SLOW T_RANGE_U8 sharedTRangeU8Descriptor11 =
// Descriptor referenced by: lockMinIntPeriod, accuracyHigh, accuracyMiddle, accuracyLow
{
  1,  // TUSIGN8 min
  4,  // TUSIGN8 max
  1  // TUSIGN8 increment
};

static const SLOW T_RANGE_U8 sharedTRangeU8Descriptor12 =
// Descriptor referenced by: lockMaxIntPeriod, noiseSampSize
{
  4,  // TUSIGN8 min
  8,  // TUSIGN8 max
  1  // TUSIGN8 increment
};

static const SLOW T_RANGE_U8 lockEstimationDescriptor =
{
  10,  // TUSIGN8 min
  30,  // TUSIGN8 max
  1  // TUSIGN8 increment
};

static const SLOW T_RANGE_I16 gainLowThreshDescriptor =
{
  2048,  // TUSIGN16 min
  10240,  // TUSIGN16 max
  1  // TUSIGN16 increment
};

static const SLOW T_RANGE_I16 gainHighThreshDescriptor =
{
  20480,  // TUSIGN16 min
  30720,  // TUSIGN16 max
  1  // TUSIGN16 increment
};

static const SLOW T_RANGE_I16 gainMarginFreqDescriptor =
{
  660,  // TUSIGN16 min
  4096,  // TUSIGN16 max
  1  // TUSIGN16 increment
};

static const SLOW T_RANGE_U8 gainMaxShiftDescriptor =
{
  0,  // TUSIGN8 min
  2,  // TUSIGN8 max
  1  // TUSIGN8 increment
};

static const SLOW T_RANGE_U8 middleBufDecimationDescriptor =
{
  1,  // TUSIGN8 min
  6,  // TUSIGN8 max
  1  // TUSIGN8 increment
};

static const SLOW T_RANGE_U8 freqMinMarginPerDescriptor =
{
  0,  // TUSIGN8 min
  30,  // TUSIGN8 max
  1  // TUSIGN8 increment
};

static const SLOW T_RANGE_U8 freqMaxMarginPerDescriptor =
{
  0,  // TUSIGN8 min
  30,  // TUSIGN8 max
  0  // TUSIGN8 increment
};

static const SLOW T_RANGE_E8 operatingModeDescriptor =
{
  _OPERATING_MODE,  // TUSIGN8* ptrCodeTab
  sizeof(_OPERATING_MODE)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 temperature_SelectionDescriptor =
{
  _TSELECTION,  // TUSIGN8* ptrCodeTab
  sizeof(_TSELECTION)  // TUSIGN8 numberOfCodes
};

static const SLOW T_UOM_PF_DESCRIPTOR frequency_ObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(frequency_UnitCode, coordinator, CDT),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(frequency, coordinator, CDT),  // void * pObject_value
  CDT_T_FREQUENCY  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR temperaturePreset_ObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(temperature_UnitCode, coordinator, CDT),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(temperaturePreset, coordinator, CDT),  // void * pObject_value
  CDT_T_TEMPERATURE  // TUSIGN8 unitsType
};

static const SLOW T_RANGE_U32 sharedTRangeU32Descriptor13 =
// Descriptor referenced by: tranMaintenanceCyc, frontendMaintenance, BothMaintenanceCycle
{
  CDT_MIN_MAINTENANCE_CYCLE_HOURS,  // TUSIGN32 min
  CDT_MAX_MAINTENANCE_CYCLE_HOURS,  // TUSIGN32 max
  1  // TUSIGN32 increment
};

static const SLOW T_RANGE_I16 lowFlowThreshCoefDescriptor =
{
  -10,  // TUSIGN16 min
  10,  // TUSIGN16 max
  1  // TUSIGN16 increment
};

static const SLOW T_RANGE_I16 lowFlowThreshDescriptor =
{
  7,  // TUSIGN16 min
  2000,  // TUSIGN16 max
  1  // TUSIGN16 increment
};

static const SLOW T_RANGE_E8 actualDensitySelectionDescriptor =
{
  DENSITY_SELECTION_TABLE,  // TUSIGN8* ptrCodeTab
  sizeof(DENSITY_SELECTION_TABLE)  // TUSIGN8 numberOfCodes
};

static const SLOW T_DEV_RNGFLT_DESCRIPTOR frequency_rangeDescriptor =
{
  SUBSYS_OBJPTR_IDX_(frequency_MaxDN,coordinator, CDT),  // void * ptrMaxDN
  SUBSYS_OBJPTR_IDX_(frequency_MinDN,coordinator, CDT),  // void * ptrMinDN
  1.0,  // TFLOAT hiLim_MaxPrcOfDnSpan
  0.0,  // TFLOAT hiLim_MinPrcOfDnSpan
  1.0,  // TFLOAT loLim_MaxPrcOfCurAllowedSpan
  0.0  // TFLOAT loLim_MinPrcOfCurAllowedSpan
};

static const SLOW T_UOM_PF_DESCRIPTOR frequency_range_ObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(frequency_UnitCode, coordinator, CDT),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(frequency_range, coordinator, CDT),  // void * pObject_value
  CDT_T_FREQUENCY  // TUSIGN8 unitsType
};

static const SLOW T_DEV_RNGFLT_DESCRIPTOR temperature_rangeDescriptor =
{
  SUBSYS_OBJPTR_IDX_(temperature_MaxDN,coordinator, CDT),  // void * ptrMaxDN
  SUBSYS_OBJPTR_IDX_(temperature_MinDN,coordinator, CDT),  // void * ptrMinDN
  1.0,  // TFLOAT hiLim_MaxPrcOfDnSpan
  0.0,  // TFLOAT hiLim_MinPrcOfDnSpan
  1.0,  // TFLOAT loLim_MaxPrcOfCurAllowedSpan
  0.0  // TFLOAT loLim_MinPrcOfCurAllowedSpan
};

static const SLOW T_UOM_PF_DESCRIPTOR temperature_range_ObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(temperature_UnitCode, coordinator, CDT),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(temperature_range, coordinator, CDT),  // void * pObject_value
  CDT_T_TEMPERATURE  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR frequency_Damped_ObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(frequency_UnitCode, coordinator, CDT),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(frequency_Damped, coordinator, CDT),  // void * pObject_value
  CDT_T_FREQUENCY  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR temperature_Span_ObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(temperature_UnitCode, coordinator, CDT),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(temperature_Span, coordinator, CDT),  // void * pObject_value
  CDT_T_TEMPERATURE  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR frequency_Span_ObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(frequency_UnitCode, coordinator, CDT),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(frequency_Span, coordinator, CDT),  // void * pObject_value
  CDT_T_FREQUENCY  // TUSIGN8 unitsType
};

static const SLOW T_RANGE_E8 analogGainDescriptor =
{
  GAIN_CONTROL_TAB,  // TUSIGN8* ptrCodeTab
  sizeof(GAIN_CONTROL_TAB)  // TUSIGN8 numberOfCodes
};

static const SLOW T_UOM_PF_DESCRIPTOR temperatureSensor_ObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(temperature_UnitCode, coordinator, CDT),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(temperatureSensor, coordinator, CDT),  // void * pObject_value
  CDT_T_TEMPERATURE  // TUSIGN8 unitsType
};

static const SLOW T_RANGE_FLOAT sharedTRangeFloatDescriptor14 =
// Descriptor referenced by: temperatureOffsetCorrect, temperatureOffsetCorrectUser
{
  -500.0f,  // TFLOAT min
  500.0f,  // TFLOAT max
  0.1f  // TFLOAT increment
};

static const SLOW T_RANGE_E8 protectedDataStatusDescriptor =
{
  _CDT_STATUS_TABLE,  // TUSIGN8* ptrCodeTab
  sizeof(_CDT_STATUS_TABLE)  // TUSIGN8 numberOfCodes
};

static const SLOW T_UOM_PF_DESCRIPTOR temperatureAlmRange_objDescriptor =
{
  SUBSYS_OBJPTR_IDX_(temperature_UnitCode, coordinator, CDT),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(temperatureAlmRange,coordinator, CDT),  // void * pObject_value
  CDT_T_TEMPERATURE  // TUSIGN8 unitsType
};

static const SLOW T_RANGE_I16 spanUpperRangeDescriptor =
{
  0,  // TUSIGN16 min
  32767,  // TUSIGN16 max
  1  // TUSIGN16 increment
};

static const SLOW T_UOM_PF_DESCRIPTOR temperatureMaxRecord_objDescriptor =
{
  SUBSYS_OBJPTR_IDX_(temperature_UnitCode, coordinator, CDT),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(temperatureMaxRecord,coordinator, CDT),  // void * pObject_value
  CDT_T_TEMPERATURE  // TUSIGN8 unitsType
};

static const SLOW T_RANGE_FLOAT temperatureMaxRecordDescriptor =
{
  -400.0f,  // TFLOAT min
  1000.0f,  // TFLOAT max
  1  // TFLOAT increment
};

static const SLOW T_RANGE_E8 reaction_InputFailDescriptor =
{
  REACTION_INPUT_FAIL,  // TUSIGN8* ptrCodeTab
  sizeof(REACTION_INPUT_FAIL)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_FLOAT sharedTRangeFloatDescriptor15 =
// Descriptor referenced by: temperatureOffsetCorrectDisp, temperatureOffsetCorrectUserDisp
{
  -50.0f,  // TFLOAT min
  50.0f,  // TFLOAT max
  0.1f  // TFLOAT increment
};

static const SLOW T_RANGE_E8 ext_algSelectionDescriptor =
{
  _FE_OUT_FILTER_EXT_ALG_TABLE,  // TUSIGN8* ptrCodeTab
  sizeof(_FE_OUT_FILTER_EXT_ALG_TABLE)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 int_algSelectionDescriptor =
{
  _FE_OUT_FILTER_INT_ALG_TABLE,  // TUSIGN8* ptrCodeTab
  sizeof(_FE_OUT_FILTER_INT_ALG_TABLE)  // TUSIGN8 numberOfCodes
};
//@SubGen end@<DESCRIPTORS>


//-------------------------------------------------------------------------------------------------
//! objectList
const SLOW T_DATA_OBJ coordinatorObjectList[] =
{
    //lint -e605 All non-volatile data are const for T_UNIT
    //           and the typecast (void FAST*) does not increase the readability
    //@SubGen start@    <OBJECTS>
    //coodinatorAlarmSimulation
    CONSTRUCT_ALARM(
        coordinatorProtected.coodinatorAlarmSimulation,
        4 * sizeof(T_DIAGNOSIS_ALARM),
        RAM,
        &coodinatorAlarmSimulationDescriptor),

    //operatingMode
    CONSTRUCT_TABENUM8(
        &coordinatorReplaceStaticFreqStaticDefault.operatingMode,
        NON_VOLATILE,
        &operatingModeDescriptor),

    //workingTimeMSec
    CONSTRUCT_SIMPLE_U32(
        &coordinatorUnprotected.workingTimeMSec,
        RAM),

    //totalWorkingTimeHour
    CONSTRUCT_SIMPLE_U32(
        &coordinatorStaticCyclic.totalWorkingTimeHour,
        NON_VOLATILE),

    //partialWorkingTimeHour
    CONSTRUCT_SIMPLE_U32(
        &coordinatorUnprotected.partialWorkingTimeHour,
        RAM),

    //simulationMode
    CONSTRUCT_TABENUM8(
        &coordinatorProtected.simulationMode,
        RAM,
        &simulationModeDescriptor),

    //frequency
    CONSTRUCT_FLOAT(
        &coordinatorDynamicDuplicated.frequency,
        RAM,
        &sharedTRangeFloatDescriptor1),

    //frequencySim_Enable
    CONSTRUCT_TABENUM8(
        &coordinatorProtected.frequencySim_Enable,
        RAM,
        &sharedTRangeE8Descriptor2),

    //frequency_Sim
    CONSTRUCT_FLOAT(
        &coordinatorUnprotected.frequency_Sim,
        RAM,
        &frequency_SimDescriptor),

    //frequency_MaxDN
    CONSTRUCT_SIMPLE_FLOAT(
        &coordinatorUnprotected.frequency_MaxDN,
        RAM),

    //frequency_MinDN
    CONSTRUCT_SIMPLE_FLOAT(
        &coordinatorUnprotected.frequency_MinDN,
        RAM),

    //frequency_Span
    CONSTRUCT_SIMPLE_FLOAT(
        &coordinatorUnprotected.frequency_Span,
        RAM),

    //frequency_Span_Obj
    CONSTRUCT_STRUCT_UOM_PF(
        &frequency_Span_ObjDescriptor),

    //frequency_range
    CONSTRUCT_STRUCT_DEV_RNGFLT(
        &coordinatorStaticRare.frequency_range,
        NON_VOLATILE,
        &frequency_rangeDescriptor),

    //frequency_range_Obj
    CONSTRUCT_STRUCT_UOM_PF(
        &frequency_range_ObjDescriptor),

    //frequency_Percentage
    CONSTRUCT_SIMPLE_FLOAT(
        &coordinatorUnprotected.frequency_Percentage,
        RAM),

    //frequency_Damped
    CONSTRUCT_FLOAT(
        &coordinatorDynamicDuplicated.frequency_Damped,
        RAM,
        &sharedTRangeFloatDescriptor1),

    //frequency_Damped_Obj
    CONSTRUCT_STRUCT_UOM_PF(
        &frequency_Damped_ObjDescriptor),

    //frequency_UnitCode
    CONSTRUCT_TABENUM8(
        &coordinatorStaticRare.frequency_UnitCode,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor4),

    //simFrequency_UnitCode
    CONSTRUCT_TABENUM8(
        &coordinatorStaticRare.simFrequency_UnitCode,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor4),

    //frequency_UnitType
    CONSTRUCT_CONST_U8(
        &coordinatorConstant.frequency_UnitType),

    //frequency_Obj
    CONSTRUCT_STRUCT_UOM_PF(
        &frequency_ObjDescriptor),

    //frequency_SimObj
    CONSTRUCT_STRUCT_UOM_PF(
        &frequency_SimObjDescriptor),

    //frequency_Classification
    CONSTRUCT_CONST_U8(
        &coordinatorConstant.frequency_Classification),

    //frequency_Status
    CONSTRUCT_SIMPLE_U8(
        &coordinatorUnprotected.frequency_Status,
        RAM),

    //frequency_DampingTime
    CONSTRUCT_FLOAT(
        &coordinatorReplaceStaticRare.frequency_DampingTime,
        NON_VOLATILE,
        &sharedTRangeFloatDescriptor5),

    //temperature
    CONSTRUCT_SIMPLE_FLOAT(
        &coordinatorDynamicDuplicated.temperature,
        RAM),

    //temperatureSim_Enable
    CONSTRUCT_TABENUM8(
        &coordinatorProtected.temperatureSim_Enable,
        RAM,
        &sharedTRangeE8Descriptor2),

    //temperature_Sim
    CONSTRUCT_FLOAT(
        &coordinatorUnprotected.temperature_Sim,
        RAM,
        &temperature_SimDescriptor),

    //temperature_DampingTime
    CONSTRUCT_FLOAT(
        &coordinatorReplaceStaticFreqStaticDefault.temperature_DampingTime,
        NON_VOLATILE,
        &sharedTRangeFloatDescriptor5),

    //temperature_Damped
    CONSTRUCT_FLOAT(
        &coordinatorDynamicDuplicated.temperature_Damped,
        RAM,
        &sharedTRangeFloatDescriptor3),

    //temperature_Damped_Obj
    CONSTRUCT_STRUCT_UOM_PF(
        &sharedTUomPfDescriptorDescriptor7),

    //temperature_UnitCode
    CONSTRUCT_TABENUM8(
        &coordinatorReplaceStaticFreqStaticDefault.temperature_UnitCode,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor6),

    //simTemperature_UnitCode
    CONSTRUCT_TABENUM8(
        &coordinatorUnprotected.simTemperature_UnitCode,
        RAM,
        &sharedTRangeE8Descriptor6),

    //temperature_UnitType
    CONSTRUCT_CONST_U8(
        &coordinatorConstant.temperature_UnitType),

    //temperature_Obj
    CONSTRUCT_STRUCT_UOM_PF(
        &sharedTUomPfDescriptorDescriptor7),

    //temperatureSim_Obj
    CONSTRUCT_STRUCT_UOM_PF(
        &temperatureSim_ObjDescriptor),

    //temperature_MaxDN
    CONSTRUCT_SIMPLE_FLOAT(
        &coordinatorUnprotected.temperature_MaxDN,
        RAM),

    //temperature_MinDN
    CONSTRUCT_SIMPLE_FLOAT(
        &coordinatorUnprotected.temperature_MinDN,
        RAM),

    //temperature_Span
    CONSTRUCT_SIMPLE_FLOAT(
        &coordinatorUnprotected.temperature_Span,
        RAM),

    //temperature_Span_Obj
    CONSTRUCT_STRUCT_UOM_PF(
        &temperature_Span_ObjDescriptor),

    //temperature_range
    CONSTRUCT_STRUCT_DEV_RNGFLT(
        &coordinatorReplaceStaticFreqStaticDefault.temperature_range,
        NON_VOLATILE,
        &temperature_rangeDescriptor),

    //temperature_range_Obj
    CONSTRUCT_STRUCT_UOM_PF(
        &temperature_range_ObjDescriptor),

    //temperature_Percentage
    CONSTRUCT_SIMPLE_FLOAT(
        &coordinatorDynamicDuplicated.temperature_Percentage,
        RAM),

    //temperature_Classification
    CONSTRUCT_CONST_U8(
        &coordinatorConstant.temperature_Classification),

    //temperature_Status
    CONSTRUCT_SIMPLE_U8(
        &coordinatorUnprotected.temperature_Status,
        RAM),

    //temperature_Selection
    CONSTRUCT_TABENUM8(
        &coordinatorUnprotected.temperature_Selection,
        RAM,
        &temperature_SelectionDescriptor),

    //intRTDType
    CONSTRUCT_TABENUM8(
        &coordinatorReplaceStaticRare.intRTDType,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor0),

    //cpuLoad
    CONSTRUCT_FLOAT(
        &coordinatorUnprotected.cpuLoad,
        RAM,
        &cpuLoadDescriptor),

    //frontEndConfParamWr
    CONSTRUCT_SIMPLE_U8(
        &coordinatorUnprotected.frontEndConfParamWr,
        RAM),

    //frontEndConfParamRd
    CONSTRUCT_SIMPLE_U8(
        &coordinatorUnprotected.frontEndConfParamRd,
        RAM),

    //frontEndTempTypeRd
    CONSTRUCT_TABENUM8(
        &coordinatorUnprotected.frontEndTempTypeRd,
        RAM,
        &sharedTRangeE8Descriptor0),

    //updateTimeFast
    CONSTRUCT_SIMPLE_U32(
        &coordinatorUnprotected.updateTimeFast,
        READONLY_RAM),

    //updateTimeSlow
    CONSTRUCT_SIMPLE_U32(
        &coordinatorUnprotected.updateTimeSlow,
        READONLY_RAM),

    //forceReplace
    CONSTRUCT_TABENUM8(
        &coordinatorStaticRare.forceReplace,
        NON_VOLATILE,
        &forceReplaceDescriptor),

    //tranMaintenanceCyc
    CONSTRUCT_UINT32(
        &coordinatorStaticCyclic.tranMaintenanceCyc,
        NON_VOLATILE,
        &sharedTRangeU32Descriptor13),

    //vortexDeviceStatus
    CONSTRUCT_SIMPLE_U8(
        &coordinatorUnprotected.vortexDeviceStatus,
        RAM),

    //additionalVortexDeviceStatus
    CONSTRUCT_SIMPLE_U8(
        &coordinatorUnprotected.additionalVortexDeviceStatus,
        RAM),

    //familyRevision
    CONSTRUCT_CONST_U8(
        &coordinatorConstant.familyRevision),

    //calibrationStatus
    CONSTRUCT_TABENUM8(
        &coordinatorReplaceStaticRare_Calibration.calibrationStatus,
        NON_VOLATILE,
        &calibrationStatusDescriptor),

    //internalRTDPassword
    CONSTRUCT_ARRAY_SIMPLE_CHAR(
        coordinatorReplaceStaticRare_Calibration.internalRTDPassword,
        6,
        NON_VOLATILE),

    //analogInputPassword
    CONSTRUCT_ARRAY_SIMPLE_CHAR(
        coordinatorReplaceStaticRare_Calibration.analogInputPassword,
        6,
        NON_VOLATILE),

    //energyFlowPassword
    CONSTRUCT_ARRAY_SIMPLE_CHAR(
        coordinatorReplaceStaticRare_Calibration.energyFlowPassword,
        6,
        NON_VOLATILE),

    //frequency_Duplicated
    CONSTRUCT_FLOAT(
        &coordinatorDynamicDuplicated.frequency_Duplicated,
        RAM,
        &sharedTRangeFloatDescriptor1),

    //frequency_Damped_Duplictated
    CONSTRUCT_FLOAT(
        &coordinatorDynamicDuplicated.frequency_Damped_Duplictated,
        RAM,
        &sharedTRangeFloatDescriptor1),

    //temperature_Duplicated
    CONSTRUCT_SIMPLE_FLOAT(
        &coordinatorDynamicDuplicated.temperature_Duplicated,
        RAM),

    //temperature_Damped_Duplicated
    CONSTRUCT_FLOAT(
        &coordinatorDynamicDuplicated.temperature_Damped_Duplicated,
        RAM,
        &sharedTRangeFloatDescriptor3),

    //temperaturePreset
    CONSTRUCT_FLOAT(
        &coordinatorReplaceStaticFreqStaticDefault.temperaturePreset,
        NON_VOLATILE,
        &sharedTRangeFloatDescriptor3),

    //temperaturePreset_Obj
    CONSTRUCT_STRUCT_UOM_PF(
        &temperaturePreset_ObjDescriptor),

    //cdtDiagnosis
    CONSTRUCT_ARRAY_SIMPLE_U8(
        coordinatorDynamicDuplicated.cdtDiagnosis,
        4,
        RAM),

    //cdtDiagnosis_Duplicated
    CONSTRUCT_ARRAY_SIMPLE_U8(
        coordinatorDynamicDuplicated.cdtDiagnosis_Duplicated,
        4,
        RAM),

    //passwordEnable
    CONSTRUCT_SIMPLE_U8(
        &coordinatorProtected.passwordEnable,
        RAM),

    //temperatureMaxRecord
    CONSTRUCT_FLOAT(
        &coordinatorReplaceStaticRare.temperatureMaxRecord,
        NON_VOLATILE,
        &temperatureMaxRecordDescriptor),

    //temperature_Percentage_Duplicated
    CONSTRUCT_SIMPLE_FLOAT(
        &coordinatorDynamicDuplicated.temperature_Percentage_Duplicated,
        RAM),

    //temperatureAlmRange
    CONSTRUCT_STRUCT_RNGFLT(
        &coordinatorReplaceStaticRare.temperatureAlmRange,
        NON_VOLATILE,
        &temperatureAlmRangeDescriptor),

    //converterType
    CONSTRUCT_TABENUM8(
        &coordinatorReplaceStaticRare.converterType,
        NON_VOLATILE,
        &converterTypeDescriptor),

    //vortexmeterSize
    CONSTRUCT_TABENUM8(
        &coordinatorReplaceStaticRare.vortexmeterSize,
        NON_VOLATILE,
        &vortexmeterSizeDescriptor),

    //swirlmeterSize
    CONSTRUCT_TABENUM8(
        &coordinatorReplaceStaticRare.swirlmeterSize,
        NON_VOLATILE,
        &swirlmeterSizeDescriptor),

    //snr
    CONSTRUCT_FLOAT(
        &coordinatorUnprotected.snr,
        RAM,
        &snrDescriptor),

    //frontendOperationHour
    CONSTRUCT_SIMPLE_U32(
        &coordinatorUnprotected.frontendOperationHour,
        RAM),

    //frontendMaintenance
    CONSTRUCT_UINT32(
        &coordinatorUnprotected.frontendMaintenance,
        RAM,
        &sharedTRangeU32Descriptor13),

    //vbrCorrectionOn
    CONSTRUCT_TABENUM8(
        &coordinatorUnprotected.vbrCorrectionOn,
        RAM,
        &sharedTRangeE8Descriptor9),

    //vbrFilStep
    CONSTRUCT_INT16(
        &coordinatorUnprotected.vbrFilStep,
        RAM,
        &vbrFilStepDescriptor),

    //vbrFilNumCoef
    CONSTRUCT_USIGN8(
        &coordinatorUnprotected.vbrFilNumCoef,
        RAM,
        &sharedTRangeU8Descriptor8),

    //fcdOn
    CONSTRUCT_TABENUM8(
        &coordinatorUnprotected.fcdOn,
        RAM,
        &sharedTRangeE8Descriptor9),

    //fcdShift
    CONSTRUCT_USIGN8(
        &coordinatorUnprotected.fcdShift,
        RAM,
        &sharedTRangeU8Descriptor10),

    //fcdDelay
    CONSTRUCT_USIGN8(
        &coordinatorUnprotected.fcdDelay,
        RAM,
        &fcdDelayDescriptor),

    //fcdNumCoef
    CONSTRUCT_USIGN8(
        &coordinatorUnprotected.fcdNumCoef,
        RAM,
        &sharedTRangeU8Descriptor8),

    //lockMinIntPeriod
    CONSTRUCT_USIGN8(
        &coordinatorUnprotected.lockMinIntPeriod,
        RAM,
        &sharedTRangeU8Descriptor11),

    //lockMaxIntPeriod
    CONSTRUCT_USIGN8(
        &coordinatorUnprotected.lockMaxIntPeriod,
        RAM,
        &sharedTRangeU8Descriptor12),

    //lockEstimation
    CONSTRUCT_USIGN8(
        &coordinatorUnprotected.lockEstimation,
        RAM,
        &lockEstimationDescriptor),

    //accuracyHigh
    CONSTRUCT_USIGN8(
        &coordinatorUnprotected.accuracyHigh,
        RAM,
        &sharedTRangeU8Descriptor11),

    //accuracyMiddle
    CONSTRUCT_USIGN8(
        &coordinatorUnprotected.accuracyMiddle,
        RAM,
        &sharedTRangeU8Descriptor11),

    //accuracyLow
    CONSTRUCT_USIGN8(
        &coordinatorUnprotected.accuracyLow,
        RAM,
        &sharedTRangeU8Descriptor11),

    //analogGain
    CONSTRUCT_TABENUM8(
        &coordinatorUnprotected.analogGain,
        RAM,
        &analogGainDescriptor),

    //gainCtrl
    CONSTRUCT_TABENUM8(
        &coordinatorUnprotected.gainCtrl,
        RAM,
        &sharedTRangeE8Descriptor9),

    //gainLowThresh
    CONSTRUCT_INT16(
        &coordinatorUnprotected.gainLowThresh,
        RAM,
        &gainLowThreshDescriptor),

    //gainHighThresh
    CONSTRUCT_INT16(
        &coordinatorUnprotected.gainHighThresh,
        RAM,
        &gainHighThreshDescriptor),

    //gainMarginFreq
    CONSTRUCT_INT16(
        &coordinatorUnprotected.gainMarginFreq,
        RAM,
        &gainMarginFreqDescriptor),

    //gainMaxShift
    CONSTRUCT_USIGN8(
        &coordinatorUnprotected.gainMaxShift,
        RAM,
        &gainMaxShiftDescriptor),

    //lowFlowThreshOn
    CONSTRUCT_TABENUM8(
        &coordinatorUnprotected.lowFlowThreshOn,
        RAM,
        &sharedTRangeE8Descriptor9),

    //lowFlowThresh
    CONSTRUCT_INT16(
        &coordinatorUnprotected.lowFlowThresh,
        RAM,
        &lowFlowThreshDescriptor),

    //lowFlowThreshCoef
    CONSTRUCT_INT16(
        &coordinatorUnprotected.lowFlowThreshCoef,
        RAM,
        &lowFlowThreshCoefDescriptor),

    //noiseSampSize
    CONSTRUCT_USIGN8(
        &coordinatorUnprotected.noiseSampSize,
        RAM,
        &sharedTRangeU8Descriptor12),

    //staticDecimation
    CONSTRUCT_USIGN8(
        &coordinatorUnprotected.staticDecimation,
        RAM,
        &sharedTRangeU8Descriptor10),

    //middleBufDecimation
    CONSTRUCT_USIGN8(
        &coordinatorUnprotected.middleBufDecimation,
        RAM,
        &middleBufDecimationDescriptor),

    //maxFlowFreq
    CONSTRUCT_FLOAT(
        &coordinatorUnprotected.maxFlowFreq,
        RAM,
        &maxFlowFreqDescriptor),

    //minFlowFreq
    CONSTRUCT_FLOAT(
        &coordinatorUnprotected.minFlowFreq,
        RAM,
        &minFlowFreqDescriptor),

    //freqMinMarginPer
    CONSTRUCT_USIGN8(
        &coordinatorUnprotected.freqMinMarginPer,
        RAM,
        &freqMinMarginPerDescriptor),

    //freqMaxMarginPer
    CONSTRUCT_USIGN8(
        &coordinatorUnprotected.freqMaxMarginPer,
        RAM,
        &freqMaxMarginPerDescriptor),

    //readParameters
    CONSTRUCT_ACTION(
        //lint -e{611} cast is OK for T_ACTION
        readParameters_CDT),

    //writeParameters
    CONSTRUCT_ACTION(
        //lint -e{611} cast is OK for T_ACTION
        writeParameters_CDT),

    //saveCustomerDef
    CONSTRUCT_ACTION(
        //lint -e{611} cast is OK for T_ACTION
        saveCustomerDef_CDT),

    //saveFactoryDef
    CONSTRUCT_ACTION(
        //lint -e{611} cast is OK for T_ACTION
        saveFactoryDef_CDT),

    //resetCustomerDef
    CONSTRUCT_ACTION(
        //lint -e{611} cast is OK for T_ACTION
        resetCustomerDef_CDT),

    //resetFactoryDef
    CONSTRUCT_ACTION(
        //lint -e{611} cast is OK for T_ACTION
        resetFactoryDef_CDT),

    //resetFE_Runhour
    CONSTRUCT_ACTION(
        //lint -e{611} cast is OK for T_ACTION
        resetFE_Runhour_CDT),

    //temperatureOffsetCorrect
    CONSTRUCT_FLOAT(
        &coordinatorReplaceStaticRare.temperatureOffsetCorrect,
        NON_VOLATILE,
        &sharedTRangeFloatDescriptor14),

    //temperatureOffsetCorrectDisp
    CONSTRUCT_FLOAT(
        &coordinatorUnprotected.temperatureOffsetCorrectDisp,
        RAM,
        &sharedTRangeFloatDescriptor15),

    //timeStamp
    CONSTRUCT_SIMPLE_U32(
        &coordinatorUnprotected.timeStamp,
        RAM),

    //actualDensitySelection
    CONSTRUCT_TABENUM8(
        &coordinatorReplaceStaticFreqStaticDefault.actualDensitySelection,
        NON_VOLATILE,
        &actualDensitySelectionDescriptor),

    //tPresetChangeFlag
    CONSTRUCT_SIMPLE_U8(
        &coordinatorUnprotected.tPresetChangeFlag,
        RAM),

    //protectedDataStatus
    CONSTRUCT_TABENUM8(
        &coordinatorProtected.protectedDataStatus,
        RAM,
        &protectedDataStatusDescriptor),

    //page1
    CONSTRUCT_ARRAY_SIMPLE_U8(
        coordinatorStaticFrequentConstantDefault.page1,
        30,
        NON_VOLATILE),

    //temperatureSensor
    CONSTRUCT_SIMPLE_FLOAT(
        &coordinatorDynamicDuplicated.temperatureSensor,
        RAM),

    //temperatureSensor_Duplicated
    CONSTRUCT_SIMPLE_FLOAT(
        &coordinatorDynamicDuplicated.temperatureSensor_Duplicated,
        RAM),

    //temperatureSensor_Obj
    CONSTRUCT_STRUCT_UOM_PF(
        &temperatureSensor_ObjDescriptor),

    //protectedDataCrc
    CONSTRUCT_SIMPLE_U16(
        &coordinatorUnprotected.protectedDataCrc,
        RAM),

    //autoselfcheck_Enable
    CONSTRUCT_TABENUM8(
        &coordinatorDynamicDuplicated.autoselfcheck_Enable,
        RAM,
        &sharedTRangeE8Descriptor9),

    //spanUpperRange
    CONSTRUCT_INT16(
        &coordinatorUnprotected.spanUpperRange,
        RAM,
        &spanUpperRangeDescriptor),

    //temperatureAlmRange_obj
    CONSTRUCT_STRUCT_UOM_PF(
        &temperatureAlmRange_objDescriptor),

    //temperatureMaxRecord_obj
    CONSTRUCT_STRUCT_UOM_PF(
        &temperatureMaxRecord_objDescriptor),

    //sigAmplitude
    CONSTRUCT_SIMPLE_U16(
        &coordinatorUnprotected.sigAmplitude,
        RAM),

    //vbrAmplitude
    CONSTRUCT_SIMPLE_U16(
        &coordinatorUnprotected.vbrAmplitude,
        RAM),

    //sigMagnitude
    CONSTRUCT_SIMPLE_U16(
        &coordinatorUnprotected.sigMagnitude,
        RAM),

    //autoselfcheck_Enable_Duplicated
    CONSTRUCT_TABENUM8(
        &coordinatorDynamicDuplicated.autoselfcheck_Enable_Duplicated,
        RAM,
        &sharedTRangeE8Descriptor9),

    //temperatureOffsetCorrectUser
    CONSTRUCT_FLOAT(
        &coordinatorStaticRare.temperatureOffsetCorrectUser,
        NON_VOLATILE,
        &sharedTRangeFloatDescriptor14),

    //temperatureOffsetCorrectUserDisp
    CONSTRUCT_FLOAT(
        &coordinatorUnprotected.temperatureOffsetCorrectUserDisp,
        RAM,
        &sharedTRangeFloatDescriptor15),

    //reaction_InputFail
    CONSTRUCT_TABENUM8(
        &coordinatorStaticRare.reaction_InputFail,
        NON_VOLATILE,
        &reaction_InputFailDescriptor),

    //BothMaintenanceCycle
    CONSTRUCT_UINT32(
        &coordinatorUnprotected.BothMaintenanceCycle,
        RAM,
        &sharedTRangeU32Descriptor13),

    //rawTemperatureSensor
    CONSTRUCT_SIMPLE_FLOAT(
        &coordinatorUnprotected.rawTemperatureSensor,
        RAM),

    //ext_algSelection
    CONSTRUCT_TABENUM8(
        &coordinatorFE_OUTPUT_FILTER_PARA.ext_algSelection,
        RAM,
        &ext_algSelectionDescriptor),

    //int_algSelection
    CONSTRUCT_TABENUM8(
        &coordinatorFE_OUTPUT_FILTER_PARA.int_algSelection,
        RAM,
        &int_algSelectionDescriptor),

    //config_alg_paras
    CONSTRUCT_ACTION(
        //lint -e{611} cast is OK for T_ACTION
        config_alg_paras_CDT),

    //read_alg_block
    CONSTRUCT_ACTION(
        //lint -e{611} cast is OK for T_ACTION
        read_alg_block_CDT),

    //ss_paras
    CONSTRUCT_ARRAY_SIMPLE_U16(
        coordinatorFE_OUTPUT_FILTER_PARA.ss_paras,
        8,
        RAM),

    //sf_paras
    CONSTRUCT_ARRAY_SIMPLE_U16(
        coordinatorFE_OUTPUT_FILTER_PARA.sf_paras,
        4,
        RAM)
    //@SubGen end@    <OBJECTS>
    //lint +e605 warning on for the rest of the code
};



//-------------------------------------------------------------------------------------------------
//! define the COORDINATOR-Class object
SLOW const T_COORDINATOR coordinator =
{
    {
        &coordinatorState,
        &coordinatorDataSemaphore,

        coordinatorObjectList,
        //lint -e{961} 
        sizeof(coordinatorObjectList)/sizeof(coordinatorObjectList[0])-1,

        coordinatorDataClassList,
        sizeof(coordinatorDataClassList)/sizeof(coordinatorDataClassList[0]),

        ChangeState_T_UNIT,
        GetState_T_UNIT,
        Initialize_COORDINATOR, // will be overloaded
        LoadRomDefaults_COORDINATOR,
        ResetToDefault_T_UNIT,
        SaveAsDefault_T_UNIT,
        Get_CDT,
        Put_CDT,
        Check_T_UNIT,
        GetAttributeDescription_CDT,// over load for enum talbe validation
        GetObjectDescription_T_UNIT
    }

    //subsystem EXE and SRV-methods
    //@SubGen start@    <METHODS>
    ,MainTaskEXE_CDT
    ,FrontendTaskEXE_CDT
    ,HartTaskEXE_CDT
    ,SlowTaskEXE_CDT
    ,ServicesTaskEXE_CDT
    ,UpdateDiagnosis_CDT
    ,UpdateWorkingTime_CDT
    ,SendConfigToFE_CDT
    ,StopTasksSRV_CDT
    ,GetTemperaturePercentageSRV_CDT
    ,UpdateMaxTemperature_CDT
    ,UpdateSteamEnthalpy_CDT
    ,CheckDataCrc_CDT
    ,UpdateFunctionPassword_CDT
    ,UpdateInternalTemperature_CDT
    //@SubGen end@    <METHODS>
};




/*

This section defines templates for the SubSystem CodeGenerator.
Do not modify or remove this section

see also: coordinator_attributes.h


//special templates for specific standard storage classes:
//@SubGen template start@<DATACLASS CONSTANT>
//-------------------------------------------------------------------------------------------------
//! definition of constant block
const SLOW T_COORDINATOR_TEMPLATE coordinatorTemplate=
{
    //@SubGen start@    <INITIALIZER TEMPLATE>
    //@SubGen end@    <INITIALIZER TEMPLATE>
};
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS CONSTANT>



//@SubGen template start@<DATACLASS DYNAMIC>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
FAST T_COORDINATOR_TEMPLATE coordinatorTemplate;
const SLOW T_COORDINATOR_TEMPLATE coordinatorTemplateInitializer=
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
  #pragma bss_seg(".AAA$COORDINATOR_DATA")
    const FAST T_COORDINATOR_TEMPLATE coordinatorTemplate;
    const FAST T_COORDINATOR_TEMPLATE coordinatorTemplateDefault;
  #pragma bss_seg()
#else
  #pragma location="COORDINATOR_TEMPLATE"
    const FAST __no_init T_COORDINATOR_TEMPLATE coordinatorTemplate;
  #pragma location="COORDINATOR_TEMPLATE_DEFAULT"
    const FAST __no_init T_COORDINATOR_TEMPLATE coordinatorTemplateDefault;
#endif
const SLOW T_COORDINATOR_TEMPLATE coordinatorTemplateInitializer=
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
  #pragma bss_seg(".AAA$COORDINATOR_DATA")
    const FAST T_COORDINATOR_TEMPLATE coordinatorTemplate;
  #pragma bss_seg()
#else
  #pragma location="COORDINATOR_TEMPLATE"
    const FAST __no_init T_COORDINATOR_TEMPLATE coordinatorTemplate;
#endif
const SLOW T_COORDINATOR_TEMPLATE coordinatorTemplateInitializer=
{
    //@SubGen start@    <INITIALIZER TEMPLATE>
    //@SubGen end@    <INITIALIZER TEMPLATE>
};
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS>

//@SubGen template start@ <DATACLASSNAME>
coordinatorTemplate
//@SubGen template end@ <DATACLASSNAME>


//@SubGen template start@ <DATACLASSLIST DYNAMIC>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_DYNAMIC(coordinatorTemplate,coordinatorTemplateInitializer)
//@SubGen template end@ <DATACLASSLIST DYNAMIC>

//@SubGen template start@ <DATACLASSLIST CONSTANT>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_CONSTANT(coordinatorTemplate)
//@SubGen template end@ <DATACLASSLIST CONSTANT>

//@SubGen template start@ <DATACLASSLIST>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_STATIC_NODEF(coordinatorTemplate,coordinatorTemplateInitializer)
//@SubGen template end@ <DATACLASSLIST>

//@SubGen template start@ <DATACLASSLIST STATIC_FREQUENT_CONSTANT_DEFAULT>
//lint -e{708} initialization is OK
CONSTRUCTOR_DC_STATIC_CONSTDEF(coordinatorTemplate,coordinatorTemplateInitializer)
//@SubGen template end@ <DATACLASSLIST STATIC_FREQUENT_CONSTANT_DEFAULT>

//@SubGen template start@ <DATACLASSLIST STATIC_FREQUENT_STATIC_DEFAULT>
//lint -e{708} initialization is OK
CONSTRUCTOR_DC_STATIC_STATDEF(coordinatorTemplate,coordinatorTemplateInitializer,coordinatorTemplateDefault)
//@SubGen template end@ <DATACLASSLIST STATIC_FREQUENT_STATIC_DEFAULT>

*/