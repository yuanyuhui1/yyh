//----------------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2004.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         common component
 Module         NV_MEM
 Description    public methods implementation of the service NV_MEM
 Remarks

*/
//----------------------------------------------------------------------------------------------------------
#include <intrinsics.h>
#include <string.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "System/Interface/assert.h"
#include "nv_mem/chip_handler/interface/chip_handler.h"
#include "nv_mem/Interface/nv_mem.h"
#include "nv_mem/nv_service/Interface/nv_service.h"
#include "nv_mem/File/Interface/File.h"

#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "Coordinator/Interface/subsystem_idx.h"
#include "ARM/Interface/arm.h"
#include "ARM/Interface/arm_idx.h"

#if defined(MODULTEST_NV_SERVICE) || defined(NV_MEM_MODULETEST)
#include "../NV_module_test/nv_mem/Interface/fileList.h"
#endif

// defined in fileList.c
// To hide the fileList we declare it here and not in nv_mem.h
extern SLOW const T_FILE SLOW *const fileList[NUM_OF_FILES];

//lint -e641  zuochen wang. 
//lint -e961  zuochen wang. 
T_NV_DIAGNOSIS diagnosisResultInitialization=NV_OK;
static TUSIGN8 nvCheckRequested;
T_NV_DIAGNOSIS_REPLACE diagnosisCommonResultInitialization = REPLACE_DISABLED;
T_NV_DIAGNOSIS_REPLACE diagnosisUncommonResultInitialization = REPLACE_DISABLED;
T_NV_REPLACE_DIRECTION directionCommonResultInitialization = REPLACE_DIRECTION_OK;
T_NV_REPLACE_DIRECTION directionUncommonResultInitialization = REPLACE_DIRECTION_OK;
TUSIGN8 nvExecuteIndex = 0;
TUSIGN8 nvExecuteTrigger = 0;

//-------------------------------------------------------------------------------------------------
/*!
 \brief  Initializes the NV-Memory-Manager
 \author Heiko Kresse
 \date   2009-03-03
 \param  -
 \return T_NV_DIAGNOSIS
   NV_ERROR --> communication not possible, maybe defect data or eeprom not formated
   NV_WARNING --> load from BACKUP
   NV_OK
 \warning
 \test
 test-date: 2009-03-17
 \n by: Heiko Kresse
 \n environment: PC7501-Emulator + High-performance workbench
 \n intention: complete instruction-coverage for the else-path of the first if. Rest: sight check
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//-------------------------------------------------------------------------------------------------
T_NV_DIAGNOSIS InitializeNV(void)
{
    T_NV_DIAGNOSIS result;
    TINT16   fileIdx;

    nvCheckRequested=eFALSE;
    Initialize_CHIPHANDLER(0);

    switch(Initialize_NVSERVICE())
    {
        case NVS_OK:
            diagnosisResultInitialization=NV_OK;

            // initialize all files
            for(fileIdx=0; fileIdx<NUM_OF_FILES; fileIdx++)
            {
                fileList[fileIdx]->Initialize(fileList[fileIdx]);
            }

            // WARNINGS are only supported from the File-Handler, so the information must be collected "by hand", the return 
            // could be ignored.
            (void)Load(ALL_FILES,NV_SYNC);
            for(fileIdx=0; fileIdx<NUM_OF_FILES; fileIdx++)
            {
                result=fileList[fileIdx]->GetNvDiagnosis(fileList[fileIdx]);
                if( result>diagnosisResultInitialization )
                {
                    diagnosisResultInitialization=result;
                }
            }

            //CheckNV(); // trigger background eeprom-test

        // Perform check and repair if necessary
        for(fileIdx=0; fileIdx<NUM_OF_FILES; fileIdx++)
        {
            (void)fileList[fileIdx]->CheckAndRepair(fileList[fileIdx]);
            
            result=fileList[fileIdx]->GetNvDiagnosis(fileList[fileIdx]);
            
            // if buffered file is corrupted
            if(result != NV_OK && fileList[fileIdx]->features & BUFFERED)
            {
                result = NV_OK;
                
                if(fileList[fileIdx]->features & COMMON)
                {
                    cyclicCorruptionCommon = eTRUE;
                }
                else if(fileList[fileIdx]->features & UNCOMMON) 
                {
                    cyclicCorruptionUncommon = eTRUE;
                }
            }
        }

        nvCheckRequested=eFALSE;
            break;

        case NVS_VIRGIN_NV:
            diagnosisResultInitialization=NV_ERROR;
            break;

        case NVS_ERROR:
        default:
            diagnosisResultInitialization=NV_ERROR;
            break;
    }

    return diagnosisResultInitialization;
}


//-------------------------------------------------------------------------------------------------
/*!
 \brief  get the dianosis result of the nv-initialization
 \author Heiko Kresse
 \date   2009-03-03
 \param  -
 \return T_NV_DIAGNOSIS
   NV_ERROR --> communication not possible, maybe defect data or eeprom not formated
   NV_WARNING --> load from BACKUP
   NV_OK
 \warning
 \test
 test-date: 2009-03-17
 \n by: Heiko Kresse
 \n environment: PC7501-Emulator + High-performance workbench
 \n intention: sight check
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//-------------------------------------------------------------------------------------------------
T_NV_DIAGNOSIS GetInitializationDiagnosisNV(void)
{
    return diagnosisResultInitialization;
}


//-------------------------------------------------------------------------------------------------
/*!
 \brief  clear (set to NV_OK) the dianosis result of the nv-initialization
 \author Heiko Kresse
 \date   2009-03-03
 \param  -
 \return -
 \warning
 \test
 test-date: 2009-03-04
 \n by: Heiko Kresse
 \n environment: PC7501-Emulator + High-performance workbench
 \n intention: sight check
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//-------------------------------------------------------------------------------------------------
void ClearInitializationDiagnosisNV(void)
{
    diagnosisResultInitialization=NV_OK;
}


//-------------------------------------------------------------------------------------------------
/*!
 \brief  get nv-state; the state is working until all files are ready
 \author Heiko Kresse
 \date   2008-01-03
 \param  -
 \return T_NV_STATE
   NV_WORKING
   NV_READY
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
T_NV_STATE GetStateNV(void)
{
    T_NV_STATE result;
    TINT16     fileIdx;
    //lint -e{731}
    if( nvCheckRequested == eTRUE )
    {
        return NV_WORKING;
    }
    else
    {
        fileIdx=0;
        do
        {
            result=fileList[fileIdx]->GetState(fileList[fileIdx]);
            fileIdx++;
        }
        while( fileIdx<NUM_OF_FILES && result!=NV_WORKING);

        return result;
    }
}


//-------------------------------------------------------------------------------------------------
/*!
 \brief  get diagnosis information for one file or for all   
 \author Heiko Kresse
 \date   2009-03-03
 \param  -
 \return T_NV_DIAGNOSIS
    NV_OK,
    NV_WARNING,
    NV_ERROR
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
T_NV_DIAGNOSIS GetNvDiagnosisNV(TINT16 fileIndex)
{
   T_NV_DIAGNOSIS result, nvResult;

    VIP_ASSERT(fileIndex<NUM_OF_FILES);

    if( fileIndex<=ALL_FILES )
    {
        nvResult=NV_OK;

        for(fileIndex=0; fileIndex<NUM_OF_FILES; fileIndex++)
        {
            result=fileList[fileIndex]->GetNvDiagnosis(fileList[fileIndex]);
            if( result>nvResult )
            {
                nvResult=result;
			}

        }
    }
    else if(fileIndex<NUM_OF_FILES)
    {
        nvResult=fileList[fileIndex]->GetNvDiagnosis(fileList[fileIndex]);
    }
    else
    {
        nvResult=NV_ERROR;
    }

    if( nvResult!=NV_ERROR )
    {
        return NV_OK;
    }
    else
    {
        return NV_ERROR;
    }
}



//-------------------------------------------------------------------------------------------------
/*!
 \brief  set ram-diagnosis to NOT_INIT and nv-diagnosis to OK, this is necessary for
         formating the NV-storage. Be carful and don't use this function for other functions.
 \author Heiko Kresse
 \date   2008-01-03
 \param  fileIndex; maybe not all files must be formated
 \warning
 test-date: 2009-03-17
 \n by: Heiko Kresse
 \n environment: PC7501-Emulator + High-performance workbench
 \n intention: complete instruction-coverage
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//-------------------------------------------------------------------------------------------------
void ClearDiagnosisNV(TINT16 fileIndex)
{
    VIP_ASSERT(fileIndex<NUM_OF_FILES);

    if( fileIndex>ALL_FILES && fileIndex<NUM_OF_FILES )
    {
        fileList[fileIndex]->ClearDiagnosis(fileList[fileIndex]);
    }
    else
    {
        for(fileIndex=0; fileIndex<NUM_OF_FILES; fileIndex++)
        {
            fileList[fileIndex]->ClearDiagnosis(fileList[fileIndex]);
        }
    }
}


//-------------------------------------------------------------------------------------------------
/*!
 \brief  private; Wait for end of operation (SYNC and WAITING)
 \author Heiko Kresse
 \date   2009-03-03
 \param  fileIndex
 \param  mode
    NV_ASYNC    : start saving in background
    NV_WAITING  : start saving in background, but the calling TASK will wait until the operation ends
                  Thus tasks with priority less the calling task could run.
    NV_SYNC     : save inside the calling TASK (saving not in background!)
 \return
    NV_OK,
    NV_WARNING,
    NV_ERROR
 \warning
 \test
 test-date: 2009-03-17
 \n by: Heiko Kresse
 \n environment: PC7501-Emulator + High-performance workbench
 \n intention: instruction-coverage + code review
    The code for the mode WAITING must be tested inside the target envoirement (integration-test)
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//-------------------------------------------------------------------------------------------------
static T_NV_DIAGNOSIS WaitForEndOperation(TINT16 fileIndex, T_NV_MODE mode)
{
    TINT16 lastFileIndex, fIdx;
    T_NV_STATE state;

    if(mode==NV_ASYNC)
    {
        return NV_OK;
    }
    else
    {
        do
        {
            if( fileIndex<=ALL_FILES || fileIndex>=NUM_OF_FILES )
            {
                fIdx=0;
                lastFileIndex=NUM_OF_FILES-1;
            }
            else
            {
                fIdx=lastFileIndex=fileIndex;
            }

            do
            {
                if(mode==NV_WAITING)
                {
                    // poll file-state, execute runs in a less prior task
                    state=fileList[fIdx]->GetState(fileList[fIdx]);
                }
                else
                {
                    // run Execute in the active task
                    state=fileList[fIdx]->Execute(fileList[fIdx]);
                }

                fIdx++;
            } // run all executes (SYNC); poll until the first file is not ready (WAITING)
            while(fIdx<=lastFileIndex && state==NV_READY);

            // in WAITING-mode the actual state shall be scheduled only once in 100ms
            if(state!=NV_READY)
            {
                //lint -e{534} Heiko Kresse 2008-01-03
                Delay_RTOS_TASK(100);
            }
        } //(WAITING) wait until all files are ready
        while(state!=NV_READY);
        // in WAITING a deadlock is possible, but nv_mem is not responsible to detect this.
        // for this the coordinator should use the watchdog-service

    	return GetNvDiagnosisNV(fileIndex);
	}
}

//-------------------------------------------------------------------------------------------------
/*!
 \brief  Write file from Ram-Shadow into nv-storage
 \author Heiko Kresse
 \date   2008-01-03
 \param  fileIndex
 \param  mode
    NV_ASYNC    : start saving in background
    NV_WAITING  : start saving in background, but the calling TASK will wait until the operation ends
                  Thus tasks with priority less the calling task could run.
    NV_SYNC     : save inside the calling TASK (saving not in background!)
 \return
    NV_OK,
    NV_ERROR
 \warning
    Mode NV_SYNC is for situation where the SlowTask is not running. But it must not be used if Execute() could be called from a lower task!
    In mode NV_WAITING a deadlock is possible. The caller is responsible to detect this!!!!!
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
T_NV_DIAGNOSIS Save(TINT16 fileIndex, T_NV_MODE mode)
{
    TINT16 i;

    VIP_ASSERT(fileIndex<NUM_OF_FILES);
    VIP_ASSERT(mode==NV_ASYNC || mode==NV_WAITING || mode==NV_SYNC);

    if( fileIndex<=ALL_FILES || fileIndex>=NUM_OF_FILES)
    {
        for(i=0; i<NUM_OF_FILES; i++)
        {
            fileList[i]->Save(fileList[i]);
        }
    }
    else
    {
        fileList[fileIndex]->Save(fileList[fileIndex]);
    }

    return WaitForEndOperation(fileIndex,mode);
}


//-------------------------------------------------------------------------------------------------
/*!
 \brief  Set all files completly to dirty and save all files in snchrone-mode
 \author Heiko Kresse
 \date   2009-03-20
 \param  fileIndex
 \param  mode 
    NV_ASYNC    : start saving in background
    NV_WAITING  : start saving in background, but the calling TASK will wait until the operation ends
                  Thus tasks with priority less the calling task could run.
    NV_SYNC     : save inside the calling TASK (saving not in background!)
    NV_OK,
    NV_ERROR
 \return
    NV_OK,
    NV_ERROR
 \warning
    In mode NV_WAITING a deadlock is possible. The caller is responsible to detect this!!!!!
 \test
    Mode NV_SYNC is for situation where the SlowTask is not running. But it must not be used if Execute() could be called from a lower task!
 test-date: 2009-03-20
 \n by: Heiko Kresse
 \n environment: PC7501-Emulator + High-performance workbench
 \n intention: sight-check + test for all-files
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//-------------------------------------------------------------------------------------------------
T_NV_DIAGNOSIS Format(TINT16 fileIndex, T_NV_MODE mode)
{
    TINT16 i, endIndex;

    diagnosisResultInitialization = NV_OK;

    VIP_ASSERT(fileIndex<NUM_OF_FILES);

    if( fileIndex<=ALL_FILES )
    {
        i=0;
        endIndex=NUM_OF_FILES-1;
    }
    else
    {
        i=endIndex=fileIndex;
    }

    for(; i<=endIndex; i++)
    {
        fileList[i]->SetFileToDirty(fileList[i]);
    }

    diagnosisResultInitialization = Save(fileIndex,mode);

    return diagnosisResultInitialization;
}

//-------------------------------------------------------------------------------------------------
/*!
 \brief  Read file from nv-storage into ram-shadow
 \author Heiko Kresse
 \date   2008-01-03
 \param  fileIndex
 \param  mode
    NV_ASYNC    : start saving in background
    NV_WAITING  : start saving in background, but the calling TASK will wait until the operation ends
                  Thus tasks with priority less the calling task could run.
    NV_SYNC     : save inside the calling TASK (saving not in background!)
 \return
    NV_OK,
    NV_ERROR
 \warning
    In mode NV_WAITING a deadlock is possible. The caller is responsible to detect this!!!!!
    Mode NV_SYNC is for situation where the SlowTask is not running. But it must not be used if Execute() could be called from a lower task!
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
T_NV_DIAGNOSIS Load(TINT16 fileIndex, T_NV_MODE mode)
{
    TINT16 i;

    VIP_ASSERT(fileIndex<NUM_OF_FILES);
    VIP_ASSERT(mode==NV_ASYNC || mode==NV_WAITING || mode==NV_SYNC);

    if( fileIndex<=ALL_FILES || fileIndex>=NUM_OF_FILES)
    {
        for(i=0; i<NUM_OF_FILES; i++)
        {
            fileList[i]->Load(fileList[i]);
        }
    }
    else
    {
        fileList[fileIndex]->Load(fileList[fileIndex]);
    }

    return WaitForEndOperation(fileIndex,mode);
}


//-------------------------------------------------------------------------------------------------
/*!
 \brief  Trigger background check of non volatile storage
 \author Heiko Kresse
 \date   2008-01-04
 \param  -
 \return -
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
void CheckNV(void)
{
    nvCheckRequested=eTRUE;
}


//-------------------------------------------------------------------------------------------------
/*!
 \brief  Execute method of the non volatile memory manager
         It shall be scheduled from a task with low priority because it takes secounds to
         save all data and check the nv-memory
 \author Heiko Kresse
 \date   2009-03-03
 \param  -
 \return -
 \warning
 \test
 test-date: 2009-03-17
 \n by: Heiko Kresse
 \n environment: PC7501-Emulator + High-performance workbench
 \n intention: complete instruction-coverage
 \n result module test: Failed, but tolerable for TZIDC 3.00
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//-------------------------------------------------------------------------------------------------
void ExecuteNV(void)
{
    TUSIGN16 i;
	
    /*for(i=0; i<NUM_OF_FILES; i++)
    {
		(void)fileList[i]->Execute(fileList[i]);
	}*/
	
    if(nvExecuteTrigger<NV_EXECUTE_TRIGGER)
    {
		nvExecuteTrigger++;
	 	return;
    }
    else 
	{	
		nvExecuteTrigger = 0;
	}
	
    (void)fileList[nvExecuteIndex]->Execute(fileList[nvExecuteIndex]);
	
    nvExecuteIndex++;
    if(nvExecuteIndex >= NUM_OF_FILES) 
	{	
		nvExecuteIndex = 0;
	}
	//lint -e{731}
    if( nvCheckRequested==eTRUE )
    {
        for(i=0; i<NUM_OF_FILES; i++)
        {
            (void)fileList[i]->CheckAndRepair(fileList[i]);
        }
		
        nvCheckRequested=eFALSE;
    }

}


//-------------------------------------------------------------------------------------------------
/*!
 \brief  Put non-volatile data, the target have to be completly inside a file
 \author Heiko Kresse
 \date   2008-01-04
 \param  ptrDestAdr  destination address
 \param  ptrSrcAdr   source address
 \param  length      length
 \return
    NV_OK
    NV_ERROR : target is not part of a file
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
TUSIGN16 PutDataNV(const TUSIGN8 FAST* ptrDestAdr, const TUSIGN8 SLOW* ptrSrcAdr, TUSIGN16 length)
{
    TINT16 i;
    T_NV_DIAGNOSIS result;

    i=0;
    do
    {
        result=fileList[i]->PutData(fileList[i],ptrDestAdr,ptrSrcAdr,length);
        i++;
    }
    while(i<NUM_OF_FILES && result!=NV_OK );

    return result;
}


//-------------------------------------------------------------------------------------------------
/*!
 \brief  Get state of crc-protection
 \author Heiko Kresse
 \date   2008-01-09
 \param  fileIndex
 \return actual state
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
T_CRC_STATE GetCRCStateNV(TINT16 fileIndex)
{
    T_CRC_STATE result, state;
    TINT16 i;

    VIP_ASSERT(fileIndex<NUM_OF_FILES);

    if( fileIndex<=ALL_FILES || fileIndex>=NUM_OF_FILES)
    {
        state=CRC_PROTECTED;
        for(i=0; i<NUM_OF_FILES; i++)
        {
            result=fileList[i]->GetCRCState(fileList[i]);
            if(  result > state )
            {
                state=result;
            }
        }

        return state;
    }
    else
    {
        return fileList[fileIndex]->pFileCRC->state;
    }
}



//-------------------------------------------------------------------------------------------------
/*!
 \brief  Get non-volatile data
         for this implementation it is only a memcopy
 \author Heiko Kresse
 \date   2008-01-04
 \param  ptrDestAdr  destination address
 \param  ptrSrcAdr   source address
 \param  length      length
 \return allways NV_OK
 \warning
 \test
 test-date: 2008-05-21
 \n by: Heiko Kresse
 \n environment: PC7501-Emulator + High-performance workbench
 \n intention: code review
 \n result module test: OK
 \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//-------------------------------------------------------------------------------------------------
TUSIGN16 GetDataNV(TUSIGN8 FAST* ptrDestAdr, const TUSIGN8 FAST*ptrSrcAdr, TUSIGN16 length)
{
    //lint -e{534} Heiko Kresse 2008-01-04
    memcpy(ptrDestAdr,ptrSrcAdr,length);

    return NV_OK;
}


//-------------------------------------------------------------------------------------------------
/*!
 \brief  Check ram-shadow via CRC in background
 \author Heiko Kresse
 \date   2008-01-04
 \param  -
 \return T_NV_DIAGNOSIS
    NV_OK
    NV_ERROR : the RAM-shadow of one (or more) file is corrupted
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
T_CRC_STATE CheckRamNV(void)
{
    TINT16 i;

    for(i=0; i<NUM_OF_FILES; i++)
    {
        //lint -e{522} Heiko Kresse 2008-01-04
        fileList[i]->CheckRam(fileList[i]);
    }

    return GetCRCStateNV(ALL_FILES);
}


//-------------------------------------------------------------------------------------------------
/*!
 \brief  	Initializes the FE NV Common/Uncommon blocks
 		Initializes the service page
 		Perfoms the Forced Replace or the Check and Repair if necessary
 		
 \author 	Roberto Colombo
 \date   	2009-03-25
 \param
   		format 			1 --> Cb has been formated, 	0 --> Cb had a normal loading
   		type (file features)	8 --> Common, 				4 --> Uncommon
   		diagnosis			pointer to the diagnosis result
 \return 
 \warning
 \test
 test-date:
 \n by:
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//-------------------------------------------------------------------------------------------------
static void Initialize_NV_REPLACE(TUSIGN8 type, T_NV_DIAGNOSIS_REPLACE* diagnosis, T_NV_REPLACE_DIRECTION* direction)
{
	TINT16   fileIdx;
	TUSIGN16 initSerResult;
	TUSIGN16 feature;

	VIP_ASSERT(diagnosis);
	VIP_ASSERT(direction);

	*diagnosis = REPLACE_OK;

	initSerResult = Initialize_NVSERVICE_REPLACE(type,direction);
	feature = REPLACED|type;

	switch(initSerResult)
	{
		case NVS_OK:
			*diagnosis = REPLACE_OK;
			break;

		case NVS_VIRGIN_NV:
		case NVS_DATA_ERROR:
		case NVS_FORCED_REPLACE:
			for(fileIdx=0; fileIdx<NUM_OF_FILES && *diagnosis == REPLACE_OK; fileIdx++)
			{
				if((fileList[fileIdx]->features & feature) == feature)
				{
					if(initSerResult == NVS_FORCED_REPLACE)
					{
						*diagnosis |= fileList[fileIdx]->ForcedReplace(fileList[fileIdx]);
					}
					else
					{
						*diagnosis = fileList[fileIdx]->CheckAndRepairReplace(fileList[fileIdx]);
					}
				}
			}

			if(*diagnosis == REPLACE_OK)
			{
			  (void)WritePage_NVSERVICE_REPLACE(type);
			}
			break;

		case NVS_ALREADY_MARRIADGE:
			for(fileIdx=0; fileIdx<NUM_OF_FILES && *diagnosis == REPLACE_OK; fileIdx++)
			{   //lint -e{731}
				if((fileList[fileIdx]->features & feature) == feature && *fileList[fileIdx]->marriedCheck == eTRUE)
				{	
					fileList[fileIdx]->SetFileToDirtyReplace(fileList[fileIdx]);
				}
			}
			break;

		default:
		case NVS_ERROR:
			*diagnosis = REPLACE_DISABLED;
			break;
	}
}


//-------------------------------------------------------------------------------------------------
/*!
 \brief  	Initializes the NV-FE-Memory-Manager
		InitializeNV_REPLACE_ALIGNMENT is called for both common and uncommon blocks
 		
 \author 	Roberto Colombo
 \date   	2009-03-25
 \param
   		format 	1 --> Cb has been formated, 0 --> Cb had a normal loading
 \return 
 \warning
 \test
 test-date:
 \n by:
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//-------------------------------------------------------------------------------------------------
T_NV_DIAGNOSIS_REPLACE InitializeNV_REPLACE(void)
{	
	if(diagnosisResultInitialization != NV_ERROR)
	{
		Initialize_NV_REPLACE(COMMON,&diagnosisCommonResultInitialization,&directionCommonResultInitialization);

		Initialize_NV_REPLACE(UNCOMMON,&diagnosisUncommonResultInitialization,&directionUncommonResultInitialization);

		if(diagnosisCommonResultInitialization != REPLACE_OK)
		{
			diagnosisCommonResultInitialization = REPLACE_DISABLED;
		}
	}
	//else
	//diagnosisCommonResultInitialization default is REPLACE_DISABLED

	return diagnosisCommonResultInitialization;
}



//-------------------------------------------------------------------------------------------------
/*!
 \brief  	Check if a nv writing is active, both main and backup
 \author 	Roberto Colombo
 \date   	2009-11-12
 \param
 \return 	eTRUE if active, else eFALSE
 \warning
 \test
 test-date:
 \n by:
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//-------------------------------------------------------------------------------------------------
TBOOL CheckWriteActive(void)
{
	TINT16 i;
	TUSIGN16 j;

	for(i=0; i<NUM_OF_FILES; i++)
	{   //lint --e{960}
		for(j=0; j<((fileList[i]->numberOfSegments+7)/8); j++)
		{   
			if(fileList[i]->mainDirtyFlags[j] != 0) return eTRUE;
			if(fileList[i]->backupDirtyFlags[j] != 0) return eTRUE;
		}
	}

	return eFALSE;
}


//-------------------------------------------------------------------------------------------------
/*!
 \brief  	Check if a nv-replace writing is active
 \author 	Roberto Colombo
 \date   	2009-11-12
 \param
 \return 	eTRUE if active, else eFALSE
 \warning
 \test
 test-date:
 \n by:
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \bug
*/
//-------------------------------------------------------------------------------------------------
TBOOL CheckWriteActive_REPLACE(void)
{
	TINT16 i;
	TUSIGN16 j;

	for(i=0; i<NUM_OF_FILES; i++)
	{   //lint --e{960}
		for(j=0; j<((fileList[i]->numberOfSegments+7)/8); j++)
		{
			if(fileList[i]->replaceDirtyFlags[j] != 0) return eTRUE;
		}
	}

	return eFALSE;
}