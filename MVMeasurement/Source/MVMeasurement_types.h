//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem MVMeasurement
 Module
 Description    Type defintions for subsystem attributes
                for Subsystems based on T_UNIT.
 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before MVMeasurement_types.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before MVMeasurement_types.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __MVMEASUREMENT_TYPES_H__
  #error MVMeasurement_types.h included more than once
#endif

#define __MVMEASUREMENT_TYPES_H__

// implement the required storage-classes

//@SubGen put@ <DATACLASS>
 //-------------------------------------------------------------------------------------------------
 //! structure of STATIC_FREQUENT_STATIC_DEFAULT block
 typedef struct _T_MVMEASUREMENT_STATIC_FREQUENT_STATIC_DEFAULT
 {
     //@SubGen start@    <DATACLASS STATIC_FREQUENT_STATIC_DEFAULT> 
    TUSIGN8 QnUnitCode;
    T_DEV_RNGFLT QnRange;
    TFLOAT QnDampingTime;
    TUSIGN8 QmUnitCode;
    T_DEV_RNGFLT QmRange;
    TFLOAT QmDampingTime;
    TUSIGN8 QpUnitCode;
    T_DEV_RNGFLT QpRange;
    TFLOAT QpDampingTime;
    T_DEV_RNGFLT QnPartialRange;
    TFLOAT QnPartialDampingTime;
    T_DEV_RNGFLT QvPartialRange;
    TFLOAT QvPartialDampingTime;
    TFLOAT heatCapacity;
    TFLOAT densityRef;
    TUSIGN8 gasRef;
    TUSIGN8 gasComputeType;
    TUSIGN8 gasDensitySelection;
    TFLOAT refTemperature;
     //@SubGen end@    <DATACLASS STATIC_FREQUENT_STATIC_DEFAULT> 
 } T_MVMEASUREMENT_STATIC_FREQUENT_STATIC_DEFAULT;


 //-------------------------------------------------------------------------------------------------
 //! structure of REPLACE_STATIC_FREQUENT_STATIC_DEFAULT block
 typedef struct _T_MVMEASUREMENT_REPLACE_STATIC_FREQUENT_STATIC_DEFAULT
 {
     //@SubGen start@    <DATACLASS REPLACE_STATIC_FREQUENT_STATIC_DEFAULT> 
    TUSIGN8 steamType;
    TUSIGN8 liquidCorrection;
    TFLOAT densityExpandBeta2;
    TFLOAT volumeExpandBeta1;
    TFLOAT calorificEnergy;
     //@SubGen end@    <DATACLASS REPLACE_STATIC_FREQUENT_STATIC_DEFAULT> 
 } T_MVMEASUREMENT_REPLACE_STATIC_FREQUENT_STATIC_DEFAULT;


 //-------------------------------------------------------------------------------------------------
 //! structure of CONSTANT block
 typedef struct _T_MVMEASUREMENT_CONSTANT
 {
     //@SubGen start@    <DATACLASS CONSTANT> 
    TUSIGN8 QnUnitType;
    TUSIGN8 QmUnitType;
    TUSIGN8 QpUnitType;
     //@SubGen end@    <DATACLASS CONSTANT> 
 } T_MVMEASUREMENT_CONSTANT;



 //-------------------------------------------------------------------------------------------------
 //! structure of Protected block
 typedef struct _T_MVMEASUREMENT_PROTECTED
 {
     //@SubGen start@    <DATACLASS Protected> 
    TUSIGN8 QnSimEnable;
    TUSIGN8 QmSimEnable;
    TUSIGN8 QpSimEnable;
    TUSIGN8 QnPartialSimEnable;
    TUSIGN8 QvPartialSimEnable;
    TUSIGN8 spline2dRequest;
    TUSIGN8 updateCFSRequest;
    TUSIGN8 updateCFPresetRequest;
    TUSIGN8 updateMaxDNRequest;
    TUSIGN8 protectedDataStatus;
     //@SubGen end@    <DATACLASS Protected> 
 } T_MVMEASUREMENT_PROTECTED;



 //-------------------------------------------------------------------------------------------------
 //! structure of DynamicDuplicated block
 typedef struct _T_MVMEASUREMENT_DYNAMIC_DUPLICATED
 {
     //@SubGen start@    <DATACLASS DynamicDuplicated> 
    TFLOAT Qn;
    TFLOAT QnDamped;
    TFLOAT QnMaxDN;
    TFLOAT QnMaxDNDuplicated;
    TFLOAT QnPercentage;
    TFLOAT Qm;
    TFLOAT QmDamped;
    TFLOAT QmMaxDN;
    TFLOAT QmMaxDNDuplicated;
    TFLOAT QmPercentage;
    TFLOAT Qp;
    TFLOAT QpDamped;
    TFLOAT QpMaxDN;
    TFLOAT QpMaxDNDuplicated;
    TFLOAT QpPercentage;
    TFLOAT QnPartial;
    TFLOAT QnPartialDamped;
    TFLOAT QnPartialMaxDN;
    TFLOAT QnPartialMaxDNDuplicated;
    TFLOAT QnPartialPercentage;
    TFLOAT QvPartial;
    TFLOAT QvPartialDamped;
    TFLOAT QvPartialMaxDN;
    TFLOAT QvPartialMaxDNDuplicated;
    TFLOAT QvPartialPercentage;
    TFLOAT compressFactorR;
    TUSIGN8 mvmAlarmSimulation;
    TUSIGN8 mvmDiagnosis;
    TFLOAT compressFactorS;
    TFLOAT Hw;
    TFLOAT Hc;
    TFLOAT QnPartialDuplicated;
    TFLOAT QnPartialPercentageDuplicated;
    TFLOAT QnPartialDampedDuplicated;
    TFLOAT QvPartialDuplicated;
    TFLOAT QvPartialPercentageDuplicated;
    TFLOAT QvPartialDampedDuplicated;
    TFLOAT QnDuplicated;
    TFLOAT QnPercentageDuplicated;
    TFLOAT QnDampedDuplicated;
    TFLOAT QmDuplicated;
    TFLOAT QmPercentageDuplicated;
    TFLOAT QmDampedDuplicated;
    TFLOAT QpDuplicated;
    TFLOAT QpPercentageDuplicated;
    TFLOAT QpDampedDuplicated;
    TFLOAT HwDuplicated;
    TFLOAT HcDuplicated;
    TFLOAT compressFactorRDuplicated;
    TFLOAT compressFactorSDuplicated;
    TUSIGN8 mvmAlarmSimulationDuplicated;
    TUSIGN8 mvmDiagnosisDuplicated;
     //@SubGen end@    <DATACLASS DynamicDuplicated> 
 } T_MVMEASUREMENT_DYNAMIC_DUPLICATED;


 //-------------------------------------------------------------------------------------------------
 //! structure of Unprotected block
 typedef struct _T_MVMEASUREMENT_UNPROTECTED
 {
     //@SubGen start@    <DATACLASS Unprotected> 
    TFLOAT QnSim;
    TUSIGN8 QnSimUnitCode;
    TFLOAT QMinDN;
    TFLOAT QnPerSim;
    TUSIGN8 QnClassification;
    TUSIGN8 QnStatus;
    TFLOAT QmSim;
    TUSIGN8 QmSimUnitCode;
    TFLOAT QmPerSim;
    TUSIGN8 QmClassification;
    TUSIGN8 QmStatus;
    TFLOAT QpSim;
    TUSIGN8 QpSimUnitCode;
    TFLOAT QpPerSim;
    TUSIGN8 QpClassification;
    TUSIGN8 QpStatus;
    TFLOAT QnPartialSim;
    TFLOAT QnPartialPerSim;
    TUSIGN8 QnPartialClassification;
    TUSIGN8 QnPartialStatus;
    TFLOAT QvPartialSim;
    TFLOAT QvPartialPerSim;
    TUSIGN8 QvPartialClassification;
    TUSIGN8 QvPartialStatus;
    TUSIGN8 steamStatus;
    TFLOAT HwPreset;
    TFLOAT HcPreset;
    TUSIGN16 protectedDataCrc;
     //@SubGen end@    <DATACLASS Unprotected> 
 } T_MVMEASUREMENT_UNPROTECTED;





 //-------------------------------------------------------------------------------------------------
 //! structure of STATIC_RARE block
 typedef struct _T_MVMEASUREMENT_STATIC_RARE
 {
     //@SubGen start@    <DATACLASS STATIC_RARE> 
    T_RANGE_FLT QAlmRange;
    TFLOAT densityS;
    TUSIGN8 gasConfFlag;
    TFLOAT molFractions[21];
    TFLOAT compressFactorArray[63];
    TFLOAT CFTempArray0;
    TFLOAT CFTempArray1;
    TFLOAT CFTempArray2;
    TFLOAT CFTempArray3;
    TFLOAT CFTempArray4;
    TFLOAT CFTempArray5;
    TFLOAT CFTempArray6;
    TFLOAT CFPressArray0;
    TFLOAT CFPressArray1;
    TFLOAT CFPressArray2;
    TFLOAT CFPressArray3;
    TFLOAT CFPressArray4;
    TFLOAT CFPressArray5;
    TFLOAT CFPressArray6;
    TFLOAT CFPressArray7;
    TFLOAT CFPressArray8;
    TFLOAT relativeDensity;
    TUSIGN8 relativeDensityRef;
    TFLOAT refCalorifcValue;
    TUSIGN8 calorificValueRef;
    TFLOAT refPressure;
    TFLOAT compressFactorPreset;
    TUSIGN8 matrixSize;
    TUSIGN8 steamPwrrFR;
     //@SubGen end@    <DATACLASS STATIC_RARE> 
 } T_MVMEASUREMENT_STATIC_RARE;


 

/*

This section defines templates for the SubSystem CodeGenerator.
Do not modify or remove this section

//@SubGen template start@ <DATACLASS>
//-------------------------------------------------------------------------------------------------
//! structure of Template block
typedef struct _T_MVMEASUREMENT_TEMPLATE
{
    //@SubGen start@    <DATACLASS Template> 
    //@SubGen end@    <DATACLASS Template> 
} T_MVMEASUREMENT_TEMPLATE;
//@SubGen template end@ <DATACLASS>

*/