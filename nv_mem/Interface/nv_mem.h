//----------------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2004.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         COMMON_COMPONENT 
 Module         NV_MEM
 Description    interface definition of service NV_MEM
 Remarks

*/
//----------------------------------------------------------------------------------------------------------

#ifndef __SYSTEM_H__
  #error system.h must include before nv_mem.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before nv_mem.h
#endif

#ifdef __NV_MEM_H__
  #error nv_mem.h included more than once
#endif

#define __NV_MEM_H__

#define NV_MEM_VERSION (0x010200) // 1.2.0

#define TRIGGERTIME      2 // 2*500ms delay = 1000ms ~ 1s
#define CRCLOOPCNT       10 // number of bytes calculated be one crc-schedule for each file
#define NV_EXECUTE_TRIGGER 1

typedef enum _T_NV_STATE
{
    NV_WORKING,
    NV_READY
} T_NV_STATE;

// state of the file CRC-protection
typedef enum _T_CRC_STATE
{
    CRC_PROTECTED,
    CRC_UNPROTECTED_UPDATING,
    CRC_UNPROTECTED,
    CRC_NOT_INIT,
    CRC_ERROR
} T_CRC_STATE;

typedef enum _NV_DIAGNOSIS
{
    NV_OK,
    NV_WARNING,
    NV_ERROR    // both communication not possible or both pages defect, invalid or not formated
} T_NV_DIAGNOSIS;

typedef enum _NV_DIAGNOSIS_REPLACE
{
    REPLACE_OK,
    REPLACE_ERROR,
    REPLACE_DISABLED
} T_NV_DIAGNOSIS_REPLACE;

typedef enum _NV_REPLACE_DIRECTION
{
    REPLACE_DIRECTION_OK = 0,
    REPLACE_DIRECTION_INVALID_FLAG = 1,
    REPLACE_DIRECTION_INVALID_DB_REVISION = 2,
    REPLACE_DIRECTION_INVALID_COMM_TYPE = 4,
    REPLACE_DIRECTION_INVALID_GENERIC = 8
} T_NV_REPLACE_DIRECTION;


typedef enum _NV_MODE
{
    NV_ASYNC,
    NV_WAITING,
    NV_SYNC
} T_NV_MODE;

// prototypes
T_NV_DIAGNOSIS InitializeNV(void);
T_NV_DIAGNOSIS GetInitializationDiagnosisNV(void);
void ClearInitializationDiagnosisNV(void);
T_NV_STATE     GetStateNV(void);
T_NV_DIAGNOSIS GetNvDiagnosisNV(TINT16 fileIndex);
void ClearDiagnosisNV(TINT16 fileIndex);
T_NV_DIAGNOSIS Save(TINT16 fileIndex, T_NV_MODE mode);
T_NV_DIAGNOSIS Format(TINT16 fileIndex, T_NV_MODE mode);
T_NV_DIAGNOSIS Load(TINT16 fileIndex, T_NV_MODE mode);
void CheckNV(void);
void ExecuteNV(void);
TUSIGN16 PutDataNV(const TUSIGN8 FAST* ptrDestAdr, const TUSIGN8 SLOW* ptrSrcAdr, TUSIGN16 length);
TUSIGN16 GetDataNV(TUSIGN8 FAST* ptrDestAdr, const TUSIGN8 FAST*ptrSrcAdr, TUSIGN16 length);
T_CRC_STATE GetCRCStateNV(TINT16 fileIndex);
T_CRC_STATE CheckRamNV(void);
T_NV_DIAGNOSIS_REPLACE InitializeNV_REPLACE(void);
T_NV_DIAGNOSIS_REPLACE InitializeNV_STACK_REPLACE(void);
TBOOL CheckWriteActive(void);
TBOOL CheckWriteActive_REPLACE(void);

enum _FILE_INDEX
{
	ALL_FILES=-1,
	FIDX_CYCLIC_COMMON_REPLACE,
	FIDX_STATIC_COMMON_REPLACE,
	FIDX_CYCLIC_CB,
	FIDX_CALIBRATION,
	FIDX_STATIC_CB,
	NUM_OF_FILES
};

extern T_NV_DIAGNOSIS diagnosisResultInitialization;
extern T_NV_DIAGNOSIS_REPLACE diagnosisCommonResultInitialization;
extern T_NV_DIAGNOSIS_REPLACE diagnosisUncommonResultInitialization;
extern T_NV_REPLACE_DIRECTION directionCommonResultInitialization;
extern T_NV_REPLACE_DIRECTION directionUncommonResultInitialization;
extern TUSIGN8 nvExecuteTrigger;
extern TUSIGN8 nvExecuteIndex;
extern TBOOL cyclicCorruptionCommon;
extern TBOOL cyclicCorruptionUncommon;

#define PutData_NV_MEM PutDataNV
#define GetData_NV_MEM GetDataNV
