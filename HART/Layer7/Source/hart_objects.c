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
#include "T_Unit/Interface/t_unit.h"


// HART Layer includes
#include "Coordinator/Interface/subsystem_idx.h"
#include "HART/Layer2/Interface/layer2.h"
#include "ObjfunctRB.h"
#include "cmddef.h"


#include "HART/Interface/t_data_obj_burstmsg_confvalues.h"
#include "HART/Interface/t_data_obj_burstmsg_managevalues.h"
#include "HART/Layer7/Interface/layer7.h"
#include "HART/Source/Hart_types.h"


//added
#include "hart_unit_convert.h"
#include "HART/Interface/Hart.h"

// Get the SUBSYSTEM IDs for local usage
#include "HART/Interface/Hart_idx.h"
#include "HART/Source/Hart_attributes.h"

#include "Mapper/Interface/Mapper_Config.h"

#include "UnitsOfMeasure/Interface/UnitsOfMeasure.h"
#include "ARM/Interface/arm.h"
// Local includes
#include "hart_arm.h"

// Buffer used to prepare a summary of request/response object commands for the following Put/Get action
/*static*/ COM_OBJ_DESCR               objComSpecCmd[70];


//--------------------------------------------------------------------------------------------------
/*!
 \brief     HART_ConfigurationChange: configuration change handling.
 \author    Bernhard Tohermes
 \date      2010-07-01
 \param
 \param
 \return
 \warning
 \test
 test-date:
 \n by:
 \n environment:
 \n lint check o.k.: B. Tohermes; 30-08-2010 \n result
 \n result
 \bug
*/
//--------------------------------------------------------------------------------------------------

void HART_ConfigurationChange(void)
{
    // init configuration flags
    TUSIGN8     configurationFlags  =   0;
    // actual configuration change counter value
    TUSIGN16    configChangeCounterValue = hartFreqNo.confChangeCounterValue;
    // increment configuration change counter value
    configChangeCounterValue++;
    // actualize value of configuration change counter object
    (void)HARTPutObjects(HART_IDX, HART_IDX_confChangeCounterValue, (void*) &configChangeCounterValue);

    // set configuration change flag for every master (write to whole object)
    (void)HARTPutObjects(HART_IDX, HART_IDX_configurationFlags, (void*)&configurationFlags);
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     HARTSpecialReadWriteObjects: Handles special Read/Write HART commands.
 \author    Horst Seele
 \date      2005-11-18
 \param     commandNumberIndex = Index of command in command table.
 \param     contained in command descriptions
 \return    HART command response
 \warning
 \test
 test-date: 2008-04-29
 \n by:     Marcel Schilg
 \n environment: old Modultest from HART subsystem and HCF Data Linklayer Test scripts
 \n intention: sight check
 \n result  module test: OK
 \n result  Lint Level 3: free // approved warnings and approved infos are suppressed
 \n lint check o.k.: B. Tohermes; 30-08-2010
 \bug
*/
//--------------------------------------------------------------------------------------------------

void HARTSpecialReadWriteObjects(TUSIGN16 commandNumberIndex, T_LAYER2_BUFFER* pRxBuffer, T_LAYER2_BUFFER* pTxBuffer)
{
    TUSIGN8                     *rcvBufPointer;
    TUSIGN8                     reqObjectCount = 0, respObjectCount = 0, usedReqRespObjCntTabEntries = 0;
    TUSIGN8                     idx, entriesInReqRespCntObjTab, maxNoOfSlotCodes = 0;
    TUSIGN8                     slotCode, actualSlotIdx, objCntPerSlot, preSlotObjCnt, postSlotObjCnt;
    TUSIGN16                    errorCode, offsetOfSlotObjects, objectIdx, offsetOfReqRespObjects, lastObjectIdx;
    COM_SPEC_TAB  const SLOW    *ptrSpecialCmdDescr;
    COM_REQ_RESP_CNT const SLOW *reqRspObjPtr;
    COM_OBJ_DESCR const SLOW    *slotCodeObjDescrPtr;
    TUSIGN8 const SLOW          *slotIdxTabPtr;

    // commandNumberIndex must be valid
    VIP_ASSERT(commandNumberIndex < lengthOfCommandTable);

    // get command description pointer from command table
    ptrSpecialCmdDescr = (void const SLOW *)commands[commandNumberIndex].cmdDescr;
    // command description pointer must be not equal zero
    VIP_ASSERT(ptrSpecialCmdDescr);

    // get pointer to table of request/response object count
    reqRspObjPtr = (void const SLOW *)ptrSpecialCmdDescr->reqRespObjCnt;
    // pointer must be not equal zero
    VIP_ASSERT(reqRspObjPtr);

    // get entries in table of request/response object counts
    entriesInReqRespCntObjTab = ptrSpecialCmdDescr->lenOfRespObjCntTab;
    // length must be > 0
    VIP_ASSERT(entriesInReqRespCntObjTab);

    // get pointer to table of slot code specific object descriptions
    slotCodeObjDescrPtr = ptrSpecialCmdDescr->slotCodeObjDescr;
    // if request with slot commands
    if (slotCodeObjDescrPtr)
    {
        // get max no. of slot codes
        maxNoOfSlotCodes = ptrSpecialCmdDescr->maxNoOfSlotCodes;
        // get no. of object descriptions for every slot
        objCntPerSlot = ptrSpecialCmdDescr->noOfSlotObjects;
        // get pointer to slotcode table
        slotIdxTabPtr = ptrSpecialCmdDescr->slotCodeTranslationTable;
        // must be > 0
        VIP_ASSERT (slotIdxTabPtr);
    }
    


    // set error code not OK
    errorCode = WRONG_STATE_ERR;

    // for all entries in request/response object counts table
    for (idx = 0; idx <= entriesInReqRespCntObjTab; idx++)
    {
        // used number of entries in table of request/response object counts also slots in request
        usedReqRespObjCntTabEntries++;
        // counter of request objects
        reqObjectCount = reqRspObjPtr->noReqObjects;
        // counter of response objects
        respObjectCount = reqRspObjPtr->noRespObjects;

        // if number of received request data bytes == maximum command request data bytes
        if (pRxBuffer->numByte == reqRspObjPtr->noReqDatabytes)
        {
            // set loop end criteria
            idx = entriesInReqRespCntObjTab;
            // set error code = OK
            errorCode = OK;
        }
        // else if (last possible table entry and received request data bytes > maximum command request data bytes)
        else if ((idx == (entriesInReqRespCntObjTab-1)) && (pRxBuffer->numByte > reqRspObjPtr->noReqDatabytes))
        {
            // set error code = OK
            errorCode = OK;
        }
        // else
        else
        {
            // set pointer to next table entry
            reqRspObjPtr = reqRspObjPtr + 1;
        }
    }

    // contents of request/response object counts table must be valid
    VIP_ASSERT (errorCode == OK);

    // if command == command 6 in compliant to hart rev 5 working device
    if (   (commands[commandNumberIndex].cmdNumber == 6)
        && (hartFreqCst.univCmdRev == 5))
    {
            // counter of response objects--:
            // only polling address to read as response data
            respObjectCount--;
    }

    // copy default response command objects corresponding to counter of response objects into RAM
    //lint -e(619) B. Tohermes 2010-08-18: precision of arg. no. 2 o.k.
    (void)memcpy(objComSpecCmd, ptrSpecialCmdDescr->cmdDefReqDescr, (sizeof(COM_OBJ_DESCR) * reqObjectCount));

    // if (command has slots)
    if (slotCodeObjDescrPtr)
    {
        // set all slot codes to �none?(code = 251)
        slotCode = SLOT_CODE_NONE;
        
        ENABLE_DATA_PROTECTION(*(hart.unit.ptrDataSemaphore));
        hartDyn.slot0 = slotCode;
        hartDyn.slot1 = slotCode;
        hartDyn.slot2 = slotCode;
        hartDyn.slot3 = slotCode;
        hartDyn.slot4 = slotCode;
        hartDyn.slot5 = slotCode;
        hartDyn.slot6 = slotCode;
        hartDyn.slot7 = slotCode;
        DISABLE_DATA_PROTECTION(*(hart.unit.ptrDataSemaphore));

        //  if (write command)
        if (commands[commandNumberIndex].fct == HART_SPECIAL_WRITE)
        {
            // if extended command
            if (commands[commandNumberIndex].cmdNumber > 255)
            {
                // set receive buffer pointer to begin of data area behind command number
                rcvBufPointer = pRxBuffer->aryData + 2;
            }
            else
            {
                // set receive buffer pointer to begin of data area
                rcvBufPointer = pRxBuffer->aryData;
            }
            // actual slot idx = slot0
            actualSlotIdx = 0;

            // for (all objects in received request)
            for (objectIdx = 0; objectIdx < reqObjectCount; objectIdx++)
            {
                // if (object description in command description is slot)
                if ((objComSpecCmd[objectIdx].subsystemId == HART_IDX) && \
                    ((objComSpecCmd[objectIdx].objId == HART_IDX_slot0) || \
                     (objComSpecCmd[objectIdx].objId == HART_IDX_slot1) || \
                     (objComSpecCmd[objectIdx].objId == HART_IDX_slot2) || \
                     (objComSpecCmd[objectIdx].objId == HART_IDX_slot3) || \
                     (objComSpecCmd[objectIdx].objId == HART_IDX_slot4) || \
                     (objComSpecCmd[objectIdx].objId == HART_IDX_slot5) || \
                     (objComSpecCmd[objectIdx].objId == HART_IDX_slot6) || \
                     (objComSpecCmd[objectIdx].objId == HART_IDX_slot7)))
                {
                    // emergency brake: request/response object counts not equal
                    VIP_ASSERT (reqObjectCount == respObjectCount);

                    // get slot code from received command
                    slotCode = *rcvBufPointer;

                    // set error before searching and reset after search with success
                    errorCode = WRONG_STATE_ERR;
                    // search in all entries of slot code table
                    for (idx = 0; idx < (maxNoOfSlotCodes + 1); idx++)
                    {
                        // if (slot code in actual table entry == slot code for command
                        //lint -e(644) B. Tohermes 2010-08-18: only used with slots; then initialized
                        if (slotIdxTabPtr[idx] == slotCode)
                        {
                            // reset error: corresponding idx found
                            errorCode = OK;

                            // calculate entry in slot code specific object description table
                            offsetOfSlotObjects = (idx * ptrSpecialCmdDescr->noOfSlotObjects);

                            // if slot object not part of slot specific command objects (leading slot)
                            //lint -e(644) B. Tohermes 2010-08-18: only used with slots; then initialized
                            if (objCntPerSlot != reqObjectCount)
                            {
                                // index of request object just behind actual slot object
                                offsetOfReqRespObjects = objectIdx + 1;
                                // index of next slot used also as counter of till now processed slots
                                actualSlotIdx++;
                                // calculate absolute last object description index of slot code description
                                lastObjectIdx = objectIdx + ptrSpecialCmdDescr->noOfSlotObjects;

                            }
                            // slot object part of slot specific command objects (contained slot)
                            else
                            {
                                // index of first request object belonging to actual slot object
                                offsetOfReqRespObjects = (actualSlotIdx * ptrSpecialCmdDescr->noOfSlotObjects);
                                // index of next slot used also as counter of till now processed slots
                                actualSlotIdx++;
                                // calculate absolute last object description index of slot code description
                                lastObjectIdx = (actualSlotIdx * ptrSpecialCmdDescr->noOfSlotObjects);
                            }
                            // replace default command description part with slot code specific part
                            //lint -e(619) B. Tohermes 2010-08-18: precision of arg. no. 2 o.k.
                            (void)memcpy(objComSpecCmd + offsetOfReqRespObjects,
                                         ptrSpecialCmdDescr->slotCodeObjDescr + offsetOfSlotObjects,
                                         sizeof(COM_OBJ_DESCR) * ptrSpecialCmdDescr->noOfSlotObjects);

                            // set receive buffer pointer to data of last slot code specific object
                            for (; objectIdx < lastObjectIdx; objectIdx++)
                            {
                                // set receive buffer pointer to data of next object
                                rcvBufPointer = rcvBufPointer + mapParaLength[(objComSpecCmd + objectIdx)->hartLenCode];
                            }

                            // stop searching in slot code table
                            idx = maxNoOfSlotCodes;
                             //@ToDo
                             //Check Whether the Device Variable is supported by the Device,this code section is specific to the different device
                             if((commands[commandNumberIndex].cmdNumber==53)
                                 ||(commands[commandNumberIndex].cmdNumber==55))
                             {
                                 //if(!((TBOOL)eTRUE==IsLableExist(slotCode)))
                               if(OK!=CheckIsSupportedMeasurement(slotCode))
                                 {

                                    pTxBuffer->firstResp = HART_RESP_ERR_INVALID_SELECTION;
                                    pTxBuffer->numByte   = 2;
                                    errorCode = WRITE_ERROR;

                                 }
                              }
                             //@End ToDo
                        }
                    }
                }
                // set receive buffer pointer to data of next object
                rcvBufPointer = rcvBufPointer + mapParaLength[(objComSpecCmd + objectIdx)->hartLenCode];

                // if (error)
                if (errorCode != OK)
                {
                    // set WRITE ERROR
                    errorCode = WRITE_ERROR;
                    if(pTxBuffer->command == 53)
                    {
                            pTxBuffer->firstResp = HART_RESP_ERR_INVALD_UNITS_CODE;
                    }
                    else
                    {
                        // set response code INVALID_SELECTION
                        pTxBuffer->firstResp = HART_RESP_ERR_INVALID_SELECTION;
                    }
                    // response contains only status bytes
                    pTxBuffer->numByte   = 2;

                    // if (extended command)
                    if (commands[commandNumberIndex].cmdNumber > 255)
                    {
                        // response contains status bytes and command number
                        pTxBuffer->numByte = 4;
                    }

                    // break of replace request objects
                    objectIdx = reqObjectCount;  // end FOR loop
                }
            }
        }
    }

    // if (no error)
    if (errorCode == OK)
    {
        // if no error during write received data into objects
        if (!HARTWriteObjects(objComSpecCmd, reqObjectCount, commands[commandNumberIndex].configChanged, pRxBuffer, pTxBuffer))
        {
            // copy default response command objects corresponding to counter of response objects into RAM
            //lint -e(619) B. Tohermes 2010-08-18: precision of arg. no. 2 o.k.
            (void)memcpy(objComSpecCmd, ptrSpecialCmdDescr->cmdDefRespDescr, (sizeof(COM_OBJ_DESCR) * respObjectCount));

            // if device works compliant to hart rev. 5 and cmd 11/21 to process
            if ((hartFreqCst.univCmdRev == 5) &&
                ((pRxBuffer->command == 11) || (pRxBuffer->command == 21)))
            {
                // truncate object command processing after 10th object
                respObjectCount = 10;
            }

            // if (command has slots)
            if (ptrSpecialCmdDescr->slotCodeObjDescr)
            {
               // get pointer to table of request/response object count
                reqRspObjPtr = (void const SLOW *)ptrSpecialCmdDescr->reqRespObjCnt;
                // no. of response objects before first object of first slot (preSlotObjects)
                preSlotObjCnt = reqRspObjPtr->noRespObjects - (objCntPerSlot + 1);
                // no. of response objects behind last object of last possible slot (postSlotObjects)
                postSlotObjCnt = (((reqRspObjPtr + entriesInReqRespCntObjTab) - 1)->noRespObjects
                                    - (TUSIGN8)((entriesInReqRespCntObjTab * (objCntPerSlot + 1))))
                                    - preSlotObjCnt;

                // actual slot idx = slot0
                actualSlotIdx = 0;


                // for all object commands contained in RAM response:
                // exchange slot specific object commands in RAM response with
                // slot specific object commands corresponding to slot codes out of request
                VIP_ASSERT((respObjectCount - postSlotObjCnt) >= 0);
                for (objectIdx = preSlotObjCnt; objectIdx < ((TUSIGN8)(respObjectCount - postSlotObjCnt)); objectIdx++)
                {

                    // set error before searching and reset after search with success
                    errorCode = WRONG_STATE_ERR;
                    // if (actual object command in RAM response is slot command)
                    if ((objComSpecCmd[objectIdx].subsystemId == HART_IDX) && \
                        ((objComSpecCmd[objectIdx].objId == HART_IDX_slot0) || \
                         (objComSpecCmd[objectIdx].objId == HART_IDX_slot1) || \
                         (objComSpecCmd[objectIdx].objId == HART_IDX_slot2) || \
                         (objComSpecCmd[objectIdx].objId == HART_IDX_slot3) || \
                         (objComSpecCmd[objectIdx].objId == HART_IDX_slot4) || \
                         (objComSpecCmd[objectIdx].objId == HART_IDX_slot5) || \
                         (objComSpecCmd[objectIdx].objId == HART_IDX_slot6) || \
                         (objComSpecCmd[objectIdx].objId == HART_IDX_slot7)))
                    {
                        // get slot code
                        (void)HARTGetObjects(HART_IDX, objComSpecCmd[objectIdx].objId, &slotCode);

                        // search in all entries of slot code table
                        for (idx = 0; idx < (maxNoOfSlotCodes + 1); idx++)
                        {
                            // if (slot code in actual table entry == slot code for command
                            if (slotIdxTabPtr[idx] == slotCode)
                            {
                                // reset error: corresponding idx found
                                errorCode = OK;
                                // calculate entry in slot code specific object description table
                                offsetOfSlotObjects = (TUSIGN16)(idx * ptrSpecialCmdDescr->noOfSlotObjects);

                                // if slot object not part of slot specific command objects (leading slot)
                                if (objCntPerSlot != respObjectCount)
                                {
                                    // index of request object just behind actual slot object
                                    offsetOfReqRespObjects = objectIdx + 1;

                                    // calculate absolute last object description index of slot code description
                                    objectIdx = objectIdx + ptrSpecialCmdDescr->noOfSlotObjects;

                                }
                                // slot object part of slot specific command objects (contained slot)
                                else
                                {
                                    // index of first request object belonging to actual slot object
                                    offsetOfReqRespObjects = (TUSIGN16)(actualSlotIdx * ptrSpecialCmdDescr->noOfSlotObjects);
                                    // index of next slot used also as counter of till now processed slots
                                    actualSlotIdx++;
                                    // calculate absolute last object description index of slot code description
                                    objectIdx = preSlotObjCnt + (TUSIGN8)(actualSlotIdx * ptrSpecialCmdDescr->noOfSlotObjects);
                                }
                                // replace default command description part with slot code specific part
                                //lint -e(619) B. Tohermes 2010-08-18: precision of arg. no. 2 o.k.
                                (void)memcpy(objComSpecCmd + offsetOfReqRespObjects,
                                             ptrSpecialCmdDescr->slotCodeObjDescr + offsetOfSlotObjects,
                                             sizeof(COM_OBJ_DESCR) * ptrSpecialCmdDescr->noOfSlotObjects);

                                // stop searching in slot code table
                                idx = maxNoOfSlotCodes;
                            }
                        }
                        // if device variable not found as slot code
                        if (errorCode == WRONG_STATE_ERR)
                        {
                            // if device variable code < 255
                            if (slotCode < 255)
                            {
                                // reset error: corresponding idx found
                                errorCode = OK;
                                // if slot object not part of slot specific command objects (leading slot)
                                if (objCntPerSlot != respObjectCount)
                                {
                                    // index of request object just behind actual slot object
                                    offsetOfReqRespObjects = objectIdx + 1;

                                    // calculate absolute last object description index of slot code description
                                    objectIdx = objectIdx + ptrSpecialCmdDescr->noOfSlotObjects;

                                }
                                // slot object part of slot specific command objects (contained slot)
                                else
                                {
                                    // index of first request object belonging to actual slot object
                                    offsetOfReqRespObjects = (TUSIGN16)(actualSlotIdx * ptrSpecialCmdDescr->noOfSlotObjects);
                                    // index of next slot used also as counter of till now processed slots
                                    actualSlotIdx++;
                                    // calculate absolute last object description index of slot code description
                                    objectIdx = preSlotObjCnt + (TUSIGN8)(actualSlotIdx * ptrSpecialCmdDescr->noOfSlotObjects);
                                }

                                // replace default command description part with non valid device variable specific codes
                                (void)memcpy((objComSpecCmd + offsetOfReqRespObjects),
                                            specObjCom9_nonValidDeviceVariableObjDescrTab,
                                            (sizeof(COM_OBJ_DESCR) * ptrSpecialCmdDescr->noOfSlotObjects));
                            }
                            else
                            {

                            // set response code INVALID_SELECTION
                            pTxBuffer->firstResp = HART_RESP_ERR_INVALID_SELECTION;
                            pTxBuffer->numByte   = 2;
                            errorCode = WRITE_ERROR;

                            // if (extended command), number of response data bytes = 4
                            if (commands[commandNumberIndex].cmdNumber > 255)
                            {
                                pTxBuffer->numByte = 4;
                            }
                            // end FOR loop:
                            objectIdx = respObjectCount;
                            }
                        }
                    }
                }
                // if (no error)
                if (errorCode == OK)
                {

                    // if (   (request with more than 1 possible variant but not with max possible variants)
                    //     && (commands exist behind last possible slot command)
                    if (   (entriesInReqRespCntObjTab > 1)
                        && (usedReqRespObjCntTabEntries < entriesInReqRespCntObjTab)
                        && (postSlotObjCnt != 0))
                    {
                        // copy postSlotObjects behind last objects of last used slot
                        //lint -e(619) B. Tohermes 2010-08-18: precision of arg. no. 2 o.k.
                        (void)memcpy(objComSpecCmd
                                         + preSlotObjCnt + ((objCntPerSlot + 1) * usedReqRespObjCntTabEntries),
                                         (ptrSpecialCmdDescr->cmdDefRespDescr)
                                         + preSlotObjCnt + ((TUSIGN16)((objCntPerSlot + 1) * entriesInReqRespCntObjTab)),
                                         (sizeof(COM_OBJ_DESCR) * postSlotObjCnt));
                        // correction of resp no.
                        respObjectCount = respObjectCount + postSlotObjCnt;
                    }
                }
            }
            // if (no error)
            if (errorCode == OK)
            {
                // read objects and store in response
                HARTReadObjects(objComSpecCmd, respObjectCount, pRxBuffer, pTxBuffer);
            }
        }
    }
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief     HARTReadObjects: Reads an object or attribute from a subsystem
 \author    Stefan Tabelander, Horst Seele
 \date      2005-12-06
 \param     cmdDescr = Pointer to command structure
 \param     objectCount = Number of objects in response
 \param     command number in transmit buffer
 \param     contained in 'commands' table
 \return    command response in transmit buffer
 \warning
 \test
 test-date: 2008-04-29
 \n by:     Marcel Schilg
 \n environment: old Modultest from HART subsystem and HCF Data Linklayer Test scripts
 \n intention: sight check
 \n result  module test: OK
 \n result  Lint Level 3: free // approved warnings and approved infos are suppressed
 \n lint check o.k.: B. Tohermes; 30-08-2010
 \bug
*/
//--------------------------------------------------------------------------------------------------

void HARTReadObjects(COM_OBJ_DESCR const SLOW *cmdDescr, TUSIGN8 objectCount, T_LAYER2_BUFFER* pRxBuffer, T_LAYER2_BUFFER* pTxBuffer)
{
    /* variable declaration */
    TUSIGN8          i, idx, bufferIdx, attribIdx, attribId, hartLenCode;
    TUSIGN8          dataTypeId, element, numOfElements;
    TUSIGN16         objId, errorCode;
    T_SUBSYSTEM_IDX  subsystemId;

    /* pointer declaration */
    void             *pobjTxValue;
    COM_OBJ_DESCR    commandDescriptor;

    // Pointer to command structure must be not zero
    VIP_ASSERT(cmdDescr != 0);

    // init values
    hartLenCode = 0;
    dataTypeId  = 0;
    errorCode  = OK;

    // set transmit buffer data area start
    bufferIdx = 0;
    if(pTxBuffer->command == EXTENSION_COMMAND_NUMBER)
    {
        bufferIdx = 2;      // if extended command data area begins after extended command number
    }

    // for all attributes of the command
    for (i = 0; i < objectCount; i++)
    {
        // get data for access function
        commandDescriptor = cmdDescr[i];
        subsystemId = commandDescriptor.subsystemId;
        objId       = commandDescriptor.objId;
        attribId    = commandDescriptor.attribId;
        attribIdx   = commandDescriptor.attribIdx;
        hartLenCode = mapParaLength[commandDescriptor.hartLenCode];
        dataTypeId  = commandDescriptor.dataTypeId;

        // Indicates the position in transmit buffer
        pobjTxValue = &(pTxBuffer->aryData[bufferIdx]);


        // If (out of bounds of receive buffer due to erroneous command object description), ...
        if (bufferIdx > (HART_XMIT_RCV_BUFFER_LEN - hartLenCode))
        {
            // set error
            errorCode = WRONG_STATE_ERR;
        }
        // Whole object
        else if (attribId == OBJECT)
        {
            // Init local buffer index
            idx = bufferIdx;

            // Read as object
            errorCode = HARTGetObjects(subsystemId, objId, pobjTxValue);

            switch (dataTypeId)
            {
                case TD_T_RANGE_FLT_U:
                    for(element=0; element<3; element++)
                    {
                        // if (data conversion not possible) return WRONG_STATE_ERROR
                        if (!HARTConvertType(t_range_float_u[element].dataTypeId, idx, ACCESS_READ, pRxBuffer, pTxBuffer))
                        {
                            errorCode = WRONG_STATE_ERR;
                        }

                        idx = idx+t_range_float_u[element].lenCode;
                    }

                    break;

                case TD_PASC_ASC:
                    // if (conversion not possible) return WRONG_STATE_ERROR
                    if (!HARTConvertPASCII(hartLenCode, bufferIdx, ACCESS_READ, pRxBuffer, pTxBuffer))
                    {
                        errorCode = WRONG_STATE_ERR;
                    }

                    break;
                    //Special TD_USIGN32 for mapping to old TZIDC
                    //In old code the object will not rotated the bytes
                case TD_SPECIAL_U32:
                    break;
                case TD_UNITTYPE:
                    {
                        TUSIGN8 sysUnitType;
                        TUSIGN8 hartUnitType;
                        
                        sysUnitType = *(TUSIGN8 SLOW *)pobjTxValue;
                        if (!HartConvUnitTypeFromSystemUnit_HART(&sysUnitType, &hartUnitType))
                        {
                            errorCode = WRONG_STATE_ERR;
                        }
                        else
                        {
                            *(TUSIGN8 SLOW *)pobjTxValue = hartUnitType;
                        } 
                    }
                    break;
                    
                case TD_DOUBLE:
                    {                     
                        if (!HARTConvertType(EIGHT_BYTES,bufferIdx,ACCESS_READ, pRxBuffer, pTxBuffer))
                        {
                            errorCode = WRONG_STATE_ERR;
                        }
                        else
                        {
                            
                        } 
                    }
                    break;
                    
                default:
                    // Check for arrays
                    numOfElements = hartLenCode / dataTypeLength[dataTypeId];

                    // Length code and data type mismatch
                    VIP_ASSERT((hartLenCode % dataTypeLength[dataTypeId]) == 0);

                    if( numOfElements > 1 )
                    {
                        for(element=0; element < numOfElements; element++)
                        {
                            // if (data conversion not possible) return WRONG_STATE_ERROR
                            if (!HARTConvertType(dataTypeId, idx, ACCESS_READ, pRxBuffer, pTxBuffer))
                            {
                                errorCode = WRONG_STATE_ERR;
                            }

                            idx = idx+dataTypeLength[dataTypeId];
                        }
                    }
                    else
                    {
                        // if (data conversion not possible) return WRONG_STATE_ERROR
                        if (!HARTConvertType(dataTypeId, idx, ACCESS_READ, pRxBuffer, pTxBuffer))
                        {
                            errorCode = WRONG_STATE_ERR;
                        }
                    }

                    break;
            }
        }
        // Attribute
        else
        {
            // No error when read.  Always read attributes individually!
            errorCode = HARTGetAttribute(subsystemId, objId, attribIdx, (void FAST*)pobjTxValue);

            // if (HART data format is PASCII) convert ASCII in PASCII
            if (dataTypeId == TD_PASC_ASC)
            {
                // if (conversion not possible) return WRONG_STATE_ERROR
                if (!HARTConvertPASCII(hartLenCode, bufferIdx, ACCESS_READ, pRxBuffer, pTxBuffer))
                {
                    errorCode = WRONG_STATE_ERR;
                }
            }
            else if (dataTypeId == TD_UNITTYPE)
            {
                TUSIGN8 sysUnitType;
                TUSIGN8 hartUnitType;
                
                sysUnitType = *(TUSIGN8 SLOW *)pobjTxValue;
                if (!HartConvUnitTypeFromSystemUnit_HART(&sysUnitType, &hartUnitType))
                {
                    errorCode = WRONG_STATE_ERR;
                }
                else
                {
                    *(TUSIGN8 SLOW *)pobjTxValue = hartUnitType;
                } 
            }
            else if (dataTypeId == TD_DOUBLE)   
            {                  
                if (!HARTConvertType(EIGHT_BYTES,bufferIdx,ACCESS_READ, pRxBuffer, pTxBuffer))
                {
                    errorCode = WRONG_STATE_ERR;
                }
                else
                {
                    
                } 
            }
            //Added for Unit Convern between HART-Unit and System Unit
            else if(dataTypeId == TD_UNIT)
            {
                //TODO TODO
                TUSIGN8 unitCode, unitsType;
                //assert(attribIdx == UOM_PF_unit);// TD_UNIT was not stated at an attribute UOM_PF_unit
                if( errorCode < (TUSIGN16)ERROR_CODES )
                {
                  // caution: the following attribute after the unit must be the unitsType !!!
                    if( (TUSIGN16)ERROR_CODES > HARTGetAttribute(subsystemId, objId, attribIdx+1, &unitsType) )
                    {
                        unitCode = *(TUSIGN8 SLOW *)pobjTxValue;
                        //Control Mapper Return Value
                        if(unitCode!=0xFA)
                        {
                            if( HartConvUnitCodeFromSystemUnit_HART( &unitCode, &unitsType ) )
                            {
                                *(TUSIGN8 SLOW *)pobjTxValue = unitCode;
                            }
                            else
                            {
                                errorCode = WRONG_STATE_ERR;
                            }
                        }

                    }
                }
            }



            // else normal convert
            else
            {
                // if (data conversion not possible) return WRONG_STATE_ERROR
                if (!HARTConvertType(dataTypeId, bufferIdx, ACCESS_READ, pRxBuffer, pTxBuffer))
                {
                    errorCode = WRONG_STATE_ERR;
                }
            }
        }
        // Index on position from where to write next value
        bufferIdx = bufferIdx + hartLenCode;

        // If (Attribute or Object could not read) end FOR loop
        if (errorCode >= ERROR_CODES)
        {
            i = objectCount;
        }
    }

    // if (read error), ...
    if (errorCode >= ERROR_CODES)
    {
        // set response code (numByte ok from command interpreter)
        pTxBuffer->firstResp = mapErrFirstResponse[errorCode-WRITE_ERROR];
    }
    // if (read OK), ...
    else
    {
        // Append number of response bytes (if applicable include expand cmd number) to standard 2 response bytes
        pTxBuffer->numByte = bufferIdx + 2;
    }
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief     Reads an object from a subsystem
            Add type Cast (void FAST*)ptrValue
 \author    Stefan Tabelander,Eric-Jiping.li
 \date      2004-07-27,2011-04-12
 \param
    <pre>
       subsysIdx:       Subsystem to be addressed
       objIdx:          Object index where to find the parameter
       ptrValue:        data pointer
    <\pre>
 \return    for return code see T_DATA_OBJ_RETURNCODES
 \warning
 \test
 test-date: 2008-04-29
 \n by:     Marcel Schilg
 \n environment: old Modultest from HART subsystem and HCF Data Linklayer Test scripts
 \n intention: sight check
 \n result  module test: OK
 \n result  Lint Level 3: free // approved warnings and approved infos are suppressed
 \n lint check o.k.: B. Tohermes; 30-08-2010
 \bug
*/
//--------------------------------------------------------------------------------------------------

TUSIGN16 HARTGetObjects(T_SUBSYSTEM_IDX subsysIdx, TUSIGN16 objIdx, void *ptrValue)
{
    TUSIGN16            returnCode;
    const T_UNIT SLOW   *pSub;          // Pointer to Subsystem

    pSub = GetSubsystemPtr(subsysIdx);  // Get the pointer

    returnCode = pSub->Get(pSub,objIdx,WHOLE_OBJECT,(void FAST*)ptrValue);

    return (returnCode);
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief     Reads an attribute from a subsystem
 \author    Stefan Tabelander,Eric-Jiping.li
 \date      2004-07-27
 \param
    <pre>
       subsysIdx:       Subsystem to be addressed
       objIdx:          Object index where to find the parameter
       attribIdx:       subindex in object 'objIdx'
       ptrValue:        data pointer
    <\pre>
 \return    for return code see T_DATA_OBJ_RETURNCODES
 \warning
 \test
 test-date: 2008-04-29
 \n by:     Marcel Schilg
 \n environment: old Modultest from HART subsystem and HCF Data Linklayer Test scripts
 \n intention: sight check
 \n result  module test: OK
 \n result  Lint Level 3: free // approved warnings and approved infos are suppressed
 \n lint check o.k.: B. Tohermes; 30-08-2010
 \bug
*/
//--------------------------------------------------------------------------------------------------

TUSIGN16 HARTGetAttribute(T_SUBSYSTEM_IDX subsysIdx, TUSIGN16 objIdx, TUSIGN8 attribIdx, void *ptrValue)
{
    TUSIGN16            returnCode;
    T_UNIT const SLOW   *pSub;          // Pointer to Subsystem

    pSub = GetSubsystemPtr(subsysIdx);  // Get the pointer

    returnCode = pSub->Get(pSub,objIdx,attribIdx,(void FAST*)ptrValue);

    return (returnCode);
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief     HARTWriteObjects: Writes all objects and attributes contained in one HART command
 \author    Stefan Tabelander, Horst Seele
 \date      2005-12-06
 \param     cmdDescr = Pointer to command structure.
 \param     objectCount = Number of objects in response.
 \param     configChanged = Indicates if command changes configuration or not.
 \param     command number in transmit buffer.
 \param     data in receive buffer.
 \param     contained in 'commands' table.
 \return    eTRUE = one or more errors occured, eFALSE = no error occured.
 \warning
 \test
 test-date: 2008-04-29
 \n by:     Marcel Schilg
 \n environment: old Modultest from HART subsystem and HCF Data Linklayer Test scripts
 \n intention: sight check
 \n result  module test: OK
 \n result  Lint Level 3: free // approved warnings and approved infos are suppressed
 \n lint check o.k.: B. Tohermes; 30-08-2010
 \bug
*/
//--------------------------------------------------------------------------------------------------
TBOOL HARTWriteObjects(COM_OBJ_DESCR const SLOW *cmdDescr, TUSIGN8 objectCount, TBOOL configChanged, T_LAYER2_BUFFER* pRxBuffer, T_LAYER2_BUFFER* pTxBuffer)
{
    /* variable definition */
    TUSIGN8                 i, element, numOfElements, objId, attribId, attribIdx;
    TUSIGN8                 hartLenCode, idx, bufferIdx, dataTypeId;
    T_SUBSYSTEM_IDX         subsystemId;
    TUSIGN16                errorCode = OK;
    TBOOL                   firstErrorOccured, firstWarningOccured;
    void                    *pobjRxValue;
    COM_OBJ_DESCR           commandDescriptor;

    // Pointer to command structure must be not zero
    VIP_ASSERT(cmdDescr != 0);

    // Initialization
    hartLenCode         = 0;
    dataTypeId          = 0;
    firstErrorOccured   = eFALSE;
    firstWarningOccured = eFALSE;

    // set receive buffer data area start
    bufferIdx = 0;
    if(pTxBuffer->command == EXTENSION_COMMAND_NUMBER)
    {
        bufferIdx = 2;      // if extended command, data area begins after extended command number
    }
    errorCode = OK;

    // For (all attributes of the command)
    for (i = 0; i <= (objectCount-1); i++)
    {
		// Exit loop if error code is an error (not warning)
		if (errorCode >= ERROR_CODES)
		{
			break;
		}
        // Get data for access function
        commandDescriptor = cmdDescr[i];

        subsystemId = commandDescriptor.subsystemId;
        objId       = commandDescriptor.objId;
        attribId    = commandDescriptor.attribId;
        attribIdx   = commandDescriptor.attribIdx;
        dataTypeId  = commandDescriptor.dataTypeId;
        hartLenCode = mapParaLength[commandDescriptor.hartLenCode];

        // reset error code
        //errorCode = OK;
        // reset error code
        errorCode = OK;



        //AccessAllowed is dependant upon ARM
        errorCode = arm.CheckAccessRightsSRV((T_SUBSYSTEM_IDX) HART_IDX,    // CallerId Subsystem index relating to the Client
                                                 (T_SUBSYSTEM_IDX) subsystemId, // Subsystem index relating to the subsystem in which the data is held
                                                 (TUSIGN16) objId,      // Object index for the data in the target subsystem
                                                 (TUSIGN16) attribIdx,  // Attribute id for the data within the target object
                                                 (TUSIGN8) READ_WRITE_ACCESS);

        //in case object is servicecode protected assign responsecode to read error

        if (errorCode)
        {
            errorCode = errorCode;
        }
        else
        {
            // Indicates the position in receive buffer
            pobjRxValue = &(pRxBuffer->aryData[bufferIdx]);

            // Mirror data for response -> already done in L2!

            // If (out of bounds of receive buffer due to erroneous command object description), ...
            if (bufferIdx > (HART_XMIT_RCV_BUFFER_LEN - hartLenCode))
            {
                // set error
                errorCode = WRONG_STATE_ERR;

                // end FOR loop
                i = objectCount;
            }
            // Action functions must be written with a value != 0.  This is done to
            // keep the code compatible to future fieldbus solutions where action
            // functions are usuably written as an enumerated value (zero is not
            // a valid number there)
            else if (hartLenCode == HART_LEN_0)
            {
                // write a value != 0 to perform an action function
                element = 0xFF;

                // write as an attribute
                errorCode = HARTPutAttribute(subsystemId, objId, 0, (void*)&element);
            }
            // Whole object
            else if (attribId == OBJECT)
            {
                // init local buffer index
                idx = bufferIdx;

                switch (dataTypeId)
                {
                    case TD_T_RANGE_FLT_U:
                        for(element = 0; element < 3; element++)
                        {
                            // if (data conversion not possible), WRONG_STATE_ERROR
                            if (!HARTConvertType(t_range_float_u[element].dataTypeId, idx, ACCESS_WRITE, pRxBuffer, pTxBuffer))
                            {
                                errorCode = WRONG_STATE_ERR;
                            }

                            idx = idx + t_range_float_u[element].lenCode;
                        }

                        // if no error) of data conversion, write as object
                        if (errorCode == OK)
                        {
                            errorCode = HARTPutObjects(subsystemId, objId, (void FAST*)pobjRxValue);
                        }

                        break;

                    case TD_PASC_ASC:
                        // shift receive buffer data after PASCII string because ASCII string is longer
                        numOfElements = (TUSIGN8)(hartLenCode * 4/3);   // calculate length of converted ASCII string
                        element = HART_XMIT_RCV_BUFFER_LEN - (bufferIdx + numOfElements);   // length of shift data
                        idx =  bufferIdx + hartLenCode + element - 1;   // last index of shift data
                        numOfElements = numOfElements + bufferIdx;      // new start address of next object
                        for (element = HART_XMIT_RCV_BUFFER_LEN-1; element >= numOfElements; element--, idx--)
                        {
                            pRxBuffer->aryData[element] = pRxBuffer->aryData[idx];
                        }

                        // if (conversion not possible) return WRONG_STATE_ERROR
                        if (!HARTConvertPASCII(hartLenCode, bufferIdx, ACCESS_WRITE, pRxBuffer, pTxBuffer))
                        {
                            errorCode = WRONG_STATE_ERR;
                        }
                        // else write attribute
                        else
                        {
                            errorCode = HARTPutObjects(subsystemId, objId, (void FAST*)pobjRxValue);
                        }

                        // correct receive buffer start address of next object
                        bufferIdx = numOfElements - hartLenCode;    // hartLenCode added at end of FOR loop

                        break;
                    case TD_SPECIAL_U32:
                        //Special TD_USIGN32 for mapping to old TZIDC
                        //In old code the object will not rotated the bytes
                        errorCode = HARTPutObjects(subsystemId, objId, (void FAST*)pobjRxValue);
                        break;
                    case TD_UNITTYPE:
                        {
                            TUSIGN8 sysUnitType;
                            TUSIGN8 hartUnitType;
                            
                            hartUnitType = *(TUSIGN8 SLOW *)pobjRxValue;
                            if (!HartConvUnitTypeFromHartUnit_HART(&hartUnitType, &sysUnitType))
                            {
                                errorCode = WRONG_STATE_ERR;
                            }
                            else
                            {
                                errorCode = HARTPutObjects(subsystemId, objId, &sysUnitType);
                            } 
                        }
                    break;
                    
                    case TD_DOUBLE:
                        {                    
                            if (!HARTConvertType(EIGHT_BYTES,bufferIdx,ACCESS_WRITE, pRxBuffer, pTxBuffer))
                            {
                                errorCode = WRONG_STATE_ERR;
                            }
                            else
                            {
                                errorCode = HARTPutObjects(subsystemId, objId, pobjRxValue);
                            } 
                        }
                        break;                    
                    
                    default:
                        // Check for arrays
                        numOfElements = hartLenCode / dataTypeLength[dataTypeId];

                        // Length code and Data type mismatch
                        VIP_ASSERT((hartLenCode % dataTypeLength[dataTypeId]) == 0);

                        if( numOfElements > 1 )
                        {
                            for(element = 0; element < numOfElements; element++)
                            {
                                // if (data conversion not possible), WRONG_STATE_ERROR
                                if (!HARTConvertType(dataTypeId, idx, ACCESS_WRITE, pRxBuffer, pTxBuffer))
                                {
                                    errorCode = WRONG_STATE_ERR;
                                }

                                idx = idx + dataTypeLength[dataTypeId];
                            }
                        }
                        else
                        {
                            // if (data conversion not possible), WRONG_STATE_ERROR
                            if (!HARTConvertType(dataTypeId, idx, ACCESS_WRITE, pRxBuffer, pTxBuffer))
                            {
                                errorCode = WRONG_STATE_ERR;
                            }
                        }

                        // if no error) of data conversion, write as object
                        if (errorCode == OK)
                        {
                            errorCode = HARTPutObjects(subsystemId, objId, (void FAST*)pobjRxValue);
                            
                            if((errorCode == SPAN_TO_SMALL)&&(pTxBuffer->command == 35))
                            {
                              pTxBuffer->firstResp = HART_RESP_WARN_SET_TO_NEAREST_VALUE;
                            }
                        }

                        break;
                }
            }
            // Write attribut
            else
            {
                // if (HART data format is PASCII) convert PASCII to ASCII
                if (dataTypeId == TD_PASC_ASC)
                {
                    // shift receive buffer data after PASCII string because ASCII string is longer
                    numOfElements = (TUSIGN8)(hartLenCode * 4/3);   // calculate length of converted ASCII string
                    element = HART_XMIT_RCV_BUFFER_LEN - (bufferIdx + numOfElements);   // length of shift data
                    idx =  bufferIdx + hartLenCode + element - 1;   // last index of shift data
                    numOfElements = numOfElements + bufferIdx;      // new start address of next object
                    for (element = HART_XMIT_RCV_BUFFER_LEN-1; element >= numOfElements; element--, idx--)
                    {
                        pRxBuffer->aryData[element] = pRxBuffer->aryData[idx];
                    }

                    // if (data conversion not possible), WRONG_STATE_ERROR
                    if (!HARTConvertPASCII(hartLenCode, bufferIdx, ACCESS_WRITE, pRxBuffer, pTxBuffer))
                    {
                        errorCode = WRONG_STATE_ERR;
                    }
                    // else write attribute
                    else
                    {
                        errorCode = HARTPutAttribute(subsystemId, objId, attribIdx, (void FAST*)pobjRxValue);
                    }

                    // correct receive buffer start address of next object
                    bufferIdx = numOfElements - hartLenCode;    // hartLenCode added at end of FOR loop
                }

                //Added 20110425 for Unit Convern between HART-Unit and System Unit
                else if(dataTypeId == TD_UNIT)
                {
                    //TODO TODO
                    TUSIGN8 unitCode, unitsType;
                    //assert(attribIdx == UOM_PF_unit);// TD_UNIT was not stated at an attribute UOM_PF_unit
                    //preload
                    errorCode = WRONG_STATE_ERR;
                    unitCode = *((TUSIGN8 SLOW*)pobjRxValue);//0x11
                    unitsType = UM_T_UNKNOWN_UNIT_GROUP;//search all groups
                    // caution: the following attribute after the unit must be the unitsType !!!
                    if( (TUSIGN16)ERROR_CODES > HARTGetAttribute(subsystemId, objId, attribIdx+1, &unitsType) )//return 0x00
                    {
                        //a write of HART_UM_UKNOWN_UNIT ...
                        if( unitCode == HART_UM_UKNOWN_UNIT )
                        {
                            //...shall be not possible
 //                           errorCode = UPPER_RANGE_TOO_LOW_ERR;//TODO response code 12 ?//del temp by spring
                              errorCode = RULE_VIOLATION_ERR;// add temp by spring
                        }
                        else
                        {
                            if( HartConvUnitCodeFromHartUnit_HART( &unitCode, &unitsType ) )
                            {
                                // write attribute if no conversion error
                                errorCode = HARTPutAttribute(subsystemId, objId, attribIdx, (void *)&unitCode);
                            }
                            else//test by spring
                            {
                                errorCode = RULE_VIOLATION_ERR;// add temp 
                            }
                        }
                        	//change date:2014.4.23
						//add specific error code for invalid unit code;
						//special handling for CMD 44 (HART_RESP_ERR_INVALID_SELECTION), 53 (HART_RESP_ERR_UPPER_RANGE_TOO_LOW);
						if(errorCode != OK)
						{
							if(transmitBuffer_LAYER7.command == 44)
							{
								errorCode = RULE_VIOLATION_ERR;
							}
							if(transmitBuffer_LAYER7.command == 53)
							{
								// Return 12 for command 53
								errorCode = UPPER_RANGE_TOO_LOW_ERR;
							}

						}    
                              
                    }
                }
                else if( dataTypeId == TD_UNITTYPE)
                {
                    TUSIGN8 sysUnitType;
                    TUSIGN8 hartUnitType;
                    
                    hartUnitType = *(TUSIGN8 SLOW *)pobjRxValue;
                    if (!HartConvUnitTypeFromHartUnit_HART(&hartUnitType, &sysUnitType))
                    {
                        errorCode = WRONG_STATE_ERR;
                    }
                    else
                    {
                        errorCode = HARTPutAttribute(subsystemId, objId,attribIdx, &sysUnitType);
                    } 
                }
                else if( dataTypeId == TD_DOUBLE)
                {                    
                    if (!HARTConvertType(EIGHT_BYTES,bufferIdx,ACCESS_WRITE, pRxBuffer, pTxBuffer))
                    {
                        errorCode = WRONG_STATE_ERR;
                    }
                    else
                    {
                        errorCode = HARTPutAttribute(subsystemId, objId, attribIdx, pobjRxValue);
                    } 
                }
                   
                // else normal convert
                else
                {
                    // if (data conversion not possible), WRONG_STATE_ERROR
                    if (!HARTConvertType(dataTypeId, bufferIdx, ACCESS_WRITE, pRxBuffer, pTxBuffer))
                    {
                        errorCode = WRONG_STATE_ERR;
                    }
                    // else write attribute
                    else
                    {
                        // write attribute if no conversion error
                        errorCode = HARTPutAttribute(subsystemId, objId, attribIdx, (void FAST*)pobjRxValue);
                    }
                }
            }

            // Indicate to position from where to write next value
            bufferIdx = bufferIdx + hartLenCode;
        }
        // Map T_UNIT error and warning code to HART response code (first byte)
        if (errorCode && (!firstErrorOccured))
        {
            // Limit errorCode to prevent mapFirstResponse[] from an overrun.
            // In a proper system this errors shall not appear.
            if (errorCode > WRONG_STATE_ERR)
            {
                errorCode = WRONG_STATE_ERR;
            }

            if (errorCode >= WRITE_ERROR)
            {
                // An error code will always overwrite a warning.  Only the first error that occurs
                // will be reported
                pTxBuffer->firstResp = mapErrFirstResponse[errorCode-WRITE_ERROR];
                pTxBuffer->numByte   = 2;
                if(pTxBuffer->command == EXTENSION_COMMAND_NUMBER)
                {
                    pTxBuffer->numByte = 4;
                }

                // The first error that occurs in a command will be returned.
                firstErrorOccured = eTRUE;
            }
            else if (!firstWarningOccured)
            {
                // Limit errorCode to prevent mapFirstResponse[] from an overrun.
                // In a proper system this errors shall not appear.
                if (errorCode > WRONG_STATE)
                {
                    errorCode = WRONG_STATE;
                }

                // Get response code from response mapping table
                pTxBuffer->firstResp = mapWarnFirstResponse[errorCode];

                // The first warning that occurs in a command will be returned.
                firstWarningOccured = eTRUE;
            }
            else
            {
            }
        }
    }
    if (!errorCode) // Writing was successful -> set config changed flag
    {
        // Determine if configuration changed flag must be set
        if (configChanged == CONF_YES)
        {
            // configuration change handling
            HART_ConfigurationChange();
        }
    }

    return(firstErrorOccured);
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief     Writes an object to a subsystem
            Add type cast (void FAST*)ptrValue
 \author    Stefan Tabelander,Eric-Jiping.li
 \date      2004-07-27,2011-04-12
 \param
    <pre>
       subsysIdx:       Subsystem to be addressed
       objIdx:          Object index where to find the parameter
       ptrValue:        data pointer
    <\pre>
 \return    for return code see T_DATA_OBJ_RETURNCODES
 \warning
 \test
 test-date: 2008-04-29
 \n by:     Marcel Schilg
 \n environment: old Modultest from HART subsystem and HCF Data Linklayer Test scripts
 \n intention: sight check
 \n result  module test: OK
 \n result  Lint Level 3: free // approved warnings and approved infos are suppressed
 \n lint check o.k.: B. Tohermes; 30-08-2010
 \bug
*/
//--------------------------------------------------------------------------------------------------

TUSIGN16 HARTPutObjects(T_SUBSYSTEM_IDX subsysIdx, TUSIGN16 objIdx, void *ptrValue)
{
    TUSIGN16            returnCode;
    T_UNIT const SLOW   *pSub;          // Pointer to Subsystem

    pSub = GetSubsystemPtr(subsysIdx);  // Get the pointer

    returnCode = pSub->Put(pSub,objIdx,WHOLE_OBJECT,(void FAST*)ptrValue);

    return (returnCode);
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief     Writes an attribute to a subsystem
            Add type cast (void FAST*)ptrValue
 \author    Stefan Tabelander,Eric-Jiping.li
 \date      2004-07-27,2011-04-12
 \param
    <pre>
       subsysIdx:       Subsystem to be addressed
       objIdx:          Object index where to find the parameter
       attribIdx:       subindex in object 'objIdx'
       ptrValue:        data pointer
    <\pre>
 \return    for return code see T_DATA_OBJ_RETURNCODES
 \warning
 \test
 test-date: 2008-04-29
 \n by:     Marcel Schilg
 \n environment: old Modultest from HART subsystem and HCF Data Linklayer Test scripts
 \n intention: sight check
 \n result  module test: OK
 \n result  Lint Level 3: free // approved warnings and approved infos are suppressed
 \n lint check o.k.: B. Tohermes; 30-08-2010
 \bug
*/
//--------------------------------------------------------------------------------------------------

TUSIGN16 HARTPutAttribute(T_SUBSYSTEM_IDX subsysIdx, TUSIGN16 objIdx, TUSIGN8 attribIdx, void *ptrValue)
{

    TUSIGN16            returnCode;
    T_UNIT SLOW const   *pSub;          // Pointer to Subsystem

    pSub = GetSubsystemPtr(subsysIdx);  // Get the pointer

    returnCode = pSub->Put(pSub,objIdx,attribIdx,(void FAST*)ptrValue);

    return (returnCode);
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief     Data type conversion function
 \author    Stefan Tabelander, Horst Seele
 \date      2005-12-06
 \param     dataTypeId:      type of data to be converted
 \param     bufferIdx:       index in transmit or receive buffer where to find the data
 \param     direction:       read or write
 \return    conversion will be done in data buffer directly at 'bufferIdx'
 \return    eTRUE = Conversion ok, eFALSE = conversion not possible
 \warning
 \test
 test-date: 2008-04-29
 \n by:     Marcel Schilg
 \n environment: old Modultest from HART subsystem and HCF Data Linklayer Test scripts
 \n intention: sight check
 \n result  module test: OK
 \n result  Lint Level 3: free // approved warnings and approved infos are suppressed
 \n lint check o.k.: B. Tohermes; 30-08-2010
 \bug
*/
//--------------------------------------------------------------------------------------------------
TBOOL HARTConvertType(TUSIGN8 dataTypeId, TUSIGN8 bufferAdr, TUSIGN8 direction, T_LAYER2_BUFFER* pRxBuffer, T_LAYER2_BUFFER* pTxBuffer)
{
    TBOOL   errorState;
    TUSIGN8 destination[8];
    TUSIGN8      *bufferPtr;

    // return no conversion fault
    errorState = eTRUE;

    // Information contained in transmit or receive buffer
    if(direction == ACCESS_READ)
    {
        bufferPtr = &(pTxBuffer->aryData[bufferAdr]);
    }
    else
    {
        bufferPtr = &(pRxBuffer->aryData[bufferAdr]);
    }

    // Convert data format
    switch (dataTypeId)
    {
       // two byte types
        case TWO_BYTES:
            // convert if not out of bounds of transmit/receive buffer
            if (bufferAdr < (HART_XMIT_RCV_BUFFER_LEN-1u))
            {
                destination[1] = bufferPtr[0];
                destination[0] = bufferPtr[1];

                bufferPtr[0] = destination[0];
                bufferPtr[1] = destination[1];
            }
            else
            {
                // return conversion fault
                errorState = eFALSE;
            }
            break;

        // four byte types
        case FOUR_BYTES:
            // convert if not out of bounds of transmit/receive buffer
            if (bufferAdr < (HART_XMIT_RCV_BUFFER_LEN-3u))
            {
                destination[3] = bufferPtr[0];
                destination[2] = bufferPtr[1];
                destination[1] = bufferPtr[2];
                destination[0] = bufferPtr[3];

                bufferPtr[0] = destination[0];
                bufferPtr[1] = destination[1];
                bufferPtr[2] = destination[2];
                bufferPtr[3] = destination[3];
            }
            else
            {
                // return conversion fault
                errorState = eFALSE;
            }

            break;
        // eight byte types double precision.
        case EIGHT_BYTES:
            // convert if not out of bounds of transmit/receive buffer
            if (bufferAdr < (HART_XMIT_RCV_BUFFER_LEN-7u))
            {
                destination[7] = bufferPtr[0];
                destination[6] = bufferPtr[1];
                destination[5] = bufferPtr[2];
                destination[4] = bufferPtr[3];
                destination[3] = bufferPtr[4];
                destination[2] = bufferPtr[5];
                destination[1] = bufferPtr[6];
                destination[0] = bufferPtr[7];

                bufferPtr[0] = destination[0];
                bufferPtr[1] = destination[1];
                bufferPtr[2] = destination[2];
                bufferPtr[3] = destination[3];
                bufferPtr[4] = destination[4];
                bufferPtr[5] = destination[5];
                bufferPtr[6] = destination[6];
                bufferPtr[7] = destination[7];
            }
            else
            {
                // return conversion fault
                errorState = eFALSE;
            }

            break;

        default:
            // if out of bounds of transmit/receive buffer, return error
            if (bufferAdr > (HART_XMIT_RCV_BUFFER_LEN-1u))
            {
                errorState = eFALSE;
            }

            break;
    }

    return (errorState);
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief     HARTConvertPASCII: Packed ASCII conversion function
 \author    Horst Seele
 \date      2005-12-07
 \param     length:     HART commando length of data to be converted
 \param     bufferIdx:  index in transmit or receive buffer where to read/write the data
 \param     direction:  read or write
 \return    conversion will be done in data buffer directly at 'bufferIdx'
 \return    eTRUE = Conversion ok, eFALSE = conversion not possible
 \warning   Because the conversion from PASC to ASC needs 1 Byte more every 3 PASC characters there
 \warning   must be enough free space in the receive buffer after the PASC string to convert him
 \warning   there (Only in WRITE direction!).
 \test
 test-date: 2008-04-29
 \n by:     Marcel Schilg
 \n environment: old Modultest from HART subsystem and HCF Data Linklayer Test scripts
 \n intention: sight check
 \n result  module test: OK
 \n result  Lint Level 3: free // approved warnings and approved infos are suppressed
 \n lint check o.k.: B. Tohermes; 30-08-2010
 \bug
*/
//--------------------------------------------------------------------------------------------------

TBOOL HARTConvertPASCII(TUSIGN8 length, TUSIGN8 bufferIdx, TUSIGN8 direction, T_LAYER2_BUFFER* pRxBuffer, T_LAYER2_BUFFER* pTxBuffer)
{
    TUSIGN8         n;
    TINT8           i;
    TUSIGN8         *stringBeg;

    // store length of PASCII string
    n = length;

    // calculate length of ASCII string
    length = (TUSIGN8)(length * 4)/3;

    // if (PASCII string length > 2 and PASCII string length multiple of 3) and
    //     receive or transmit buffer not too short for conversion), ...
    if ((n > 2) && ((n % 3) == 0) && ((bufferIdx + length) <= HART_XMIT_RCV_BUFFER_LEN))
    {
        // if (read), ...
        if(direction == ACCESS_READ)
        {
            // start conversion at index
            stringBeg = &(pTxBuffer->aryData[bufferIdx]);

            // for (not all possible ASCII bytes converted in Packed-ASCII format)
            for ((i = 0), (n = 0); i <= (length-4); i = (i+4), n = (n+3))
            {
                // format 4 bytes ASCII format in 3 bytes HART Packed-ASCII format
                stringBeg[n] = ((stringBeg[i] & 0x3f) << 2) | ((stringBeg[i+1] & 0x30) >> 4);
                stringBeg[n+1] = ((stringBeg[i+1] & 0x0f) << 4) | ((stringBeg[i+2] & 0x3c) >> 2);
                stringBeg[n+2] = ((stringBeg[i+2] & 0x03) << 6) | (stringBeg[i+3] & 0x3f);
            }
        }
        // else write, ...
        else
        {
            // start conversion at index
            stringBeg = &(pRxBuffer->aryData[bufferIdx]);

            // for (not all possible Packed-ASCII bytes converted in ASCII format)
            for (i = (TINT8)length-1, n = n-1; i >= 0; i = i-4, n = n-3)
            {
                // format 3 bytes HART Packed-ASCII format in 4 bytes ASCII format
                stringBeg[i] = stringBeg[n] & 0x3f;
                if (stringBeg[i] < 0x20)             // shift PASCII codes, 0x00 - 0x1f to ASCII 0x40 - 0x5f
                    stringBeg[i] += 0x40;
                stringBeg[i-1] = ((stringBeg[n-1] & 0x0f) << 2) | ((stringBeg[n] & 0xc0) >> 6);
                if (stringBeg[i-1] < 0x20)             // shift PASCII codes, 0x00 - 0x1f to ASCII 0x40 - 0x5f
                    stringBeg[i-1] += 0x40;
                stringBeg[i-2] = ((stringBeg[n-2] & 0x03) << 4) | ((stringBeg[n-1] & 0xf0) >> 4);
                if (stringBeg[i-2] < 0x20)             // shift PASCII codes, 0x00 - 0x1f to ASCII 0x40 - 0x5f
                    stringBeg[i-2] += 0x40;
                stringBeg[i-3] = (stringBeg[n-2] & 0xfc) >> 2;
                if (stringBeg[i-3] < 0x20)             // shift PASCII codes, 0x00 - 0x1f to ASCII 0x40 - 0x5f
                    stringBeg[i-3] += 0x40;
            }
        }
        // return conversion ok
        return (eTRUE);
    }
    // else return conversion error
    {
        return (eFALSE);
    }
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     HARTReadWriteBurstObjects: Handles Read/Write of HART burst commands (103-105, 107-108)
 \author    Bernhard Tohermes
 \date      2010-02-09
 \param     commandNumberIndex = Index of command in command table.
 \param     contained in command descriptions
 \return    HART command response
 \warning
 \test
 test-date:
 \n by:
 \n environment:
 \n lint check o.k.: B. Tohermes; 30-08-2010
 \bug
*/
//--------------------------------------------------------------------------------------------------

void HARTReadWriteBurstObjects(TUSIGN16 commandNumberIndex, TUSIGN8 burstMsgNumber, TUSIGN8 cmdFunction, T_LAYER2_BUFFER* pRxBuffer, T_LAYER2_BUFFER* pTxBuffer)
{
    TUSIGN8                         idx, tableIdx, objectCount;
    COM_SPEC_TAB  const SLOW        *ptrSpecialCmdDescr;
    COM_REQ_RESP_CNT const SLOW     *reqRspObjCntPtr;

    // commandNumberIndex must be valid
    VIP_ASSERT(commandNumberIndex < lengthOfCommandTable);

    // get command description pointer from command table
    ptrSpecialCmdDescr = (void const SLOW *)commands[commandNumberIndex].cmdDescr;
    // command description pointer must be not equal zero
    VIP_ASSERT(ptrSpecialCmdDescr);

    // get pointer to table of request/response object count
    reqRspObjCntPtr = (void const SLOW *)ptrSpecialCmdDescr->reqRespObjCnt;
    // pointer must be not equal zero
    VIP_ASSERT(reqRspObjCntPtr);

    // number of entries in table of request/response object count
    tableIdx = ptrSpecialCmdDescr->lenOfRespObjCntTab;

    // if special write command
    if (cmdFunction == HART_SPECIAL_WRITE)
    {
        // number of request objects
        objectCount = reqRspObjCntPtr[tableIdx - 1].noReqObjects;
        // copy default request command objects corresponding to object counter into RAM
        //lint -e(619) B. Tohermes 2010-08-18: precision of arg. no. 2 o.k.
        (void)memcpy(objComSpecCmd,
                     ptrSpecialCmdDescr->cmdDefReqDescr,
                     (sizeof(COM_OBJ_DESCR) * objectCount));
    }
    // else special read command
    else
    {
        // number of response objects
        objectCount = reqRspObjCntPtr[tableIdx - 1].noRespObjects;

        // copy default response command objects corresponding to number of response objects into RAM
        //lint -e(619) B. Tohermes 2010-08-18: precision of arg. no. 2 o.k.
        (void)memcpy(objComSpecCmd,
                     ptrSpecialCmdDescr->cmdDefRespDescr,
                     (sizeof(COM_OBJ_DESCR) * objectCount));
    }

    // for all specific objects of command
    for (idx = 0; idx < (objectCount); idx++)
    {
        // if not object id of object with total number of possible burst messages
        if (objComSpecCmd[idx].objId != HART_IDX_bursttotalNumberOfMsg)
        {
            // exchange default object id with burst message number specific object id
            objComSpecCmd[idx].objId = burstMsgConfObjRefTable[burstMsgNumber].confObjId;
        }
    }

    // if special write command
    if (cmdFunction == HART_SPECIAL_WRITE)
    {
        // write data into objects
        (void)HARTWriteObjects(objComSpecCmd, objectCount, commands[commandNumberIndex].configChanged, pRxBuffer, pTxBuffer);

        // set response data length (length equal to request data length)
        pTxBuffer->numByte += reqRspObjCntPtr[tableIdx - 1].noReqDatabytes;
    }
    // else special read command
    else
    {
        // read objects and store in response
        HARTReadObjects(objComSpecCmd, objectCount, pRxBuffer, pTxBuffer);
    }
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief     HARTReadObjects: Read objects subscribed over slot numbers
 \author    Bernhard Tohermes
 \date      2010-02-24
 \param     cmdNumberIndex = Index of command in command table.
 \param     contained in command descriptions
 \return    HART command response
 \warning
 \test
 test-date:
 \n by:
 \n environment:
 \n lint check o.k.: B. Tohermes; 30-08-2010
 \n result  module test:
 \n result
*/
//--------------------------------------------------------------------------------------------------

void HARTReadSubscribedObjects (TUSIGN16 cmdNumber, T_LAYER2_BUFFER* pRxBuffer, T_LAYER2_BUFFER* pTxBuffer)
{

    COM_SPEC_TAB  const SLOW    *ptrSpecialCmdDescr;
    TUSIGN8                     objCntPerSlot;
    TUSIGN8 const SLOW          *slotIdxTabPtr;
    TUSIGN16                    cmdNumberIndex;

    // Search for command table index
    cmdNumberIndex = HARTFindCmdTableIndex(cmdNumber);

    // Command not implemented?
    if (cmdNumberIndex == INVALID_CMD_NUMBER_INDEX)
    {
        // Set command not implemented response code in 1. response byte
        pTxBuffer->firstResp = HART_RESP_ERR_CMD_NOT_IMPLEMENTED;
    }
    else
    {
        // get command description pointer from command table
        ptrSpecialCmdDescr = (void const SLOW *)commands[cmdNumberIndex].cmdDescr;
        // command description pointer must be not equal zero
        VIP_ASSERT(ptrSpecialCmdDescr);

        // get no. of object descriptions for every slot
        objCntPerSlot = ptrSpecialCmdDescr->noOfSlotObjects;

        // must be > 0
        VIP_ASSERT(objCntPerSlot);

        // get pointer to slotcode table
        slotIdxTabPtr = ptrSpecialCmdDescr->slotCodeTranslationTable;
        // must be > 0
        VIP_ASSERT (slotIdxTabPtr);

        // if device works compliant to hart rev 5
        if (slotIdxTabPtr[0] == hartFreqCst.univCmdRev)
        {
            // prepare hart rev 5 specific cmd objects
            //lint -e(619) B. Tohermes 2010-08-18: precision of arg. no. 2 o.k.
            (void)memcpy(objComSpecCmd,
                         ptrSpecialCmdDescr->slotCodeObjDescr,
                         (sizeof(COM_OBJ_DESCR) * objCntPerSlot));
        }
        // else device works compliant to hart rev 7
        else
        {
            // wrong relation of hart revision to slot no.
            VIP_ASSERT(slotIdxTabPtr[1] == hartFreqCst.univCmdRev);

            // prepare hart rev 7 specific cmd objects
            //lint -e(619) B. Tohermes 2010-08-18: precision of arg. no. 2 o.k.
            (void)memcpy(objComSpecCmd,
                         ptrSpecialCmdDescr->slotCodeObjDescr + objCntPerSlot,
                         (sizeof(COM_OBJ_DESCR) * objCntPerSlot));
        }

        // read objects and prepare as response data
        HARTReadObjects(objComSpecCmd, objCntPerSlot, pRxBuffer, pTxBuffer);
    }
}

