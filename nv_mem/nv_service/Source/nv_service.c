//----------------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2004.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         MiLe 2
 Module         NV_SERVICE
 Description    implementation of subsystem NV_SERVICE part of NV_MEM
 <pre>
    This NV_SERVICE handles sericeinformations via two segments, one for main- and one for the backup-page.
 </pre>
 Remarks

*/
//----------------------------------------------------------------------------------------------------------
#include <string.h>
#include <intrinsics.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "System/Interface/assert.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"

#include "T_Unit/Interface/t_unit.h"
#include "Coordinator/Interface/Coordinator.h"
#include "Coordinator/Interface/Coordinator_idx.h"
#include "ElettronicServices/Interface/ElettronicServices.h"
#include "ElettronicServices/Interface/ElettronicServices_idx.h"
#include "nv_mem/chip_handler/interface/chip_handler.h"
#include "nv_mem/FEBus/FEBus.h"
#include "nv_mem/Interface/nv_mem.h"
#include "nv_mem/File/Interface/File.h"
#include "nv_mem/nv_service/Interface/nv_service.h"

#if defined(MODULTEST_NV_SERVICE) || defined(NV_MEM_MODULETEST)
#include "../../NV_module_test/nv_mem/Interface/fileList.h"
#endif
// defined in fileList.c
// To hide the fileList we declare it here and not in nv_mem.h
extern SLOW const T_FILE SLOW *const fileList[NUM_OF_FILES];

#define NVS_INVALID 0
#define NVS_VALID   0xA05F
#define NVS_RESET			0xFFFF
#define HART_BLOCK 			0x00
//#define HART_MV_BLOCK 		0x0A
//#define PA_BLOCK	 		0x20
//#define PA_MV_BLOCK	 		0x2A
//#define FF_BLOCK	 		0x40
//#define FF_MV_BLOCK	 		0x4A
//#define MODBUS_BLOCK 		0x60
#define COMMON_BLOCK_STD 	0xAA
//#define COMMON_BLOCK_MV 	0xDD


//add by zuochen wang
#define DB_VERS_COMMON 0
#define DB_VERS_UNCOMMON 1

//lint -e641  zuochen wang. 
//lint -e961  zuochen wang. 
//lint --e{754}
typedef struct _NVS_DATA_EXPLODE
{
	TUSIGN8 nvDbVers;
	TUSIGN8 nvCommType;
	TUSIGN8	nvCBSerialNumber[3];
	TUSIGN8 nvFESerialNumber[3];
	TUSIGN8 nvFree[NVS_DATALENGTH-8];
}NVS_DATA_EXPLODE;


typedef union
{
	NVS_DATA_EXPLODE nvDataExplode; 
	TUSIGN8 nvData[NVS_DATALENGTH];
} NVS_DATA;


//! service-segments ram-shadow
typedef struct _NVS_STRUCT
{
    TUSIGN16 validFlag;
    TUSIGN32 writeCnt;
    NVS_DATA  data;
} NVS_STRUCT;


//! buffer for servcie-segment operations reduces the stack size of each task that uses NV
NVS_STRUCT mSrvSegBuffer;
NVS_STRUCT rSrvSegPages[NUM_OF_FILES];

#ifdef HART_COMMUNICATION
#ifdef MV
#define COMN_TYPE  HART_MV_BLOCK
#else
#define COMN_TYPE  HART_BLOCK
#endif
#else
#define COMN_TYPE  HART_BLOCK
#endif

#ifdef MV
#define COMMON_BLOCK		COMMON_BLOCK_MV
#else
#define COMMON_BLOCK		COMMON_BLOCK_STD
#endif

#define NO_COMN 0xff
#define NO_DB 0xff

//! initializer for a service-segment
//lint --e{708,651}
const NVS_STRUCT cmInitServiceSeg = { NVS_INVALID,
									0,
									{NO_DB
									,NO_COMN
								   ,0xff,0xff,0xff
								   ,0xff,0xff,0xff
								   ,0,0, 0,0,0,0,0,0
								   ,0,0,0,0,0,0,0,0    } };


static TBOOL mInitialized=eFALSE;	//Cb Initialization
//--------------------------------------------------------------------------------------------------
/*!
 \brief  Initialize the NV_SERVICE
 <pre>
    The method checks all service segments. For each file there are one Main and one Backup 
    service segment. If main and backup-service-segment for one ore more files could not be read,
    then the whole NV is invalid, which will be called a virgin nv-chip.
    If only one service-segment of a file is damaged, then usually the writing of this segment 
    was interrupted. The damaged segment will be repaired with the opposite one. 
 </pre>
 \author Heiko Kresse
 \date   2009-03-12
 \return TUSIGN16
 <pre>
    NVS_OK
    NVS_ERROR
    NVS_VIRGIN_NV
 </pre>
 \warning after an error-free initialize, initialize must not be called again
 \test
  test-date:       2009-03-17
  \n by:           Heiko Kresse
  \n environment:  Emulator PC7501 + High-Performance-Work-Bench debugger
  \n intention:    1. complete instruction coverage; (not the marked write-error)
  \n result module test: OK
  \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 Initialize_NVSERVICE(void)
{
    TBOOL virginFlag = eFALSE;
    TBOOL mainDataFlag, backupDataFlag;
    TUSIGN16 results[NVS_NUMOFPAGES][NVS_NUMOF_FILES], result;
    TUSIGN8  f,p;

    // check the service-segments for each file
    for(f=0; f<NVS_NUMOF_FILES; f++)
    {
        mainDataFlag = eFALSE;
		backupDataFlag = eFALSE;
		
        results[NVS_MAIN_PAGE][f]=ReadData_CHIPHANDLER(NVS_MAIN_PAGE,NVS_SERVICE_SEGNUM-f,&mSrvSegBuffer);
		
		if(results[NVS_MAIN_PAGE][f] == CH_OK)
		 {
			if((mSrvSegBuffer.data.nvDataExplode.nvCommType != COMMON_BLOCK && mSrvSegBuffer.data.nvDataExplode.nvCommType != COMN_TYPE) ||
			   (mSrvSegBuffer.data.nvDataExplode.nvCommType == COMMON_BLOCK && mSrvSegBuffer.data.nvDataExplode.nvDbVers != DB_VERS_COMMON) ||
		     (mSrvSegBuffer.data.nvDataExplode.nvCommType != COMMON_BLOCK && mSrvSegBuffer.data.nvDataExplode.nvDbVers != DB_VERS_UNCOMMON)	)
			{
				mainDataFlag = eTRUE;
			}
		}		
		
		results[NVS_BACKUP_PAGE][f]=ReadData_CHIPHANDLER(NVS_BACKUP_PAGE,NVS_SERVICE_SEGNUM-f,&mSrvSegBuffer);
		
		if(results[NVS_BACKUP_PAGE][f] == CH_OK)
		{
			if((mSrvSegBuffer.data.nvDataExplode.nvCommType != COMMON_BLOCK && mSrvSegBuffer.data.nvDataExplode.nvCommType != COMN_TYPE) ||
			(mSrvSegBuffer.data.nvDataExplode.nvCommType == COMMON_BLOCK && mSrvSegBuffer.data.nvDataExplode.nvDbVers != DB_VERS_COMMON) ||
			(mSrvSegBuffer.data.nvDataExplode.nvCommType != COMMON_BLOCK && mSrvSegBuffer.data.nvDataExplode.nvDbVers != DB_VERS_UNCOMMON)
			)
			{
				backupDataFlag=eTRUE;
			}
		}	
		
		// if both service-segments of one file are illeagel or damaged the whole chip is empty or invalid
		if( results[NVS_MAIN_PAGE][f]!=CH_OK && results[NVS_BACKUP_PAGE][f]!=CH_OK )
		{
			virginFlag=eTRUE;
        }
		else
		{
            if(results[NVS_MAIN_PAGE][f] == CH_OK)
            {
                if(mainDataFlag)
                {
                    virginFlag=eTRUE;	
                }
                else
                {
                    if(results[NVS_BACKUP_PAGE][f] != CH_OK || backupDataFlag)
                    {
                        results[NVS_BACKUP_PAGE][f]  = CH_DATA_ERROR;
                    }
                }
				
	        }
            else
            {
                if(backupDataFlag)
                {
                    virginFlag=eTRUE;
                }
                else
                {
                    results[NVS_MAIN_PAGE][f]  = CH_DATA_ERROR;
                }
            }
	    }
    }

    // in case of a virgin chip, a invalid chip or EMV problems segments must not be repaired
    if( virginFlag )
    {
        result = NVS_VIRGIN_NV;
    }
    else
	{
        result = NVS_OK;		
        // repair damaged service segments, an repaired service segment marks the page of the file as invalid
        for(f=0; f<NVS_NUMOF_FILES; f++)
        {
            for( p=0; p<NVS_NUMOFPAGES; p++)
            {
                if( results[p][f]!=CH_OK )
                {
                    // repair with the segment from the opposite page
                    if( CH_OK!=ReadData_CHIPHANDLER(p^1,NVS_SERVICE_SEGNUM-f,&mSrvSegBuffer) )
            		{
                		result = NVS_ERROR;
        			}
                    else
                    {
                        // mark the repaired page of the file as invalid
                        mSrvSegBuffer.validFlag=NVS_INVALID;
						
                        if( CH_OK!=WriteData_CHIPHANDLER(p,NVS_SERVICE_SEGNUM-f,&mSrvSegBuffer) )
        				{
                            // this instruction is not covered by the module-test
                            result=NVS_ERROR;
                        }
                    }
                }
            }
        }
    }

    mInitialized = eTRUE;

    return result;
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief  Get the valid-information for page of file 
 \author Heiko Kresse
 \date   2009-03-12
 \param  file; fileindex
 \param  page;   0 -> MainPage;   1 -> BackupPage
 \return TBOOL eTRUE-->Page is valid
 \warning
 \test
  test-date:       2009-03-17
  \n by:           Heiko Kresse
  \n environment:  Emulator PC7501 + High-Performance-Work-Bench debugger
  \n intention:    1. complete instruction coverage; 
  \n result module test: OK
  \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
TBOOL PageIsValid_NVSERVICE(TUSIGN8 page, TUSIGN8 file)
{
    TUSIGN16 result;

    VIP_ASSERT(mInitialized);
    VIP_ASSERT(page<NVS_NUMOFPAGES && file<NVS_NUMOF_FILES);

    result=ReadData_CHIPHANDLER(page,NVS_SERVICE_SEGNUM-file,&mSrvSegBuffer);

    if( result==CH_OK && mSrvSegBuffer.validFlag == NVS_VALID )
    {
        return eTRUE;
	}
    else
    {
        return eFALSE;
    }
}



//--------------------------------------------------------------------------------------------------
/*!
 \brief  set page of file valid
 \author Heiko Kresse
 \date   2009-03-12
 \param  page;   0 -> MainPage;   1 -> BackupPage
 \param  file; fileindex
 \return TUSIGN16
 <pre>
    NVS_OK
    NVS_ERROR valid-floag could not be storred
 </pre>
 \warning
 \test
  test-date:       2009-03-17
  \n by:           Heiko Kresse
  \n environment:  Emulator PC7501 + High-Performance-Work-Bench debugger
  \n intention:    1. complete instruction coverage; 
  \n result module test: OK
  \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 SetPageValid_NVSERVICE(TUSIGN8 page, TUSIGN8 file)
{
    TUSIGN16 result;

    VIP_ASSERT(mInitialized);
    VIP_ASSERT(page<NVS_NUMOFPAGES && file<NVS_NUMOF_FILES);

    result=ReadData_CHIPHANDLER(page,NVS_SERVICE_SEGNUM-file,&mSrvSegBuffer);

    if( result==CH_OK )
    {
        mSrvSegBuffer.validFlag=NVS_VALID;
        mSrvSegBuffer.writeCnt++;

        result=WriteData_CHIPHANDLER(page,NVS_SERVICE_SEGNUM-file,&mSrvSegBuffer);

        return (result==CH_OK)?NVS_OK:NVS_ERROR;
    }
    else
    {
        return NVS_ERROR;
    }
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief  set page of file invalid
 \author Heiko Kresse
 \date   2009-03-12
 \param  page;   0 -> MainPage;   1 -> BackupPage
 \param  file; fileindex
 \return TUSIGN16
 <pre>
   NVS_OK
    NVS_ERROR valid-flag could not be storred
 </pre>
 \warning
 \test
  test-date:       2009-03-17
  \n by:           Heiko Kresse
  \n environment:  Emulator PC7501 + High-Performance-Work-Bench debugger
  \n intention:    1. complete instruction coverage; 
  \n result module test: OK
  \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 SetPageInvalid_NVSERVICE(TUSIGN8 page, TUSIGN8 file)
{
    TUSIGN16 result;

    VIP_ASSERT(mInitialized);
    VIP_ASSERT(page<NVS_NUMOFPAGES && file<NVS_NUMOF_FILES);

    result=ReadData_CHIPHANDLER(page,NVS_SERVICE_SEGNUM-file,&mSrvSegBuffer);

    if( result==CH_OK )
    {
        mSrvSegBuffer.validFlag=NVS_INVALID;
        mSrvSegBuffer.writeCnt++;

        result=WriteData_CHIPHANDLER(page,NVS_SERVICE_SEGNUM-file,&mSrvSegBuffer);

        return (result==CH_OK)?NVS_OK:NVS_ERROR;
    }
    else
    {
        return NVS_ERROR;
    }
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief  write data in the not used part of the service-segment.
         The information will be nv-stored immediately

         The part of service-segment not used by nv-service, could be accessed from other subsystems
         to store special data.

         The writeCnt will be incremented.
 \author Heiko Kresse
 \date   2009-03-12
 \param  page;    0 -> MainPage;   1 -> BackupPage
 \param  file; fileindex
 \param  length;  number of databytes
 \param  ptrData; source pointer
 \param  formatSegment; 
 \return TUSIGN16
 <pre>
   NVS_OK
    NVS_ERROR data could not be storred
 </pre>
 \warning
 \test
  test-date:       2009-03-17
  \n by:           Heiko Kresse
  \n environment:  Emulator PC7501 + High-Performance-Work-Bench debugger
  \n intention:    1. complete instruction coverage; 
  \n result module test: OK
  \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 WriteData_NVSERVICE(TUSIGN8 page, TUSIGN8 file, TUSIGN8 length, const void *ptrData, TBOOL formatSegment)
{
    TUSIGN16 result;

    VIP_ASSERT(mInitialized);
    VIP_ASSERT(page<NVS_NUMOFPAGES && file<NVS_NUMOF_FILES);
    VIP_ASSERT(length<=NVS_DATALENGTH);
    VIP_ASSERT(ptrData);
    //lint -e{731}
    if( formatSegment==eTRUE)
    {
        result=CH_OK;
        mSrvSegBuffer=cmInitServiceSeg;
    }
    else
    {
        result=ReadData_CHIPHANDLER(page,NVS_SERVICE_SEGNUM-file,&mSrvSegBuffer);
    }

    if( result==CH_OK )
    {
        //lint -e{619} Heiko Kresse 2009-03-12
        (void)memcpy(&mSrvSegBuffer.data, ptrData, length);
        mSrvSegBuffer.writeCnt++;

        result=WriteData_CHIPHANDLER(page,NVS_SERVICE_SEGNUM-file,&mSrvSegBuffer);

        return (result==CH_OK)?NVS_OK:NVS_ERROR;
    }
    else
    {
        return NVS_ERROR;
    }
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief  get data in the not used part of the service-segment.

         The part of service-segment not used by nv-service, could be accessed from other subsystems
         to store special data.

 \author Heiko Kresse
 \date   2009-03-12
 \param  page;   0 -> MainPage;   1 -> BackupPage
 \param  file; fileindex
 \param  length;  number of databytes
 \param  ptrData; pointer to target
 \return TUSIGN16
 <pre>
    NVS_OK
 </pre>
 \warning
 \test
  test-date:       2009-03-17
  \n by:           Heiko Kresse
  \n environment:  Emulator PC7501 + High-Performance-Work-Bench debugger
  \n intention:    1. complete instruction coverage; 
  \n result module test: OK
  \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 GetData_NVSERVICE(TUSIGN8 page, TUSIGN8 file, TUSIGN8 length, void *ptrData)
{
    VIP_ASSERT(mInitialized);
    VIP_ASSERT(page<NVS_NUMOFPAGES && file<NVS_NUMOF_FILES);
    VIP_ASSERT(length<NVS_DATALENGTH);
    VIP_ASSERT(ptrData);

    if( CH_OK==ReadData_CHIPHANDLER(page,NVS_SERVICE_SEGNUM-file,&mSrvSegBuffer) )
    {
        (void)memcpy(ptrData, &mSrvSegBuffer.data, length);
        return NVS_OK;
    }
    else
    {
        return NVS_ERROR;
    }
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief  get write counter

         The part of service-segment not used by nv-service, could be accessed from other subsystems
         to store special data.

 \author Heiko Kresse
 \date   2009-03-12
 \param  page;   0 -> MainPage;   1 -> BackupPage
 \param  file; fileindex
 \param  pWrtCnt; call by reference 
 \return TUSIGN16 NVS_OK, NVS_ERROR
 \warning
 \test
  test-date:       2009-03-17
  \n by:           Heiko Kresse
  \n environment:  Emulator PC7501 + High-Performance-Work-Bench debugger
  \n intention:    1. complete instruction coverage; 
  \n result module test: OK
  \n result Lint Level 3: free // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 GetWriteCnt_NVSERVICE(TUSIGN8 page, TUSIGN8 file, TUSIGN32 *pWrtCnt)
{
    VIP_ASSERT(mInitialized);
    VIP_ASSERT(page<NVS_NUMOFPAGES && file<NVS_NUMOF_FILES);
    VIP_ASSERT(pWrtCnt);

    if( CH_OK==ReadData_CHIPHANDLER(page,NVS_SERVICE_SEGNUM-file,&mSrvSegBuffer) )
    {
        *pWrtCnt=mSrvSegBuffer.writeCnt;
        return NVS_OK;
    }
    else
    {
        return NVS_ERROR;
    }
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief  	Initialize all the Service Pages after a cb nv format
 \author 	Roberto Colombo
 \date   	2009-03-26
 \param  void
 \return 	void
 \warning
  \test
  \n by:
  \n intention:
  \n result module test:
  \n result Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 Format_NVSERVICE(TINT16 fileIndex)
{
	TUSIGN8 fileIdx = 0;
	TUSIGN16 result = OK;
	NVS_DATA initData = cmInitServiceSeg.data;	
	
    if(fileIndex<=ALL_FILES)
	{
		for(fileIdx = 0; fileIdx<NVS_NUMOF_FILES && result == OK; fileIdx++)
		{
			if(fileList[fileIdx]->features & COMMON)
			{
				initData.nvDataExplode.nvCommType = COMMON_BLOCK;
				initData.nvDataExplode.nvDbVers = DB_VERS_COMMON;
			}
			else
			{
				initData.nvDataExplode.nvCommType = COMN_TYPE;
				initData.nvDataExplode.nvDbVers = DB_VERS_UNCOMMON;
			}
			
			result = WriteData_NVSERVICE(NVS_MAIN_PAGE,fileIdx,sizeof(NVS_DATA),&initData,eTRUE);
			
			result |= WriteData_NVSERVICE(NVS_BACKUP_PAGE,fileIdx,sizeof(NVS_DATA),&initData,eTRUE);
		}
	}
    else if(fileIndex<NVS_NUMOF_FILES)
    {
		fileIdx = fileIndex;
		if(fileList[fileIdx]->features & COMMON)
		{
			initData.nvDataExplode.nvCommType = COMMON_BLOCK;
			initData.nvDataExplode.nvDbVers = DB_VERS_COMMON;
		}
		else
		{
			initData.nvDataExplode.nvCommType = COMN_TYPE;
			initData.nvDataExplode.nvDbVers = DB_VERS_UNCOMMON;
		}
		
		result = WriteData_NVSERVICE(NVS_MAIN_PAGE,fileIdx,sizeof(NVS_DATA),&initData,eTRUE);
		
		result |= WriteData_NVSERVICE(NVS_BACKUP_PAGE,fileIdx,sizeof(NVS_DATA),&initData,eTRUE);
    }
    else
    {
        result=NVS_ERROR;
    }	
	return result;
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief  	Initialize the Service Pages stored into Front End Board
 		Common and uncommon blocks are handled in the same way at this level
 		
 \author 	Roberto Colombo
 \date   	2009-03-25
 \param  
 		type;   0 -> Common Pages;   1 -> Uncommon Pages
 \return 
		TUSIGN16:
		NVS_OK				File is ok
		NVS_DATA_ERROR		File needs a repair
		NVS_ERROR			File is defect
		NVS_FORCED_REPLACE	Manifacturing forced replace is called
	
 \warning after an error-free initialize, initialize must not be called again
  \test
  \n by:
  \n intention:
  \n result module test:
  \n result Lint Level 3: OK
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 Initialize_NVSERVICE_REPLACE(TUSIGN8 type, T_NV_REPLACE_DIRECTION* direction)
{
	TUSIGN16 result = NVS_OK;
	TUSIGN16 pageResult;
	TUSIGN8 fileIdx;
	TUSIGN8 forcedReplace;
	TUSIGN8 u;
	TUSIGN8 feature = REPLACED|type;

	VIP_ASSERT(direction);

	*direction = REPLACE_DIRECTION_OK;

	//Init the service page segments store in ram with the default values
	for(fileIdx = 0; fileIdx<NVS_NUMOF_FILES; fileIdx++)
	{
		if((fileList[fileIdx]->features & feature) == feature)
		{
			rSrvSegPages[fileIdx] = cmInitServiceSeg;
		}
	}

	result = coordinator_Get(CDT_IDX_forceReplace,WHOLE_OBJECT,&forcedReplace);
	// force for test
	if(forcedReplace == CDT_FORCE_REPLACE_NONE)
	{
		//Common files are written in the first nv files
		//If type is Common, only the first x files have to be comparedd
		//If type is Uncommon, only the last y files have to be compared
		//For all files:
		//Read from Cb nv
		//Read from Fe nv
		//Compare
		for(fileIdx = 0; (fileIdx<NVS_NUMOF_FILES) && ((result == NVS_OK) ||(result == NVS_DATA_ERROR)) && (*direction == REPLACE_DIRECTION_OK); fileIdx++)
		{
			if((fileList[fileIdx]->features & feature) == feature)
			{
				pageResult = ReadData_CHIPHANDLER(NVS_MAIN_PAGE,NVS_SERVICE_SEGNUM-fileIdx,&mSrvSegBuffer);

				if(pageResult == CH_OK)
				{
					TUSIGN16 replaceResult;

					replaceResult = GetNvData_Replace( fileIdx,FESVR_SEG,(TUSIGN8*)&(rSrvSegPages[fileIdx]),CH_SEG_NETTO_LENGTH);

					if(replaceResult == OK)
					{
						// Check data
						for(u = 0; u < sizeof(NVS_DATA); u++)
						{
							if(rSrvSegPages[fileIdx].data.nvData[u] != mSrvSegBuffer.data.nvData[u])
							{
								result = NVS_DATA_ERROR;

								if(mSrvSegBuffer.data.nvDataExplode.nvDbVers != rSrvSegPages[fileIdx].data.nvDataExplode.nvDbVers)
								{
									//lint -e{655}
									*direction |= REPLACE_DIRECTION_INVALID_DB_REVISION;
								}
								else if(mSrvSegBuffer.data.nvDataExplode.nvCommType != rSrvSegPages[fileIdx].data.nvDataExplode.nvCommType)
								{
									//lint -e{655}
									*direction |= REPLACE_DIRECTION_INVALID_COMM_TYPE;
								}
								else
								{}

								break;
							}
						}

						// Validity Flag
						if(rSrvSegPages[fileIdx].validFlag != NVS_VALID)
						{
							if((fileList[fileIdx]->features & BUFFERED) == 0)
							{
								result = NVS_DATA_ERROR;
								//lint -e{655}
								*direction |= REPLACE_DIRECTION_INVALID_FLAG;
							}
						}

						// Remove replace flag if cb and fe already married
						//lint -e{655}
						if((result == NVS_DATA_ERROR) && !(*direction & (REPLACE_DIRECTION_INVALID_DB_REVISION | REPLACE_DIRECTION_INVALID_COMM_TYPE)))
						{
							TBOOL cbSnCheck = eTRUE;
							TBOOL feSnCheck = eTRUE;

							for(u = 0; u<sizeof(rSrvSegPages[fileIdx].data.nvDataExplode.nvCBSerialNumber); u++)
							{
								if(rSrvSegPages[fileIdx].data.nvDataExplode.nvCBSerialNumber[u] != mSrvSegBuffer.data.nvDataExplode.nvCBSerialNumber[u])
								{
									cbSnCheck = eFALSE;
									break;
								}
							}

							for(u = 0; u<sizeof(rSrvSegPages[fileIdx].data.nvDataExplode.nvFESerialNumber); u++)
							{
								if(rSrvSegPages[fileIdx].data.nvDataExplode.nvFESerialNumber[u] != mSrvSegBuffer.data.nvDataExplode.nvFESerialNumber[u])
								{
									feSnCheck = eFALSE;
									break;
								}
							}

							if(cbSnCheck && feSnCheck)
							{
								result = NVS_ALREADY_MARRIADGE;

								*fileList[fileIdx]->marriedCheck = eTRUE;

								*direction = REPLACE_DIRECTION_OK;
							}
						}

					}

					//Write error is the virgin response code
					//Nv handling gives a write error when read crc is not correct and all bytes are 0xff
					else if(replaceResult == WRITE_ERROR)
					{
							result = NVS_VIRGIN_NV;
					}

					//Other type of response code
					//Or communication problems
					else
					{
						result = NVS_ERROR;
					}
				}
				else
				{
					result = NVS_ERROR;
				}
			}
		}
	}
	else
	{
		result = NVS_FORCED_REPLACE;
	}

	if((result == NVS_VIRGIN_NV) || (result == NVS_ERROR))
	{
		//lint -e{655}
		*direction |= REPLACE_DIRECTION_INVALID_GENERIC;
	}

	return result;
  
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief  	Write new data into the service-segment.
         	The information will be nv-stored immediately
         The writeCnt will be incremented.
        	
 \author 	Roberto Colombo
 \date   	2009-03-25
 \param
 		file		file list index
 		length	data length
 		ptrData	source data pointer
 \return 
 		TUSIGN16:
 		NVS_ERROR	page is valid due to a communcation error
 		NVS_OK		communication is ok
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
//--------------------------------------------------------------------------------------------------
TUSIGN16 WriteData_NVSERVICE_REPLACE(TUSIGN8 file, TUSIGN8 length, const void FAST* ptrData)
{
	TUSIGN16 result;
	
    VIP_ASSERT(ptrData);
	VIP_ASSERT(length<=NVS_DATALENGTH);

	//Reset data buffer
	//Set data buffer with data pointer received
	//Update write counter
	//lint -e534
	memset(rSrvSegPages[file].data.nvData, 0, NVS_DATALENGTH);
	memcpy(rSrvSegPages[file].data.nvData, ptrData, length);
	rSrvSegPages[file].writeCnt++;
	//lint +e534
	result = PutNvData_Replace(file,FESVR_SEG, (TUSIGN8*)&rSrvSegPages[file],sizeof(NVS_STRUCT));
	if(result != REPLACE_OK)
    {
		//If a communication error occurs, page flag is set invalid
		rSrvSegPages[file].validFlag = NVS_INVALID;
        return NVS_ERROR;
    }
    else
    {
        return NVS_OK;
    }
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief  	Reset Fe Service Page
 		Update Cb Service Page

 \author 	Roberto Colombo
 \date   	2009-03-25
 \param
 \return 
 		TUSIGN16:
 		OK
 		NOT_OK		all other response code are handled as not ok.
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
//--------------------------------------------------------------------------------------------------
TUSIGN16 WritePage_NVSERVICE_REPLACE(TUSIGN8 feature)
{
	TUSIGN16 result = OK;
	TUSIGN16 pageResult;
	TUSIGN8 fileIdx;
	//TUSIGN8 trasducerId[7];

	for(fileIdx = 0; fileIdx<NVS_NUMOF_FILES && result == NVS_OK; fileIdx++)
	{
		if((fileList[fileIdx]->features & feature) == feature)
		{				
			pageResult = ReadData_CHIPHANDLER(NVS_MAIN_PAGE,NVS_SERVICE_SEGNUM-fileIdx,&mSrvSegBuffer);

			//If Cb nv is not iniatilized, Fe service page is initialized with default value
			//Else it is used the value read above
			if(mInitialized && pageResult == CH_OK)
			{
				rSrvSegPages[fileIdx] = mSrvSegBuffer;
			}
			else
			{
				rSrvSegPages[fileIdx] = cmInitServiceSeg;
			}

			if(feature & COMMON){
				rSrvSegPages[fileIdx].data.nvDataExplode.nvCommType = COMMON_BLOCK;
				rSrvSegPages[fileIdx].data.nvDataExplode.nvDbVers = DB_VERS_COMMON;
			}else{ 
				rSrvSegPages[fileIdx].data.nvDataExplode.nvCommType = COMN_TYPE;
				rSrvSegPages[fileIdx].data.nvDataExplode.nvDbVers = DB_VERS_UNCOMMON;
			}
			
			rSrvSegPages[fileIdx].validFlag = NVS_VALID;

		    //Fe serial number
			//Cb serial number
			//Above value are stored into nv data buffer
			
			result |= elettronicServices_Get(ELETTRONICSERVICES_IDX_feSerialNumber,WHOLE_OBJECT,rSrvSegPages[fileIdx].data.nvDataExplode.nvFESerialNumber);
			//lint -e{534}
			result |= elettronicServices_Get(ELETTRONICSERVICES_IDX_cbSerialNumber,WHOLE_OBJECT,rSrvSegPages[fileIdx].data.nvDataExplode.nvCBSerialNumber);

			if(result == OK)
			{
			   result =PutNvData_Replace(fileIdx,FESVR_SEG, (TUSIGN8*)&rSrvSegPages[fileIdx],sizeof(NVS_STRUCT));

				//If no communication error
				//Set both main and backpage
				   //lint --e{730}   
				if(result == OK)
				{
					result = WriteData_NVSERVICE(NVS_MAIN_PAGE,fileIdx,NVS_DATALENGTH,rSrvSegPages[fileIdx].data.nvData,eFALSE);
					result |= WriteData_NVSERVICE(NVS_BACKUP_PAGE,fileIdx,NVS_DATALENGTH,rSrvSegPages[fileIdx].data.nvData,eFALSE);
				}
			}
		}
	}

	return result;
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief  	Set replace service page valid

 \author 	Roberto Colombo
 \date   	2009-04-01
 \param
 \return   TSUGIN16 response of communication
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
//--------------------------------------------------------------------------------------------------
TUSIGN16 SetPageValid_NVSERVICE_REPLACE(TUSIGN8 file)
{

	TUSIGN16 result = OK;

	if(rSrvSegPages[file].validFlag != NVS_VALID)
	{

		rSrvSegPages[file].validFlag = NVS_VALID;
		result = PutNvData_Replace(file,FESVR_SEG, (TUSIGN8*)&rSrvSegPages[file],sizeof(NVS_STRUCT));
		if(result != OK)
		{
			rSrvSegPages[file].validFlag = NVS_INVALID;
		}
	}
	return result;
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief  	Set replace service page invalid
 		
 \author 	Roberto Colombo
 \date   	2009-04-01
 \param
 \return   TSUGIN16 response of communication
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
//--------------------------------------------------------------------------------------------------
TUSIGN16 SetPageInvalid_NVSERVICE_REPLACE(TUSIGN8 file)
{
	TUSIGN16 result = OK;

	if(rSrvSegPages[file].validFlag != NVS_INVALID)
	{
		rSrvSegPages[file].validFlag = NVS_INVALID;
//Todo
		result = PutNvData_Replace(file,FESVR_SEG, (TUSIGN8*)&rSrvSegPages[file],sizeof(NVS_STRUCT));
		if(result != REPLACE_OK)
		{
			rSrvSegPages[file].validFlag = NVS_RESET;
		}
	}
	return result;
}