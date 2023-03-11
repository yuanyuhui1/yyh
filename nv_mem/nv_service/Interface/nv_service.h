//----------------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2004.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System        MiLe 2
 Module         NV_SERVICE
 Description    interface definition of subsystem NV_SERVICE part of NV_MEM
 <pre>
    This NV_SERVICE handles sericeinformations via two segments, one for main- and one for the backup-page.
    Implements teh NV_SERVICE methods Initialize(), SetPageValid() and SetPageInvalid()
    Additional methods are GetWriteCnt(), GetData() and WriteData()
 </pre>
 Remarks

*/
//----------------------------------------------------------------------------------------------------------


#ifndef __SYSTEM_H__
  #error system.h must include before nv_service.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must include before nv_service.h
#endif

#ifndef __CHIP_HANDLER_H__
  #error chip_handler.h must include before nv_service.h
#endif

#ifndef __NV_MEM_H__
  #error nv_mem.h must include before nv_service.h
#endif

#ifdef __NV_SERVICE_H__
  #error nv_service.h included more than once
#endif

#define __NV_SERVICE_H__

//----------------------------------------------------------------------------------------------------------
// the define below must be remarked for normal operating
//#define MODULTEST_NV_SERVICE



//----------------------------------------------------------------------------------------------------------
//! error-code
enum E_RESULT_NVS
{
    /* 0*/NVS_OK,
    /* 1*/NVS_ERROR,
    /* 2*/NVS_VIRGIN_NV,
    /* 3*/NVS_DATA_ERROR,
    /* 4*/NVS_FORCED_REPLACE,
    /* 5*/NVS_ALREADY_MARRIADGE
};

#define NVS_NUMOFPAGES  2  					//! two pages are supported
#define NVS_MAIN_PAGE   0  					//! the main-page
#define NVS_BACKUP_PAGE 1  					//! the backup-page
#define NVS_NUMOF_FILES (NUM_OF_FILES)	 	//! for each file there is a service segment

//! segment address of srvSeg for file0 inside a page
#define NVS_SERVICE_SEGNUM       (CH_SEGMENTS_PER_PAGE-1)
#define NVS_MAX_SERVICE_SEGNUM   (CH_MAX_SEGMENTS_PER_PAGE-1)
//! there is only one servcie segment per page
#define NVS_NUM_SRV_SEGS_PE_PAGE 1
//! the free usable data length in a service-segment
#define NVS_DATALENGTH (CH_SEG_NETTO_LENGTH-(sizeof(TUSIGN16)+sizeof(TUSIGN32)))


// prototypes
TUSIGN16 Initialize_NVSERVICE(void);
TBOOL    PageIsValid_NVSERVICE(TUSIGN8 page, TUSIGN8 file);
TUSIGN16 SetPageValid_NVSERVICE(TUSIGN8 page, TUSIGN8 file);
TUSIGN16 SetPageInvalid_NVSERVICE(TUSIGN8 page, TUSIGN8 file);
TUSIGN16 WriteData_NVSERVICE(TUSIGN8 page, TUSIGN8 file, TUSIGN8 length, const void *ptrData, TBOOL formatSegment);
TUSIGN16 GetData_NVSERVICE(TUSIGN8 page, TUSIGN8 file, TUSIGN8 length, void *ptrData);
TUSIGN16 GetWriteCnt_NVSERVICE(TUSIGN8 page, TUSIGN8 file, TUSIGN32 *pWrtCnt);
TUSIGN16 Format_NVSERVICE(TINT16 fileIndex);
TUSIGN16 Initialize_NVSERVICE_REPLACE(TUSIGN8 type,T_NV_REPLACE_DIRECTION*direction);
TUSIGN16 WriteData_NVSERVICE_REPLACE(TUSIGN8 file, TUSIGN8 length, const void FAST* ptrData);
TUSIGN16 WritePage_NVSERVICE_REPLACE(TUSIGN8 feature);
TUSIGN16 SetPageValid_NVSERVICE_REPLACE(TUSIGN8 file);
TUSIGN16 SetPageInvalid_NVSERVICE_REPLACE(TUSIGN8 file);
TBOOL    CheckData_NVSERVICE_REPLACE(TUSIGN8 file);
