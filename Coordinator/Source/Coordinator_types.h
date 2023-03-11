//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Coordinator
 Module
 Description    Type defintions for subsystem attributes
                for Subsystems based on T_UNIT.
 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before Coordinator_types.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before Coordinator_types.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __COORDINATOR_TYPES_H__
  #error Coordinator_types.h included more than once
#endif

#define __COORDINATOR_TYPES_H__

// implement the required storage-classes

//@SubGen put@ <DATACLASS>
 //-------------------------------------------------------------------------------------------------
 //! structure of FE_OUTPUT_FILTER_PARA block
 typedef struct _T_COORDINATOR_FE_OUTPUT_FILTER_PARA
 {
     //@SubGen start@    <DATACLASS FE_OUTPUT_FILTER_PARA> 
    TUSIGN8 ext_algSelection;
    TUSIGN8 int_algSelection;
    TUSIGN16 ss_paras[8];
    TUSIGN16 sf_paras[4];
     //@SubGen end@    <DATACLASS FE_OUTPUT_FILTER_PARA> 
 } T_COORDINATOR_FE_OUTPUT_FILTER_PARA;



 //-------------------------------------------------------------------------------------------------
 //! structure of ReplaceStaticFreqStaticDefault block
 typedef struct _T_COORDINATOR_REPLACE_STATIC_FREQ_STATIC_DEFAULT
 {
     //@SubGen start@    <DATACLASS ReplaceStaticFreqStaticDefault> 
    TUSIGN8 operatingMode;
    TFLOAT temperature_DampingTime;
    TUSIGN8 temperature_UnitCode;
    T_DEV_RNGFLT temperature_range;
    TFLOAT temperaturePreset;
    TUSIGN8 actualDensitySelection;
     //@SubGen end@    <DATACLASS ReplaceStaticFreqStaticDefault> 
 } T_COORDINATOR_REPLACE_STATIC_FREQ_STATIC_DEFAULT;



 //-------------------------------------------------------------------------------------------------
 //! structure of ReplaceStaticRare_Calibration block
 typedef struct _T_COORDINATOR_REPLACE_STATIC_RARE_CALIBRATION
 {
     //@SubGen start@    <DATACLASS ReplaceStaticRare_Calibration> 
    TUSIGN8 calibrationStatus;
    TCHAR internalRTDPassword[6];
    TCHAR analogInputPassword[6];
    TCHAR energyFlowPassword[6];
     //@SubGen end@    <DATACLASS ReplaceStaticRare_Calibration> 
 } T_COORDINATOR_REPLACE_STATIC_RARE_CALIBRATION;







 //-------------------------------------------------------------------------------------------------
 //! structure of STATIC_FREQUENT_CONSTANT_DEFAULT block
 typedef struct _T_COORDINATOR_STATIC_FREQUENT_CONSTANT_DEFAULT
 {
     //@SubGen start@    <DATACLASS STATIC_FREQUENT_CONSTANT_DEFAULT> 
    TUSIGN8 page1[30];
     //@SubGen end@    <DATACLASS STATIC_FREQUENT_CONSTANT_DEFAULT> 
 } T_COORDINATOR_STATIC_FREQUENT_CONSTANT_DEFAULT;




 //-------------------------------------------------------------------------------------------------
 //! structure of Unprotected block
 typedef struct _T_COORDINATOR_UNPROTECTED
 {
     //@SubGen start@    <DATACLASS Unprotected> 
    TUSIGN32 workingTimeMSec;
    TUSIGN32 partialWorkingTimeHour;
    TFLOAT frequency_Sim;
    TFLOAT frequency_MaxDN;
    TFLOAT frequency_MinDN;
    TFLOAT frequency_Span;
    TFLOAT frequency_Percentage;
    TUSIGN8 frequency_Status;
    TFLOAT temperature_Sim;
    TUSIGN8 simTemperature_UnitCode;
    TFLOAT temperature_MaxDN;
    TFLOAT temperature_MinDN;
    TFLOAT temperature_Span;
    TUSIGN8 temperature_Status;
    TUSIGN8 temperature_Selection;
    TFLOAT cpuLoad;
    TUSIGN8 frontEndConfParamWr;
    TUSIGN8 frontEndConfParamRd;
    TUSIGN8 frontEndTempTypeRd;
    TUSIGN32 updateTimeFast;
    TUSIGN32 updateTimeSlow;
    TUSIGN8 vortexDeviceStatus;
    TUSIGN8 additionalVortexDeviceStatus;
    TFLOAT snr;
    TUSIGN32 frontendOperationHour;
    TUSIGN32 frontendMaintenance;
    TUSIGN8 vbrCorrectionOn;
    TINT16 vbrFilStep;
    TUSIGN8 vbrFilNumCoef;
    TUSIGN8 fcdOn;
    TUSIGN8 fcdShift;
    TUSIGN8 fcdDelay;
    TUSIGN8 fcdNumCoef;
    TUSIGN8 lockMinIntPeriod;
    TUSIGN8 lockMaxIntPeriod;
    TUSIGN8 lockEstimation;
    TUSIGN8 accuracyHigh;
    TUSIGN8 accuracyMiddle;
    TUSIGN8 accuracyLow;
    TUSIGN8 analogGain;
    TUSIGN8 gainCtrl;
    TINT16 gainLowThresh;
    TINT16 gainHighThresh;
    TINT16 gainMarginFreq;
    TUSIGN8 gainMaxShift;
    TUSIGN8 lowFlowThreshOn;
    TINT16 lowFlowThresh;
    TINT16 lowFlowThreshCoef;
    TUSIGN8 noiseSampSize;
    TUSIGN8 staticDecimation;
    TUSIGN8 middleBufDecimation;
    TFLOAT maxFlowFreq;
    TFLOAT minFlowFreq;
    TUSIGN8 freqMinMarginPer;
    TUSIGN8 freqMaxMarginPer;
    TFLOAT temperatureOffsetCorrectDisp;
    TUSIGN32 timeStamp;
    TUSIGN8 tPresetChangeFlag;
    TUSIGN16 protectedDataCrc;
    TINT16 spanUpperRange;
    TUSIGN16 sigAmplitude;
    TUSIGN16 vbrAmplitude;
    TUSIGN16 sigMagnitude;
    TFLOAT temperatureOffsetCorrectUserDisp;
    TUSIGN32 BothMaintenanceCycle;
    TFLOAT rawTemperatureSensor;
     //@SubGen end@    <DATACLASS Unprotected> 
 } T_COORDINATOR_UNPROTECTED;



 //-------------------------------------------------------------------------------------------------
 //! structure of ReplaceStaticRare block
 typedef struct _T_COORDINATOR_REPLACE_STATIC_RARE
 {
     //@SubGen start@    <DATACLASS ReplaceStaticRare> 
    TFLOAT frequency_DampingTime;
    TUSIGN8 intRTDType;
    TFLOAT temperatureMaxRecord;
    T_RANGE_FLT temperatureAlmRange;
    TUSIGN8 converterType;
    TUSIGN8 vortexmeterSize;
    TUSIGN8 swirlmeterSize;
    TFLOAT temperatureOffsetCorrect;
     //@SubGen end@    <DATACLASS ReplaceStaticRare> 
 } T_COORDINATOR_REPLACE_STATIC_RARE;


 //-------------------------------------------------------------------------------------------------
 //! structure of Protected block
 typedef struct _T_COORDINATOR_PROTECTED
 {
     //@SubGen start@    <DATACLASS Protected> 
    TUSIGN8 coodinatorAlarmSimulation[4];
    TUSIGN8 simulationMode;
    TUSIGN8 frequencySim_Enable;
    TUSIGN8 temperatureSim_Enable;
    TUSIGN8 passwordEnable;
    TUSIGN8 protectedDataStatus;
     //@SubGen end@    <DATACLASS Protected> 
 } T_COORDINATOR_PROTECTED;


 //-------------------------------------------------------------------------------------------------
 //! structure of DynamicDuplicated block
 typedef struct _T_COORDINATOR_DYNAMIC_DUPLICATED
 {
     //@SubGen start@    <DATACLASS DynamicDuplicated> 
    TFLOAT frequency;
    TFLOAT frequency_Damped;
    TFLOAT temperature;
    TFLOAT temperature_Damped;
    TFLOAT temperature_Percentage;
    TFLOAT frequency_Duplicated;
    TFLOAT frequency_Damped_Duplictated;
    TFLOAT temperature_Duplicated;
    TFLOAT temperature_Damped_Duplicated;
    TUSIGN8 cdtDiagnosis[4];
    TUSIGN8 cdtDiagnosis_Duplicated[4];
    TFLOAT temperature_Percentage_Duplicated;
    TFLOAT temperatureSensor;
    TFLOAT temperatureSensor_Duplicated;
    TUSIGN8 autoselfcheck_Enable;
    TUSIGN8 autoselfcheck_Enable_Duplicated;
     //@SubGen end@    <DATACLASS DynamicDuplicated> 
 } T_COORDINATOR_DYNAMIC_DUPLICATED;



 //-------------------------------------------------------------------------------------------------
 //! structure of STATIC_RARE block
 typedef struct _T_COORDINATOR_STATIC_RARE
 {
     //@SubGen start@    <DATACLASS STATIC_RARE> 
    T_DEV_RNGFLT frequency_range;
    TUSIGN8 frequency_UnitCode;
    TUSIGN8 simFrequency_UnitCode;
    TUSIGN8 forceReplace;
    TFLOAT temperatureOffsetCorrectUser;
    TUSIGN8 reaction_InputFail;
     //@SubGen end@    <DATACLASS STATIC_RARE> 
 } T_COORDINATOR_STATIC_RARE;



 //-------------------------------------------------------------------------------------------------
 //! structure of CONSTANT block
 typedef struct _T_COORDINATOR_CONSTANT
 {
     //@SubGen start@    <DATACLASS CONSTANT> 
    TUSIGN8 frequency_UnitType;
    TUSIGN8 frequency_Classification;
    TUSIGN8 temperature_UnitType;
    TUSIGN8 temperature_Classification;
    TUSIGN8 familyRevision;
     //@SubGen end@    <DATACLASS CONSTANT> 
 } T_COORDINATOR_CONSTANT;



 //-------------------------------------------------------------------------------------------------
 //! structure of STATIC_CYCLIC block
 typedef struct _T_COORDINATOR_STATIC_CYCLIC
 {
     //@SubGen start@    <DATACLASS STATIC_CYCLIC> 
    TUSIGN32 totalWorkingTimeHour;
    TUSIGN32 tranMaintenanceCyc;
     //@SubGen end@    <DATACLASS STATIC_CYCLIC> 
 } T_COORDINATOR_STATIC_CYCLIC;




/*

This section defines templates for the SubSystem CodeGenerator.
Do not modify or remove this section

//@SubGen template start@ <DATACLASS>
//-------------------------------------------------------------------------------------------------
//! structure of Template block
typedef struct _T_COORDINATOR_TEMPLATE
{
    //@SubGen start@    <DATACLASS Template> 
    //@SubGen end@    <DATACLASS Template> 
} T_COORDINATOR_TEMPLATE;
//@SubGen template end@ <DATACLASS>

*/

