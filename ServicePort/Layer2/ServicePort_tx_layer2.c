//--------------------------------------------------------------------------------------------------
/*
 Copyright      Copyright ABB, 2005.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Subsystem Hart
 Module
 Description    implementation layer 2 Service Port-Transmitter

 Remarks
                

*/
//--------------------------------------------------------------------------------------------------
#include "System/Interface/common_type.h"
#include "System/Interface/system.h"
#include "System/Interface/microcontroller.h"
#include "serviceport_layer2.h"

TUSIGN16 sendStateCnt_SP = 0;
//----------------------------------------------------------------------------------------------------------
//! module variables
static FAST enum
{
    SP_TRANSMIT_STATE_PREAMBLE,   //!< send preambles
    SP_TRANSMIT_STATE_DELIMITER,  //!< send delimiter
    SP_TRANSMIT_STATE_SHORTADR,   //!< send short address
    SP_TRANSMIT_STATE_ADDRESS,    //!< send long address
    SP_TRANSMIT_STATE_COMMAND,    //!< send command
    SP_TRANSMIT_STATE_NUMBYTE,    //!< send number of databytes
    SP_TRANSMIT_STATE_FIRSTRESP,  //!< send first response code
    SP_TRANSMIT_STATE_SECRESP,    //!< send second response code
    SP_TRANSMIT_STATE_EXPANSION,  //!< send expansion-bytes
    SP_TRANSMIT_STATE_DATA,       //!< send data-bytes
    SP_TRANSMIT_STATE_CHKSUM,     //!< send checksum
    SP_TRANSMIT_STATE_DUMMY      //!< send an extra byte to guarantee carrier stand long enough
   
} mSpTransmitState;

static TUSIGN8 mSpTxBuffer[SP_XMIT_BUFFER_LEN];
static TUSIGN8 mSpTxBufferIndex;
static TUSIGN8 mSpBufferIndex;

// -----------Module variables defined externally-----------------------------------------------  
extern T_SPLAYER2_BUFFER   ServPortRxBuffer;
extern T_SPLAYER2_BUFFER   ServPortTxBuffer;
extern T_SPLAYER2_FLAGS    SpLayer2flags;
extern TUSIGN8 SpExpBytesCounter;

//--------------------------------------------------------------------------------------------------
/*!
 \brief     transmission message assembly( ServPortTxBuffer -> mSpTxBuffer)
 \author    Pluto Xu
 \date      2022-11-07
 \param
 \return
 \warning
 \test
 test-date: 
 \n by:     
 \n environment:
 \n intention: 
 \n result  link layer test:
 \n result  Lint Level 3: 
 \bug
*/
//--------------------------------------------------------------------------------------------------
static void SpResponseMessageAssembly_LAYER2(void)
{
    TUSIGN16 dataLen = 0;
    TUSIGN8 cmd;
    
    cmd = ServPortTxBuffer.command&0x7F;
    mSpTxBufferIndex = 0;
    
    while(mSpTransmitState != SP_TRANSMIT_STATE_DUMMY)
    {
        switch(mSpTransmitState)
        {
            case SP_TRANSMIT_STATE_PREAMBLE:
                sendStateCnt_SP = 1;
                mSpTxBuffer[mSpTxBufferIndex++] = 0xFF;
                ServPortTxBuffer.cntPreamble--;
                if( ServPortTxBuffer.cntPreamble < 1 )
                {
                    if((cmd==UARTI_IDENT_DISPLAY_ECHO)
                    || (cmd==UARTI_IDENT_DISPLAY_FRAME)
                    || (cmd==UARTI_IDENT_COMMAND)
                    || (cmd==UARTI_IDENT_COMMAND_ECHO)
                    || (cmd==UARTI_IDENT_SHMI_READ))
                    {
                        mSpTransmitState = SP_TRANSMIT_STATE_COMMAND;
                    }
                    else
                    {
                        mSpTransmitState = SP_TRANSMIT_STATE_DELIMITER;
                    }
                }
                break;

            case SP_TRANSMIT_STATE_DELIMITER:
                mSpTxBuffer[mSpTxBufferIndex++] = ServPortTxBuffer.delimiter;
                mSpTransmitState = SP_TRANSMIT_STATE_SHORTADR;
                break;

            case SP_TRANSMIT_STATE_SHORTADR:
                mSpTxBuffer[mSpTxBufferIndex++] = ServPortTxBuffer.aryAdr[0];
                // short or long frame?
                if( !(ServPortTxBuffer.delimiter & SP_LAYER2_LONGFRAME_FLAG) )
                {
                    // OK, Shortframe
                    // expansion bytes to send if exists.
                    if( SpExpBytesCounter )
                    {
                        // send first expansion byte
                        mSpBufferIndex = 0;
                        mSpTransmitState  = SP_TRANSMIT_STATE_EXPANSION;
                    }
                    else
                    {
                        mSpTransmitState  = SP_TRANSMIT_STATE_COMMAND;
                    }
                }
                else
                {
                    // go ahead with next addressbyte
                    mSpBufferIndex = 1;
                    mSpTransmitState  = SP_TRANSMIT_STATE_ADDRESS;
                }
                break;

            case SP_TRANSMIT_STATE_ADDRESS:
                mSpTxBuffer[mSpTxBufferIndex++] = ServPortTxBuffer.aryAdr[mSpBufferIndex];
                mSpBufferIndex++;

                if( mSpBufferIndex>=sizeof(ServPortTxBuffer.aryAdr) )
                {
                    // expansion bytes to send
                    if( SpExpBytesCounter )
                    {
                        // send first expansion byte
                        mSpBufferIndex = 0;
                        mSpTransmitState  = SP_TRANSMIT_STATE_EXPANSION;
                    }
                    else
                    {
                        mSpTransmitState  = SP_TRANSMIT_STATE_COMMAND;
                    }
                }
                break;

            case SP_TRANSMIT_STATE_EXPANSION:
                mSpTxBuffer[mSpTxBufferIndex++] = ServPortTxBuffer.aryExp[mSpBufferIndex];
                mSpBufferIndex++;

                if( mSpBufferIndex>=sizeof(ServPortTxBuffer.aryExp) )
                {
                    mSpTransmitState  = SP_TRANSMIT_STATE_COMMAND;
                }
                break;

            case SP_TRANSMIT_STATE_COMMAND:
                mSpTxBuffer[mSpTxBufferIndex++] = ServPortTxBuffer.command;
                mSpTransmitState = SP_TRANSMIT_STATE_NUMBYTE;
                break;

            case SP_TRANSMIT_STATE_NUMBYTE:
                mSpTxBuffer[mSpTxBufferIndex++] = ServPortTxBuffer.numByte;
                if((cmd==UARTI_IDENT_DISPLAY_ECHO)
                 || (cmd==UARTI_IDENT_DISPLAY_FRAME)
                 || (cmd==UARTI_IDENT_COMMAND)
                 || (cmd==UARTI_IDENT_COMMAND_ECHO)
                 || (cmd==UARTI_IDENT_SHMI_READ))
                {               
                    if(ServPortTxBuffer.numByte==0)
                    {
                      mSpTransmitState = SP_TRANSMIT_STATE_CHKSUM;
                    }
                    else
                    {
                      mSpTransmitState = SP_TRANSMIT_STATE_DATA;
                    }
                }
                else
                {
                    mSpTransmitState = SP_TRANSMIT_STATE_FIRSTRESP;
                }
                break;

            case SP_TRANSMIT_STATE_FIRSTRESP:
                mSpTxBuffer[mSpTxBufferIndex++] = (TUSIGN8)ServPortTxBuffer.firstResp;
                mSpTransmitState = SP_TRANSMIT_STATE_SECRESP;
                break;

            case SP_TRANSMIT_STATE_SECRESP:
                mSpTxBuffer[mSpTxBufferIndex++] = ServPortTxBuffer.secResp;
                // the frame has data bytes ?
                if( ServPortTxBuffer.numByte>2 )
                {
                    // yes, go on with data
                    mSpTransmitState = SP_TRANSMIT_STATE_DATA;
                    mSpBufferIndex = 0;
                }
                else // else, go on with the checksum
                {
                    mSpTransmitState = SP_TRANSMIT_STATE_CHKSUM;
                }
                break;

            case SP_TRANSMIT_STATE_DATA:
                mSpTxBuffer[mSpTxBufferIndex++] = ServPortTxBuffer.aryData[mSpBufferIndex];
                mSpBufferIndex++;
                if((cmd==UARTI_IDENT_DISPLAY_ECHO)
                || (cmd==UARTI_IDENT_DISPLAY_FRAME)
                || (cmd==UARTI_IDENT_COMMAND)
                || (cmd==UARTI_IDENT_COMMAND_ECHO)
                || (cmd==UARTI_IDENT_SHMI_READ))
                {
                    dataLen = ServPortTxBuffer.numByte;
                }
                else
                {
                    dataLen = ServPortTxBuffer.numByte-2; // cause 2 Bytes: first and sec resp bytes already sent.
                } 
                // reached last data ?
                if( mSpBufferIndex >= dataLen) // cause 2 Bytes: first and sec resp bytes already sent.
                {
                    // yes, go on with checksum
                    mSpTransmitState = SP_TRANSMIT_STATE_CHKSUM;
                }
                break;

            case SP_TRANSMIT_STATE_CHKSUM:
                mSpTxBuffer[mSpTxBufferIndex++] = ServPortTxBuffer.checksum;
                mSpTransmitState = SP_TRANSMIT_STATE_DUMMY;
                break;
                
            default:
              break;
        }
    }
}
//--------------------------------------------------------------------------------------------------
/*!
 \brief     prepare and start transmission
 \author    Ganapathi
 \date      2008-01-21
 \param
 \return
 \warning
 \test
 test-date: 
 \n by:     
 \n environment: defined in HCF-Specification
 \n intention: 1. complete instruction covarage 
 \n            2. black-box-test --> link-layer-test
 \n result  link layer test:
 \n result  Lint Level 3: free // approved warnings and approved infos are suppressed
 \bug
*/
//--------------------------------------------------------------------------------------------------
void StartSpResponse_LAYER2(void)
{
    TUSIGN16 u;
    TUSIGN8  chkSum;
    TUSIGN8  cmd;
    uint8_t transmitData;

    cmd = ServPortTxBuffer.command&0x7F;

    ServPortRxBuffer.command = 0xFF;
    if((cmd==UARTI_IDENT_DISPLAY_ECHO)
    || (cmd==UARTI_IDENT_DISPLAY_FRAME)
    || (cmd==UARTI_IDENT_COMMAND)
    || (cmd==UARTI_IDENT_COMMAND_ECHO)
    || (cmd==UARTI_IDENT_SHMI_READ))//display Data

    {
        ServPortTxBuffer.cntPreamble = 2;
        ServPortTxBuffer.firstResp = SP_RESP_OK;
        ServPortTxBuffer.secResp = 0;
    }
    else
    {
        ServPortTxBuffer.cntPreamble = 5; 
        // delimiter = request delimiter (but with physical layer type = asynchronous)  + acknowledge-flag
        ServPortTxBuffer.delimiter = (ServPortRxBuffer.delimiter & 0xE7) | 0x04;
        
        // Service Port doesn't support Burst Mode.
        ServPortTxBuffer.aryAdr[0] &= ~SP_LAYER2_BURST_FLAG;

        // we are not microsoft, so we have to be sure that no buffer overrun will happen :)
        if( ServPortTxBuffer.numByte>sizeof(ServPortTxBuffer.aryData) )
        {
            ServPortTxBuffer.numByte=sizeof(ServPortTxBuffer.aryData);
        }
    }

    // start checksum generating with the delimiter
    chkSum = ServPortTxBuffer.delimiter;

    // checksum over address, expansion, command, first and second response, data
    for(u=0;
            u<( sizeof(ServPortTxBuffer.aryAdr)
           + sizeof(ServPortTxBuffer.aryExp)
           + sizeof(ServPortTxBuffer.command)
           + sizeof(ServPortTxBuffer.numByte)
           +2
           + ServPortTxBuffer.numByte);   // includes first and second response byte
           u++)
    {
        // this operation leaves the borders of aryAdr, but the structure guarantees that the next
        // data are expansion bytes, command, number of bytes, first resp, second resp, data
        chkSum ^=ServPortTxBuffer.aryAdr[u];
    }

    ServPortTxBuffer.checksum = chkSum;
    // Reset index.
    mSpBufferIndex = 0;

    // reset state machine
    mSpTransmitState = SP_TRANSMIT_STATE_PREAMBLE;
    // Decrement Preamble count, as we will be sending a Preamble immediately after this.
    ServPortTxBuffer.cntPreamble = ServPortTxBuffer.cntPreamble - 1;
    // Sending first Preamble.
    transmitData = 0xFF;
    if(HAL_UART_Transmit_IT(&HMI_UartHandle, (uint8_t*)&transmitData, 1)!= HAL_OK)
    {
        Error_Handler();
    }
    // Clear layer 7 request flag
    SpLayer2flags.layer7requestedBit = 0;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     transmission state machine
 \author    Ganapathi
 \date      2008-01-21
 \param
 \return
 \warning
 \test
 test-date: 
 \n by:     
 \n environment: defined in HCF-Specification
 \n intention: 1. complete instruction covarage (code analyzes shows that the link-layer-test guaranties this)
 \n            2. black-box-test --> link-layer-test
 \n result  link layer test: OK
 \n result  Lint Level 3: free // approved warnings and approved infos are suppressed
 \bug
*/
//--------------------------------------------------------------------------------------------------
void servicePortTransmit(void)
{
    SpLayer2flags.receiverActivBit = 0;
    
    SpResponseMessageAssembly_LAYER2();
    if(mSpTxBufferIndex != 0)
    {
      if(HAL_UART_Transmit_IT(&HMI_UartHandle, (uint8_t*)mSpTxBuffer, mSpTxBufferIndex)!= HAL_OK)
      {
          Error_Handler();
      }
    }
    
    sendStateCnt_SP = 0;
    SpLayer2flags.layer7requestedBit = 0;
    SpLayer2flags.receiverActivBit = 1;
}
//-------------------------------------------------------------------------------------------------
