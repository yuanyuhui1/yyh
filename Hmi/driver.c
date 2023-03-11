//----------------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2004.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System					Low Level HMI

 Description			Driver Code
 						Information required to display the wanted image is assembled
 						and transmitted to the display

 Author					Martin Ashford
 Remarks
 $Id $
 </pre>
*/
//----------------------------------------------------------------------------------------------------------
//#include "..\..\implementation.h"
#include "implementation.h"

#if defined(TYPE_5_IMPLEMENTATION) || defined(TYPE_4_IMPLEMENTATION) || defined(TYPE_3_IMPLEMENTATION) || defined(TYPE_2_IMPLEMENTATION) || (defined(TYPE_1_IMPLEMENTATION) && !defined (_MSP430))
#pragma diag_suppress=Go003
#endif

#if defined(TYPE_5_IMPLEMENTATION) || defined(TYPE_4_IMPLEMENTATION) || defined(TYPE_3_IMPLEMENTATION) || defined(TYPE_2_IMPLEMENTATION) || ( defined(TYPE_1_IMPLEMENTATION) && defined(_MSP430))
#ifdef TYPE_1_IMPLEMENTATION
  #include "../../common.h"
#elif defined(TYPE_5_IMPLEMENTATION) || defined(TYPE_4_IMPLEMENTATION) || defined(TYPE_3_IMPLEMENTATION) || defined(TYPE_2_IMPLEMENTATION)
  #include "Hmi/HmiGen/VT5/source/hmi_framework_definition.h"
  
  #if(HOST_FRAMEWORK_VERSION < 0x020300)
  #include "../../../../common/interface/common_type.h"
  #else
  #include "System/Interface/common_type.h"
  #endif

  #include "Hmi/HmiGen/VT5/source/CompileTimeDefinitions.h"
  
  #pragma diag_suppress=Pm004
  #pragma diag_suppress=Pm084
  #pragma diag_suppress=Pm098
  #include "common_defs.h"
  #pragma diag_default=Pm098
  #pragma diag_default=Pm084
  #pragma diag_default=Pm004
  
  
  #include "common_structs.h"
  #include "globals.h"
  #include "HART/Layer2/Interface/layer2.h"
  #include "string.h"
  #include "driver.h"
#endif


#if defined(TYPE_5_IMPLEMENTATION) || defined (TYPE_4_IMPLEMENTATION)
#include "../../tft_controller.h"
#include "../../../../TFT_DISPLAY/GUI/inc/gdispcfg.h"
#endif

#if defined(TYPE_5_IMPLEMENTATION)
void himax_write(TUSIGN32 address, TUSIGN16 data);
#endif

#include "Hmi/Source/FixedIconDefinitions.h"

#if defined(TYPE_5_IMPLEMENTATION) || defined(TYPE_4_IMPLEMENTATION) || defined(TYPE_3_IMPLEMENTATION) || defined(TYPE_2_IMPLEMENTATION)
#pragma diag_suppress=Pm011
#pragma diag_suppress=Pm022
#include "lcd_patterns.h"
#pragma diag_default=Pm022
#pragma diag_default=Pm011
#else
#include "lcd_patterns.h"
#endif

#pragma diag_suppress=Pm077,Pm084
#include "Hmi/Source/Low_Level/Implementation/implementation_manager.h"
#pragma diag_default=Pm077,Pm084

#if defined (TYPE_4_IMPLEMENTATION) || defined (TYPE_5_IMPLEMENTATION)
#include "Hmi/Source/Low_Level/Implementation/Icons/abb_splash.h"
#endif

// Constant strings
const TINT8 abbWebMessage[] = "www.abb.com";
const TINT8 connectMessage[] = "Connecting...";

#ifndef _MSP430
// this function not size optimized beyond medium
#pragma optimize=z 6
#endif

/*------------------------------------------------------------------------------
Function:				InitController
Parameters:
	initFromMaster		Flag that determines how the initialisation was performed
Returns:				None
Description:			Initialises the display
//------------------------------------------------------------------------------*/
TBOOL InitController(TBOOL displayConnectMessage)
{
	TINT8 revBuff[10], revBuffPtr=0;
	TUSIGN8	swRevCopy = SOFTWARE_REVISION;

    Implementation_ResetController();

#if defined(DISPLAY_FLIP_HORIZONTAL)
    Implementation_InitController_Flip_H();
#elif defined(DISPLAY_FLIP_VERTICAL)
    Implementation_InitController_Flip_V();
#elif defined(DISPLAY_ROTATE_180)
    Implementation_InitController_Rotate();
#else
    Implementation_InitController();
#endif
    Implementation_DrawSplashScreen();

	// Display H/W & S/W revisions
    revBuff[revBuffPtr++] = PREFIX;		// Needs to be 'R' for release version
    Implementation_AssembleHardwareRevision();
	if(swRevCopy/100)
	{
		revBuff[revBuffPtr++] = '0' + (swRevCopy/100);
		while(swRevCopy/100) {swRevCopy-=100;}
	}
	if(swRevCopy/10)
	{
		revBuff[revBuffPtr++] = '0' + (swRevCopy/10);
		while(swRevCopy/10) {swRevCopy-=10;}
	}
	revBuff[revBuffPtr++] = '.';
	revBuff[revBuffPtr++] = '0' + swRevCopy;
	revBuff[revBuffPtr++] = 0;

	// Invert version string
	for(i=0; i<revBuffPtr-1; i++)
    {
		revBuff[i] += 128;
    }

    Implementation_ShowRevisionInfo();
    Implementation_DisplayStartMessage();
    Implementation_DisableLCD();

    Implementation_InitControllerExit();
}

#ifndef _MSP430
// this function not size optimized beyond medium
#pragma optimize=z 6
#endif
/*------------------------------------------------------------------------------
Function:				WriteStringEuropean
Parameters:				None
Returns:				None
Description:			Writes a european language string to the display.

Packet Format:
FUNCTION CODE, LENGTH, XPOS, OTHER INFO, CHARACTERS(1 - 21), CHECKSUM
//------------------------------------------------------------------------------*/
void WriteStringEuropean(void)
{
	// WriteStringEurope can be called by other functions, where the string
	// length calculation & string termination have already been performed.
	if(functionCode == WRITE_STRING_EUROPE)
	{
		numChars = lengthOfData-2;

		// Terminate string by overwriting checksum
		dataPtr[numChars] = NULL;
		string = dataPtr;

#ifdef LOW_LEVEL_USES_EXTENDED_CHARACTER_SET
#pragma diag_suppress=pm088
        numChars --;                        // decrement numChars to allow for "invchar" byte
        string ++;                          // increment pointer to string to allow for "invchar" byte
#pragma diag_default=pm088
#endif
	}

	// Set limits for given font
	switch(textInfo.field.font)
	{
		// 7 x 5
		case FONT1:
			scaleWidth=1;
			pagesToWrite = 1;
			maxCharsPerLine = FONT1_MAX_CHARS;
		break;
		// 14 x 5
		case FONT2:
			scaleWidth=1;
			pagesToWrite = 2;
			maxCharsPerLine = FONT2_MAX_CHARS;
		break;
#ifndef _PROGRAM_NONVOL
		// 21 x 15
		case FONT4:
			scaleWidth=3;
			pagesToWrite = 3;
			maxCharsPerLine = FONT4_MAX_CHARS;
		break;

		// 28 x 20
		case FONT5:
			scaleWidth=4;
			pagesToWrite = 4;
			maxCharsPerLine = FONT5_MAX_CHARS;
		break;

		default:
			pagesToWrite = 0;
			maxCharsPerLine = 0;
            break;
#endif
	}

#ifndef _PROGRAM_NONVOL
	CheckLimits();
#endif

    Implementation_BuildImageEuropean();
}

#ifdef _NONVOL_IMPLEMENTATION

#ifndef _MSP430
// this function not size optimized beyond medium
#pragma optimize=z 6
#endif
/*------------------------------------------------------------------------------
Function:				WriteStringAsian
Parameters:				None
Returns:				None
Description:			Writes an asian language string to the display.

Packet Format:
FUNCTION CODE, LENGTH, XPOS, OTHER INFO, CHARACTERS(1 - 21), CHECKSUM
//------------------------------------------------------------------------------*/
void WriteStringAsian()
{
	// WriteStringAsian can be called by other functions, where the string
	// length calculation & string termination have already been performed.
	if(functionCode == WRITE_STRING_ASIAN)
	{
		numChars = lengthOfData-2;

		// Terminate string by overwriting checksum
		dataPtr[numChars] = NULL;
		string = dataPtr;
	}

	pagesToWrite = 2;
	maxCharsPerLine = ASIAN_MAX_CHARS;

	CheckLimits();
	BuildImageAsian();
}
#endif	// #ifdef _NONVOL_IMPLEMENTATION

#ifndef _MSP430
// this function not size optimized beyond medium
#pragma optimize=z 6
#endif
/*------------------------------------------------------------------------------
Function:				WriteArray
Parameters:				None
Returns:				None
Description:			Writes a user defined array to the display

Packet Format:
FUNCTION CODE, LENGTH, XPOS, YPOS, PIXEL PATTERNS(1 - 55), CHECKSUM
//------------------------------------------------------------------------------*/
void WriteArray(void)
{
    // Account for xPos & yPos elements
    numBytesToWrite = lengthOfData-2;

    Implementation_TransmitImage(dataPtr);
}

#ifndef _MSP430
// this function not size optimized beyond medium
#pragma optimize=z 6
#endif
/*------------------------------------------------------------------------------
Function:				SetupDisplay
Parameters:				None
Returns:				None
Description:			Performs a user defined display operation. This may be any
						of the following:
						Invert display, Display on/off, Switch all pixels on,
						Reset display, Test display, Set default contrast

Packet Format:
FUNCTION CODE, LENGTH, ZERO, CHECKSUM
//------------------------------------------------------------------------------*/
void SetupDisplay(void)
{
    Implementation_EnableLCD();

	LcdWrite((TUSIGN8)(SET_DISPLAY + setupDispInfo.bit.displayOn));
	LcdWrite((TUSIGN8)(SET_INVERSE_DISPLAY + setupDispInfo.bit.invertDisplay));
	LcdWrite((TUSIGN8)(SET_ALL_PIXELS + setupDispInfo.bit.allPixelsOn));
	if(setupDispInfo.bit.defaultCont)
	{
		LcdWrite(SET_VREF_PM);
		LcdWrite(DEFAULT_CONTRAST);
	}
	if(setupDispInfo.bit.resetDisplay)
	{
		InitController(eTRUE);
	}

	if(setupDispInfo.bit.testDisplay)
	{
	}

    Implementation_DisableLCD();
}

#ifndef _MSP430
// this function not size optimized beyond medium
#pragma optimize=z 6
#endif
/*------------------------------------------------------------------------------
Function:				SetContrast
Parameters:				None
Returns:				None
Description:			Set the displays contrast to a user defined level.

Packet Format:
FUNCTION CODE, LENGTH, ZERO, CHECKSUM
//------------------------------------------------------------------------------*/
void SetContrast(void)
{
	if(temp > MAX_CONTRAST)
	{
		temp = MAX_CONTRAST;
	}

    Implementation_EnableLCD();

	LcdWrite(SET_VREF_PM);
	LcdWrite(temp);

    Implementation_DisableLCD();
}


#ifndef _MSP430
// this function not size optimized beyond medium
#pragma optimize=z 6
#endif
/*------------------------------------------------------------------------------
Function:				BlankArea
Parameters:				None
Returns:				None
Description:			Blanks a number of lines on the display.

Packet Format:
FUNCTION CODE, LENGTH, PAGE INFO, CHECKSUM
//------------------------------------------------------------------------------*/
void BlankArea(void)
{
    if((blankAreaInfo.field.error)
    || (blankAreaInfo.field.startPage > blankAreaInfo.field.endPage))
    {
		replyPacket.bit.procError = 1;
    }

	else
	{
        Implementation_BlankArea();
	}
}


#ifndef _MSP430
// this function not size optimized beyond medium
#pragma optimize=z 6
#endif
/*------------------------------------------------------------------------------
Function:				DrawSliderBar
Parameters:				None
Returns:				None
Description:			Draw a slider bar, at a given position, on the right hand
						edge of the screen. A user defined number is drawn above
						the slider bar (top right of screen).

Packet Format:
FUNCTION CODE, LENGTH, NUMBER, YPOS, PIXEL PATTERNS...(3 or 6), CHECKSUM
//------------------------------------------------------------------------------*/
void DrawSliderBar(void)
{
    Implementation_DrawSliderBar();
}

#ifndef _MSP430
// this function not size optimized beyond medium
#pragma optimize=z 6
#endif
/*------------------------------------------------------------------------------
Function:				WriteFrameTitle
Parameters:				None
Returns:				None
Description:			Draws a horizontal line across the top of the display and
						writes a string central to this line.

Packet Format (European):
FUNCTION CODE, LENGTH, CHARS...(1-21), CHECKSUM
Packet Format (Asian):
FUNCTION CODE, LENGTH, FONT, CHARS...(1-21), CHECKSUM
//------------------------------------------------------------------------------*/
void WriteFrameTitle(void)
{
	// Blank screen before writing title
	blankAreaInfo.field.startPage = 0;
	blankAreaInfo.field.endPage = 7;
	blankAreaInfo.field.error = 0;
	BlankArea();

	// Terminate string embedded in packet
	numChars = lengthOfData;

	// Remove extra byte inherent within WRITE_FRAME_TITLE_ASIAN data
	if(functionCode==WRITE_FRAME_TITLE_ASIAN)
    {
		numChars--;
    }

	if((functionCode==WRITE_FRAME_TITLE_EUROPE) && (numChars > 17))
	{
		numChars = 17;
	}
	else if((functionCode==WRITE_FRAME_TITLE_ASIAN) && (numChars > 8))
	{
		numChars = 8;
	}
    else
    {
    }
	dataPtr[numChars]=0;

    Implementation_WriteFrameTitle();
}

#ifndef _MSP430
// this function not size optimized beyond medium
#pragma optimize=z 6
#endif
/*------------------------------------------------------------------------------
Function:				DrawAndRepeat
Parameters:				None
Returns:				None
Description:			Draws a number of user defined bit patterns to the display.
						The patterns may be repeated a defined number of times.

Packet Format:
FUNCTION CODE, LENGTH, XPOS, YPOS, REPEATS, PIXEL PATTERNS(1 - 54), CHECKSUM
//------------------------------------------------------------------------------*/
void DrawAndRepeat(void)
{
#if defined (TYPE_4_IMPLEMENTATION) || defined (TYPE_5_IMPLEMENTATION)
    drawRepeat = eTRUE;
#endif

	TUSIGN8 k=0;
	j=0;

	// Calculate pattern length
	temp = lengthOfData-3;

	numBytesToWrite = numRepeats ? (numRepeats*temp)+temp : temp;

	// Ensure that pattern will fit into array
	if(numBytesToWrite > MAX_COMMON_HMI_WIDTH)
	{
		replyPacket.bit.procError = 1;
	}
	else
	{
		if(numBytesToWrite > MAX_BIT_PATTERNS)
		{
			// j holds the number of bytes to write on the second transmit
			j = numBytesToWrite - MAX_BIT_PATTERNS;
		}
		for(bitPatternIndex=0;bitPatternIndex<numBytesToWrite;bitPatternIndex+=temp)
		{
			for(k=0;k<temp;k++)
			{
				bitPattern[bitPatternIndex+k] = dataPtr[k];

				// Write MAX_BIT_PATTERNS bytes
				if(bitPatternIndex+k == MAX_BIT_PATTERNS-1)
				{
					numBytesToWrite = MAX_BIT_PATTERNS;
					Implementation_TransmitImage(bitPattern);
					bitPatternIndex = 0;
					numBytesToWrite = j;
					if((xPos + MAX_BIT_PATTERNS) < MAX_COMMON_HMI_WIDTH)
                    {
						xPos += MAX_BIT_PATTERNS;
                    }
				}
			}
		}

		// Write remainder
		if(numBytesToWrite)
        {
			Implementation_TransmitImage(bitPattern);
        }
	}

#if defined (TYPE_4_IMPLEMENTATION) || defined (TYPE_5_IMPLEMENTATION)
    drawRepeat = eFALSE;
#endif
}

#ifndef _MSP430
// this function not size optimized beyond medium
#pragma optimize=z 6
#endif
/*------------------------------------------------------------------------------
Function:				CheckLimits
Parameters:				None
Returns:				None
Description:			Performs limit checking on the WriteString..() packet.
//------------------------------------------------------------------------------*/
void CheckLimits(void)
{
	// Limit certain parameters if necessary
	if(numChars > maxCharsPerLine)
	{
		replyPacket.bit.procError=1;
		numChars = maxCharsPerLine;
	}
	if(xPos > MAX_COMMON_HMI_WIDTH)
	{
		replyPacket.bit.procError=1;
		xPos = MAX_COMMON_HMI_WIDTH;
	}
	if(yPos > MAX_PAGES)
	{
		replyPacket.bit.procError=1;
		yPos = MAX_PAGES;
	}
}

#ifdef _NONVOL_IMPLEMENTATION

#ifndef _MSP430
// this function not size optimized beyond medium
#pragma optimize=z 6
#endif
/*------------------------------------------------------------------------------
Function:		BuildImageAsian
Parameters:		None
Returns:		None
Description:	Called by WriteStringAsian(). Assembles the data to send to the
				display.
//------------------------------------------------------------------------------*/
void BuildImageAsian()
{
	TUSIGN8	blockChangeOffset;
	TUSIGN8 startingXpos = xPos;
	TBOOL block2 = eFALSE;
	TUSIGN8	asianArray[11];
	TUSIGN8 asianLangOffset = 0;

	numBytesToWrite = numChars*FONT_ASIAN_WIDTH;
	while(pagesToWrite)
   {
		for(charPos=0;charPos < numChars;charPos++)
		{
			if(block2)
				bitPatternIndex = (charPos*FONT_ASIAN_WIDTH) - (blockChangeOffset*FONT_ASIAN_WIDTH);
			else
				bitPatternIndex = charPos*FONT_ASIAN_WIDTH;

			// Check for string or line inversion
			invert = textInfo.field.invert;

			// Support for other asian languages will be added here.
			// The font will allow us to calculate the offset into the non-vol
			switch(textInfo.field.font)
			{
				case  FONT_CHINESE:
					asianLangOffset = ASIAN_FONT_TOP_PAGE_OFFSET;
				break;

				case FONT_JAPANESE:
					asianLangOffset = ASIAN_FONT_TOP_PAGE_OFFSET;
				break;

				// Default case should never occur!!!!
				default:
					asianLangOffset = ASIAN_FONT_TOP_PAGE_OFFSET;
			}

			// Get index for the start of the character within the font table
			if((string[charPos]) >= ASIAN_FIRST && (string[charPos]) <= ASIAN_LAST)
			{
				fontTableIndex = string[charPos];
			}

			// Get index for '?' character within the font table
			else
			{
				fontTableIndex = ASIAN_QUESTION_MARK;
			}
			fontTableIndex -= ASIAN_FIRST;
			fontTableIndex *= (FONT_ASIAN_WIDTH-1);
			fontTableIndex += asianLangOffset;
			if(pagesToWrite == 1)
				fontTableIndex += ASIAN_SINGLE_PAGE_SIZE;//FONT_ASIAN_WIDTH;

			if(string[charPos] != ' ')
			{
				ReadNonVolData(fontTableIndex, asianArray, 11);
			}

			for(i=0;i<(FONT_ASIAN_WIDTH-1);i++)
			{
				if(string[charPos] != ' ')
				{
					bitPattern[bitPatternIndex+i] = invert ? ~asianArray[i] : asianArray[i];
				}
				else
				{
					bitPattern[bitPatternIndex+i] = invert ? 0xFF : 0x00;
				}
			}

			// Add spacer to character
			bitPattern[bitPatternIndex+i] = invert ? 0xFF : 0x00;

			// Routine will add another x bytes to the transmit buffer, so
			// we need to ensure that the array limits will not be exceeded
			if((bitPatternIndex+(2*FONT_ASIAN_WIDTH)-1) >= MAX_BIT_PATTERNS)
			{
				numBytesToWrite = bitPatternIndex+(FONT_ASIAN_WIDTH);
				Implementation_TransmitImage(bitPattern);
				blockChangeOffset = charPos+1;
				xPos += bitPatternIndex+(FONT_ASIAN_WIDTH);
				block2 = eTRUE;
			}

			else if(charPos+1 == numChars)
			{
				numBytesToWrite = bitPatternIndex+(FONT_ASIAN_WIDTH);
				Implementation_TransmitImage(bitPattern);
			}
		}

		block2 = eFALSE;
		xPos = startingXpos;
		yPos++;
		pagesToWrite--;
	}
}
#endif // #ifdef _NONVOL_IMPLEMENTATION


#ifndef _MSP430
// this function not size optimized beyond medium
#pragma optimize=z 6
#endif
/*------------------------------------------------------------------------------
Function:				DrawIcon
Parameters:				None
Returns:				None
Description:			Draws an icon that is held in memory

Packet Format:
FUNCTION CODE, LENGTH, XPOS, YPOS, ICON NUMBER, CHECKSUM
//------------------------------------------------------------------------------*/
void DrawIcon(void)
{
    Implementation_DrawIcon();
}


#ifndef _MSP430
// this function not size optimized beyond medium
#pragma optimize=z 6
#endif
/*------------------------------------------------------------------------------
Function:			StaticTextWrite
Parameters:
	str				Pointer to the string to display
	txtInfo			Additional information relating to the string. See _text_info
					struct
	xPosition		Horizontal starting position of string
	blankScreen		Flag to indicate whether to blank the screen before writing
					the string.
Returns:			None
Description:		Writes a string to the display. This is an internal function
					call.
//------------------------------------------------------------------------------*/
void StaticTextWrite(const TINT8 *str, TUSIGN8 txtInfo, TUSIGN8 xPosition, TBOOL blankScreen)
{
	packet[0] = 0x01;

	// Build packet
	i = 0;
	while((i < 23) && (str[i]))
	{
		packet[4+i] = str[i];
		i++;
	}
	temp = (TUSIGN8)strlen((const char*)str);
	if(i == 23)
	{
		strcpy((char*)&packet[4],"ERROR 1");
		temp = 7;
	}

	packet[1] = temp+2;
	packet[2] = xPosition;
	packet[3] = txtInfo;
	packet[packet[1]+2] = 0;
	for(i=0;i<packet[1]+2;i++)
    {
		packet[packet[1]+2] += packet[i];
    }

	packet[packet[1]+2]++;

	// Blank screen if necessary
	if(blankScreen)
	{
		blankAreaInfo.byte = 0x38;
		BlankArea();
	}

	// Force write to display
	functionCode = WRITE_STRING_EUROPE;
	lengthOfData = packet[1];
	textInfo.byte = packet[3];
	xPos = packet[2];
	yPos = textInfo.field.yPos;
	dataPtr = &packet[4];
	WriteStringEuropean();
}



#ifndef _MSP430
// this function not size optimized beyond medium
#pragma optimize=z 6
#endif
/*------------------------------------------------------------------------------
Function:			ReadSwitches
Parameters:			None
Returns:			None
Description:		Does what is says!
//------------------------------------------------------------------------------*/
void ReadSwitches(void)
{
    Implementation_ReadSwitches();

	//       ------- ------- ------- ------- ------- ------- ------- -------
	// temp | bit 7 | bit 6 | bit 5 | bit 4 | bit 3 | bit 2 | bit 1 | bit 0 |
	//      |-------|-------|-------|-------|-------|-------|-------|-------|
	//      |   X   |   X   |   X   |   X   | Right | Down  |  Up   | Left  |
	//       ------- ------- ------- ------- ------- ------- ------- -------
	replyPacket.field.switches = 0;
	if ((temp&0x08)!=0) {replyPacket.bit.sw0 = 1;}	// Right
	if ((temp&0x04)!=0) {replyPacket.bit.sw3 = 1;}   // Down
	if ((temp&0x02)!=0) {replyPacket.bit.sw2 = 1;}   // Up
	if ((temp&0x01)!=0) {replyPacket.bit.sw1 = 1;}   // left
}


#ifndef _MSP430
// this function not size optimized beyond medium
#pragma optimize=z 6
#endif
/*------------------------------------------------------------------------------
Function:			LcdWrite
Parameters:
	data			Byte to write to display
Returns:			None
Description:		Writes a byte to the display
//------------------------------------------------------------------------------*/
void LcdWrite(TUSIGN8 data)
{
    Implementation_LcdWrite();
}

#if defined(TYPE_5_IMPLEMENTATION) || defined(TYPE_4_IMPLEMENTATION) || defined(TYPE_3_IMPLEMENTATION) || defined(TYPE_2_IMPLEMENTATION)
// this function not size optimized beyond medium
#pragma optimize=z 6
void Delay100uS(void)
{
    #ifndef CSPY_DEBUG
    TUSIGN16    ui;
    for (ui=0; ui< 200; ui++)
    {
    }
    #endif
}

#endif  // #if defined(TYPE_5_IMPLEMENTATION) || defined(TYPE_4_IMPLEMENTATION) || defined(TYPE_3_IMPLEMENTATION) || defined(TYPE_2_IMPLEMENTATION)


#endif	// #if defined(TYPE_4_IMPLEMENTATION) || defined(TYPE_3_IMPLEMENTATION) || defined(TYPE_2_IMPLEMENTATION) || ( defined(TYPE_1_IMPLEMENTATION) && !defined(M16C_62) && !defined(M16C_28) && !defined(M32C))

