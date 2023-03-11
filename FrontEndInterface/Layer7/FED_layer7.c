//--------------------------------------------------------------------------------------------------
/*
 Copyright      Copyright ABB, 2011.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Subsystem frontend
 Module
 Remarks


  $Author: $  Spring kunli.zhou

*/
//--------------------------------------------------------------------------------------------------

// General includes

#include <intrinsics.h>
#include <limits.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "System/Interface/assert.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"

#include "FrontEndInterface/Interface/FrontEndInterface.h"
#include "FrontEndInterface/Layer7/FED_layer7.h"
#include "FrontEndInterface/Layer2/FED_layer2.h"

//------------------------------------------------------------------------------------

//! modul variables

#pragma location="STATIC_INTERRUPT_DATA"
__no_init FAST T_FED_FRAME_TX_BUFFER uTransmitBuffer_FED_layer7;   //Send Buffer With frame Structure
#pragma location="STATIC_INTERRUPT_DATA"
__no_init FAST T_FED_FRAME_RX_BUFFER uReceiveBuffer_FED_layer7;    //receiver Buffer With frame Structure


FAST T_FED_FRAME_RX_BUFFER *pRxBuf=&uReceiveBuffer_FED_layer7;
FAST T_FED_FRAME_TX_BUFFER *pTxBuf=&uTransmitBuffer_FED_layer7; 

static TUSIGN8 frameNum = 0;
static TUSIGN8 UpdateFrameNum(void);
//--------------------------------------------------------------------------
/*
 \brief  .  Common Read data . 
 \date   2012-08-30
 \author Spring
 \param -subCmd: cmdnum
 \              ptrData:  point to put the data
 \return -void
 \warning
 \test
 test-date: 2012-08-30
 \n by: spring kunli.zhou
 \n environment:IAR Embedded Workbench V3.40 C-SPY
 \n intention:100% Code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3: OK
 \bug
*/
//--------------------------------------------------------------------------
void ReadData_layer7(const CMD_MSG* pMessage)
{
	VIP_ASSERT(pMessage);
    pTxBuf->msgtype = FED_FRAMETYPE_RD_CMD|FED_FRAMETYPE_SERIAL_DATAOK;     //MSG TYPE
    pTxBuf->framenum = UpdateFrameNum(); 
    pTxBuf->cmd = pMessage->cmd;            //cmd num
    pTxBuf->datalen = 0;
    SendFrame_LAYER2();
}


//--------------------------------------------------------------------------
/*
 \brief  .  Common write data . 
 \date   2012-08-30
 \param -subCmd: cmdnum
 \              ptrData:  point to get the data
 \return -void
 \warning
 \test
 test-date: 2012-08-30
 \n by: spring kunli.zhou
 \n environment:IAR Embedded Workbench V3.40 C-SPY
 \n intention:100% Code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3: OK
 \bug
*/
//--------------------------------------------------------------------------
void WriteData_layer7(const CMD_MSG* pMessage)
{
	VIP_ASSERT(pMessage);
    pTxBuf->msgtype = FED_FRAMETYPE_WR_CMD|FED_FRAMETYPE_SERIAL_DATAOK;     //MSG TYPE
    pTxBuf->framenum = UpdateFrameNum(); 
    pTxBuf->cmd = (pMessage->cmd - 0x80);           				//cmd num
    pTxBuf->datalen = pMessage->datalen;
    pMessage->procData(pMessage ->datalen, &pTxBuf->arydata[0]);
    SendFrame_LAYER2();
}

//--------------------------------------------------------------------------
/*!
 \brief  Update the frame num
 \date   2011-07-14
 \param -
 \return TUSIGN8  framenum
 \warning
 \test
 test-date: 2012-08-30
 \n by: spring kunli.zhou
 \n environment:IAR Embedded Workbench V3.40 C-SPY
 \n intention:100% Code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3: OK
 \bug
*/
//--------------------------------------------------------------------------
static TUSIGN8 UpdateFrameNum(void)
{
	if(frameNum == 0xFF)
	{
		frameNum = 0;
	}
	else
	{
		frameNum++;
	}
	return frameNum;
}

//--------------------------------------------------------------------------
/*!
 \brief  Return current the frame num
 \date   2011-07-14
 \param -
 \return TUSIGN8  framenum
 \warning
 \test
 test-date: 2012-08-30
 \n by: spring kunli.zhou
 \n environment:IAR Embedded Workbench V3.40 C-SPY
 \n intention:100% Code coverage
 \n result module test: Pass, 100% code coverage
 \n result Lint Level 3: OK
 \bug
*/
//--------------------------------------------------------------------------
TUSIGN8 GetFrameNum(void)
{
	return frameNum;
}