//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2011.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Hart
 Module
 Description    Implementation of execute methods
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#include <limits.h>
#include <string.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#ifdef LOCALSERVER_SELECT
#include "UnitTests/LocalServer/webserver_main.h"
#endif
#include "RTOS/Interface/rtos.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"


// Get the SUBSYSTEM IDs for local usage
// @@adjust
#include "Coordinator/Interface/Coordinator.h"
#include "Coordinator/Interface/subsystem_idx.h"

#include "nv_mem/Interface/nv_mem.h"
#include "nv_mem/chip_handler/interface/chip_handler.h"
//ElectronicServices
#include "ElettronicServices/Interface/ElettronicServices.h"

#include "HART/Interface/Hart.h"
#include "HART/Interface/Hart_idx.h"

#include "HART/Layer2/Interface/layer2.h"
#include "HART/Layer7/Source/ObjfunctRB.h"
#include "HART/Layer7/Source/cmddef.h"
#include "HART/Interface/t_data_obj_burstmsg_confvalues.h"
#include "HART/Interface/t_data_obj_burstmsg_managevalues.h"

#include "HART/Layer7/Interface/layer7.h"
#include "hart_types.h"
#include "hart_attributes.h"

//@SubGen put@ <METHOD>
//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER HART_PUTRECEIVEBUFFER_SRV> \endif
\brief  other subsystem put data to HART Receive Buffer
\param  systemIdx 
\param  processCmd 
\param  ptrArray 
\param  arrayLength 
 \if @SubGen end@ <METHODHEADER HART_PUTRECEIVEBUFFER_SRV> \endif
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
//@SubGen start@<METHOD HART_PUTRECEIVEBUFFER_SRV>
TBOOL HART_PutReceiveBuffer_SRV_HART(TUSIGN16 systemIdx, TUSIGN8 processCmd, TUSIGN8* ptrArray, TUSIGN8 arrayLength)
//@SubGen end@<METHOD HART_PUTRECEIVEBUFFER_SRV>
{
    TUSIGN8* pData;
    TUSIGN8 lengthOffset = 0;
    RTOS_BASETYPE xHigherPriorityTaskWoken = RTOS_FALSE;
    
    /*parameter check*/
    if(ptrArray == NULL)
    {
      return eFALSE;
    }
    
    if(systemIdx == SERVICEPORT_IDX)
    {
        switch(processCmd)
        {
        case HART_SP_SAVE_RX_DATA:
            /* array length check*/
            if(arrayLength<6)
            {
                return eFALSE;
            }
            //clear the old buffer data
            if(HART_SPbufferState!=SP_BUFFER_STATE_FREE)
            {
                HART_SPbufferState = SP_BUFFER_STATE_FREE;
            }    
            
            HART_SPreceiveBuffer.aryExp[0] = 0;
            HART_SPreceiveBuffer.aryExp[1] = 0;
            HART_SPreceiveBuffer.aryExp[2] = 0;
            HART_SPreceiveBuffer.aryAdr[0] = 0;
            HART_SPreceiveBuffer.aryAdr[1] = 0;
            HART_SPreceiveBuffer.aryAdr[2] = 0;
            HART_SPreceiveBuffer.aryAdr[3] = 0;
            HART_SPreceiveBuffer.aryAdr[4] = 0;
            HART_SPreceiveBuffer.firstResp = 0;
            HART_SPreceiveBuffer.secResp = 0;
            
            HART_SPreceiveBuffer.delimiter = ptrArray[0];
            if(HART_SPreceiveBuffer.delimiter&0x80)
            {//long address
                if(((ptrArray[1]&0x3F)!=s_addrLong[0])
                || (ptrArray[2]!=s_addrLong[1])
                || (ptrArray[3]!=s_addrLong[2])
                || (ptrArray[4]!=s_addrLong[3])
                || (ptrArray[5]!=s_addrLong[4]))
                {
                    //not send any response message
                    return eFALSE;
                }
                
                HART_SPreceiveBuffer.aryAdr[0] = ptrArray[1];
                HART_SPreceiveBuffer.aryAdr[1] = ptrArray[2];
                HART_SPreceiveBuffer.aryAdr[2] = ptrArray[3];
                HART_SPreceiveBuffer.aryAdr[3] = ptrArray[4];
                HART_SPreceiveBuffer.aryAdr[4] = ptrArray[5];
                pData = &ptrArray[6];
                lengthOffset = 6;
            }
            else
            {
                if((ptrArray[1]&0x0F)!=s_addr) 
                {
                    return eFALSE;
                }
                HART_SPreceiveBuffer.aryAdr[0] = ptrArray[1];
                pData = &ptrArray[2];
                lengthOffset = 2;
            }     
            
            HART_SPreceiveBuffer.command = *pData++;
            HART_SPreceiveBuffer.numByte = *pData++;
            /*array length check*/
            if(HART_SPreceiveBuffer.numByte > (arrayLength - lengthOffset - 2))
            {
                return eFALSE;
            }
            if((HART_SPreceiveBuffer.numByte>0) && (HART_SPreceiveBuffer.numByte<HART_XMIT_RCV_BUFFER_LEN))
            {
                memcpy(HART_SPreceiveBuffer.aryData, pData, HART_SPreceiveBuffer.numByte);
                pData += HART_SPreceiveBuffer.numByte;
            }
            HART_SPreceiveBuffer.checksum = *pData;
            HART_SPbufferState = SP_BUFFER_STATE_RECEIVED;
            
            break;
        default:
            return eFALSE;
        }
    }else
    {
      return eFALSE;
    }
    
    //Notify HART Task, relieve HART Task block
    NotifyISR_Give_RTOS_TASK_NOTIFY(HartTskHandle, &xHigherPriorityTaskWoken);
    
    return eTRUE;
}



//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER GETDEVICELOCKSTATUS> \endif
 \brief  Gets the write lock state as applied by command 71 (Lock Device)
 \if @SubGen end@ <METHODHEADER GETDEVICELOCKSTATUS> \endif
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
//@SubGen start@<METHOD GETDEVICELOCKSTATUS>
TUSIGN8 GetDeviceLockStatus_HART(void)
//@SubGen end@<METHOD GETDEVICELOCKSTATUS>
{
    TUSIGN8 lockDeviceStatus;
    (void)Get_T_UNIT(HART_ME, HART_IDX_lockDeviceStatus, WHOLE_OBJECT, &lockDeviceStatus);
    return lockDeviceStatus;
}



//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER INCREMENTCONFICHANGECOUNTER> \endif
 \brief  Increments Configuration Change Counter
 \if @SubGen end@ <METHODHEADER INCREMENTCONFICHANGECOUNTER> \endif
 \author
 \date
 \param void
 \return void
 \warning
 \test
 test-date: 2004-MM-DD
 \n by: pp ss
 \n environment:
 \n lint check o.k.: B. Tohermes; 30-08-2010
 \bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD INCREMENTCONFICHANGECOUNTER>
void IncrementConfiChangeCounter_HART(void)
//@SubGen end@<METHOD INCREMENTCONFICHANGECOUNTER>
{
    /* variable definition */
    TUSIGN8                 element;
    TUSIGN16                configChangeValue;

    // actual configuration change counter value ++
    configChangeValue = hartFreqNo.confChangeCounterValue;
    configChangeValue++;
    // actualize object with new value
    (void)HARTPutObjects(HART_IDX, HART_IDX_confChangeCounterValue, (void*) &configChangeValue);
    // configuration change flag for every master will be set simultaneously
    element = 1;
    (void)HARTPutObjects(HART_IDX, HART_IDX_configurationFlags, (void*)&element);
}


//-------------------------------------------------------------------------------------------------
/*!
 \if @SubGen start@ <METHODHEADER HARTCOMMANDINTERPRETEREXE> \endif
 \brief  build the response frame dependend on the received command-request. this function polls layer2 if a response has to be build.
 \if @SubGen end@ <METHODHEADER HARTCOMMANDINTERPRETEREXE> \endif
 \author    Stefan Tabelander
 \date      2010-10-04
 \param     void
 \return    void
 \warning
 \test
 \see       HARTLayer7CommandInterpreter
 test-date:
 \n by:
 \n environment:
 \n intention:
 \n result module test:
 \n result Lint Level 3:
 \n lint check o.k.:
 \bug
*/
//-------------------------------------------------------------------------------------------------
//@SubGen start@<METHOD HARTCOMMANDINTERPRETEREXE>
void HARTCommandInterpreterEXE_HART(void)
//@SubGen end@<METHOD HARTCOMMANDINTERPRETEREXE>
{
//    TUSIGN8 reset = 1;

    // Handle device reset.  Reset is done here after a proper response was send out by the device using transmit IR
    if ((performDeviceReset == HART_PERFORM_RESET) && (HART_SPbufferState != SP_BUFFER_STATE_WAIT_SEND))//check is action object ResetDevice be set and no HART Cmd in sending.
    {
        // Save all data before reset
        // (void)Save(FIDX_CONF_AUTO,NV_WAITING);

        performDeviceReset = HART_PERFORM_RESET_DEFAULT;
        coordinator.StopTasksSRV(CDT_MAIN_TASK|CDT_CPULOAD_TASK|CDT_SERVICE_TASK|CDT_FE_TASK|CDT_SLOW_TASK);
		Save(ALL_FILES,NV_SYNC);
        elettronicServices.ExitSRV();
    }
    else
    {
        // command interpreter shall not be started in case of a reset.
        HARTLayer7CommandInterpreter();
    }
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

