//-------------------------------------------------------------------------------------------------
/*
<pre>
Copyright              Copyright ABB, 2007.
All rights reserved. Reproduction, modification,
use or disclosure to third parties without express
authority is forbidden.

System         Subsystem FrontEndInterface
Module
Description    Implementation of execute methods
for Subsystems based on T_UNIT.

Remarks
</pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>
#include <intrinsics.h>
#include <string.h>
#include <limits.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "System/Interface/microcontroller.h"
#include "System/Interface/assert.h"
#include "Common/Interface/common_functions.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "FrontEndInterface/Layer2/FED_layer2.h"
#include "frontEndinterface_types.h"
#include "FrontEndInterface/Interface/FrontEndInterface.h"
#include "frontEndinterface_attributes.h"
#include "frontEndinterface_execute.h"
#include "FrontEndInterface/Layer7/FED_layer7.h"

#include "Coordinator/Interface/subsystem_idx.h"
#include "Diagnosis/interface/DiagnosisCompileTimeDefinitions.h"
#include "Diagnosis/interface/t_data_obj_alarm.h"
#include "Diagnosis/interface/t_data_obj_alarm_counter.h"
#include "Diagnosis/interface/Diagnosis_idx.h"
#include "Diagnosis/interface/diagnosis.h"
#include "ElettronicServices/Interface/ElettronicServices.h"
#include "ElettronicServices/Source/dynamicVariablesCheck.h"

static TUSIGN8 commErr;
static TUSIGN8 frameNumOld = 0x00;
static TUSIGN8 frmStuckCnt = 0x00;

extern TBOOL readCalculationFlag ;
extern void readCalculationData_cdt(TUSIGN8 length,  TUSIGN8* const pDataBuf);

static Buffer_STRUCT cmdBuffer;


static TUSIGN8 retryTimes = 0;       //use to count retry time, reset after 3 times or command finish
static TUSIGN16 totErrTimes = 0;    //use to count the alarm, reset when com ok
static TUSIGN8 resetCnt = 0;           //use to count the reset, if FE not on will recount, reset when com ok.
static TUSIGN8 queueCrcErrTimes = 0;
static TUSIGN16 feOldDiagnosis = 0;     

const T_DYNAMIC_CHECK_TABLE FEDynamicVariablesTable[2] = 
{
	{(void *)&frontEndInterfaceDynamicDuplicated.frontendDiagnosis, 		(void *)&frontEndInterfaceDynamicDuplicated.frontendDiagnosisDuplicated, 	        sizeof(frontEndInterfaceDynamicDuplicated.frontendDiagnosis)},
	{(void *)&frontEndInterfaceDynamicDuplicated.frontendAlarmSimulation, 	(void *)&frontEndInterfaceDynamicDuplicated.frontendAlarmSimulationDuplicated, 	 sizeof(frontEndInterfaceDynamicDuplicated.frontendAlarmSimulation)},
};

void (*ReadFunc[])(const CMD_MSG* pMessage) =
{ 
    ReadData_layer7
};  

void (*WriteFunc[])(const CMD_MSG* pMessage) =
{  
	WriteData_layer7,
};  

const SLOW TUSIGN16 _FE_LOCAL_DIAGNOSIS_ALARM_TABLE[FE_DIAG_SIZE] =
{
	FE_LOCAL_BOARD_COMM_ERROR
};

//-------------------------------------------------------------------------------------------------
/*!
\if @SubGen start@ <METHODHEADER READCOMCMD> \endif
\brief  read data command
\param  pMessage 
\if @SubGen end@ <METHODHEADER READCOMCMD> \endif
\author
\date
\param void
\return void
\warning
\test
test-date: 2004-MM-DD
\n by: pp ss
\n environment:
\n intention:
\n result module test:
\n result Lint Level 3:
\bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD READCOMCMD>
void ReadComCmd_FE(const CMD_MSG* pMessage)
//@SubGen end@<METHOD READCOMCMD>
{
	VIP_ASSERT(pMessage);
	(ReadFunc[0])(pMessage);
}


//-------------------------------------------------------------------------------------------------
/*!
\if @SubGen start@ <METHODHEADER WRITECOMCMD> \endif
\brief  write data command
\param  pMessage 
\if @SubGen end@ <METHODHEADER WRITECOMCMD> \endif
\author
\date
\param void
\return void
\warning
\test
test-date: 2004-MM-DD
\n by: pp ss
\n environment:
\n intention:
\n result module test:
\n result Lint Level 3:
\bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD WRITECOMCMD>
void WriteComCmd_FE(const CMD_MSG* pMessage)
//@SubGen end@<METHOD WRITECOMCMD>
{
	VIP_ASSERT(pMessage);
    (WriteFunc[0])(pMessage);
}


//-------------------------------------------------------------------------------------------------
/*!
\if @SubGen start@ <METHODHEADER CMDPUTCOMMAND> \endif
\brief  put the cmd message into the command queues
\param  cmdnum 
\param  datalen 
\param  (*pFunc)(TUSIGN8 length,TUSIGN8* const pDataBuf) 
\param  (*pFuncFlag)(TUSIGN8 flag) 
\if @SubGen end@ <METHODHEADER CMDPUTCOMMAND> \endif
\author
\date
\param void
\return void
\warning
\test
test-date: 2004-MM-DD
\n by: pp ss
\n environment:
\n intention:
\n result module test:
\n result Lint Level 3:
\bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD CMDPUTCOMMAND>
TINT16 PutCommand_FE(TUSIGN8 cmdnum, TUSIGN8 datalen, void (*pFunc)(TUSIGN8 length,TUSIGN8* const pDataBuf), void (*pFuncFlag)(TUSIGN8 flag))
//@SubGen end@<METHOD CMDPUTCOMMAND>
{
    TINT16 ReturnCode;
    TINT8 retry_cnt = 3;
	CMD_MSG cmdMsg;
	TUSIGN16 crc = InitCRC16_COMMON();
	VIP_ASSERT(pFunc);
    cmdMsg.cmd = cmdnum;
	cmdMsg.datalen = datalen;
	cmdMsg.procData = pFunc;
    cmdMsg.setFlag = pFuncFlag;
	cmdMsg.crc = CalcCrc16Mem_COMMON((TUSIGN8*)&cmdMsg,crc,(sizeof(CMD_MSG)-4));
    ReturnCode = WriteFEBuffer(&cmdMsg);
	while(ReturnCode)
    {
        Delay_RTOS_TASK(10);
		retry_cnt--;
		if(retry_cnt <= 0)
		{
            ReturnCode = (TINT16)WRONG_STATE_ERR;
			break;
		}  
		ReturnCode = WriteFEBuffer(&cmdMsg);     
    }
  	return ReturnCode;
}



//-------------------------------------------------------------------------------------------------
/*!
\if @SubGen start@ <METHODHEADER CHECKFEREADYSRV> \endif
 \brief  Check if the FE is ready for communcation
\if @SubGen end@ <METHODHEADER CHECKFEREADYSRV> \endif
\author spring zhou
\date 2012-8-17
\param void
\return void
\warning
\test
test-date: 2012-08-11
\n by: spring kunli.zhou
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: OK
\bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD CHECKFEREADYSRV>
TBOOL CheckFEreadySRV_FE(void)
//@SubGen end@<METHOD CHECKFEREADYSRV>
{
	return (HAL_GPIO_ReadPin(FE_HANDSHAKE_IN_GPIO_PORT, FE_HANDSHAKE_IN_GPIO_PIN) == GPIO_PIN_RESET);
}


//-------------------------------------------------------------------------------------------------
/*!
\if @SubGen start@ <METHODHEADER UPDATEDIAGDATA> \endif
 \brief  Update the diagnosis subsystem alarm object
 \param  setDiag 
 \param  clearDiag 
\if @SubGen end@ <METHODHEADER UPDATEDIAGDATA> \endif
\author
\date 2012-8-17
\return void
\warning
\test
test-date: 2012-08-11
\n by: spring kunli.zhou
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: OK
\bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD UPDATEDIAGDATA>
void UpdateDiagData_FE(TUSIGN16 setDiag, TUSIGN16 clearDiag)
//@SubGen end@<METHOD UPDATEDIAGDATA>
{
    TUSIGN8 i;
    for(i = 0; i < FE_DIAG_SIZE; i++)
    {
        if(setDiag & _FE_LOCAL_DIAGNOSIS_ALARM_TABLE[i]) 
        {
            SetDiagnosis_FE(_FE_DIAGNOSIS_ALARM_TABLE[i]);
        }
        else if(clearDiag & _FE_LOCAL_DIAGNOSIS_ALARM_TABLE[i])  
        {
            ClearDiagnosis_FE(_FE_DIAGNOSIS_ALARM_TABLE[i]);
        }
		else
		{
		}
    }
}

//-------------------------------------------------------------------------------------------------
/*!
\if @SubGen start@ <METHODHEADER UPDATEDIAGNOSIS> \endif
 \brief  update alarm to diagnosis subsystem
\if @SubGen end@ <METHODHEADER UPDATEDIAGNOSIS> \endif
\author spring kunli.zhou
\date 2012-8-17
\param void
\return void
\warning
\test
test-date: 2012-08-11
\n by: spring kunli.zhou
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: OK
\bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD UPDATEDIAGNOSIS>
void UpdateDiagnosis_FE(void)
//@SubGen end@<METHOD UPDATEDIAGNOSIS>
{
    TUSIGN16 result;
    TUSIGN16 alarm,setDiagnosis, clearDiagnosis;
    TUSIGN16 alarmSimulation = 0x0000;
    TUSIGN8 diagnosisSimulationStatus;
    
	ENABLE_DATA_PROTECTION(*(frontEndInterface.unit.ptrDataSemaphore));
    alarm = frontEndInterfaceDynamicDuplicated.frontendDiagnosis;
	DISABLE_DATA_PROTECTION(*(frontEndInterface.unit.ptrDataSemaphore));
    result = diagnosis_Get((TUSIGN16)DIAGNOSIS_IDX_simulationStatus,(TINT16)WHOLE_OBJECT,&diagnosisSimulationStatus);
    if((diagnosisSimulationStatus == DIAGNOSIS_SIMULATION_ENABLED)&& (result == (TINT16)OK))
    {
    	ENABLE_DATA_PROTECTION(*(frontEndInterface.unit.ptrDataSemaphore));
        alarmSimulation = frontEndInterfaceDynamicDuplicated.frontendAlarmSimulation;
		DISABLE_DATA_PROTECTION(*(frontEndInterface.unit.ptrDataSemaphore));
        alarm = alarmSimulation;
    }
	setDiagnosis = alarm & ~feOldDiagnosis;
	clearDiagnosis = ~alarm & feOldDiagnosis;
    
    feOldDiagnosis = alarm;
    
    frontEndInterface.UpdateDiagData(setDiagnosis, clearDiagnosis); 
    
    //=================diagnosis frontend reset process================//
    if((resetCnt > MAXERRORCNT))
    {	
    	if(frontEndInterface.CheckFEreadySRV())
		{	
            FE_RESET_ENABLE();
			Delay_RTOS_TASK(20);
            FE_RESET_DISABLE();
		}
		resetCnt = 0;
	}
} 


//-------------------------------------------------------------------------------------------------
/*!
\if @SubGen start@ <METHODHEADER SETDIAGNOSIS> \endif
 \brief  Set diagnosis
 \param  attributeIdx 
\if @SubGen end@ <METHODHEADER SETDIAGNOSIS> \endif
\author spring kunli.zhou
\date 2012-8-17
\return void
\warning
\test
test-date: 2012-08-11
\n by: spring kunli.zhou
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: OK
\bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD SETDIAGNOSIS>
void SetDiagnosis_FE(TUSIGN16 attributeIdx)
//@SubGen end@<METHOD SETDIAGNOSIS>
{
   	//set external
   	TUSIGN8 value=SET_ALARM;
   	VIP_ASSERT((TINT16)attributeIdx >= 0);
   	VIP_ASSERT( (TUSIGN16)OK == diagnosis_Put((TUSIGN16)DIAGNOSIS_IDX_frontendinterfaceAlarm, (TINT16)attributeIdx, &value) );
}


//-------------------------------------------------------------------------------------------------
/*!
\if @SubGen start@ <METHODHEADER CLEARDIAGNOSIS> \endif
 \brief  Clear diagnosis
 \param  attributeIdx 
\if @SubGen end@ <METHODHEADER CLEARDIAGNOSIS> \endif
\author spring kunli.zhou
\date 2012-8-17
\param void
\return void
\warning
\test
test-date: 2012-08-11
\n by: spring kunli.zhou
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: OK
\bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD CLEARDIAGNOSIS>
void ClearDiagnosis_FE(TUSIGN16 attributeIdx)
//@SubGen end@<METHOD CLEARDIAGNOSIS>
{
	//clear external
	TUSIGN8 value=CLEAR_ALARM;
	VIP_ASSERT((TINT16)attributeIdx >= 0 );
	VIP_ASSERT( (TUSIGN16)OK == diagnosis_Put((TUSIGN16)DIAGNOSIS_IDX_frontendinterfaceAlarm, (TINT16)attributeIdx, &value) );
}


//-------------------------------------------------------------------------------------------------
/*!
\if @SubGen start@ <METHODHEADER COMMANDINTERPRETER> \endif
 \brief  interpreter frame and process data
 \param  pMessage 
\if @SubGen end@ <METHODHEADER COMMANDINTERPRETER> \endif
\author spring kunli.zhou
\date   2012-09-12  
\param pMessage - the pointer of command message
\return void
\warning
\test
test-date: 2012-08-11
\n by: spring kunli.zhou
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: OK
\bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD COMMANDINTERPRETER>
void InterpreterCmd_FE(const CMD_MSG* pMessage)
//@SubGen end@<METHOD COMMANDINTERPRETER>
{
	TUSIGN8 retCrcErr = FRAME_OK;
	TUSIGN8 retFrameNum = FRAMENUM_OK;
	TUSIGN8 retStatus = FRAME_OK;
	VIP_ASSERT(pMessage);
    
	if(commErr == FECOMOK)  			//com not overtime
	{
		retCrcErr = FrameCRCCheck();
		if(retCrcErr == FRAME_OK)      //frame crc ok
		{
			retStatus = CheckMessageStatus();
			if(retStatus == FRAME_OK)   //message status ok
			{
				retFrameNum = CheckFrameNum();
				if(retFrameNum == FRAMENUM_OK)   //frame number ok
				{	
					if(totErrTimes >= 9)
					{
						totErrTimes = 8;
					}
					else if(totErrTimes > 0)
					{
						totErrTimes--;
					}
					else
					{}
					resetCnt = 0;
					if(pMessage->setFlag != NULL)
					{
						pMessage->setFlag(FECOMOK);  
					}
					frontEndInterface.ProcessComData(pMessage); 	//receive ok	
				}
				else			//frame number not ok
				{
					commErr = FECOMFAIL;	
					retryTimes++;
					resetCnt++;
					totErrTimes++;					
				}
			}
			else    //message status ok error
			{
				commErr = FECOMFAIL;
				retryTimes++;
				totErrTimes++;
				resetCnt++;

				switch(retStatus)
				{
					case FRAME_FE_CRC_ERROR:			
                    break;

					case FRAME_FE_ACCESS_ERROR: 							
                    break;

					default:break;	
				}
			}
		}
		else       //frame crc not ok
		{
			commErr = FECOMFAIL;
			retryTimes++;
			totErrTimes	++;
			resetCnt++;		 			
		}
	}
	else         //com overtime
	{
		totErrTimes++;
		retryTimes++;
		resetCnt++;	
	}
	
	if(totErrTimes > 65530) 
	{
		totErrTimes = 9;
	}
	ENABLE_DATA_PROTECTION(*(frontEndInterface.unit.ptrDataSemaphore));
	if(totErrTimes >= 9 )
	{
		frontEndInterfaceDynamicDuplicated.frontendDiagnosis |= (1<<FE_BOARD_COMM_ERROR);
		frontEndInterfaceDynamicDuplicated.frontendDiagnosisDuplicated |= (1<<FE_BOARD_COMM_ERROR);
	}
	else if(totErrTimes == 0)
	{
		frontEndInterfaceDynamicDuplicated.frontendDiagnosis &= (~(1<<FE_BOARD_COMM_ERROR));
		frontEndInterfaceDynamicDuplicated.frontendDiagnosisDuplicated &= (~(1<<FE_BOARD_COMM_ERROR));
	}
	else
	{}
	DISABLE_DATA_PROTECTION(*(frontEndInterface.unit.ptrDataSemaphore));
}


//-------------------------------------------------------------------------------------------------
/*!
\if @SubGen start@ <METHODHEADER COMDATAPROCESS> \endif
 \brief  process the data
 \param  pMessage 
\if @SubGen end@ <METHODHEADER COMDATAPROCESS> \endif
\author spring kunli.zhou
\date 2012-8-17
\param pMessage -point of command message 
\return void
\warning
\test
test-date: 2012-08-11
\n by: spring kunli.zhou
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: OK
\bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD COMDATAPROCESS>
void ProcessComData_FE(const CMD_MSG* pMessage)
//@SubGen end@<METHOD COMDATAPROCESS>
{
	//don't need parse msgType ,data must be ok.
	//parse msgType  
	VIP_ASSERT(pMessage);
	VIP_ASSERT(pMessage->datalen < FEB_XMIT_RCV_BUFFER_LEN);
	VIP_ASSERT(pMessage->procData);
	if(pMessage->cmd < READ_CMD_END)
	{   
		pMessage->procData(pRxBuf->datalen,&pRxBuf->arydata[0]);
	}
}

//-------------------------------------------------------------------------------------------------
/*!
\if @SubGen start@ <METHODHEADER CMDPROCESS> \endif
 \brief  process the FEI com commands
\if @SubGen end@ <METHODHEADER CMDPROCESS> \endif
\author spring kunli.zhou
\date 2012-8-17
\param void
\return void
\warning
\test
test-date: 2012-08-11
\n by: spring kunli.zhou
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: OK
\bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD CMDPROCESS>
void ProcessCmd_FE(void)
//@SubGen end@<METHOD CMDPROCESS>
{
	const TUSIGN32 FE_RESET_LAST_TIME_MS = 8000;  //if fe resets, the handshake pin can remain not ready until about 8 seconds.

    TINT16 result;
    RTOS_TASK_NOTIFICATION_VALUE evtResult = 0;
    CMD_MSG Message;
	TBOOL lFeIsReady = eFALSE;
	TUSIGN32 lSystemTime = 0;
	TUSIGN32 lDeltaTime = 0;
	static TUSIGN32 lLastTimeStampOfFEReady = 0;

	lFeIsReady = frontEndInterface.CheckFEreadySRV();
     
    if(lFeIsReady)
	{
        result = ReadFEBuffer(&Message);
	  	if((result== FE_BUFFER_OK)||((result == FE_BUFFER_EMPTY)&&(readCalculationFlag == eTRUE)))
	   	{
	        //send buffer command
            if((result == FE_BUFFER_EMPTY))
            {

                 Message.cmd = FE_READ_CALCULATION_DATA;
                 Message.datalen = FE_CALCULATION_DATA_LENGTH;
                 Message.procData = readCalculationData_cdt;
                 Message.setFlag = NULL;
            }
            queueCrcErrTimes = 0;
            frontEndInterface.SendCmd(&Message);  
            //wait for receive finished
            Wait_RTOS_TASK_NOTIFY(0, FERX_EVENT, &evtResult, RTOS_TICKS_FROM_MS(FERX_TIMEOUT));
            if(evtResult & FERX_EVENT)
            {
                commErr = FECOMOK;
                evtResult &= ~FERX_EVENT;
            }
            else 
            {
                commErr = FECOMFAIL;
            }
            
	   		frontEndInterface.InterpreterCmd(&Message);  //interpreter the reply data
	        if((retryTimes > MAX_RETRY)||(commErr == FECOMOK)) //only when comok or finish retry, then delete the current buffer
	        {
	            if((retryTimes > MAX_RETRY)&&(Message.setFlag != NULL))
	            {
	                Message.setFlag(FECOMFAIL);
	            }
	            /*lint -e534*/
                if( Message.cmd != FE_READ_CALCULATION_DATA)
                {
                    ClearFEBuffer();
                }            
	            retryTimes = 0;
		    }
            //check frame if stuck at.
            if(GetFrameNum() == frameNumOld)
            {
                frmStuckCnt++;
                if(frmStuckCnt >= 0x05)
                {
                  elettronicServices.ExitSRV();
                }
            }
            else
            {
                frmStuckCnt=0;
            }
            
            frameNumOld = GetFrameNum();

	    }
	    else if(result == FE_BUFFER_CRCERR)
	    {
	        if(Message.setFlag != NULL)
	        {
	            Message.setFlag(FECOMFAIL);
	        }
	        ClearFEBuffer();
	        /*lint +e534*/
	        queueCrcErrTimes++;
	        if(queueCrcErrTimes > MAX_BUFFER_CRCERR_CNT)
	        {
	            elettronicServices.ExitSRV();
	        }
	    }
        else
        {
        }        
    }
   //lint +e731 zuochen wang  
	else
	{		 
	}

	// added by Jaden, 20200521:
	// if fe reset and back to normal in time, do not raise communication error
	lSystemTime = GetTickCount_RTOS_TASK();
	if (lFeIsReady)
	{
		lLastTimeStampOfFEReady = lSystemTime;
	}
	else
	{
		lDeltaTime = (lSystemTime >= lLastTimeStampOfFEReady) ? (lSystemTime - lLastTimeStampOfFEReady) : ((0xFFFFFFFF - lLastTimeStampOfFEReady) + lSystemTime);

		if (lDeltaTime >= FE_RESET_LAST_TIME_MS)
		{
			ENABLE_DATA_PROTECTION(*(frontEndInterface.unit.ptrDataSemaphore));
			frontEndInterfaceDynamicDuplicated.frontendDiagnosis |= (1<<FE_BOARD_COMM_ERROR);
			frontEndInterfaceDynamicDuplicated.frontendDiagnosisDuplicated |= (1<<FE_BOARD_COMM_ERROR);
			DISABLE_DATA_PROTECTION(*(frontEndInterface.unit.ptrDataSemaphore));

			/* reset FE to back FE to normal state */
			FE_RESET_ENABLE();
			Delay_RTOS_TASK(10);
            FE_RESET_DISABLE();

			lLastTimeStampOfFEReady = lSystemTime;
		}
	}
}

//-------------------------------------------------------------------------------------------------
/*!
\if @SubGen start@ <METHODHEADER SENDCMD> \endif
 \brief  send queue command
 \param  pMessage 
\if @SubGen end@ <METHODHEADER SENDCMD> \endif
\author spring kunli.zhou
\date 2012-8-17
\param pMessage-point of command message 
\return void
\warning
\test
test-date: 2012-08-11
\n by: spring kunli.zhou
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: OK
\bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD SENDCMD>
void SendCmd_FE(const CMD_MSG* pMessage)
//@SubGen end@<METHOD SENDCMD>
{
	TUSIGN8 cmd;
    
	VIP_ASSERT(pMessage);
	commErr = FECOMOK;
    cmd = pMessage->cmd;
    
    if(cmd <= READ_CMD_END)
    {
        ReadComCmd_FE(pMessage);
    }
    else
    {
        WriteComCmd_FE(pMessage);
    }
}

//-------------------------------------------------------------------------------------------------
/*!
\brief  WriteFEBuffer - put message into buffer
\author spring kunli.zhou
\date 2013-4-28
\param cmdMsg-point of command message 
\return TINIT16  FE_BUFFER_FULL
FE_BUFFER_OK
\warning
\test
test-date: 2013-05-08
\n by: spring kunli.zhou
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: OK
\bug
*/
//-------------------------------------------------------------------------------------------------
TINT16 WriteFEBuffer(const CMD_MSG* cmdMsg)
{
	TINT16 result = FE_BUFFER_FULL;
	if(IsFEBuferEnpty() < MAX_Q_SIZE)
	{	
		SuspendAll_RTOS_TASK();
		(void)memcpy(&cmdBuffer.memBuf[cmdBuffer.putIndex],cmdMsg,sizeof(CMD_MSG));
		cmdBuffer.putIndex++;
		if(cmdBuffer.putIndex == MAX_Q_SIZE)
		{
			cmdBuffer.putIndex = 0;
		}
		cmdBuffer.size++;
		result = FE_BUFFER_OK;
		ResumeAll_RTOS_TASK();
	}
	return result;
}
//-------------------------------------------------------------------------------------------------
/*!
\brief  ReadFEBuffer - get buffer message
\author spring kunli.zhou
\date 2013-4-28
\param cmdMsg-point of command message 
\return TINIT16  FE_BUFFER_EMPTY
FE_BUFFER_OK
FE_BUFFER_CRCERR
\warning
\test
test-date: 2013-05-08
\n by: spring kunli.zhou
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: OK
\bug
*/
//-------------------------------------------------------------------------------------------------
TINT16 ReadFEBuffer(CMD_MSG* cmdMsg)
{
	TINT16 result = FE_BUFFER_EMPTY;
	TUSIGN16 tempcrc = InitCRC16_COMMON();
    if(IsFEBuferEnpty() > 0)
    {
    	// do not need to data process protect, because WriteFEBuffer will not change the purge related data
        tempcrc = CalcCrc16Mem_COMMON((TUSIGN8*)&cmdBuffer.memBuf[cmdBuffer.purgeIndex],tempcrc,(sizeof(CMD_MSG)-4));
		cmdMsg->cmd = cmdBuffer.memBuf[cmdBuffer.purgeIndex].cmd;
		cmdMsg->datalen = cmdBuffer.memBuf[cmdBuffer.purgeIndex].datalen;
		cmdMsg->procData = cmdBuffer.memBuf[cmdBuffer.purgeIndex].procData;
		cmdMsg->setFlag = cmdBuffer.memBuf[cmdBuffer.purgeIndex].setFlag;
		cmdMsg->crc = cmdBuffer.memBuf[cmdBuffer.purgeIndex].crc;
		
        if(tempcrc == cmdBuffer.memBuf[cmdBuffer.purgeIndex].crc)
        {
            result = FE_BUFFER_OK;
        }
        else
        {
            result = FE_BUFFER_CRCERR;
        }
    }
	return result;
}

//-------------------------------------------------------------------------------------------------
/*!
\brief  ClearFEBuffer - Purge buffer message
\author spring kunli.zhou
\date 2013-4-28
\param void
\return TINIT16  FE_BUFFER_EMPTY
FE_BUFFER_OK
\warning
\test
test-date: 2013-05-08
\n by: spring kunli.zhou
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: OK
\bug
*/
//-------------------------------------------------------------------------------------------------
TINT16 ClearFEBuffer(void)
{
	TINT16 result = FE_BUFFER_EMPTY;
	if((IsFEBuferEnpty() > 0))
	{
		SuspendAll_RTOS_TASK();
	    (void)memset(&cmdBuffer.memBuf[cmdBuffer.purgeIndex],0,sizeof(CMD_MSG));
	  	cmdBuffer.purgeIndex++;
	    if(cmdBuffer.purgeIndex == MAX_Q_SIZE)
	    {
	        cmdBuffer.purgeIndex = 0;
	    }
	    if(cmdBuffer.purgeIndex == cmdBuffer.putIndex)
	    {
	        cmdBuffer.size = 0;
	    }
	    else
	    {
	        cmdBuffer.size--;
	    }
	    result = FE_BUFFER_OK;
		ResumeAll_RTOS_TASK();
    }
	return result;
}

//-------------------------------------------------------------------------------------------------
/*!
\brief  IsFEBuferEnpty - Check buffer size
\author spring kunli.zhou
\date 2013-4-28
\param void
\return TUSIGN8 buffer
\warning
\test
test-date: 2013-05-08
\n by: spring kunli.zhou
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: OK
\bug
*/
//-------------------------------------------------------------------------------------------------

TUSIGN8 IsFEBuferEnpty()
{
    TUSIGN8 result = 0;
	SuspendAll_RTOS_TASK();
	result = cmdBuffer.size;
	ResumeAll_RTOS_TASK();
    return result;
}


//-------------------------------------------------------------------------------------------------
/*!
\brief  InitFEBuffer - Initialize buffer data
\author spring kunli.zhou
\date 2013-4-28
\param void
\return void
\warning
\test
test-date: 2013-05-08
\n by: spring kunli.zhou
\n environment:IAR Embedded Workbench V3.40 C-SPY
\n intention:100% Code coverage
\n result module test: Pass, 100% code coverage
\n result Lint Level 3: OK
\bug
*/
//-------------------------------------------------------------------------------------------------
void InitFEBuffer()
{
	(void)memset(cmdBuffer.memBuf,0,sizeof(CMD_MSG)*MAX_Q_SIZE);
	cmdBuffer.putIndex = 0;
	cmdBuffer.purgeIndex = 0;
	cmdBuffer.size = 0;
}




/*
This section defines templates for the SubSystem CodeGenerator.
Do not modify or remove this section


//@SubGen template start@ <METHOD>
//-------------------------------------------------------------------------------------------------
@SubGen OpenComment@!
\if @SubGen start@ <METHODHEADER TEMPLATE> \endif
\if @SubGen end@ <METHODHEADER TEMPLATE> \endif
\author
\date
\param void
\return void
\warning
\test
test-date: 2004-MM-DD
\n by: pp ss
\n environment:
\n intention:
\n result module test:
\n result Lint Level 3:
\bug
@SubGen CloseComment@
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD TEMPLATE>
//@SubGen end@<METHOD TEMPLATE>
{
}
//@SubGen template end@ <METHOD>

*/