//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Mapper
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
#include "T_DATA_OBJ/Interface/t_data_obj_float.h"
#include "T_DATA_OBJ/Interface/t_data_obj_simple.h"
#include "T_DATA_OBJ/Interface/t_data_obj_usign8.h"
#include "T_DATA_OBJ/Interface/t_data_obj_tabenum8.h"
#include "T_DATA_OBJ/Interface/t_data_obj_action.h"
#include "T_DATA_OBJ/Interface/t_data_obj_const.h"
#include "T_DATA_OBJ/Interface/t_data_obj_uom_pf.h"
#include "T_DATA_OBJ/Interface/t_data_obj_tabenum16.h"
#include "Diagnosis/interface/t_data_obj_alarm.h"
#include "T_DATA_OBJ/Interface/t_data_obj_uint32.h"

#include "T_Unit/Interface/t_unit.h"
#include "T_Unit/Interface/T_DATACLASSES.H"

#include "Coordinator/Interface/Coordinator_idx.h"

#include "Mapper/Interface/Mapper.h"
#include "Mapper/Interface/Mapper_idx.h"
#include "mapper_types.h"
#include "mapper_attributes.h"
#include "mapper_overload.h"
#include "mapper_execute.h"
#include "mapper_action_fct.h"

extern const SLOW T_DATA_OBJ mapperObjectList[];
extern const SLOW T_DATA_OBJ coordinatorObjectList[];
//-------------------------------------------------------------------------------------------------
//! unit global attributes
FAST TUSIGN16 mapperState;     // Subsystem state
FAST T_DATA_SEMAPHORE mapperDataSemaphore;



const SLOW T_DATACLASS mapperDataClassList[]=
{
    //lint -e{651} Initializers are OK, the warning pop up with the first 708
    //@SubGen start@    <DATACLASSLIST>
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_DYNAMIC(mapperUnprotected,mapperUnprotectedInitializer),
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_DYNAMIC(mapperProtected,mapperProtectedInitializer),
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_DYNAMIC(mapperDynamicDuplicated,mapperDynamicDuplicatedInitializer),
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_CONSTANT(mapperConstant),
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_STATIC_NODEF(mapperStaticRare,mapperStaticRareInitializer),
    //lint -e{708} initialization is OK
    CONSTRUCTOR_DC_STATIC_CONSTDEF(mapperStaticFrequentConstantDefault,mapperStaticFrequentConstantDefaultInitializer),
    //lint -e{708} initialization is OK
    CONSTRUCTOR_DC_STATIC_STATDEF(mapperStaticFrequentStaticDefault,mapperStaticFrequentStaticDefaultInitializer,mapperStaticFrequentStaticDefaultDefault)
    //@SubGen end@    <DATACLASSLIST>
};

//@SubGen put@<DATACLASS>
//-------------------------------------------------------------------------------------------------
//! definition of StaticFrequentStaticDefault data
#ifdef WIN32
  #pragma bss_seg(".AAA$MAPPER_DATA")
    const FAST T_MAPPER_STATIC_FREQUENT_STATIC_DEFAULT mapperStaticFrequentStaticDefault;
    const FAST T_MAPPER_STATIC_FREQUENT_STATIC_DEFAULT mapperStaticFrequentStaticDefaultDefault;
  #pragma bss_seg()
#else
  #pragma location="MAPPER_STATIC_FREQUENT_STATIC_DEFAULT"
    const FAST __no_init T_MAPPER_STATIC_FREQUENT_STATIC_DEFAULT mapperStaticFrequentStaticDefault;
  #pragma location="MAPPER_STATIC_FREQUENT_STATIC_DEFAULT_DEFAULT"
    const FAST __no_init T_MAPPER_STATIC_FREQUENT_STATIC_DEFAULT mapperStaticFrequentStaticDefaultDefault;
#endif
const SLOW T_MAPPER_STATIC_FREQUENT_STATIC_DEFAULT mapperStaticFrequentStaticDefaultInitializer=
{
    //@SubGen start@    <INITIALIZER STATIC_FREQUENT_STATIC_DEFAULT>
    {500.0,-200.0,700.0},    // DEV_RNGFLT dvTExtRange {hiLim,loLim,span}
    20.0f,    // FLOAT dvTExtPreset
    MAPPER_KILOPASCAL,    // TABENUM8 dvPressureUnitCode
    {40000.0f,0.0f,40000.0f},    // DEV_RNGFLT dvPressureRangeRelative {hiLim,loLim,span}
    {40101.325f,0.0f,40101.325f},    // DEV_RNGFLT dvPressureRange {hiLim,loLim,span}
    101.325f,    // FLOAT dvPressurePreset
    MAPPER_KILOGRAM_PER_CUBIC_METER,    // TABENUM8 dvDensityUnitCode
    0.1f,    // FLOAT dvDensityPreset
    {100.0f,0.0f,100.0f},    // DEV_RNGFLT dvContentRange {hiLim,loLim,span}
    100.0f    // FLOAT dvContentPreset
    //@SubGen end@    <INITIALIZER STATIC_FREQUENT_STATIC_DEFAULT>
};
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
FAST T_MAPPER_PROTECTED mapperProtected;
const SLOW T_MAPPER_PROTECTED mapperProtectedInitializer=
{
    //@SubGen start@    <INITIALIZER PROTECTED>
    MAPPER_DEVICE_VAR_QV,    // TABENUM8 PV_Assignment
    MAPPER_DEVICE_VAR_T,    // TABENUM8 SV_Assignment
    MAPPER_FLOWOVER103_BHV_ALARM_OFF,    // TABENUM8 flowOver103Bhv
    MAPPER_SIMU_DISABLE,    // TABENUM8 dvTExtSimEnable
    MAPPER_SIMU_DISABLE,    // TABENUM8 dvPressureSimEnable
    MAPPER_SIMU_DISABLE,    // TABENUM8 dvDensitySimEnable
    MAPPER_SIMU_DISABLE,    // TABENUM8 dvContentSimEnable
    MAPPER_SELECTION_LIQUIDQV,    // TABENUM16 localOperatingModeSelection
    MAPPER_PRESET_ACCESSED,    // TABENUM8 presetChanged
    MAPPER_CALCULATE    // TABENUM8 protectedDataStatus
    //@SubGen end@    <INITIALIZER PROTECTED>
};
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
FAST T_MAPPER_UNPROTECTED mapperUnprotected;
const SLOW T_MAPPER_UNPROTECTED mapperUnprotectedInitializer=
{
    //@SubGen start@    <INITIALIZER UNPROTECTED>
    MAPPER_DISABLE,    // TABENUM8 diagSimulationStatus
    MAPPER_OFF,    // TABENUM8 diagSimulationType
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},    // SIMPLE_U8 diagnosisSimulation[16]
    {0,0,0,0,0,0},    // SIMPLE_U8 diagDeviceStatus[6]
    0,    // SIMPLE_U8 diagnosisHighCondition
    0,    // SIMPLE_U8 diagnosisHighClass
    0,    // SIMPLE_U8 diagnosisHighGroup
    {0,0,0,0,0,0},    // SIMPLE_U8 diagHistory[6]
    0,    // SIMPLE_U8 diagnosisConditionNumber
    0,    // SIMPLE_U8 diagnosisConditionDetailsClass
    0,    // SIMPLE_U8 diagnosisConditionDetailsGroup
    0,    // SIMPLE_U8 diagnosisConditionDetailsPriority
    0,    // SIMPLE_U16 diagnosisConditionDetailsAlarmCounter
    0,    // SIMPLE_U16 diagnosisConditionDetailsAlarmTimeCounterDay
    0,    // SIMPLE_U32 diagnosisConditionDetailsAlarmTimeCounterMsec
    0,    // SIMPLE_U16 diagnosisConditionDetailsTimeStampLastAlarmDay
    0,    // SIMPLE_U32 diagnosisConditionDetailsTimeStampLastAlarmMsec
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},    // SIMPLE_U8 diagnosisConditionDetailsCodes[15]
    {0,0,0,0,0,0},    // SIMPLE_U8 diagnosisMasking[6]
    6.1f,    // FLOAT PV
    0.0f,    // FLOAT SV
    0.0f,    // FLOAT TV
    0.0f,    // FLOAT QV
    {MAPPER_CUBIC_METER_PER_HOUR,MAPPER_T_VOLUME_FLOW},    // USIGN8 PV_Unit[2]
    {MAPPER_CELSIUS,MAPPER_T_TEMPERATURE},    // USIGN8 SV_Unit[2]
    {MAPPER_KILOGRAM_PER_HOUR,MAPPER_T_MASS_FLOW},    // USIGN8 TV_Unit[2]
    {MAPPER_NORMAL_CUBIC_METER_PER_HOUR_0C_1ATM,MAPPER_T_NORMAL_VOLUME_FLOW},    // USIGN8 QV_Unit[2]
    {MAPPER_CUBIC_METER_PER_HOUR,MAPPER_T_VOLUME_FLOW},    // USIGN8 PV_RangeUnit[2]
    {MAPPER_CELSIUS,MAPPER_T_TEMPERATURE},    // USIGN8 SV_RangeUnit[2]
    {MAPPER_KILOGRAM_PER_HOUR,MAPPER_T_MASS_FLOW},    // USIGN8 TV_RangeUnit[2]
    {MAPPER_NORMAL_CUBIC_METER_PER_HOUR_0C_1ATM,MAPPER_T_NORMAL_VOLUME_FLOW},    // USIGN8 QV_RangeUnit[2]
    110.0f,    // FLOAT PV_UpperRange
    110.0f,    // FLOAT SV_UpperRange
    110.0f,    // FLOAT TV_UpperRange
    110.0f,    // FLOAT QV_UpperRange
    6.1f,    // FLOAT PV_LowerRange
    6.1f,    // FLOAT SV_LowerRange
    6.1f,    // FLOAT TV_LowerRange
    6.1f,    // FLOAT QV_LowerRange
    MAPPER_UNSUPPORTED_DEVICE_VAR,    // TABENUM8 TV_Assignment
    MAPPER_UNSUPPORTED_DEVICE_VAR,    // TABENUM8 QV_Assignment
    0,    // USIGN8 PV_Classification
    0,    // USIGN8 SV_Classification
    0,    // USIGN8 TV_Classification
    0,    // USIGN8 QV_Classification
    0,    // USIGN8 PV_Status
    0,    // USIGN8 SV_Status
    0,    // USIGN8 TV_Status
    0,    // USIGN8 QV_Status
    MAPPER_LINEAR,    // TABENUM8 PV_TFCode
    MAPPER_NOT_CONNECTED,    // TABENUM8 PV_ACFlag
    0.0f,    // FLOAT PV_PercentageForDO
    500000.0f,    // SIMPLE_FLOAT PVSensorLimitHigh
    0.0f,    // SIMPLE_FLOAT PVSensorLimitLow
    {0,0},    // SIMPLE_U8 PVSensorLimitUnitcode[2]
    0.1f,    // SIMPLE_FLOAT PVSensorminimumSpan
    100.0f,    // FLOAT PVPercent_Range
    0.0f,    // SIMPLE_FLOAT PV_DampingTime
    MAPPER_UPPER_RANGE,    // TABENUM8 setCurrentValueAsPVRange
    0.0f,    // FLOAT dvTExtSim
    MAPPER_T_TEMPERATURE,    // SIMPLE_U8 dvTExtClassification
    0xC0,    // SIMPLE_U8 dvTExtStatus
    0.0f,    // SIMPLE_FLOAT dvTExtPercentage
    MAPPER_TEXT_FROM_PRESET,    // TABENUM8 dvTExtSrc
    10.0f,    // FLOAT dvPressureSim
    40101.325f,    // SIMPLE_FLOAT dvPressureMaxDN
    0.0f,    // SIMPLE_FLOAT dvPressureMinDN
    40000.0f,    // SIMPLE_FLOAT dvPressureRelativeMaxDN
    0.0f,    // SIMPLE_FLOAT dvPressureRelativeMinDN
    MAPPER_T_PRESSURE,    // SIMPLE_U8 dvPressureClassification
    0xC0,    // SIMPLE_U8 dvPressureStatus
    MAPPER_PRESSURE_FROM_AI,    // TABENUM8 dvPressureSrc
    0.1f,    // FLOAT dvDensitySim
    2000.0f,    // SIMPLE_FLOAT dvDensityMaxDN
    0.08f,    // SIMPLE_FLOAT dvDensityMinDN
    MAPPER_T_MASS_DENSITY,    // SIMPLE_U8 dvDensityClassification
    0xC0,    // SIMPLE_U8 dvDensityStatus
    MAPPER_DENSITY_FROM_PRESET,    // TABENUM8 dvDensitySrc
    1.0f,    // FLOAT dvContentSim
    100.0f,    // SIMPLE_FLOAT dvContentMaxDN
    0.0f,    // SIMPLE_FLOAT dvContentMinDN
    MAPPER_T_ANALYTICAL,    // SIMPLE_U8 dvContentClassification
    0xC0,    // SIMPLE_U8 dvContentStatus
    MAPPER_CONTENT_FORM_PRESET,    // TABENUM8 dvContentSrc
    0.0f,    // SIMPLE_FLOAT dvDummyPercentage
    0.0f,    // SIMPLE_FLOAT dvDummyDampingTime
    0,    // SIMPLE_U8 dvDummyUnitCode
    0,    // SIMPLE_U8 dvDummyUnitType
    0,    // T_DIAGNOSIS_ALARM mapperAlarmSimulation
    0,    // SIMPLE_U8 mapperDiagnosis
    0,    // SIMPLE_U32 diagnosisCounter
    0,    // SIMPLE_U16 protectedDataCrc
    60,    // USIGN32 hartBurstInAlarmPeriod
    0.0f,    // SIMPLE_FLOAT dvTExtUserUnit
    0.0f,    // SIMPLE_FLOAT dvPressureUserUnit
    0.0f,    // SIMPLE_FLOAT dvDensityUserUnit
    {MAPPER_NAMUR_FAILURE,MAPPER_NAMUR_FAILURE,MAPPER_NAMUR_FAILURE,MAPPER_NAMUR_OFFSPECIFICATION,MAPPER_NAMUR_OFFSPECIFICATION,MAPPER_NAMUR_OFFSPECIFICATION,MAPPER_NAMUR_OFFSPECIFICATION,MAPPER_NAMUR_OFFSPECIFICATION,MAPPER_NAMUR_NO_ALARM,MAPPER_NAMUR_NO_ALARM,MAPPER_NAMUR_OFFSPECIFICATION,MAPPER_NAMUR_OFFSPECIFICATION,MAPPER_NAMUR_OFFSPECIFICATION,MAPPER_NAMUR_OFFSPECIFICATION,MAPPER_NAMUR_NO_ALARM,MAPPER_NAMUR_NO_ALARM,MAPPER_NAMUR_FAILURE,MAPPER_NAMUR_FAILURE,MAPPER_NAMUR_MAINTENANCE,MAPPER_NAMUR_FAILURE,MAPPER_NAMUR_FAILURE,MAPPER_NAMUR_NO_ALARM,MAPPER_NAMUR_NO_ALARM,MAPPER_NAMUR_FAILURE,MAPPER_NAMUR_CHECKFUNCTION,MAPPER_NAMUR_OFFSPECIFICATION,MAPPER_NAMUR_OFFSPECIFICATION,MAPPER_NAMUR_NO_ALARM,MAPPER_NAMUR_NO_ALARM,MAPPER_NAMUR_NO_ALARM,MAPPER_NAMUR_OFFSPECIFICATION,MAPPER_NAMUR_MAINTENANCE,MAPPER_NAMUR_MAINTENANCE,MAPPER_NAMUR_OFFSPECIFICATION,MAPPER_NAMUR_OFFSPECIFICATION,MAPPER_NAMUR_OFFSPECIFICATION,MAPPER_NAMUR_OFFSPECIFICATION,MAPPER_NAMUR_CHECKFUNCTION,MAPPER_NAMUR_CHECKFUNCTION,MAPPER_NAMUR_CHECKFUNCTION,MAPPER_NAMUR_FAILURE,MAPPER_NAMUR_FAILURE,MAPPER_NAMUR_FAILURE,MAPPER_NAMUR_MAINTENANCE,MAPPER_NAMUR_FAILURE,MAPPER_NAMUR_CHECKFUNCTION,MAPPER_NAMUR_NO_ALARM,MAPPER_NAMUR_CHECKFUNCTION}    // TABENUM8 diagAlarmNamurConfiguration[48]
    //@SubGen end@    <INITIALIZER UNPROTECTED>
};
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
FAST T_MAPPER_DYNAMIC_DUPLICATED mapperDynamicDuplicated;
const SLOW T_MAPPER_DYNAMIC_DUPLICATED mapperDynamicDuplicatedInitializer=
{
    //@SubGen start@    <INITIALIZER DYNAMIC_DUPLICATED>
    0.0f,    // FLOAT dvTExt
    0.0f,    // FLOAT dvTExtDuplicated
    10.0f,    // FLOAT dvPressure
    10.0f,    // FLOAT dvPressureDuplicated
    0.1f,    // FLOAT dvDensity
    0.1f,    // FLOAT dvDensityDuplicated
    1.0f,    // FLOAT dvContent
    1.0f    // FLOAT dvContentDuplicated
    //@SubGen end@    <INITIALIZER DYNAMIC_DUPLICATED>
};
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//! definition of constant block
const SLOW T_MAPPER_CONSTANT mapperConstant=
{
    //@SubGen start@    <INITIALIZER CONSTANT>
    {MAPPER_PERCENT_UNIT_CODE,0},    // CONST_U8 PVPercent_Unit[2]
    0,    // CONST_U8 PVPercent_Classifiction
    MAPPER_T_TEMPERATURE,    // CONST_U8 dvTExtUnitType
    MAPPER_T_PRESSURE,    // CONST_U8 dvPressureUnitType
    MAPPER_T_MASS_DENSITY    // CONST_U8 dvDensityUnitType
    //@SubGen end@    <INITIALIZER CONSTANT>
};
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//! definition of StaticRare data
#ifdef WIN32
  #pragma bss_seg(".AAA$MAPPER_DATA")
    const FAST T_MAPPER_STATIC_RARE mapperStaticRare;
  #pragma bss_seg()
#else
  #pragma location="MAPPER_STATIC_RARE"
    const FAST __no_init T_MAPPER_STATIC_RARE mapperStaticRare;
#endif
const SLOW T_MAPPER_STATIC_RARE mapperStaticRareInitializer=
{
    //@SubGen start@    <INITIALIZER STATIC_RARE>
    500.0f,    // SIMPLE_FLOAT dvTExtMaxDN
    -200.0f,    // SIMPLE_FLOAT dvTExtMinDN
    MAPPER_KILOPASCAL,    // TABENUM8 dvPressureSimUnitCode
    {40101.325f,0.0f},    // RANGEFLT dvPressureAlmRange {hiLim,loLim}
    MAPPER_KILOGRAM_PER_CUBIC_METER,    // TABENUM8 dvDensitySimUnitCode
    {3000.0,0.08,2999.92},    // DEV_RNGFLT dvDensityRange {hiLim,loLim,span}
    {MAPPER_PERCENT,MAPPER_T_ANALYTICAL}    // SIMPLE_U8 dvContentUnitCode[2]
    //@SubGen end@    <INITIALIZER STATIC_RARE>
};
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//! definition of StaticFrequentConstantDefault data
#ifdef WIN32
  #pragma bss_seg(".AAA$MAPPER_DATA")
    const FAST T_MAPPER_STATIC_FREQUENT_CONSTANT_DEFAULT mapperStaticFrequentConstantDefault;
  #pragma bss_seg()
#else
  #pragma location="MAPPER_STATIC_FREQUENT_CONSTANT_DEFAULT"
    const FAST __no_init T_MAPPER_STATIC_FREQUENT_CONSTANT_DEFAULT mapperStaticFrequentConstantDefault;
#endif
const SLOW T_MAPPER_STATIC_FREQUENT_CONSTANT_DEFAULT mapperStaticFrequentConstantDefaultInitializer=
{
    //@SubGen start@    <INITIALIZER STATIC_FREQUENT_CONSTANT_DEFAULT>
    MAPPER_DISABLE,    // TABENUM8 diagFunCheckMask
    MAPPER_DISABLE,    // TABENUM8 diagOffSpecMask
    MAPPER_DISABLE,    // TABENUM8 diagMaintainMask
    MAPPER_DISABLE,    // TABENUM8 diagMinFlowMask
    MAPPER_DISABLE,    // TABENUM8 diagMaxFlowMask
    MAPPER_DISABLE,    // TABENUM8 diagFlow103Mask
    MAPPER_DISABLE,    // TABENUM8 diagTSensorOffSpecMask
    MAPPER_DISABLE,    // TABENUM8 diagTxSensorOffSpecMask
    MAPPER_DISABLE,    // TABENUM8 diagTSensorFailMask
    MAPPER_ENABLE    // TABENUM8 diagLowCutoffMask
    //@SubGen end@    <INITIALIZER STATIC_FREQUENT_CONSTANT_DEFAULT>
};
//-------------------------------------------------------------------------------------------------





//-------------------------------------------------------------------------------------------------
//! object check and PutGet descriptions
//@SubGen start@<DESCRIPTORS>
const SLOW TUSIGN8 STATUS[] =
// Table referenced by: diagFunCheckMask, diagOffSpecMask, diagSimulationStatus, diagMinFlowMask, diagMaxFlowMask, diagFlow103Mask, diagMaintainMask, diagTSensorOffSpecMask, diagTSensorFailMask, diagTxSensorOffSpecMask, diagLowCutoffMask
{
  MAPPER_DISABLE,
  MAPPER_ENABLE
};

const SLOW TUSIGN8 ALARM_SIMULATION_TYPE[] =
// Table referenced by: diagSimulationType
{
  MAPPER_OFF,
  MAPPER_Simulate_Digital_Out,
  MAPPER_VORTEX_SENSOR_FAILURE,
  MAPPER_TEMP_SENSOR_FAILURE,
  MAPPER_VIBRAT_SENSOR_FAILURE,
  MAPPER_ANALOGINPUT_OUT_OF_RANGE,
  MAPPER_FLOWRATE_REACH_QMAX,
  MAPPER_PROCESS_TEMP_REACH_TMAX,
  MAPPER_ANALOGINPUT_CUT_OFF,
  MAPPER_PRESSURE_REACH_PMAX,
  MAPPER_FLOWRATE_REACH_QMIN,
  MAPPER_PROCESS_TEMP_REACH_TMIN,
  MAPPER_PRESSURE_REACH_PMIN,
  MAPPER_SNR_FAILURE,
  MAPPER_FE_NV_FAILURE,
  MAPPER_SENSOR_CAL_STATUS,
  MAPPER_FE_SYNC_MISSING,
  MAPPER_FE_BOARD_COMM_ERROR,
  MAPPER_CB_NV_FAILURE,
  MAPPER_AI_SPI_COM_FAIL,
  MAPPER_DIGITAL_OUT_CUTOFF,
  MAPPER_RE_OUT_LINEAR_RANGE,
  MAPPER_STEAM_TYPE_MISMATCH,
  MAPPER_MAINTENANCE_WARNING,
  MAPPER_CB_INSUF_INPUT_VOLTAGE,
  MAPPER_AMBIENT_TEMP_REACH_TMIN,
  MAPPER_AMBIENT_TEMP_REACH_TMAX,
  MAPPER_LOW_FLOWRATE_CUTOFF,
  MAPPER_FLOWRATE_BG_103,
  MAPPER_DATA_SIMU_ALARM,
  MAPPER_DIAG_SIMU_ALARM,
  MAPPER_NV_REPLACE_ERROR,
  MAPPER_FE_RAM_FAILURE,
  MAPPER_TOT_COUNTER_STOP,
  MAPPER_NO_HART_BURST_IN
};

const SLOW TUSIGN8 _T_MASS_DENSITY_UNIT[] =
// Table referenced by: dvDensityUnitCode, dvDensitySimUnitCode
{
  MAPPER_KILOGRAM_PER_CUBIC_METER,
  MAPPER_GRAM_PER_CUBIC_CENTIMETER,
  MAPPER_KILOGRAM_PER_LITER,
  MAPPER_GRAM_PER_MILLILITER,
  MAPPER_GRAM_PER_LITER,
  MAPPER_POUND_PER_CUBIC_INCH,
  MAPPER_POUND_PER_CUBIC_FOOT
};

const SLOW TUSIGN8 _T_PRESSURE_UNIT[] =
// Table referenced by: dvPressureUnitCode, dvPressureSimUnitCode
{
  MAPPER_PASCAL_U,
  MAPPER_MEGAPASCAL,
  MAPPER_KILOPASCAL,
  MAPPER_HECTOPASCAL,
  MAPPER_BAR,
  MAPPER_MILLIBAR,
  MAPPER_POUNDFORCE_PER_SQUARE_INCH,
  MAPPER_KILOGRAMFORCE_PER_SQUARE_CENTIMETER,
  MAPPER_MILLIMETER_H2O
};

const SLOW TUSIGN8 TF_CODE[] =
// Table referenced by: PV_TFCode
{
  MAPPER_LINEAR,
  MAPPER_SQUARE_ROOT,
  MAPPER_SQUARE_ROOT_THIRD_POWER,
  MAPPER_SQUARE_ROOT_FIFTH_POWER,
  MAPPER_SPECIAL_CURVE,
  MAPPER_SQUARE,
  MAPPER_DISCRETE,
  MAPPER_SQUARE_ROOT_PLUS_SPECIAL_CURVE,
  MAPPER_SQUARE_ROOT_THIRD_POWER_PLUS_SPECIAL_CURVE,
  MAPPER_SQUARE_ROOT_FIFTH_POWER_PLUS_SPECIAL_CURVE,
  MAPPER_NOT_USED,
  MAPPER_NONE,
  MAPPER_UNKNOWN,
  MAPPER_SPECIAL
};

const SLOW TUSIGN8 AC_FLAG[] =
// Table referenced by: PV_ACFlag
{
  MAPPER_NOT_CONNECTED,
  MAPPER_CONNECTED
};

const SLOW TUSIGN8 DEVICE_VAR_CODE[] =
// Table referenced by: PV_Assignment, SV_Assignment, TV_Assignment, QV_Assignment
{
  MAPPER_DEVICE_VAR_QV,
  MAPPER_DEVICE_VAR_QN,
  MAPPER_DEVICE_VAR_QM,
  MAPPER_DEVICE_VAR_POWER,
  MAPPER_DEVICE_VAR_PARTIAL_QV,
  MAPPER_DEVICE_VAR_PARTIAL_QN,
  MAPPER_DEVICE_VAR_T,
  MAPPER_DEVICE_VAR_TOTQV,
  MAPPER_DEVICE_VAR_TOTQN,
  MAPPER_DEVICE_VAR_TOTQM,
  MAPPER_DEVICE_VAR_TOTENERGY,
  MAPPER_DEVICE_VAR_TOTQV_PARTIAL,
  MAPPER_DEVICE_VAR_TOTQN_PARTIAL,
  MAPPER_UNSUPPORTED_DEVICE_VAR
};

const SLOW TUSIGN8 T_RANGE_CHOOSE[] =
// Table referenced by: setCurrentValueAsPVRange
{
  MAPPER_UPPER_RANGE,
  MAPPER_LOWERER_RANGE
};

const SLOW TUSIGN8 FLOWOVER103_BHV[] =
// Table referenced by: flowOver103Bhv
{
  MAPPER_FLOWOVER103_BHV_ALARM_OFF,
  MAPPER_FLOWOVER103_BHV_ALARM_HI,
  MAPPER_FLOWOVER103_BHV_ALARM_LO
};

const SLOW TUSIGN16 _OPM_SEL_MASK[] =
// Table referenced by: localOperatingModeSelection
{
  MAPPER_SELECTION_LIQUIDQV,
  MAPPER_SELECTION_LIQUIDQN,
  MAPPER_SELECTION_LIQUIDQM,
  MAPPER_SELECTION_LIQUIDPOWER,
  MAPPER_SELECTION_GASQV,
  MAPPER_SELECTION_GASQN,
  MAPPER_SELECTION_GASQM,
  MAPPER_SELECTION_GASPOWER,
  MAPPER_SELECTION_GASPARTIALQV,
  MAPPER_SELECTION_GASPARTIALQN,
  MAPPER_SELECTION_STEAMQV,
  MAPPER_SELECTION_STEAMQM,
  MAPPER_SELECTION_STEAMPOWER
};

const SLOW TUSIGN8 _SIMU_STATUS[] =
// Table referenced by: dvTExtSimEnable, dvPressureSimEnable, dvDensitySimEnable, dvContentSimEnable
{
  MAPPER_SIMU_DISABLE,
  MAPPER_SIMU_ENABLE
};

const SLOW TUSIGN8 MapperStatusTable[] =
// Table referenced by: protectedDataStatus
{
  MAPPER_UNPROTECTED,
  MAPPER_CALCULATE,
  MAPPER_PROTECTED
};

const SLOW TUSIGN8 _TEXT_SRC[] =
// Table referenced by: dvTExtSrc
{
  MAPPER_TEXT_FROM_AI,
  MAPPER_TEXT_FROM_HART,
  MAPPER_TEXT_FROM_PRESET
};

const SLOW TUSIGN8 _PRESSURE_SRC[] =
// Table referenced by: dvPressureSrc
{
  MAPPER_PRESSURE_FROM_AI,
  MAPPER_PRESSURE_FROM_HART,
  MAPPER_PRESSURE_FROM_PRESET
};

const SLOW TUSIGN8 _DENSITY_SRC[] =
// Table referenced by: dvDensitySrc
{
  MAPPER_DENSITY_FROM_AI,
  MAPPER_DENSITY_FROM_HART,
  MAPPER_DENSITY_FROM_PRESET
};

const SLOW TUSIGN8 _CONTENT_SRC[] =
// Table referenced by: dvContentSrc
{
  MAPPER_CONTENT_FORM_AI,
  MAPPER_CONTENT_FORM_HART,
  MAPPER_CONTENT_FORM_PRESET
};

const SLOW TUSIGN8 _MAPPER_DIAGNOSIS_ALARM_TABLE[] =
// Table referenced by: mapperAlarmSimulation
{
  MAPPER_ALM_PRESSURE_REACH_MAX,
  MAPPER_ALM_PRESSURE_REACH_MIN,
  MAPPER_ALM_NO_HART_BURST_IN
};

const SLOW TUSIGN8 _PRESET_STATE[] =
// Table referenced by: presetChanged
{
  MAPPER_PRESET_ACCESSED,
  MAPPER_PRESET_CHANGED
};

const SLOW TUSIGN8 namurAlarmStatusTable[] =
// Table referenced by: diagAlarmNamurConfiguration
{
  MAPPER_NAMUR_NO_ALARM,
  MAPPER_NAMUR_CHECKFUNCTION,
  MAPPER_NAMUR_OFFSPECIFICATION,
  MAPPER_NAMUR_MAINTENANCE,
  MAPPER_NAMUR_FAILURE
};

static const SLOW T_RANGE_E8 sharedTRangeE8Descriptor0 =
// Descriptor referenced by: diagFunCheckMask, diagOffSpecMask, diagSimulationStatus, diagMinFlowMask, diagMaxFlowMask, diagFlow103Mask, diagMaintainMask, diagTSensorOffSpecMask, diagTSensorFailMask, diagTxSensorOffSpecMask, diagLowCutoffMask
{
  STATUS,  // TUSIGN8* ptrCodeTab
  sizeof(STATUS)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 diagSimulationTypeDescriptor =
{
  ALARM_SIMULATION_TYPE,  // TUSIGN8* ptrCodeTab
  sizeof(ALARM_SIMULATION_TYPE)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_FLOAT sharedTRangeFloatDescriptor1 =
// Descriptor referenced by: PV, SV, TV, QV
{
  MAPPER_MINVALUE,  // TFLOAT min
  MAPPER_MAXVALUE,  // TFLOAT max
  0.1f  // TFLOAT increment
};

static const SLOW T_RANGE_E8 sharedTRangeE8Descriptor2 =
// Descriptor referenced by: PV_Assignment, SV_Assignment, TV_Assignment, QV_Assignment
{
  DEVICE_VAR_CODE,  // TUSIGN8* ptrCodeTab
  sizeof(DEVICE_VAR_CODE)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_U8 sharedTRangeU8Descriptor3 =
// Descriptor referenced by: PV_Unit, SV_Unit, TV_Unit, QV_Unit, PV_RangeUnit, PV_Classification, SV_Classification, TV_Classification, QV_Classification, PV_Status, SV_Status, TV_Status, QV_Status, SV_RangeUnit, TV_RangeUnit, QV_RangeUnit
{
  0,  // TUSIGN8 min
  255,  // TUSIGN8 max
  1  // TUSIGN8 increment
};

static const SLOW T_RANGE_FLOAT sharedTRangeFloatDescriptor4 =
// Descriptor referenced by: PV_UpperRange, SV_UpperRange, TV_UpperRange, QV_UpperRange, PV_LowerRange, SV_LowerRange, TV_LowerRange, QV_LowerRange
{
  0.0f,  // TFLOAT min
  MAPPER_MAXVALUE,  // TFLOAT max
  0.1f  // TFLOAT increment
};

static const SLOW T_RANGE_FLOAT sharedTRangeFloatDescriptor5 =
// Descriptor referenced by: PV_PercentageForDO, dvContent
{
  0.0f,  // TFLOAT min
  100.0f,  // TFLOAT max
  0.1f  // TFLOAT increment
};

static const SLOW T_RANGE_E8 PV_TFCodeDescriptor =
{
  TF_CODE,  // TUSIGN8* ptrCodeTab
  sizeof(TF_CODE)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 PV_ACFlagDescriptor =
{
  AC_FLAG,  // TUSIGN8* ptrCodeTab
  sizeof(AC_FLAG)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 setCurrentValueAsPVRangeDescriptor =
{
  T_RANGE_CHOOSE,  // TUSIGN8* ptrCodeTab
  sizeof(T_RANGE_CHOOSE)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_FLOAT PVPercent_RangeDescriptor =
{
  0.0f,  // TFLOAT min
  255.0f,  // TFLOAT max
  0.1f  // TFLOAT increment
};

static const SLOW T_RANGE_E8 flowOver103BhvDescriptor =
{
  FLOWOVER103_BHV,  // TUSIGN8* ptrCodeTab
  sizeof(FLOWOVER103_BHV)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 sharedTRangeE8Descriptor6 =
// Descriptor referenced by: dvTExtSimEnable, dvPressureSimEnable, dvDensitySimEnable, dvContentSimEnable
{
  _SIMU_STATUS,  // TUSIGN8* ptrCodeTab
  sizeof(_SIMU_STATUS)  // TUSIGN8 numberOfCodes
};

static const SLOW T_UOM_PF_DESCRIPTOR dvTExtObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(temperature_UnitCode, coordinator, CDT),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(dvTExt,mapper,MAPPER),  // void * pObject_value
  MAPPER_T_TEMPERATURE  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR dvTExtSimObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(simTemperature_UnitCode, coordinator, CDT),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(dvTExtSim,mapper,MAPPER),  // void * pObject_value
  MAPPER_T_TEMPERATURE  // TUSIGN8 unitsType
};

static const SLOW T_RANGE_E8 sharedTRangeE8Descriptor7 =
// Descriptor referenced by: dvPressureUnitCode, dvPressureSimUnitCode
{
  _T_PRESSURE_UNIT,  // TUSIGN8* ptrCodeTab
  sizeof(_T_PRESSURE_UNIT)  // TUSIGN8 numberOfCodes
};

static const SLOW T_UOM_PF_DESCRIPTOR dvPressureSimObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(dvPressureSimUnitCode, mapper, MAPPER),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(dvPressureSim, mapper, MAPPER),  // void * pObject_value
  MAPPER_T_PRESSURE  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR dvPressureObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(dvPressureUnitCode, mapper, MAPPER),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(dvPressure, mapper, MAPPER),  // void * pObject_value
  MAPPER_T_PRESSURE  // TUSIGN8 unitsType
};

static const SLOW T_RANGE_E8 sharedTRangeE8Descriptor8 =
// Descriptor referenced by: dvDensityUnitCode, dvDensitySimUnitCode
{
  _T_MASS_DENSITY_UNIT,  // TUSIGN8* ptrCodeTab
  sizeof(_T_MASS_DENSITY_UNIT)  // TUSIGN8 numberOfCodes
};

static const SLOW T_UOM_PF_DESCRIPTOR dvDensitySimObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(dvDensitySimUnitCode, mapper, MAPPER),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(dvDensitySim, mapper, MAPPER),  // void * pObject_value
  MAPPER_T_MASS_DENSITY  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR dvDensityObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(dvDensityUnitCode, mapper, MAPPER),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(dvDensity, mapper, MAPPER),  // void * pObject_value
  MAPPER_T_MASS_DENSITY  // TUSIGN8 unitsType
};

static const SLOW T_RANGE_E16 localOperatingModeSelectionDescriptor =
{
  _OPM_SEL_MASK,  // TUSIGN16* ptrCodeTab
  MAPPER_OPM_NUM  // TUSIGN16 numberOfCodes
};

static const SLOW T_UOM_PF_DESCRIPTOR dvPressurePresetObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(dvPressureUnitCode, mapper, MAPPER),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(dvPressurePreset, mapper, MAPPER),  // void * pObject_value
  MAPPER_T_PRESSURE  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR dvDensityPresetObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(dvDensityUnitCode, mapper, MAPPER),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(dvDensityPreset, mapper, MAPPER),  // void * pObject_value
  MAPPER_T_MASS_DENSITY  // TUSIGN8 unitsType
};

static const SLOW T_RANGEFLT_DESCRIPTOR dvPressureAlmRangeDescriptor =
{
  40101.325f,  // TFLOAT maxHi
  0.0f  // TFLOAT maxLo
};

static const SLOW T_UOM_PF_DESCRIPTOR dvTExtPresetObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(temperature_UnitCode, coordinator, CDT),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(dvTExtPreset,mapper,MAPPER),  // void * pObject_value
  MAPPER_T_TEMPERATURE  // TUSIGN8 unitsType
};

static const SLOW T_RANGE_E8 dvTExtSrcDescriptor =
{
  _TEXT_SRC,  // TUSIGN8* ptrCodeTab
  sizeof(_TEXT_SRC)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 dvPressureSrcDescriptor =
{
  _PRESSURE_SRC,  // TUSIGN8* ptrCodeTab
  sizeof(_PRESSURE_SRC)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 dvDensitySrcDescriptor =
{
  _DENSITY_SRC,  // TUSIGN8* ptrCodeTab
  sizeof(_DENSITY_SRC)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 dvContentSrcDescriptor =
{
  _CONTENT_SRC,  // TUSIGN8* ptrCodeTab
  sizeof(_CONTENT_SRC)  // TUSIGN8 numberOfCodes
};

const SLOW T_RANGE_E8 mapperAlarmSimulationDescriptor =
{
  _MAPPER_DIAGNOSIS_ALARM_TABLE,  // TUSIGN8* ptrCodeTab
  sizeof(_MAPPER_DIAGNOSIS_ALARM_TABLE)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 presetChangedDescriptor =
{
  _PRESET_STATE,  // TUSIGN8* ptrCodeTab
  sizeof(_PRESET_STATE)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_FLOAT sharedTRangeFloatDescriptor9 =
// Descriptor referenced by: dvTExtPreset, dvTExtSim
{
  -200.0f,  // TFLOAT min
  500.0f,  // TFLOAT max
  0.1f  // TFLOAT increment
};

static const SLOW T_RANGE_FLOAT sharedTRangeFloatDescriptor10 =
// Descriptor referenced by: dvTExt, dvTExtDuplicated
{
  -300.0f,  // TFLOAT min
  600.0f,  // TFLOAT max
  0.1f  // TFLOAT increment
};

static const SLOW T_RANGE_FLOAT sharedTRangeFloatDescriptor11 =
// Descriptor referenced by: dvPressure, dvPressureDuplicated, dvPressureSim, dvPressurePreset
{
  0.0f,  // TFLOAT min
  40101.325f,  // TFLOAT max
  0.1f  // TFLOAT increment
};

static const SLOW T_RANGE_FLOAT sharedTRangeFloatDescriptor12 =
// Descriptor referenced by: dvDensity, dvDensityDuplicated, dvDensitySim
{
  0.0f,  // TFLOAT min
  3000.0f,  // TFLOAT max
  0.1f  // TFLOAT increment
};

static const SLOW T_RANGE_FLOAT sharedTRangeFloatDescriptor13 =
// Descriptor referenced by: dvContentDuplicated, dvContentSim, dvContentPreset
{
  0.0f,  // TFLOAT min
  100.0f,  // TFLOAT max
  0.01f  // TFLOAT increment
};

static const SLOW T_RANGE_FLOAT dvDensityPresetDescriptor =
{
  0.08f,  // TFLOAT min
  3000.0f,  // TFLOAT max
  0.01f  // TFLOAT increment
};

static const SLOW T_DEV_RNGFLT_DESCRIPTOR dvTExtRangeDescriptor =
{
  SUBSYS_OBJPTR_IDX_(dvTExtMaxDN,mapper,MAPPER),  // void * ptrMaxDN
  SUBSYS_OBJPTR_IDX_(dvTExtMinDN,mapper,MAPPER),  // void * ptrMinDN
  1.0f,  // TFLOAT hiLim_MaxPrcOfDnSpan
  0.0f,  // TFLOAT hiLim_MinPrcOfDnSpan
  1.0f,  // TFLOAT loLim_MaxPrcOfCurAllowedSpan
  0.0f  // TFLOAT loLim_MinPrcOfCurAllowedSpan
};

static const SLOW T_UOM_PF_DESCRIPTOR dvTExtRangeObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(temperature_UnitCode, coordinator, CDT),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(dvTExtRange,mapper,MAPPER),  // void * pObject_value
  MAPPER_T_TEMPERATURE  // TUSIGN8 unitsType
};

static const SLOW T_DEV_RNGFLT_DESCRIPTOR dvPressureRangeRelativeDescriptor =
{
  SUBSYS_OBJPTR_IDX_(dvPressureRelativeMaxDN,mapper,MAPPER),  // void * ptrMaxDN
  SUBSYS_OBJPTR_IDX_(dvPressureRelativeMinDN,mapper,MAPPER),  // void * ptrMinDN
  1.0f,  // TFLOAT hiLim_MaxPrcOfDnSpan
  0.0f,  // TFLOAT hiLim_MinPrcOfDnSpan
  1.0f,  // TFLOAT loLim_MaxPrcOfCurAllowedSpan
  0.0f  // TFLOAT loLim_MinPrcOfCurAllowedSpan
};

static const SLOW T_DEV_RNGFLT_DESCRIPTOR dvDensityRangeDescriptor =
{
  SUBSYS_OBJPTR_IDX_(dvDensityMaxDN,mapper,MAPPER),  // void * ptrMaxDN
  SUBSYS_OBJPTR_IDX_(dvDensityMinDN,mapper,MAPPER),  // void * ptrMinDN
  1.0f,  // TFLOAT hiLim_MaxPrcOfDnSpan
  0.0f,  // TFLOAT hiLim_MinPrcOfDnSpan
  1.0f,  // TFLOAT loLim_MaxPrcOfCurAllowedSpan
  0.0f  // TFLOAT loLim_MinPrcOfCurAllowedSpan
};

static const SLOW T_DEV_RNGFLT_DESCRIPTOR dvContentRangeDescriptor =
{
  SUBSYS_OBJPTR_IDX_(dvContentMaxDN,mapper,MAPPER),  // void * ptrMaxDN
  SUBSYS_OBJPTR_IDX_(dvContentMinDN,mapper,MAPPER),  // void * ptrMinDN
  1.0f,  // TFLOAT hiLim_MaxPrcOfDnSpan
  0.0f,  // TFLOAT hiLim_MinPrcOfDnSpan
  1.0f,  // TFLOAT loLim_MaxPrcOfCurAllowedSpan
  0.0f  // TFLOAT loLim_MinPrcOfCurAllowedSpan
};

static const SLOW T_UOM_PF_DESCRIPTOR dvPressureRangeObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(dvPressureUnitCode, mapper, MAPPER),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(dvPressureRange, mapper, MAPPER),  // void * pObject_value
  MAPPER_T_PRESSURE  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR dvDensityRangeObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(dvDensityUnitCode, mapper, MAPPER),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(dvDensityRange, mapper, MAPPER),  // void * pObject_value
  MAPPER_T_MASS_DENSITY  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR dvTExtMaxDN_ObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(temperature_UnitCode, coordinator, CDT),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(dvTExtMaxDN,mapper,MAPPER),  // void * pObject_value
  MAPPER_T_TEMPERATURE  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR dvTExtMinDN_ObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(temperature_UnitCode, coordinator, CDT),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(dvTExtMinDN,mapper,MAPPER),  // void * pObject_value
  MAPPER_T_TEMPERATURE  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR dvDensityMinDN_ObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(dvDensityUnitCode, mapper, MAPPER),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(dvDensityMinDN, mapper, MAPPER),  // void * pObject_value
  MAPPER_T_MASS_DENSITY  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR dvDensityMaxDN_ObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(dvDensityUnitCode, mapper, MAPPER),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(dvDensityMaxDN, mapper, MAPPER),  // void * pObject_value
  MAPPER_T_MASS_DENSITY  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR dvPressureMaxDN_ObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(dvPressureUnitCode, mapper, MAPPER),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(dvPressureMaxDN, mapper, MAPPER),  // void * pObject_value
  MAPPER_T_PRESSURE  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR dvPressureMinDN_ObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(dvPressureUnitCode, mapper, MAPPER),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(dvPressureMinDN, mapper, MAPPER),  // void * pObject_value
  MAPPER_T_PRESSURE  // TUSIGN8 unitsType
};

static const SLOW T_RANGE_E8 protectedDataStatusDescriptor =
{
  MapperStatusTable,  // TUSIGN8* ptrCodeTab
  sizeof(MapperStatusTable)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_U32 hartBurstInAlarmPeriodDescriptor =
{
  5,  // TUSIGN32 min
  10800,  // TUSIGN32 max
  1  // TUSIGN32 increment
};

static const SLOW T_DEV_RNGFLT_DESCRIPTOR dvPressureRangeDescriptor =
{
  SUBSYS_OBJPTR_IDX_(dvPressureMaxDN,mapper,MAPPER),  // void * ptrMaxDN
  SUBSYS_OBJPTR_IDX_(dvPressureMinDN,mapper,MAPPER),  // void * ptrMinDN
  1.0f,  // TFLOAT hiLim_MaxPrcOfDnSpan
  0.0f,  // TFLOAT hiLim_MinPrcOfDnSpan
  1.0f,  // TFLOAT loLim_MaxPrcOfCurAllowedSpan
  0.0f  // TFLOAT loLim_MinPrcOfCurAllowedSpan
};

static const SLOW T_UOM_PF_DESCRIPTOR dvPressureRangeRelativeObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(dvPressureUnitCode, mapper, MAPPER),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(dvPressureRangeRelative, mapper, MAPPER),  // void * pObject_value
  MAPPER_T_PRESSURE  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR dvPressureAlmRangeObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(dvPressureUnitCode, mapper, MAPPER),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(dvPressureAlmRange, mapper, MAPPER),  // void * pObject_value
  MAPPER_T_PRESSURE  // TUSIGN8 unitsType
};

static const SLOW T_RANGE_E8 diagAlarmNamurConfigurationDescriptor =
{
  namurAlarmStatusTable,  // TUSIGN8* ptrCodeTab
  sizeof(namurAlarmStatusTable)  // TUSIGN8 numberOfCodes
};
//@SubGen end@<DESCRIPTORS>


//-------------------------------------------------------------------------------------------------
//! objectList
const SLOW T_DATA_OBJ mapperObjectList[] =
{
    //lint -e605 All non-volatile data are const for T_UNIT
    //           and the typecast (void FAST*) does not increase the readability
    //@SubGen start@    <OBJECTS>
    //diagFunCheckMask
    CONSTRUCT_TABENUM8(
        &mapperStaticFrequentConstantDefault.diagFunCheckMask,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor0),

    //diagOffSpecMask
    CONSTRUCT_TABENUM8(
        &mapperStaticFrequentConstantDefault.diagOffSpecMask,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor0),

    //diagMaintainMask
    CONSTRUCT_TABENUM8(
        &mapperStaticFrequentConstantDefault.diagMaintainMask,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor0),

    //diagSimulationStatus
    CONSTRUCT_TABENUM8(
        &mapperUnprotected.diagSimulationStatus,
        RAM,
        &sharedTRangeE8Descriptor0),

    //diagSimulationType
    CONSTRUCT_TABENUM8(
        &mapperUnprotected.diagSimulationType,
        RAM,
        &diagSimulationTypeDescriptor),

    //diagnosisSimulation
    CONSTRUCT_ARRAY_SIMPLE_U8(
        mapperUnprotected.diagnosisSimulation,
        16,
        RAM),

    //diagClearHistory
    CONSTRUCT_ACTION(
        //lint -e{611} cast is OK for T_ACTION
        diagClearHistory_MAPPER),

    //diagMinFlowMask
    CONSTRUCT_TABENUM8(
        &mapperStaticFrequentConstantDefault.diagMinFlowMask,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor0),

    //diagMaxFlowMask
    CONSTRUCT_TABENUM8(
        &mapperStaticFrequentConstantDefault.diagMaxFlowMask,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor0),

    //diagFlow103Mask
    CONSTRUCT_TABENUM8(
        &mapperStaticFrequentConstantDefault.diagFlow103Mask,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor0),

    //diagTSensorOffSpecMask
    CONSTRUCT_TABENUM8(
        &mapperStaticFrequentConstantDefault.diagTSensorOffSpecMask,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor0),

    //diagTxSensorOffSpecMask
    CONSTRUCT_TABENUM8(
        &mapperStaticFrequentConstantDefault.diagTxSensorOffSpecMask,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor0),

    //diagTSensorFailMask
    CONSTRUCT_TABENUM8(
        &mapperStaticFrequentConstantDefault.diagTSensorFailMask,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor0),

    //diagLowCutoffMask
    CONSTRUCT_TABENUM8(
        &mapperStaticFrequentConstantDefault.diagLowCutoffMask,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor0),

    //diagDeviceStatus
    CONSTRUCT_ARRAY_SIMPLE_U8(
        mapperUnprotected.diagDeviceStatus,
        6,
        RAM),

    //diagnosisHighCondition
    CONSTRUCT_SIMPLE_U8(
        &mapperUnprotected.diagnosisHighCondition,
        RAM),

    //diagnosisHighClass
    CONSTRUCT_SIMPLE_U8(
        &mapperUnprotected.diagnosisHighClass,
        RAM),

    //diagnosisHighGroup
    CONSTRUCT_SIMPLE_U8(
        &mapperUnprotected.diagnosisHighGroup,
        RAM),

    //diagHistory
    CONSTRUCT_ARRAY_SIMPLE_U8(
        mapperUnprotected.diagHistory,
        6,
        RAM),

    //diagnosisConditionNumber
    CONSTRUCT_SIMPLE_U8(
        &mapperUnprotected.diagnosisConditionNumber,
        RAM),

    //diagnosisConditionDetailsClass
    CONSTRUCT_SIMPLE_U8(
        &mapperUnprotected.diagnosisConditionDetailsClass,
        RAM),

    //diagnosisConditionDetailsGroup
    CONSTRUCT_SIMPLE_U8(
        &mapperUnprotected.diagnosisConditionDetailsGroup,
        RAM),

    //diagnosisConditionDetailsPriority
    CONSTRUCT_SIMPLE_U8(
        &mapperUnprotected.diagnosisConditionDetailsPriority,
        RAM),

    //diagnosisConditionDetailsAlarmCounter
    CONSTRUCT_SIMPLE_U16(
        &mapperUnprotected.diagnosisConditionDetailsAlarmCounter,
        RAM),

    //diagnosisConditionDetailsAlarmTimeCounterDay
    CONSTRUCT_SIMPLE_U16(
        &mapperUnprotected.diagnosisConditionDetailsAlarmTimeCounterDay,
        RAM),

    //diagnosisConditionDetailsAlarmTimeCounterMsec
    CONSTRUCT_SIMPLE_U32(
        &mapperUnprotected.diagnosisConditionDetailsAlarmTimeCounterMsec,
        RAM),

    //diagnosisConditionDetailsTimeStampLastAlarmDay
    CONSTRUCT_SIMPLE_U16(
        &mapperUnprotected.diagnosisConditionDetailsTimeStampLastAlarmDay,
        RAM),

    //diagnosisConditionDetailsTimeStampLastAlarmMsec
    CONSTRUCT_SIMPLE_U32(
        &mapperUnprotected.diagnosisConditionDetailsTimeStampLastAlarmMsec,
        RAM),

    //diagnosisConditionDetailsCodes
    CONSTRUCT_ARRAY_SIMPLE_U8(
        mapperUnprotected.diagnosisConditionDetailsCodes,
        15,
        RAM),

    //diagnosisMasking
    CONSTRUCT_ARRAY_SIMPLE_U8(
        mapperUnprotected.diagnosisMasking,
        6,
        RAM),

    //PV
    CONSTRUCT_FLOAT(
        &mapperUnprotected.PV,
        RAM,
        &sharedTRangeFloatDescriptor1),

    //SV
    CONSTRUCT_FLOAT(
        &mapperUnprotected.SV,
        RAM,
        &sharedTRangeFloatDescriptor1),

    //TV
    CONSTRUCT_FLOAT(
        &mapperUnprotected.TV,
        RAM,
        &sharedTRangeFloatDescriptor1),

    //QV
    CONSTRUCT_FLOAT(
        &mapperUnprotected.QV,
        RAM,
        &sharedTRangeFloatDescriptor1),

    //PV_Unit
    CONSTRUCT_ARRAY_USIGN8(
        mapperUnprotected.PV_Unit,
        2,
        RAM,
        &sharedTRangeU8Descriptor3),

    //SV_Unit
    CONSTRUCT_ARRAY_USIGN8(
        mapperUnprotected.SV_Unit,
        2,
        RAM,
        &sharedTRangeU8Descriptor3),

    //TV_Unit
    CONSTRUCT_ARRAY_USIGN8(
        mapperUnprotected.TV_Unit,
        2,
        RAM,
        &sharedTRangeU8Descriptor3),

    //QV_Unit
    CONSTRUCT_ARRAY_USIGN8(
        mapperUnprotected.QV_Unit,
        2,
        RAM,
        &sharedTRangeU8Descriptor3),

    //PV_RangeUnit
    CONSTRUCT_ARRAY_USIGN8(
        mapperUnprotected.PV_RangeUnit,
        2,
        RAM,
        &sharedTRangeU8Descriptor3),

    //SV_RangeUnit
    CONSTRUCT_ARRAY_USIGN8(
        mapperUnprotected.SV_RangeUnit,
        2,
        RAM,
        &sharedTRangeU8Descriptor3),

    //TV_RangeUnit
    CONSTRUCT_ARRAY_USIGN8(
        mapperUnprotected.TV_RangeUnit,
        2,
        RAM,
        &sharedTRangeU8Descriptor3),

    //QV_RangeUnit
    CONSTRUCT_ARRAY_USIGN8(
        mapperUnprotected.QV_RangeUnit,
        2,
        RAM,
        &sharedTRangeU8Descriptor3),

    //PV_UpperRange
    CONSTRUCT_FLOAT(
        &mapperUnprotected.PV_UpperRange,
        RAM,
        &sharedTRangeFloatDescriptor4),

    //SV_UpperRange
    CONSTRUCT_FLOAT(
        &mapperUnprotected.SV_UpperRange,
        RAM,
        &sharedTRangeFloatDescriptor4),

    //TV_UpperRange
    CONSTRUCT_FLOAT(
        &mapperUnprotected.TV_UpperRange,
        RAM,
        &sharedTRangeFloatDescriptor4),

    //QV_UpperRange
    CONSTRUCT_FLOAT(
        &mapperUnprotected.QV_UpperRange,
        RAM,
        &sharedTRangeFloatDescriptor4),

    //PV_LowerRange
    CONSTRUCT_FLOAT(
        &mapperUnprotected.PV_LowerRange,
        RAM,
        &sharedTRangeFloatDescriptor4),

    //SV_LowerRange
    CONSTRUCT_FLOAT(
        &mapperUnprotected.SV_LowerRange,
        RAM,
        &sharedTRangeFloatDescriptor4),

    //TV_LowerRange
    CONSTRUCT_FLOAT(
        &mapperUnprotected.TV_LowerRange,
        RAM,
        &sharedTRangeFloatDescriptor4),

    //QV_LowerRange
    CONSTRUCT_FLOAT(
        &mapperUnprotected.QV_LowerRange,
        RAM,
        &sharedTRangeFloatDescriptor4),

    //PV_Assignment
    CONSTRUCT_TABENUM8(
        &mapperProtected.PV_Assignment,
        RAM,
        &sharedTRangeE8Descriptor2),

    //SV_Assignment
    CONSTRUCT_TABENUM8(
        &mapperProtected.SV_Assignment,
        RAM,
        &sharedTRangeE8Descriptor2),

    //TV_Assignment
    CONSTRUCT_TABENUM8(
        &mapperUnprotected.TV_Assignment,
        RAM,
        &sharedTRangeE8Descriptor2),

    //QV_Assignment
    CONSTRUCT_TABENUM8(
        &mapperUnprotected.QV_Assignment,
        RAM,
        &sharedTRangeE8Descriptor2),

    //PV_Classification
    CONSTRUCT_USIGN8(
        &mapperUnprotected.PV_Classification,
        RAM,
        &sharedTRangeU8Descriptor3),

    //SV_Classification
    CONSTRUCT_USIGN8(
        &mapperUnprotected.SV_Classification,
        RAM,
        &sharedTRangeU8Descriptor3),

    //TV_Classification
    CONSTRUCT_USIGN8(
        &mapperUnprotected.TV_Classification,
        RAM,
        &sharedTRangeU8Descriptor3),

    //QV_Classification
    CONSTRUCT_USIGN8(
        &mapperUnprotected.QV_Classification,
        RAM,
        &sharedTRangeU8Descriptor3),

    //PV_Status
    CONSTRUCT_USIGN8(
        &mapperUnprotected.PV_Status,
        RAM,
        &sharedTRangeU8Descriptor3),

    //SV_Status
    CONSTRUCT_USIGN8(
        &mapperUnprotected.SV_Status,
        RAM,
        &sharedTRangeU8Descriptor3),

    //TV_Status
    CONSTRUCT_USIGN8(
        &mapperUnprotected.TV_Status,
        RAM,
        &sharedTRangeU8Descriptor3),

    //QV_Status
    CONSTRUCT_USIGN8(
        &mapperUnprotected.QV_Status,
        RAM,
        &sharedTRangeU8Descriptor3),

    //PV_TFCode
    CONSTRUCT_TABENUM8(
        &mapperUnprotected.PV_TFCode,
        RAM,
        &PV_TFCodeDescriptor),

    //PV_ACFlag
    CONSTRUCT_TABENUM8(
        &mapperUnprotected.PV_ACFlag,
        RAM,
        &PV_ACFlagDescriptor),

    //PV_PercentageForDO
    CONSTRUCT_FLOAT(
        &mapperUnprotected.PV_PercentageForDO,
        RAM,
        &sharedTRangeFloatDescriptor5),

    //PVSensorLimitHigh
    CONSTRUCT_SIMPLE_FLOAT(
        &mapperUnprotected.PVSensorLimitHigh,
        RAM),

    //PVSensorLimitLow
    CONSTRUCT_SIMPLE_FLOAT(
        &mapperUnprotected.PVSensorLimitLow,
        RAM),

    //PVSensorLimitUnitcode
    CONSTRUCT_ARRAY_SIMPLE_U8(
        mapperUnprotected.PVSensorLimitUnitcode,
        2,
        RAM),

    //PVSensorminimumSpan
    CONSTRUCT_SIMPLE_FLOAT(
        &mapperUnprotected.PVSensorminimumSpan,
        RAM),

    //PVPercent_Unit
    CONSTRUCT_ARRAY_CONST_U8(
        mapperConstant.PVPercent_Unit,
        2),

    //PVPercent_Range
    CONSTRUCT_FLOAT(
        &mapperUnprotected.PVPercent_Range,
        RAM,
        &PVPercent_RangeDescriptor),

    //PVPercent_Classifiction
    CONSTRUCT_CONST_U8(
        &mapperConstant.PVPercent_Classifiction),

    //PV_DampingTime
    CONSTRUCT_SIMPLE_FLOAT(
        &mapperUnprotected.PV_DampingTime,
        RAM),

    //setCurrentValueAsPVRange
    CONSTRUCT_TABENUM8(
        &mapperUnprotected.setCurrentValueAsPVRange,
        RAM,
        &setCurrentValueAsPVRangeDescriptor),

    //flowOver103Bhv
    CONSTRUCT_TABENUM8(
        &mapperProtected.flowOver103Bhv,
        RAM,
        &flowOver103BhvDescriptor),

    //dvTExt
    CONSTRUCT_FLOAT(
        &mapperDynamicDuplicated.dvTExt,
        RAM,
        &sharedTRangeFloatDescriptor10),

    //dvTExtDuplicated
    CONSTRUCT_FLOAT(
        &mapperDynamicDuplicated.dvTExtDuplicated,
        RAM,
        &sharedTRangeFloatDescriptor10),

    //dvTExtSimEnable
    CONSTRUCT_TABENUM8(
        &mapperProtected.dvTExtSimEnable,
        RAM,
        &sharedTRangeE8Descriptor6),

    //dvTExtSim
    CONSTRUCT_FLOAT(
        &mapperUnprotected.dvTExtSim,
        RAM,
        &sharedTRangeFloatDescriptor9),

    //dvTExtUnitType
    CONSTRUCT_CONST_U8(
        &mapperConstant.dvTExtUnitType),

    //dvTExtObj
    CONSTRUCT_STRUCT_UOM_PF(
        &dvTExtObjDescriptor),

    //dvTExtSimObj
    CONSTRUCT_STRUCT_UOM_PF(
        &dvTExtSimObjDescriptor),

    //dvTExtRange
    CONSTRUCT_STRUCT_DEV_RNGFLT(
        &mapperStaticFrequentStaticDefault.dvTExtRange,
        NON_VOLATILE,
        &dvTExtRangeDescriptor),

    //dvTExtRangeObj
    CONSTRUCT_STRUCT_UOM_PF(
        &dvTExtRangeObjDescriptor),

    //dvTExtClassification
    CONSTRUCT_SIMPLE_U8(
        &mapperUnprotected.dvTExtClassification,
        READONLY_RAM),

    //dvTExtStatus
    CONSTRUCT_SIMPLE_U8(
        &mapperUnprotected.dvTExtStatus,
        RAM),

    //dvTExtPercentage
    CONSTRUCT_SIMPLE_FLOAT(
        &mapperUnprotected.dvTExtPercentage,
        RAM),

    //dvTExtPreset
    CONSTRUCT_FLOAT(
        &mapperStaticFrequentStaticDefault.dvTExtPreset,
        NON_VOLATILE,
        &sharedTRangeFloatDescriptor9),

    //dvTExtPresetObj
    CONSTRUCT_STRUCT_UOM_PF(
        &dvTExtPresetObjDescriptor),

    //dvTExtSrc
    CONSTRUCT_TABENUM8(
        &mapperUnprotected.dvTExtSrc,
        RAM,
        &dvTExtSrcDescriptor),

    //dvTExtMaxDN
    CONSTRUCT_SIMPLE_FLOAT(
        &mapperStaticRare.dvTExtMaxDN,
        NON_VOLATILE),

    //dvTExtMinDN
    CONSTRUCT_SIMPLE_FLOAT(
        &mapperStaticRare.dvTExtMinDN,
        NON_VOLATILE),

    //dvTExtMaxDN_Obj
    CONSTRUCT_STRUCT_UOM_PF(
        &dvTExtMaxDN_ObjDescriptor),

    //dvTExtMinDN_Obj
    CONSTRUCT_STRUCT_UOM_PF(
        &dvTExtMinDN_ObjDescriptor),

    //dvPressure
    CONSTRUCT_FLOAT(
        &mapperDynamicDuplicated.dvPressure,
        RAM,
        &sharedTRangeFloatDescriptor11),

    //dvPressureDuplicated
    CONSTRUCT_FLOAT(
        &mapperDynamicDuplicated.dvPressureDuplicated,
        RAM,
        &sharedTRangeFloatDescriptor11),

    //dvPressureSimEnable
    CONSTRUCT_TABENUM8(
        &mapperProtected.dvPressureSimEnable,
        RAM,
        &sharedTRangeE8Descriptor6),

    //dvPressureSim
    CONSTRUCT_FLOAT(
        &mapperUnprotected.dvPressureSim,
        RAM,
        &sharedTRangeFloatDescriptor11),

    //dvPressureUnitCode
    CONSTRUCT_TABENUM8(
        &mapperStaticFrequentStaticDefault.dvPressureUnitCode,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor7),

    //dvPressureUnitType
    CONSTRUCT_CONST_U8(
        &mapperConstant.dvPressureUnitType),

    //dvPressureSimUnitCode
    CONSTRUCT_TABENUM8(
        &mapperStaticRare.dvPressureSimUnitCode,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor7),

    //dvPressureObj
    CONSTRUCT_STRUCT_UOM_PF(
        &dvPressureObjDescriptor),

    //dvPressureSimObj
    CONSTRUCT_STRUCT_UOM_PF(
        &dvPressureSimObjDescriptor),

    //dvPressureRangeRelative
    CONSTRUCT_STRUCT_DEV_RNGFLT(
        &mapperStaticFrequentStaticDefault.dvPressureRangeRelative,
        NON_VOLATILE,
        &dvPressureRangeRelativeDescriptor),

    //dvPressureRangeRelativeObj
    CONSTRUCT_STRUCT_UOM_PF(
        &dvPressureRangeRelativeObjDescriptor),

    //dvPressureRange
    CONSTRUCT_STRUCT_DEV_RNGFLT(
        &mapperStaticFrequentStaticDefault.dvPressureRange,
        NON_VOLATILE,
        &dvPressureRangeDescriptor),

    //dvPressureRangeObj
    CONSTRUCT_STRUCT_UOM_PF(
        &dvPressureRangeObjDescriptor),

    //dvPressureMaxDN
    CONSTRUCT_SIMPLE_FLOAT(
        &mapperUnprotected.dvPressureMaxDN,
        READONLY_RAM),

    //dvPressureMinDN
    CONSTRUCT_SIMPLE_FLOAT(
        &mapperUnprotected.dvPressureMinDN,
        READONLY_RAM),

    //dvPressureRelativeMaxDN
    CONSTRUCT_SIMPLE_FLOAT(
        &mapperUnprotected.dvPressureRelativeMaxDN,
        READONLY_RAM),

    //dvPressureRelativeMinDN
    CONSTRUCT_SIMPLE_FLOAT(
        &mapperUnprotected.dvPressureRelativeMinDN,
        READONLY_RAM),

    //dvPressureMaxDN_Obj
    CONSTRUCT_STRUCT_UOM_PF(
        &dvPressureMaxDN_ObjDescriptor),

    //dvPressureMinDN_Obj
    CONSTRUCT_STRUCT_UOM_PF(
        &dvPressureMinDN_ObjDescriptor),

    //dvPressureClassification
    CONSTRUCT_SIMPLE_U8(
        &mapperUnprotected.dvPressureClassification,
        READONLY_RAM),

    //dvPressureStatus
    CONSTRUCT_SIMPLE_U8(
        &mapperUnprotected.dvPressureStatus,
        RAM),

    //dvPressurePreset
    CONSTRUCT_FLOAT(
        &mapperStaticFrequentStaticDefault.dvPressurePreset,
        NON_VOLATILE,
        &sharedTRangeFloatDescriptor11),

    //dvPressurePresetObj
    CONSTRUCT_STRUCT_UOM_PF(
        &dvPressurePresetObjDescriptor),

    //dvPressureAlmRange
    CONSTRUCT_STRUCT_RNGFLT(
        &mapperStaticRare.dvPressureAlmRange,
        NON_VOLATILE,
        &dvPressureAlmRangeDescriptor),

    //dvPressureSrc
    CONSTRUCT_TABENUM8(
        &mapperUnprotected.dvPressureSrc,
        RAM,
        &dvPressureSrcDescriptor),

    //dvDensity
    CONSTRUCT_FLOAT(
        &mapperDynamicDuplicated.dvDensity,
        RAM,
        &sharedTRangeFloatDescriptor12),

    //dvDensityDuplicated
    CONSTRUCT_FLOAT(
        &mapperDynamicDuplicated.dvDensityDuplicated,
        RAM,
        &sharedTRangeFloatDescriptor12),

    //dvDensitySimEnable
    CONSTRUCT_TABENUM8(
        &mapperProtected.dvDensitySimEnable,
        RAM,
        &sharedTRangeE8Descriptor6),

    //dvDensitySim
    CONSTRUCT_FLOAT(
        &mapperUnprotected.dvDensitySim,
        RAM,
        &sharedTRangeFloatDescriptor12),

    //dvDensityUnitCode
    CONSTRUCT_TABENUM8(
        &mapperStaticFrequentStaticDefault.dvDensityUnitCode,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor8),

    //dvDensityUnitType
    CONSTRUCT_CONST_U8(
        &mapperConstant.dvDensityUnitType),

    //dvDensitySimUnitCode
    CONSTRUCT_TABENUM8(
        &mapperStaticRare.dvDensitySimUnitCode,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor8),

    //dvDensityObj
    CONSTRUCT_STRUCT_UOM_PF(
        &dvDensityObjDescriptor),

    //dvDensitySimObj
    CONSTRUCT_STRUCT_UOM_PF(
        &dvDensitySimObjDescriptor),

    //dvDensityRange
    CONSTRUCT_STRUCT_DEV_RNGFLT(
        &mapperStaticRare.dvDensityRange,
        NON_VOLATILE,
        &dvDensityRangeDescriptor),

    //dvDensityRangeObj
    CONSTRUCT_STRUCT_UOM_PF(
        &dvDensityRangeObjDescriptor),

    //dvDensityMaxDN
    CONSTRUCT_SIMPLE_FLOAT(
        &mapperUnprotected.dvDensityMaxDN,
        READONLY_RAM),

    //dvDensityMinDN
    CONSTRUCT_SIMPLE_FLOAT(
        &mapperUnprotected.dvDensityMinDN,
        READONLY_RAM),

    //dvDensityMaxDN_Obj
    CONSTRUCT_STRUCT_UOM_PF(
        &dvDensityMaxDN_ObjDescriptor),

    //dvDensityMinDN_Obj
    CONSTRUCT_STRUCT_UOM_PF(
        &dvDensityMinDN_ObjDescriptor),

    //dvDensityClassification
    CONSTRUCT_SIMPLE_U8(
        &mapperUnprotected.dvDensityClassification,
        READONLY_RAM),

    //dvDensityStatus
    CONSTRUCT_SIMPLE_U8(
        &mapperUnprotected.dvDensityStatus,
        RAM),

    //dvDensityPreset
    CONSTRUCT_FLOAT(
        &mapperStaticFrequentStaticDefault.dvDensityPreset,
        NON_VOLATILE,
        &dvDensityPresetDescriptor),

    //dvDensityPresetObj
    CONSTRUCT_STRUCT_UOM_PF(
        &dvDensityPresetObjDescriptor),

    //dvDensitySrc
    CONSTRUCT_TABENUM8(
        &mapperUnprotected.dvDensitySrc,
        RAM,
        &dvDensitySrcDescriptor),

    //dvContent
    CONSTRUCT_FLOAT(
        &mapperDynamicDuplicated.dvContent,
        RAM,
        &sharedTRangeFloatDescriptor5),

    //dvContentDuplicated
    CONSTRUCT_FLOAT(
        &mapperDynamicDuplicated.dvContentDuplicated,
        RAM,
        &sharedTRangeFloatDescriptor13),

    //dvContentSimEnable
    CONSTRUCT_TABENUM8(
        &mapperProtected.dvContentSimEnable,
        RAM,
        &sharedTRangeE8Descriptor6),

    //dvContentUnitCode
    CONSTRUCT_ARRAY_SIMPLE_U8(
        mapperStaticRare.dvContentUnitCode,
        2,
        NON_VOLATILE),

    //dvContentSim
    CONSTRUCT_FLOAT(
        &mapperUnprotected.dvContentSim,
        RAM,
        &sharedTRangeFloatDescriptor13),

    //dvContentRange
    CONSTRUCT_STRUCT_DEV_RNGFLT(
        &mapperStaticFrequentStaticDefault.dvContentRange,
        NON_VOLATILE,
        &dvContentRangeDescriptor),

    //dvContentMaxDN
    CONSTRUCT_SIMPLE_FLOAT(
        &mapperUnprotected.dvContentMaxDN,
        READONLY_RAM),

    //dvContentMinDN
    CONSTRUCT_SIMPLE_FLOAT(
        &mapperUnprotected.dvContentMinDN,
        READONLY_RAM),

    //dvContentClassification
    CONSTRUCT_SIMPLE_U8(
        &mapperUnprotected.dvContentClassification,
        READONLY_RAM),

    //dvContentStatus
    CONSTRUCT_SIMPLE_U8(
        &mapperUnprotected.dvContentStatus,
        RAM),

    //dvContentPreset
    CONSTRUCT_FLOAT(
        &mapperStaticFrequentStaticDefault.dvContentPreset,
        NON_VOLATILE,
        &sharedTRangeFloatDescriptor13),

    //dvContentSrc
    CONSTRUCT_TABENUM8(
        &mapperUnprotected.dvContentSrc,
        RAM,
        &dvContentSrcDescriptor),

    //dvDummyPercentage
    CONSTRUCT_SIMPLE_FLOAT(
        &mapperUnprotected.dvDummyPercentage,
        RAM),

    //dvDummyDampingTime
    CONSTRUCT_SIMPLE_FLOAT(
        &mapperUnprotected.dvDummyDampingTime,
        RAM),

    //dvDummyUnitCode
    CONSTRUCT_SIMPLE_U8(
        &mapperUnprotected.dvDummyUnitCode,
        RAM),

    //dvDummyUnitType
    CONSTRUCT_SIMPLE_U8(
        &mapperUnprotected.dvDummyUnitType,
        RAM),

    //localOperatingModeSelection
    CONSTRUCT_TABENUM16(
        &mapperProtected.localOperatingModeSelection,
        RAM,
        &localOperatingModeSelectionDescriptor),

    //mapperAlarmSimulation
    CONSTRUCT_ALARM(
        &mapperUnprotected.mapperAlarmSimulation,
        sizeof(T_DIAGNOSIS_ALARM),
        RAM,
        &mapperAlarmSimulationDescriptor),

    //mapperDiagnosis
    CONSTRUCT_SIMPLE_U8(
        &mapperUnprotected.mapperDiagnosis,
        RAM),

    //presetChanged
    CONSTRUCT_TABENUM8(
        &mapperProtected.presetChanged,
        RAM,
        &presetChangedDescriptor),

    //diagnosisCounter
    CONSTRUCT_SIMPLE_U32(
        &mapperUnprotected.diagnosisCounter,
        RAM),

    //protectedDataStatus
    CONSTRUCT_TABENUM8(
        &mapperProtected.protectedDataStatus,
        RAM,
        &protectedDataStatusDescriptor),

    //protectedDataCrc
    CONSTRUCT_SIMPLE_U16(
        &mapperUnprotected.protectedDataCrc,
        RAM),

    //hartBurstInAlarmPeriod
    CONSTRUCT_UINT32(
        &mapperUnprotected.hartBurstInAlarmPeriod,
        RAM,
        &hartBurstInAlarmPeriodDescriptor),

    //dvPressureAlmRangeObj
    CONSTRUCT_STRUCT_UOM_PF(
        &dvPressureAlmRangeObjDescriptor),

    //dvTExtUserUnit
    CONSTRUCT_SIMPLE_FLOAT(
        &mapperUnprotected.dvTExtUserUnit,
        RAM),

    //dvPressureUserUnit
    CONSTRUCT_SIMPLE_FLOAT(
        &mapperUnprotected.dvPressureUserUnit,
        RAM),

    //dvDensityUserUnit
    CONSTRUCT_SIMPLE_FLOAT(
        &mapperUnprotected.dvDensityUserUnit,
        RAM),

    //diagAlarmNamurConfiguration
    CONSTRUCT_ARRAY_TABENUM8(
        mapperUnprotected.diagAlarmNamurConfiguration,
        48,
        RAM,
        &diagAlarmNamurConfigurationDescriptor)
    //@SubGen end@    <OBJECTS>
    //lint +e605 warning on for the rest of the code
};



//-------------------------------------------------------------------------------------------------
//! define the MAPPER-Class object
SLOW const T_MAPPER mapper =
{
    {
        &mapperState,
        &mapperDataSemaphore,

        mapperObjectList,
        //lint -e{778}
        sizeof(mapperObjectList)/sizeof(mapperObjectList[0])-1,

        mapperDataClassList,
        sizeof(mapperDataClassList)/sizeof(mapperDataClassList[0]),

        ChangeState_T_UNIT,
        GetState_T_UNIT,
        Initialize_MAPPER, // will be overloaded
        LoadRomDefaults_T_UNIT,
        ResetToDefault_T_UNIT,
        SaveAsDefault_T_UNIT,
        Get_T_MAPPER,  //over load get
        Put_T_MAPPER,  //over load put
        Check_T_UNIT,
        GetAttributeDescription_T_UNIT,
        GetObjectDescription_T_UNIT
    }

    //subsystem EXE and SRV-methods
    //@SubGen start@    <METHODS>
    ,GetPVPercentageForDOSRV_MAPPER
    ,SetSelection_MAPPER
    ,CheckDataCrc_MAPPER
    ,UpdateDeviceVariablesEXE_MAPPER
    ,UpdatePVSVAssignment_MAPPER
    ,UpdateDiagnosis_MAPPER
    ,GetDummyPercentageSRV_MAPPER
    //@SubGen end@    <METHODS>
};




/*

This section defines templates for the SubSystem CodeGenerator.
Do not modify or remove this section

see also: mapper_attributes.h


//special templates for specific standard storage classes:
//@SubGen template start@<DATACLASS CONSTANT>
//-------------------------------------------------------------------------------------------------
//! definition of constant block
const SLOW T_MAPPER_TEMPLATE mapperTemplate=
{
    //@SubGen start@    <INITIALIZER TEMPLATE>
    //@SubGen end@    <INITIALIZER TEMPLATE>
};
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS CONSTANT>



//@SubGen template start@<DATACLASS DYNAMIC>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
FAST T_MAPPER_TEMPLATE mapperTemplate;
const SLOW T_MAPPER_TEMPLATE mapperTemplateInitializer=
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
  #pragma bss_seg(".AAA$MAPPER_DATA")
    const FAST T_MAPPER_TEMPLATE mapperTemplate;
    const FAST T_MAPPER_TEMPLATE mapperTemplateDefault;
  #pragma bss_seg()
#else
  #pragma location="MAPPER_TEMPLATE"
    const FAST __no_init T_MAPPER_TEMPLATE mapperTemplate;
  #pragma location="MAPPER_TEMPLATE_DEFAULT"
    const FAST __no_init T_MAPPER_TEMPLATE mapperTemplateDefault;
#endif
const SLOW T_MAPPER_TEMPLATE mapperTemplateInitializer=
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
  #pragma bss_seg(".AAA$MAPPER_DATA")
    const FAST T_MAPPER_TEMPLATE mapperTemplate;
  #pragma bss_seg()
#else
  #pragma location="MAPPER_TEMPLATE"
    const FAST __no_init T_MAPPER_TEMPLATE mapperTemplate;
#endif
const SLOW T_MAPPER_TEMPLATE mapperTemplateInitializer=
{
    //@SubGen start@    <INITIALIZER TEMPLATE>
    //@SubGen end@    <INITIALIZER TEMPLATE>
};
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS>

//@SubGen template start@ <DATACLASSNAME>
mapperTemplate
//@SubGen template end@ <DATACLASSNAME>


//@SubGen template start@ <DATACLASSLIST DYNAMIC>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_DYNAMIC(mapperTemplate,mapperTemplateInitializer)
//@SubGen template end@ <DATACLASSLIST DYNAMIC>

//@SubGen template start@ <DATACLASSLIST CONSTANT>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_CONSTANT(mapperTemplate)
//@SubGen template end@ <DATACLASSLIST CONSTANT>

//@SubGen template start@ <DATACLASSLIST>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_STATIC_NODEF(mapperTemplate,mapperTemplateInitializer)
//@SubGen template end@ <DATACLASSLIST>

//@SubGen template start@ <DATACLASSLIST STATIC_FREQUENT_CONSTANT_DEFAULT>
//lint -e{708} initialization is OK
CONSTRUCTOR_DC_STATIC_CONSTDEF(mapperTemplate,mapperTemplateInitializer)
//@SubGen template end@ <DATACLASSLIST STATIC_FREQUENT_CONSTANT_DEFAULT>

//@SubGen template start@ <DATACLASSLIST STATIC_FREQUENT_STATIC_DEFAULT>
//lint -e{708} initialization is OK
CONSTRUCTOR_DC_STATIC_STATDEF(mapperTemplate,mapperTemplateInitializer,mapperTemplateDefault)
//@SubGen template end@ <DATACLASSLIST STATIC_FREQUENT_STATIC_DEFAULT>

*/

