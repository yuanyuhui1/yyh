//--------------------------------------------------------------------------------------------------
/*
 Copyright      Copyright ABB, 2011.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Subsystem Hart
 Module
 Description    Read/Write Object Functions.
 Remarks

*/
//--------------------------------------------------------------------------------------------------

// General includes
#include <string.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#ifdef LOCALSERVER_SELECT
#include "UnitTests/LocalServer/webserver_main.h"
#endif
#include "RTOS/Interface/rtos.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_DATA_OBJ/Interface/t_data_obj_rngflt.h"
#include "T_Unit/Interface/t_unit.h"

// HART Layer includes
// @@adjust
#include "Coordinator/Interface/subsystem_idx.h"
#include "HART/Layer2/Interface/layer2.h"
#include "ObjfunctRB.h"
#include "cmddef.h"
#include "HART/Interface/Hart.h"
#include "HART/Interface/t_data_obj_burstmsg_confvalues.h"
#include "HART/Interface/t_data_obj_burstmsg_managevalues.h"
#include "HART/Layer7/Interface/layer7.h"
#include "HART/Source/Hart_types.h"
#include "burst_cmd.h"
#include "burst_functions.h"
#include "hart_arm.h"
#include "hart_unit_convert.h"

//Mapper
#include "Mapper/Interface/Mapper.h"
#include "Mapper/Interface/Mapper_idx.h"//____???

// Get the SUBSYSTEM IDs for local usage
#include "HART/Source/Hart_attributes.h"



// buffer for burst message specific config data objects
__no_init T_BURSTMSG_CONFVALUES   burstMsgConfValues;

#define lengthBurstMsgConfValues sizeof(T_BURSTMSG_CONFVALUES)


//--------------------------------------------------------------------------------------------------
/*!
 \brief     burst command 103 execution
 \author    IIMS
 \date      2010-06-28
 \param
 \warning
 \test      lint free level 3 OK: 2012-06-21
 \module    TEST-KIT_192 OK
 \bug       None
*/
//--------------------------------------------------------------------------------------------------

void HARTCommand_103(T_LAYER2_BUFFER* pRxBuffer, T_LAYER2_BUFFER* pTxBuffer)
{
    TUSIGN16                        objIdx;
    const T_BURSTMSG_CONFVALUES     *burstMsgConfDataPtr;
    CMD_103_REQ_RESP_DATA           *reqDataPtr, *respDataPtr;

    // set request data ptr
    reqDataPtr = (CMD_103_REQ_RESP_DATA *)&pRxBuffer->aryData[0];
    //if not valid burst message number
    if (reqDataPtr->burstMsgNum >= hartCst.bursttotalNumberOfMsg)
    {
        // error code invalid burst message
        pRxBuffer->firstResp = HART_RESP_ERR_INVALID_BURST_MESSAGE;
    }
    else
    {
        // convert update period (bytes 1 - 4) in receive buffer
        (void)HARTConvertType(TD_USIGN32, 1, ACCESS_WRITE, pRxBuffer, pTxBuffer);
        // convert maximum update period (bytes 5 - 8) in receive buffer
        (void)HARTConvertType(TD_USIGN32, 5, ACCESS_WRITE, pRxBuffer, pTxBuffer);

        // pointer of burst message specific config data object
        burstMsgConfDataPtr = burstMsgConfObjRefTable[reqDataPtr->burstMsgNum].burstMsgConfObjPtr;
        // object id of burst message specific config data object
        objIdx = burstMsgConfObjRefTable[reqDataPtr->burstMsgNum].confObjId;
        // provide config data of specific burst message
        (void)memcpy(&burstMsgConfValues, burstMsgConfDataPtr, lengthBurstMsgConfValues);

        // correct update period counter according to table entries in request
        pTxBuffer->firstResp = CorrectUpdatePeriod(&(reqDataPtr->updatePeriod));
        // correct update period counter according to table entries in request
        pTxBuffer->firstResp = CorrectUpdatePeriod(&(reqDataPtr->updatePeriodMax));

        // store corrected update period in burst message config data
        burstMsgConfValues.updatePeriod = reqDataPtr->updatePeriod;
        // store corrected maximum update period in burst message config data
        burstMsgConfValues.maximumUpdatePeriod = reqDataPtr->updatePeriodMax;

        // pointer of burst message specific config data object
        burstMsgConfDataPtr = burstMsgConfObjRefTable[reqDataPtr->burstMsgNum].burstMsgConfObjPtr;
        // object id of burst message specific config data object
        objIdx = burstMsgConfObjRefTable[reqDataPtr->burstMsgNum].confObjId;
        // store complete burst message specific config data object
        (void)HARTPutObjects(HART_IDX, objIdx, (void *) &burstMsgConfValues);
        // identify and store number of burst message with shortest update period
        IdentDefaultBurstMsg();

        // configuration change handling
        HART_ConfigurationChange();

        // set response data ptr
        respDataPtr = (CMD_103_REQ_RESP_DATA *) &pTxBuffer->aryData[0];
        // store corrected update period in response
        respDataPtr->updatePeriod = burstMsgConfValues.updatePeriod;
        // store corrected update period in response
        respDataPtr->updatePeriodMax = burstMsgConfValues.maximumUpdatePeriod;
        // convert update period (bytes 1 - 4) in transmit buffer
        (void)HARTConvertType(TD_USIGN32, 1, ACCESS_READ, pRxBuffer, pTxBuffer);
        // convert maximum update period (bytes 5 - 8) in transmit buffer
        (void)HARTConvertType(TD_USIGN32, 5, ACCESS_READ, pRxBuffer, pTxBuffer);

        //set response data length
        pTxBuffer->numByte = 2 + sizeof(CMD_103_REQ_RESP_DATA);
    }
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief     burst command 104 execution
 \author    IIMS
 \date      2010-06-28
 \param
 \warning
 \test      lint free level 3 OK: 2012-06-21
 \module    TEST-KIT_192 OK
 \bug       None
*/
//--------------------------------------------------------------------------------------------------
void HARTCommand_104(T_LAYER2_BUFFER* pRxBuffer, T_LAYER2_BUFFER* pTxBuffer)
{
    TUSIGN16                        objIdx, errorCode;
    const T_BURSTMSG_CONFVALUES     *burstMsgConfDataPtr;
    CMD_104_REQ_RESP_DATA           *reqDataPtr;


    // set request data ptr
    reqDataPtr = (CMD_104_REQ_RESP_DATA *)&pRxBuffer->aryData[0];
    //if not valid burst message numer
    if (reqDataPtr->burstMsgNum >= hartCst.bursttotalNumberOfMsg)
    {
        // error code invalid burst message
        pTxBuffer->firstResp = HART_RESP_ERR_INVALID_BURST_MESSAGE;
        pTxBuffer->numByte = 2;
    }
    else
    {
        // convert trigger level (bytes 4 - 7) in receive buffer
        (void)HARTConvertType(TD_FLOAT, 4, ACCESS_WRITE, pRxBuffer, pTxBuffer);

        // pointer of burst message specific config data object
        burstMsgConfDataPtr = burstMsgConfObjRefTable[reqDataPtr->burstMsgNum].burstMsgConfObjPtr;
        // provide config data of specific burst message
        (void)memcpy(&burstMsgConfValues, burstMsgConfDataPtr, lengthBurstMsgConfValues);

        
        TUSIGN8 HartUnitTypeCode,systemUnitType;
        HartUnitTypeCode = burstMsgConfDataPtr ->processValueClassification;               
        
        if(reqDataPtr->unitsCode != 0xFA)
        {
            if(HartConvUnitCodeFromHartUnit_HART(&(reqDataPtr->unitsCode),&HartUnitTypeCode))
            {            
            }
            else
            {
                // set error code: units code must be identically
                errorCode = RULE_VIOLATION_ERR;
                // error code invalid units code in 1. response byte
                transmitBuffer_LAYER7.firstResp = HART_RESP_ERR_INVALD_UNITS_CODE;              
            }        
        }
        
        if(reqDataPtr->deviceVariableClassForTriggerLevel != 0x00)
        {
            if(HartConvUnitTypeFromHartUnit_HART(&(reqDataPtr->deviceVariableClassForTriggerLevel),&systemUnitType))
            {     
                reqDataPtr->deviceVariableClassForTriggerLevel = systemUnitType;
            }
            else
            {
                // set error code: units code must be identically
                errorCode = RULE_VIOLATION_ERR;
                // error code invalid units code in 1. response byte
                transmitBuffer_LAYER7.firstResp = HART_RESP_ERR_INVALD_UNITS_CODE;              
            }                         
        }        
                
        if(errorCode == OK)
        {
            // check burst trigger data in command request
            errorCode = HARTCheckBurstTriggerCmdSettings(burstMsgConfDataPtr, reqDataPtr, &pTxBuffer->firstResp);        
        }

        // if no error
        if (errorCode == OK)
        {
            // store trigger mode in burst message config data
            burstMsgConfValues.triggerModeSelectionCode = reqDataPtr->burstTriggerMode;
            // store classification for trigger level in burst message config data
            burstMsgConfValues.triggerDevVariableClassification = reqDataPtr->deviceVariableClassForTriggerLevel;
            // store units code in burst message config data
            burstMsgConfValues.triggerUnitsCode = reqDataPtr->unitsCode;
            // store trigger level in burst message config data
            burstMsgConfValues.triggerValue = reqDataPtr->triggerLevel;

            // object id of burst message specific config data object
            objIdx = burstMsgConfObjRefTable[reqDataPtr->burstMsgNum].confObjId;
            // store complete burst message specific config data object
            (void)HARTPutObjects(HART_IDX, objIdx, (void *) &burstMsgConfValues);


            // configuration change handling
            HART_ConfigurationChange();

            //set response data length
            pTxBuffer->numByte = 2 + sizeof(CMD_104_REQ_RESP_DATA);
        }
    }
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     burst command 105 execution
 \author    IIMS
 \date      2010-06-29
 \param
 \warning
 \test      lint free level 3 OK: 2012-06-21
 \module    TEST-KIT_192 OK
 \bug       None
*/
//--------------------------------------------------------------------------------------------------
void HARTCommand_105(T_LAYER2_BUFFER* pRxBuffer, T_LAYER2_BUFFER* pTxBuffer)
{
    TUSIGN8                         numByte;
    const T_BURSTMSG_CONFVALUES     *burstMsgConfDataPtr;
    CMD_105_REQ_DATA                *reqDataPtr;
    CMD_105_RESP_DATA               *respDataPtr;

    // memorize length of request data
    numByte = pRxBuffer->numByte;
    // prepare ptr of request data
    reqDataPtr = (CMD_105_REQ_DATA *) &pRxBuffer->aryData[0];
    // if command compliant to hart rev. 6
    if (pRxBuffer->numByte == 0)
    {
        // make request compliant to hart rev. 7
        pRxBuffer->numByte = 1;
        // set burst message number 0
        reqDataPtr->burstMsgNum = 0;
    }

    //if not valid burst message number
    if (reqDataPtr->burstMsgNum >= hartCst.bursttotalNumberOfMsg)
    {
        // error code invalid burst message
        pTxBuffer->firstResp = HART_RESP_ERR_INVALID_BURST_MESSAGE;
        pTxBuffer->numByte = 2;
    }
    else
    {
        // pointer of burst message specific config data object
        burstMsgConfDataPtr = burstMsgConfObjRefTable[reqDataPtr->burstMsgNum].burstMsgConfObjPtr;
        // prepare ptr of request data
        respDataPtr = (CMD_105_RESP_DATA *) &pTxBuffer->aryData[0];

        //store burst mode code to response
        respDataPtr->burstModeControlCode = burstMsgConfDataPtr->modeControlCode;
        //store command number expansion flag to response
        respDataPtr->cmdNumberExpansionFlag = burstMsgConfDataPtr->cmdExpansionFlag;
        //store 8 burst command slots to response
        respDataPtr->burstSlot0 = burstMsgConfDataPtr->burstSlot0;
        respDataPtr->burstSlot1 = burstMsgConfDataPtr->burstSlot1;
        respDataPtr->burstSlot2 = burstMsgConfDataPtr->burstSlot2;
        respDataPtr->burstSlot3 = burstMsgConfDataPtr->burstSlot3;
        respDataPtr->burstSlot4 = burstMsgConfDataPtr->burstSlot4;
        respDataPtr->burstSlot5 = burstMsgConfDataPtr->burstSlot5;
        respDataPtr->burstSlot6 = burstMsgConfDataPtr->burstSlot6;
        respDataPtr->burstSlot7 = burstMsgConfDataPtr->burstSlot7;
        //store burst message number to response
        respDataPtr->burstMsgNum = burstMsgConfDataPtr->burstMsgNumber;
        //store maximum number of burst messages to response
        respDataPtr->maxNumberOfBurstMsg = hartCst.bursttotalNumberOfMsg;
        //store extended command number to response
        respDataPtr->extendedCmdNumber = burstMsgConfDataPtr->commandNumber;
        // convert extended command number (bytes 12 - 13) in transmit buffer
        (void)HARTConvertType(TD_USIGN16, 12, ACCESS_READ, pRxBuffer, pTxBuffer);
        //store update time to response
        respDataPtr->updatePeriod = burstMsgConfDataPtr->updatePeriod;
        // convert extended command number (bytes 14 - 17) in transmit buffer
        (void)HARTConvertType(TD_USIGN32, 14, ACCESS_READ, pRxBuffer, pTxBuffer);
        //store maximun update time to response
        respDataPtr->updatePeriodMax = burstMsgConfDataPtr->maximumUpdatePeriod;
        // convert extended command number (bytes 18 - 21) in transmit buffer
        (void)HARTConvertType(TD_USIGN32, 18, ACCESS_READ, pRxBuffer, pTxBuffer);
        //store burst trigger mode code to response
        respDataPtr->burstTriggerMode = burstMsgConfDataPtr->triggerModeSelectionCode;
        //store device variable classification for trigger value to response
        respDataPtr->deviceVariableClassForTriggerLevel = burstMsgConfDataPtr->triggerDevVariableClassification;
        //store units code to response
        respDataPtr->unitsCode = burstMsgConfDataPtr->triggerUnitsCode;
        //store trigger value to response
        respDataPtr->triggerLevel = burstMsgConfDataPtr->triggerValue;
        // convert extended command number (bytes 25 - 28) in transmit buffer
        (void)HARTConvertType(TD_FLOAT, 25, ACCESS_READ, pRxBuffer, pTxBuffer);

        // if request compliant to rev. 6
        if(numByte == 0)
        {
            // copy to low byte of extended command number to response byte 1
            pTxBuffer->aryData[1] = pTxBuffer->aryData[13];
        }

        //set response data length
        pTxBuffer->numByte = 2 + sizeof(CMD_105_RESP_DATA);
    }
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief     burst command 107 execution
 \author    IIMS
 \date      2010-06-28
 \param
 \warning
 \test      lint free level 3 OK: 2012-06-21
 \module    TEST-KIT_192 OK
 \bug       None
*/
//--------------------------------------------------------------------------------------------------
void HARTCommand_107(T_LAYER2_BUFFER* pRxBuffer, T_LAYER2_BUFFER* pTxBuffer)
{
    TUSIGN8                         numByte, idx;
    TUSIGN16                        objIdx;
    const T_BURSTMSG_CONFVALUES     *burstMsgConfDataPtr;
    CMD_107_REQ_RESP_DATA           *reqDataPtr;

    // prepare ptr of request data
    reqDataPtr = (CMD_107_REQ_RESP_DATA *) &pRxBuffer->aryData[0];

    // memorize length of request data
    numByte = pRxBuffer->numByte;
    //if not 8 slots in frame
    if ((numByte > 4) && (numByte < 9))
    {
        // error too few data
        pTxBuffer->firstResp = HART_RESP_ERR_TOO_FEW_DATA;
    }
    else
    {
        // if command compliant to hart rev. 5
        if (numByte != 9)
        {
            // set burst message number to 0
            reqDataPtr->burstMsgNum = 0;
        }

        //if not valid burst message number
        if (reqDataPtr->burstMsgNum >= hartCst.bursttotalNumberOfMsg)
        {
            // error code invalid burst message
            pTxBuffer->firstResp = HART_RESP_ERR_INVALID_BURST_MESSAGE;
        }
        else
        {
            // pointer of burst message specific config data object
            burstMsgConfDataPtr = burstMsgConfObjRefTable[reqDataPtr->burstMsgNum].burstMsgConfObjPtr;
            // provide config data of specific burst message
            (void)memcpy(&burstMsgConfValues, burstMsgConfDataPtr, lengthBurstMsgConfValues);

            // for all not configured slots in request
            for (idx = numByte; idx <= 7; idx++)
            {
                // set indexed slot code to not used
                reqDataPtr->burstSlot[idx] = SLOT_CODE_NONE;
            }

            // copy device variable idx out of slots in request into burst slots of burst config data
            burstMsgConfValues.burstSlot0 = reqDataPtr->burstSlot[0];
            burstMsgConfValues.burstSlot1 = reqDataPtr->burstSlot[1];
            burstMsgConfValues.burstSlot2 = reqDataPtr->burstSlot[2];
            burstMsgConfValues.burstSlot3 = reqDataPtr->burstSlot[3];
            burstMsgConfValues.burstSlot4 = reqDataPtr->burstSlot[4];
            burstMsgConfValues.burstSlot5 = reqDataPtr->burstSlot[5];
            burstMsgConfValues.burstSlot6 = reqDataPtr->burstSlot[6];
            burstMsgConfValues.burstSlot7 = reqDataPtr->burstSlot[7];

            // if command compliant hart rev. 7
            if (numByte == 9)
            {
                // if command 9 OR 33 to burst
                if (   (burstMsgConfValues.commandNumber ==  9)
                    || (burstMsgConfValues.commandNumber == 33))
                {
                    // prepare burst message specific trigger objects (variable index, burst msg number)
                    HARTPrepareBurstTriggerObjectInfo(reqDataPtr->burstSlot[0],
                                                      reqDataPtr->burstMsgNum );
                }
            }
            // object id of burst message specific config data object
            objIdx = burstMsgConfObjRefTable[reqDataPtr->burstMsgNum].confObjId;
            // store complete burst message specific config data object
            (void)HARTPutObjects(HART_IDX, objIdx, (void FAST*) &burstMsgConfValues);

            // configuration change handling
            HART_ConfigurationChange();

            // response length corresponding to request length
            pTxBuffer->numByte = 11;// 2 + numByte
        }
    }
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief     burst command 108 execution
 \author    IIMS
 \date      2010-06-28
 \param
 \warning
 \test      lint free level 3 OK: 2012-06-21
 \module    TEST-KIT_192 OK
 \bug       None
*/
//--------------------------------------------------------------------------------------------------
void HARTCommand_108(T_LAYER2_BUFFER* pRxBuffer, T_LAYER2_BUFFER* pTxBuffer)
{
    TUSIGN8                         numByte, deviceVariableCode;
    TUSIGN16                        objIdx, cmdTableIdx, lastExtendedCmdNumber;
    const T_BURSTMSG_CONFVALUES     *burstMsgConfDataPtr;
    CMD_108_REQ_RESP_DATA           *reqDataPtr;

    // init error code
    TUSIGN16                        errorCode = OK;
    deviceVariableCode=250;
    // set request data length
    numByte = pRxBuffer->numByte;
    // if command compliant to hart rev. 5/6
    if ( numByte == 1)
    {
        // make request/response compliant to hart rev. 7
        pRxBuffer->numByte = 3;
        // copy command number to lsb of to burst command
        pRxBuffer->aryData[1] = pRxBuffer->aryData[0];
        pTxBuffer->aryData[1] = pRxBuffer->aryData[0];
        //set msb of to burst command
        pRxBuffer->aryData[0] = 0;
        pTxBuffer->aryData[0] = 0;
        // set burst message number 0
        pRxBuffer->aryData[2] = 0;
        pTxBuffer->aryData[2] = 0;
    }
    // convert trigger level (bytes 4 - 7) in receive buffer
    (void)HARTConvertType(TD_USIGN16, 0, ACCESS_WRITE, pRxBuffer, pTxBuffer);
    // prepare ptr of request data
    reqDataPtr = (CMD_108_REQ_RESP_DATA FAST*) &pRxBuffer->aryData[0];
    // idx in command table
    cmdTableIdx = HARTFindCmdTableIndex(reqDataPtr->cmdNumber);
    //if device works compliant to hart rev. 5
    //   AND (not one of valid commands (1,2,3,33) OR not implemented)
    if (   (hartFreqCst.univCmdRev == 5)
        && (   (reqDataPtr->cmdNumber == 0)
            || (   (reqDataPtr->cmdNumber > 3)
                && (reqDataPtr->cmdNumber != 33))
            || ( cmdTableIdx == INVALID_CMD_NUMBER_INDEX)))
    {
        // response code "invalid selection"
        pTxBuffer->firstResp = HART_RESP_ERR_INVALID_SELECTION;
        // set error code
        errorCode = RULE_VIOLATION_ERR;
    }
    else
    {
        //if not valid burst message number
        if (reqDataPtr->burstMsgNum >= hartCst.bursttotalNumberOfMsg)
        {
            // error code invalid burst message
            pTxBuffer->firstResp = HART_RESP_ERR_INVALID_BURST_MESSAGE;
            // set error code
            errorCode = RULE_VIOLATION_ERR;
        }
        else
        {
            // if non valid rev 7 burst command number (1,2,3,9,33,48) OR command not implemented
            if (   (reqDataPtr->cmdNumber == 0)
                || (   (reqDataPtr->cmdNumber > 3)
                    && (reqDataPtr->cmdNumber != 9)
                    && (reqDataPtr->cmdNumber != 33)
                    && (reqDataPtr->cmdNumber != 48))
                || (cmdTableIdx == INVALID_CMD_NUMBER_INDEX))
            {
                // response code "invalid selection"
                pTxBuffer->firstResp = HART_RESP_ERR_INVALID_SELECTION;
                // set error code
                errorCode = RULE_VIOLATION_ERR;
            }
        }
    }
    // if request data o.k.
    if(errorCode == OK)
    {
        // pointer of burst message number specific conf data objects
        burstMsgConfDataPtr = burstMsgConfObjRefTable[reqDataPtr->burstMsgNum].burstMsgConfObjPtr;

        // memorize extended command number out of burst message specific config data object
        lastExtendedCmdNumber = burstMsgConfDataPtr->commandNumber;

        //if request compliant to hart rev. 5/6
        if (numByte == 1)
        {

            // if burst command changed
            if (reqDataPtr->cmdNumber != lastExtendedCmdNumber)
            {
                // someone changed the burst command during active burst function
                uLayer2flags.burstCmdChanged = 1;

                // object id of burst message specific config data object
                objIdx = burstMsgConfObjRefTable[reqDataPtr->burstMsgNum].confObjId;
                // store new extended command number in burst message number specific conf data object
                (void)HARTPutAttribute(HART_IDX,
                                       objIdx,
                                       BURSTMSG_CONFVALUES_commandNumber,
                                       (void *) &(reqDataPtr->cmdNumber));
            }

            // response compliant to hart rev. 5/6
            pTxBuffer->aryData[0] = pTxBuffer->aryData[1];
            // set data length in response
            pTxBuffer->numByte = 3;
        }
        // request compliant to hart rev. 7
        else
        {
            // if burst command changed
            if (reqDataPtr->cmdNumber != lastExtendedCmdNumber)
            {
                // someone changed the burst command during active burst function
                uLayer2flags.burstCmdChanged = 1;

                // provide config data of specific burst message
                (void)memcpy(&burstMsgConfValues, burstMsgConfDataPtr, lengthBurstMsgConfValues);
                //store new extended command number in burst message number specific conf data object
                burstMsgConfValues.commandNumber = reqDataPtr->cmdNumber;

                // if change to command 9 OR 33
                if (   (reqDataPtr->cmdNumber ==  9)
                         || (reqDataPtr->cmdNumber == 33))
                {
                    // device variable coude out of burst slot 0 of burst message specific object
                    deviceVariableCode = burstMsgConfDataPtr->burstSlot0;
                }
                // else if change to command 48
                else if(reqDataPtr->cmdNumber == 48)
                {
                    // slotcode 250: not used
                    deviceVariableCode = 250;
                }
                // else if change to 1 OR 3
                else if (reqDataPtr->cmdNumber != 2)
                {
                    // slotcode 246: primary variable
                    deviceVariableCode = 246;
                    //(void) mapper_Get(MAPPER_IDX_PV_Assignment,WHOLE_OBJECT,&deviceVariableCode);
                }
                // else change to command 2
                else
                {
                    // slotcode 244: percent of range
                    deviceVariableCode = 244;
                }
                // prepare burst message specific trigger objects (variable index, burst msg number)
                HARTPrepareBurstTriggerObjectInfo(deviceVariableCode,
                                                  reqDataPtr->burstMsgNum);

                // object id of burst message specific config data object
                objIdx = burstMsgConfObjRefTable[reqDataPtr->burstMsgNum].confObjId;
                // store complete burst message specific config data object
                (void)HARTPutObjects(HART_IDX, objIdx, (void *) &burstMsgConfValues);

            }
            //set response data length
            pTxBuffer->numByte = 2 + sizeof(CMD_108_REQ_RESP_DATA);
        }

        // configuration change handling
        HART_ConfigurationChange();

    }
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief     burst command 109 execution
 \author    IIMS
 \date      2010-06-28
 \param
 \warning
 \test      lint free level 3 OK: 2012-06-21
 \module    TEST-KIT_192 OK
 \bug       None
*/
//--------------------------------------------------------------------------------------------------
void HARTCommand_109(T_LAYER2_BUFFER* pRxBuffer, T_LAYER2_BUFFER* pTxBuffer)
{
    TUSIGN8                         numByte;
    TUSIGN16                        objIdx;
    T_BURSTMSG_MANAGEVALUES         *burstMsgManageDataPtr;
    CMD_109_REQ_RESP_DATA           *reqDataPtr;

    // memorize number of request data bytes
    numByte = pRxBuffer->numByte;
    // if command not compliant to hart rev. 7 OR working compliant to rev. 5
    if ((numByte != 2) || (hartFreqCst.univCmdRev == 5))
    {
        // make request/response data compliant to hart rev. 7
        pRxBuffer->numByte = 2;
        // set burst message number 0
        pRxBuffer->aryData[1] = 0;
        pTxBuffer->aryData[1] = 0;
    }

    // prepare ptr of request data
    reqDataPtr = (CMD_109_REQ_RESP_DATA FAST*) &pRxBuffer->aryData[0];
    //if not valid burst message number
    if (reqDataPtr->burstMsgNum >= hartCst.bursttotalNumberOfMsg)
    {
        // error code invalid burst message
        pTxBuffer->firstResp = HART_RESP_ERR_INVALID_BURST_MESSAGE;
    }
    // else if not valid burst mode code
    else if (reqDataPtr->burstModeControlCode > HART_BURSTMODE_ENABLE_ON_TOKEN_PASSING_DLL)
    {
        // error code invalid selection
        pTxBuffer->firstResp = HART_RESP_ERR_INVALID_SELECTION;
    }
    else
    {
        // object id of burst message specific config data object
        objIdx = burstMsgConfObjRefTable[reqDataPtr->burstMsgNum].confObjId;
        // store new extended command number in burst message number specific conf data object
        (void)HARTPutAttribute(HART_IDX,
                         objIdx,
                         BURSTMSG_CONFVALUES_modeControlCode,
                         (void *) &(reqDataPtr->burstModeControlCode));

        // identify burst message with shortest update period
        IdentDefaultBurstMsg();


        // configuration change handling
        HART_ConfigurationChange();

        // if burst mode switched to "off"
        if(reqDataPtr->burstModeControlCode == HART_BURSTMODE_OFF)
        {
            // pointer of burst message specific management objects
            burstMsgManageDataPtr = burstMsgManageObjRefTable[reqDataPtr->burstMsgNum].burstMsgManageObjPtr;
            // reset update period counter
            burstMsgManageDataPtr->updatePeriodCounter = 0;
            // reset maximum update period counter
            burstMsgManageDataPtr->maximumUpdatePeriodCounter = 0;
            // init counter of burst messages with elapsed max update period
            counterBurstMsgToSend = 0;
        }
        // set data length in response
        pTxBuffer->numByte = 2 + sizeof(CMD_109_REQ_RESP_DATA);
    }
    // if read/write successfully done
    if (   (pTxBuffer->numByte > 2)
        && (hartFreqCst.univCmdRev == 5))
    {
        // set hart rev 5 response length
        pTxBuffer->numByte = 3;
    }

}
