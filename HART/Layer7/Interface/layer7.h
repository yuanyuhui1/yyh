//--------------------------------------------------------------------------------------------------
/*
 Copyright      Copyright ABB, 2011.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Common Components HART
 Module
 Description    Interface Layer 7

 Remarks

*/
//--------------------------------------------------------------------------------------------------


#ifndef __COMMON_TYPE_H__
  #error common_type.h must be include before layer2_attributes.h
#endif

#ifndef __LAYER2_H__
  #error layer2.h must be include before layer7.h
#endif


#ifdef __LAYER7_H__
  #error layer7.h included more than once
#endif

#define __LAYER7_H__

// Definition of extension command number
#define EXTENSION_COMMAND_NUMBER    31

// Definition of not used slot code
#define SLOT_CODE_NONE  250

// Definition of Max. Slot Number
#define MAX_SLOT_NUMBER 8

// Definition of LAYER 2 request codes
#define NO_LAYER2_REQ   0x00        // No Layer 2 request of Layer 7
#define FIRST_LAYER2    0x01        // First Layer 2 requests Layer 7
#define SECOND_LAYER2   0x02        // Second Layer 2 requests Layer 7

typedef struct
{
    TUSIGN16                        confObjId;
    const T_BURSTMSG_CONFVALUES     *burstMsgConfObjPtr;
}BURST_MSG_CONF_OBJECT;

typedef struct
{
    TUSIGN16                  manageObjId;
    T_BURSTMSG_MANAGEVALUES   *burstMsgManageObjPtr;
}BURST_MSG_MANAGE_OBJECT;

typedef enum
{
    SP_BUFFER_STATE_FREE,
    SP_BUFFER_STATE_RECEIVED,
    SP_BUFFER_STATE_WAIT_SEND
}E_STATE_SP_BUFFER;
// Shows Layer 7 request from first or second Layer 2
extern __no_init TUSIGN8  requestsLayer2_LAYER7;

// Receive and transmit buffer of layer 7
extern __no_init T_LAYER2_BUFFER transmitBuffer_LAYER7;
extern __no_init T_LAYER2_BUFFER receiveBuffer_LAYER7;

extern __no_init T_LAYER2_BUFFER HART_SPtransmitBuffer;
extern __no_init T_LAYER2_BUFFER HART_SPreceiveBuffer;

extern  E_STATE_SP_BUFFER HART_SPbufferState;

extern __no_init FAST TUSIGN8 performDeviceReset;
// squawk counter ~ 1 minute
extern TUSIGN16     squawkCounter;

// counter of burst messages with elapsed max update period
extern TUSIGN8      counterBurstMsgToSend;

// squawk timer value = 1 minute (msec)
#define SQUAWK_TIME      60000

// address table for max. possible burst messages
extern const BURST_MSG_CONF_OBJECT burstMsgConfObjRefTable[];
extern const BURST_MSG_MANAGE_OBJECT burstMsgManageObjRefTable[];

// HART Device Address
extern TUSIGN8 s_addr;
extern TUSIGN8 s_addrLong[5];

// HART Task Handler
extern RTOS_TASK_HANDLE HartTskHandle;
//--------------------------------------------------------------------------------------------------
/*!
 \brief  Command interpreter
 \author Stefan Tabelander
 \date   2004-06-30
 \param  -
 \return -
 \warning
 \test
 \bug
*/
//--------------------------------------------------------------------------------------------------
void HARTLayer7CommandInterpreter(void);

