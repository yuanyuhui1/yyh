//--------------------------------------------------------------------------------------------------
/*
Copyright      Copyright ABB, 2005.
All rights reserved. Reproduction, modification,
use or disclosure to third parties without express
authority is forbidden.

System         Subsystem Service Port
Module
Description    implementation layer 2 Service Port, Receiver

Remarks


*/
//--------------------------------------------------------------------------------------------------

#include <intrinsics.h>
#include <string.h>
#include "System/Interface/system.h"
#include "System/Interface/assert.h"
#include "System/Interface/common_type.h"
#include "System/Interface/microcontroller.h"
#include "RTOS/Interface/rtos.h"
#include "serviceport_layer2.h"

#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"


#include "Coordinator/Interface/subsystem_idx.h"


#include "HART/Layer2/Interface/layer2.h"
#include "HART/Layer7/Source/ObjfunctRB.h"
#include "HART/Layer7/Source/cmddef.h"
#include "HART/Interface/t_data_obj_burstmsg_confvalues.h"
#include "HART/Interface/t_data_obj_burstmsg_managevalues.h"
#include "HART/Layer7/Interface/layer7.h"
#include "HART/Interface/Hart.h"
#include "HART/Interface/Hart_idx.h"		// to identify client subsystem(s)

#include "Hmi/Interface/HMI.h"
#include "Hmi/Interface/HMI_idx.h"

#include "ServicePort/Layer7/servicePort_layer7.h"
#include "ServicePort/Interface/servicePort.h"

#define SW_REV01 7
//------------------Local Defines---------------------------------------------------------------------
#define MASTER2SLAVE      2
#define SLAVE2MASTER      6
#define BURST_FRAME       1

//-----------------Local Constants--------------------------------------------------------------------
static const TUSIGN8  spHartPollAdrs = 0;

//-----------------Module Variables-------------------------------------------------------------------
T_SPLAYER2_BUFFER    ServPortRxBuffer;
T_SPLAYER2_BUFFER    ServPortTxBuffer;

T_SPLAYER2_FLAGS     SpLayer2flags = {0, 0, 0};
TUSIGN16 SPRxTime  = 0;

TUSIGN8 SpExpBytesCounter;

//------------------Variables local to the file--------------------------------------------------------------------
static FAST enum
{
	SP_RECEIVE_STATE_PREAMBLE,
	SP_RECEIVE_STATE_DELIMITER,
	SP_RECEIVE_STATE_SHORTADR,
	SP_RECEIVE_STATE_LONGADR,
	SP_RECEIVE_STATE_EXPANSION,
	SP_RECEIVE_STATE_COMMAND,
	SP_RECEIVE_STATE_NUMBYTE,
	SP_RECEIVE_STATE_DATA,
	SP_RECEIVE_STATE_CHECKSUM
} SpReceiveState;

static TUSIGN8  SpBufferIndex;

TUSIGN32 SP_LastRecTime = 0;
//TUSIGN8 SP_UART0_rx_buffer[SP_UART0_RX_BUF_SIZE];

static TBOOL SaveRxData_HMIFrame(void)
{    
    return hmi.HMI_PutPacketBuffer_SRV(SERVICEPORT_IDX, SP_HMI_PROCESS_HMIFRAME, ServPortRxBuffer.aryData, ServPortRxBuffer.numByte);
}

static TBOOL SaveRxData_HMIKey(void)
{
    return hmi.HMI_PutPacketBuffer_SRV(SERVICEPORT_IDX, SP_HMI_PROCESS_HMIKEY, ServPortRxBuffer.aryData, ServPortRxBuffer.numByte);
}

static TBOOL SaveRxData_HART(void)
{
    if(hart.HART_PutReceiveBuffer_SRV(SERVICEPORT_IDX, HART_SP_SAVE_RX_DATA, ServPortRxBuffer.aryData, SP_XMIT_BUFFER_LEN))
    {
        return eTRUE;
    }else{
        ServPortTxBuffer.numByte = 0;
        return eFALSE;
    }
}

//-------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/*!
\brief     initialize service port receive state machine
\author    Ganapathi
\date      2008-01-21
\param
\return
\warning
\test
test-date: 
\n by:     
\n environment: defined in HCF-Specification
\n intention: 1. complete instruction covarage.
\n            2. black-box-test --> link-layer-test
\n result  link layer test:
\n result  Lint Level 3: free // approved warnings and approved infos are suppressed
\bug
*/
//--------------------------------------------------------------------------------------------------

void InitServicePortRxStateMachine(void)
{
	// RxD-Puffer zur�cksetzen
	SpBufferIndex = 0;
	SpReceiveState  = SP_RECEIVE_STATE_PREAMBLE;
	ServPortRxBuffer.cntPreamble=0;
	ServPortRxBuffer.aryExp[0]=0;
	ServPortRxBuffer.aryExp[1]=0;
	ServPortRxBuffer.aryExp[2]=0;

	// reset layer2 flags
	SpLayer2flags.monitoringActivBit  = 0;
}




//--------------------------------------------------------------------------------------------------
/*!
\brief     Service Port receive state machine
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
void servicePortReceive(void)
{
	volatile TUSIGN8  rxdError;
	TUSIGN8  rxdData;
	TUSIGN16 u;

	// empty the receive-buffer anyway
	rxdError = 0;//U1RBH & 0xF0; // No checking. Has some unknown problem.
	//rxdData = U1RBL;
        rxdData = mSpHmiRxData;

	if( SpLayer2flags.receiverActivBit )
	{
                // Clear First response.     
                ServPortRxBuffer.firstResp = SP_RESP_OK;
		// consider communication errors
		if( rxdError )
		{
			// translete M16C-Flags to SP flags
			//Note: Too many type casts to supress lint warnings.
			if( rxdError & 0x40 ) 
			{
				ServPortRxBuffer.firstResp = (T_SP_RESP_ENUM)((TUSIGN8)ServPortRxBuffer.firstResp | (TUSIGN8)SP_PARITY_ERROR); // parity error
			}
			else{}
			if( rxdError & 0x20 )
			{
				ServPortRxBuffer.firstResp = (T_SP_RESP_ENUM)((TUSIGN8)ServPortRxBuffer.firstResp |(TUSIGN8)SP_FRAMING_ERROR); // framing error
			}
			else{}
			if( rxdError & 0x10 )
			{
				ServPortRxBuffer.firstResp = (T_SP_RESP_ENUM)((TUSIGN8)ServPortRxBuffer.firstResp |(TUSIGN8)SP_OVERRUN_ERROR); // overrun error
			}
			else{}
		}

		// receiver state machine
		switch( SpReceiveState )
		{

		case SP_RECEIVE_STATE_PREAMBLE:
			if((rxdData == 0xFF) && (!rxdError))
			{
				ServPortRxBuffer.cntPreamble++;

				// if two preambles received error free 
				if( ServPortRxBuffer.cntPreamble>=2 )
				{
					// we could be sure that a whole frame will follow
					SpReceiveState = SP_RECEIVE_STATE_DELIMITER;
				}
			}
                        else if((rxdData == UARTI_IDENT_DISPLAY_FRAME) 
                                        || (rxdData == UARTI_IDENT_DISPLAY_ECHO) 
                                        || (rxdData == UARTI_IDENT_DISPLAY_KEY_INFO)
                                        || (rxdData == UARTI_IDENT_COMMAND)
                                        || (rxdData == UARTI_IDENT_COMMAND_ECHO)
                                        || (rxdData == UARTI_IDENT_BOOT)
                                        || (rxdData == UARTI_IDENT_SHMI_READ))  //indicate the HMI display packets
                        {                    
                                ServPortRxBuffer.command = rxdData;
                                SpReceiveState = SP_RECEIVE_STATE_NUMBYTE;

                                if(ServPortRxBuffer.command==UARTI_IDENT_SHMI_READ)
                                {
                                        ServPortRxBuffer.command++;
                                        ServPortRxBuffer.command--;
                                }
                        }
			else
			{
				ServPortRxBuffer.cntPreamble=0;
			}

			break;

		case SP_RECEIVE_STATE_DELIMITER:
			// reinitialize receiver in case of an error
			if( rxdError )
			{
				InitServicePortRxStateMachine();
			}
			else if( rxdData == 0xFF ) // ignore preambles
			{
				// give oferflow no chance
				if( ServPortRxBuffer.cntPreamble<255) 
				{
					ServPortRxBuffer.cntPreamble++;
				}
				else
				{}
			}
			else if( ServPortRxBuffer.cntPreamble>=2 )// check the potential delimiter
			{
				switch( rxdData & 0x07 )
				{
				case MASTER2SLAVE: // STX-Frame
					// monitoring mode if more than 20 preambles received
					// if( ServPortRxBuffer.cntPreamble>20 )    //@@ TODO this rule was broken by a master not by us, should we really don't response ???
					// {
					//     SpLayer2Flags.monitoringActivBit  = 1;
					// }
					// consider expansionbyts
					SpExpBytesCounter = (rxdData & 0x60) >> 5;
					// store delimiter
					ServPortRxBuffer.delimiter = rxdData;

					if( rxdData & SP_LAYER2_LONGFRAME_FLAG )
					{
						SpReceiveState = SP_RECEIVE_STATE_LONGADR;
					}
					else
					{
						SpReceiveState = SP_RECEIVE_STATE_SHORTADR;
					}

					SpBufferIndex = 0; // mit adrByte 0 anfangen
					break;

				case BURST_FRAME: // Burst-Frame
				case SLAVE2MASTER: // ACK-Frame
				default:
					// any other code is an illegal code
					ServPortRxBuffer.cntPreamble = 0;
					SpLayer2flags.monitoringActivBit = 1;
					break;
				}
			}
			else
			{
				ServPortRxBuffer.cntPreamble=0;
			}
			break;

		case SP_RECEIVE_STATE_SHORTADR:
			// not the polling address of this device OR receive-error
			if( rxdError || ((rxdData & 0x3F)!= spHartPollAdrs))
			{
				SpLayer2flags.monitoringActivBit  = 1;
			}
			ServPortRxBuffer.aryAdr[0]=rxdData;
			// set address-bytes 1-4 to 0, so they don't influence the checksum. calculation
			ServPortRxBuffer.aryAdr[1]=0;
			ServPortRxBuffer.aryAdr[2]=0;
			ServPortRxBuffer.aryAdr[3]=0;
			ServPortRxBuffer.aryAdr[4]=0;

			// go on with expansion-bytes, if expansion bytes are expected
			if( SpExpBytesCounter )
			{
				SpReceiveState  = SP_RECEIVE_STATE_EXPANSION;
				SpBufferIndex = 0;
			}
			else
			{
				SpReceiveState = SP_RECEIVE_STATE_COMMAND;
			}
			break;

		case SP_RECEIVE_STATE_LONGADR:
			// in case of an error the address is uncertain --> monitoring
			if( rxdError )
			{
				SpLayer2flags.monitoringActivBit  = 1;
			}
			ServPortRxBuffer.aryAdr[SpBufferIndex] = rxdData;
			SpBufferIndex++;

			// compare address after all addressbytes are received
			if( SpBufferIndex >= sizeof(ServPortRxBuffer.aryAdr) )
			{
				// broadcast is allowed for every command  //note: We don't expect a broadcast for a service port!
				//   but still we adhere to protocol, we retain this.
				if( (ServPortRxBuffer.aryAdr[0] & 0x3F) 
                                   | ServPortRxBuffer.aryAdr[1] 
                                   | ServPortRxBuffer.aryAdr[2] 
                                   | ServPortRxBuffer.aryAdr[3] 
                                   | ServPortRxBuffer.aryAdr[4] )
				{
					if( !( ((ServPortRxBuffer.aryAdr[0] & 0x3F) == 26)
						&& (ServPortRxBuffer.aryAdr[1] == SW_REV01)))
					{
						// neither broadcast nor address --> monitoring
						SpLayer2flags.monitoringActivBit  = 1;   
					}
				}

				// go on with expansion-bytes, if expansion bytes are expected
				if( SpExpBytesCounter )
				{
					SpReceiveState  = SP_RECEIVE_STATE_EXPANSION;
					SpBufferIndex = 0;
				}
				else
				{
					SpReceiveState = SP_RECEIVE_STATE_COMMAND;
				}
			}
			break;

		case SP_RECEIVE_STATE_EXPANSION:
			ServPortRxBuffer.aryExp[SpBufferIndex] = rxdData;
			SpBufferIndex++;

			// no frame with expansion bytes handled by the device, go to monitoring mode
			SpLayer2flags.monitoringActivBit  = 1;

			// if (all expansion bytes received) go on with command
			if( SpBufferIndex >= SpExpBytesCounter )
			{
				SpReceiveState = SP_RECEIVE_STATE_COMMAND;
			}
			break;

		case SP_RECEIVE_STATE_COMMAND:
			ServPortRxBuffer.command=rxdData;

			// shorframe format is allowed only for command #0
			if( !(ServPortRxBuffer.delimiter & SP_LAYER2_LONGFRAME_FLAG) && (rxdData !=0 ) )
			{
				SpLayer2flags.monitoringActivBit  = 1;
			}
			SpReceiveState = SP_RECEIVE_STATE_NUMBYTE;
			break;

		case SP_RECEIVE_STATE_NUMBYTE:
			// an uncertain byte-cnt causes to many problems, so reinitialize receiver
			if( rxdError )
			{
				InitServicePortRxStateMachine();
			}
			else
			{
				ServPortRxBuffer.numByte = rxdData;
				// data count = 0 --> go on with checksum else with data
				if( !rxdData )
				{
					SpReceiveState  = SP_RECEIVE_STATE_CHECKSUM;
				}
				else
				{
					SpReceiveState  = SP_RECEIVE_STATE_DATA;
					SpBufferIndex = 0;
				}
			}
			break;


		case SP_RECEIVE_STATE_DATA:
			ServPortRxBuffer.aryData[SpBufferIndex]=rxdData;
			SpBufferIndex++;

			// check buffer-overflow
			if( SpBufferIndex>=sizeof(ServPortRxBuffer.aryData) )
			{
				// build error response
				ServPortRxBuffer.firstResp = SP_BUFFER_OVERFLOW; //@@ Bufferoverflow
			}
			// all data received
			else if( SpBufferIndex>=ServPortRxBuffer.numByte )
			{
				SpReceiveState  = SP_RECEIVE_STATE_CHECKSUM;
			}
			else{}
			break;


		case SP_RECEIVE_STATE_CHECKSUM:
			// frame was only monitored ? If so, we don't respond. Also, no need to validate checksum.
			if( !SpLayer2flags.monitoringActivBit )
			{

				// calculate checksum of the received data
				ServPortRxBuffer.checksum = ServPortRxBuffer.delimiter;

				// checksum over address, expansion, command, first and second response, dat
				for(u=0;
					u<   (sizeof(ServPortRxBuffer.aryAdr)
					+ sizeof(ServPortRxBuffer.aryExp)
					+ sizeof(ServPortRxBuffer.command)
					+ sizeof(ServPortRxBuffer.numByte));
				u++)
				{
					// this operation leaves the borders of aryAdr, but the structure guarantees that
					// the next data are expansion bytes, command, number of bytes, first resp,
					// second resp, data
					//lint -e{661} -e{662} Heiko Kresse 2004-11-10
					ServPortRxBuffer.checksum^=ServPortRxBuffer.aryAdr[u];
				}

				// at this moment the number of bytes couldn't be greater than the buffer size
				// cause this was checked in state SP_RECEIVE_STATE_DATA
				for(u=0; u<ServPortRxBuffer.numByte; u++)
				{
					ServPortRxBuffer.checksum ^= ServPortRxBuffer.aryData[u];
				}

				if( ServPortRxBuffer.checksum != rxdData )
				{
					// Different type casts used here, to suppress lint warning. 
					// ServPortRxBuffer.firstResp = (T_SP_RESP_ENUM)((TUSIGN8)ServPortRxBuffer.firstResp | (TUSIGN8)SP_CHECKSUM_ERROR);  //@@ longitudinal exor error
                                        ServPortRxBuffer.firstResp = SP_CHECKSUM_ERROR;  //@@ longitudinal 
				}
				else
				{
					// know in any case a response will be send
					// stop receiving
					SpLayer2flags.receiverActivBit = 0;
					// signal Layer 7 that it has something to do
                                        if(ServPortRxBuffer.command==UARTI_IDENT_DISPLAY_KEY_INFO)
                                        {
                                                SpLayer2flags.layer7requestedBit = SaveRxData_HMIKey();
                                        }
                                        else if(ServPortRxBuffer.command==UARTI_IDENT_DISPLAY_FRAME)
                                        {
                                                SpLayer2flags.layer7requestedBit = SaveRxData_HMIFrame();
                                        }
                                        else if(ServPortRxBuffer.command==UARTI_IDENT_COMMAND)
                                        {
                                                SpLayer2flags.layer7requestedBit = SaveRxData_HART();
                                        }
                                        else
                                        {
                                                SpLayer2flags.layer7requestedBit = 1;
                                        }
                                        SPRxTime = 0;   //this is used for time since receive the data
				}
			}
			// Re init state machine.
			InitServicePortRxStateMachine();
			break;

		default:
			InitServicePortRxStateMachine();
			break;
		}

		if(ServPortRxBuffer.firstResp)
		{
			// know in any case a response will be send
			// stop receiving further.
			SpLayer2flags.receiverActivBit = 0;
			InitServicePortRxStateMachine();
			SpLayer2flags.layer7requestedBit = 1;
                        SPRxTime = 0; 
		}
	}// if( receive active)

    // Enable Receive interrupt
    if(HAL_UART_Receive_IT(&HMI_UartHandle, (uint8_t*)&mSpHmiRxData, 1)!= HAL_OK)
    {
        Error_Handler();
    }
}


//--------------------------------------------------------------------------------------------------
/*!
\brief     Transmit request pending? -> A call from Layer7, to check if a tx request is pending.
\author    Ganapathi
\date      2008-01-08
\param
\return   
eTRUE: Transmit Request is Pending.

\warning
\test
test-date: 
\bug
*/
//--------------------------------------------------------------------------------------------------
TBOOL TxRequestPending(void)
{
	return ((TBOOL)SpLayer2flags.layer7requestedBit);
}
//--------------------------------------------------------------------------------------------------

