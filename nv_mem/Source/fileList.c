//----------------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2004.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         common component
 Module         NV_MEM
 Description    file-list NV_MEM
 Remarks

*/
//----------------------------------------------------------------------------------------------------------
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "nv_mem/chip_handler/interface/chip_handler.h"
#include "nv_mem/Interface/nv_mem.h"
#include "nv_mem/File/Interface/File.h"


/*---------------------------------------------------------------------------------------------------------------------------------------
 begin file CYCLIC_COMMON_REPLACE
---------------------------------------------------------------------------------------------------------------------------------------*/
#define FEATURES_CYCLIC_COMMON_REPLACE         (BUFFERED+REPLACED+COMMON)
#define NUMBEROF_SEGMENS_CYCLIC_COMMON_REPLACE 4  // 2 + 2
#define NVIDX_FIRST_SEG_CYCLIC_COMMON_REPLACE  0

#pragma location="SEGMENTS_CYCLIC_COMMON_REPLACE"
__no_init TUSIGN8 segmentsCYCLIC_COMMON_REPLACE[NUMBEROF_SEGMENS_CYCLIC_COMMON_REPLACE][CH_SEG_NETTO_LENGTH];

#if (FEATURES_CYCLIC_COMMON_REPLACE & BUFFERED)!=0
#pragma location="BUFFER_CYCLIC_COMMON_REPLACE"
__no_init TUSIGN8 bufferCYCLIC_COMMON_REPLACE[NUMBEROF_SEGMENS_CYCLIC_COMMON_REPLACE][CH_SEG_NETTO_LENGTH];
#endif

FAST TUSIGN8 mainDirtyFlagsCYCLIC_COMMON_REPLACE[(NUMBEROF_SEGMENS_CYCLIC_COMMON_REPLACE+7)/8];
FAST TUSIGN8 backupDirtyFlagsCYCLIC_COMMON_REPLACE[(NUMBEROF_SEGMENS_CYCLIC_COMMON_REPLACE+7)/8];
FAST TUSIGN8 replaceDirtyFlagsCYCLIC_COMMON_REPLACE[(NUMBEROF_SEGMENS_CYCLIC_COMMON_REPLACE+7)/8];
FAST T_FILE_CRC crcCYCLIC_COMMON_REPLACE;
FAST T_FILE_DYNAMICS dynamicsCYCLIC_COMMON_REPLACE;
FAST TUSIGN8 marriedCheckCYCLIC_COMMON_REPLACE;

SLOW const T_FILE fileCYCLIC_COMMON_REPLACE = 
{
    mainDirtyFlagsCYCLIC_COMMON_REPLACE,
    backupDirtyFlagsCYCLIC_COMMON_REPLACE,
    replaceDirtyFlagsCYCLIC_COMMON_REPLACE,
    #if (FEATURES_CYCLIC_COMMON_REPLACE & BUFFERED)!=0
    &(bufferCYCLIC_COMMON_REPLACE[0][0]),
    &(bufferCYCLIC_COMMON_REPLACE[NUMBEROF_SEGMENS_CYCLIC_COMMON_REPLACE-1][CH_SEG_NETTO_LENGTH-1]), 
    #else
    &(segmentsCYCLIC_COMMON_REPLACE[0][0]),
    &(segmentsCYCLIC_COMMON_REPLACE[NUMBEROF_SEGMENS_CYCLIC_COMMON_REPLACE-1][CH_SEG_NETTO_LENGTH-1]), 
    #endif
    &(segmentsCYCLIC_COMMON_REPLACE[0][0]),
    &(segmentsCYCLIC_COMMON_REPLACE[NUMBEROF_SEGMENS_CYCLIC_COMMON_REPLACE-1][CH_SEG_NETTO_LENGTH-1]), 
    sizeof(segmentsCYCLIC_COMMON_REPLACE), 
    NUMBEROF_SEGMENS_CYCLIC_COMMON_REPLACE,
    NVIDX_FIRST_SEG_CYCLIC_COMMON_REPLACE,    
    &crcCYCLIC_COMMON_REPLACE,
    &dynamicsCYCLIC_COMMON_REPLACE,
    FEATURES_CYCLIC_COMMON_REPLACE,
    (TUSIGN8)FIDX_CYCLIC_COMMON_REPLACE,
    &marriedCheckCYCLIC_COMMON_REPLACE,

    InitializeFILE,
    GetStateFILE,
    GetCRCStateFILE,
    GetNvDiagnosisFILE,
    ClearDiagnosisFILE,
    PutDataFILE,
    SaveFILE,
    LoadFILE,
    ExecuteFILE,
    CheckRamFILE,
    CheckAndRepairFILE,
    SetFileToDirtyFILE,
    ForcedReplaceFILE,
    CheckAndRepairReplaceFILE,
    GetNvDiagnosisReplaceFILE,
    SetFileToDirtyReplaceFILE,
};



/*---------------------------------------------------------------------------------------------------------------------------------------
 begin file STATIC_COMMON_REPLACE
---------------------------------------------------------------------------------------------------------------------------------------*/
#define FEATURES_STATIC_COMMON_REPLACE         (AUTOMATIC+PROTECTED+REPLACED+COMMON)
#define NUMBEROF_SEGMENS_STATIC_COMMON_REPLACE 38  // 19 + 19
#define NVIDX_FIRST_SEG_STATIC_COMMON_REPLACE  (NVIDX_FIRST_SEG_CYCLIC_COMMON_REPLACE+NUMBEROF_SEGMENS_CYCLIC_COMMON_REPLACE)

#pragma location="SEGMENTS_STATIC_COMMON_REPLACE"
__no_init TUSIGN8 segmentsSTATIC_COMMON_REPLACE[NUMBEROF_SEGMENS_STATIC_COMMON_REPLACE][CH_SEG_NETTO_LENGTH];

#if (FEATURES_STATIC_COMMON_REPLACE & BUFFERED)!=0
#pragma location="BUFFER_STATIC_COMMON_REPLACE"
__no_init TUSIGN8 bufferSTATIC_COMMON_REPLACE[NUMBEROF_SEGMENS_STATIC_COMMON_REPLACE][CH_SEG_NETTO_LENGTH];
#endif

FAST TUSIGN8 mainDirtyFlagsSTATIC_COMMON_REPLACE[(NUMBEROF_SEGMENS_STATIC_COMMON_REPLACE+7)/8];
FAST TUSIGN8 backupDirtyFlagsSTATIC_COMMON_REPLACE[(NUMBEROF_SEGMENS_STATIC_COMMON_REPLACE+7)/8];
FAST TUSIGN8 replaceDirtyFlagsSTATIC_COMMON_REPLACE[(NUMBEROF_SEGMENS_STATIC_COMMON_REPLACE+7)/8];
FAST T_FILE_CRC crcSTATIC_COMMON_REPLACE;
FAST T_FILE_DYNAMICS dynamicsSTATIC_COMMON_REPLACE;
FAST TUSIGN8 marriedCheckSTATIC_COMMON_REPLACE;

SLOW const T_FILE fileSTATIC_COMMON_REPLACE = 
{
    mainDirtyFlagsSTATIC_COMMON_REPLACE,
    backupDirtyFlagsSTATIC_COMMON_REPLACE,
    replaceDirtyFlagsSTATIC_COMMON_REPLACE,
    #if (FEATURES_STATIC_COMMON_REPLACE & BUFFERED)!=0
    &(bufferSTATIC_COMMON_REPLACE[0][0]),
    &(bufferSTATIC_COMMON_REPLACE[NUMBEROF_SEGMENS_STATIC_COMMON_REPLACE-1][CH_SEG_NETTO_LENGTH-1]), 
    #else
    &(segmentsSTATIC_COMMON_REPLACE[0][0]),
    &(segmentsSTATIC_COMMON_REPLACE[NUMBEROF_SEGMENS_STATIC_COMMON_REPLACE-1][CH_SEG_NETTO_LENGTH-1]), 
    #endif
    &(segmentsSTATIC_COMMON_REPLACE[0][0]),
    &(segmentsSTATIC_COMMON_REPLACE[NUMBEROF_SEGMENS_STATIC_COMMON_REPLACE-1][CH_SEG_NETTO_LENGTH-1]), 
    sizeof(segmentsSTATIC_COMMON_REPLACE), 
    NUMBEROF_SEGMENS_STATIC_COMMON_REPLACE,
    NVIDX_FIRST_SEG_STATIC_COMMON_REPLACE,    
    &crcSTATIC_COMMON_REPLACE,
    &dynamicsSTATIC_COMMON_REPLACE,
    FEATURES_STATIC_COMMON_REPLACE,
    (TUSIGN8)FIDX_STATIC_COMMON_REPLACE,
	&marriedCheckSTATIC_COMMON_REPLACE,

    InitializeFILE,
    GetStateFILE,
    GetCRCStateFILE,
    GetNvDiagnosisFILE,
    ClearDiagnosisFILE,
    PutDataFILE,
    SaveFILE,
    LoadFILE,
    ExecuteFILE,
    CheckRamFILE,
    CheckAndRepairFILE,
    SetFileToDirtyFILE,
    ForcedReplaceFILE,
    CheckAndRepairReplaceFILE,
    GetNvDiagnosisReplaceFILE,
    SetFileToDirtyReplaceFILE,
};


/*---------------------------------------------------------------------------------------------------------------------------------------
 begin file CYCLIC_UNCOMMON_REPLACE
---------------------------------------------------------------------------------------------------------------------------------------*/
#define FEATURES_CYCLIC_CB         (BUFFERED)
#define NUMBEROF_SEGMENS_CYCLIC_CB 56  // 28 + 28
#define NVIDX_FIRST_SEG_CYCLIC_CB  (NVIDX_FIRST_SEG_STATIC_COMMON_REPLACE+NUMBEROF_SEGMENS_STATIC_COMMON_REPLACE)

#pragma location="SEGMENTS_CYCLIC_CB"
__no_init TUSIGN8 segmentsCYCLIC_CB[NUMBEROF_SEGMENS_CYCLIC_CB][CH_SEG_NETTO_LENGTH];

#if (FEATURES_CYCLIC_CB & BUFFERED)!=0
#pragma location="BUFFER_CYCLIC_CB"
__no_init TUSIGN8 bufferCYCLIC_CB[NUMBEROF_SEGMENS_CYCLIC_CB][CH_SEG_NETTO_LENGTH];
#endif

FAST TUSIGN8 mainDirtyFlagsCYCLIC_CB[(NUMBEROF_SEGMENS_CYCLIC_CB+7)/8];
FAST TUSIGN8 backupDirtyFlagsCYCLIC_CB[(NUMBEROF_SEGMENS_CYCLIC_CB+7)/8];
FAST TUSIGN8 replaceDirtyFlagsCYCLIC_CB[(NUMBEROF_SEGMENS_CYCLIC_CB+7)/8];
FAST T_FILE_CRC crcCYCLIC_CB;
FAST T_FILE_DYNAMICS dynamicsCYCLIC_CB;
FAST TUSIGN8 marriedCheckCYCLIC_CB;

SLOW const T_FILE fileCYCLIC_CB = 
{
    mainDirtyFlagsCYCLIC_CB,
    backupDirtyFlagsCYCLIC_CB,
    replaceDirtyFlagsCYCLIC_CB,
    #if (FEATURES_CYCLIC_CB & BUFFERED)!=0
    &(bufferCYCLIC_CB[0][0]),
    &(bufferCYCLIC_CB[NUMBEROF_SEGMENS_CYCLIC_CB-1][CH_SEG_NETTO_LENGTH-1]), 
    #else
    &(segmentsCYCLIC_CB[0][0]),
    &(segmentsCYCLIC_CB[NUMBEROF_SEGMENS_CYCLIC_CB-1][CH_SEG_NETTO_LENGTH-1]), 
    #endif
    &(segmentsCYCLIC_CB[0][0]),
    &(segmentsCYCLIC_CB[NUMBEROF_SEGMENS_CYCLIC_CB-1][CH_SEG_NETTO_LENGTH-1]), 
    sizeof(segmentsCYCLIC_CB), 
    NUMBEROF_SEGMENS_CYCLIC_CB,
    NVIDX_FIRST_SEG_CYCLIC_CB,    
    &crcCYCLIC_CB,
    &dynamicsCYCLIC_CB,
    FEATURES_CYCLIC_CB,
    (TUSIGN8)FIDX_CYCLIC_CB,
	&marriedCheckCYCLIC_CB,

    InitializeFILE,
    GetStateFILE,
    GetCRCStateFILE,
    GetNvDiagnosisFILE,
    ClearDiagnosisFILE,
    PutDataFILE,
    SaveFILE,
    LoadFILE,
    ExecuteFILE,
    CheckRamFILE,
    CheckAndRepairFILE,
    SetFileToDirtyFILE,
    ForcedReplaceFILE,
    CheckAndRepairReplaceFILE,
    GetNvDiagnosisReplaceFILE,
    SetFileToDirtyReplaceFILE,
};


/*---------------------------------------------------------------------------------------------------------------------------------------
 begin file CALIBRATION
---------------------------------------------------------------------------------------------------------------------------------------*/
#define FEATURES_CALIBRATION         (AUTOMATIC+PROTECTED)
#define NUMBEROF_SEGMENS_CALIBRATION   8  // 4 + 4
#define NVIDX_FIRST_SEG_CALIBRATION  (NVIDX_FIRST_SEG_CYCLIC_CB+NUMBEROF_SEGMENS_CYCLIC_CB)

#pragma location="SEGMENTS_CALIBRATION"
__no_init TUSIGN8 segmentsCALIBRATION[NUMBEROF_SEGMENS_CALIBRATION][CH_SEG_NETTO_LENGTH];

#if (FEATURES_CALIBRATION & BUFFERED)!=0
#pragma location="BUFFER_CALIBRATION"
__no_init TUSIGN8 bufferCALIBRATION[NUMBEROF_SEGMENS_CALIBRATION][CH_SEG_NETTO_LENGTH];
#endif

FAST TUSIGN8 mainDirtyFlagsCALIBRATION[(NUMBEROF_SEGMENS_CALIBRATION+7)/8];
FAST TUSIGN8 backupDirtyFlagsCALIBRATION[(NUMBEROF_SEGMENS_CALIBRATION+7)/8];
FAST TUSIGN8 replaceDirtyFlagsCALIBRATION[(NUMBEROF_SEGMENS_CALIBRATION+7)/8];
FAST T_FILE_CRC crcCALIBRATION;
FAST T_FILE_DYNAMICS dynamicsCALIBRATION;
FAST TUSIGN8 marriedCheckCALIBRATION;

SLOW const T_FILE fileCALIBRATION =
{
    mainDirtyFlagsCALIBRATION,
    backupDirtyFlagsCALIBRATION,
    replaceDirtyFlagsCALIBRATION,
    #if (FEATURES_CALIBRATION & BUFFERED)!=0
    &(bufferCALIBRATION[0][0]),
    &(bufferCALIBRATION[NUMBEROF_SEGMENS_CALIBRATION-1][CH_SEG_NETTO_LENGTH-1]),
    #else
    &(segmentsCALIBRATION[0][0]),
    &(segmentsCALIBRATION[NUMBEROF_SEGMENS_CALIBRATION-1][CH_SEG_NETTO_LENGTH-1]),
    #endif
    &(segmentsCALIBRATION[0][0]),
    &(segmentsCALIBRATION[NUMBEROF_SEGMENS_CALIBRATION-1][CH_SEG_NETTO_LENGTH-1]),
    sizeof(segmentsCALIBRATION),
    NUMBEROF_SEGMENS_CALIBRATION,
    NVIDX_FIRST_SEG_CALIBRATION,
    &crcCALIBRATION,
    &dynamicsCALIBRATION,
    FEATURES_CALIBRATION,
    (TUSIGN8)FIDX_CALIBRATION,
	&marriedCheckCALIBRATION,

    InitializeFILE,
    GetStateFILE,
    GetCRCStateFILE,
    GetNvDiagnosisFILE,
    ClearDiagnosisFILE,
    PutDataFILE,
    SaveFILE,
    LoadFILE,
    ExecuteFILE,
    CheckRamFILE,
    CheckAndRepairFILE,
    SetFileToDirtyFILE,
    ForcedReplaceFILE,
    CheckAndRepairReplaceFILE,
    GetNvDiagnosisReplaceFILE,
    SetFileToDirtyReplaceFILE,
};

/*---------------------------------------------------------------------------------------------------------------------------------------
 begin file CB
---------------------------------------------------------------------------------------------------------------------------------------*/
#define FEATURES_STATIC_CB         (AUTOMATIC+PROTECTED)
#define NUMBEROF_SEGMENS_STATIC_CB 130  // 65 + 65 

#define NVIDX_FIRST_SEG_STATIC_CB  (NVIDX_FIRST_SEG_CALIBRATION+NUMBEROF_SEGMENS_CALIBRATION)

#pragma location="SEGMENTS_STATIC_CB"
__no_init TUSIGN8 segmentsSTATIC_CB[NUMBEROF_SEGMENS_STATIC_CB][CH_SEG_NETTO_LENGTH];

#if (FEATURES_STATIC_CB & BUFFERED)!=0
#pragma location="BUFFER_STATIC_CB"
__no_init TUSIGN8 bufferSTATIC_CB[NUMBEROF_SEGMENS_STATIC_CB][CH_SEG_NETTO_LENGTH];
#endif

FAST TUSIGN8 mainDirtyFlagsSTATIC_CB[(NUMBEROF_SEGMENS_STATIC_CB+7)/8];
FAST TUSIGN8 backupDirtyFlagsSTATIC_CB[(NUMBEROF_SEGMENS_STATIC_CB+7)/8];
FAST TUSIGN8 replaceDirtyFlagsSTATIC_CB[(NUMBEROF_SEGMENS_STATIC_CB+7)/8];
FAST T_FILE_CRC crcSTATIC_CB;
FAST T_FILE_DYNAMICS dynamicsSTATIC_CB;
FAST TUSIGN8 marriedCheckSTATIC_CB;

SLOW const T_FILE fileSTATIC_CB = 
{
    mainDirtyFlagsSTATIC_CB,
    backupDirtyFlagsSTATIC_CB,
    replaceDirtyFlagsSTATIC_CB,
    #if (FEATURES_STATIC_CB & BUFFERED)!=0
    &(bufferSTATIC_CB[0][0]),
    &(bufferSTATIC_CB[NUMBEROF_SEGMENS_STATIC_CB-1][CH_SEG_NETTO_LENGTH-1]), 
    #else
    &(segmentsSTATIC_CB[0][0]),
    &(segmentsSTATIC_CB[NUMBEROF_SEGMENS_STATIC_CB-1][CH_SEG_NETTO_LENGTH-1]), 
    #endif
    &(segmentsSTATIC_CB[0][0]),
    &(segmentsSTATIC_CB[NUMBEROF_SEGMENS_STATIC_CB-1][CH_SEG_NETTO_LENGTH-1]), 
    sizeof(segmentsSTATIC_CB), 
    NUMBEROF_SEGMENS_STATIC_CB,
    NVIDX_FIRST_SEG_STATIC_CB,    
    &crcSTATIC_CB,
    &dynamicsSTATIC_CB,
    FEATURES_STATIC_CB,
    (TUSIGN8)FIDX_STATIC_CB,
	&marriedCheckSTATIC_CB,
	
    InitializeFILE,
    GetStateFILE,
    GetCRCStateFILE,
    GetNvDiagnosisFILE,
    ClearDiagnosisFILE,
    PutDataFILE,
    SaveFILE,
    LoadFILE,
    ExecuteFILE,
    CheckRamFILE,
    CheckAndRepairFILE,
    SetFileToDirtyFILE,
    ForcedReplaceFILE,
    CheckAndRepairReplaceFILE,
    GetNvDiagnosisReplaceFILE,
    SetFileToDirtyReplaceFILE,
};

SLOW const T_FILE SLOW *const fileList[NUM_OF_FILES]=
{
	&fileCYCLIC_COMMON_REPLACE,
	&fileSTATIC_COMMON_REPLACE,
	&fileCYCLIC_CB,
	&fileCALIBRATION,
	&fileSTATIC_CB,
};

