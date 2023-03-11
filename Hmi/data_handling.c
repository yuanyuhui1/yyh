//----------------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2004.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System					Low Level HMI

 Description			Data Handling.
						Is responsible for checking validity of packets and
						extracting information within the packet. Interrupt error
						processing & Rom check is also performed here.

 Author					Martin Ashford
 Remarks
 $Id $
 </pre>
*/
//----------------------------------------------------------------------------------------------------------

//#include "..\..\implementation.h"
#include "implementation.h"

#if defined(TYPE_5_IMPLEMENTATION) || defined(TYPE_4_IMPLEMENTATION) || defined (TYPE_3_IMPLEMENTATION) || defined (TYPE_2_IMPLEMENTATION)
#pragma diag_suppress=Go003
#endif

// File scope definition - file contents are not compiled if the build is a type 1 build on the M16C
#if defined(TYPE_5_IMPLEMENTATION) || defined(TYPE_4_IMPLEMENTATION) || defined (TYPE_3_IMPLEMENTATION) || defined (TYPE_2_IMPLEMENTATION) ||  defined (_MSP430)

#ifdef TYPE_1_IMPLEMENTATION
#include "../../common.h"
#elif defined(TYPE_2_IMPLEMENTATION) || defined(TYPE_3_IMPLEMENTATION) || defined(TYPE_4_IMPLEMENTATION) || defined(TYPE_5_IMPLEMENTATION)
#include "Hmi/HmiGen/VT5/source/hmi_framework_definition.h"

#if(HOST_FRAMEWORK_VERSION < 0x020300)
#include "../../../../common/interface/common_type.h"
#else
#include "System/Interface/common_type.h"
#endif

#pragma diag_suppress=Pm004
#pragma diag_suppress=Pm084
#pragma diag_suppress=Pm098
#include "common_defs.h"
#pragma diag_default=Pm098
#pragma diag_default=Pm084
#pragma diag_default=Pm004

#include "common_structs.h"
#include "globals.c"
#include "globals.h"
////
#include "string.h"
////
#include "data_handling.h"
#include "driver.h"
#endif

const TINT8 englishErrorMessage[] = "Communications Error";


//--------------------------------------------------------------------------------------------------
// Function definitions for devices that have an integrated HMI
// i.e. Display is driven using the main processor
//--------------------------------------------------------------------------------------------------
#if defined(TYPE_2_IMPLEMENTATION) || defined(TYPE_3_IMPLEMENTATION) || defined(TYPE_4_IMPLEMENTATION) || defined(TYPE_5_IMPLEMENTATION)

// this function not size optimized beyond medium
#pragma optimize=z 6
void LowLevelDriverExe(void)
{
    TUSIGN8 lastPacketProcessed = 0;
    numPacketsProcessed = 0;

    ReadSwitches();



    while ((numPacketsProcessed != numPacketsReceived) && (lastPacketProcessed == 0))
    {
		numPacketsProcessed++;
		if(PacketErrorCheck() == NO_ERRORS)
		{
			WriteToDisplay();
		}
		// Check for 'reply request' packet
		if((packet[processPacketStartPos]&0x80) != 0)
		{
		    lastPacketProcessed = 1;
		}
		else
		{
    		// Point to next packet
			processPacketStartPos += (lengthOfData+3);
		}
	}
}

// this function not size optimized beyond medium
#pragma optimize=z 6
TUSIGN8 LowLevelDriver_Reply(void)
{
    TUSIGN8 replyByte = replyPacket.byte;
	numPacketsProcessed = numPacketsReceived = 0;
	processPacketStartPos = rxPacketStartPos = 0;
	rxByteNum = 0;
	rxComplete = eFALSE;

	replyPacket.byte = 0;

    return replyByte;
}

// this function not size optimized beyond medium
#pragma optimize=z 6
void LowLevelDriver_Receive(TUSIGN8 receivedByte)
{
    if (!rxComplete)
    {
        TUSIGN8 rxPacketLengthPos = rxPacketStartPos+1;

        packet[rxByteNum] = receivedByte;

        if (rxByteNum == rxPacketStartPos)
        {
    	    if(((packet[rxPacketStartPos]&0x7F) > COMMS_V2_INIT) || ((packet[rxPacketStartPos]&0x7F) < WRITE_STRING_EUROPE))
            {
                intrptErrors.bit.funcCodeError = eTRUE;
            }
        }
        else
        {
            if (rxByteNum == rxPacketLengthPos)
            {
                if(packet[rxPacketLengthPos] > MAX_DATA_LENGTH)
                {
                    intrptErrors.bit.lengthError = eTRUE;
                }
            }
        }

        rxByteNum ++;

		if(intrptErrors.byte)
		{
			rxComplete = eTRUE;
		}

        // Check for end of packet
        else
        {
            if(rxByteNum == (rxPacketStartPos+packet[rxPacketLengthPos]+3))
            {
                // Check whether function code contains 'send reply' bit
                if((packet[rxPacketStartPos]&0x80) != 0)
                {
                    rxComplete = eTRUE;
                    rxByteNum = rxPacketStartPos = 0;
                }
                else
                {
                    rxPacketStartPos += (packet[rxPacketLengthPos]+3);
                }

                numPacketsReceived++;
            }
        }
    }
	// Should not receive byte after receiving 'send reply' packet
	// ignore received byte
    else
    {
        intrptErrors.bit.synchError = eTRUE;
    }
}
#endif


//--------------------------------------------------------------------------------------------------
// Function definitions for devices that DO NOT have an integrated HMI
// i.e. Display is driven using dedicated processor (msp430)
//--------------------------------------------------------------------------------------------------
#ifdef TYPE_1_IMPLEMENTATION

/*------------------------------------------------------------------------------
Function:			CheckForPacketToProcess
Parameters:			None
Returns:			TRUE - if a new packet has been received
					FALSE - otherwise
Description:		Inspects the data received within the interrupt routine &
					determines whether there is a valid new packet to process.
//------------------------------------------------------------------------------*/
TUSIGN8 CheckForPacketToProcess(void)
{
	// Check if there is a packet waiting to be processed
	if(numPacketsProcessed != numPacketsReceived)
	{
		numPacketsProcessed++;
		if(PacketErrorCheck() == NO_ERRORS)
		{
			WriteToDisplay();
		}

		// Check for 'reply request' packet
		if(packet[processPacketStartPos]&0x80)
		{
			TransmitReply();
		}
		else
		{
			// Point to next packet
			processPacketStartPos += (lengthOfData+3);
		}
		return 1;
	}
	else
	{
		return 0;
	}
}

/*------------------------------------------------------------------------------
Function:			TransmitReply
Parameters:			None
Returns:			None
Description:		Transmits a reply to the master. Re-initialises packet data.
//------------------------------------------------------------------------------*/
void TransmitReply()
{
	// 1. Old style Main HMI only supports single byte replies
	if(mainHMI_type == MAIN_HMI_REV_1)
	{
		TRANSMIT_BYTE(replyPacket.byte);
	}

	// 2. New style Main HMI asking for nonvol information (always first received message)
	else if((packet[processPacketStartPos]&0x7F) == COMMS_V2_INIT)
	{
#ifdef 	_NONVOL_IMPLEMENTATION
		// Use 'packet' buffer to store nonvol header. This is ok as a reply has been
		// requested & the current packet information has been processed & may be
		// overwritten
		ReadNonVolData(0, packet, NON_VOL_DATA_INFO_SIZE);
#else
		memset(packet,0,NON_VOL_DATA_INFO_SIZE);
#endif
		// Calculate nonvol part of checksum
		temp = 1;
		for(i=0; i<NON_VOL_DATA_INFO_SIZE; i++)
			temp += packet[i];

		// Need to send...
		// FUNCTION CODE, LENGTH, ERROR INFO, NONVOL HEADER (8 BYTES), CHECKSUM
		TRANSMIT_BYTE(COMMS_V2_INIT);			// Function Code
		TRANSMIT_BYTE(COMMS_V2_INIT_REPLY_LEN);	// Length
		TRANSMIT_BYTE(replyPacket.byte);		// Switch/Error Info.

		// Nonvol Header
		for(i=0; i < NON_VOL_DATA_INFO_SIZE; i++)
		{
			TRANSMIT_BYTE(packet[i]);
		}

		// Checksum
		TRANSMIT_BYTE(COMMS_V2_INIT + COMMS_V2_INIT_REPLY_LEN + replyPacket.byte + temp);
	}

	// 3. New style Main HMI asking for Version information
	else if((packet[processPacketStartPos]&0x7F) == LOW_LEVEL_INFO)
	{
		TRANSMIT_BYTE(LOW_LEVEL_INFO);
		TRANSMIT_BYTE(LOW_LEVEL_INFO_REPLY_LEN);
		TRANSMIT_BYTE(replyPacket.byte);		// Switch/Error Info.
		TRANSMIT_BYTE(HARDWARE_REVISION);
		TRANSMIT_BYTE(SOFTWARE_REVISION);
		TRANSMIT_BYTE(LOW_LEVEL_INFO + LOW_LEVEL_INFO_REPLY_LEN + replyPacket.byte + HARDWARE_REVISION + SOFTWARE_REVISION +1);
    }

	// 4. New style Main HMI asking for switch/error information
	else
	{
		// Need to send...
		// FUNCTION CODE, LENGTH, SWITCH/ERROR INFO, CHECKSUM
		TRANSMIT_BYTE(READ_SWITCHES);
		TRANSMIT_BYTE(READ_SWITCHES_REPLY_LEN);
		TRANSMIT_BYTE(replyPacket.byte);
		TRANSMIT_BYTE(READ_SWITCHES + READ_SWITCHES_REPLY_LEN + replyPacket.byte + 1);
	}

	numPacketsProcessed = numPacketsReceived = 0;
	processPacketStartPos = rxPacketStartPos = 0;
	rxByteNum = 0;
	rxComplete = eFALSE;
	replyPacket.byte = 0;
}

/*------------------------------------------------------------------------------
Function:			CommsErrorCheck
Parameters:			None
Returns:			None
Description:		Checks for errors that may have been reported within the
					interrupt routine. Also checks that communication between
					master and slave is still apparent.
				 	NOTE: Function is only called when system is 'idle'.
//------------------------------------------------------------------------------*/
void CommsErrorCheck(void)
{
	// Check for errors within interrupt
	if(intrptErrors.byte)
	{
		// Need to ensure that UHTE transmission has stopped
		if(TIMER_A > COMS_TIMEOUT_1)		// No chars received for 120ms
		{
			if(intrptErrors.field.commErrors)
			{
				replyPacket.bit.commsError = 1;
			}
			if(intrptErrors.field.pktErrors)
			{
				replyPacket.bit.procError = 1;
			}
			intrptErrors.byte = 0;
			TransmitReply();
			TIMER_A = 0;
		}
	}
	else if(TIMER_A > COMS_TIMEOUT_2)
	{
        #if((HARDWARE_REVISION==HW_NORMAL_X_X)||(HARDWARE_REVISION==HW_EEPROM_X_X))
        // Version x.2.2 (compatible with x.2.1)
        // No comms for 10 seconds...

		// Assemble error packet
		StaticTextWrite(englishErrorMessage,0x0B,4,eTRUE);

		// Send error
		replyPacket.byte = 0;
		replyPacket.bit.commsError = 1;
		TransmitReply();

		TIMER_A = 0;

        #else
        // Version x.2.3 onwards
        // No comms for 2 seconds...

		TIMER_A = 0;
        // Software Reset
        // Force PUC by causing watchdog timer security key violation by writing invalid data to WDTCTL
    	WDTCTL = 0x0000;
        #endif
    }
}

/*------------------------------------------------------------------------------
Function:			RomCheck
Parameters:			None
Returns:			None
Description:		Checks the ROM for possible corruption. Forces the master to
				 	issue a reset if a problem is found.
				 	NOTE: Function is only called when system is 'idle'.
//------------------------------------------------------------------------------*/
void RomCheck(void)
{
	calcRomCS += *((TUSIGN8*)(address));
	address++;

	// end of info memory - set to start of main memory
	if(address == 0x1100)
		address = 0xE000;

	// location of checksum - set to start of interrupt vectors
	else if(address == 0xFFDE)
		address = 0xFFE0;

	// rollover - end of main memory
	else if(address == 0x0000)
	{
		// start of info memory
		address = 0x1000;
		if(calcRomCS != *((TUSIGN16*)0xFFDE))
		{
			replyPacket.bit.dispError = 1;
			TransmitReply();
		}
		calcRomCS = 0;
	}
}
#endif	// #ifdef TYPE_1_IMPLEMENTATION


//--------------------------------------------------------------------------------------------------
// Common Function Definitions
//--------------------------------------------------------------------------------------------------
#ifndef _MSP430
// this function not size optimized beyond medium
#pragma optimize=z 6
#endif
/*------------------------------------------------------------------------------
Function:			PacketErrorCheck
Parameters:			None
Returns:			Errors identified whilst checking the packet
Description:		Checks for function code error, function length error &
					checksum error. The packet is discarded if an error is
					detected, and an error is reported to the master
//------------------------------------------------------------------------------*/
TUSIGN8 PacketErrorCheck(void)
{
	// Calculate data length
	lengthOfData = packet[processPacketStartPos+1];

	// Perform checksum calculation
	temp = 0;
	checksum = packet[processPacketStartPos+lengthOfData+2];
	for(i=0;i<lengthOfData+2;i++)
	{
		temp += packet[processPacketStartPos+i];
	}
	temp++;

#ifndef _MODULE_TESTING
	// Invalid checksum
	if(checksum != temp)
	{
		replyPacket.bit.commsError = 1;
	}
#endif
	// Perform data length & function code check
	temp = 0;
	functionCode = packet[processPacketStartPos]&0x7F;

	switch(functionCode)
	{
			case WRITE_STRING_EUROPE:
#ifdef _NONVOL_IMPLEMENTATION
			case WRITE_STRING_ASIAN:
#endif
			if((lengthOfData > MAX_WRITE_STRING_LEN) || (lengthOfData < MIN_WRITE_STRING_LEN))
				{
					replyPacket.bit.procError = 1;
				}
			break;

			case WRITE_ARRAY:
				if((lengthOfData > MAX_WRITE_ARRAY_LEN) || (lengthOfData < MIN_WRITE_ARRAY_LEN))
				{
					replyPacket.bit.procError = 1;
				}
			break;

			case SETUP_DISPLAY:
			case READ_SWITCHES:
			case BLANK_AREA:
			case SET_CONTRAST:
				if((lengthOfData > MAX_SETUP_DISP_LEN) || (lengthOfData < MIN_SETUP_DISP_LEN))
				{
					replyPacket.bit.procError = 1;
				}
			break;

			case DRAW_SLIDER_BAR:
				if((lengthOfData > MAX_DRAW_SLIDER_BAR_LEN) || (lengthOfData < MIN_DRAW_SLIDER_BAR_LEN))
				{
					replyPacket.bit.procError = 1;
				}
				break;

			case WRITE_FRAME_TITLE_EUROPE:
				if((lengthOfData > MAX_WRITE_FRAME_TITLE_LEN) || (lengthOfData < MIN_WRITE_FRAME_TITLE_LEN))
				{
					replyPacket.bit.procError = 1;
				}
				break;
#ifdef _NONVOL_IMPLEMENTATION
			case WRITE_FRAME_TITLE_ASIAN:
				if((lengthOfData > (MAX_WRITE_FRAME_TITLE_LEN+1)) || (lengthOfData < (MIN_WRITE_FRAME_TITLE_LEN+1)))
				{
					replyPacket.bit.procError = 1;
				}
				break;
#endif
			case DRAW_AND_REPEAT:
				if((lengthOfData > MAX_DRAW_AND_REPEAT_LEN) || (lengthOfData < MIN_DRAW_AND_REPEAT_LEN))
				{
					replyPacket.bit.procError = 1;
				}
				break;

			case DRAW_ICON:
				if((lengthOfData > MAX_DRAW_ICON_LEN) || (lengthOfData < MIN_DRAW_ICON_LEN))
				{
					replyPacket.bit.procError = 1;
				}
				break;

			case COMMS_V2_INIT:
				if((lengthOfData > MAX_COMMS_V2_INIT_LEN) || (lengthOfData < MIN_COMMS_V2_INIT_LEN))
				{
					replyPacket.bit.procError = 1;
				}
				break;

            case LOW_LEVEL_INFO:
				if((lengthOfData > MAX_LOW_LEVEL_INFO_LEN) || (lengthOfData < MIN_LOW_LEVEL_INFO_LEN))
				{
					replyPacket.bit.procError = 1;
				}
				break;

			default:
				replyPacket.bit.procError = 1;
			break;
	}

	return replyPacket.field.errors;
}

#ifndef _MSP430
// this function not size optimized beyond medium
#pragma optimize=z 6
#endif
/*------------------------------------------------------------------------------
Function:			WriteToDisplay
Parameters:			None
Returns:			None
Description:		Called if initial checks on packet indicate that the packet is
					valid. Sets up horizontal & vertical starting position, assigns
					a pointer to the data, and calls the relevant function.
//------------------------------------------------------------------------------*/
void WriteToDisplay(void)
{
	// Extract packet data
	switch(functionCode)
	{
		case WRITE_STRING_EUROPE:
		case WRITE_STRING_ASIAN:
			textInfo.byte = packet[processPacketStartPos+3];

			// Check for blank line bit
			if((packet[processPacketStartPos+2]&0x80)!=0)
			{
				blankAreaInfo.field.startPage = textInfo.field.yPos;
				blankAreaInfo.field.endPage = blankAreaInfo.field.startPage;
				blankAreaInfo.field.error = 0;
				if(functionCode == WRITE_STRING_EUROPE)
				{
					switch(textInfo.field.font)
					{
					case FONT1:
						break;

					case FONT2:
						blankAreaInfo.field.endPage++;
						break;

					case FONT4:
						blankAreaInfo.field.endPage += 2;
						break;

					case FONT5:
						blankAreaInfo.field.endPage += 3;
						break;

					case FONT6:
						blankAreaInfo.field.endPage += 5;
						break;

					default:
						blankAreaInfo.field.error = 1;
                        break;
					}
				}

				// Asian chars are always two pages high
				else
                {
					blankAreaInfo.field.endPage++;
                }

				BlankArea();
			}
			// Remove 'blank line' bit
			xPos = packet[processPacketStartPos+2]&0x7F;
			yPos = textInfo.field.yPos;
			dataPtr = &packet[processPacketStartPos+4];

#ifdef _NONVOL_IMPLEMENTATION
			if(functionCode == WRITE_STRING_ASIAN)
				WriteStringAsian();
			else
#endif
				WriteStringEuropean();
		break;

		case WRITE_ARRAY:
			xPos = packet[processPacketStartPos+2];
			yPos = packet[processPacketStartPos+3];
			dataPtr = &packet[processPacketStartPos+4];
			WriteArray();
		break;

		case SETUP_DISPLAY:
			setupDispInfo.byte = packet[processPacketStartPos+2];
			SetupDisplay();
		break;

		case BLANK_AREA:
			blankAreaInfo.byte = packet[processPacketStartPos+2];
			BlankArea();
		break;

		case READ_SWITCHES:
        case LOW_LEVEL_INFO:
		break;

		// COMMS_V2_INIT contains contrast info., therefore no 'break'!
        #if defined (M16C_62)
        #pragma diag_suppress=Pm057
        #endif
		case COMMS_V2_INIT:
			// Get New Style Main HMI revision number
			mainHMI_type = packet[processPacketStartPos+3] + (packet[processPacketStartPos+4]*256);
        #if defined (M16C_62)
        #pragma diag_default=Pm057
        #endif

		case SET_CONTRAST:
			temp = packet[processPacketStartPos+2];
			SetContrast();
		break;

		case DRAW_SLIDER_BAR:
			dataPtr = &packet[processPacketStartPos+2];
			DrawSliderBar();
		break;

		case WRITE_FRAME_TITLE_EUROPE:
			dataPtr = &packet[processPacketStartPos+2];
			WriteFrameTitle();
		break;

		case WRITE_FRAME_TITLE_ASIAN:
			dataPtr = &packet[processPacketStartPos+3];
			WriteFrameTitle();
		break;

		case DRAW_AND_REPEAT:
			xPos = packet[processPacketStartPos+2];
			yPos = packet[processPacketStartPos+3];
			numRepeats = packet[processPacketStartPos+4];
			dataPtr = &packet[processPacketStartPos+5];
			DrawAndRepeat();
		break;

		case DRAW_ICON:
			xPos = packet[processPacketStartPos+2];
			yPos = packet[processPacketStartPos+3];
			dataPtr = &packet[processPacketStartPos+4];
			DrawIcon();
            break;

		default:
			break;
	}
}

#endif	// #if defined(TYPE_5_IMPLEMENTATION) || defined(TYPE_4_IMPLEMENTATION) || defined (TYPE_3_IMPLEMENTATION) || defined (TYPE_2_IMPLEMENTATION) ||  defined (_MSP430)


