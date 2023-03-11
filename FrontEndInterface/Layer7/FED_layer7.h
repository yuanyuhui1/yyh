//--------------------------------------------------------------------------------------------------
/*
 Copyright      Copyright ABB, 2010.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.
 System         frontend
 Module
 Description    Frontend Interface Layer 7
 Remarks
*/
//--------------------------------------------------------------------------------------------------


#ifdef __FED_LAYER7_H__
  #error layer7.h included more than once
#endif
#ifndef __COMMON_TYPE_H__
  #error common_type.h must be include before FED_LAYER2_attributes.h
#endif


#define __FED_LAYER7_H__


// MSG TYPE Definitions
#define FED_FRAMETYPE_RD_CMD      				0x00 
#define FED_FRAMETYPE_WR_CMD      			0x01 

#define FED_FRAMETYPE_SERIAL_DATAOK    0x02  //serial is ok
#define FED_FRAMETYPE_CRCERROR   			0x04  //crc error
#define FED_FRAMETYPE_FEBUSY      				0x10  //frontend busy
#define FED_DATA_ACCESS_ERROR        			0x20  //data access error

//Framnum definition
#define SUBCMD00						0x00
#define SUBCMD01						0x01
#define SUBCMD02						0x02
#define SUBCMD03						0x03
#define SUBCMD04						0x04
#define SUBCMD05						0x05
#define SUBCMD06						0x06
#define SUBCMD07						0x07
#define SUBCMD08						0x08
#define SUBCMD09						0x09

#define  NVFRAMELENGTH					30
#define WRMETERSIZELEN					2
#define WRAUTOZEROLEN					0
#define WRSELFDIAGLEN					0
#define WRFERTCLEN							5
#define WRMAINTAINLEN					4

#define MAXNVFRAMENUM					8

#define FE_OK										0



// length of receive/transmit buffer data bytes area
#define FEB_XMIT_RCV_BUFFER_LEN  248
// structure used by transmitter and receiver

#pragma pack(push,t_FED_FRAME_BUFFER,1)
typedef struct
{
    TUSIGN8  msgtype;       //MSG TYPE
    TUSIGN8  framenum;      //COMAND CODE
    TUSIGN8  cmd;           //Number of data bytes
    TUSIGN8  datalen;
    TUSIGN8  arydata[FEB_XMIT_RCV_BUFFER_LEN]; // data bytes
    TUSIGN16 checkCRC;      //CRC CHECK
} T_FED_FRAME_RX_BUFFER;

typedef struct
{
    TUSIGN8  msgtype;     	//MSG TYPE
    TUSIGN8  framenum;
    TUSIGN8  cmd;     		//COMAND CODE
    TUSIGN8  datalen;
    TUSIGN8  arydata[FEB_XMIT_RCV_BUFFER_LEN]; // data bytes
    TUSIGN16 checkCRC;      //CRC CHECK
} T_FED_FRAME_TX_BUFFER;
#pragma pack(pop,t_FED_FRAME_BUFFER)

extern FAST T_FED_FRAME_TX_BUFFER *pTxBuf;
extern FAST T_FED_FRAME_RX_BUFFER *pRxBuf;

void ReadData_layer7(const CMD_MSG* pMessage);
void WriteData_layer7(const CMD_MSG* pMessage);
TUSIGN8 GetFrameNum(void);