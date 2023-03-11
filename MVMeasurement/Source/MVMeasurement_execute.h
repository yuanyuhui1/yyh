//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem MVMeasurement
 Module
 Description    Prototypes of execute methods
                for Subsystems based on T_UNIT

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before MVMeasurement_execute.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before MVMeasurement_execute.h
#endif

#ifndef __MVMEASUREMENT_H__
  #error  MVMeasurement.h must be included before MVMeasurement_execute.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __MVMEASUREMENT_EXECUTE_H__
  #error MVMeasurement_execute.h included more than once
#endif

#define __MVMEASUREMENT_EXECUTE_H__

#define MVM_SET_DIAG(name)           SetDiagnosis_MVM(MVM_ALM_##name##)
#define MVM_CLR_DIAG(name)           ClearDiagnosis_MVM(MVM_ALM_##name##)
#define MVM_OBJECT_LOCAL_DIAGNOSIS    mVMeasurementDynamicDuplicated.mvmDiagnosis
#define MVM_OBJECT_LOCAL_DIAGNOSISDUPLICATED    mVMeasurementDynamicDuplicated.mvmDiagnosisDuplicated


//@SubGen start@<METHODS>
TFLOAT CalculateGasQnEXE_MVM(TFLOAT volumeFlow, TFLOAT tReal, TFLOAT pReal);
TFLOAT CalculateGasQnPartialEXE_MVM(TFLOAT inNVF, TFLOAT biogasPerc);
TFLOAT CalculateGasQvPartialEXE_MVM(TFLOAT inVF, TFLOAT biogasPerc);
TFLOAT CalculateGasQmEXE_MVM(TFLOAT inVF, TFLOAT inNVF, TFLOAT densityReal);
TFLOAT CalculateGasQpEXE_MVM(TFLOAT inVF);
TFLOAT CalculateLiquidQnEXE_MVM(TFLOAT inVF, TFLOAT tReal);
TFLOAT CalculateLiquidQmEXE_MVM(TFLOAT inVF, TFLOAT DensityReal, TFLOAT tReal);
TFLOAT CalculateLiquidQpEXE_MVM(TFLOAT inVF, TFLOAT T1Real, TFLOAT T2Real);
TFLOAT CalculateSteamQmEXE_MVM(TFLOAT inVF, TFLOAT DensityReal);
TFLOAT CalculateSteamQpEXE_MVM(TFLOAT inMF);
void UpdateDiagnosisEXE_MVM(void);
TFLOAT CalcSteamDensityEXE_MVM(TFLOAT inTReal, TFLOAT inPReal);
void CalcSteamHEXE_MVM(TFLOAT inPReal, TFLOAT inT1Real, TFLOAT inT2Real);
void CalcCompressFactor_MVM(void);
void CheckDataCrcSRV_MVM(void);
TFLOAT GetQnPercentageSRV_MVM(void);
TFLOAT GetQmPercentageSRV_MVM(void);
TFLOAT GetQpPercentageSRV_MVM(void);
TFLOAT GetQnPartialPercentageSRV_MVM(void);
TFLOAT GetQvPartialPercentageSRV_MVM(void);
void UpdateLiquidQnMaxDN_MVM(void);
void UpdateGasQnMaxDN_MVM(void);
void UpdateLiquidPowerMaxDN_MVM(void);
void UpdateGasPowerMaxDN_MVM(void);
void UpdateSteamPowerMaxDN_MVM(void);
void UpdateQmMaxDN_MVM(void);
void UpdateBiagasQvMaxDN_MVM(void);
void UpdateQAlarmEXE_MVM(TFLOAT perc);
//@SubGen end@<METHODS>
void UpdateMaxDN(void);
void UpdateGasStandardCompressFactor(void);
void UpdateGasCompressFactor(void);
void UpdateGasISO12212_Molar_CNS(void);
void UpdateSteamCoefficient(void);
void QmAlarmProcess(void);