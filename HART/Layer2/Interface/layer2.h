//--------------------------------------------------------------------------------------------------
/*
 Copyright      Copyright ABB, 2011.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Subsystem Hart
 Module
 Description    interface Layer 2

 Remarks

*/
//--------------------------------------------------------------------------------------------------


#ifndef __COMMON_TYPE_H__
  #error common_type.h must be include before layer2_attributes.h
#endif


#ifdef __LAYER2_H__
  #error layer2.h included more than once
#endif

#define __LAYER2_H__

// @@adjust
// Activation of second layer 2 implementation
// A second layer 2 implementation may resemble the ordinary layer2 in most parts.
// Due to the wide differences in device implementations there is no common second layer 2 implemented here
// A possible implementation is known from the Contrac Actuator project (DE Minden)
// This second layer 2 features also an automatic baud rate detection that allows to automatically
// find baud rates from 1200 to 115200.


// @@adjust
// In this implemenation the soft modem is used
//#define MSP_430_SOFT_MODEM// not used in UHTE

// @@adjust
// In this implemenation the soft modem is used
// In other implementations Layer1 functions must be implemented here

//#define SOFT_MODEM_CLK_SOURCE()             {                   
//                                                TA4MR=0x04;     
//                                                TA4=0;          
//                                                PD8_bit.PD8_0=1;
//                                                TABSR|=0x10;    
//                                            }
//#define SEND_REQUEST_TO_SEND                (P6&=~0x10)
//#define RELEASE_REQUEST_TO_SEND             (P6|=0x10)
//#define RTS_PORT_DIRECTION                  (PD6|=0x10)
//#define RESET_SOFT_MODEM                    (P7&=~0x20)
//#define PORT_DIRECTION_RESET_SM             (PD7|=0x21)
//#define START_SOFT_MODEM                    (P7|=0x20)


// @@adjust
// Example from EDP300 implementation
//#define CARRIER_DETECTED                    ((P7&0x04)!=0 || uLayer2flags.fskDisabled==1)

// @@adjust
// only if necessary
#define HARTL2_IRQ_LEVEL                    (0x05)

// @@adjust USART hardware according to your system
//Comment
//#define RECEIVE_BUFFER_HI                   (U2RBH)
//#define RECEIVE_BUFFER_LO                   (U2RBL)
//#define BUFFER_CONTROL_REG                  (U2C1)
//#define TRANSMIT_BUFFER_LO                  (U2TBL)
//#define BAUD_RATE_GEN                       (U2BRG)
//#define RECEIVE_IRQ                         (S2RIC)
//#define TRANSMIT_IRQ                        (S2TIC)
//#define UART_MODE_REG                       (U2MR)
//Change to use UART0
#define RECEIVE_BUFFER_HI                   //(U0RBH)
#define RECEIVE_BUFFER_LO                   //(U0RBL)
#define BUFFER_CONTROL_REGH                 //(U0C1)
#define BUFFER_CONTROL_REGL                 //(U0C0)
#define TRANSMIT_BUFFER_LO                  //(U0TBL)
#define BAUD_RATE_GEN                       //(U0BRG)
#define RECEIVE_IRQ                         //(S0RIC)
#define TRANSMIT_IRQ                        //(S0TIC)
#define UART_MODE_REG                       //(U0MR)

// @@adjust
// Burst Timer
//#define BURST_TIMER_MODE_REG                (TB0MR)
//#define BURST_TIMER_INTERRUPT_CONTROL_REG   (TB0IC)
//#define BURST_STOP_TIMER                    (TABSR_bit.TB0S=0)
//#define BURST_START_TIMER                   (TABSR_bit.TB0S=1)
//#define BURST_TIMER_VALUE                   (TB0)
//Change to Use TA2
#define BURST_TIMER_MODE_REG                //(TA2MR)
#define BURST_TIMER_INTERRUPT_CONTROL_REG   //(TA2IC)
#define BURST_STOP_TIMER                    //(TABSR_bit.TA2S=0)
#define BURST_START_TIMER                   //(TABSR_bit.TA2S=1)
#define BURST_TIMER_VALUE                   //(TA2)

// @@adjust
// Gap Timer
//#define GAP_TIMER_MODE_REG                  (TB1MR)
//#define GAP_TIMER_INTERRUPT_CONTROL_REG     (TB1IC)
//#define GAP_STOP_TIMER                      (TABSR_bit.TB1S=0)
//#define GAP_START_TIMER                     (TABSR_bit.TB1S=1)
//#define GAP_TIMER_VALUE                     (TB1)

//Change to Use TB3
#define GAP_TIMER_MODE_REG                  //(TB3MR)
#define GAP_TIMER_INTERRUPT_CONTROL_REG     //(TB3IC)
#define GAP_STOP_TIMER                      //(TBSR_bit.TB3S=0)
#define GAP_START_TIMER                     //(TBSR_bit.TB3S=1)
#define GAP_TIMER_VALUE                     //(TB3)



//--------------------------------------------------------------------------------------------------
//! Definitions
#define LAYER2_BURST_FLAG       0x40
#define LAYER2_PRIMARY_MASTER   0x80
#define LAYER2_BURST_DELIMITER  0x81
#define LAYER2_LONGADR_FLAG     0x80
#define LAYER2_BURSTMSG_REPEAT  0x80

// length of receive/transmit buffer data bytes area
#define HART_XMIT_RCV_BUFFER_LEN 80

//#define PROVIDE_HIGHER_BITRATES

//! structure used by transmitter and receiver
typedef struct
{
    TUSIGN8 cntPreamble; //!< number of preambles to be send
    TUSIGN8 delimiter;
    TUSIGN8 aryAdr[5];   //!< devicde address
    TUSIGN8 aryExp[3];   //!< expansion bytes
    TUSIGN8 command;
    TUSIGN8 numByte;     //!< number of data bytes
    TUSIGN8 firstResp;   //!< first Response-Byte
    TUSIGN8 secResp;     //!< secound Response-Byte
    TUSIGN8 aryData[HART_XMIT_RCV_BUFFER_LEN]; //!< data bytes
    TUSIGN8 checksum;    //!< Longitudinal XOR
} T_LAYER2_BUFFER;

// flags for communication between timer, receiver and transmitter
//lint -e46 Marcel Schilg 2007-10-25 (Field type is not int or unsigned, ok for this compiler)
typedef struct
{
    TUSIGN16 layer7requestedBit : 1; //!< eTRUE --> responsecode has to be generated
    TUSIGN16 receiverActivBit   : 1; //!< eTRUE --> transmission not allowed
    TUSIGN16 masterAddressBit   : 1; //!< eTRUE --> last frame sent to primary master
    TUSIGN16 acknowledgeBit     : 1; //!< eTRUE --> monitored frame was an acknowledge form another
                                    //!<           slave
    TUSIGN16 monitoringActivBit : 1; //!< eTRUE --> only monitoring the actual frame
                                    //!<           function for K#109 response
    TUSIGN16 fskDisabled        : 1; //!< eTRUE --> lks is pluged (or DEVCIE without FSK)
    TUSIGN16 burstMsgInit       : 1; //!< eTRUE --> first burst message after device restart
    TUSIGN16 burstMsgReqBuilt   : 1; //!< eTRUE --> burst request built
    TUSIGN16 burstMsgRespBuilt  : 1; //!< eTRUE --> burst response built
    TUSIGN16 burstMsg1stSent    : 1; //!< eTRUE --> burst frame sent to first master
    TUSIGN16 burstMsgWoutBuilt  : 1; //!< eTRUE --> burst frame sent without built
    TUSIGN16 burstBufferInit    : 1; //!< eTRUE --> init done after first burst start
    TUSIGN16 slaveTimeOutFlag   : 1; //!< eTRUE --> slave time out expired
    TUSIGN16 burstCmdChanged    : 1; //!< eTRUE --> the command to be burst was changed via #108 during active burst
    TUSIGN16 bolDeviceIsBusy    : 1; //!< eTRUE --> device is busy doing something else
    TUSIGN16 burstFrameReceiving: 1; //!< eTRUE --> device is receving burst message
} T_LAYER2_FLAGS;

//! structure used for the mapping of T_UNIT objects used in receive or transmit IR
//! Necessary as the HART ISR have a higher prio then the RAM pattern test.
//! Note: this handling could be improved if these objects were assigned a dedicated file
//! which RAM immage would go into STATIC_INTERRUPT_DATA.  Here for simplicity clone objects are used
typedef struct
{
    TUSIGN8 findDeviceSwitch;
    TUSIGN8 pollAddr;
    TUSIGN8 tag[6];
    TUSIGN8 longTag[32];
    TUSIGN8 univCmdRev;
    TUSIGN8 deviceIdNumber[3];
    TUSIGN8 numberRespPream;
} T_LAYER2_T_UNIT_MAPPER;

// length of soft modem info
#define HART_SOFT_MODEM_INFO_LEN 8
//--------------------------------------------------------------------------------------------------
//! transmit and receive buffer of layer 2
//#pragma dataseg=FAST STATIC_INTERRUPT_DATA
extern __no_init FAST T_LAYER2_BUFFER uTransmitBuffer_LAYER2;
extern __no_init FAST T_LAYER2_BUFFER uReceiveBuffer_LAYER2;
extern __no_init FAST TUSIGN8  softModemInfo[HART_SOFT_MODEM_INFO_LEN];

// flags for communication between timer, receiver and transmitter also for burst management
extern FAST T_LAYER2_FLAGS uLayer2flags;
extern FAST T_LAYER2_T_UNIT_MAPPER t_unit_mapper;
extern FAST TUSIGN8 performDeviceReset;
extern FAST TUSIGN8 bitRateCode;
//#pragma dataseg=default
//--------------------------------------------------------------------------------------------------
/*!
 \brief     initialize mapper for T_UNIT parameters used in ISR
 \author    Stefan Tabelander
 \date      2011-04-11
 \param
 \return
 \warning
 \bug
*/
//--------------------------------------------------------------------------------------------------

void Initialize_T_UNIT_MAPPER(TUSIGN16);


//--------------------------------------------------------------------------------------------------
/*!
 \brief     initialize Layer 2 and the HART-UART
 \author    Heiko Kresse
 \date      2004-02-24
 \param
 \return
 \warning
 \bug
*/
//--------------------------------------------------------------------------------------------------

void Initialize_LAYER2(void);

//--------------------------------------------------------------------------------------------------
/*!
 \brief     returns eTRUE if layer 7 is requested
 \author    Heiko Kresse
 \date      2004-02-24
 \param
 \return
 \warning
 \bug
*/
//--------------------------------------------------------------------------------------------------

TBOOL AskIfLayer7requested_LAYER2(void);


//--------------------------------------------------------------------------------------------------
/*!
 \brief     clears the layer7 request
 \author    Heiko Kresse
 \date      2004-02-24
 \param
 \return
 \warning
 \bug
*/
//--------------------------------------------------------------------------------------------------

void ClearLayer7request_LAYER2(void);


//--------------------------------------------------------------------------------------------------
/*!
 \brief     initialialize burst buffer with always identical frame start bytes
 \author    Bernhard tohermes
 \date      2010-04-15
 \param
 \return
 \warning
 \bug
*/
//--------------------------------------------------------------------------------------------------

void InitBurstBuffer_LAYER2(void);


//--------------------------------------------------------------------------------------------------
/*!
 \brief     initialialize burst buffer with always identical frame start bytes
 \author    Bernhard tohermes
 \date      2010-04-15
 \param
 \return    eTRUE: device in burst mode
 \warning
 \test
 test-date:
 \n by:
 \n environment:
 \n intention:
 \n result  module test:
 \n result  Lint Level 3:
 \bug
*/
//--------------------------------------------------------------------------------------------------
TBOOL DetectBurstMode_LAYER2(void);

//--------------------------------------------------------------------------------------------------
/*!
 \brief     prepare transmitbuffer for transmission of a burst frame
 \author    Heiko Kresse
 \date      2004-02-23
 \param     command number to include in burst frame
 \return
 \warning
 \test
 test-date: 2005-03-14
 \n by:     Heiko Kresse
 \n environment: defined in HCF-Specification
 \n intention: 1. complete instruction covarage (guarantied by link-layer-test)
 \n            2. black-box-test --> link-layer-test
 \n result  link layer test: OK
 \n result  Lint Level 3: free // approved warnings and approved infos are suppressed
 \bug
*/
//--------------------------------------------------------------------------------------------------
void PrepareBurstBuf_LAYER2(TUSIGN8 cmdNumber);

extern void InitBurstBuffer_LAYER2(void);
