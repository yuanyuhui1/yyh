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


#ifndef __FEBUS_H__
#define __FEBUS_H__

#define FESVR_SEG 0xFF
//ADD BY ZUOCHEN  

#define RW_RETRY (20)
#define RW_WATI_TICK (60)

#define READ_CR_BASE (80)
#define READ_SR_BASE (62)

#define READ_CR_SVR (61)
#define READ_SR_SVR (60)

#define WRITE_CR_BASE (80+0X80)
#define WRITE_SR_BASE (62+0X80)

#define WRITE_CR_SVR (61+0X80)
#define WRITE_SR_SVR (60+0X80)

#define PutData_NV_MEM PutDataNV
#define GetData_NV_MEM GetDataNV

#ifdef MODULTEST_FEBUS

#define FIDX_STATIC_COMMON_REPLACE 0x00
#define FIDX_CYCLIC_COMMON_REPLACE 0x01
#define FIDX_NO_REPLACE 0x02

extern void FEBusModuleTest(void);
#endif

TUSIGN16  PutNvData_Replace (TUSIGN8 fIdx,TUSIGN8 sgIdx,const TUSIGN8* pBuf,TUSIGN8 dataLength);
TUSIGN16  GetNvData_Replace (TUSIGN8 fIdx,TUSIGN8 sgIdx,const TUSIGN8* pBuf,TUSIGN8 dataLength);
void ReadSegment_Replace(TUSIGN8 length,TUSIGN8* const pDataBuf);
void WriteSegment_Replace(TUSIGN8 length,TUSIGN8* const pDataBuf);
void SetFlagFEBus(TUSIGN8 flag);
#endif







