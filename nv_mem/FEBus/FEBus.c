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
#include "nv_mem/Interface/nv_mem.h"
#include "nv_mem/chip_handler/interface/chip_handler.h"

#ifdef MODULTEST_FEBUS
#include "../NV_module_test/System/Interface/stub.h"
#else
#include "FrontEndInterface/Interface/FrontEndInterface.h"
#endif

#include "FEBus.h"
//add for use FE Function Interface.
FAST TUSIGN8  mReadBufferReplace[CH_SEGMENT_LENGTH];
FAST TUSIGN8  mReplaceFlag;
FAST TUSIGN8  mWaitFlag;

//add by zuochen wang for repplace 
//lint -e641  zuochen wang. 
//-------------------------------------------------------------------------------------------------
/*!
\brief write a segment data to FEBoard. common compitable inteface with 266
\author zuochen Wang
\date 2013-5-17
\param
	fIdx -  file index.
	sgIdx - segment idx in the file.
	pBuf - pointer to receiver data buffuer
	dataLength - data length
\return
\warning
\test
test-date: 2013-07-17
\n by: zuochen wang
\n environment:C-spy
\n intention:
\n result module test:  Pass
\n result Lint Level 3:
\bug
*/
//-------------------------------------------------------------------------------------------------
TUSIGN16  PutNvData_Replace (TUSIGN8 fIdx,TUSIGN8 sgIdx,const TUSIGN8* pBuf,TUSIGN8 dataLength)
{ 
  	TUSIGN16  replaceResult =  REPLACE_ERROR;
  	TUSIGN8 CMD = 0x00;
	TUSIGN8 retry = 0x00;
    VIP_ASSERT(dataLength == CH_SEG_NETTO_LENGTH);

	if(fIdx == FIDX_STATIC_COMMON_REPLACE )
	{
	  if(sgIdx != FESVR_SEG)
	  {
		  CMD =  WRITE_SR_BASE + sgIdx;  
	  }
	  else
	  {
		  CMD = WRITE_SR_SVR; 
	  }
	}
	else if((fIdx == FIDX_CYCLIC_COMMON_REPLACE))
	{
	  if(sgIdx != FESVR_SEG)
	  {
		  CMD = WRITE_CR_BASE + sgIdx;
	  }
	  else
	  {
		  CMD = WRITE_CR_SVR; 
	  }
	}
	else
	{
  		return replaceResult; //Wrong File idx to replace..
	}
	 //data exchange....
	mReplaceFlag = 0x01;//Reset Flags.
	mWaitFlag = 0x00;//Reset Flags.
	(void)memcpy(mReadBufferReplace,pBuf,dataLength);
    (void)frontEndInterface.PutCommand(CMD,dataLength,WriteSegment_Replace,SetFlagFEBus);  //To Pass Module TEST ZUOCHEN
	
	while(mWaitFlag == 0x00)
	{
		Delay_RTOS_TASK(RW_WATI_TICK);//Delay 800ms to check if the value is read successfully.
		retry ++;
		if( retry >= RW_RETRY)
		{
			break;
		}
	}
	
	//if read successfully.
	if( mReplaceFlag == 0x00) 
	{
		replaceResult = REPLACE_OK;
	}
	else
	{
		replaceResult = REPLACE_ERROR;
	}
	
  	return replaceResult;
}
//-------------------------------------------------------------------------------------------------
/*!
\brief Get a segment data from FEBoard. compitable inteface with 266
\author zuochen Wang
\date 2013-5-17
\param
	fIdx -  file index.
	sgIdx - segment idx in the file.
	pBuf - pointer to receiver data buffuer
	dataLength - data length
\return
\warning
\test
test-date: 2013-07-17
\n by: zuochen wang
\n environment:C-spy
\n intention:
\n result module test:  Pass
\n result Lint Level 3:
\bug
*/
//-------------------------------------------------------------------------------------------------
TUSIGN16  GetNvData_Replace (TUSIGN8 fIdx,TUSIGN8 sgIdx,const TUSIGN8* pBuf,TUSIGN8 dataLength)
{ 
  	TUSIGN16  replaceResult =  REPLACE_ERROR;
  	TUSIGN8 CMD = 0x00;
	TUSIGN8 retry = 0x00;
    VIP_ASSERT(dataLength == CH_SEG_NETTO_LENGTH);

	if((fIdx == FIDX_STATIC_COMMON_REPLACE))
	{
	  if(sgIdx != FESVR_SEG)
	  {
		  CMD =  READ_SR_BASE + sgIdx;
	  }
	  else
	  {
		  CMD =  READ_SR_SVR;
	  }
	}
	
	else if((fIdx == FIDX_CYCLIC_COMMON_REPLACE))
	{
	  if(sgIdx != FESVR_SEG)
	  {
		  CMD = READ_CR_BASE + sgIdx;
	  }
	  else
	  {
		  CMD =  READ_CR_SVR;
	  }
	}
	else
	{
  		return replaceResult; //Wrong File idx to replace..
	}
	 //data exchange....
	mReplaceFlag = 0x01;//Reset Flags.
	mWaitFlag = 0x00;//Reset Flags.
	
    (void)frontEndInterface.PutCommand(CMD,dataLength,ReadSegment_Replace,SetFlagFEBus); 
	while(mWaitFlag == 0x00)
	{
		Delay_RTOS_TASK(RW_WATI_TICK);//Delay 800ms to check if the value is read successfully.
		retry ++;
		if( retry >= RW_RETRY)
		{
			break;
		}
	}
	
	//if read successfully.
	if( mReplaceFlag == 0x00) 
	{
		replaceResult = REPLACE_OK;
		(void)memcpy((void*)pBuf,mReadBufferReplace,dataLength);

	}
	else
	{
		replaceResult = REPLACE_ERROR;
	}
	
  	return replaceResult;
}
//-------------------------------------------------------------------------------------------------
/*!
\brief   Will called by FE. to copy data to from fe read buffer.
\author zuochen Wang
\date 2013-5-17
\param 
	length- data length
	pDataBuf- pointer to data buffer.
\return
	void
\warning
\test
test-date: 2013-07-17
\n by: zuochen wang
\n environment:C-spy
\n intention:
\n result module test:  Pass
\n result Lint Level 3:
\bug
*/
//-------------------------------------------------------------------------------------------------
//lint -e{818} 2013-04-16  ignore const pointer because of read/write use same function
void ReadSegment_Replace(TUSIGN8 length,TUSIGN8* const pDataBuf)
{
    SuspendAll_RTOS_TASK();
    if((length != CH_SEG_NETTO_LENGTH))
    {
        ResumeAll_RTOS_TASK();
        return;
    } 
    //lint -e{419} 
    (void)memcpy(mReadBufferReplace,pDataBuf,length);
    ResumeAll_RTOS_TASK();
}


//-------------------------------------------------------------------------------------------------
/*!
\brief   Will called by FE. to copy data to fe write buffer.
\author zuochen Wang
\date 2013-5-17
\param 
	length- data length
	pDataBuf- pointer to data buffer.
\return
	void
\warning
\test
test-date: 2013-07-17
\n by: zuochen wang
\n environment:C-spy
\n intention:
\n result module test:  Pass
\n result Lint Level 3:
\bug
*/
//-------------------------------------------------------------------------------------------------
void WriteSegment_Replace(TUSIGN8 length,TUSIGN8* const pDataBuf)
{
    SuspendAll_RTOS_TASK();
    VIP_ASSERT(length == CH_SEG_NETTO_LENGTH);
    //lint -e{420} 
    (void)memcpy((void*)pDataBuf,mReadBufferReplace,length);
    ResumeAll_RTOS_TASK();
}

//-------------------------------------------------------------------------------------------------
/*!
\brief   Set a flag to indicate if read/write operation is successful.
\author zuochen Wang
\date 2013-5-17
\param flag 0x00 success 0x01 failure.
\return
\warning
\test
test-date: 2013-07-17
\n by: zuochen wang
\n environment:C-spy
\n intention:
\n result module test:  Pass
\n result Lint Level 3:
\bug
*/
//-------------------------------------------------------------------------------------------------
void SetFlagFEBus(TUSIGN8 flag)
{
    SuspendAll_RTOS_TASK();
	mReplaceFlag = flag;//Data Write Successful or not
	mWaitFlag = 0x01; 
    ResumeAll_RTOS_TASK();
}