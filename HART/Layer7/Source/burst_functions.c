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
#include <intrinsics.h>
#include <string.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#ifdef LOCALSERVER_SELECT
#include "UnitTests/LocalServer/webserver_main.h"
#endif
#include "RTOS/Interface/rtos.h"
#include "System/Interface/assert.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_DATA_OBJ/Interface/t_data_obj_rngflt.h"
#include "T_Unit/Interface/t_unit.h"

// HART Layer includes
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

// Get the SUBSYSTEM IDs for local usage
#include "HART/Interface/Hart_idx.h"
#include "HART/Source/Hart_attributes.h"

// correction table of allowed update period / maximum update period counter
typedef struct
{
    TUSIGN32    counterValue;
    TUSIGN32    lowerLimit;
    TUSIGN32    upperLimit;
} PERIOD_COUNTER_VALUES;

// table of update period and maximum update period counter values
// with their limits for correction to the corresponding counter value
static SLOW const PERIOD_COUNTER_VALUES periodCounterValueTab[] =
{
//  counter      lowerLimit  upperLimit
    {16000,      16000,      23968},
    {32000,      24000,      47968},
    {64000,      48000,      95968},
    {128000,     96000,      191968},
    {256000,     192000,     383968},
    {512000,     384000,     767968},
    {1024000,    768000,     1471968},
    {1920000,    1472000,    1920000}
};
static const TUSIGN8 lastIdxOfPeriodCounterValueTable = sizeof(periodCounterValueTab)/sizeof(PERIOD_COUNTER_VALUES)- 1;

static SLOW const TUSIGN32 maxPeriodCounterValue = 115200000;



//--------------------------------------------------------------------------------------------------
/*!
 \brief     Perform correction of Update Period values
 \author    IIMS
 \date      2010-01-28
 \param     ptrValue = pointer to update period counter value.
 \warning
 \test      lint free level 3 OK: 2012-06-21
 \module    TEST-KIT_192 OK
 \bug       None
*/
//--------------------------------------------------------------------------------------------------
TUSIGN8 CorrectUpdatePeriod(TUSIGN32 *ptrValue)
{
    TUSIGN8     idx;
    // set correction flag
    TBOOL       correctionFlag = eTRUE;

    // if  value to correct < minimum update period counter value
    if (*ptrValue <= periodCounterValueTab[0].lowerLimit)
    {
        // update period counter = minimum update period counter value
        *ptrValue = periodCounterValueTab[0].counterValue;
    }
    // else if value to correct > upper limit of last table entry
    else if (*ptrValue > periodCounterValueTab[lastIdxOfPeriodCounterValueTable].upperLimit)
    {
        // if value to correct >= maximum valid update period counter value
        if (*ptrValue >= maxPeriodCounterValue)
        {
            // update period counter = maximum update period valid counter value
            *ptrValue = maxPeriodCounterValue;
        }
        // no correction of update period counter:
        // value of highest table entry < update period counter
        // update period counter < maximum update period valid counter value
        else
        {
            // reset correction flag
            correctionFlag = eFALSE;
        }
    }
    // else counter value correction using table entries
    else
    {
        // for all entries of update period table
        for (idx = 0; idx < lastIdxOfPeriodCounterValueTable; idx++)
        {
            // if low limit of actual to check entry <= value to correct
            // AND value to correct <= high limit of actual to check entry
            if (   (*ptrValue >= periodCounterValueTab[idx].lowerLimit)
                && (*ptrValue <= periodCounterValueTab[idx].upperLimit))
            {
                // if value to correct != corrected value
                if (*ptrValue != periodCounterValueTab[idx].counterValue)
                {
                    // counter value = corrected value of actual entry
                    *ptrValue = periodCounterValueTab[idx].counterValue;
                }
                else
                {
                    // reset correction flag
                    correctionFlag = eFALSE;
                }
                // end criteria of for loop
                idx = lastIdxOfPeriodCounterValueTable;
            }
        }
    }
    // if correction of update period value done
    if (correctionFlag == eTRUE)
    {
        //warning: set to nearest possible value
        return HART_RESP_WARN_SET_TO_NEAREST_VALUE;
    }
    else
    {
        return HART_RESP_OK;
    }

}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Identify burst message with shortest update period as default burst message
 \author    IIMS
 \date      2010-01-28
 \param
 \warning
 \test      lint free level 3 OK: 2012-06-21
 \module    TEST-KIT_192 OK
 \bug       None
*/
//--------------------------------------------------------------------------------------------------
void IdentDefaultBurstMsg(void)
{
    TUSIGN8                         idx, burstMsgNo;
    TUSIGN32                        compareUpdatePeriod = 0xFFFFFFFF;
    const T_BURSTMSG_CONFVALUES     *burstMsgConfDataPtr;

    // reset flag "one burst mode activated burst message found"
    TBOOL                           burstMsgFoundFlag = eFALSE;

    // for all burst messages
    for (idx = 0; idx < hartCst.bursttotalNumberOfMsg; idx++)
    {
        // pointer of burst message specific confi objects reference table
        burstMsgConfDataPtr = burstMsgConfObjRefTable[idx].burstMsgConfObjPtr;

        // if burst mode activated for current burst message
        if (burstMsgConfDataPtr->modeControlCode == HART_BURSTMODE_ENABLE_ON_TOKEN_PASSING_DLL)
        {
            // if first burst mode activated burst message found
            if (burstMsgFoundFlag == eFALSE)
            {
                // current update period as compare value
                compareUpdatePeriod = burstMsgConfDataPtr->updatePeriod;
                // store current burst message number as to send message
                burstMsgNo = idx;
                // set flag "burst mode activated burst message found"
                burstMsgFoundFlag = eTRUE;
            }
            // else an other burst mode activated burst message found
            else
            {
                // if current update period < compare value
                if (burstMsgConfDataPtr->updatePeriod < compareUpdatePeriod)
                {
                    // current update period as compare value
                    compareUpdatePeriod = burstMsgConfDataPtr->updatePeriod;
                    // store current burst message number as to send message
                    burstMsgNo = idx;
                }
            }
        }
    }
    // if one burst mode activated burst message found
    if (burstMsgFoundFlag == eTRUE)
    {
        // store burst message number as idle burst message
        (void)HARTPutObjects(HART_IDX,
                       HART_IDX_burstDefaultMsg,
                       (void *) &burstMsgNo);

        // if first start of a burst message
        if(uLayer2flags.burstMsgReqBuilt == 0)
        {
            //set flag: burst buffer init done after first burst start
            uLayer2flags.burstBufferInit = 1;
        }
    }
    // no burst activated
    else
    {
        // reset flags used to manage burst msg transmit
        uLayer2flags.burstMsgReqBuilt = 0;
        uLayer2flags.burstMsgRespBuilt = 0;
        uLayer2flags.burstMsg1stSent = 0;
        uLayer2flags.burstMsgWoutBuilt = 0;
        // reset flag: burst buffer init done after first burst start
        uLayer2flags.burstBufferInit = 0;
    }
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief     HARTCheckBurstTriggerCmdSettings: Check burst trigger parameter in data of command 104
 \author    IIMS
 \date      2010-02-02
 \param
 \return    errorCode:  OK      - trigger data ok: request data can be written to objects
 \return                not OK  - trigger data not ok: response contains specific fault code
 \warning
 \test      lint free level 3 OK: 2012-06-21
 \module    TEST-KIT_192 OK
 \bug       None
*/
//--------------------------------------------------------------------------------------------------

TUSIGN16 HARTCheckBurstTriggerCmdSettings (const T_BURSTMSG_CONFVALUES  *burstMsgConfDataPtr,
                                                        const CMD_104_REQ_RESP_DATA  *reqDataPtr,
                                                        TUSIGN8* pFirstResp)
{
    TPOLY16     cmdToBurst;

    // reset error code
    TUSIGN16    errorCode = OK;

    // prepare burst command number
    cmdToBurst.u16 = burstMsgConfDataPtr->commandNumber;

    //if    trigger mode "on change" to set for command 1, 2, 3, 9 or 33
    //   OR not trigger mode "on change" to set for command 48
    if (   (   (reqDataPtr->burstTriggerMode == HART_BURST_TRIGGER_MODE_ON_CHANGE)
            && (cmdToBurst.aryU8[0] != HART_BURST_COMMAND_48))
        || (   (reqDataPtr->burstTriggerMode != HART_BURST_TRIGGER_MODE_CONTINUOUS)
            && (reqDataPtr->burstTriggerMode != HART_BURST_TRIGGER_MODE_ON_CHANGE)
            && (cmdToBurst.aryU8[0] == HART_BURST_COMMAND_48)))
    {
        // set error code: trigger mode on change only valid with command 48
        errorCode = RULE_VIOLATION_ERR;
        // error code invalid burst trigger mode selection code in 1. response byte
        *pFirstResp = HART_RESP_ERR_INVALID_BRST_TRIG_MODE_SEL_CODE;
    }
    // else if commando 1, 2, 3, 9 or 33 to burst
    else if (cmdToBurst.aryU8[0] != HART_BURST_COMMAND_48)
    {
        //  if classification of variable and trigger value not identical
        if ( (burstMsgConfDataPtr -> processValueClassification != reqDataPtr->deviceVariableClassForTriggerLevel)
            // to check jax 2012 1213
            && (reqDataPtr->deviceVariableClassForTriggerLevel != 0) )
        {
            // set error code: trigger mode on change only valid with command 48
            errorCode = RULE_VIOLATION_ERR;
            // error code invalid device variable classification in 1. response byte
            *pFirstResp  = HART_RESP_ERR_INVALID_DEVICE_VARIABLE_CLASS;
        }
        //  else if units code of variable and trigger value not identical
        else if (burstMsgConfDataPtr ->processValueUnitsCode != reqDataPtr->unitsCode)
        {
            // set error code: units code must be identically
            errorCode = RULE_VIOLATION_ERR;
            // error code invalid units code in 1. response byte
            *pFirstResp  = HART_RESP_ERR_INVALD_UNITS_CODE;
        }
        // else if trigger mode "windowed" AND trigger level negative
        else if (reqDataPtr->burstTriggerMode == HART_BURST_TRIGGER_MODE_WINDOW)
        {
            //if trigger level negative
            if (reqDataPtr->triggerLevel < 0)
            {
                // set error code: units code must be identically
                errorCode = RULE_VIOLATION_ERR;
                // error code parameter too small code in 1. response byte
                *pFirstResp  = HART_RESP_ERR_DATA_TOO_SMALL;
            }
        }
        else
        {
        }
    }
    else
    {
    }
    return (errorCode);
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief     HARTPrepareBurstTriggerObjectInfo: Prepare burst trigger objects classification, units code, value
 \author    IIMS
 \date      2010-02-02
 \param     slotCode:       number of slot code corresponding to defined slot codes for command 9
 \param     burstMsgNumber: number of burst message
 \return
 \warning
 \test      lint free level 3 OK: 2012-06-21
 \module    TEST-KIT_192 OK
 \bug       None
*/
//--------------------------------------------------------------------------------------------------

void HARTPrepareBurstTriggerObjectInfo(TUSIGN8 slotCode, TUSIGN8 burstMsgNumber)
{
    TUSIGN8                         idx, maxNoOfSlotCodes;
    TUSIGN16                        offsetOfSlotObjects, cmdNumberIndex, errorCode;
    COM_SPEC_TAB  const SLOW        *ptrSpecialCmdDescr;
    COM_OBJ_DESCR const SLOW        *slotCodeObjDescrPtr;
    TUSIGN8 const SLOW              *slotIdxTabPtr;
    const T_BURSTMSG_CONFVALUES     *burstMsgConfDataPtr;

    // code: not classified
    TUSIGN8                         classification = 0;
    // code: not used
    TUSIGN8                         unitsCode = 250;


    // set error before searching and reset after search with success
    errorCode = WRONG_STATE_ERR;

    // Search for index of command 9 in command index table
    cmdNumberIndex = HARTFindCmdTableIndex(9);

    // Command not implemented?
    if (cmdNumberIndex != INVALID_CMD_NUMBER_INDEX)
    {
        // get command description pointer from command table
        ptrSpecialCmdDescr = (void const SLOW *)commands[cmdNumberIndex].cmdDescr;
        // command description pointer must be not equal zero
        VIP_ASSERT(ptrSpecialCmdDescr);
        // get pointer to table of slot code specific object descriptions
        slotCodeObjDescrPtr = ptrSpecialCmdDescr->slotCodeObjDescr;
        // get max no. of slot codes
        maxNoOfSlotCodes = ptrSpecialCmdDescr->maxNoOfSlotCodes;
        // get pointer to slotcode table
        slotIdxTabPtr = ptrSpecialCmdDescr->slotCodeTranslationTable;
        // must be > 0
        VIP_ASSERT (slotIdxTabPtr);

        // search in all entries of slot code table
        for (idx = 0; idx < (maxNoOfSlotCodes + 1); idx++)
        {
            // if (slot code (device variable index) in actual table entry == slot code for command
            if (slotIdxTabPtr[idx] == slotCode)
            {
                // reset error: corresponding idx found
                errorCode = OK;
                // calculate entry in slot code specific object description table
                offsetOfSlotObjects = idx * ptrSpecialCmdDescr->noOfSlotObjects;
                // read device variable specific classification
                (void)HARTGetObjects((slotCodeObjDescrPtr + offsetOfSlotObjects) -> subsystemId,
                               (slotCodeObjDescrPtr + offsetOfSlotObjects) -> objId,
                               (void *) &classification);
                // read device variable specific units code
//                (void)HARTGetObjects((slotCodeObjDescrPtr + offsetOfSlotObjects + 1) -> subsystemId,
//                               (slotCodeObjDescrPtr + offsetOfSlotObjects + 1) -> objId,
//                               (void *) &unitsCode);
                 // read device variable specific units code
                 (void)HARTGetAttribute((slotCodeObjDescrPtr + offsetOfSlotObjects + 1) -> subsystemId,
                                        (slotCodeObjDescrPtr + offsetOfSlotObjects + 1) -> objId,
                                        (slotCodeObjDescrPtr + offsetOfSlotObjects + 1) -> attribIdx,
                                         (void FAST*) &unitsCode);
                // store subsystem index of process value to burst message specific object
                burstMsgConfValues.processValueSubSysId = (slotCodeObjDescrPtr + offsetOfSlotObjects + 2)->subsystemId;
                // store object index of process value to burst message specific object
                burstMsgConfValues.processValueObjId = (slotCodeObjDescrPtr + offsetOfSlotObjects + 2)->objId;
                // loop end criteria
                idx = maxNoOfSlotCodes + 1;
            }
        }
    }

    // store device variable classification to burst message specific object
    burstMsgConfValues.processValueClassification = classification;
    // store device variable units code to burst message specific object
    burstMsgConfValues.processValueUnitsCode = unitsCode;

    // pointer of burst message specific confi data reference table
    burstMsgConfDataPtr = burstMsgConfObjRefTable[burstMsgNumber].burstMsgConfObjPtr;

    // if classification OR units code of trigger objects OR device variable objects different
    if (   (errorCode != OK)
        || (classification != burstMsgConfDataPtr -> triggerDevVariableClassification)
        || (unitsCode != burstMsgConfDataPtr -> triggerUnitsCode))
    {
        // store "continous trigger mode code" as burst message specific trigger mode
        burstMsgConfValues.triggerModeSelectionCode = HART_BURST_TRIGGER_MODE_CONTINUOUS;
        // store device variable classification as burst message specific trigger classification
        burstMsgConfValues.triggerDevVariableClassification = classification;
        // store device variable units code as burst message specific trigger units code
        burstMsgConfValues.triggerUnitsCode = unitsCode;
        // store trigger value 0 to burst message specific object
        burstMsgConfValues.triggerValue = 0;
    }
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief     PrepareBurstBuf_LAYER7: Prepare LAYER7 receive buffer with request data bytes of burst command.
 \author    IIMS
 \date      2005-11-22
 \param     cmdNumber: command number.
 \return    Prepared Receive buffer of Layer7.
 \warning
 \test      lint free level 3 OK: 2012-06-21
 \module    TEST-KIT_192 OK
 \bug       None
*/
//--------------------------------------------------------------------------------------------------

void PrepareBurstBuf_LAYER7 (void)
{
    TFLOAT                          processValue, lastSentProcessValue;
    TUSIGN8                         idx, slot;
    TUSIGN16                        otherDiagnosis;
    TPOLY16                         cmdToBurst;
    const T_BURSTMSG_CONFVALUES     *burstMsgConfDataPtr;
    T_BURSTMSG_MANAGEVALUES         *burstMsgManageDataPtr;

    // non valid burst messsage number
    TUSIGN8                         burstMsgNo = 0xFF;


    // for all burst messages
    for (idx = 0; idx < hartCst.bursttotalNumberOfMsg; idx++)
    {
        // pointer of burst message specific confi objects reference table
        burstMsgConfDataPtr = burstMsgConfObjRefTable[idx].burstMsgConfObjPtr;

        // pointer of burst message specific management objects reference table
        burstMsgManageDataPtr = burstMsgManageObjRefTable[idx].burstMsgManageObjPtr;

        // if burst mode activated for current burst message AND maximum update period counter expired
        if (   (burstMsgConfDataPtr->modeControlCode == HART_BURSTMODE_ENABLE_ON_TOKEN_PASSING_DLL)
            && (burstMsgManageDataPtr -> maximumUpdatePeriodCounter == 0))
        {
            // if trigger mode "windowed" configured for burst message
            if (burstMsgConfDataPtr ->triggerModeSelectionCode == HART_BURST_TRIGGER_MODE_WINDOW)
            {
                // get process value belonging to burst message of current index
                (void)HARTGetObjects(burstMsgConfDataPtr ->processValueSubSysId,
                               burstMsgConfDataPtr->processValueObjId,
                               (void *)&(processValue));
                // put actual process value as trigger specific last sent process value
                // belonging to burst message of current index
                burstMsgManageDataPtr ->lastSentProcessValue = processValue;
            }
            // if trigger mode "on change" configured for burst message
            if (burstMsgConfDataPtr ->triggerModeSelectionCode == HART_BURST_TRIGGER_MODE_ON_CHANGE)
            {
                // get counter of last diagnosis status change
                // @@adjust
                //(void)HARTGetObjects(MAPPER_IDX, MAPPER_IDX_diagnosisCounter, &otherDiagnosis);
                // store counter value for change compare
                burstMsgManageDataPtr ->lastDiagnosisCounter = otherDiagnosis;

            }
            // to send command response from burst message belonging to index
            burstMsgNo = idx;
            // actualize counter of message with elapsed max update period
            counterBurstMsgToSend --;
            // loop end criteria
            idx = hartCst.bursttotalNumberOfMsg;
        }
    }
    // if no burst msg to send till now
    if (burstMsgNo == 0xFF)
    {
        // for all burst messages
        for (idx = 0; idx < hartCst.bursttotalNumberOfMsg; idx++)
        {
            // pointer of burst message specific confi objects reference table
            burstMsgConfDataPtr = burstMsgConfObjRefTable[idx].burstMsgConfObjPtr;

            // pointer of burst message specific management objects reference table
            burstMsgManageDataPtr = burstMsgManageObjRefTable[idx].burstMsgManageObjPtr;

            // if burst mode activated for current burst message AND minimum update period expired
            if (   (burstMsgConfDataPtr->modeControlCode == HART_BURSTMODE_ENABLE_ON_TOKEN_PASSING_DLL)
                && (burstMsgManageDataPtr -> updatePeriodCounter == 0))
            {
                // select trigger mode
                switch (burstMsgConfDataPtr ->triggerModeSelectionCode)
                {
                    default:
                    case (HART_BURST_TRIGGER_MODE_CONTINUOUS):
                        // to send command response from burst message belonging to index
                        burstMsgNo = idx;
                        // loop end criteria
                        idx = hartCst.bursttotalNumberOfMsg;
                        break;

                    case (HART_BURST_TRIGGER_MODE_WINDOW):
                        // burst message specific last sent process value
                        lastSentProcessValue = burstMsgManageDataPtr->lastSentProcessValue;
                        // get trigger specific last sent process value belonging
                        // to burst message of current index
                        (void)HARTGetObjects(burstMsgConfDataPtr->processValueSubSysId,
                                       burstMsgConfDataPtr->processValueObjId,
                                       (void *)&(processValue));
                        // if    process value < lower windowed trigger limit
                        //    OR process value > upper windowed trigger limit
                        if (   (processValue < (lastSentProcessValue - burstMsgConfDataPtr->triggerValue))
                            || (processValue > (lastSentProcessValue + burstMsgConfDataPtr->triggerValue)))
                        {
                            // to send command response from burst message belonging to index
                            burstMsgNo = idx;
                            // loop end criteria
                            idx = hartCst.bursttotalNumberOfMsg;
                            // put actual process value as trigger specific last sent process value
                            // belonging to burst message of current index
                            burstMsgManageDataPtr ->lastSentProcessValue = processValue;
                        }
                        break;

                    case (HART_BURST_TRIGGER_MODE_RISING):
                       // get trigger specific process value belonging
                        // to burst message of current index
                        (void)HARTGetObjects(burstMsgConfDataPtr->processValueSubSysId,
                                       burstMsgConfDataPtr->processValueObjId,
                                       (void *)&(processValue));
                        // if process value > trigger limit
                        if (processValue > burstMsgConfDataPtr->triggerValue)
                        {
                            // to send command response from burst message belonging to index
                            burstMsgNo = idx;
                            // loop end criteria
                            idx = hartCst.bursttotalNumberOfMsg;
                        }
                        break;

                    case (HART_BURST_TRIGGER_MODE_FAILING):
                       // get trigger specific process value belonging
                        // to burst message of current index
                        (void)HARTGetObjects(burstMsgConfDataPtr->processValueSubSysId,
                                       burstMsgConfDataPtr->processValueObjId,
                                       (void *)&(processValue));
                        // if process value < trigger limit
                        if (processValue < burstMsgConfDataPtr->triggerValue)
                        {
                            // to send command response from burst message belonging to index
                            burstMsgNo = idx;
                            // loop end criteria
                            idx = hartCst.bursttotalNumberOfMsg;
                        }
                        break;

                    case (HART_BURST_TRIGGER_MODE_ON_CHANGE):
                        // get counter of last diagnosis status change
                        // @@adjust
                      //comment 2011 0412
                        //(void)HARTGetObjects(MAPPER_IDX, MAPPER_IDX_diagnosisCounter, &otherDiagnosis);
                        // if status change since last send
                        if (otherDiagnosis != burstMsgManageDataPtr ->lastDiagnosisCounter)
                        {
                            // store actual value for next compare
                            burstMsgManageDataPtr ->lastDiagnosisCounter = otherDiagnosis;
                            // to send command response from burst message belonging to index
                            burstMsgNo = idx;
                            // loop end criteria
                            idx = hartCst.bursttotalNumberOfMsg;
                        }
                        break;
                }
            }
        }
    }
    // if no activated burst message found
    if (burstMsgNo == 0xFF)
    {
        // number of default burst message
        burstMsgNo = hartFreqNo.burstDefaultMsg;

        // init counter of message with elapsed max update period
        counterBurstMsgToSend = 0;
    }

    // pointer of burst message specific confi objects reference table
    burstMsgConfDataPtr = burstMsgConfObjRefTable[burstMsgNo].burstMsgConfObjPtr;
    // command number from specific burst message data
    cmdToBurst.u16 = burstMsgConfDataPtr->commandNumber;

    // *** prepare needed request data ***
    // command number from specific burst message data
    receiveBuffer_LAYER7.command = cmdToBurst.aryU8[0];
    // set request data length 0
    receiveBuffer_LAYER7.numByte = 0;

    // if (burst command #9 or #33)
    if ((cmdToBurst.aryU8[0] == 9) || (cmdToBurst.aryU8[0] == 33))
    {
        // for all posible slot numbers
        for (idx = 0; idx < MAX_SLOT_NUMBER; idx++)
        {
            // select slot index
            switch (idx)
            {
                default:
                case 0:
                    // get code of slot 0
                    slot = burstMsgConfDataPtr->burstSlot0;
                    break;
                case 1:
                    // get code of slot 1
                    slot = burstMsgConfDataPtr->burstSlot1;
                    break;
                case 2:
                    // get code of slot 2
                    slot = burstMsgConfDataPtr->burstSlot2;
                    break;
                case 3:
                    // get code of slot 3
                    slot = burstMsgConfDataPtr->burstSlot3;
                    break;
                case 4:
                    // get code of slot 4
                    slot = burstMsgConfDataPtr->burstSlot4;
                    break;
                case 5:
                    // get code of slot 5
                    slot = burstMsgConfDataPtr->burstSlot5;
                    break;
                case 6:
                    // get code of slot 6
                    slot = burstMsgConfDataPtr->burstSlot6;
                    break;
                case 7:
                    // get code of slot 7
                    slot = burstMsgConfDataPtr->burstSlot7;
                    break;
            }
            // if slot code configured in slot 1 - 7 (1 - 3)
            if ((idx == 0) || (slot != SLOT_CODE_NONE))
            {
                // store slot code of current index in receive buffer
                receiveBuffer_LAYER7.aryData[idx] = slot;
                // increment byte count
                receiveBuffer_LAYER7.numByte += 1;
            }
            // else not used slot found
            else
            {
                // truncate burst message
                idx = MAX_SLOT_NUMBER;
            }
        }
    }
    //prepare transmitbuffer for transmission of burst frame
    PrepareBurstBuf_LAYER2(cmdToBurst.aryU8[0]);

    // prepare layer 7 transmit buffer
    transmitBuffer_LAYER7 = uTransmitBuffer_LAYER2;

    // pointer of burst message specific management objects reference table
    burstMsgManageDataPtr = burstMsgManageObjRefTable[burstMsgNo].burstMsgManageObjPtr;
    // set counter to value "update period (1/32 msec) / command interpreter cycle time (100 msec)"
    burstMsgManageDataPtr->updatePeriodCounter
        = burstMsgConfDataPtr->updatePeriod / (HART_LAYER7_CMD_INTERPRETER_CYCL_TIME * 32);
    // set counter to value "maximum update period (1/32 msec) / command interpreter cycle time (100 msec)"
    burstMsgManageDataPtr->maximumUpdatePeriodCounter
        = burstMsgConfDataPtr->maximumUpdatePeriod / (HART_LAYER7_CMD_INTERPRETER_CYCL_TIME * 32);
}

