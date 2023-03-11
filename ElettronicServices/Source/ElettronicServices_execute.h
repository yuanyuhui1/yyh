//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem ElettronicServices
 Module
 Description    Prototypes of execute methods
                for Subsystems based on T_UNIT

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before ElettronicServices_execute.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before ElettronicServices_execute.h
#endif

#ifndef __ELETTRONIC_SERVICES_H__
  #error  ElettronicServices.h must be included before ElettronicServices_execute.h
#endif

//............................................
//!  extend include list if neccessary

#ifdef __ELETTRONIC_SERVICES_EXECUTE_H__
  #error ElettronicServices_execute.h included more than once
#endif

#define __ELETTRONIC_SERVICES_EXECUTE_H__
#define esTime (GetTickCount_RTOS_TASK())

extern TBOOL esResetDevice;
extern TBOOL esResetToDefault;
extern TBOOL esLoadRomDefault;
extern TUSIGN32 esAutoZeroCount;

extern void ( * comRead_es[])(TUSIGN8 , TUSIGN8* const);
extern void ( * comWrite_es[])(TUSIGN8 , TUSIGN8* const);

//@SubGen start@<METHODS>
void ElettornicServicesEXE_ELETTRONICSERVICES(void);
void CyclicAsyncSaveSRV_ELETTRONICSERVICES(void);
void UpdateDiagnosis_ELETTRONICSERVICES(void);
void ExitSRV_ELETTRONICSERVICES(void);
void InizializeNvSRV_ELETTRONICSERVICES(void);
TUSIGN8 InitializeNvCheckSRV_ELETTRONICSERVICES(void);
void ResetToDefaultSRV_ELETTRONICSERVICES(void);
void LoadRomDefaultSRV_ELETTRONICSERVICES(void);
void DeviceResetSRV_ELETTRONICSERVICES(void);
void CheckCpuAsm_ELETTRONICSERVICES(void);
void CheckDataCrc_ELETTRONICSERVICES(void);
void CheckAutoZeroStatus_ELETTRONICSERVICES(void);
TBOOL CheckDecreasingVoltage_ELETTRONICSERVICES(void);
//@SubGen end@<METHODS>

#ifdef HART_COMMUNICATION
void RamCheck_ElectronicServices(TUSIGN8 mode);
void RomCheckInitCrc_ElectronicServices(void);
void RomCheckStartup_ElectronicServices(void);
#endif


TUSIGN16 SaveAsDefault_ElectronicServices(TINT16 subIdx);
void ButtonZeroReleaseFunction(void);
void ButtonSpanReleaseFunction(void);
void ADC1_Start_ElectronicServices(void);
void ADC2_Start_ElectronicServices(void);
void TMP112_ReadTemp_ElectronicServices(TFLOAT *pTempData);

typedef void (*RegAndInstructionFunc)(void);


