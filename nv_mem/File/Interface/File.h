//----------------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2004.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         COMMON_COMPONENT
 Module         NV_MEM.File
 Description    interface definition of class file
 Remarks

*/
//----------------------------------------------------------------------------------------------------------

#ifndef __SYSTEM_H__
  #error system.h must include before file.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before file.h
#endif

#ifndef __NV_MEM_H__
  #error nv_mem.h must be included before file.h
#endif

#ifndef __CHIP_HANDLER_H__
  #error chip_handler.h must be included before file.h
#endif

#ifdef __FILE_H__
  #error file.h included more than once
#endif

#define __FILE_H__
#define FILE_RETRY_CNT 3

//#define USE_RAMPAGE_FOR_REPAIR 1
#ifndef USE_RAMPAGE_FOR_REPAIR
  #define USE_RAMPAGE_FOR_REPAIR 0
#endif

// structure for ram-data used by CRC-protection
typedef struct _T_FILE_CRC
{
    TUSIGN16 reference;
    TUSIGN16 newCRC;
    TUSIGN16 byteIndex;
    T_CRC_STATE state;
}T_FILE_CRC;

// structure for the dynamic attributes of a file
typedef struct _T_FILE_DYNAMICS
{
    TUSIGN16 saveTimer;
    TUSIGN16 saveTimerReplace;
    TBOOL saveRequested;
    TBOOL loadRequested;
    T_NV_DIAGNOSIS nvDiagnosis;
    T_NV_DIAGNOSIS_REPLACE nvDiagnosisReplace;
    T_NV_STATE     nvState;
} T_FILE_DYNAMICS;

// file feature
#define AUTOMATIC 0x80
#define BUFFERED  0x40
#define PROTECTED 0x20
#define REPLACED 0x10
#define COMMON 0x08
#define UNCOMMON 0x04
#define STACK_FILE 0x02 //Special case of uncommon

extern TBOOL forceStackChangeCommType;

typedef struct _T_FILE
{
    // private attributes
    TUSIGN8    FAST* mainDirtyFlags;
    TUSIGN8    FAST* backupDirtyFlags;
    TUSIGN8    FAST* replaceDirtyFlags;
    TUSIGN8    FAST* startAdrBuffer;  // first Address inside buffer
    TUSIGN8    FAST* endAdrBuffer;    // last address inside buffer
    TUSIGN8    FAST* startAdrNvRam;
    TUSIGN8    FAST* endAdrNvRam;
    TUSIGN16   length;
    TUSIGN16   numberOfSegments;
    TUSIGN16   nvIdxFirstSeg;    
    T_FILE_CRC FAST* pFileCRC;
    T_FILE_DYNAMICS FAST* pDynamics;
    TUSIGN8    features;
    TUSIGN8    fileIndex;
    TUSIGN8    FAST* marriedCheck;

    // public methods
    void (*Initialize)(const struct _T_FILE SLOW* me);
    T_NV_STATE (*GetState)(const struct _T_FILE SLOW* me);
    T_CRC_STATE (*GetCRCState)(const struct _T_FILE SLOW* me);
    T_NV_DIAGNOSIS (*GetNvDiagnosis)(const struct _T_FILE SLOW* me);
    void (*ClearDiagnosis)(const struct _T_FILE SLOW* me);
    T_NV_DIAGNOSIS (*PutData)(const struct _T_FILE SLOW* me,
                              const TUSIGN8 FAST* ptrDestAdr, 
                              const TUSIGN8 SLOW* ptrSrcAdr, 
                              TUSIGN16 length); 
    void (*Save)(const struct _T_FILE SLOW* me); 
    void (*Load)(const struct _T_FILE SLOW* me);
    T_NV_STATE (*Execute)(const struct _T_FILE SLOW* me);
    T_CRC_STATE (*CheckRam)(const struct _T_FILE SLOW* me);
    T_NV_DIAGNOSIS (*CheckAndRepair)(const struct _T_FILE SLOW* me);
    void (*SetFileToDirty)(const struct _T_FILE SLOW* me);
    T_NV_DIAGNOSIS_REPLACE (*ForcedReplace)(const struct _T_FILE SLOW* me);
    T_NV_DIAGNOSIS_REPLACE (*CheckAndRepairReplace)(const struct _T_FILE SLOW* me);
    T_NV_DIAGNOSIS_REPLACE (*GetNvDiagnosisReplace)(const struct _T_FILE SLOW* me);
    void (*SetFileToDirtyReplace)(const struct _T_FILE SLOW* me);

} T_FILE;

// prototypes
void InitializeFILE(const struct _T_FILE SLOW* me);
T_NV_STATE GetStateFILE(const struct _T_FILE SLOW* me);
T_CRC_STATE GetCRCStateFILE(const struct _T_FILE SLOW* me);
T_NV_DIAGNOSIS GetNvDiagnosisFILE(const struct _T_FILE SLOW* me); 
void ClearDiagnosisFILE(const struct _T_FILE SLOW* me); 
T_NV_DIAGNOSIS PutDataFILE(const struct _T_FILE SLOW* me,
                           const TUSIGN8 FAST* ptrDestAdr, 
                           const TUSIGN8 SLOW* ptrSrcAdr, 
                           TUSIGN16 length);
void SaveFILE(const struct _T_FILE SLOW* me);
void LoadFILE(const struct _T_FILE SLOW* me);
T_NV_STATE ExecuteFILE(const struct _T_FILE SLOW* me);
T_CRC_STATE CheckRamFILE(const struct _T_FILE SLOW* me);
T_NV_DIAGNOSIS CheckAndRepairFILE(const T_FILE SLOW* me);
void SetFileToDirtyFILE(const struct _T_FILE SLOW* me);
T_NV_DIAGNOSIS_REPLACE ForcedReplaceFILE(const T_FILE SLOW* me);
T_NV_DIAGNOSIS_REPLACE CheckAndRepairReplaceFILE(const T_FILE SLOW* me);
T_NV_DIAGNOSIS_REPLACE GetNvDiagnosisReplaceFILE(const struct _T_FILE SLOW* me); 
void SetFileToDirtyReplaceFILE(const struct _T_FILE SLOW* me);
