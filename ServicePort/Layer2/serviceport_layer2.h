//--------------------------------------------------------------------------------------------------
/*
 Copyright      Copyright ABB, 2005.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Subsystem Service Port.
 Module
 Description    interface Layer 2

 Remarks

*/
//--------------------------------------------------------------------------------------------------


#ifndef __COMMON_TYPE_H__
  #error common_type.h must be include before layer2_attributes.h
#endif

#ifndef STM32H7xx_HAL_UART_H
  #error stm32h7xx_hal_uart.h must be include defore layer2.h
#endif

#ifdef __SP_LAYER2_H__
  #error layer2.h included more than once
#endif

#define __SP_LAYER2_H__

// length of receive/transmit buffer data bytes area
#define SP_XMIT_BUFFER_LEN         240 //43
#define SP_LAYER2_LONGFRAME_FLAG   0x80
#define SP_LAYER2_BURST_FLAG       0x40
#define SP_OBJECT_DATA_BUFFER_LEN  32
//#define PROVIDE_HIGHER_BITRATES


typedef enum _E_UARTI_MSG_IDENT
{
    //19200
    UARTI_IDENT_READ = 1,         //for service tool
    UARTI_IDENT_WRITE = 2,        //for service tool
    UARTI_IDENT_READ_MEMORY = 3,  //for service tool
    UARTI_IDENT_WRITE_MEMORY = 4, //for service tool
    UARTI_IDENT_READ_WAVEFORM = 5, //for service tool

    //9600bps,other format
    UARTI_IDENT_COMMAND_ECHO = 0xA, 
    UARTI_IDENT_COMMAND = 0xB,        //used for transfer hart command to CB
    UARTI_IDENT_SERVICE_ECHO = 0xC,   //used for service tool type
    UARTI_IDENT_SERVICE = 0xD,
    UARTI_IDENT_BOOT = 0xE,            //bootloader communication   

    UARTI_IDENT_STATUS = 0x10,         //smartHMI   <-->    CB, periodally executed.
    UARTI_IDENT_SHMI_READ = 0x11,        
    UARTI_IDENT_SHMI_WRITE = 0x12,         
    UARTI_IDENT_SHMI_READ_MEMORY = 0x13,         
    UARTI_IDENT_SHMI_WRITE_MEMORY = 0x14,         
    
    UARTI_IDENT_DISPLAY_CONTROL = 0x40,         //for remoteHMI tool
    UARTI_IDENT_DISPLAY_ECHO = 0x42,
    UARTI_IDENT_DISPLAY_KEY_INFO = 0x45,    
    UARTI_IDENT_DISPLAY_FRAME = 0x4F
    
}E_UARTI_MSG_IDENT;

// flags for communication between timer, receiver and transmitter
typedef struct
{
    TUSIGN8 layer7requestedBit; //!< eTRUE --> responsecode has to be generated
    TUSIGN8 receiverActivBit; //!< eTRUE --> transmission not allowed
    TUSIGN8 monitoringActivBit; //!< eTRUE --> only monitoring the actual frame
} T_SPLAYER2_FLAGS;


typedef enum
{
    SP_RESP_OK,                               // #0 Success - no command specific error
    SP_RESP_ERR_RESERVED_1,                   // #1 Reserved
    SP_RESP_ERR_INVALID_SELECTION,            // #2 Invalid selection
    SP_RESP_ERR_DATA_TOO_LARGE,               // #3 Passed parameter too large
    SP_RESP_ERR_DATA_TOO_SMALL,               // #4 Passed parameter too small
    SP_RESP_ERR_TOO_FEW_DATA,                 // #5 Too few data bytes receive
    SP_RESP_ERR_SPECIFIC_CMD_ERROR,           // #6 Transmitter specific command error
    SP_RESP_ERR_WRITE_PROTECTED,              // #7 Write protected
    SP_RESP_WARN_SET_TO_NEAREST_VALUE,        // #8 Warning: set to nearest possible value
    SP_RESP_ERR_LOWER_RANGE_TOO_HIGH,         // #9 Lower range value too high
    SP_RESP_ERR_LOWER_RANGE_TOO_LOW,          // #10 Lower range value too low
    SP_RESP_ERR_UPPER_RANGE_TOO_HIGH = 11,    // #11 Upper range value too high
    SP_RESP_ERR_DEVICE_IN_MULTI_DROP = 11,    // #11 Device was set to multi drop mode
    SP_RESP_ERR_UPPER_RANGE_TOO_LOW,          // #12 Upper range value too low
    SP_RESP_ERR_UPPER_LOWER_RANGE_OUT_OF_LIM, // #13 span too small
    SP_RESP_WARN_SPAN_TOO_SMALL,              // #14 Warning: span too small
    SP_RESP_ERR_RESERVED_15,                  // #15 Reserved
    SP_RESP_ERR_ACCESS_RESTRICTED,            // #16 Access restricted
    SP_RESP_ERR_RESERVED_17,                  // #17 Reserved
    SP_RESP_ERR_RESERVED_18,                  // #18 Reserved
    SP_RESP_ERR_RESERVED_19,                  // #19 Reserved
    SP_RESP_ERR_INVALID_EXT_CMD_NUMBER,       // #20 Invalid extended command number
    SP_RESP_ERR_RESERVED_21,                  // #21 Reserved
    SP_RESP_ERR_RESERVED_22,                  // #22 Reserved
    SP_RESP_ERR_RESERVED_23,                  // #23 Reserved

    SP_RESP_ERR_DEVICE_BUSY         = 32,     // #32 Device is busy

    SP_RESP_ERR_CMD_NOT_IMPLEMENTED = 64,     // #64 Command not implemented
    SP_RESP_ERR_SPAN_TOO_SMALL,               // #65 Reserved: Span too small error
    SP_RESP_ERR_SET_TO_NEAREST_VALUE,         // #66 Reserved: Set to nearest possible value error

    SP_RESP_WARN_LOWER_RANGE_TOO_LOW = 112,   // #112 Reserved: Lower range value too low
    SP_RESP_WARN_UPPER_RANGE_TOO_HIGH,        // #113 Reserved: Upper range value too high
    SP_RESP_WARN_ACCESS_RESTRICTED,           // #114 Reserved: Wrong state for cmd
    SP_RESP_WARN_INVALID_SELECTION,            // #115 Reserved: Code or index not allowed for cmd
    
    SP_BUFFER_OVERFLOW = 130,                 // #130: Buffer overflow.
    SP_CHECKSUM_ERROR = 136,                   // #136: Check sum error.
    SP_FRAMING_ERROR = 144,                   //#144: Framing error.
    SP_OVERRUN_ERROR = 160,                   //#160: Overrun error.
    SP_PARITY_ERROR = 192                    //#192 Parity error. 
      
}T_SP_RESP_ENUM;

//! structure used by transmitter and receiver
typedef struct
{
    TUSIGN8 reserved;
    TUSIGN8 cntPreamble; //!< number of preambles to be send
    TUSIGN8 delimiter;
    TUSIGN8 aryAdr[5];   //!< devicde address
    TUSIGN8 aryExp[3];   //!< expansion bytes
    TUSIGN8 command;
    TUSIGN8 numByte;     //!< number of data bytes
    T_SP_RESP_ENUM firstResp;   //!< first Response-Byte
    TUSIGN8 secResp;     //!< secound Response-Byte
    TUSIGN8 aryData[SP_XMIT_BUFFER_LEN]; //!< data bytes
    TUSIGN8 checksum;    //!< Longitudinal XOR
} T_SPLAYER2_BUFFER;

extern UART_HandleTypeDef HMI_UartHandle;
extern TUSIGN8 mSpHmiRxData;

void InitializeHmi(void);
void InitializeHMI_LAYER2(void);

extern void  InitializeSp_LAYER2(void);
extern void InitializeSmartHmi_LAYER2(void);
extern TBOOL TxRequestPending(void);
extern void  StartSpResponse_LAYER2(void);

void layer2_ApplyPower(void);
extern void servicePortReceive(void);
extern void servicePortTransmit(void);
extern void InitServicePortRxStateMachine(void);

extern TUSIGN8  isSmartHMIConnected;
extern TUSIGN8  SPRxErr;
extern TUSIGN16 SPRxTime;
extern TUSIGN16 sendStateCnt_SP;
