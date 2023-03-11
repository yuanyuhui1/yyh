//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem MVMeasurement
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
#include "T_DATA_OBJ/Interface/t_data_obj_uom_pf.h"
#include "T_DATA_OBJ/Interface/t_data_obj_float.h"
#include "T_DATA_OBJ/Interface/t_data_obj_tabenum8.h"
#include "T_DATA_OBJ/Interface/t_data_obj_simple.h"
#include "T_DATA_OBJ/Interface/t_data_obj_rngflt.h"
#include "T_DATA_OBJ/Interface/t_data_obj_const.h"
#include "T_DATA_OBJ/Interface/t_data_obj_dev_rngflt.h"

#include "T_Unit/Interface/t_unit.h"
#include "T_Unit/Interface/T_DATACLASSES.H"
#include "MVMeasurement/Interface/MVMeasurement.h"
//lint -e{91}
#include "MVMeasurement/Interface/MVMeasurement_idx.h"

#include "mVMeasurement_types.h"
#include "mVMeasurement_attributes.h"
#include "mVMeasurement_overload.h"
#include "mVMeasurement_execute.h"

#include "Diagnosis/interface/t_data_obj_alarm.h"
#include "SVMeasurement/Interface/SVMeasurement.h"
#include "SVMeasurement/Interface/SVMeasurement_idx.h"
#include "SVMeasurement/Interface/t_data_obj_dyn_float.h"

#include "Mapper/Interface/Mapper_idx.h"
#include "Coordinator/Interface/Coordinator_idx.h"
//-------------------------------------------------------------------------------------------------
//! unit global attributes
FAST TUSIGN16 mVMeasurementState;     // Subsystem state
FAST T_DATA_SEMAPHORE mVMeasurementDataSemaphore;

//lint -e{85} mVMeasurementObjectList declaration.
const SLOW T_DATA_OBJ mVMeasurementObjectList[];
extern const SLOW T_DATA_OBJ mapperObjectList[];
extern const SLOW T_DATA_OBJ coordinatorObjectList[];
extern const SLOW T_DATA_OBJ sVMeasurementObjectList[];

const SLOW T_DATACLASS mVMeasurementDataClassList[]=
{
    //lint -e{651} Initializers are OK, the warning pop up with the first 708
    //@SubGen start@    <DATACLASSLIST>
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_DYNAMIC(mVMeasurementProtected,mVMeasurementProtectedInitializer),
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_DYNAMIC(mVMeasurementUnprotected,mVMeasurementUnprotectedInitializer),
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_DYNAMIC(mVMeasurementDynamicDuplicated,mVMeasurementDynamicDuplicatedInitializer),
    //lint -e{708} initialization is OK
    CONSTRUCTOR_DC_STATIC_STATDEF(mVMeasurementREPLACE_STATIC_FREQUENT_STATIC_DEFAULT,mVMeasurementREPLACE_STATIC_FREQUENT_STATIC_DEFAULTInitializer,mVMeasurementREPLACE_STATIC_FREQUENT_STATIC_DEFAULTDefault),
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_CONSTANT(mVMeasurementConstant),
    //lint -e{708} union initialization is OK
    CONSTRUCTOR_DC_STATIC_NODEF(mVMeasurementStaticRare,mVMeasurementStaticRareInitializer),
    //lint -e{708} initialization is OK
    CONSTRUCTOR_DC_STATIC_STATDEF(mVMeasurementStaticFrequentStaticDefault,mVMeasurementStaticFrequentStaticDefaultInitializer,mVMeasurementStaticFrequentStaticDefaultDefault)
    //@SubGen end@    <DATACLASSLIST>
};

//@SubGen put@<DATACLASS>
//-------------------------------------------------------------------------------------------------
//! definition of REPLACE_STATIC_FREQUENT_STATIC_DEFAULT data
#ifdef WIN32
  #pragma bss_seg(".AAA$MVMEASUREMENT_DATA")
    const FAST T_MVMEASUREMENT_REPLACE_STATIC_FREQUENT_STATIC_DEFAULT mVMeasurementREPLACE_STATIC_FREQUENT_STATIC_DEFAULT;
    const FAST T_MVMEASUREMENT_REPLACE_STATIC_FREQUENT_STATIC_DEFAULT mVMeasurementREPLACE_STATIC_FREQUENT_STATIC_DEFAULTDefault;
  #pragma bss_seg()
#else
  #pragma location="MVMEASUREMENT_REPLACE_STATIC_FREQUENT_STATIC_DEFAULT"
    const FAST __no_init T_MVMEASUREMENT_REPLACE_STATIC_FREQUENT_STATIC_DEFAULT mVMeasurementREPLACE_STATIC_FREQUENT_STATIC_DEFAULT;
  #pragma location="MVMEASUREMENT_REPLACE_STATIC_FREQUENT_STATIC_DEFAULT_DEFAULT"
    const FAST __no_init T_MVMEASUREMENT_REPLACE_STATIC_FREQUENT_STATIC_DEFAULT mVMeasurementREPLACE_STATIC_FREQUENT_STATIC_DEFAULTDefault;
#endif
const SLOW T_MVMEASUREMENT_REPLACE_STATIC_FREQUENT_STATIC_DEFAULT mVMeasurementREPLACE_STATIC_FREQUENT_STATIC_DEFAULTInitializer=
{
    //@SubGen start@    <INITIALIZER REPLACE_STATIC_FREQUENT_STATIC_DEFAULT>
    MVM_STEAMTYPE_SATURATED,    // TABENUM8 steamType
    MVM_WITHOUT_CORRECTION,    // TABENUM8 liquidCorrection
    0.025,    // FLOAT densityExpandBeta2
    0.325f,    // FLOAT volumeExpandBeta1
    36.0    // FLOAT calorificEnergy
    //@SubGen end@    <INITIALIZER REPLACE_STATIC_FREQUENT_STATIC_DEFAULT>
};
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//! definition of StaticFrequentStaticDefault data
#ifdef WIN32
  #pragma bss_seg(".AAA$MVMEASUREMENT_DATA")
    const FAST T_MVMEASUREMENT_STATIC_FREQUENT_STATIC_DEFAULT mVMeasurementStaticFrequentStaticDefault;
    const FAST T_MVMEASUREMENT_STATIC_FREQUENT_STATIC_DEFAULT mVMeasurementStaticFrequentStaticDefaultDefault;
  #pragma bss_seg()
#else
  #pragma location="MVMEASUREMENT_STATIC_FREQUENT_STATIC_DEFAULT"
    const FAST __no_init T_MVMEASUREMENT_STATIC_FREQUENT_STATIC_DEFAULT mVMeasurementStaticFrequentStaticDefault;
  #pragma location="MVMEASUREMENT_STATIC_FREQUENT_STATIC_DEFAULT_DEFAULT"
    const FAST __no_init T_MVMEASUREMENT_STATIC_FREQUENT_STATIC_DEFAULT mVMeasurementStaticFrequentStaticDefaultDefault;
#endif
const SLOW T_MVMEASUREMENT_STATIC_FREQUENT_STATIC_DEFAULT mVMeasurementStaticFrequentStaticDefaultInitializer=
{
    //@SubGen start@    <INITIALIZER STATIC_FREQUENT_STATIC_DEFAULT>
    SVM_CUBIC_METER_PER_HOUR,    // TABENUM8 QnUnitCode
    {80.0f,0.00f,80.00f},    // DEV_RNGFLT QnRange {hiLim,loLim,span}
    1.00f,    // FLOAT QnDampingTime
    MVM_KILOGRAM_PER_HOUR,    // TABENUM8 QmUnitCode
    {80000.0f,0.00f,80000.0f},    // DEV_RNGFLT QmRange {hiLim,loLim,span}
    1.00f,    // FLOAT QmDampingTime
    MVM_KILOWATT,    // TABENUM8 QpUnitCode
    {960000.00f,0.000f,960000.00f},    // DEV_RNGFLT QpRange {hiLim,loLim,span}
    1.00f,    // FLOAT QpDampingTime
    {80.0f,0.0f,80.0f},    // DEV_RNGFLT QnPartialRange {hiLim,loLim,span}
    1.00f,    // FLOAT QnPartialDampingTime
    {80.0f,0.0f,80.0f},    // DEV_RNGFLT QvPartialRange {hiLim,loLim,span}
    1.00f,    // FLOAT QvPartialDampingTime
    1.0f,    // FLOAT heatCapacity
    998,    // FLOAT densityRef
    MVM_UK,    // TABENUM8 gasRef
    MVM_IDEAL_GAS,    // TABENUM8 gasComputeType
    MVM_ACTUAL_DENSITY,    // TABENUM8 gasDensitySelection
    20.00f    // FLOAT refTemperature
    //@SubGen end@    <INITIALIZER STATIC_FREQUENT_STATIC_DEFAULT>
};
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//! definition of constant block
const SLOW T_MVMEASUREMENT_CONSTANT mVMeasurementConstant=
{
    //@SubGen start@    <INITIALIZER CONSTANT>
    MVM_T_VOLUME_FLOW,    // CONST_U8 QnUnitType
    MVM_T_MASS_FLOW,    // CONST_U8 QmUnitType
    MVM_T_POWER    // CONST_U8 QpUnitType
    //@SubGen end@    <INITIALIZER CONSTANT>
};
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
FAST T_MVMEASUREMENT_PROTECTED mVMeasurementProtected;
const SLOW T_MVMEASUREMENT_PROTECTED mVMeasurementProtectedInitializer=
{
    //@SubGen start@    <INITIALIZER PROTECTED>
    MVM_DISABLE,    // TABENUM8 QnSimEnable
    MVM_DISABLE,    // TABENUM8 QmSimEnable
    MVM_DISABLE,    // TABENUM8 QpSimEnable
    MVM_DISABLE,    // TABENUM8 QnPartialSimEnable
    MVM_DISABLE,    // TABENUM8 QvPartialSimEnable
    MVM_GONFIGING,    // TABENUM8 spline2dRequest
    MVM_GONFIGING,    // TABENUM8 updateCFSRequest
    MVM_GONFIGING,    // TABENUM8 updateCFPresetRequest
    1,    // SIMPLE_BIT8 updateMaxDNRequest
    MVM_CALCULATE    // TABENUM8 protectedDataStatus
    //@SubGen end@    <INITIALIZER PROTECTED>
};
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
FAST T_MVMEASUREMENT_UNPROTECTED mVMeasurementUnprotected;
const SLOW T_MVMEASUREMENT_UNPROTECTED mVMeasurementUnprotectedInitializer=
{
    //@SubGen start@    <INITIALIZER UNPROTECTED>
    0.00f,    // DNY_FLOAT QnSim
    SVM_CUBIC_METER_PER_HOUR,    // TABENUM8 QnSimUnitCode
    0.0f,    // SIMPLE_FLOAT QMinDN
    0.00f,    // FLOAT QnPerSim
    MVM_T_VOLUME_FLOW,    // SIMPLE_U8 QnClassification
    0xC0,    // SIMPLE_U8 QnStatus
    0.00f,    // DNY_FLOAT QmSim
    MVM_KILOGRAM_PER_HOUR,    // TABENUM8 QmSimUnitCode
    0.00f,    // FLOAT QmPerSim
    MVM_T_MASS_FLOW,    // SIMPLE_U8 QmClassification
    0xC0,    // SIMPLE_U8 QmStatus
    0.00f,    // DNY_FLOAT QpSim
    MVM_KILOWATT,    // TABENUM8 QpSimUnitCode
    0.00f,    // FLOAT QpPerSim
    MVM_T_POWER,    // SIMPLE_U8 QpClassification
    0xC0,    // SIMPLE_U8 QpStatus
    0.00f,    // DNY_FLOAT QnPartialSim
    0.00f,    // FLOAT QnPartialPerSim
    MVM_T_VOLUME_FLOW,    // SIMPLE_U8 QnPartialClassification
    0xC0,    // SIMPLE_U8 QnPartialStatus
    0.00f,    // DNY_FLOAT QvPartialSim
    0.00f,    // FLOAT QvPartialPerSim
    MVM_T_VOLUME_FLOW,    // SIMPLE_U8 QvPartialClassification
    0xC0,    // SIMPLE_U8 QvPartialStatus
    MVM_SATURATED_STEAM,    // TABENUM8 steamStatus
    1.0f,    // SIMPLE_FLOAT HwPreset
    0.0f,    // SIMPLE_FLOAT HcPreset
    0    // SIMPLE_U16 protectedDataCrc
    //@SubGen end@    <INITIALIZER UNPROTECTED>
};
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
FAST T_MVMEASUREMENT_DYNAMIC_DUPLICATED mVMeasurementDynamicDuplicated;
const SLOW T_MVMEASUREMENT_DYNAMIC_DUPLICATED mVMeasurementDynamicDuplicatedInitializer=
{
    //@SubGen start@    <INITIALIZER DYNAMIC_DUPLICATED>
    0.00f,    // SIMPLE_FLOAT Qn
    0.00f,    // SIMPLE_FLOAT QnDamped
    30000.00f,    // SIMPLE_FLOAT QnMaxDN
    30000.00f,    // SIMPLE_FLOAT QnMaxDNDuplicated
    0.00f,    // SIMPLE_FLOAT QnPercentage
    0.00f,    // SIMPLE_FLOAT Qm
    0.00f,    // SIMPLE_FLOAT QmDamped
    100000.00f,    // SIMPLE_FLOAT QmMaxDN
    100000.00f,    // SIMPLE_FLOAT QmMaxDNDuplicated
    0.00f,    // SIMPLE_FLOAT QmPercentage
    0.00f,    // SIMPLE_FLOAT Qp
    0.00f,    // SIMPLE_FLOAT QpDamped
    3200000.00f,    // SIMPLE_FLOAT QpMaxDN
    3200000.00f,    // SIMPLE_FLOAT QpMaxDNDuplicated
    0.00f,    // SIMPLE_FLOAT QpPercentage
    0.00f,    // SIMPLE_FLOAT QnPartial
    0.00f,    // SIMPLE_FLOAT QnPartialDamped
    30000.00f,    // SIMPLE_FLOAT QnPartialMaxDN
    30000.00f,    // SIMPLE_FLOAT QnPartialMaxDNDuplicated
    0.00f,    // SIMPLE_FLOAT QnPartialPercentage
    0.00f,    // SIMPLE_FLOAT QvPartial
    0.00f,    // SIMPLE_FLOAT QvPartialDamped
    80.00f,    // SIMPLE_FLOAT QvPartialMaxDN
    80.00f,    // SIMPLE_FLOAT QvPartialMaxDNDuplicated
    0.00f,    // SIMPLE_FLOAT QvPartialPercentage
    1.0f,    // SIMPLE_FLOAT compressFactorR
    0,    // T_DIAGNOSIS_ALARM mvmAlarmSimulation
    0,    // SIMPLE_U8 mvmDiagnosis
    1.0f,    // SIMPLE_FLOAT compressFactorS
    1.0f,    // SIMPLE_FLOAT Hw
    0.0f,    // SIMPLE_FLOAT Hc
    0.00f,    // SIMPLE_FLOAT QnPartialDuplicated
    0.00f,    // SIMPLE_FLOAT QnPartialPercentageDuplicated
    0.00f,    // SIMPLE_FLOAT QnPartialDampedDuplicated
    0.00f,    // SIMPLE_FLOAT QvPartialDuplicated
    0.00f,    // SIMPLE_FLOAT QvPartialPercentageDuplicated
    0.00f,    // SIMPLE_FLOAT QvPartialDampedDuplicated
    0.00f,    // SIMPLE_FLOAT QnDuplicated
    0.00f,    // SIMPLE_FLOAT QnPercentageDuplicated
    0.00f,    // SIMPLE_FLOAT QnDampedDuplicated
    0.00f,    // SIMPLE_FLOAT QmDuplicated
    0.00f,    // SIMPLE_FLOAT QmPercentageDuplicated
    0.00f,    // SIMPLE_FLOAT QmDampedDuplicated
    0.00f,    // SIMPLE_FLOAT QpDuplicated
    0.00f,    // SIMPLE_FLOAT QpPercentageDuplicated
    0.00f,    // SIMPLE_FLOAT QpDampedDuplicated
    1.00f,    // SIMPLE_FLOAT HwDuplicated
    0.0f,    // SIMPLE_FLOAT HcDuplicated
    1.0f,    // SIMPLE_FLOAT compressFactorRDuplicated
    1.0f,    // SIMPLE_FLOAT compressFactorSDuplicated
    0x00,    // T_DIAGNOSIS_ALARM mvmAlarmSimulationDuplicated
    0x00    // SIMPLE_U8 mvmDiagnosisDuplicated
    //@SubGen end@    <INITIALIZER DYNAMIC_DUPLICATED>
};
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//! definition of StaticRare data
#ifdef WIN32
  #pragma bss_seg(".AAA$MVMEASUREMENT_DATA")
    const FAST T_MVMEASUREMENT_STATIC_RARE mVMeasurementStaticRare;
  #pragma bss_seg()
#else
  #pragma location="MVMEASUREMENT_STATIC_RARE"
    const FAST __no_init T_MVMEASUREMENT_STATIC_RARE mVMeasurementStaticRare;
#endif
const SLOW T_MVMEASUREMENT_STATIC_RARE mVMeasurementStaticRareInitializer=
{
    //@SubGen start@    <INITIALIZER STATIC_RARE>
    {100.0f,0.00f},    // RANGEFLT QAlmRange {hiLim,loLim}
    998,    // FLOAT densityS
    MVM_NONGONFIGED,    // TABENUM8 gasConfFlag
    {0.8120,0.0430,0.0090,0.0015,0.0015,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0760,0.0570,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000},    // FLOAT molFractions[21]
    {1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f},    // FLOAT compressFactorArray[63]
    -10.0f,    // FLOAT CFTempArray0
    0.01f,    // FLOAT CFTempArray1
    0.05f,    // FLOAT CFTempArray2
    20.0f,    // FLOAT CFTempArray3
    21.0f,    // FLOAT CFTempArray4
    22.0f,    // FLOAT CFTempArray5
    60.0f,    // FLOAT CFTempArray6
    10.0f,    // FLOAT CFPressArray0
    100.0f,    // FLOAT CFPressArray1
    200.0f,    // FLOAT CFPressArray2
    1000.0f,    // FLOAT CFPressArray3
    2000.0f,    // FLOAT CFPressArray4
    3000.0f,    // FLOAT CFPressArray5
    4000.0f,    // FLOAT CFPressArray6
    5000.0f,    // FLOAT CFPressArray7
    6000.0f,    // FLOAT CFPressArray8
    0.686,    // FLOAT relativeDensity
    MVM_RELATIVE_DENSITY_REF_DEG0_101325,    // TABENUM8 relativeDensityRef
    36.64,    // FLOAT refCalorifcValue
    MVM_CALORIFIC_REF_DEG25_DEG0_101325,    // TABENUM8 calorificValueRef
    101.132f,    // FLOAT refPressure
    1.0f,    // SIMPLE_FLOAT compressFactorPreset
    MVM_SIZE9X7,    // TABENUM8 matrixSize
    MVM_FWD_ONLY    // TABENUM8 steamPwrrFR
    //@SubGen end@    <INITIALIZER STATIC_RARE>
};
//-------------------------------------------------------------------------------------------------





//-------------------------------------------------------------------------------------------------
//! object check and PutGet descriptions
//@SubGen start@<DESCRIPTORS>
const SLOW TUSIGN8 _T_VOLUME_FLOW[] =
// Table referenced by: QnUnitCode, QnSimUnitCode, QnSimUnitCode, QmSimUnitCode, QpSimUnitCode
{
  MVM_CUBIC_METER_PER_SECOND,
  MVM_CUBIC_METER_PER_MINUTE,
  MVM_CUBIC_METER_PER_HOUR,
  MVM_CUBIC_METER_PER_DAY,
  MVM_CUBIC_FEET_PER_SECOND,
  MVM_CUBIC_FEET_PER_MINUTE,
  MVM_CUBIC_FEET_PER_HOUR,
  MVM_CUBIC_FEET_PER_DAY,
  MVM_CUBIC_KILOFEET_PER_SECOND,
  MVM_CUBIC_KILOFEET_PER_MINUTE,
  MVM_CUBIC_KILOFEET_PER_HOUR,
  MVM_CUBIC_KILOFEET_PER_DAY,
  MVM_LITER_PER_SECOND,
  MVM_LITER_PER_MINUTE,
  MVM_LITER_PER_HOUR,
  MVM_LITER_PER_DAY,
  MVM_KILOLITER_PER_SECOND,
  MVM_HECTOLITER_PER_SECOND,
  MVM_HECTOLITER_PER_MINUTE,
  MVM_HECTOLITER_PER_HOUR,
  MVM_HECTOLITER_PER_DAY,
  MVM_KILOLITER_PER_MINUTE,
  MVM_KILOLITER_PER_HOUR,
  MVM_KILOLITER_PER_DAY,
  MVM_US_GALLON_PER_SECOND,
  MVM_US_GALLON_PER_MINUTE,
  MVM_US_GALLON_PER_HOUR,
  MVM_US_GALLON_PER_DAY,
  MVM_IMPERIAL_GALLON_PER_SECOND,
  MVM_IMPERIAL_GALLON_PER_MINUTE,
  MVM_IMPERIAL_GALLON_PER_HOUR,
  MVM_IMPERIAL_GALLON_PER_DAY,
  MVM_BARREL_PER_SECOND,
  MVM_BARREL_PER_MINUTE,
  MVM_BARREL_PER_HOUR,
  MVM_BARREL_PER_DAY,
  MVM_BARREL_US_BEER_PER_DAY,
  MVM_BARREL_US_BEER_PER_HOUR,
  MVM_BARREL_US_BEER_PER_MINUTE,
  MVM_BARREL_US_BEER_PER_SECOND
};

const SLOW TUSIGN8 _T_MASS_FLOW[] =
// Table referenced by: QmUnitCode, QmSimUnitCode
{
  MVM_GRAM_PER_SECOND,
  MVM_GRAM_PER_MINUTE,
  MVM_GRAM_PER_HOUR,
  MVM_KILOGRAM_PER_SECOND,
  MVM_KILOGRAM_PER_MINUTE,
  MVM_KILOGRAM_PER_HOUR,
  MVM_KILOGRAM_PER_DAY,
  MVM_METRIC_TON_PER_SECOND,
  MVM_METRIC_TON_PER_MINUTE,
  MVM_METRIC_TON_PER_HOUR,
  MVM_METRIC_TON_PER_DAY,
  MVM_POUND_PER_SECOND,
  MVM_POUND_PER_MINUTE,
  MVM_POUND_PER_HOUR,
  MVM_POUND_PER_DAY,
  MVM_SHORT_TON_PER_SECOND,
  MVM_SHORT_TON_PER_MINUTE,
  MVM_SHORT_TON_PER_HOUR,
  MVM_SHORT_TON_PER_DAY
};

const SLOW TUSIGN8 _T_POWER[] =
// Table referenced by: QpUnitCode, QpSimUnitCode
{
  MVM_WATT,
  MVM_MEGAWATT,
  MVM_KILOWATT,
  MVM_KILOJOULE_PER_SECOND,
  MVM_KILOJOULE_PER_MINUTE,
  MVM_KILOJOULE_PER_HOUR,
  MVM_KILOJOULE_PER_DAY,
  MVM_MEGAJOULE_PER_HOUR,
  MVM_BRITISH_THERMAL_UNIT_PER_SECOND,
  MVM_BRITISH_THERMAL_UNIT_PER_MINUTE,
  MVM_BRITISH_THERMAL_UNIT_PER_HOUR,
  MVM_BRITISH_THERMAL_UNIT_PER_DAY,
  MVM_THOUSAND_BRITISH_THERMAL_UNIT_PER_MINUTE,
  MVM_THOUSAND_BRITISH_THERMAL_UNIT_PER_HOUR,
  MVM_THOUSAND_BRITISH_THERMAL_UNIT_PER_DAY,
  MVM_MILLION_BRITISH_THERMAL_UNIT_PER_HOUR,
  MVM_MILLION_BRITISH_THERMAL_UNIT_PER_DAY
};

const SLOW TUSIGN8 _STEAMSTATUS[] =
// Table referenced by: steamStatus
{
  MVM_WATER,
  MVM_SATURATED_STEAM,
  MVM_OVERHEAT_STEAM
};

const SLOW TUSIGN8 _STEAMTYPE[] =
// Table referenced by: steamType
{
  MVM_STEAMTYPE_OVERHEAT,
  MVM_STEAMTYPE_SATURATED,
  MVM_STEAMTYPE_WATER
};

const SLOW TUSIGN8 _LIQUIDCORRECTION[] =
// Table referenced by: liquidCorrection
{
  MVM_DENSITY_CORRECTION,
  MVM_VOLUME_CORRECTION,
  MVM_WITHOUT_CORRECTION
};

const SLOW TUSIGN8 _GAS_REF_CONDITIONS[] =
// Table referenced by: gasRef
{
  MVM_UK,
  MVM_US,
  MVM_DEG0,
  MVM_DEG20
};

const SLOW TUSIGN8 _GAS_COMPUTATION_TYPE[] =
// Table referenced by: gasComputeType
{
  MVM_IDEAL_GAS,
  MVM_ISO12213_MOLAR,
  MVM_ISO12213_PHISICAL_PROPERTY
};

const SLOW TUSIGN8 _GAS_MASS_COMPUTATION[] =
// Table referenced by: gasDensitySelection
{
  MVM_REFERENCY_DENSITY,
  MVM_ACTUAL_DENSITY
};

const SLOW TUSIGN8 _MVM_DIAGNOSIS_ALARM_TABLE[] =
// Table referenced by: mvmAlarmSimulation, mvmAlarmSimulationDuplicated
{
  MVM_ALM_FLOWRATE_REACH_QMAX,
  MVM_ALM_FLOWRATE_REACH_QMIN,
  MVM_ALM_FLOWRATE_GT_103,
  MVM_ALM_STEAM_STATUS_MISMATCH,
  MVM_ALM_LOW_Q_VALUE_CUTOFF
};

const SLOW TUSIGN8 _RELATIVE_DENSITY_REF[] =
// Table referenced by: relativeDensityRef
{
  MVM_RELATIVE_DENSITY_REF_DEG0_101325,
  MVM_RELATIVE_DENSITY_REF_DEG15_101325,
  MVM_RELATIVE_DENSITY_REF__60F_101592,
  MVM_RELATIVE_DENSITY_REF__60F_101560
};

const SLOW TUSIGN8 _CALORIFIC_REF[] =
// Table referenced by: calorificValueRef
{
  MVM_CALORIFIC_REF_DEG25_DEG0_101325,
  MVM_CALORIFIC_REF_DEG0_DEG0_101325,
  MVM_CALORIFIC_REF_DEG15_DEG15_101325,
  MVM_CALORIFIC_REF_60F_6OF_101592,
  MVM_CALORIFIC_REF_60F_6OF_101560
};

const SLOW TUSIGN8 _MVM_STATUS_TABLE[] =
// Table referenced by: protectedDataStatus
{
  MVM_UNPROTECTED,
  MVM_CALCULATE,
  MVM_PROTECTED
};

const SLOW TUSIGN8 _MVM_SW[] =
// Table referenced by: QnSimEnable, QmSimEnable, QpSimEnable, QnPartialSimEnable, QnPartialSimEnable, QvPartialSimEnable, QvPartialSimEnable
{
  MVM_ENABLE,
  MVM_DISABLE
};

const SLOW TUSIGN8 _GAS_CFG[] =
// Table referenced by: gasConfFlag, spline2dRequest, updateCFSRequest, updateCFPresetRequest
{
  MVM_NONGONFIGED,
  MVM_GONFIGING,
  MVM_GONFIGED
};

const SLOW TUSIGN8 _MATRIX_TABLE[] =
// Table referenced by: matrixSize
{
  MVM_SIZE3X3,
  MVM_SIZE4X3,
  MVM_SIZE9X7
};

const SLOW TUSIGN8 _PWR_FWD_REV_SW[] =
// Table referenced by: steamPwrrFR
{
  MVM_FWD_ONLY,
  MVM_FWD_REV
};

static const SLOW T_RANGE_FLOAT densityRefDescriptor =
{
  0.01f,  // TFLOAT min
  3000.00f,  // TFLOAT max
  0.01f  // TFLOAT increment
};

static const SLOW T_RANGE_FLOAT densitySDescriptor =
{
  0.01f,  // TFLOAT min
  3000.0f,  // TFLOAT max
  0.01f  // TFLOAT increment
};

static const SLOW T_RANGE_FLOAT densityExpandBeta2Descriptor =
{
  -100.00f,  // TFLOAT min
  100.00f,  // TFLOAT max
  0.001  // TFLOAT increment
};

static const SLOW T_RANGE_FLOAT volumeExpandBeta1Descriptor =
{
  0.00f,  // TFLOAT min
  1000.00f,  // TFLOAT max
  0.001f  // TFLOAT increment
};

static const SLOW T_RANGE_E8 steamStatusDescriptor =
{
  _STEAMSTATUS,  // TUSIGN8* ptrCodeTab
  sizeof(_STEAMSTATUS)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 steamTypeDescriptor =
{
  _STEAMTYPE,  // TUSIGN8* ptrCodeTab
  sizeof(_STEAMTYPE)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 liquidCorrectionDescriptor =
{
  _LIQUIDCORRECTION,  // TUSIGN8* ptrCodeTab
  sizeof(_LIQUIDCORRECTION)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_FLOAT heatCapacityDescriptor =
{
  0.0001f,  // TFLOAT min
  10.00f,  // TFLOAT max
  0.001f  // TFLOAT increment
};

static const SLOW T_RANGE_E8 gasRefDescriptor =
{
  _GAS_REF_CONDITIONS,  // TUSIGN8* ptrCodeTab
  sizeof(_GAS_REF_CONDITIONS)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 gasComputeTypeDescriptor =
{
  _GAS_COMPUTATION_TYPE,  // TUSIGN8* ptrCodeTab
  sizeof(_GAS_COMPUTATION_TYPE)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_FLOAT calorificEnergyDescriptor =
{
  1.0f,  // TFLOAT min
  300.0f,  // TFLOAT max
  0.01f  // TFLOAT increment
};

static const SLOW T_RANGE_FLOAT molFractionsDescriptor =
{
  0.0000f,  // TFLOAT min
  100.00f,  // TFLOAT max
  0.0001f  // TFLOAT increment
};

static const SLOW T_RANGE_FLOAT relativeDensityDescriptor =
{
  0.5500f,  // TFLOAT min
  0.9000f,  // TFLOAT max
  0.0001f  // TFLOAT increment
};

static const SLOW T_RANGE_FLOAT refCalorifcValueDescriptor =
{
  20.000f,  // TFLOAT min
  48.000f,  // TFLOAT max
  0.001f  // TFLOAT increment
};

 const SLOW T_RANGE_E8 mvmAlarmSimulationDescriptor =
{
  _MVM_DIAGNOSIS_ALARM_TABLE,  // TUSIGN8* ptrCodeTab
  sizeof(_MVM_DIAGNOSIS_ALARM_TABLE)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 relativeDensityRefDescriptor =
{
  _RELATIVE_DENSITY_REF,  // TUSIGN8* ptrCodeTab
  sizeof(_RELATIVE_DENSITY_REF)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 calorificValueRefDescriptor =
{
  _CALORIFIC_REF,  // TUSIGN8* ptrCodeTab
  sizeof(_CALORIFIC_REF)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 sharedTRangeE8Descriptor0 =
// Descriptor referenced by: QnSimEnable, QmSimEnable, QpSimEnable, QnPartialSimEnable, QvPartialSimEnable
{
  _MVM_SW,  // TUSIGN8* ptrCodeTab
  sizeof(_MVM_SW)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_FLOAT sharedTRangeFloatDescriptor1 =
// Descriptor referenced by: QnPerSim, QmPerSim, QpPerSim, QnPartialPerSim, QvPartialPerSim
{
  0.00f,  // TFLOAT min
  120.00f,  // TFLOAT max
  0.01f  // TFLOAT increment
};

static const SLOW T_RANGE_E8 sharedTRangeE8Descriptor2 =
// Descriptor referenced by: QnUnitCode, QnSimUnitCode
{
  _T_VOLUME_FLOW,  // TUSIGN8* ptrCodeTab
  sizeof(_T_VOLUME_FLOW)  // TUSIGN8 numberOfCodes
};

static const SLOW T_UOM_PF_DESCRIPTOR QnObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(QnUnitCode,mVMeasurement, MVM),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(Qn, mVMeasurement, MVM),  // void * pObject_value
  MVM_T_VOLUME_FLOW  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR QnSimObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(QnSimUnitCode, mVMeasurement, MVM),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(QnSim, mVMeasurement, MVM),  // void * pObject_value
  MVM_T_VOLUME_FLOW  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR QnDampedObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(QnUnitCode, mVMeasurement, MVM),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(QnDamped, mVMeasurement, MVM),  // void * pObject_value
  MVM_T_VOLUME_FLOW  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR QnRangeObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(QnUnitCode, mVMeasurement, MVM),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(QnRange, mVMeasurement, MVM),  // void * pObject_value
  MVM_T_VOLUME_FLOW  // TUSIGN8 unitsType
};

static const SLOW T_RANGE_FLOAT sharedTRangeFloatDescriptor3 =
// Descriptor referenced by: QnDampingTime, QmDampingTime, QpDampingTime, QnPartialDampingTime
{
  0.0f,  // TFLOAT min
  100.0f,  // TFLOAT max
  0.1f  // TFLOAT increment
};

static const SLOW T_DNY_FLOAT_DESCRIPTOR QnSimDescriptor =
{
  MVM_T_VOLUME_FLOW,  // TUSIGN8 UnitsType
  NULL,  // void * ptrUnitsCodeObj
  SUBSYS_OBJPTR_IDX_(QnMaxDN, mVMeasurement, MVM),  // void * ptrLinkObj
  1.1,  // TFLOAT MAX
  0.00  // TFLOAT Min
};

static const SLOW T_RANGE_E8 sharedTRangeE8Descriptor4 =
// Descriptor referenced by: QmUnitCode, QmSimUnitCode
{
  _T_MASS_FLOW,  // TUSIGN8* ptrCodeTab
  sizeof(_T_MASS_FLOW)  // TUSIGN8 numberOfCodes
};

static const SLOW T_UOM_PF_DESCRIPTOR QmObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(QmUnitCode,mVMeasurement, MVM),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(Qm, mVMeasurement, MVM),  // void * pObject_value
  MVM_T_MASS_FLOW  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR QmSimObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(QmSimUnitCode, mVMeasurement, MVM),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(QmSim, mVMeasurement, MVM),  // void * pObject_value
  MVM_T_MASS_FLOW  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR QmDampedObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(QmUnitCode, mVMeasurement, MVM),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(QmDamped, mVMeasurement, MVM),  // void * pObject_value
  MVM_T_MASS_FLOW  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR QmRangeObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(QmUnitCode, mVMeasurement, MVM),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(QmRange, mVMeasurement, MVM),  // void * pObject_value
  MVM_T_MASS_FLOW  // TUSIGN8 unitsType
};

static const SLOW T_DNY_FLOAT_DESCRIPTOR QmSimDescriptor =
{
  MVM_T_MASS_FLOW,  // TUSIGN8 UnitsType
  NULL,  // void * ptrUnitsCodeObj
  SUBSYS_OBJPTR_IDX_(QmMaxDN, mVMeasurement, MVM),  // void * ptrLinkObj
  1.15,  // TFLOAT MAX
  0.00  // TFLOAT Min
};

static const SLOW T_RANGE_E8 sharedTRangeE8Descriptor5 =
// Descriptor referenced by: QpUnitCode, QpSimUnitCode
{
  _T_POWER,  // TUSIGN8* ptrCodeTab
  sizeof(_T_POWER)  // TUSIGN8 numberOfCodes
};

static const SLOW T_UOM_PF_DESCRIPTOR QpObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(QpUnitCode,mVMeasurement, MVM),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(Qp, mVMeasurement, MVM),  // void * pObject_value
  MVM_T_POWER  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR QpSimObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(QpSimUnitCode, mVMeasurement, MVM),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(QpSim, mVMeasurement, MVM),  // void * pObject_value
  MVM_T_POWER  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR QpDampedObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(QpUnitCode, mVMeasurement, MVM),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(QpDamped, mVMeasurement, MVM),  // void * pObject_value
  MVM_T_POWER  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR QpRangeObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(QpUnitCode, mVMeasurement, MVM),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(QpRange, mVMeasurement, MVM),  // void * pObject_value
  MVM_T_POWER  // TUSIGN8 unitsType
};

static const SLOW T_DNY_FLOAT_DESCRIPTOR QpSimDescriptor =
{
  MVM_T_POWER,  // TUSIGN8 UnitsType
  NULL,  // void * ptrUnitsCodeObj
  SUBSYS_OBJPTR_IDX_(QpMaxDN, mVMeasurement, MVM),  // void * ptrLinkObj
  1.15,  // TFLOAT MAX
  0.00  // TFLOAT Min
};

static const SLOW T_UOM_PF_DESCRIPTOR QnPartialObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(QnUnitCode,mVMeasurement, MVM),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(QnPartial, mVMeasurement, MVM),  // void * pObject_value
  MVM_T_VOLUME_FLOW  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR QnPartialSimObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(QnSimUnitCode, mVMeasurement, MVM),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(QnPartialSim, mVMeasurement, MVM),  // void * pObject_value
  MVM_T_VOLUME_FLOW  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR QnPartialDampedObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(QnUnitCode, mVMeasurement, MVM),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(QnPartialDamped, mVMeasurement, MVM),  // void * pObject_value
  MVM_T_VOLUME_FLOW  // TUSIGN8 unitsType
};

static const SLOW T_DNY_FLOAT_DESCRIPTOR QnPartialSimDescriptor =
{
  MVM_T_VOLUME_FLOW,  // TUSIGN8 UnitsType
  NULL,  // void * ptrUnitsCodeObj
  SUBSYS_OBJPTR_IDX_(QnPartialMaxDN, mVMeasurement, MVM),  // void * ptrLinkObj
  1.15,  // TFLOAT MAX
  0.00  // TFLOAT Min
};

static const SLOW T_UOM_PF_DESCRIPTOR QnPartialRangeObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(QnUnitCode, mVMeasurement, MVM),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(QnPartialRange, mVMeasurement, MVM),  // void * pObject_value
  MVM_T_VOLUME_FLOW  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR QvPartialObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(QvUnitCode,sVMeasurement, SVM),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(QvPartial, mVMeasurement, MVM),  // void * pObject_value
  MVM_T_VOLUME_FLOW  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR QvPartialSimObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(QvSimUnitCode, sVMeasurement, SVM),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(QvPartialSim, mVMeasurement, MVM),  // void * pObject_value
  MVM_T_VOLUME_FLOW  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR QvPartialDampedObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(QvUnitCode, sVMeasurement, SVM),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(QvPartialDamped, mVMeasurement, MVM),  // void * pObject_value
  MVM_T_VOLUME_FLOW  // TUSIGN8 unitsType
};

static const SLOW T_DNY_FLOAT_DESCRIPTOR QvPartialSimDescriptor =
{
  MVM_T_VOLUME_FLOW,  // TUSIGN8 UnitsType
  NULL,  // void * ptrUnitsCodeObj
  SUBSYS_OBJPTR_IDX_(QvPartialMaxDN, mVMeasurement, MVM),  // void * ptrLinkObj
  1.15,  // TFLOAT MAX
  0.00  // TFLOAT Min
};

static const SLOW T_UOM_PF_DESCRIPTOR QvPartialRangeObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(QvUnitCode, sVMeasurement, SVM),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(QvPartialRange, mVMeasurement, MVM),  // void * pObject_value
  MVM_T_VOLUME_FLOW  // TUSIGN8 unitsType
};

static const SLOW T_RANGE_FLOAT QvPartialDampingTimeDescriptor =
{
  0.00f,  // TFLOAT min
  100.0f,  // TFLOAT max
  0.1f  // TFLOAT increment
};

static const SLOW T_RANGE_E8 protectedDataStatusDescriptor =
{
  _MVM_STATUS_TABLE,  // TUSIGN8* ptrCodeTab
  sizeof(_MVM_STATUS_TABLE)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_E8 mvmAlarmSimulationDuplicatedDescriptor =
{
  _MVM_DIAGNOSIS_ALARM_TABLE,  // TUSIGN8* ptrCodeTab
  MVM_DIAG_SIZE  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_FLOAT refTemperatureDescriptor =
{
  -200.0f,  // TFLOAT min
  500.0f,  // TFLOAT max
  0.01f  // TFLOAT increment
};

static const SLOW T_RANGE_FLOAT refPressureDescriptor =
{
  0.001f,  // TFLOAT min
  40000.00f,  // TFLOAT max
  0.001f  // TFLOAT increment
};

static const SLOW T_RANGE_E8 gasDensitySelectionDescriptor =
{
  _GAS_MASS_COMPUTATION,  // TUSIGN8* ptrCodeTab
  sizeof(_GAS_MASS_COMPUTATION)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGE_FLOAT compressFactorArrayDescriptor =
{
  0.00f,  // TFLOAT min
  1.01f,  // TFLOAT max
  0.001f  // TFLOAT increment
};

static const SLOW T_RANGE_E8 sharedTRangeE8Descriptor6 =
// Descriptor referenced by: gasConfFlag, spline2dRequest, updateCFSRequest, updateCFPresetRequest
{
  _GAS_CFG,  // TUSIGN8* ptrCodeTab
  sizeof(_GAS_CFG)  // TUSIGN8 numberOfCodes
};

static const SLOW T_RANGEFLT_DESCRIPTOR QAlmRangeDescriptor =
{
  105.0f,  // TFLOAT maxHi
  0.00f  // TFLOAT maxLo
};

static const SLOW T_UOM_PF_DESCRIPTOR densigyRefObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(dvDensityUnitCode, mapper, MAPPER),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(densityRef, mVMeasurement, MVM),  // void * pObject_value
  MVM_T_MASS_DENSITY  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR refPressureObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(dvPressureUnitCode, mapper, MAPPER),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(refPressure,mVMeasurement, MVM),  // void * pObject_value
  MVM_T_PRESSURE  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR refTemperatureObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(temperature_UnitCode, coordinator, CDT),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(refTemperature,mVMeasurement, MVM),  // void * pObject_value
  MVM_T_TEMPERATURE  // TUSIGN8 unitsType
};

static const SLOW T_RANGE_FLOAT sharedTRangeFloatDescriptor7 =
// Descriptor referenced by: CFTempArray0, CFTempArray6, CFTempArray5, CFTempArray4, CFTempArray3, CFTempArray2, CFTempArray1
{
  -10.05f,  // TFLOAT min
  64.85f,  // TFLOAT max
  0.01f  // TFLOAT increment
};

static const SLOW T_RANGE_FLOAT sharedTRangeFloatDescriptor8 =
// Descriptor referenced by: CFPressArray0, CFPressArray1, CFPressArray2, CFPressArray3, CFPressArray4, CFPressArray5, CFPressArray6, CFPressArray7, CFPressArray8
{
  0.0f,  // TFLOAT min
  12000.00f,  // TFLOAT max
  0.00f  // TFLOAT increment
};

static const SLOW T_UOM_PF_DESCRIPTOR CFTempArray0ObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(temperature_UnitCode, coordinator, CDT),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(CFTempArray0,mVMeasurement, MVM),  // void * pObject_value
  MVM_T_TEMPERATURE  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR CFTempArray1ObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(temperature_UnitCode, coordinator, CDT),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(CFTempArray1,mVMeasurement, MVM),  // void * pObject_value
  MVM_T_TEMPERATURE  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR CFTempArray2ObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(temperature_UnitCode, coordinator, CDT),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(CFTempArray2,mVMeasurement, MVM),  // void * pObject_value
  MVM_T_TEMPERATURE  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR CFTempArray3ObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(temperature_UnitCode, coordinator, CDT),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(CFTempArray3,mVMeasurement, MVM),  // void * pObject_value
  MVM_T_TEMPERATURE  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR CFTempArray4ObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(temperature_UnitCode, coordinator, CDT),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(CFTempArray4,mVMeasurement, MVM),  // void * pObject_value
  MVM_T_TEMPERATURE  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR CFTempArray5ObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(temperature_UnitCode, coordinator, CDT),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(CFTempArray5,mVMeasurement, MVM),  // void * pObject_value
  MVM_T_TEMPERATURE  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR CFTempArray6ObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(temperature_UnitCode, coordinator, CDT),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(CFTempArray6,mVMeasurement, MVM),  // void * pObject_value
  MVM_T_TEMPERATURE  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR CFPressArray0ObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(dvPressureUnitCode, mapper, MAPPER),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(CFPressArray0,mVMeasurement, MVM),  // void * pObject_value
  MVM_T_PRESSURE  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR CFPressArray1ObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(dvPressureUnitCode, mapper, MAPPER),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(CFPressArray1,mVMeasurement, MVM),  // void * pObject_value
  MVM_T_PRESSURE  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR CFPressArray2ObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(dvPressureUnitCode, mapper, MAPPER),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(CFPressArray2,mVMeasurement, MVM),  // void * pObject_value
  MVM_T_PRESSURE  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR CFPressArray3ObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(dvPressureUnitCode, mapper, MAPPER),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(CFPressArray3,mVMeasurement, MVM),  // void * pObject_value
  MVM_T_PRESSURE  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR CFPressArray4ObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(dvPressureUnitCode, mapper, MAPPER),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(CFPressArray4,mVMeasurement, MVM),  // void * pObject_value
  MVM_T_PRESSURE  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR CFPressArray5ObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(dvPressureUnitCode, mapper, MAPPER),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(CFPressArray5,mVMeasurement, MVM),  // void * pObject_value
  MVM_T_PRESSURE  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR CFPressArray6ObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(dvPressureUnitCode, mapper, MAPPER),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(CFPressArray6,mVMeasurement, MVM),  // void * pObject_value
  MVM_T_PRESSURE  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR CFPressArray7ObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(dvPressureUnitCode, mapper, MAPPER),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(CFPressArray7,mVMeasurement, MVM),  // void * pObject_value
  MVM_T_PRESSURE  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR CFPressArray8ObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(dvPressureUnitCode, mapper, MAPPER),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(CFPressArray8,mVMeasurement, MVM),  // void * pObject_value
  MVM_T_PRESSURE  // TUSIGN8 unitsType
};

static const SLOW T_RANGE_E8 matrixSizeDescriptor =
{
  _MATRIX_TABLE,  // TUSIGN8* ptrCodeTab
  sizeof(_MATRIX_TABLE)  // TUSIGN8 numberOfCodes
};

static const SLOW T_UOM_PF_DESCRIPTOR QnMaxDNObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(QnUnitCode, mVMeasurement, MVM),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(QnMaxDN, mVMeasurement, MVM),  // void * pObject_value
  MVM_T_VOLUME_FLOW  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR QmMaxDNObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(QmUnitCode, mVMeasurement, MVM),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(QmMaxDN, mVMeasurement, MVM),  // void * pObject_value
  MVM_T_MASS_FLOW  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR QpMaxDNObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(QpUnitCode, mVMeasurement, MVM),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(QpMaxDN, mVMeasurement, MVM),  // void * pObject_value
  MVM_T_POWER  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR QnPartialMaxDNObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(QnUnitCode, mVMeasurement, MVM),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(QnPartialMaxDN, mVMeasurement, MVM),  // void * pObject_value
  MVM_T_VOLUME_FLOW  // TUSIGN8 unitsType
};

static const SLOW T_UOM_PF_DESCRIPTOR QvPartialMaxDNObjDescriptor =
{
  SUBSYS_OBJPTR_IDX_(QvUnitCode, sVMeasurement, SVM),  // void * pObject_unit
  SUBSYS_OBJPTR_IDX_(QvPartialMaxDN, mVMeasurement, MVM),  // void * pObject_value
  MVM_T_VOLUME_FLOW  // TUSIGN8 unitsType
};

static const SLOW T_DEV_RNGFLT_DESCRIPTOR QnRangeDescriptor =
{
  SUBSYS_OBJPTR_IDX_(QnMaxDN, mVMeasurement, MVM),  // void * ptrMaxDN
  SUBSYS_OBJPTR_IDX_(QMinDN, mVMeasurement, MVM),  // void * ptrMinDN
  1.0f,  // TFLOAT hiLim_MaxPrcOfDnSpan
  0.15f,  // TFLOAT hiLim_MinPrcOfDnSpan
  1.0f,  // TFLOAT loLim_MaxPrcOfCurAllowedSpan
  0.0f  // TFLOAT loLim_MinPrcOfCurAllowedSpan
};

static const SLOW T_DEV_RNGFLT_DESCRIPTOR QmRangeDescriptor =
{
  SUBSYS_OBJPTR_IDX_(QmMaxDN, mVMeasurement, MVM),  // void * ptrMaxDN
  SUBSYS_OBJPTR_IDX_(QMinDN, mVMeasurement, MVM),  // void * ptrMinDN
  1.0f,  // TFLOAT hiLim_MaxPrcOfDnSpan
  0.15f,  // TFLOAT hiLim_MinPrcOfDnSpan
  1.0f,  // TFLOAT loLim_MaxPrcOfCurAllowedSpan
  0.0f  // TFLOAT loLim_MinPrcOfCurAllowedSpan
};

static const SLOW T_DEV_RNGFLT_DESCRIPTOR QpRangeDescriptor =
{
  SUBSYS_OBJPTR_IDX_(QpMaxDN, mVMeasurement, MVM),  // void * ptrMaxDN
  SUBSYS_OBJPTR_IDX_(QMinDN, mVMeasurement, MVM),  // void * ptrMinDN
  1.0f,  // TFLOAT hiLim_MaxPrcOfDnSpan
  0.15f,  // TFLOAT hiLim_MinPrcOfDnSpan
  1.0f,  // TFLOAT loLim_MaxPrcOfCurAllowedSpan
  0.0f  // TFLOAT loLim_MinPrcOfCurAllowedSpan
};

static const SLOW T_DEV_RNGFLT_DESCRIPTOR QnPartialRangeDescriptor =
{
  SUBSYS_OBJPTR_IDX_(QnPartialMaxDN, mVMeasurement, MVM),  // void * ptrMaxDN
  SUBSYS_OBJPTR_IDX_(QMinDN, mVMeasurement, MVM),  // void * ptrMinDN
  1.0f,  // TFLOAT hiLim_MaxPrcOfDnSpan
  0.15f,  // TFLOAT hiLim_MinPrcOfDnSpan
  1.0f,  // TFLOAT loLim_MaxPrcOfCurAllowedSpan
  0.0f  // TFLOAT loLim_MinPrcOfCurAllowedSpan
};

static const SLOW T_DEV_RNGFLT_DESCRIPTOR QvPartialRangeDescriptor =
{
  SUBSYS_OBJPTR_IDX_(QvPartialMaxDN, mVMeasurement, MVM),  // void * ptrMaxDN
  SUBSYS_OBJPTR_IDX_(QMinDN, mVMeasurement, MVM),  // void * ptrMinDN
  1.0f,  // TFLOAT hiLim_MaxPrcOfDnSpan
  0.15f,  // TFLOAT hiLim_MinPrcOfDnSpan
  1.0f,  // TFLOAT loLim_MaxPrcOfCurAllowedSpan
  0.0f  // TFLOAT loLim_MinPrcOfCurAllowedSpan
};

static const SLOW T_RANGE_E8 steamPwrrFRDescriptor =
{
  _PWR_FWD_REV_SW,  // TUSIGN8* ptrCodeTab
  sizeof(_PWR_FWD_REV_SW)  // TUSIGN8 numberOfCodes
};
//@SubGen end@<DESCRIPTORS>


//-------------------------------------------------------------------------------------------------
//! objectList
//lint -e{31} as declaration in line 66
const SLOW T_DATA_OBJ mVMeasurementObjectList[] =
{
    //lint -e605 All non-volatile data are const for T_UNIT
    //           and the typecast (void FAST*) does not increase the readability
    //@SubGen start@    <OBJECTS>
    //Qn
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.Qn,
        RAM),

    //QnSimEnable
    CONSTRUCT_TABENUM8(
        &mVMeasurementProtected.QnSimEnable,
        RAM,
        &sharedTRangeE8Descriptor0),

    //QnSim
    CONSTRUCT_DYN_FLOAT(
        &mVMeasurementUnprotected.QnSim,
        RAM,
        &QnSimDescriptor),

    //QnDamped
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.QnDamped,
        RAM),

    //QnUnitCode
    CONSTRUCT_TABENUM8(
        &mVMeasurementStaticFrequentStaticDefault.QnUnitCode,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor2),

    //QnUnitType
    CONSTRUCT_CONST_U8(
        &mVMeasurementConstant.QnUnitType),

    //QnObj
    CONSTRUCT_STRUCT_UOM_PF(
        &QnObjDescriptor),

    //QnDampedObj
    CONSTRUCT_STRUCT_UOM_PF(
        &QnDampedObjDescriptor),

    //QnSimObj
    CONSTRUCT_STRUCT_UOM_PF(
        &QnSimObjDescriptor),

    //QnRange
    CONSTRUCT_STRUCT_DEV_RNGFLT(
        &mVMeasurementStaticFrequentStaticDefault.QnRange,
        NON_VOLATILE,
        &QnRangeDescriptor),

    //QnSimUnitCode
    CONSTRUCT_TABENUM8(
        &mVMeasurementUnprotected.QnSimUnitCode,
        RAM,
        &sharedTRangeE8Descriptor2),

    //QnRangeObj
    CONSTRUCT_STRUCT_UOM_PF(
        &QnRangeObjDescriptor),

    //QnMaxDN
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.QnMaxDN,
        RAM),

    //QnMaxDNDuplicated
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.QnMaxDNDuplicated,
        RAM),

    //QMinDN
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementUnprotected.QMinDN,
        RAM),

    //QnMaxDNObj
    CONSTRUCT_STRUCT_UOM_PF(
        &QnMaxDNObjDescriptor),

    //QnPerSim
    CONSTRUCT_FLOAT(
        &mVMeasurementUnprotected.QnPerSim,
        RAM,
        &sharedTRangeFloatDescriptor1),

    //QnPercentage
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.QnPercentage,
        RAM),

    //QnClassification
    CONSTRUCT_SIMPLE_U8(
        &mVMeasurementUnprotected.QnClassification,
        READONLY_RAM),

    //QnStatus
    CONSTRUCT_SIMPLE_U8(
        &mVMeasurementUnprotected.QnStatus,
        RAM),

    //QnDampingTime
    CONSTRUCT_FLOAT(
        &mVMeasurementStaticFrequentStaticDefault.QnDampingTime,
        NON_VOLATILE,
        &sharedTRangeFloatDescriptor3),

    //Qm
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.Qm,
        RAM),

    //QmSimEnable
    CONSTRUCT_TABENUM8(
        &mVMeasurementProtected.QmSimEnable,
        RAM,
        &sharedTRangeE8Descriptor0),

    //QmSim
    CONSTRUCT_DYN_FLOAT(
        &mVMeasurementUnprotected.QmSim,
        RAM,
        &QmSimDescriptor),

    //QmDamped
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.QmDamped,
        RAM),

    //QmUnitCode
    CONSTRUCT_TABENUM8(
        &mVMeasurementStaticFrequentStaticDefault.QmUnitCode,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor4),

    //QmUnitType
    CONSTRUCT_CONST_U8(
        &mVMeasurementConstant.QmUnitType),

    //QmObj
    CONSTRUCT_STRUCT_UOM_PF(
        &QmObjDescriptor),

    //QmDampedObj
    CONSTRUCT_STRUCT_UOM_PF(
        &QmDampedObjDescriptor),

    //QmSimObj
    CONSTRUCT_STRUCT_UOM_PF(
        &QmSimObjDescriptor),

    //QmRange
    CONSTRUCT_STRUCT_DEV_RNGFLT(
        &mVMeasurementStaticFrequentStaticDefault.QmRange,
        NON_VOLATILE,
        &QmRangeDescriptor),

    //QAlmRange
    CONSTRUCT_STRUCT_RNGFLT(
        &mVMeasurementStaticRare.QAlmRange,
        NON_VOLATILE,
        &QAlmRangeDescriptor),

    //QmSimUnitCode
    CONSTRUCT_TABENUM8(
        &mVMeasurementUnprotected.QmSimUnitCode,
        RAM,
        &sharedTRangeE8Descriptor4),

    //QmRangeObj
    CONSTRUCT_STRUCT_UOM_PF(
        &QmRangeObjDescriptor),

    //QmMaxDN
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.QmMaxDN,
        RAM),

    //QmMaxDNDuplicated
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.QmMaxDNDuplicated,
        RAM),

    //QmMaxDNObj
    CONSTRUCT_STRUCT_UOM_PF(
        &QmMaxDNObjDescriptor),

    //QmPerSim
    CONSTRUCT_FLOAT(
        &mVMeasurementUnprotected.QmPerSim,
        RAM,
        &sharedTRangeFloatDescriptor1),

    //QmPercentage
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.QmPercentage,
        RAM),

    //QmClassification
    CONSTRUCT_SIMPLE_U8(
        &mVMeasurementUnprotected.QmClassification,
        READONLY_RAM),

    //QmStatus
    CONSTRUCT_SIMPLE_U8(
        &mVMeasurementUnprotected.QmStatus,
        RAM),

    //QmDampingTime
    CONSTRUCT_FLOAT(
        &mVMeasurementStaticFrequentStaticDefault.QmDampingTime,
        NON_VOLATILE,
        &sharedTRangeFloatDescriptor3),

    //Qp
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.Qp,
        RAM),

    //QpSimEnable
    CONSTRUCT_TABENUM8(
        &mVMeasurementProtected.QpSimEnable,
        RAM,
        &sharedTRangeE8Descriptor0),

    //QpSim
    CONSTRUCT_DYN_FLOAT(
        &mVMeasurementUnprotected.QpSim,
        RAM,
        &QpSimDescriptor),

    //QpDamped
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.QpDamped,
        RAM),

    //QpUnitCode
    CONSTRUCT_TABENUM8(
        &mVMeasurementStaticFrequentStaticDefault.QpUnitCode,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor5),

    //QpUnitType
    CONSTRUCT_CONST_U8(
        &mVMeasurementConstant.QpUnitType),

    //QpObj
    CONSTRUCT_STRUCT_UOM_PF(
        &QpObjDescriptor),

    //QpDampedObj
    CONSTRUCT_STRUCT_UOM_PF(
        &QpDampedObjDescriptor),

    //QpSimObj
    CONSTRUCT_STRUCT_UOM_PF(
        &QpSimObjDescriptor),

    //QpRange
    CONSTRUCT_STRUCT_DEV_RNGFLT(
        &mVMeasurementStaticFrequentStaticDefault.QpRange,
        NON_VOLATILE,
        &QpRangeDescriptor),

    //QpSimUnitCode
    CONSTRUCT_TABENUM8(
        &mVMeasurementUnprotected.QpSimUnitCode,
        RAM,
        &sharedTRangeE8Descriptor5),

    //QpRangeObj
    CONSTRUCT_STRUCT_UOM_PF(
        &QpRangeObjDescriptor),

    //QpMaxDN
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.QpMaxDN,
        RAM),

    //QpMaxDNDuplicated
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.QpMaxDNDuplicated,
        RAM),

    //QpMaxDNObj
    CONSTRUCT_STRUCT_UOM_PF(
        &QpMaxDNObjDescriptor),

    //QpPerSim
    CONSTRUCT_FLOAT(
        &mVMeasurementUnprotected.QpPerSim,
        RAM,
        &sharedTRangeFloatDescriptor1),

    //QpPercentage
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.QpPercentage,
        RAM),

    //QpClassification
    CONSTRUCT_SIMPLE_U8(
        &mVMeasurementUnprotected.QpClassification,
        READONLY_RAM),

    //QpStatus
    CONSTRUCT_SIMPLE_U8(
        &mVMeasurementUnprotected.QpStatus,
        RAM),

    //QpDampingTime
    CONSTRUCT_FLOAT(
        &mVMeasurementStaticFrequentStaticDefault.QpDampingTime,
        NON_VOLATILE,
        &sharedTRangeFloatDescriptor3),

    //QnPartial
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.QnPartial,
        RAM),

    //QnPartialSimEnable
    CONSTRUCT_TABENUM8(
        &mVMeasurementProtected.QnPartialSimEnable,
        RAM,
        &sharedTRangeE8Descriptor0),

    //QnPartialSim
    CONSTRUCT_DYN_FLOAT(
        &mVMeasurementUnprotected.QnPartialSim,
        RAM,
        &QnPartialSimDescriptor),

    //QnPartialDamped
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.QnPartialDamped,
        RAM),

    //QnPartialObj
    CONSTRUCT_STRUCT_UOM_PF(
        &QnPartialObjDescriptor),

    //QnPartialDampedObj
    CONSTRUCT_STRUCT_UOM_PF(
        &QnPartialDampedObjDescriptor),

    //QnPartialSimObj
    CONSTRUCT_STRUCT_UOM_PF(
        &QnPartialSimObjDescriptor),

    //QnPartialRange
    CONSTRUCT_STRUCT_DEV_RNGFLT(
        &mVMeasurementStaticFrequentStaticDefault.QnPartialRange,
        NON_VOLATILE,
        &QnPartialRangeDescriptor),

    //QnPartialRangeObj
    CONSTRUCT_STRUCT_UOM_PF(
        &QnPartialRangeObjDescriptor),

    //QnPartialMaxDN
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.QnPartialMaxDN,
        RAM),

    //QnPartialMaxDNDuplicated
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.QnPartialMaxDNDuplicated,
        RAM),

    //QnPartialMaxDNObj
    CONSTRUCT_STRUCT_UOM_PF(
        &QnPartialMaxDNObjDescriptor),

    //QnPartialPerSim
    CONSTRUCT_FLOAT(
        &mVMeasurementUnprotected.QnPartialPerSim,
        RAM,
        &sharedTRangeFloatDescriptor1),

    //QnPartialPercentage
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.QnPartialPercentage,
        RAM),

    //QnPartialClassification
    CONSTRUCT_SIMPLE_U8(
        &mVMeasurementUnprotected.QnPartialClassification,
        RAM),

    //QnPartialStatus
    CONSTRUCT_SIMPLE_U8(
        &mVMeasurementUnprotected.QnPartialStatus,
        RAM),

    //QnPartialDampingTime
    CONSTRUCT_FLOAT(
        &mVMeasurementStaticFrequentStaticDefault.QnPartialDampingTime,
        NON_VOLATILE,
        &sharedTRangeFloatDescriptor3),

    //QvPartial
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.QvPartial,
        RAM),

    //QvPartialSimEnable
    CONSTRUCT_TABENUM8(
        &mVMeasurementProtected.QvPartialSimEnable,
        RAM,
        &sharedTRangeE8Descriptor0),

    //QvPartialSim
    CONSTRUCT_DYN_FLOAT(
        &mVMeasurementUnprotected.QvPartialSim,
        RAM,
        &QvPartialSimDescriptor),

    //QvPartialDamped
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.QvPartialDamped,
        RAM),

    //QvPartialObj
    CONSTRUCT_STRUCT_UOM_PF(
        &QvPartialObjDescriptor),

    //QvPartialDampedObj
    CONSTRUCT_STRUCT_UOM_PF(
        &QvPartialDampedObjDescriptor),

    //QvPartialSimObj
    CONSTRUCT_STRUCT_UOM_PF(
        &QvPartialSimObjDescriptor),

    //QvPartialRange
    CONSTRUCT_STRUCT_DEV_RNGFLT(
        &mVMeasurementStaticFrequentStaticDefault.QvPartialRange,
        NON_VOLATILE,
        &QvPartialRangeDescriptor),

    //QvPartialRangeObj
    CONSTRUCT_STRUCT_UOM_PF(
        &QvPartialRangeObjDescriptor),

    //QvPartialMaxDN
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.QvPartialMaxDN,
        RAM),

    //QvPartialMaxDNDuplicated
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.QvPartialMaxDNDuplicated,
        RAM),

    //QvPartialMaxDNObj
    CONSTRUCT_STRUCT_UOM_PF(
        &QvPartialMaxDNObjDescriptor),

    //QvPartialPerSim
    CONSTRUCT_FLOAT(
        &mVMeasurementUnprotected.QvPartialPerSim,
        RAM,
        &sharedTRangeFloatDescriptor1),

    //QvPartialPercentage
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.QvPartialPercentage,
        RAM),

    //QvPartialClassification
    CONSTRUCT_SIMPLE_U8(
        &mVMeasurementUnprotected.QvPartialClassification,
        RAM),

    //QvPartialStatus
    CONSTRUCT_SIMPLE_U8(
        &mVMeasurementUnprotected.QvPartialStatus,
        RAM),

    //QvPartialDampingTime
    CONSTRUCT_FLOAT(
        &mVMeasurementStaticFrequentStaticDefault.QvPartialDampingTime,
        NON_VOLATILE,
        &QvPartialDampingTimeDescriptor),

    //steamStatus
    CONSTRUCT_TABENUM8(
        &mVMeasurementUnprotected.steamStatus,
        RAM,
        &steamStatusDescriptor),

    //steamType
    CONSTRUCT_TABENUM8(
        &mVMeasurementREPLACE_STATIC_FREQUENT_STATIC_DEFAULT.steamType,
        NON_VOLATILE,
        &steamTypeDescriptor),

    //liquidCorrection
    CONSTRUCT_TABENUM8(
        &mVMeasurementREPLACE_STATIC_FREQUENT_STATIC_DEFAULT.liquidCorrection,
        NON_VOLATILE,
        &liquidCorrectionDescriptor),

    //heatCapacity
    CONSTRUCT_FLOAT(
        &mVMeasurementStaticFrequentStaticDefault.heatCapacity,
        NON_VOLATILE,
        &heatCapacityDescriptor),

    //densityRef
    CONSTRUCT_FLOAT(
        &mVMeasurementStaticFrequentStaticDefault.densityRef,
        NON_VOLATILE,
        &densityRefDescriptor),

    //densigyRefObj
    CONSTRUCT_STRUCT_UOM_PF(
        &densigyRefObjDescriptor),

    //densityS
    CONSTRUCT_FLOAT(
        &mVMeasurementStaticRare.densityS,
        NON_VOLATILE,
        &densitySDescriptor),

    //densityExpandBeta2
    CONSTRUCT_FLOAT(
        &mVMeasurementREPLACE_STATIC_FREQUENT_STATIC_DEFAULT.densityExpandBeta2,
        NON_VOLATILE,
        &densityExpandBeta2Descriptor),

    //volumeExpandBeta1
    CONSTRUCT_FLOAT(
        &mVMeasurementREPLACE_STATIC_FREQUENT_STATIC_DEFAULT.volumeExpandBeta1,
        NON_VOLATILE,
        &volumeExpandBeta1Descriptor),

    //gasRef
    CONSTRUCT_TABENUM8(
        &mVMeasurementStaticFrequentStaticDefault.gasRef,
        NON_VOLATILE,
        &gasRefDescriptor),

    //gasComputeType
    CONSTRUCT_TABENUM8(
        &mVMeasurementStaticFrequentStaticDefault.gasComputeType,
        NON_VOLATILE,
        &gasComputeTypeDescriptor),

    //gasDensitySelection
    CONSTRUCT_TABENUM8(
        &mVMeasurementStaticFrequentStaticDefault.gasDensitySelection,
        NON_VOLATILE,
        &gasDensitySelectionDescriptor),

    //calorificEnergy
    CONSTRUCT_FLOAT(
        &mVMeasurementREPLACE_STATIC_FREQUENT_STATIC_DEFAULT.calorificEnergy,
        NON_VOLATILE,
        &calorificEnergyDescriptor),

    //gasConfFlag
    CONSTRUCT_TABENUM8(
        &mVMeasurementStaticRare.gasConfFlag,
        NON_VOLATILE,
        &sharedTRangeE8Descriptor6),

    //molFractions
    CONSTRUCT_ARRAY_FLOAT(
        mVMeasurementStaticRare.molFractions,
        21,
        NON_VOLATILE,
        &molFractionsDescriptor),

    //compressFactorArray
    CONSTRUCT_ARRAY_FLOAT(
        mVMeasurementStaticRare.compressFactorArray,
        63,
        NON_VOLATILE,
        &compressFactorArrayDescriptor),

    //CFTempArray0
    CONSTRUCT_FLOAT(
        &mVMeasurementStaticRare.CFTempArray0,
        NON_VOLATILE,
        &sharedTRangeFloatDescriptor7),

    //CFTempArray1
    CONSTRUCT_FLOAT(
        &mVMeasurementStaticRare.CFTempArray1,
        NON_VOLATILE,
        &sharedTRangeFloatDescriptor7),

    //CFTempArray2
    CONSTRUCT_FLOAT(
        &mVMeasurementStaticRare.CFTempArray2,
        NON_VOLATILE,
        &sharedTRangeFloatDescriptor7),

    //CFTempArray3
    CONSTRUCT_FLOAT(
        &mVMeasurementStaticRare.CFTempArray3,
        NON_VOLATILE,
        &sharedTRangeFloatDescriptor7),

    //CFTempArray4
    CONSTRUCT_FLOAT(
        &mVMeasurementStaticRare.CFTempArray4,
        NON_VOLATILE,
        &sharedTRangeFloatDescriptor7),

    //CFTempArray5
    CONSTRUCT_FLOAT(
        &mVMeasurementStaticRare.CFTempArray5,
        NON_VOLATILE,
        &sharedTRangeFloatDescriptor7),

    //CFTempArray6
    CONSTRUCT_FLOAT(
        &mVMeasurementStaticRare.CFTempArray6,
        NON_VOLATILE,
        &sharedTRangeFloatDescriptor7),

    //CFTempArray0Obj
    CONSTRUCT_STRUCT_UOM_PF(
        &CFTempArray0ObjDescriptor),

    //CFTempArray1Obj
    CONSTRUCT_STRUCT_UOM_PF(
        &CFTempArray1ObjDescriptor),

    //CFTempArray2Obj
    CONSTRUCT_STRUCT_UOM_PF(
        &CFTempArray2ObjDescriptor),

    //CFTempArray3Obj
    CONSTRUCT_STRUCT_UOM_PF(
        &CFTempArray3ObjDescriptor),

    //CFTempArray4Obj
    CONSTRUCT_STRUCT_UOM_PF(
        &CFTempArray4ObjDescriptor),

    //CFTempArray5Obj
    CONSTRUCT_STRUCT_UOM_PF(
        &CFTempArray5ObjDescriptor),

    //CFTempArray6Obj
    CONSTRUCT_STRUCT_UOM_PF(
        &CFTempArray6ObjDescriptor),

    //CFPressArray0
    CONSTRUCT_FLOAT(
        &mVMeasurementStaticRare.CFPressArray0,
        NON_VOLATILE,
        &sharedTRangeFloatDescriptor8),

    //CFPressArray1
    CONSTRUCT_FLOAT(
        &mVMeasurementStaticRare.CFPressArray1,
        NON_VOLATILE,
        &sharedTRangeFloatDescriptor8),

    //CFPressArray2
    CONSTRUCT_FLOAT(
        &mVMeasurementStaticRare.CFPressArray2,
        NON_VOLATILE,
        &sharedTRangeFloatDescriptor8),

    //CFPressArray3
    CONSTRUCT_FLOAT(
        &mVMeasurementStaticRare.CFPressArray3,
        NON_VOLATILE,
        &sharedTRangeFloatDescriptor8),

    //CFPressArray4
    CONSTRUCT_FLOAT(
        &mVMeasurementStaticRare.CFPressArray4,
        NON_VOLATILE,
        &sharedTRangeFloatDescriptor8),

    //CFPressArray5
    CONSTRUCT_FLOAT(
        &mVMeasurementStaticRare.CFPressArray5,
        NON_VOLATILE,
        &sharedTRangeFloatDescriptor8),

    //CFPressArray6
    CONSTRUCT_FLOAT(
        &mVMeasurementStaticRare.CFPressArray6,
        NON_VOLATILE,
        &sharedTRangeFloatDescriptor8),

    //CFPressArray7
    CONSTRUCT_FLOAT(
        &mVMeasurementStaticRare.CFPressArray7,
        NON_VOLATILE,
        &sharedTRangeFloatDescriptor8),

    //CFPressArray8
    CONSTRUCT_FLOAT(
        &mVMeasurementStaticRare.CFPressArray8,
        NON_VOLATILE,
        &sharedTRangeFloatDescriptor8),

    //CFPressArray0Obj
    CONSTRUCT_STRUCT_UOM_PF(
        &CFPressArray0ObjDescriptor),

    //CFPressArray1Obj
    CONSTRUCT_STRUCT_UOM_PF(
        &CFPressArray1ObjDescriptor),

    //CFPressArray2Obj
    CONSTRUCT_STRUCT_UOM_PF(
        &CFPressArray2ObjDescriptor),

    //CFPressArray3Obj
    CONSTRUCT_STRUCT_UOM_PF(
        &CFPressArray3ObjDescriptor),

    //CFPressArray4Obj
    CONSTRUCT_STRUCT_UOM_PF(
        &CFPressArray4ObjDescriptor),

    //CFPressArray5Obj
    CONSTRUCT_STRUCT_UOM_PF(
        &CFPressArray5ObjDescriptor),

    //CFPressArray6Obj
    CONSTRUCT_STRUCT_UOM_PF(
        &CFPressArray6ObjDescriptor),

    //CFPressArray7Obj
    CONSTRUCT_STRUCT_UOM_PF(
        &CFPressArray7ObjDescriptor),

    //CFPressArray8Obj
    CONSTRUCT_STRUCT_UOM_PF(
        &CFPressArray8ObjDescriptor),

    //compressFactorR
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.compressFactorR,
        RAM),

    //relativeDensity
    CONSTRUCT_FLOAT(
        &mVMeasurementStaticRare.relativeDensity,
        NON_VOLATILE,
        &relativeDensityDescriptor),

    //relativeDensityRef
    CONSTRUCT_TABENUM8(
        &mVMeasurementStaticRare.relativeDensityRef,
        NON_VOLATILE,
        &relativeDensityRefDescriptor),

    //refCalorifcValue
    CONSTRUCT_FLOAT(
        &mVMeasurementStaticRare.refCalorifcValue,
        NON_VOLATILE,
        &refCalorifcValueDescriptor),

    //calorificValueRef
    CONSTRUCT_TABENUM8(
        &mVMeasurementStaticRare.calorificValueRef,
        NON_VOLATILE,
        &calorificValueRefDescriptor),

    //mvmAlarmSimulation
    CONSTRUCT_ALARM(
        &mVMeasurementDynamicDuplicated.mvmAlarmSimulation,
        sizeof(T_DIAGNOSIS_ALARM),
        RAM,
        &mvmAlarmSimulationDescriptor),

    //mvmDiagnosis
    CONSTRUCT_SIMPLE_U8(
        &mVMeasurementDynamicDuplicated.mvmDiagnosis,
        RAM),

    //refTemperature
    CONSTRUCT_FLOAT(
        &mVMeasurementStaticFrequentStaticDefault.refTemperature,
        NON_VOLATILE,
        &refTemperatureDescriptor),

    //refTemperatureObj
    CONSTRUCT_STRUCT_UOM_PF(
        &refTemperatureObjDescriptor),

    //refPressure
    CONSTRUCT_FLOAT(
        &mVMeasurementStaticRare.refPressure,
        NON_VOLATILE,
        &refPressureDescriptor),

    //refPressureObj
    CONSTRUCT_STRUCT_UOM_PF(
        &refPressureObjDescriptor),

    //compressFactorS
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.compressFactorS,
        RAM),

    //compressFactorPreset
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementStaticRare.compressFactorPreset,
        NON_VOLATILE),

    //matrixSize
    CONSTRUCT_TABENUM8(
        &mVMeasurementStaticRare.matrixSize,
        NON_VOLATILE,
        &matrixSizeDescriptor),

    //spline2dRequest
    CONSTRUCT_TABENUM8(
        &mVMeasurementProtected.spline2dRequest,
        RAM,
        &sharedTRangeE8Descriptor6),

    //updateCFSRequest
    CONSTRUCT_TABENUM8(
        &mVMeasurementProtected.updateCFSRequest,
        RAM,
        &sharedTRangeE8Descriptor6),

    //updateCFPresetRequest
    CONSTRUCT_TABENUM8(
        &mVMeasurementProtected.updateCFPresetRequest,
        RAM,
        &sharedTRangeE8Descriptor6),

    //updateMaxDNRequest
    CONSTRUCT_SIMPLE_BIT8(
        &mVMeasurementProtected.updateMaxDNRequest,
        RAM),

    //Hw
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.Hw,
        RAM),

    //Hc
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.Hc,
        RAM),

    //HwPreset
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementUnprotected.HwPreset,
        RAM),

    //HcPreset
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementUnprotected.HcPreset,
        RAM),

    //steamPwrrFR
    CONSTRUCT_TABENUM8(
        &mVMeasurementStaticRare.steamPwrrFR,
        NON_VOLATILE,
        &steamPwrrFRDescriptor),

    //QnPartialDuplicated
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.QnPartialDuplicated,
        RAM),

    //QnPartialPercentageDuplicated
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.QnPartialPercentageDuplicated,
        RAM),

    //QnPartialDampedDuplicated
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.QnPartialDampedDuplicated,
        RAM),

    //QvPartialDuplicated
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.QvPartialDuplicated,
        RAM),

    //QvPartialPercentageDuplicated
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.QvPartialPercentageDuplicated,
        RAM),

    //QvPartialDampedDuplicated
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.QvPartialDampedDuplicated,
        RAM),

    //QnDuplicated
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.QnDuplicated,
        RAM),

    //QnPercentageDuplicated
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.QnPercentageDuplicated,
        RAM),

    //QnDampedDuplicated
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.QnDampedDuplicated,
        RAM),

    //QmDuplicated
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.QmDuplicated,
        RAM),

    //QmPercentageDuplicated
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.QmPercentageDuplicated,
        RAM),

    //QmDampedDuplicated
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.QmDampedDuplicated,
        RAM),

    //QpDuplicated
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.QpDuplicated,
        RAM),

    //QpPercentageDuplicated
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.QpPercentageDuplicated,
        RAM),

    //QpDampedDuplicated
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.QpDampedDuplicated,
        RAM),

    //HwDuplicated
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.HwDuplicated,
        RAM),

    //HcDuplicated
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.HcDuplicated,
        RAM),

    //compressFactorRDuplicated
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.compressFactorRDuplicated,
        RAM),

    //compressFactorSDuplicated
    CONSTRUCT_SIMPLE_FLOAT(
        &mVMeasurementDynamicDuplicated.compressFactorSDuplicated,
        RAM),

    //mvmAlarmSimulationDuplicated
    CONSTRUCT_ALARM(
        &mVMeasurementDynamicDuplicated.mvmAlarmSimulationDuplicated,
        sizeof(T_DIAGNOSIS_ALARM),
        RAM,
        &mvmAlarmSimulationDuplicatedDescriptor),

    //mvmDiagnosisDuplicated
    CONSTRUCT_SIMPLE_U8(
        &mVMeasurementDynamicDuplicated.mvmDiagnosisDuplicated,
        RAM),

    //protectedDataCrc
    CONSTRUCT_SIMPLE_U16(
        &mVMeasurementUnprotected.protectedDataCrc,
        RAM),

    //protectedDataStatus
    CONSTRUCT_TABENUM8(
        &mVMeasurementProtected.protectedDataStatus,
        RAM,
        &protectedDataStatusDescriptor)
    //@SubGen end@    <OBJECTS>
    //lint +e605 warning on for the rest of the code
};



//-------------------------------------------------------------------------------------------------
//! define the MVMEASUREMENT-Class object
SLOW const T_MVMEASUREMENT mVMeasurement =
{
    {
        &mVMeasurementState,
        &mVMeasurementDataSemaphore,

        mVMeasurementObjectList,
        //lint -e{778,961} 
        sizeof(mVMeasurementObjectList)/sizeof(mVMeasurementObjectList[0])-1,

        mVMeasurementDataClassList,
        sizeof(mVMeasurementDataClassList)/sizeof(mVMeasurementDataClassList[0]),

        ChangeState_T_UNIT,
        GetState_T_UNIT,
        Initialize_MVM, // will be overloaded
        LoadRomDefaults_T_UNIT,
        ResetToDefault_T_UNIT,
        SaveAsDefault_T_UNIT,
        Get_T_UNIT,
        Put_T_MVM,
        Check_T_UNIT,
        GetAttributeDescription_T_UNIT,
        GetObjectDescription_T_UNIT
    }

    //subsystem EXE and SRV-methods
    //@SubGen start@    <METHODS>
    ,CalculateGasQnEXE_MVM
    ,CalculateGasQnPartialEXE_MVM
    ,CalculateGasQvPartialEXE_MVM
    ,CalculateGasQmEXE_MVM
    ,CalculateGasQpEXE_MVM
    ,CalculateLiquidQnEXE_MVM
    ,CalculateLiquidQmEXE_MVM
    ,CalculateLiquidQpEXE_MVM
    ,CalculateSteamQmEXE_MVM
    ,CalculateSteamQpEXE_MVM
    ,UpdateDiagnosisEXE_MVM
    ,CalcSteamDensityEXE_MVM
    ,CalcSteamHEXE_MVM
    ,CalcCompressFactor_MVM
    ,CheckDataCrcSRV_MVM
    ,GetQnPercentageSRV_MVM
    ,GetQmPercentageSRV_MVM
    ,GetQpPercentageSRV_MVM
    ,GetQnPartialPercentageSRV_MVM
    ,GetQvPartialPercentageSRV_MVM
    ,UpdateLiquidQnMaxDN_MVM
    ,UpdateGasQnMaxDN_MVM
    ,UpdateLiquidPowerMaxDN_MVM
    ,UpdateGasPowerMaxDN_MVM
    ,UpdateSteamPowerMaxDN_MVM
    ,UpdateQmMaxDN_MVM
    ,UpdateBiagasQvMaxDN_MVM
    ,UpdateQAlarmEXE_MVM
    //@SubGen end@    <METHODS>
};




/*

This section defines templates for the SubSystem CodeGenerator.
Do not modify or remove this section

see also: mVMeasurement_attributes.h


//special templates for specific standard storage classes:
//@SubGen template start@<DATACLASS CONSTANT>
//-------------------------------------------------------------------------------------------------
//! definition of constant block
const SLOW T_MVMEASUREMENT_TEMPLATE mVMeasurementTemplate=
{
    //@SubGen start@    <INITIALIZER TEMPLATE>
    //@SubGen end@    <INITIALIZER TEMPLATE>
};
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS CONSTANT>



//@SubGen template start@<DATACLASS DYNAMIC>
//-------------------------------------------------------------------------------------------------
//! definition of dynamic data
FAST T_MVMEASUREMENT_TEMPLATE mVMeasurementTemplate;
const SLOW T_MVMEASUREMENT_TEMPLATE mVMeasurementTemplateInitializer=
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
  #pragma bss_seg(".AAA$MVMEASUREMENT_DATA")
    const FAST T_MVMEASUREMENT_TEMPLATE mVMeasurementTemplate;
    const FAST T_MVMEASUREMENT_TEMPLATE mVMeasurementTemplateDefault;
  #pragma bss_seg()
#else
  #pragma location="MVMEASUREMENT_TEMPLATE"
    const FAST __no_init T_MVMEASUREMENT_TEMPLATE mVMeasurementTemplate;
  #pragma location="MVMEASUREMENT_TEMPLATE_DEFAULT"
    const FAST __no_init T_MVMEASUREMENT_TEMPLATE mVMeasurementTemplateDefault;
#endif
const SLOW T_MVMEASUREMENT_TEMPLATE mVMeasurementTemplateInitializer=
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
  #pragma bss_seg(".AAA$MVMEASUREMENT_DATA")
    const FAST T_MVMEASUREMENT_TEMPLATE mVMeasurementTemplate;
  #pragma bss_seg()
#else
  #pragma location="MVMEASUREMENT_TEMPLATE"
    const FAST __no_init T_MVMEASUREMENT_TEMPLATE mVMeasurementTemplate;
#endif
const SLOW T_MVMEASUREMENT_TEMPLATE mVMeasurementTemplateInitializer=
{
    //@SubGen start@    <INITIALIZER TEMPLATE>
    //@SubGen end@    <INITIALIZER TEMPLATE>
};
//-------------------------------------------------------------------------------------------------
//@SubGen template end@<DATACLASS>

//@SubGen template start@ <DATACLASSNAME>
mVMeasurementTemplate
//@SubGen template end@ <DATACLASSNAME>


//@SubGen template start@ <DATACLASSLIST DYNAMIC>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_DYNAMIC(mVMeasurementTemplate,mVMeasurementTemplateInitializer)
//@SubGen template end@ <DATACLASSLIST DYNAMIC>

//@SubGen template start@ <DATACLASSLIST CONSTANT>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_CONSTANT(mVMeasurementTemplate)
//@SubGen template end@ <DATACLASSLIST CONSTANT>

//@SubGen template start@ <DATACLASSLIST>
//lint -e{708} union initialization is OK
CONSTRUCTOR_DC_STATIC_NODEF(mVMeasurementTemplate,mVMeasurementTemplateInitializer)
//@SubGen template end@ <DATACLASSLIST>

//@SubGen template start@ <DATACLASSLIST STATIC_FREQUENT_CONSTANT_DEFAULT>
//lint -e{708} initialization is OK
CONSTRUCTOR_DC_STATIC_CONSTDEF(mVMeasurementTemplate,mVMeasurementTemplateInitializer)
//@SubGen template end@ <DATACLASSLIST STATIC_FREQUENT_CONSTANT_DEFAULT>

//@SubGen template start@ <DATACLASSLIST STATIC_FREQUENT_STATIC_DEFAULT>
//lint -e{708} initialization is OK
CONSTRUCTOR_DC_STATIC_STATDEF(mVMeasurementTemplate,mVMeasurementTemplateInitializer,mVMeasurementTemplateDefault)
//@SubGen template end@ <DATACLASSLIST STATIC_FREQUENT_STATIC_DEFAULT>

*/