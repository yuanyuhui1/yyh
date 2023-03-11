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


#include <intrinsics.h>
#include <string.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "System/Interface/microcontroller.h"
#include "Common/Interface/common_functions.h"
#include "System/Interface/assert.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "Coordinator/Interface/Coordinator.h"
#include "Coordinator/Interface/Coordinator_idx.h"
#include "nv_mem/chip_handler/interface/chip_handler.h"
#include "nv_mem/FEBus/FEBus.h"
#include "nv_mem/Interface/nv_mem.h"
#include "nv_mem/nv_service/Interface/nv_service.h"
#include "nv_mem/File/Interface/File.h"
//#include "..\..\..\coordinator\interface\nv_startup.h"


#define DISABLE_CONTEXTSWITCHING SuspendAll_RTOS_TASK()
#define ENABLE_CONTEXTSWITCHING  ResumeAll_RTOS_TASK()


#define REPLACE_RETRY 10
#ifdef HART_COMMUNICATION
#define REPLACE_TRIGGER 1
#else
#define REPLACE_TRIGGER 15
#endif

static TBOOL resetDiagnosisReplace = eTRUE;
TBOOL forceStackChangeCommType = eFALSE;

SLOW const T_FILE_CRC crcInitValues = { 0, 0, 0, CRC_NOT_INIT};
SLOW const T_FILE_DYNAMICS dynInitValues = { 0, 0, eFALSE, eFALSE, NV_OK, REPLACE_OK ,NV_READY};


//lint -e641  zuochen wang. 
//lint -e522  zuochen wang. 

enum _REPAIR_CODES
{
    NVR_OK,
    NVR_MAIN_WITH_BACKUP_REPAIR,
    #if (USE_RAMPAGE_FOR_REPAIR==1)
    NVR_MAIN_WITH_RAMPAGE_REPAIR,
    #endif
    NVR_BACKUP_REPAIR,
    NVR_DATA_DEFECT
};

// to reduce to stack consumption, the buffers are stored global
static FAST TUSIGN8   mMainSegBuf[CH_SEG_NETTO_LENGTH];
static FAST TUSIGN8   mBackupSegBuf[CH_SEG_NETTO_LENGTH];

static T_NV_DIAGNOSIS_REPLACE FileToNV_REPLACE(const T_FILE SLOW*me,TUSIGN8 srcPage);
static void SetAllSegClean(TUSIGN8 FAST* pDirtyFlags, TUSIGN16 length);

//-------------------------------------------------------------------------------------------------
/*!
 \brief  Initializes the File-Handler
 \author Heiko Kresse
 \date   2007-12-10
 \param  me; pointer to the objects attributes
 \return void
 \warning
 \test
 test-date: 2009-03-17
 \n by: Heiko Kresse
 \n environment: PC7501-Emulator + High-performance workbench
 \n intention: complete instruction-coverage
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//-------------------------------------------------------------------------------------------------
void InitializeFILE(const struct _T_FILE SLOW* me)
{
    TUSIGN16 length;

    VIP_ASSERT(me);

    // initialize dynamic variables with default values
    *me->pDynamics = dynInitValues;
    *me->pFileCRC  = crcInitValues;

    //-------------------------------------------------------------------------
    // clear dirtyflags
    length=(me->numberOfSegments+7)/8;

    //lint -e{534} Heiko Kresse 2007-12-06 ignoring the return value is OK
    memset(me->mainDirtyFlags,0,length);
    //lint -e{534} Heiko Kresse 2007-12-06 ignoring the return value is OK
    memset(me->backupDirtyFlags,0,length);
    //lint -e{534} Robreto Colombo 2009-10-10 ignoring the return value is OK
    memset(me->replaceDirtyFlags,0,length);

    //-------------------------------------------------------------------------
    // clear buffer
    length = me->length;

    //lint -e{534} Heiko Kresse 2007-12-06 ignoring the return value is OK
    memset(me->startAdrNvRam, 0, length);

    // if nv-shadow and buffer are not the same then
    if( me->features & BUFFERED )
    {
        //lint -e{534} Heiko Kresse 2007-12-06 ignoring the return value is OK
        memset(me->startAdrBuffer,  0, length);
    }

    //-------------------------------------------------------------------------
    // clear marriedCheck flag
    *me->marriedCheck = eFALSE;
}


//-------------------------------------------------------------------------------------------------
/*!
 \brief  Get actual file-state
 \author Heiko Kresse
 \date   2007-12-10
 \param  me; pointer to the objects attributes
 \return file-state ==NV_WORKING : Loading,saving or check    ==NV_READY: no operation in progress
 \warning
 \test
 test-date: 2009-03-09
 \n by: Heiko Kresse
 \n environment: code review
 \n intention:
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//-------------------------------------------------------------------------------------------------
//lint --e{960}
T_NV_STATE GetStateFILE(const struct _T_FILE SLOW* me)
{
    VIP_ASSERT(me);

    if( me->pDynamics->loadRequested || me->pDynamics->saveRequested )
        return NV_WORKING;
    else
        return me->pDynamics->nvState;
}


//-------------------------------------------------------------------------------------------------
/*!
 \brief  Get actuale state of the NV-Shadow (RAM)
 \author Heiko Kresse
 \date   2007-12-10
 \param  me; pointer to the objects attributes
 \return ramDiagnosis
 <pre>
    NV_NOT_INIT: shadow not initialized
    NV_OK : No error recognized yet
    NV_ERROR: crc of the ram-shadow is corrupted
 </pre>
 \warning
 \test
 test-date: 2009-03-09
 \n by: Heiko Kresse
 \n environment:
 \n intention: code review
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//-------------------------------------------------------------------------------------------------
T_CRC_STATE GetCRCStateFILE(const struct _T_FILE SLOW* me)
{
    VIP_ASSERT(me);

    return me->pFileCRC->state;
}


//-------------------------------------------------------------------------------------------------
/*!
 \brief  Get actual state of the nv-memory
 \author Heiko Kresse
 \date   2007-12-10
 \param  me; pointer to the objects attributes
 \return nv-Diagnosis
 <pre>
    NV_OK        : OK
    NV_NOT_INIT  : not initialized
    NV_WARNING   : main repaired with backup; maybe last data are lost
    NV_ERROR     : main could not be repaired
    NV_HARDWAREDEFECT : EEPROM could not be connected, maybe problems with I2C
 </pre>
 \warning
 \test
 test-date: 2009-03-09
 \n by: Heiko Kresse
 \n environment:
 \n intention: code review
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//-------------------------------------------------------------------------------------------------
T_NV_DIAGNOSIS GetNvDiagnosisFILE(const struct _T_FILE SLOW* me)
{
    VIP_ASSERT(me);

    return me->pDynamics->nvDiagnosis;
}

//-------------------------------------------------------------------------------------------------
/*!
 \brief  private methode; resets the CRC-state machine
 \author Heiko Kresse
 \date   2007-12-10
 \param  me; pointer to the objects attributes
 \return void
 \warning
 \test
 test-date: 2009-03-17
 \n by: Heiko Kresse
 \n environment: PC7501-Emulator + High-performance workbench
 \n intention: complete instruction-coverage
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//-------------------------------------------------------------------------------------------------
static void ResetCRC(const T_FILE SLOW* me)
{
    T_FILE_CRC FAST*pCrc;

    pCrc=me->pFileCRC;

    // there is no way to leave ERROR-state
    if( pCrc->state!=CRC_ERROR )
    {
        pCrc->state=CRC_UNPROTECTED;

        if( me->features & PROTECTED )
        {
            pCrc->newCRC=0xFFFF;
            pCrc->byteIndex=0;
        }
    }
}

//-------------------------------------------------------------------------------------------------
/*!
 \brief  resets ram and nv-diagnosis to the initialize-state
 \author Heiko Kresse
 \date   2007-12-10
 \param  me; pointer to the objects attributes
 \return void
 \warning it is possible to declare a corrupted shadow to a valid one.
          That must be possible for formating the NV but is dangorous to do so in
          normal operation!
 \test
 test-date: 2009-03-09
 \n by: Heiko Kresse
 \n environment:
 \n intention: code review
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//-------------------------------------------------------------------------------------------------
void ClearDiagnosisFILE(const struct _T_FILE SLOW* me)
{
    VIP_ASSERT(me);
    VIP_ASSERT(me->pDynamics);
    VIP_ASSERT(me->pFileCRC);

    me->pDynamics->nvDiagnosis=NV_OK;
    me->pFileCRC->state=CRC_NOT_INIT;
}

//-------------------------------------------------------------------------------------------------
/*!
 \brief  Use this methode to write data into the nv-shadow. It handles dirtyflags and CRC
 \author Heiko Kresse
 \date   2007-12-10
 \param  me; pointer to the objects attributes
 \param  ptrDestAdr; destination address only near memory is allowed
 \param  ptrDestAdr; source address; FAR ROM is allowed
 \param  length; in byte
 \return Error-Code: NV_ERROR --> Destination-range not completly inside File-Area
                     NV_OK --> data was copied into shadow
 \warning
 \test
 test-date: 2009-03-17
 \n by: Heiko Kresse
 \n environment: PC7501-Emulator + High-performance workbench
 \n intention: complete instruction-coverage + code review
 \n result module test: module-test OK
                        the module-test does not check the case that the resulting end-address
                        leaves 64K. The code-review shows ( pStart>pEnd ....) that this case
                        will be handeld as "Target range not within file-range"
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//-------------------------------------------------------------------------------------------------
T_NV_DIAGNOSIS PutDataFILE(const struct _T_FILE SLOW* me,
                           const TUSIGN8 FAST* ptrDestAdr, 
                           const TUSIGN8 SLOW* ptrSrcAdr, 
                           TUSIGN16 length)
{
    TUSIGN8 FAST*pStart, FAST*pEnd;
    TUSIGN16 segIdx, endSeg;

    VIP_ASSERT(me);
    VIP_ASSERT(ptrDestAdr);

    pStart=(TUSIGN8 FAST*)ptrDestAdr;
    pEnd=pStart+length-1;
	
    //check if the target-range exceeds file address range
	//lint -e{946}
    if( pStart>pEnd || pStart<me->startAdrBuffer || pEnd>me->endAdrBuffer)
    {
        return NV_ERROR;
    }
    else 
    {
        // restart CRC-calculation
        ResetCRC(me);

        // transfer data
        //lint -e{619} Heiko Kresse 2007-12-04 lint does not know that const causes a FAR-pointer
        //lint -e{534} Heiko Kresse 2007-12-04 ignoring the return value is OK
        memcpy(pStart,(void const*)ptrSrcAdr,length);

        // set dirtyflags for accessed segments, but not for buffered files
        if( !(me->features & BUFFERED) )
        {
            segIdx = (TUSIGN16)(pStart-me->startAdrBuffer);
            endSeg = (segIdx + length-1)/CH_SEG_NETTO_LENGTH;
            segIdx/=CH_SEG_NETTO_LENGTH;

            do
            {
                me->mainDirtyFlags[segIdx>>3] |= (1<<(segIdx&7)); // <<3 and bitTab[] have the same speed

		 		if(me->features & REPLACED) me->replaceDirtyFlags[segIdx>>3] |= (1<<(segIdx&7));

                segIdx++;
            }
            while(segIdx<=endSeg);
        }

        // start savetimer for automatic-files
        if( me->features & AUTOMATIC )
        {
            me->pDynamics->saveTimer=TRIGGERTIME;
	     if( me->features & REPLACED)
	     { 
	         me->pDynamics->saveTimerReplace = REPLACE_TRIGGER;
	     }
        }

        return NV_OK;
    }
}


//-------------------------------------------------------------------------------------------------
/*!
 \brief  request save operation; execute starts save next time execute will be scheduled
 \author Heiko Kresse
 \date   2007-12-10
 \param  me; pointer to the objects attributes
 \return void
 \warning
 \test
 test-date: 2009-03-09
 \n by: Heiko Kresse
 \n environment:
 \n intention: code review
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//-------------------------------------------------------------------------------------------------
void SaveFILE(const struct _T_FILE SLOW* me)
{
    VIP_ASSERT(me);

    me->pDynamics->saveRequested=eTRUE;
    me->pDynamics->saveTimer=0;
}


//-------------------------------------------------------------------------------------------------
/*!
 \brief  request load operation; execute starts save next time execute will be scheduled
 \author Heiko Kresse
 \date   2007-12-10
 \param  me; pointer to the objects attributes
 \return void
 \warning
 \test
 test-date: 2009-03-09
 \n by: Heiko Kresse
 \n environment:
 \n intention: code review
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//-------------------------------------------------------------------------------------------------
void LoadFILE(const struct _T_FILE SLOW* me)
{
    VIP_ASSERT(me);

    me->pDynamics->loadRequested=eTRUE;
}

// realisiert den Automaten innerhalb von VALID
//-------------------------------------------------------------------------------------------------
/*!
 \brief  Execute-Methode for CRC-Calculation. It realizes the statemachine inside the state VALID
 \author Heiko Kresse
 \date   2007-12-10
 \param  me; pointer to the objects attributes
 \return crc-state
 <pre>
   CRC_NOT_INIT:    actually there are no valid data in the shadow
   CRC_UNPROTECTED: after changing data inside the shadow, the CRC is not valid, the shadow is not
                    protected by a CRC
   CRC_PROTECTED: the CRC of the shadow is calculated continously and compared with the refrence-CRC
   CRC_ERROR:     shadow was corrupted; CRC != reference-CRC
 </pre>
 \warning
 \test
 test-date: 2009-03-17
 \n by: Heiko Kresse
 \n environment: PC7501-Emulator + High-performance workbench
 \n intention: complete instruction-coverage
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//-------------------------------------------------------------------------------------------------
T_CRC_STATE CheckRamFILE(const struct _T_FILE SLOW* me)
{
    TUSIGN8  FAST*pData;
    T_FILE_CRC FAST*pCrc;
    TUSIGN16 u, idx;

    VIP_ASSERT(me);

    pCrc  = me->pFileCRC;

    // first of all, is it a protected file?
    if( me->features & PROTECTED )
    {
        // data are valid?
        if( pCrc->state==CRC_UNPROTECTED || pCrc->state==CRC_PROTECTED || pCrc->state==CRC_UNPROTECTED_UPDATING )
        {

	     	if (pCrc->state == CRC_UNPROTECTED) 
            {
                pCrc->state = CRC_UNPROTECTED_UPDATING;
                // reset loop
                pCrc->byteIndex = 0;
                pCrc->newCRC=0xFFFF;
            }
		 
            pData = me->startAdrNvRam;    
            idx   = pCrc->byteIndex;

            for( u=0; u<CRCLOOPCNT && idx<me->length; u++, idx++)
            {
                pCrc->newCRC=CalcCRC16_COMMON(pData[idx],pCrc->newCRC);
            }

            // CRC-calculation ready?
            if( idx>=me->length )
            {
                //lint -e{961} Heiko Kresse 2007-12-04 else not necessary
                if( pCrc->state==CRC_UNPROTECTED_UPDATING )
                {
                    pCrc->reference = pCrc->newCRC;
                    pCrc->state = CRC_PROTECTED;
                }
                else if(pCrc->reference!=pCrc->newCRC)
                {
                    pCrc->state=CRC_ERROR;
                }

                pCrc->byteIndex=0;
                pCrc->newCRC=0xFFFF;
            }
            else
            {
                pCrc->byteIndex=idx;
            }
        }
    }

    return pCrc->state; 
}

//-------------------------------------------------------------------------------------------------
/*!
 \brief  privat method; Write shadow into nv-memory (main or backup).
         writes only dirty segments and set dirty-flag for backup. exit when error
 \author Heiko Kresse
 \date   2007-12-10
 \param  me; pointer to the objects attributes
 \param  page; CH_MAIN_PAGE or CH_BACKUP_PAGE
 \return diagnosis; NV_OK or NV_ERROR
 \warning
 \test
 test-date: 2009-03-17
 \n by: Heiko Kresse
 \n environment: PC7501-Emulator + High-performance workbench
 \n intention: complete instruction-coverage + code review
 \n result module test: OK
    module-test OK (+ module-test SaveFile() = OK)
    the module-test consists of testing FileToNV and SaveFile!
    The handling PutData during FileToNV will not be tested by the Module-tests.
    The fileDirty-loop (code review) is OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//-------------------------------------------------------------------------------------------------
static T_NV_DIAGNOSIS FileToNV(const T_FILE SLOW*me, TUSIGN8 page)
{
    TUSIGN8 fileDirty;
    T_NV_DIAGNOSIS result;
    TUSIGN16 sIdx, dIdx;
    TUSIGN8  mask;
    TUSIGN32 coordinatorDiagnosis;
    TUSIGN16 diagResult;

	if( NVS_OK!=SetPageInvalid_NVSERVICE(page,me->fileIndex) )
    {
        me->pDynamics->nvDiagnosis = NV_ERROR;
    }
    else
    {
        result=NV_OK;

        // repeat until all dirty-flags are cleared; break if error occures
		do
		{
            fileDirty=eFALSE;

            for(sIdx=0; sIdx<me->numberOfSegments && result==NV_OK; sIdx++)
            {
                mask = (1<<(sIdx&7));
                dIdx = sIdx>>3;
               
                if( me->mainDirtyFlags[dIdx] & mask )
                {    //lint --e{679}
					if( CH_OK!=WriteData_CHIPHANDLER(page,(TUSIGN16)(sIdx+me->nvIdxFirstSeg),&(me->startAdrNvRam[sIdx*CH_SEG_NETTO_LENGTH])) )
                    {
                        result=NV_ERROR;
                    }
                    else
                    {
                        // clear dirty-flag and mark segment for backup-operation
                        me->mainDirtyFlags[dIdx]&= ~mask;
                        me->backupDirtyFlags[dIdx]|= mask;
                    }

                    fileDirty=eTRUE;
                }
            }
        }while( result==NV_OK && fileDirty);


        // set page valid only if page is OK
        if( result!=NV_OK )
        {
            me->pDynamics->nvDiagnosis=NV_ERROR;
        }
		else if( NVS_OK!=SetPageValid_NVSERVICE(page,me->fileIndex) )
        {
            me->pDynamics->nvDiagnosis=NV_ERROR;
        }
        else
        {
            me->pDynamics->nvDiagnosis=NV_OK;
        }
    }

	//diagnois ignore. because  coordinator do not have the objects need updating
	diagResult = coordinator_Get(CDT_IDX_cdtDiagnosis,WHOLE_OBJECT,&coordinatorDiagnosis);

	if(diagResult == OK)
	{
		if(me->pDynamics->nvDiagnosis == NV_ERROR)
		{
			coordinatorDiagnosis |= ((TUSIGN32)1<<CDT_ALARM_CB_NV_ERROR);
		}
		else
		{
			coordinatorDiagnosis &= ~((TUSIGN32)1<<CDT_ALARM_CB_NV_ERROR);
		}
        //lint -e{522}
		coordinator_Put(CDT_IDX_cdtDiagnosis,WHOLE_OBJECT,&coordinatorDiagnosis);
	}

    return me->pDynamics->nvDiagnosis;
}


//-------------------------------------------------------------------------------------------------
/*!
 \brief  privat method; copies main to backup (without using shadow).
         copies only dirty segments, exit when error
 \author Heiko Kresse
 \date   2007-12-10
 \param  me; pointer to the objects attributes
 \return diagnosis; NV_OK or NV_ERROR
 \warning
 \test
 test-date: 2009-03-17
 \n by: Heiko Kresse
 \n environment: PC7501-Emulator + High-performance workbench
 \n intention: complete instruction-coverage
 \n result module test: OK
    module-test OK (+ module-test SaveFile() = OK)
    the module-test consists of testing CpyPageToPage and SaveFile!
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//-------------------------------------------------------------------------------------------------
static T_NV_DIAGNOSIS CpyPageToPage(const T_FILE SLOW*me, TUSIGN8 dstPage, TUSIGN8 srcPage)
{
    T_NV_DIAGNOSIS result;
    TUSIGN16 sIdx, dIdx;
    TUSIGN8  mask;
    TUSIGN8  buffer[CH_SEG_NETTO_LENGTH];

    // only a valid source-page should be copied to target
    if( !PageIsValid_NVSERVICE(srcPage,me->fileIndex) )
    {
        me->pDynamics->nvDiagnosis=NV_ERROR;
    }
    else if( NVS_OK!=SetPageInvalid_NVSERVICE(dstPage,me->fileIndex) )
    {
        me->pDynamics->nvDiagnosis=NV_ERROR;
    }
    else
    {
        result=NV_OK;

        for(sIdx=0; sIdx<me->numberOfSegments && result==NV_OK; sIdx++)
        {
            mask = (1<<(sIdx&7));
            dIdx = sIdx>>3;

            // copy only segments that have changed
            if( me->backupDirtyFlags[dIdx] & mask )
            {
                if( CH_OK!=ReadData_CHIPHANDLER(srcPage,
                                                (TUSIGN8)(sIdx+me->nvIdxFirstSeg),
                                                buffer ) )
                {
                    result=NV_ERROR;
                }
                else if( CH_OK!=WriteData_CHIPHANDLER(dstPage,
                                                      (TUSIGN8)(sIdx+me->nvIdxFirstSeg),
                                                      buffer ) )
                {
                    result=NV_ERROR;
                }
                else
                {
                    me->backupDirtyFlags[dIdx]&=~mask;
                }
            }
        }

        // set backup valid if page is OK
        if( result!=NV_OK )
        {
            me->pDynamics->nvDiagnosis=NV_ERROR;
        }
        else if( NVS_OK!=SetPageValid_NVSERVICE(dstPage, me->fileIndex) )
        {
            me->pDynamics->nvDiagnosis=NV_ERROR;
        }
        else
        {
            me->pDynamics->nvDiagnosis=NV_OK;
        }
    }

    return me->pDynamics->nvDiagnosis;
}

//-------------------------------------------------------------------------------------------------
/*!
 \brief  private method; Set all segments of the file for main or backup to dirty
 \author Heiko Kresse
 \date   2007-12-10
 \param  pDirtyFlags; address of the dirty-flag array
 \param  length; number of segments
 \return void
 \warning
 \test
 test-date: 2009-03-17
 \n by: Heiko Kresse
 \n environment: PC7501-Emulator + High-performance workbench
 \n intention: complete instruction-coverage
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//-------------------------------------------------------------------------------------------------
static void SetAllSegDirty(TUSIGN8 FAST* pDirtyFlags, TUSIGN16 length)
{
    TUSIGN16 sIdx, dIdx;
    TUSIGN8  mask;

    VIP_ASSERT(pDirtyFlags);

    // Because not used bits shall remain zero, the flags are handled bitwise instead
    // of setting the bytes to 0xFF!
    mask=1;
    dIdx=0;
    for( sIdx=0; sIdx<length; sIdx++)
    {
        pDirtyFlags[dIdx]|=mask;
        mask=(TUSIGN8)(mask<<1);
        if( !mask )
        {
            mask=1;
            dIdx++;
        }
    }
}


//-------------------------------------------------------------------------------------------------
/*!
 \brief  Set all segments of the file for to dirty (used by format)
 \author Heiko Kresse
 \date   2007-12-10
 \param  me
 \return void
 \warning
 \test
 test-date: 2009-03-09
 \n by: Heiko Kresse
 \n environment: PC7501-Emulator + High-performance workbench
 \n intention: code review
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//-------------------------------------------------------------------------------------------------
void SetFileToDirtyFILE(const struct _T_FILE SLOW* me)
{
    VIP_ASSERT(me);

    SetAllSegDirty(me->mainDirtyFlags,me->numberOfSegments);
}

//-------------------------------------------------------------------------------------------------
/*!
 \brief  private method; Save-File --> write all dirty-segments into nv-memory
 \author Heiko Kresse
 \date   2007-12-10
 \param  me; pointer to the objects attributes
 \return void
 \warning
 \test
 test-date: 2009-03-17
 \n by: Heiko Kresse
 \n environment: PC7501-Emulator + High-performance workbench
 \n intention: complete instruction-coverage + code review
 \n result module test: OK
        The module-test could not test if nvState will be NV_WORKING during the save operation.
        Code-Review OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//-------------------------------------------------------------------------------------------------
static void SaveFile(const T_FILE SLOW*me)
{
    T_NV_DIAGNOSIS result;
    TUSIGN16 loopCnt, retry;

    VIP_ASSERT(me);

    if( me->pDynamics->saveRequested && diagnosisResultInitialization == NV_OK )
    {
        // clear request
        me->pDynamics->saveTimer=0;
        me->pDynamics->nvState=NV_WORKING;
        me->pDynamics->saveRequested=eFALSE;

        // copy buffer into shadow, if the file is a buffered one
        if( me->features & BUFFERED )
        {
            // it is not allowed to write T_DATA_OBJ-objects in TASKs lower than NV
            DISABLE_CONTEXTSWITCHING;
            //lint -e{534} Heiko Kresse 2007-12-05 ignoring the return value is OK
            memcpy(me->startAdrNvRam, me->startAdrBuffer, me->length);
            ENABLE_CONTEXTSWITCHING;

            SetAllSegDirty(me->mainDirtyFlags, me->numberOfSegments);
	     	SetAllSegDirty(me->replaceDirtyFlags, me->numberOfSegments);
        }

        loopCnt=0;

        // copy File-memory into eeprom, error-->try some times again
        do
        {
            result=FileToNV(me,NVS_MAIN_PAGE);

            // in case of an addresserror any segment could be damaged
            // to be shure that at least the main-page is OK mark all 
            // segments as dirty, so all segments will be written and  
            // the page becomes OK
            if( result==NV_ERROR )
            {
                SetAllSegDirty(me->mainDirtyFlags, me->numberOfSegments);
            }
            else // copy main to backup
            {
                // an address-error could have damaged main so it is not possible
                // to retry this operation without repairing main in first place.
                // But then both pages are invalid for some time.
                // The better way is to write File into backup instead of main.
                // Thus reduces the chance to get two damaged pages.
                result=CpyPageToPage(me,CH_BACKUP_PAGE,CH_MAIN_PAGE);
                retry=1;

                while(result!=NV_OK && retry<FILE_RETRY_CNT)
                {
                    SetAllSegDirty(me->mainDirtyFlags, me->numberOfSegments);
                    result=FileToNV(me,NVS_BACKUP_PAGE);
                    retry++;
                }
            }
            loopCnt++;

        }while(loopCnt<FILE_RETRY_CNT && result==NV_ERROR);

        me->pDynamics->nvDiagnosis=result;
        me->pDynamics->nvState=NV_READY;

	if(me->pDynamics->nvDiagnosis != NV_OK)
	{	
		me->Save(me);
	}
	}	

    if(me->features & REPLACED && diagnosisResultInitialization == NV_OK)
	{
		 (void)FileToNV_REPLACE(me,CH_MAIN_PAGE);
	}

    if(diagnosisResultInitialization != NV_OK)
	{
		 me->pDynamics->nvDiagnosis = NV_ERROR;
	}
}

//-------------------------------------------------------------------------------------------------
/*!
 \brief  private method; load all segments of the file from nv-memory into shadow
 \author Heiko Kresse
 \date   2007-12-10
 \param  me; pointer to the objects attributes
 \param  loadMain; load from main-page when eTRUE else load from backup-page
 \return diagnosis; NV_OK or NV_ERROR
 \warning
 \test
 test-date: 2009-03-17
 \n by: Heiko Kresse
 \n environment: PC7501-Emulator + High-performance workbench
 \n intention: complete instruction-coverage
 \n result module test: OK
    module-test OK (+ module-test LoadFile() = OK)
    the module-test consists of testing LoadAllSegments and LoadFile!
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//-------------------------------------------------------------------------------------------------
static T_NV_DIAGNOSIS LoadAllSegments(const T_FILE SLOW*me, TUSIGN8 page)
{
    T_NV_DIAGNOSIS result;
    TUSIGN16 sIdx, length;

    for( sIdx=0, result=NV_OK; sIdx<me->numberOfSegments && result==NV_OK; sIdx++)
    {   //lint --e{679}
        if( CH_OK!=ReadData_CHIPHANDLER(page,
                                        (TUSIGN16)(sIdx+me->nvIdxFirstSeg),
                                        &(me->startAdrNvRam[sIdx*CH_SEG_NETTO_LENGTH])) )
        {
            result=NV_ERROR;
        }
    }

    if( result==NV_OK )
    {
        length=(me->numberOfSegments+7)/8;
        /*lint -e{534} Heiko Kresse 2007-12-18 ignoring return-value is ok*/
        memset(me->mainDirtyFlags,0,length);
        /*lint -e{534} Heiko Kresse 2007-12-18 ignoring return-value is ok*/
        memset(me->backupDirtyFlags,0,length);
    }

    return result;
}


//-------------------------------------------------------------------------------------------------
/*!
 \brief  private method; Load-File
 \author Heiko Kresse
 \date   2007-12-10
 \param  me; pointer to the objects attributes
 \return void
 \warning
 \test
 test-date: 2009-03-17
 \n by: Heiko Kresse
 \n environment: PC7501-Emulator + High-performance workbench
 \n intention: complete instruction-coverage + code review
 \n result module test: OK
        The module-test could not test if nvState will be NV_WORKING during the save operation.
        Code-Review OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//-------------------------------------------------------------------------------------------------
static void LoadFile(const T_FILE SLOW*me)
{
    T_NV_DIAGNOSIS diagnosis;

    if( me->pDynamics->loadRequested )
    {
        me->pDynamics->saveTimer=0;
        me->pDynamics->nvState=NV_WORKING;
        me->pDynamics->loadRequested=eFALSE;

        if( !PageIsValid_NVSERVICE(NVS_MAIN_PAGE,me->fileIndex) )
        {
            diagnosis=NV_ERROR;
        }
        else
        {
            diagnosis=LoadAllSegments(me,CH_MAIN_PAGE);
        }

        // if the operation was good, the crc-protection must be started
        if( diagnosis==NV_OK )
        {
            me->pDynamics->nvDiagnosis=NV_OK;
            // without NOT_INIT the load could not repair NV_ERROR
            me->pFileCRC->state=CRC_NOT_INIT; 
            ResetCRC(me);
        }
        // otherwise try to load backup
        else if( !PageIsValid_NVSERVICE(NVS_BACKUP_PAGE,me->fileIndex) )
        {
            me->pDynamics->nvDiagnosis=NV_ERROR;
            me->pFileCRC->state=CRC_ERROR;
        }
        // load valid backup-page, start crc-protection when OK
        else if( NV_OK==LoadAllSegments(me,CH_BACKUP_PAGE) )
        {
            me->pDynamics->nvDiagnosis=NV_WARNING;
            // without NOT_INIT the load could not repair NV_ERROR
            me->pFileCRC->state=CRC_NOT_INIT; 
            ResetCRC(me);
        }
        // EEPROM defect
        else
        {
            me->pDynamics->nvDiagnosis=NV_ERROR;
            me->pFileCRC->state=CRC_ERROR;
        }

        // for buffered pages the nv-shadow must be copied into the ram-buffer
        if( (me->features & BUFFERED) && (me->pDynamics->nvDiagnosis!=NV_ERROR) )
        {
            DISABLE_CONTEXTSWITCHING;
            //lint -e{534} Heiko Kresse 2007-12-05 ignoring the return value is OK
            memcpy(me->startAdrBuffer, me->startAdrNvRam, me->length);
            ENABLE_CONTEXTSWITCHING;
        }

        me->pDynamics->nvState=NV_READY;
    }
}

//-------------------------------------------------------------------------------------------------
/*!
 \brief  Execute-method for Loading, Saveing and Checking NV
 \author Heiko Kresse
 \date   2007-12-10
 \param  me; pointer to the objects attributes
 \return nvState ==NV_WORKING : Loading,saving or check    ==NV_READY: no operation in progress
 \warning
 \test
 test-date: 2009-03-17
 \n by: Heiko Kresse
 \n environment:
 \n intention: code review
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//-------------------------------------------------------------------------------------------------
T_NV_STATE ExecuteFILE(const struct _T_FILE SLOW* me)
{
    VIP_ASSERT(me);

    SaveFile(me);
    LoadFile(me);

    if( me->pDynamics->saveTimer>0 )
    {
        me->pDynamics->saveTimer--;

        if( me->pDynamics->saveTimer==0 )
        {
            me->Save(me);
        }
    }

    // in order to avoid corruption
    if(me->pDynamics->saveTimer > TRIGGERTIME)
    {
        me->pDynamics->saveTimer = TRIGGERTIME;
    }

    return me->GetState(me);
}


//----------------------------------------------------------------------------------------------------------
/*!
  \author Heiko Kresse
  \date   2008-01-04
  \brief  privat; check non volatile storage

  Check if all segment-checksums of one file
  IF both pages OK compare each segment of the pages. If both pages equal the NV is OK otherwise
  copy main to backup.
  IF backup is defect copy main to backup.
  If main is defect and the rampage is valid, save the complete rampage. If rampage is invalid copy
  backup to main.
  If both pages defect the nv-storage is defect.

  \param  me
  \return TUSIGN16 error-code
  <pre>
    this return-code defines the necessary repair mechanism
    NVR_OK                           no error
    NVR_MAIN_WITH_BACKUP_REPAIR      error in main --> copy backup to main for repair
    NVR_MAIN_WITH_RAMPAGE_REPAIR     error in main, rampage is valid --> save rampage again
    NVR_BACKUP_REPAIR                error in backup --> copy main to backup
    NVR_DATA_DEFECT                  error in both pages, repair not possible (without complete lost of data)
 </pre>
 \warning
 test-date: 2009-03-17
 \n by: Heiko Kresse
 \n environment: PC7501-Emulator + High-performance workbench
 \n intention: complete instruction-coverage
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug

*/
//----------------------------------------------------------------------------------------------------------
static TUSIGN16 CheckNv(const T_FILE SLOW* me)
{
    TBOOL    bolMainIsOK;
    TBOOL    bolBackupIsOK;
    TBOOL    bolMainEqualBackup;
    TUSIGN16 segNr;
    TUSIGN16 result;
    TUSIGN16 u;

    // check if pages are valid. Maybe a write-operation was interrupted, than the page written to is
    // marked as invalid.
    bolMainIsOK   = PageIsValid_NVSERVICE(NVS_MAIN_PAGE,me->fileIndex);
    bolBackupIsOK = PageIsValid_NVSERVICE(NVS_BACKUP_PAGE,me->fileIndex);

    // the pages are not eqaul if one page is not valid
    bolMainEqualBackup = (TBOOL)(bolMainIsOK && bolBackupIsOK);

    // the nv-storage is defect (or not formated) if both Pages are invalid
    if( !bolMainIsOK && !bolBackupIsOK )
    {
        result = NVR_DATA_DEFECT;
    }
    else
    {
        // check if both pages ar OK and if they are equal
        for( segNr=(TUSIGN8)me->nvIdxFirstSeg;
             segNr<me->nvIdxFirstSeg+me->numberOfSegments && (bolMainIsOK || bolBackupIsOK);
             segNr++)
        {
            // check main-page as long as no error is recognized
            if( bolMainIsOK )
            {
                if( CH_OK!=ReadData_CHIPHANDLER(CH_MAIN_PAGE, segNr, mMainSegBuf) )
                {
                    bolMainIsOK = eFALSE;
                    bolMainEqualBackup = eFALSE;
                }
            }

            // check backup-page as long as no error is recognized
            if( bolBackupIsOK )
            {
                if( CH_OK!=ReadData_CHIPHANDLER(CH_BACKUP_PAGE, segNr, mBackupSegBuf) )
                {
                    bolBackupIsOK = eFALSE;
                    bolMainEqualBackup = eFALSE;
                }
            }

            // compare main with backup until one byte is not equal
            for(u=0; bolMainEqualBackup && u<sizeof(mMainSegBuf); u++)
            {
                bolMainEqualBackup = (TBOOL)(mMainSegBuf[u]==mBackupSegBuf[u]);
            }
        }

        // map the state of both pages to result-code
        if( bolMainIsOK )
        {
            if( bolBackupIsOK && bolMainEqualBackup )
            {
                result = NVR_OK;
            }
            else
            {
                result = NVR_BACKUP_REPAIR;
            }
        }
        #if (USE_RAMPAGE_FOR_REPAIR==1)
        else if( me->pFileCRC->state==CRC_UNPROTECTED || me->pFileCRC->state==CRC_PROTECTED )
        {
            result = NVR_MAIN_WITH_RAMPAGE_REPAIR;
        }
        #endif
        else if( bolBackupIsOK )
        {
            result = NVR_MAIN_WITH_BACKUP_REPAIR;
        }
        else
        {
            result = NVR_DATA_DEFECT;
        }
    }

    return result;
}


//----------------------------------------------------------------------------------------------------------
/*!
  \brief  Check and Repair File; first check main agains backup, decide what to and repair if necessary
  \author Heiko Kresse
  \date   2008-01-04
  \param  me
  \return T_NV_DIAGNOSIS
    NV_OK,
    NV_NOT_INIT,
    NV_WARNING,
    NV_ERROR,         // both pages defect, invalid or not formated
    NV_HARDWAREDEFECT
 \warning
 test-date: 2009-03-17
 \n by: Heiko Kresse
 \n environment: PC7501-Emulator + High-performance workbench
 \n intention: complete instruction-coverage
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug

*/
//----------------------------------------------------------------------------------------------------------
T_NV_DIAGNOSIS CheckAndRepairFILE(const T_FILE SLOW* me)
{
    T_NV_DIAGNOSIS result;
    TUSIGN16       retry;

    VIP_ASSERT(me);

    switch(CheckNv(me))
    {
        case NVR_OK:
            me->pDynamics->nvDiagnosis=NV_OK;
            break;

        case NVR_MAIN_WITH_BACKUP_REPAIR: 
            SetAllSegDirty(me->backupDirtyFlags, me->numberOfSegments);
            retry=0;
            do
            {
                result=CpyPageToPage(me,CH_MAIN_PAGE,CH_BACKUP_PAGE);
                retry++;
            }
            while(result!=NV_OK && retry<FILE_RETRY_CNT);
            if( result==NV_OK )
            {
                me->pDynamics->nvDiagnosis=NV_WARNING;
            }
            break;

        #if (USE_RAMPAGE_FOR_REPAIR==1)
        case NVR_MAIN_WITH_RAMPAGE_REPAIR:
            SetAllSegDirty(me->mainDirtyFlags, me->numberOfSegments);
            retry=0;
            do
            {
                result=FileToNV(me,NVS_MAIN_PAGE);
                retry++;
            }
            while(result!=NV_OK && retry<FILE_RETRY_CNT);
            break;
        #endif

        case NVR_BACKUP_REPAIR:  
            SetAllSegDirty(me->backupDirtyFlags, me->numberOfSegments);
            retry=0;
            do
            {
                result=CpyPageToPage(me,CH_BACKUP_PAGE,CH_MAIN_PAGE);
                retry++;
            }
            while(result!=NV_OK && retry<FILE_RETRY_CNT);
            break;

        default:
        case NVR_DATA_DEFECT:             
            me->pDynamics->nvDiagnosis=NV_ERROR;
            break;
    }

    return me->pDynamics->nvDiagnosis;
}


//-------------------------------------------------------------------------------------------------
/*!
 \brief  	Set all file replace segments to dirty
 \author	Roberto Colombo
 \date   	2009-03-26
 \param  	me
 \return 	void
 \warning
 \test
 test-date:
 \n by:
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3: OK
 \bug
*/
//-------------------------------------------------------------------------------------------------
void SetFileToDirtyReplaceFILE(const struct _T_FILE SLOW* me)
{
	VIP_ASSERT(me);

	SetAllSegDirty(me->replaceDirtyFlags,me->numberOfSegments);
}


//----------------------------------------------------------------------------------------------------------
/*!
  \brief  	Repair ram with nv fe data
  		Called into chech and repair replace

  \author 	Roberto Colombo
  \date   	2008-03-26
  \param  me
  \return
  		T_NV_DIAGNOSIS_REPLACE
    		REPLACE_OK
    		REPLACE_DISABLED
 \warning
 test-date:
 \n by:
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3: OK
 \bug
*/
//----------------------------------------------------------------------------------------------------------
static T_NV_DIAGNOSIS_REPLACE StartupRepairRamWithFe_REPLACE(const T_FILE SLOW* me)
{
	T_NV_DIAGNOSIS_REPLACE result;
	TUSIGN16 replaceResult;
	TUSIGN16 retryCount = 0;
	TUSIGN16 sIdx;

	do
	{
		result = REPLACE_OK;

		for(sIdx=0; sIdx<me->numberOfSegments && result!=REPLACE_DISABLED;sIdx++)
		{
		  //lint -e{734,679}
			replaceResult = GetNvData_Replace(me->fileIndex,sIdx,(TUSIGN8*)&(me->startAdrBuffer[sIdx*CH_SEG_NETTO_LENGTH]),CH_SEG_NETTO_LENGTH);
			if(replaceResult != OK)
			{
				result = REPLACE_DISABLED;
			}
		}

		retryCount++;	
	}
	while(result == REPLACE_DISABLED && retryCount<REPLACE_RETRY);

	if(result == REPLACE_OK)
	{
		SetAllSegClean(me->replaceDirtyFlags,me->numberOfSegments);
	}

   return result;
}


//----------------------------------------------------------------------------------------------------------
/*!
  \brief  	Repair nv fe data with ram
  		Called into chech and repair replace

  \author 	Roberto Colombo
  \date   	2008-03-26
  \param  me
  \return
  		T_NV_DIAGNOSIS_REPLACE
    		REPLACE_OK
    		REPLACE_DISABLED
 \warning
 test-date:
 \n by:
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3: OK
 \bug
*/
//----------------------------------------------------------------------------------------------------------
static T_NV_DIAGNOSIS_REPLACE StartupRepairFeWithRam_REPLACE(const T_FILE SLOW* me)
{
	T_NV_DIAGNOSIS_REPLACE result;
	TUSIGN16 replaceResult;
	TUSIGN16 retryCount = 0;
	TUSIGN16 sIdx;

	do
	{
		result = REPLACE_OK;
		for(sIdx=0; sIdx<me->numberOfSegments && result!=REPLACE_DISABLED;sIdx++)
		{   //lint -e{734,679}
			replaceResult = PutNvData_Replace(me->fileIndex,sIdx,(TUSIGN8*)&(me->startAdrNvRam[sIdx*CH_SEG_NETTO_LENGTH]),CH_SEG_NETTO_LENGTH);
			if(replaceResult != OK)
			{
				result = REPLACE_DISABLED;
			}
		}

		retryCount++;
	}
	while(result == REPLACE_DISABLED && retryCount<REPLACE_RETRY);

	if(result == REPLACE_OK)
	{
		SetAllSegClean(me->replaceDirtyFlags,me->numberOfSegments);
	}

	return result;
}


//----------------------------------------------------------------------------------------------------------
/*!
  \brief  	Check and Repair File; first check main agains backup, decide what to and repair if necessary
  \author 	Roberto Colombo
  \date   	2008-03-26
  \param  me
  \return
  		T_NV_DIAGNOSIS_REPLACE
    		REPLACE_OK
    		REPLACE_DISABLED
 \warning
 test-date:
 \n by:
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3: OK
 \bug
*/
//----------------------------------------------------------------------------------------------------------
T_NV_DIAGNOSIS_REPLACE CheckAndRepairReplaceFILE(const T_FILE SLOW* me)
{

	TUSIGN8 dataStatus = eTRUE;
	TUSIGN8 changeCommType = eFALSE;
	TUSIGN32 coordinatorDiagnosis;
	T_NV_REPLACE_DIRECTION direction;
	VIP_ASSERT(me);

	coordinator_Get(CDT_IDX_cdtDiagnosis,WHOLE_OBJECT,&coordinatorDiagnosis);

	//Reset diagnosis flags
	if(resetDiagnosisReplace)
	{
//		coordinatorDiagnosis &= ~((TUSIGN32)1<<CDT_ALARM_FE_NV_FAILED);
		coordinatorDiagnosis &= ~((TUSIGN32)1<<CDT_ALARM_REPLACE_FE_TO_CB_BLOCKED);
		coordinatorDiagnosis &= ~((TUSIGN32)1<<CDT_ALARM_REPLACE_REQUIRED_FE_TO_CB_BLOCKED);
		coordinatorDiagnosis &= ~((TUSIGN32)1<<CDT_ALARM_REPLACE_REQUIRED_BOTH_DIRECTION_ENABLED);
  	    resetDiagnosisReplace = eFALSE;
	}


	//Get available direction
	if(me->features & COMMON) 
	{	
		direction = directionCommonResultInitialization;
	}
	else 
	{	
		direction = directionUncommonResultInitialization;
	}

	//Check if a communication type change is done
 	if(((me->features & UNCOMMON) || (me->features & STACK_FILE)) && ((direction == REPLACE_DIRECTION_INVALID_COMM_TYPE) || forceStackChangeCommType))
 	{
		changeCommType = eTRUE;

		forceStackChangeCommType = eTRUE;
	}

	//Check the direction status
	if(direction != REPLACE_DIRECTION_OK)
	{
		dataStatus = eFALSE;
	}

    //Perform replace
	//lint --e{522,731}
	if(dataStatus == eFALSE)
	{
		if(DIP_SWITCH_2_IS_ON)// Enable And Disable SIWTCH
		{
			if(DIP_SWITCH_3_IS_OFF)
			{
				if(changeCommType)
				{
					me->pDynamics->nvDiagnosisReplace = StartupRepairRamWithFe_REPLACE(me);
				}
				else
				{
					coordinatorDiagnosis |= ((TUSIGN32)1<<CDT_ALARM_REPLACE_FE_TO_CB_BLOCKED); //NVReplace_Warining
					me->pDynamics->nvDiagnosisReplace = REPLACE_DISABLED;
				}
			}
			else
			{
				me->pDynamics->nvDiagnosisReplace = StartupRepairFeWithRam_REPLACE(me);
			}
		}
		else
		{
		    coordinatorDiagnosis |= ((TUSIGN32)1<<CDT_ALARM_REPLACE_REQUIRED_FE_TO_CB_BLOCKED); //NVReplace_Warining
			me->pDynamics->nvDiagnosisReplace = REPLACE_DISABLED;
		}
	}
	else
	{
		if(DIP_SWITCH_2_IS_ON)
		{
			if(DIP_SWITCH_3_IS_OFF)
			{   
				if((me->pDynamics->nvDiagnosisReplace = StartupRepairRamWithFe_REPLACE(me)) == REPLACE_DISABLED)
				{
					me->Load(me);
					me->Execute(me);
				}
				else
				{
					me->SetFileToDirty(me);
					me->Save(me);
					me->Execute(me);
				}
			}
			else
			{
				me->pDynamics->nvDiagnosisReplace = StartupRepairFeWithRam_REPLACE(me);
			}
		}
		else
		{
			coordinatorDiagnosis |= ((TUSIGN32)1<<CDT_ALARM_REPLACE_REQUIRED_BOTH_DIRECTION_ENABLED); //NVReplace_Warining

			me->pDynamics->nvDiagnosisReplace = REPLACE_DISABLED;
		}
	}

	coordinator_Put(CDT_IDX_cdtDiagnosis,WHOLE_OBJECT,&coordinatorDiagnosis);

	return me->pDynamics->nvDiagnosisReplace;
}


//----------------------------------------------------------------------------------------------------------
/*!
  \brief  	Force replace operation
  		Can be done only by manifacturing
  \author 	Roberto Colombo
  \date   	2008-03-26
  \param  me
  \return
  		T_NV_DIAGNOSIS_REPLACE
    		REPLACE_OK
    		REPLACE_DISABLED
 \warning
 test-date:
 \n by:
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3: OK
 \bug
*/
//----------------------------------------------------------------------------------------------------------
T_NV_DIAGNOSIS_REPLACE ForcedReplaceFILE(const struct _T_FILE SLOW* me)
{
	TUSIGN8 forcedReplace;
	TUSIGN16 result;
	VIP_ASSERT(me);


	result = coordinator_Get(CDT_IDX_forceReplace,WHOLE_OBJECT,&forcedReplace);

	if(forcedReplace == CDT_FORCE_REPLACE_FE_TO_CB_COMMON_ONLY)
	{
		if(me->features & COMMON)
		{
			forcedReplace = CDT_FORCE_REPLACE_FE_TO_CB;
		}
		else
		{
			forcedReplace = CDT_FORCE_REPLACE_CB_TO_FE;
		}
	}

	if(result == OK)
	{
		//Forced Fe to Cb
		if(forcedReplace == CDT_FORCE_REPLACE_FE_TO_CB)
		{
			if((me->pDynamics->nvDiagnosisReplace = StartupRepairRamWithFe_REPLACE(me)) == REPLACE_DISABLED)
			{
				me->Load(me);
				me->Execute(me);
			}
			else
			{
				me->SetFileToDirty(me);
				me->Save(me);
				me->Execute(me);
			}
		}

		//Forced Cb to Fe
		else if(forcedReplace == CDT_FORCE_REPLACE_CB_TO_FE)
		{
			if((me->pDynamics->nvDiagnosisReplace = StartupRepairFeWithRam_REPLACE(me)) == REPLACE_DISABLED)
			{
				me->pDynamics->nvDiagnosisReplace = REPLACE_DISABLED;
			}
		}
		else
		{	
			me->pDynamics->nvDiagnosisReplace = REPLACE_DISABLED;
		}
	}
	else
	{	
		me->pDynamics->nvDiagnosisReplace = REPLACE_DISABLED;
	}

	return me->pDynamics->nvDiagnosisReplace;
}


//----------------------------------------------------------------------------------------------------------
/*!
  \brief  	Private Method
  		Perform the nv alignment between cb and fe boards
  \author 	Roberto Colombo
  \date   	2008-03-26
  \param  me
  \return
  		T_NV_DIAGNOSIS_REPLACE
    		REPLACE_OK
    		REPLACE_DISABLED
 \warning
 test-date:
 \n by:
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3: OK
 \bug
*/
//----------------------------------------------------------------------------------------------------------
static T_NV_DIAGNOSIS_REPLACE FileToNV_REPLACE(const T_FILE SLOW*me,TUSIGN8 srcPage)
{
	T_NV_DIAGNOSIS_REPLACE result;
	TUSIGN16 alignResult = OK;
	TUSIGN16 srvResult = OK;
	TUSIGN16  sIdx, dIdx;
	TUSIGN8  mask;
	TUSIGN8  buffer[CH_SEG_NETTO_LENGTH];
//	TBOOL setPageInvalid = eTRUE;
	TUSIGN32 coordinatorDiagnosis;
	TUSIGN16 diagResult;

	if((diagnosisCommonResultInitialization == REPLACE_OK) && ((me->features & COMMON) || (((me->features & UNCOMMON) || (me->features & STACK_FILE)) && (diagnosisUncommonResultInitialization == REPLACE_OK))))
	{
		result = REPLACE_OK;

		for(sIdx=0; sIdx<me->numberOfSegments && alignResult == OK && result == REPLACE_OK; sIdx++)
		{
			mask = (1<<(sIdx&7));
			dIdx = sIdx>>3;

			//Copy only segments that have changed
			if((me->replaceDirtyFlags[dIdx] & mask) && ((me->mainDirtyFlags[dIdx] & mask) == 0) && ((me->backupDirtyFlags[dIdx] & mask) == 0))
			{

				//Copy has to be done only in burst mode
				//At startup, other functions has to be called
				//Todo
			  //lint -e{506,774}
				if(1)// no matter what mode . we need to update data.
				{

					if(me->pDynamics->saveTimerReplace == 0)
					{
						//Set page invalid at the first dirty segment
						//if(setPageInvalid)
						//{
							srvResult = SetPageInvalid_NVSERVICE_REPLACE(me->fileIndex);
							//setPageInvalid = eFALSE;
						//}

						if(srvResult == OK)
						{			
							if(CH_OK!=ReadData_CHIPHANDLER(srcPage,(TUSIGN16)(sIdx+me->nvIdxFirstSeg),buffer))
							{
								alignResult = ERROR_NV_STORAGE;
								result = REPLACE_ERROR;
							}
							else
							{   //lint -e{734}
                          		alignResult = PutNvData_Replace(me->fileIndex,sIdx,(TUSIGN8*)buffer,CH_SEG_NETTO_LENGTH); //SEND 32 INCLUDE CRC 
                 				if(alignResult == OK)
								{
									me->replaceDirtyFlags[dIdx] &= ~mask;
								}
								else
								{
									result = REPLACE_ERROR;
								}
							}
						}
						else
						{
							result = REPLACE_ERROR;
						}
                        //if(setPageInvalid == eFALSE && result != REPLACE_ERROR)
                        if(result != REPLACE_ERROR)
                        {
                            srvResult = SetPageValid_NVSERVICE_REPLACE(me->fileIndex);
                            
                            if(srvResult != OK)
                            {
                                result = REPLACE_ERROR;
                            }
                        }
					}
					else
					{
						if(me->pDynamics->saveTimerReplace > 0)
						{
							me->pDynamics->saveTimerReplace--;
							break;
						}
					}
				}
				else
				{
					////Leave trigger as it is
					//me->pDynamics->saveTimerReplace = REPLACE_TRIGGER;
					break;
				}
			}
		}

		//Check Result
		//if(result == REPLACE_ERROR)
		//{
			//If replace had an error during data storing or service page setting
			//all dirty flags are set to dirty
			//Really needed???
			//me->SetFileToDirtyReplace(me);

			//Leave trigger as it is
			//me->pDynamics->saveTimerReplace = REPLACE_TRIGGER;
		//}


		me->pDynamics->nvDiagnosisReplace = result;


		//Set or clear fe nv write failed warning
		//Warning is set only with a write error response code
		if(alignResult == REPLACE_ERROR)
		{
			//TUSIGN32 coordinatorExternalDiagnosis;
			//TUSIGN16 diagResult;
			diagResult = coordinator_Get(CDT_IDX_cdtDiagnosis,WHOLE_OBJECT,&coordinatorDiagnosis);

			if(diagResult == OK)
			{
				if(!(coordinatorDiagnosis & (TUSIGN32)1<<CDT_ALARM_REPLACE_FE_TO_CB_BLOCKED))
				{
					coordinatorDiagnosis |= ((TUSIGN32)1<<CDT_ALARM_REPLACE_FE_TO_CB_BLOCKED);//NVReplace_Warining 
					coordinator_Put(CDT_IDX_cdtDiagnosis,WHOLE_OBJECT,&coordinatorDiagnosis);
				}
			}
		}
		else
		{
			diagResult = coordinator_Get(CDT_IDX_cdtDiagnosis,WHOLE_OBJECT,&coordinatorDiagnosis);

			if(diagResult == OK)
			{
				if(coordinatorDiagnosis & (TUSIGN32)1<<CDT_ALARM_REPLACE_FE_TO_CB_BLOCKED)
				{
					coordinatorDiagnosis &= ~((TUSIGN32)1<<CDT_ALARM_REPLACE_FE_TO_CB_BLOCKED);
					coordinator_Put(CDT_IDX_cdtDiagnosis,WHOLE_OBJECT,&coordinatorDiagnosis);
				}
			}
		}
	}

    return me->pDynamics->nvDiagnosisReplace;
}


//----------------------------------------------------------------------------------------------------------
/*!
  \brief  	Return the dignosis replace status
  \author 	Roberto Colombo
  \date   	2008-03-27
  \param  me
  \return
  		T_NV_DIAGNOSIS_REPLACE
    		REPLACE_OK
    		REPLACE_ERROR
    		REPLACE_DISABLED
 \warning
 test-date:
 \n by:
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3: OK
 \bug
*/
//----------------------------------------------------------------------------------------------------------
T_NV_DIAGNOSIS_REPLACE GetNvDiagnosisReplaceFILE(const struct _T_FILE SLOW* me)
{
	VIP_ASSERT(me);

	return me->pDynamics->nvDiagnosisReplace;
}


//----------------------------------------------------------------------------------------------------------
/*!
  \brief  	Clean all dirty segment
  \author 	Roberto Colombo
  \date   	2008-03-31
  \param  pDityFlags	pointer to dirty segment flags
  		length		number of segment
  \return
 \warning
 test-date:
 \n by:
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3: OK
 \bug
*/
//----------------------------------------------------------------------------------------------------------
static void SetAllSegClean(TUSIGN8 FAST* pDirtyFlags, TUSIGN16 length)
{
	VIP_ASSERT(pDirtyFlags);
    //lint -e{534}
	memset(pDirtyFlags,0x00,(TUSIGN16)((length+7)/8));
}