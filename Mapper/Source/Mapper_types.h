//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Mapper
 Module
 Description    Type defintions for subsystem attributes
                for Subsystems based on T_UNIT.
 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before Mapper_types.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before Mapper_types.h
#endif

//............................................
//!  extend include list if neccessary
#include "T_DATA_OBJ/Interface/t_data_obj_rngflt.h"
#include "T_DATA_OBJ/Interface/t_data_obj_dev_rngflt.h"

#ifdef __MAPPER_TYPES_H__
  #error Mapper_types.h included more than once
#endif

#define __MAPPER_TYPES_H__

// implement the required storage-classes

//@SubGen put@ <DATACLASS>
 //-------------------------------------------------------------------------------------------------
 //! structure of STATIC_FREQUENT_STATIC_DEFAULT block
 typedef struct _T_MAPPER_STATIC_FREQUENT_STATIC_DEFAULT
 {
     //@SubGen start@    <DATACLASS STATIC_FREQUENT_STATIC_DEFAULT> 
    T_DEV_RNGFLT dvTExtRange;
    TFLOAT dvTExtPreset;
    TUSIGN8 dvPressureUnitCode;
    T_DEV_RNGFLT dvPressureRangeRelative;
    T_DEV_RNGFLT dvPressureRange;
    TFLOAT dvPressurePreset;
    TUSIGN8 dvDensityUnitCode;
    TFLOAT dvDensityPreset;
    T_DEV_RNGFLT dvContentRange;
    TFLOAT dvContentPreset;
     //@SubGen end@    <DATACLASS STATIC_FREQUENT_STATIC_DEFAULT> 
 } T_MAPPER_STATIC_FREQUENT_STATIC_DEFAULT;



 //-------------------------------------------------------------------------------------------------
 //! structure of Protected block
 typedef struct _T_MAPPER_PROTECTED
 {
     //@SubGen start@    <DATACLASS Protected> 
    TUSIGN8 PV_Assignment;
    TUSIGN8 SV_Assignment;
    TUSIGN8 flowOver103Bhv;
    TUSIGN8 dvTExtSimEnable;
    TUSIGN8 dvPressureSimEnable;
    TUSIGN8 dvDensitySimEnable;
    TUSIGN8 dvContentSimEnable;
    TUSIGN16 localOperatingModeSelection;
    TUSIGN8 presetChanged;
    TUSIGN8 protectedDataStatus;
     //@SubGen end@    <DATACLASS Protected> 
 } T_MAPPER_PROTECTED;



 //-------------------------------------------------------------------------------------------------
 //! structure of STATIC_FREQUENT_CONSTANT_DEFAULT block
 typedef struct _T_MAPPER_STATIC_FREQUENT_CONSTANT_DEFAULT
 {
     //@SubGen start@    <DATACLASS STATIC_FREQUENT_CONSTANT_DEFAULT> 
    TUSIGN8 diagFunCheckMask;
    TUSIGN8 diagOffSpecMask;
    TUSIGN8 diagMaintainMask;
    TUSIGN8 diagMinFlowMask;
    TUSIGN8 diagMaxFlowMask;
    TUSIGN8 diagFlow103Mask;
    TUSIGN8 diagTSensorOffSpecMask;
    TUSIGN8 diagTxSensorOffSpecMask;
    TUSIGN8 diagTSensorFailMask;
    TUSIGN8 diagLowCutoffMask;
     //@SubGen end@    <DATACLASS STATIC_FREQUENT_CONSTANT_DEFAULT> 
 } T_MAPPER_STATIC_FREQUENT_CONSTANT_DEFAULT;


 //-------------------------------------------------------------------------------------------------
 //! structure of Unprotected block
 typedef struct _T_MAPPER_UNPROTECTED
 {
     //@SubGen start@    <DATACLASS Unprotected> 
    TUSIGN8 diagSimulationStatus;
    TUSIGN8 diagSimulationType;
    TUSIGN8 diagnosisSimulation[16];
    TUSIGN8 diagDeviceStatus[6];
    TUSIGN8 diagnosisHighCondition;
    TUSIGN8 diagnosisHighClass;
    TUSIGN8 diagnosisHighGroup;
    TUSIGN8 diagHistory[6];
    TUSIGN8 diagnosisConditionNumber;
    TUSIGN8 diagnosisConditionDetailsClass;
    TUSIGN8 diagnosisConditionDetailsGroup;
    TUSIGN8 diagnosisConditionDetailsPriority;
    TUSIGN16 diagnosisConditionDetailsAlarmCounter;
    TUSIGN16 diagnosisConditionDetailsAlarmTimeCounterDay;
    TUSIGN32 diagnosisConditionDetailsAlarmTimeCounterMsec;
    TUSIGN16 diagnosisConditionDetailsTimeStampLastAlarmDay;
    TUSIGN32 diagnosisConditionDetailsTimeStampLastAlarmMsec;
    TUSIGN8 diagnosisConditionDetailsCodes[15];
    TUSIGN8 diagnosisMasking[6];
    TFLOAT PV;
    TFLOAT SV;
    TFLOAT TV;
    TFLOAT QV;
    TUSIGN8 PV_Unit[2];
    TUSIGN8 SV_Unit[2];
    TUSIGN8 TV_Unit[2];
    TUSIGN8 QV_Unit[2];
    TUSIGN8 PV_RangeUnit[2];
    TUSIGN8 SV_RangeUnit[2];
    TUSIGN8 TV_RangeUnit[2];
    TUSIGN8 QV_RangeUnit[2];
    TFLOAT PV_UpperRange;
    TFLOAT SV_UpperRange;
    TFLOAT TV_UpperRange;
    TFLOAT QV_UpperRange;
    TFLOAT PV_LowerRange;
    TFLOAT SV_LowerRange;
    TFLOAT TV_LowerRange;
    TFLOAT QV_LowerRange;
    TUSIGN8 TV_Assignment;
    TUSIGN8 QV_Assignment;
    TUSIGN8 PV_Classification;
    TUSIGN8 SV_Classification;
    TUSIGN8 TV_Classification;
    TUSIGN8 QV_Classification;
    TUSIGN8 PV_Status;
    TUSIGN8 SV_Status;
    TUSIGN8 TV_Status;
    TUSIGN8 QV_Status;
    TUSIGN8 PV_TFCode;
    TUSIGN8 PV_ACFlag;
    TFLOAT PV_PercentageForDO;
    TFLOAT PVSensorLimitHigh;
    TFLOAT PVSensorLimitLow;
    TUSIGN8 PVSensorLimitUnitcode[2];
    TFLOAT PVSensorminimumSpan;
    TFLOAT PVPercent_Range;
    TFLOAT PV_DampingTime;
    TUSIGN8 setCurrentValueAsPVRange;
    TFLOAT dvTExtSim;
    TUSIGN8 dvTExtClassification;
    TUSIGN8 dvTExtStatus;
    TFLOAT dvTExtPercentage;
    TUSIGN8 dvTExtSrc;
    TFLOAT dvPressureSim;
    TFLOAT dvPressureMaxDN;
    TFLOAT dvPressureMinDN;
    TFLOAT dvPressureRelativeMaxDN;
    TFLOAT dvPressureRelativeMinDN;
    TUSIGN8 dvPressureClassification;
    TUSIGN8 dvPressureStatus;
    TUSIGN8 dvPressureSrc;
    TFLOAT dvDensitySim;
    TFLOAT dvDensityMaxDN;
    TFLOAT dvDensityMinDN;
    TUSIGN8 dvDensityClassification;
    TUSIGN8 dvDensityStatus;
    TUSIGN8 dvDensitySrc;
    TFLOAT dvContentSim;
    TFLOAT dvContentMaxDN;
    TFLOAT dvContentMinDN;
    TUSIGN8 dvContentClassification;
    TUSIGN8 dvContentStatus;
    TUSIGN8 dvContentSrc;
    TFLOAT dvDummyPercentage;
    TFLOAT dvDummyDampingTime;
    TUSIGN8 dvDummyUnitCode;
    TUSIGN8 dvDummyUnitType;
    TUSIGN8 mapperAlarmSimulation;
    TUSIGN8 mapperDiagnosis;
    TUSIGN32 diagnosisCounter;
    TUSIGN16 protectedDataCrc;
    TUSIGN32 hartBurstInAlarmPeriod;
    TFLOAT dvTExtUserUnit;
    TFLOAT dvPressureUserUnit;
    TFLOAT dvDensityUserUnit;
    TUSIGN8 diagAlarmNamurConfiguration[48];
     //@SubGen end@    <DATACLASS Unprotected> 
 } T_MAPPER_UNPROTECTED;


 //-------------------------------------------------------------------------------------------------
 //! structure of CONSTANT block
 typedef struct _T_MAPPER_CONSTANT
 {
     //@SubGen start@    <DATACLASS CONSTANT> 
    TUSIGN8 PVPercent_Unit[2];
    TUSIGN8 PVPercent_Classifiction;
    TUSIGN8 dvTExtUnitType;
    TUSIGN8 dvPressureUnitType;
    TUSIGN8 dvDensityUnitType;
     //@SubGen end@    <DATACLASS CONSTANT> 
 } T_MAPPER_CONSTANT;


 //-------------------------------------------------------------------------------------------------
 //! structure of STATIC_RARE block
 typedef struct _T_MAPPER_STATIC_RARE
 {
     //@SubGen start@    <DATACLASS STATIC_RARE> 
    TFLOAT dvTExtMaxDN;
    TFLOAT dvTExtMinDN;
    TUSIGN8 dvPressureSimUnitCode;
    T_RANGE_FLT dvPressureAlmRange;
    TUSIGN8 dvDensitySimUnitCode;
    T_DEV_RNGFLT dvDensityRange;
    TUSIGN8 dvContentUnitCode[2];
     //@SubGen end@    <DATACLASS STATIC_RARE> 
 } T_MAPPER_STATIC_RARE;


 //-------------------------------------------------------------------------------------------------
 //! structure of DynamicDuplicated block
 typedef struct _T_MAPPER_DYNAMIC_DUPLICATED
 {
     //@SubGen start@    <DATACLASS DynamicDuplicated> 
    TFLOAT dvTExt;
    TFLOAT dvTExtDuplicated;
    TFLOAT dvPressure;
    TFLOAT dvPressureDuplicated;
    TFLOAT dvDensity;
    TFLOAT dvDensityDuplicated;
    TFLOAT dvContent;
    TFLOAT dvContentDuplicated;
     //@SubGen end@    <DATACLASS DynamicDuplicated> 
 } T_MAPPER_DYNAMIC_DUPLICATED;





/*

This section defines templates for the SubSystem CodeGenerator.
Do not modify or remove this section

//@SubGen template start@ <DATACLASS>
//-------------------------------------------------------------------------------------------------
//! structure of Template block
typedef struct _T_MAPPER_TEMPLATE
{
    //@SubGen start@    <DATACLASS Template> 
    //@SubGen end@    <DATACLASS Template> 
} T_MAPPER_TEMPLATE;
//@SubGen template end@ <DATACLASS>

*/

