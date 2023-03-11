//--------------------------------------------------------------------------------------------------
/*
 Copyright      Copyright ABB, 2011.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Subsystem Hart
 Module
 Description    Parameter mapping between HART and subsystems.  User-interface
 Remarks

*/
//--------------------------------------------------------------------------------------------------

// General includes
#include <intrinsics.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#ifdef LOCALSERVER_SELECT
#include "UnitTests/LocalServer/webserver_main.h"
#endif
#include "RTOS/Interface/rtos.h"
#include "System/Interface/assert.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "nv_mem/Interface/nv_mem.h"

#include "nv_mem/chip_handler/interface/chip_handler.h"
#include "nv_mem/File/Interface/File.h"

// Get the SUBSYSTEM IDs for local usage

// @@adjust
// replace with device specific functions and objects
//#include "..\..\..\application\interface\application.h"
#include "Coordinator/Interface/subsystem_idx.h"
#include "Mapper/Interface/Mapper_idx.h"
#include "Mapper/Interface/Mapper.h"
#include "Mapper/Interface/Mapper_Config.h"


#include "HART/Interface/Hart_idx.h"
#include "HART/Interface/Hart.h"

// HART Layer includes
#include "HART/Layer2/Interface/layer2.h"

// local includes
#include "ObjfunctRB.h"
#include "cmddef.h"
#include "hart_arm.h"

#include "HART/Interface/t_data_obj_burstmsg_confvalues.h"
#include "HART/Interface/t_data_obj_burstmsg_managevalues.h"
#include "HART/Source/Hart_types.h"
#include "HART/Source/Hart_attributes.h"
#include "burst_cmd.h"
#include "burst_functions.h"

#include "HART/Layer7/Interface/layer7.h"

//#pragma dataseg= HART_LAYER7_DATA
// shows Layer 7 request from first or second Layer 2
__no_init TUSIGN8  requestsLayer2_LAYER7;
// receive and transmit buffer of layer 7
__no_init T_LAYER2_BUFFER transmitBuffer_LAYER7;
__no_init T_LAYER2_BUFFER receiveBuffer_LAYER7;
//#pragma dataseg=default
__no_init T_LAYER2_BUFFER HART_SPtransmitBuffer;
__no_init T_LAYER2_BUFFER HART_SPreceiveBuffer;
// #pragma dataseg=default

E_STATE_SP_BUFFER HART_SPbufferState = SP_BUFFER_STATE_FREE;
// squawk counter ~ 1 minute
TUSIGN16        squawkCounter;

// counter of burst messages with elapsed max update period
TUSIGN8         counterBurstMsgToSend;

// HART Device Address
TUSIGN8         s_addr = 0xFF;
TUSIGN8         s_addrLong[5] = {0xFF,0xFF,0xFF,0xFF,0xFF};

// confi data object reference table for max. possible burst messages
const BURST_MSG_CONF_OBJECT burstMsgConfObjRefTable[3] =
{
    {HART_IDX_burstMsg0ConfData, &hartFreqNo.burstMsg0ConfData},
    {HART_IDX_burstMsg1ConfData, &hartFreqNo.burstMsg1ConfData},
    {HART_IDX_burstMsg2ConfData, &hartFreqNo.burstMsg2ConfData}
};

// manage data object reference table for max. possible burst messages
const BURST_MSG_MANAGE_OBJECT burstMsgManageObjRefTable[3] =
{
    {HART_IDX_burstMsg0ManageData, &hartDyn.burstMsg0ManageData},
    {HART_IDX_burstMsg1ManageData, &hartDyn.burstMsg1ManageData},
    {HART_IDX_burstMsg2ManageData, &hartDyn.burstMsg2ManageData}
};

typedef struct
{
    TUSIGN8     day;
    TUSIGN8     month;
    TUSIGN8     year;
} HART_DATE_VALUE;

#define COLDSTART_DEFAULT (HART_SECMAS_COLDSTART_FLAG_1 + HART_MASTER_COLDSTART_FLAG_1 + \
                            HART_SECMAS_COLDSTART_FLAG_2 + HART_MASTER_COLDSTART_FLAG_2)

//--------------------------------------------------------------------------------------------------
/*!
 \brief     checks the hart date value
 \author    Bernhard Tohermes
 \date      2006-02-06
 \param     pointer to hart data
 \return
            OK                       operation was successful
            RULE_VIOLATION           harte date code invalid
 \warning
 \test
 date:
 \n by:
 \n environment:
 \n lint check o.k.: B. Tohermes; 30-08-2010
 \bug
*/
//--------------------------------------------------------------------------------------------------

static TUSIGN16 Check_HART_Date(const HART_DATE_VALUE* hartDatePtr, T_LAYER2_BUFFER* pTxBuffer)
{
   TUSIGN16     ret             = OK;

   // debug zero pointer and illegal values
   VIP_ASSERT(hartDatePtr);

   if(hartDatePtr->day > 28)
   {
       if(   (hartDatePtr->day == 0)
          || (hartDatePtr->day > 31)
          || (hartDatePtr->month == 0)
          || (hartDatePtr->month > 12)
          || (   (hartDatePtr->day == 31)
              && (   (hartDatePtr->month == 4)
                  || (hartDatePtr->month == 6)
                  || (hartDatePtr->month == 9)
                  || (hartDatePtr->month == 11)))
          || (   (hartDatePtr->month == 2)
              && (   ((hartDatePtr->year & 0x03) != 0)
                  || (hartDatePtr->day > 29))))
       {
           // set error code
           ret = RULE_VIOLATION_ERR;
           // set response code "invalid date code detected" in 1. response byte
           pTxBuffer->firstResp = HART_RESP_ERR_INVALID_DATE_CODE_DETECTED;
           // set 2 response bytes
           pTxBuffer->numByte = 2;

       }
   }
   return ret;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     
 \author    
 \date      
 \param
 \return
 \warning
 \test
 test-date: 
 \n by:     
 \n environment: 
 \n intention: 
 \n result  module test: 
 \n result  Lint Level 3: 
 \n lint check o.k.: B. Tohermes; 
 \bug
*/
//--------------------------------------------------------------------------------------------------
/* return: first response code */
static void ProcessHARTCmd(T_LAYER2_BUFFER* pRxBuffer, T_LAYER2_BUFFER* pTxBuffer, TBOOL fromSPPort)
{   
    TUSIGN16 errorCode = OK;
    TUSIGN16 cmdNumber = 0;
    TUSIGN16 cmdIndex = 0;
    TUSIGN8 objectCount = 0;
    TUSIGN8 extendCmdLen = 0;
    TUSIGN8 direction;
    TBOOL isPrimaryMasterActive;
    TBOOL configChanged;
    TUSIGN8 element;
    TUSIGN16 i;
    COM_OBJ_DESCR SLOW * pCmdDescr;
    TUSIGN16 result;

    
    pTxBuffer->numByte = 2;    // set default number of data bytes = 2 for response bytes    
    cmdNumber = (TUSIGN16)pTxBuffer->command; 

        
    if (cmdNumber == EXTENSION_COMMAND_NUMBER)
    {
        extendCmdLen = 2;
        if (hartFreqCst.univCmdRev == 5)
        {            
            pTxBuffer->firstResp = HART_RESP_ERR_CMD_NOT_IMPLEMENTED;            
            cmdIndex = INVALID_CMD_NUMBER_INDEX;     // set error flag
        }        
        else if (pRxBuffer->numByte < 2)
        {            
            pTxBuffer->firstResp = HART_RESP_ERR_TOO_FEW_DATA; 
            cmdIndex = INVALID_CMD_NUMBER_INDEX;    
        }        
        else
        {
            //copy extended command number in transmit buffer
            pTxBuffer->aryData[0] = pRxBuffer->aryData[0];
            pTxBuffer->aryData[1] = pRxBuffer->aryData[1];            
            pTxBuffer->numByte = 4;    // set default number of data bytes = 4 for response bytes and extended command number
            
            cmdNumber = (TUSIGN16)(pRxBuffer->aryData[0] << 8) + (TUSIGN16)pRxBuffer->aryData[1];  // get extended command number
            if (cmdNumber < 256)  // if command number < 256 (see HCF_SPEC-99 Rev. 9.0 page 29 range for expanded cmd number is 256-65535)
            {               
                pTxBuffer->firstResp = HART_RESP_ERR_INVALID_EXT_CMD_NUMBER;
                cmdIndex = INVALID_CMD_NUMBER_INDEX; // set error flag
            }

            /* Trums in 2019.1.11: use a RAM object to avoid unnecessary NV write operation */
            (void)HARTPutObjects(HART_IDX, HART_IDX_lastReceiveCommandNumber, (void FAST*)&cmdNumber);
        }
    }
    else
    {
        extendCmdLen = 0;     

        //changed by Jaden, 2018-8-20, shall not record the hart number which is used to read itself        
        (void)HARTPutObjects(HART_IDX, HART_IDX_lastReceiveCommandNumber, (void FAST*)&cmdNumber);        
        
        if (hartFreqCst.univCmdRev == 5)
        {            
            if (cmdNumber <= 30)
            {
                // if command number not hart revision 5 universal command
                if(((cmdNumber > 6) && (cmdNumber < 11)) || (cmdNumber > 19))
                {
                    cmdIndex = INVALID_CMD_NUMBER_INDEX;   // set error flag                    
                    pTxBuffer->firstResp = HART_RESP_ERR_CMD_NOT_IMPLEMENTED;
                }
            }
            else
            {
                if((cmdNumber > 70)
                && ((cmdNumber < 107) || (cmdNumber > 111))
                && (cmdNumber < 121))
                {                    
                    cmdIndex = INVALID_CMD_NUMBER_INDEX;    // set error flag
                    pTxBuffer->firstResp = HART_RESP_ERR_CMD_NOT_IMPLEMENTED;
                }
            }
        }
        else
        { }
    }
    
    if (cmdNumber == 73)
    {// if did not support find device for 2wire board!
        
        cmdIndex = INVALID_CMD_NUMBER_INDEX;    // set error flag
        pTxBuffer->firstResp = HART_RESP_ERR_CMD_NOT_IMPLEMENTED;
    }    
    
    if (cmdIndex != INVALID_CMD_NUMBER_INDEX)
    {// if (no error from extended command so long), ...
        
        cmdIndex = HARTFindCmdTableIndex(cmdNumber);       // Search for command table index        
        if (cmdIndex == INVALID_CMD_NUMBER_INDEX)
        {// Command not implemented?            
            pTxBuffer->firstResp = HART_RESP_ERR_CMD_NOT_IMPLEMENTED;
        }        
        // if not enough data received && not hart rev. 5 specific data of cmd 6, 38, 48
        else if ((pRxBuffer->numByte < (commands[cmdIndex].requestDataLength +extendCmdLen ))
              && (!((cmdNumber ==  6) && (pRxBuffer->numByte == 1)))
              && (!((cmdNumber == 38) && (pRxBuffer->numByte == 0)))
              && (!((cmdNumber == 48) && (pRxBuffer->numByte == 0))) // contradiction in HART spec and HCF test about how to handle no requeset bytes 2015/12/10
              && (!((cmdNumber == 105) && (pRxBuffer->numByte == 0)))
              && (!((cmdNumber == 107) && (pRxBuffer->numByte > 0))) // (receiveBuffer_LAYER7.numByte <= 4))) // updated 3/27 2013 just follow the Hart7 spec to support more slots;
              && (!((cmdNumber == 51) && (pRxBuffer->numByte > 0) && (pRxBuffer->numByte <= 4)))
              && (!((cmdNumber == 108) && (pRxBuffer->numByte == 1)))
              && (!((cmdNumber == 109) && (pRxBuffer->numByte == 1))))
                
        {
            pTxBuffer->firstResp = HART_RESP_ERR_TOO_FEW_DATA;  // too few data bytes
        }
        else
        {            
            if ((commands[cmdIndex].fct == HART_READ) || (commands[cmdIndex].fct == HART_SPECIAL_READ))
            {
                direction = ACCESS_READ;
            }
            else
            {
                direction = ACCESS_WRITE;
            }

            //Check for write protection
            isPrimaryMasterActive = (TBOOL)((pRxBuffer->aryAdr[0] & LAYER2_PRIMARY_MASTER) != 0x00);
            if(fromSPPort)
            {
                isPrimaryMasterActive = eTRUE;
            }
            else
            {
                isPrimaryMasterActive = (TBOOL)((pRxBuffer->aryAdr[0] & LAYER2_PRIMARY_MASTER) != 0x00);
            }
           
            pTxBuffer->firstResp = HARTCanAccess(cmdNumber, direction, isPrimaryMasterActive);    //Set access right response code in 1. response byte
                        
            // FPC 9/6/2013
            // CMDs 41 & 42 do not support RC 7, so have to change to access restricted
            if ((pTxBuffer->firstResp == HART_RESP_ERR_WRITE_PROTECTED) 
             && ((cmdNumber == 41) || (cmdNumber == 42))) 
            {
                pTxBuffer->firstResp = HART_RESP_ERR_ACCESS_RESTRICTED;
            }
            // Proceed with read / write activity if access is permitted
            if(pTxBuffer->firstResp == HART_RESP_OK)
            {
                pCmdDescr = (COM_OBJ_DESCR SLOW *) commands[cmdIndex].cmdDescr;
                objectCount = commands[cmdIndex].objectCount;       
                configChanged = commands[cmdIndex].configChanged;  
                
                switch (commands[cmdIndex].fct)
                {
                    case (HART_READ):   
                        //if device works compliant to hart rev. 5
                        if (hartFreqCst.univCmdRev == 5)
                        {
                            // if cmd 0 to process
                            if (cmdNumber == 0)
                            {
                                // truncate object command processing after 10th object
                                objectCount = 10;
                            }
                            // if cmd 15 to process
                            if (cmdNumber == 15)
                            {
                                // truncate object command processing after 7th object
                                objectCount = 8;                                        
                            }
                        }
                        
                        //if cmd 72 (squawk) to process
                        if(cmdNumber == 72)
                        {
                            if(squawkCounter == 0)
                            {//if squawk not active
                                // switch squawk to "on"
                                // @@adjust
                                //application.SquawkMessageOn_SRV();
                            }
                            squawkCounter = (SQUAWK_TIME / HART_LAYER7_CMD_INTERPRETER_CYCL_TIME) + 1;
                        }
                        else if(errorCode==OK)
                        {
                            HARTReadObjects(pCmdDescr, objectCount, pRxBuffer, pTxBuffer);
                        }
                        break;

                    case (HART_WRITE):
                        // set data length anyway
                        if(pTxBuffer->command==EXTENSION_COMMAND_NUMBER)
                        {
                            pTxBuffer->numByte +=commands[cmdIndex].requestDataLength;
                        }
                        else
                        {
                            pTxBuffer->numByte = 2 + commands[cmdIndex].requestDataLength;
                        }
                        
                        if ((cmdNumber == 141) || (cmdNumber == 147) )
                        {
                            if (CheckIsSupportedMeasurement(MAPPER_DEVICE_VAR_POWER) != OK)
                            {
                              errorCode = HART_RESP_ERR_CMD_NOT_IMPLEMENTED;
                              pTxBuffer->firstResp = HART_RESP_ERR_CMD_NOT_IMPLEMENTED;
                            }
                            else
                            {}
                        }
                        
                        if(cmdNumber == 18)
                        {
                            errorCode = Check_HART_Date ((HART_DATE_VALUE *) &pRxBuffer->aryData[18], pTxBuffer);   //check date value
                        }
                        else
                        {}
                        
                        if (errorCode == OK)
                        {
                            // set pointer to command object description
                            pCmdDescr = (COM_OBJ_DESCR SLOW *) commands[cmdIndex].cmdDescr;

                            // get number of objects
                            objectCount = commands[cmdIndex].objectCount;

                            // get config changed state
                            configChanged = commands[cmdIndex].configChanged;

                            // if cmd 6 request contains only polling address
                            if ((cmdNumber ==  6) && (pRxBuffer->numByte == 1))
                            {
                                // Set no. of request specific object commands to 1
                                objectCount= 1;
                                // write only polling address to its object
                                (void)HARTWriteObjects(pCmdDescr, objectCount, configChanged, pRxBuffer, pTxBuffer);
                                //If device works conform to HART 7
                                if (hartFreqCst.univCmdRev == 7)
                                {
                                    //copy loop current mode in response behind polling adrress
                                    pTxBuffer->aryData[1] = hartFreqCst.loopCurrentMode;
                                    // correct response length
                                    pTxBuffer->numByte++;
                                }
                            }
                            else
                            {
                                // write objects
                                (void)HARTWriteObjects(pCmdDescr, objectCount, configChanged, pRxBuffer, pTxBuffer);
                            }
                        }
                        break;

                    case (HART_SPECIAL_READ):
                    case (HART_SPECIAL_WRITE):
                        switch (cmdNumber)
                        {
                            case 9:  
                            case 33:
                            case 54:
                                //set error code
                                errorCode = OK;
                                for(i = 0; i < pRxBuffer->numByte; i++)
                                {                               
                                    // if non valid device variable index
                                  //add,2013-01-25.
                                  //@Ajust:
                                  //ToDo: The hartCst.maxNoOfDevVariables shall be changed according to different number of device variable in different device. 
                                  //The value is 3 in current APP. The max sloc number is 249.
                                    if(pRxBuffer->aryData[i] > 250)
                                    {
                                        // error code invalid burst message
                                        pTxBuffer->firstResp = HART_RESP_ERR_INVALID_SELECTION;
                                        pTxBuffer->numByte = 2;
                                        // set error code
                                        errorCode = RULE_VIOLATION_ERR;
                                        break;
                                    }
                                    else { }                 
                                }       
                                
                                // if no error
                                if(errorCode == OK)
                                {              
                                    // read/write objects of special commands
                                    HARTSpecialReadWriteObjects(cmdIndex, pRxBuffer, pTxBuffer);                                          
                                }
                                else { }
                                break;

                            case 38:
                                // write a value != 0 to perform an action function
                                element = 0xFF;
                                // write as an attribute
                                (void)HARTPutAttribute(HART_IDX, HART_IDX_resetConfigFlag, 0, (void FAST*)&element);
                                break;
                                
                            case 48: 
                                element = 0xFF;  // write a value != 0 to perform an action function                                 
                                result = HARTPutAttribute(HART_IDX, HART_IDX_resetMoreStatusAvailable, 0, (void FAST*)&element);    // write as an attribute
                                if (result == OK) pTxBuffer->firstResp = HART_RESP_OK;
                                else pTxBuffer->firstResp = HART_RESP_ERR_DEVICE_SPECIFIC_CMD_ERROR;
                                break;
                            
                            case 51:
                                HARTSpecialReadWriteObjects(cmdIndex, pRxBuffer, pTxBuffer);
                                break;
                            
                            case 103:
                                // burst command 103 execution
                                HARTCommand_103(pRxBuffer, pTxBuffer);
                                break;
                                
                            case 104:
                                // burst command 104 execution
                                HARTCommand_104(pRxBuffer, pTxBuffer);
                                break;
                           
                            case 105:
                                // burst command 105 execution
                                HARTCommand_105(pRxBuffer, pTxBuffer);
                                break;
                            
                            case 107:
                                // burst command 103 execution
                                HARTCommand_107(pRxBuffer, pTxBuffer);
                                break;
                            
                            case 108:
                                // burst command 108 execution
                                HARTCommand_108(pRxBuffer, pTxBuffer);
                                break;
                                
                            case 109:
                                // burst command 109 execution
                                HARTCommand_109(pRxBuffer, pTxBuffer);
                                break;
                            
                            default:                                
                                HARTSpecialReadWriteObjects(cmdIndex, pRxBuffer, pTxBuffer);      // read/write objects of special commands
                                break;
                        }
                        break;

                    default:
                        break;
                }
            }
            else{}
        }
    }

    
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     HART command interpreter requested by HART L2
            Comment line 252,472
 \author    Stefan Tabelander, Horst Seele,Eric-Jiping.li
 \date      2006-03-21,2011-04-12
 \param
 \return
 \warning
 \test
 test-date: 2008-07-14
 \n by:     Marcel Schilg
 \n environment: PD30F / Modultest Test_layer7
 \n intention: instruction coverage & HCF Application Layer Testscripts
 \n result  module test: ok
 \n result  Lint Level 3: ok, 2008-07-14
 \n lint check o.k.: B. Tohermes; 30-08-2010
 \bug
*/
//--------------------------------------------------------------------------------------------------

void HARTLayer7CommandInterpreter(void)
{
    const T_BURSTMSG_CONFVALUES     *burstMsgConfDataPtr;
    T_BURSTMSG_MANAGEVALUES         *burstMsgManageDataPtr;
    TUSIGN8 idx;
    
    //prepare hart receive address
    if(s_addrLong[0]==0xFF)
    {
#ifdef HART_MODULE_TEST
        s_addrLong[0] = 1;
        s_addrLong[1] = 2;
        s_addrLong[2] = 0;
        s_addrLong[3] = 0;
        s_addrLong[4] = 1;
        s_addr = 1;
#else      
        hart_Get(HART_IDX_mfrId, WHOLE_OBJECT, &s_addrLong[0]);
        hart_Get(HART_IDX_mfrDeviceType, WHOLE_OBJECT, &s_addrLong[1]);
        hart_Get(HART_IDX_deviceIdNumber, WHOLE_OBJECT, &s_addrLong[2]);
        hart_Get(HART_IDX_pollAddr, WHOLE_OBJECT, &s_addr);
#endif
    } 
    
    // *** burst time management ***
    // if all burst messages with elapsed max update period sent
    if (counterBurstMsgToSend == 0)
    {
        // for all burst messages
        for (idx = 0; idx < hartCst.bursttotalNumberOfMsg; idx++)
        {
            // pointer of burst message specific management objects
            burstMsgManageDataPtr = burstMsgManageObjRefTable[idx].burstMsgManageObjPtr;
            // pointer of burst message number specific conf data objects
            burstMsgConfDataPtr = burstMsgConfObjRefTable[idx].burstMsgConfObjPtr;
            // if update period counter of actual burst message != 0
            if (burstMsgManageDataPtr->updatePeriodCounter != 0)
            {
                // decrement counter value
                burstMsgManageDataPtr->updatePeriodCounter--;
            }
            // if maximum update period counter of actual burst message != 0
            if (burstMsgManageDataPtr->maximumUpdatePeriodCounter != 0)
            {
                // decrement counter value
                burstMsgManageDataPtr->maximumUpdatePeriodCounter--;
                // if max update period elapsed
                if(burstMsgManageDataPtr->maximumUpdatePeriodCounter == 0)
                {
                    // actualize counter of burst messages with elapsed max update period
                    counterBurstMsgToSend++;
                }
            }
            else
            {
                // if burst mode enabled for current burst message
                if(burstMsgConfDataPtr->modeControlCode != 0)
                {
                    // actualize counter of burst messages with elapsed max update period
                    counterBurstMsgToSend++;
                }
            }
        }
    }

    //if squawk active
    if(squawkCounter != 0)
    {
        // actualize remaining squawk
        squawkCounter--;
        //if squawk not active
        if(squawkCounter == 0)
        {
            // switch squawk to "off"
            // @@adjust
            //application.SquawkMessageOff_SRV();
        }
    }

    // clear all requests
    requestsLayer2_LAYER7 = NO_LAYER2_REQ;
    
    // if layer2 identifier set
    if (requestsLayer2_LAYER7 != NO_LAYER2_REQ)
    {
        //Process HART Command
        ProcessHARTCmd(&receiveBuffer_LAYER7, &transmitBuffer_LAYER7, eFALSE);
         
        // Determine value of second response byte
        transmitBuffer_LAYER7.secResp = HARTCheckSecRespByte(requestsLayer2_LAYER7, &transmitBuffer_LAYER7);

        // if first layer 2 request done, ...
        if (requestsLayer2_LAYER7 == FIRST_LAYER2)
        {
            // copy layer 7 transmit buffer to layer 2 transmit buffer
            uTransmitBuffer_LAYER2 = transmitBuffer_LAYER7;

            // clear request flags
            ClearLayer7request_LAYER2();
            requestsLayer2_LAYER7 = NO_LAYER2_REQ;

            // start response
            //StartResponse_LAYER2();
        }

        if (uLayer2flags.burstMsgReqBuilt == 1)
        {
            uLayer2flags.burstMsgRespBuilt = 1;
            uLayer2flags.burstMsgReqBuilt = 0;
        }
    }
    
    if(HART_SPbufferState == SP_BUFFER_STATE_RECEIVED)
    {
        HART_SPtransmitBuffer = HART_SPreceiveBuffer;
        
        ProcessHARTCmd(&HART_SPreceiveBuffer, &HART_SPtransmitBuffer, eTRUE);
        
        HART_SPbufferState = SP_BUFFER_STATE_WAIT_SEND;
    }

}

static TUSIGN8 bit_Set = 0;
static TUSIGN8 coldstart = COLDSTART_DEFAULT;
//--------------------------------------------------------------------------------------------------
/*!
 \brief     Change the cold statrt status only when data is sent out.
 \author    Paul Li
 \date      2014-04-20
 \param-
 \return-
 \warning
 \test
 test-date: 2014-04-20
 \n by:     Paul Li
 \n environment:  HCF- CAL071a and CAL042
 \n intention: instruction coverage & HCF ApplicationLayer Test Scripts
 \n result  module test: OK
 \n result  Lint Level 3: free // approved warnings and approved infos are suppressed
 \n lint check o.k.
 \bug
*/
//--------------------------------------------------------------------------------------------------
void ClearColdBit(void)
{
  	switch(bit_Set)
	{
	case HART_MASTER_COLDSTART_FLAG_1:
	case HART_SECMAS_COLDSTART_FLAG_1:
	case HART_MASTER_COLDSTART_FLAG_2:
	case HART_SECMAS_COLDSTART_FLAG_2:
	  coldstart &= ~bit_Set;
	  break;
	default:
	  break;
	}
	bit_Set = 0;
}



//--------------------------------------------------------------------------------------------------
/*!
 \brief     Determines the HART second response byte
            Comment Line 770,780 for temperary cause no objects exists
 \author    Stefan Tabelander, Horst Seele,Eric-Jiping.li
 \date      2005-11-08,2011-04-12
 \param     requestsLayer2: Flags which Layer 2 requests Layer 7.
 \return    secRespByte:   Second response byte
 \warning
 \test
 test-date: 2008-05-07
 \n by:     Marcel Schilg
 \n environment:  PD30F / Test_layer7_HART_CheckSecRespByte
 \n intention: instruction coverage & HCF ApplicationLayer Test Scripts
 \n result  module test: OK
 \n result  Lint Level 3: free // approved warnings and approved infos are suppressed
 \n lint check o.k.: B. Tohermes; 30-08-2010
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN8 HARTCheckSecRespByte (TUSIGN8 requestsLayer2, T_LAYER2_BUFFER* pTxBuffer)
{

    TUSIGN8     secRespByte = 0;
    TUSIGN16    otherDiagnosis = 0;



    // *************************************************
    // Is config changed flag set?                     *
    // *************************************************

    // Bit 6: CONFIGURATION_CHANGED

    // get master specific configuration change flags (requested by GetConf)
    (void)HARTGetObjects(HART_IDX, HART_IDX_configurationFlags, &otherDiagnosis);

    // if flag set
    if ((TUSIGN8)otherDiagnosis == 0x01)
    {
        // set configuration change status in response status
        secRespByte |= CONFIGURATION_CHANGED;
    }


    // *************************************************
    // Cold start performed?                           *
    // *************************************************

    // cold start Bit 5
    // will be send in the first response only
    // Reset coldstart flags depends on communication type individually
    // If (response for first Layer 2 request)

    // No cold start flag shall be cleared if the request was generated by burst mode
    // layer7requestedBit is only set if a sucessful request was generated by any Layer 2
    if(uLayer2flags.layer7requestedBit == 1)
    {
        if (requestsLayer2 == FIRST_LAYER2)
        {
            // Reset primary master coldstart flag
            if ((coldstart & HART_MASTER_COLDSTART_FLAG_1) &&
                ((pTxBuffer->aryAdr[0] & LAYER2_PRIMARY_MASTER) != 0))
            {
                secRespByte |= COLD_START;
				bit_Set =  HART_MASTER_COLDSTART_FLAG_1;
            }
            // Reset secondary master coldstart flag
            else if ((coldstart & HART_SECMAS_COLDSTART_FLAG_1) &&
                     ((pTxBuffer->aryAdr[0] & LAYER2_PRIMARY_MASTER) == 0))
            {
                secRespByte |= COLD_START;
			    bit_Set = HART_SECMAS_COLDSTART_FLAG_1;
            }
            else
            {
            }
        }
    // else if (response for second Layer 2 request)
        else if (requestsLayer2 == SECOND_LAYER2)
        {
            // Reset primary master coldstart flag
            if ((coldstart & HART_MASTER_COLDSTART_FLAG_2) &&
                ((pTxBuffer->aryAdr[0] & LAYER2_PRIMARY_MASTER) != 0))
            {
                secRespByte |= COLD_START;
                bit_Set = HART_MASTER_COLDSTART_FLAG_2;
            }
            // Reset secondary master coldstart flag
            else if ((coldstart & HART_SECMAS_COLDSTART_FLAG_2) &&
                     ((pTxBuffer->aryAdr[0] & LAYER2_PRIMARY_MASTER) == 0))
            {
                secRespByte |= COLD_START;
                bit_Set = HART_SECMAS_COLDSTART_FLAG_2;
            }
            else
            {
            }
        }
        else
        {
        }
    }
    // In case of burst during start-up set cold start flag as long as no other command was handled via Layer2
    else if(coldstart == COLDSTART_DEFAULT)
    {
        secRespByte |= COLD_START;
    }
    else{}

    // *************************************************
    // More Status available?                           *
    // *************************************************

    // Bit 4
    // get counter of last diagnosis status change
    // @@adjust
    (void)HARTGetObjects(MAPPER_IDX, MAPPER_IDX_diagnosisCounter, &otherDiagnosis);
    //(void)HARTGetObjects(MAPPER_IDX, MAPPER_IDX_diagnosisConditionDetailsAlarmCounter, &otherDiagnosis);

    // if new more diagnosis status available
    if((pTxBuffer->aryAdr[0] & LAYER2_PRIMARY_MASTER) != 0)
    {
        // Check for primary master more status bit
        if(hartDyn.diagCounterMoreStatusAvailablePrimMaster != otherDiagnosis)
        {
            secRespByte |= MORE_STATUS_AVAILABLE;
        }
    }
    else
    {
        // Check for secondary master more status bit
        if(hartDyn.diagCounterMoreStatusAvailableSecMaster != otherDiagnosis)
        {
            secRespByte |= MORE_STATUS_AVAILABLE;
        }
    }

    // *************************************************
    // Loop current fixed?                             *
    // *************************************************

    // Bit 3
    // Loop Current Fixed (earlier: Primary Variable Analog Output Fixed)
    // if loop current mode == enabled
    if (hartFreqCst.loopCurrentMode == HART_LOOP_CURRENT_MODE_DISABLED)
    {
        // set status loop current fixed
        secRespByte |= PV_AO_FIXED;
    }
    else{}

    return secRespByte;
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief     HARTFindCmdTableIndex: Search command table index for command
 \author    Horst Seele
 \date      2005-11-15
 \param     cmdNumber: command number.
 \return    Index in command table or invalid index if not found.
 \warning
 \test
 test-date: 2008-05-07
 \n by:     Marcel Schilg
 \n environment:  PD30F / Modultest_layer7_HART_findcmdTableIndex
 \n intention: instruction coverage
 \n result  module test: ok
 \n result  Lint Level 3: ok, 2008-05-07
 \n lint check o.k.: B. Tohermes; 30-08-2010
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 HARTFindCmdTableIndex (TUSIGN16 commandNumber)
{
    TUSIGN16    lowLimitIndex, uppLimitIndex, searchIndex;

    VIP_ASSERT(lengthOfCommandTable != 0);

    // set index limits for search
    lowLimitIndex = 0;
    uppLimitIndex = lengthOfCommandTable - 1;

    // as long as command table index not found and not invalid, ...
    do
    {
        // calculate new index search range
        searchIndex = uppLimitIndex - lowLimitIndex;

        // if (search range more then 1 index), ...
        if (searchIndex > 1)
        {
            // divide by 2
            searchIndex = (searchIndex >> 1);

            // set search index to middle of search index range
            searchIndex = searchIndex + lowLimitIndex;
        }
        // else ...
        else
        {
            // set search index to low limit of search index range
            searchIndex = lowLimitIndex;
        }



        // if (received command number > indexed command number), ...
        if (commandNumber > commands[searchIndex].cmdNumber)
        {
            // if (no new search index possible), ...
            if (searchIndex == uppLimitIndex)
            {
                // set invalid command number index as return value
                uppLimitIndex = INVALID_CMD_NUMBER_INDEX;

                // end while loop
                searchIndex = INVALID_CMD_NUMBER_INDEX;
            }
            // else ...
            else
            {
                // set new lower index limit for search
                lowLimitIndex = searchIndex + 1;
            }
        }
        // else if (received command number < indexed command number), ...
        else if (commandNumber < commands[searchIndex].cmdNumber)
        {
            // if (no new search index possible) set invalid index
            if (searchIndex == lowLimitIndex)
            {
                // set invalid command number index as return value
                uppLimitIndex = INVALID_CMD_NUMBER_INDEX;

                // end while loop
                searchIndex = INVALID_CMD_NUMBER_INDEX;
            }
            // else ...
            else
            {
                // set new lower index limit for search
                uppLimitIndex = searchIndex - 1;
            }
        }
        // else index found, ...
        else
        {
            // set command number index as return value
            uppLimitIndex = searchIndex;

            // end while loop
            searchIndex = INVALID_CMD_NUMBER_INDEX;
        }
    }
    while (searchIndex != INVALID_CMD_NUMBER_INDEX);

    // RETURN command number index
    return (uppLimitIndex);
}

