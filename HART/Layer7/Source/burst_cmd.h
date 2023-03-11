//--------------------------------------------------------------------------------------------------
/*
 Copyright      Copyright ABB, 2011.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Subsystem Hart
 Module
 Description    Access right management

 Remarks


*/
//--------------------------------------------------------------------------------------------------


#ifndef __COMMON_TYPE_H__
  #error common_type.h must be include before layer2_attributes.h
#endif


#ifdef __BURST_CMD_H__
  #error burst_cmd.h included more than once
#endif


#define __BURST_CMD_H__

// command 103 request/response structure
typedef struct
{
    TUSIGN8     burstMsgNum;
    TUSIGN32    updatePeriod;
    TUSIGN32    updatePeriodMax;
} CMD_103_REQ_RESP_DATA;


// command 104 request/response structure
typedef struct
{
    TUSIGN8     burstMsgNum;
    TUSIGN8     burstTriggerMode;
    TUSIGN8     deviceVariableClassForTriggerLevel;
    TUSIGN8     unitsCode;
    TFLOAT      triggerLevel;
} CMD_104_REQ_RESP_DATA;

// command 105 requeststructure
typedef struct
{
    TUSIGN8     burstMsgNum;
} CMD_105_REQ_DATA;

// command 105 request/response structure
typedef struct
{
    TUSIGN8     burstModeControlCode;
    TUSIGN8     cmdNumberExpansionFlag;
    TUSIGN8     burstSlot0;
    TUSIGN8     burstSlot1;
    TUSIGN8     burstSlot2;
    TUSIGN8     burstSlot3;
    TUSIGN8     burstSlot4;
    TUSIGN8     burstSlot5;
    TUSIGN8     burstSlot6;
    TUSIGN8     burstSlot7;
    TUSIGN8     burstMsgNum;
    TUSIGN8     maxNumberOfBurstMsg;
    TUSIGN16    extendedCmdNumber;
    TUSIGN32    updatePeriod;
    TUSIGN32    updatePeriodMax;
    TUSIGN8     burstTriggerMode;
    TUSIGN8     deviceVariableClassForTriggerLevel;
    TUSIGN8     unitsCode;
    TFLOAT      triggerLevel;
} CMD_105_RESP_DATA;

// command 107 request/response structure
typedef struct
{
    TUSIGN8     burstSlot[8];
    TUSIGN8     burstMsgNum;
} CMD_107_REQ_RESP_DATA;

// command 108 request/response structure
typedef struct
{
    TUSIGN16    cmdNumber;
    TUSIGN8     burstMsgNum;
} CMD_108_REQ_RESP_DATA;

// command 109 request/response structure
typedef struct
{
    TUSIGN8     burstModeControlCode;
    TUSIGN8     burstMsgNum;
} CMD_109_REQ_RESP_DATA;

// buffer of burst message specific config data objects
extern T_BURSTMSG_CONFVALUES   burstMsgConfValues;


//--------------------------------------------------------------------------------------------------
/*!
 \brief     burst command 103 execution
 \author    Bernhard Tohermes
 \date      2010-06-28
 \param
 \warning
 \test
 test-date:
 \n by:
 \n environment:
 \n intention:
 \n result
 \bug
*/
//--------------------------------------------------------------------------------------------------

void HARTCommand_103(T_LAYER2_BUFFER* pRxBuffer, T_LAYER2_BUFFER* pTxBuffer);



//--------------------------------------------------------------------------------------------------
/*!
 \brief     burst command 104 execution
 \author    Bernhard Tohermes
 \date      2010-06-28
 \param
 \warning
 \test
 test-date:
 \n by:
 \n environment:
 \n intention:
 \n result
 \bug
*/
//--------------------------------------------------------------------------------------------------
void HARTCommand_104(T_LAYER2_BUFFER* pRxBuffer, T_LAYER2_BUFFER* pTxBuffer);



//--------------------------------------------------------------------------------------------------
/*!
 \brief     burst command 105 execution
 \author    Bernhard Tohermes
 \date      2010-06-29
 \param
 \warning
 \test
 test-date:
 \n by:
 \n environment:
 \n intention:
 \n result
 \bug
*/
//--------------------------------------------------------------------------------------------------
void HARTCommand_105(T_LAYER2_BUFFER* pRxBuffer, T_LAYER2_BUFFER* pTxBuffer);



//--------------------------------------------------------------------------------------------------
/*!
 \brief     burst command 107 execution
 \author    Bernhard Tohermes
 \date      2010-06-28
 \param
 \warning
 \test
 test-date:
 \n by:
 \n environment:
 \n intention:
 \n result
 \bug
*/
//--------------------------------------------------------------------------------------------------
void HARTCommand_107(T_LAYER2_BUFFER* pRxBuffer, T_LAYER2_BUFFER* pTxBuffer);



//--------------------------------------------------------------------------------------------------
/*!
 \brief     burst command 108 execution
 \author    Bernhard Tohermes
 \date      2010-06-28
 \param
 \warning
 \test
 test-date:
 \n by:
 \n environment:
 \n intention:
 \n result
 \bug
*/
//--------------------------------------------------------------------------------------------------
void HARTCommand_108(T_LAYER2_BUFFER* pRxBuffer, T_LAYER2_BUFFER* pTxBuffer);



//--------------------------------------------------------------------------------------------------
/*!
 \brief     burst command 109 execution
 \author    Bernhard Tohermes
 \date      2010-06-28
 \param
 \warning
 \test
 test-date:
 \n by:
 \n environment:
 \n intention:
 \n result
 \bug
*/
//--------------------------------------------------------------------------------------------------
void HARTCommand_109(T_LAYER2_BUFFER* pRxBuffer, T_LAYER2_BUFFER* pTxBuffer);

