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
#include "Hmi/Source/IncludeList_Implementation.h"
//#include "..\..\implementation_HighLevel.h"
//
//#ifdef HIGH_LEVEL
//    #include "..\..\IncludeList_Implementation.h"
//#else
//    #include "..\..\implementation.h"
//#endif

#if defined(TYPE_5_IMPLEMENTATION) || defined(TYPE_4_IMPLEMENTATION) || defined(TYPE_3_IMPLEMENTATION) || defined(TYPE_2_IMPLEMENTATION) || (defined(TYPE_1_IMPLEMENTATION) && !defined (_MSP430))
    #pragma diag_suppress=Go003
#endif

// File scope definition - file contents are not compiled if the build is a type 1 build on the M16C
#if defined(TYPE_5_IMPLEMENTATION) || defined(TYPE_4_IMPLEMENTATION) || defined(TYPE_3_IMPLEMENTATION) || defined(TYPE_2_IMPLEMENTATION) || ( defined(TYPE_1_IMPLEMENTATION) && defined(_MSP430))

    #ifdef TYPE_1_IMPLEMENTATION

        #include "../../common.h"

    #elif defined(TYPE_5_IMPLEMENTATION) || defined(TYPE_4_IMPLEMENTATION) || defined(TYPE_3_IMPLEMENTATION) || defined(TYPE_2_IMPLEMENTATION)

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
        #include "globals.h"

        #if !defined(HMIGEN_RELEASE_VERSION) || !defined(HMIGEN_MAJOR_VERSION) || !defined(HMIGEN_MINOR_VERSION) || !defined(HMIGEN_CONFIG_VERSION) || (HMIGEN_MAJOR_VERSION < 12) || ((HMIGEN_MAJOR_VERSION == 12) && (HMIGEN_MINOR_VERSION < 2))
            #include "Hmi/Source/layer2.h"
        #else
            #include "Hmi/Source/IncludeList_LowLevelLayer2.h"
        #endif

        #include "string.h"
        #include "driver.h"

        #ifdef EMBEDDED_ASIAN				
            #include "../../lcd_patterns_Asian.h"
        #endif

    #endif


    #if defined(TYPE_5_IMPLEMENTATION) || defined (TYPE_4_IMPLEMENTATION)
        #include "System/Interface/system.h"

        #if(HOST_FRAMEWORK_VERSION < 0x020300)
        #include "../../../../embos/interface/rtos.h"
        #include "../../../../common/interface/assert.h"
        #else
        #include "RTOS/Interface/rtos.h"
        #include "System/Interface/assert.h"
        #endif

        #include "T_DATA_OBJ/Interface/simple_type.h"
        #include "T_DATA_OBJ/Interface/t_data_obj.h"
        #include "T_Unit/Interface/t_unit.h"
        #include "Coordinator/Interface/subsystem_idx.h"
        #include "../../tft_controller.h"
        #include "../../../../TFT_DISPLAY/GUI/inc/gdispcfg.h"
        #include "../../../../tft_display/interface/tft_display.h"
        #include "../../../../tft_display/interface/tft_display_idx.h"
    #endif

    #if defined(TYPE_5_IMPLEMENTATION)
        void himax_write(TUSIGN32 address, TUSIGN16 data);
        void himax_reset_auto_increment(void);
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
        TBOOL diagnosticHistory = eFALSE;
        TBOOL manualAdjust = eFALSE;
        TBOOL stringParam = eFALSE;
    #endif

// Constant strings
#if defined(OEM_BLANK) || defined(OEM_HMI_BUTTONS) || defined(OEM_AQUATECH)
const TINT8 abbWebMessage[] = "           ";
const TINT8 connectMessage[] = "             ";
#else
const TINT8 abbWebMessage[] = "www.abb.com";
const TINT8 connectMessage[] = "Connecting...";
#endif

#ifndef _MSP430
    #if defined (TYPE_4_IMPLEMENTATION) || defined (TYPE_5_IMPLEMENTATION)
        // no optimization to allow correct Implementation_DrawSplashScreen() operation
        #ifdef OPTIMIZE_V3_4
            #pragma optimize=size none
        #else
            #pragma optimize=z 0
        #endif
    #else
        // this function not size optimized beyond medium
        #ifdef OPTIMIZE_V3_4
        #pragma optimize=size medium
        #else
        #pragma optimize=z 6
        #endif
    #endif
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

#ifdef TYPE_1_IMPLEMENTATION
	TINT8 revBuff[10], revBuffPtr=0;
	TUSIGN8	swRevCopy = SOFTWARE_REVISION;	
#elif !defined(TYPE_4_IMPLEMENTATION) && !defined(TYPE_5_IMPLEMENTATION)
    TINT8 revBuffPtr=5;
#endif

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

#ifdef TYPE_1_IMPLEMENTATION

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

    Implementation_ShowRevisionInfo();
#endif

    Implementation_DisplayStartMessage();
    Implementation_DisableLCD();

    Implementation_InitControllerExit();
}

#ifndef _MSP430
// this function not size optimized beyond medium
#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
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
// High level HMI versions earlier than and including HMI/2002.03.02 (mainHMI_type <= 20302) use the earlier version
// High level HMI versions later than HMI/2002.03.02 (mainHMI_type > 20302) use the later version


// WRITE_FRAME_TITLE:       LATER VERSION                                           EARLY VERSION
// -----------------        --------------                                          -------------
                            // [0] function code                                    // [0] function code
                            // [1] len                                              // [1] len
  // dataPtr points here -> // [2] extendedCharacterPage                            // [2] first byte of string     <- dataPtr points here
                            // [3] first byte of string


// WRITE_STRING_EUROPE:     LATER VERSION                                           EARLY VERSION
// --------------------     --------------                                          -------------
                            // [0] function code                                    // [0] function code
                            // [1] len                                              // [1] len
                            // [2] xPos byte (blank line bit has been cleared)      // [2] xPos byte (blank line bit has been cleared)
                            // [3] textInfo byte (invert,font,yPos)                 // [3] textInfo byte (invert,font,yPos)
  // dataPtr points here -> // [4] additional info byte *SEE NOTE                   // [4] first byte of string     <- dataPtr points here
                            // [5] first byte of string

  //* NOTE additional info byte contains extendedCharacterPage & inverted char data



	// WriteStringEuropean can be called by other functions, where the string length calculation has
    // already been performed.
    if (functionCode != WRITE_FRAME_TITLE_EUROPE)
    {
        numChars = lengthOfData;
    }
	if(functionCode == WRITE_STRING_EUROPE)
	{
		numChars -= 3;          // decrement numChars to allow for xPos byte, textInfo byte & extendedCharacterPage byte
	}

    #pragma diag_suppress=Pm088
	string = dataPtr + 1;       // "string" points to first character of string - next byte after dataPtr (which points to extendedCharacterPage byte)
    #pragma diag_default=Pm088

    #ifdef TYPE_1_IMPLEMENTATION
    if (mainHMI_type <= 20302)
    {
        // EARLY VERSION - There is no extendedCharacterPage byte so ...

        // ...there was no need to allow for it when calculating "string" position
	    string --;

        // WRITE_FRAME_TITLE_EUROPE code already accounts for the extra byte, but if not called from
        // there then...
        if (functionCode != WRITE_FRAME_TITLE_EUROPE)
        {
            // ...there was no need to allow for it when calculating "numChars"
            numChars ++;
        }
    }
    #endif

    string[numChars] = NULL;   // Terminate string by overwriting checksum


	// Set limits for given font
	switch(textInfo.field.font)
	{
		case FONT1: /*  7 x  5 */   scaleWidth=1;   pagesToWrite = 1;   maxCharsPerLine = FONT1_MAX_CHARS;  break;	
		case FONT2: /* 14 x  5 */   scaleWidth=1;   pagesToWrite = 2;   maxCharsPerLine = FONT2_MAX_CHARS;  break;
        #ifndef _PROGRAM_NONVOL
		case FONT4: /* 21 x 15 */   scaleWidth=3;   pagesToWrite = 3;   maxCharsPerLine = FONT4_MAX_CHARS;  break;
		case FONT5: /* 28 x 20 */   scaleWidth=4;   pagesToWrite = 4;   maxCharsPerLine = FONT5_MAX_CHARS;  break;
		default:                                    pagesToWrite = 0;	maxCharsPerLine = 0;                break;
        #endif
	}		
	
    #ifndef _PROGRAM_NONVOL			
	CheckLimits();
    #endif
	
    Implementation_BuildImageEuropean();
}

#if defined (_NONVOL_IMPLEMENTATION) || (defined(EMBEDDED_ASIAN))

#ifndef _MSP430
// this function not size optimized beyond medium
#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
#endif					
/*------------------------------------------------------------------------------
Function:				WriteStringAsian
Parameters:				None	
Returns:				None
Description:			Writes an asian language string to the display.
						
Packet Format:						
FUNCTION CODE, LENGTH, XPOS, OTHER INFO, CHARACTERS(1 - 21), CHECKSUM
//------------------------------------------------------------------------------*/
void WriteStringAsian(void)
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


#if defined (TYPE_4_IMPLEMENTATION) || defined(TYPE_5_IMPLEMENTATION)
	pagesToWrite = 3;
#else
	pagesToWrite = 2;
#endif

	maxCharsPerLine = ASIAN_MAX_CHARS_2;	

	CheckLimits();
	BuildImageAsian();
}
#endif	// #if defined (_NONVOL_IMPLEMENTATION) || (defined(EMBEDDED_ASIAN))
					

#ifndef _MSP430
// this function not size optimized beyond medium
#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
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
#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
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
    #if defined(TYPE_1_IMPLEMENTATION) && defined (_MSP430)
	if(setupDispInfo.bit.backlightOn)
    {
        P2OUT = P2OUT ^ 0x20;
    }
    else
    #endif
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
}

#ifndef _MSP430
// this function not size optimized beyond medium
#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
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
#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
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
#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
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
#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
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

// High level HMI versions earlier than and including HMI/2002.03.02 (mainHMI_type <= 20302) use the earlier version
// High level HMI versions later than HMI/2002.03.02 (mainHMI_type > 20302) use the later version


// WRITE_FRAME_TITLE:       LATER VERSION                                           EARLY VERSION
// -----------------        --------------                                          -------------
                            // [0] function code                                    // [0] function code
                            // [1] len                                              // [1] len
  // dataPtr points here -> // [2] extendedCharacterPage                            // [2] first byte of string <- dataPtr points here
                            // [3] first byte of string

    // WRITE_FRAME_TITLE_ASIAN:
    // [0] function code
    // [1] len
    // [2] character Set
    // [3] first byte of string     <- dataPtr points here

	// calculate length of string
    // Assume the length of the string is length of data (from byte[1]) less the information byte (extendedCharacterPage or character Set) like the LATER VERSIONM
    numChars = lengthOfData - 1;
    		
	if(functionCode==WRITE_FRAME_TITLE_EUROPE)
    {
        #ifdef TYPE_1_IMPLEMENTATION
        if (mainHMI_type <= 20302)
        {                           // EARLY VERSION
    	    numChars ++;            // - There is no extendedCharacterPage byte so there was no need to allow for the information byte
            i = 0;                  // - There is no offset between "dataPtr" and the start of the string
        }
        else
        #endif
        {                           // LATER VERSION
            i = 1;                  // - There is a 1 byte offset between "dataPtr" and the start of the string
        }

        if (numChars > MAX_TITLBAR_CHARS)
    	{
	    	numChars = MAX_TITLBAR_CHARS;
	    }

	    dataPtr[numChars+i]=0;  // Terminate string embedded in packet by overwriting checksum
    }
    else
    {
        #ifdef SINGLE_BYTE_ASIAN
            if (numChars > 8)
            {
                numChars = 8;
            }
        #else
	        if (numChars > 16)
	        {
		        numChars = 16;
            }
        #endif
        dataPtr[numChars]=0;  // Terminate string embedded in packet by overwriting checksum
	}
    Implementation_WriteFrameTitle();
}

#ifndef _MSP430
// this function not size optimized beyond medium
#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
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
#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
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

#if defined(_NONVOL_IMPLEMENTATION) || defined(EMBEDDED_ASIAN)

#ifndef _MSP430
// this function not size optimized beyond medium
#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
#endif
/*------------------------------------------------------------------------------
Function:		BuildImageAsian
Parameters:		None	
Returns:		None
Description:	Called by WriteStringAsian(). Assembles the data to send to the
				display.
//------------------------------------------------------------------------------*/
void BuildImageAsian(void)
{			
    numBytesToWrite = numChars * FONT_ASIAN_WIDTH_2;

	invert = textInfo.field.invert;
	TUSIGN8 startingXpos = xPos;

    #if defined (TYPE_4_IMPLEMENTATION) || defined(TYPE_5_IMPLEMENTATION)
    numBytesToWrite = (numChars/2) * FONT_ASIAN_WIDTH_TFT;
    writeText = eTRUE;
    AsianString = eTRUE;
    TUSIGN8 yPos_Start = yPos;
//    numBytesToWrite *= 2;

    xPos *= 2;
    startingXpos = xPos;

    // Find inverted character span
    if (invert)
    {
	    // String or line inversion (character inversion not available for Asian)
        characterInvertStartIndex = 0;

        characterInvertEndIndex = (characterInvertStartIndex + numBytesToWrite);  // Highlight the whole line
        if (numChars > 6)
        {
            characterInvertEndIndex -= 2;  // Highlight the whole line but not the last pixel so that there is a gap between highlight & slider bar
        }
    }
    else
    {
        characterInvertStartIndex = -1;
        characterInvertEndIndex   = -1;
    }
    #endif

    #ifdef _NONVOL_IMPLEMENTATION
    TUSIGN16 firstSupportedChar = 0xFFFF;
    TUSIGN16 lastSupportedChar  = 0x0000;

        #if (BLOCK_INFO_ALLOCATED_SIZE >= ASIAN_CHARACTER_ALLOCATED_SIZE)
            TUSIGN8  nonVolData[BLOCK_INFO_ALLOCATED_SIZE];
        #else
            TUSIGN8  nonVolData[ASIAN_CHARACTER_ALLOCATED_SIZE];
        #endif

    // Get the ASIAN Block Info
    TUSIGN16 readAddr = GetBlockInfo(BLOCK_ID_ASIAN, nonVolData);

    if (readAddr)
    {
        // Extract the start address and the supported character ID's from the BLOCK_DATA
        readAddr           = ((TUSIGN16)nonVolData[NONVOL_BLOCK_INFO__BLOCK_START_MSBYTE] << 8) + nonVolData[NONVOL_BLOCK_INFO__BLOCK_START_LSBYTE];
        firstSupportedChar = ((TUSIGN16)nonVolData[NONVOL_BLOCK_INFO__FIRST_CHAR_MSBYTE]  << 8) + nonVolData[NONVOL_BLOCK_INFO__FIRST_CHAR_LSBYTE];
        lastSupportedChar  = ((TUSIGN16)nonVolData[NONVOL_BLOCK_INFO__LAST_CHAR_MSBYTE]   << 8) + nonVolData[NONVOL_BLOCK_INFO__LAST_CHAR_LSBYTE];
    }

    #endif

    while(pagesToWrite)
    {
        #if defined (TYPE_4_IMPLEMENTATION) || defined(TYPE_5_IMPLEMENTATION)
        yPos = ((yPos_Start + 1) * 3) - pagesToWrite;
        #endif

        bitPatternIndex = 0;
        for(charPos=0;charPos < numChars;charPos++)
        {
            #ifndef SINGLE_BYTE_ASIAN
                fontTableIndex = ((TUSIGN16)string[charPos] << 8);
                charPos ++;
                fontTableIndex += ((TUSIGN16)string[charPos]);
                fontTableIndex -= 0x0100;
            #else
                fontTableIndex = string[charPos] * (FONT_ASIAN_WIDTH - 1);
            #endif

            // fontTableIndex now contains the character code

            #ifdef _NONVOL_IMPLEMENTATION
            if ((fontTableIndex < firstSupportedChar)
             || (fontTableIndex > lastSupportedChar)
             || (readAddr == 0))
            {
                // Asian character not valid - draw a square box
                for (i = 0; i < FONT_ASIAN_WIDTH; i ++)
                {
                    nonVolData[i] = pagesToWrite == 2 ? 0xFC : 0x3F;
                }
            }
            else
            {
                // Asian Character is valid - read bit patterns from NONVOL
                // (1) calculate Offset into BLOCK_DATA
                fontTableIndex *= (FONT_ASIAN_WIDTH - 1);
                fontTableIndex *= 2;
                fontTableIndex += pagesToWrite == 1 ? FONT_ASIAN_WIDTH - 1 : 0;
                // (2) Add BLOCK_DATA start location
                fontTableIndex += readAddr;
                // (2) read bit patterns
                ReadNonVolData(fontTableIndex, nonVolData, FONT_ASIAN_WIDTH - 1);
            }
            // "fontTableIndex" will be used as offset into "nonVolData" so start at zero
            fontTableIndex = 0;

            #else

                #if defined (TYPE_4_IMPLEMENTATION) || defined(TYPE_5_IMPLEMENTATION)
                // "fontTableIndex" will be used as offset into "AsianPatternTable" so calculate Offset
                fontTableIndex *= (FONT_ASIAN_WIDTH - 1);
                fontTableIndex *= 2;


                #else
                // "fontTableIndex" will be used as offset into "AsianPatternTable" so calculate Offset
                fontTableIndex *= (FONT_ASIAN_WIDTH - 1);
                fontTableIndex *= 2;
                fontTableIndex += pagesToWrite == 1 ? FONT_ASIAN_WIDTH - 1 : 0;
                #endif

            #endif

            // Add bit patterns to bitPattern[]
            for (i = 0; i < (FONT_ASIAN_WIDTH - 1); i ++)
            {
                #if defined(_NONVOL_IMPLEMENTATION)

                    bitPattern[bitPatternIndex] = nonVolData[fontTableIndex];

                #elif defined (TYPE_4_IMPLEMENTATION) || defined(TYPE_5_IMPLEMENTATION)

//                    switch (pagesToWrite)
//                    {
//                    case 3:     bitPattern[bitPatternIndex] = (AsianPatternTable[fontTableIndex] & 0x0F) << 4;                                                                                  break;
//                    case 2:     bitPattern[bitPatternIndex] = ((AsianPatternTable[fontTableIndex] & 0xF0) >> 4) | ((AsianPatternTable[fontTableIndex + (FONT_ASIAN_WIDTH - 1)] & 0x0F) << 4);   break;
//                    case 1:     bitPattern[bitPatternIndex] = (AsianPatternTable[fontTableIndex + (FONT_ASIAN_WIDTH - 1)] & 0xF0) >> 4;                                                         break;
//                    default:    bitPattern[bitPatternIndex] = 0;                                                                                                                                break;
//                    }

                    TUSIGN16 fontTableIndex_2 = fontTableIndex + (FONT_ASIAN_WIDTH - 1);
                    bitPattern[bitPatternIndex]   = 0;
                    bitPattern[bitPatternIndex+1] = 0;
                    switch (pagesToWrite)
                    {
                    case 3:
                        if (AsianPatternTable[ fontTableIndex ] & 0x04)   {bitPattern[bitPatternIndex] |= 0x03; bitPattern[bitPatternIndex+1] |= 0x03;}
                        if (AsianPatternTable[ fontTableIndex ] & 0x08)   {bitPattern[bitPatternIndex] |= 0x0C; bitPattern[bitPatternIndex+1] |= 0x0C;}
                        if (AsianPatternTable[ fontTableIndex ] & 0x10)   {bitPattern[bitPatternIndex] |= 0x30; bitPattern[bitPatternIndex+1] |= 0x30;}
                        if (AsianPatternTable[ fontTableIndex ] & 0x20)   {bitPattern[bitPatternIndex] |= 0xC0; bitPattern[bitPatternIndex+1] |= 0xC0;}
                        break;

                    case 2:
                        if (AsianPatternTable[ fontTableIndex ] & 0x40)   {bitPattern[bitPatternIndex] |= 0x03; bitPattern[bitPatternIndex+1] |= 0x03;}
                        if (AsianPatternTable[ fontTableIndex ] & 0x80)   {bitPattern[bitPatternIndex] |= 0x0C; bitPattern[bitPatternIndex+1] |= 0x0C;}
                        if (AsianPatternTable[fontTableIndex_2] & 0x01)   {bitPattern[bitPatternIndex] |= 0x30; bitPattern[bitPatternIndex+1] |= 0x30;}
                        if (AsianPatternTable[fontTableIndex_2] & 0x02)   {bitPattern[bitPatternIndex] |= 0xC0; bitPattern[bitPatternIndex+1] |= 0xC0;}
                        break;

                    case 1:
                        if (AsianPatternTable[fontTableIndex_2] & 0x04)   {bitPattern[bitPatternIndex] |= 0x03; bitPattern[bitPatternIndex+1] |= 0x03;}
                        if (AsianPatternTable[fontTableIndex_2] & 0x08)   {bitPattern[bitPatternIndex] |= 0x0C; bitPattern[bitPatternIndex+1] |= 0x0C;}
                        if (AsianPatternTable[fontTableIndex_2] & 0x10)   {bitPattern[bitPatternIndex] |= 0x30; bitPattern[bitPatternIndex+1] |= 0x30;}
                        if (AsianPatternTable[fontTableIndex_2] & 0x20)   {bitPattern[bitPatternIndex] |= 0xC0; bitPattern[bitPatternIndex+1] |= 0xC0;}
                        break;

                    default:
                        bitPattern[bitPatternIndex]   = 0;
                        bitPattern[bitPatternIndex+1] = 0;
                        break;
                    }

                #else

                    bitPattern[bitPatternIndex] = AsianPatternTable[fontTableIndex];

                #endif

                if (invert)
                {
                    bitPattern[bitPatternIndex] ^= 0xFF;
                    #if defined (TYPE_4_IMPLEMENTATION) || defined(TYPE_5_IMPLEMENTATION)
                    bitPattern[bitPatternIndex +1] ^= 0xFF;
                    #endif
                }

                bitPatternIndex ++;
                #if defined (TYPE_4_IMPLEMENTATION) || defined(TYPE_5_IMPLEMENTATION)
                bitPatternIndex ++;
                #endif
                fontTableIndex ++;
            }

            // Add a spacer

            if (invert)
            {
                #if defined (TYPE_4_IMPLEMENTATION) || defined(TYPE_5_IMPLEMENTATION)
                if (bitPatternIndex >= characterInvertEndIndex)
                {
                    bitPattern[bitPatternIndex] = 0x00;
                }
                else
                #endif
                {
                    bitPattern[bitPatternIndex] = 0xFF;
                }
            }
            else
            {
                bitPattern[bitPatternIndex] = 0x00;
            }
            bitPatternIndex ++;

            if (charPos + 1 < numChars)
            {
                // character that has been processed is not the last character so at least 12 (FONT_ASIAN_WIDTH) more bytes will be added to bitPattern[]

                #if defined (TYPE_4_IMPLEMENTATION) || defined(TYPE_5_IMPLEMENTATION)
                if ((bitPatternIndex + FONT_ASIAN_WIDTH + FONT_ASIAN_WIDTH) > MAX_BIT_PATTERNS)
                #else
                if ((bitPatternIndex + FONT_ASIAN_WIDTH) > MAX_BIT_PATTERNS)
                #endif
                {
                    // Next character will not fit into bitPattern[] - So transmit what we have
                    numBytesToWrite = bitPatternIndex;

                    #if defined (TYPE_4_IMPLEMENTATION) || defined(TYPE_5_IMPLEMENTATION)

                    if ((functionCode != WRITE_FRAME_TITLE_EUROPE) && (functionCode != WRITE_FRAME_TITLE_ASIAN))
                    {
                        switch (yPos)
                        {
                        case 0:     case 1:     case 2:     textLineShift = ROW_SEPARATION * -2;    break;      // Line 0
                        case 3:     case 4:     case 5:     textLineShift = ROW_SEPARATION * -1;    break;      // Line 1
                        case 6:     case 7:     case 8:     textLineShift = ROW_SEPARATION *  0;    break;      // Line 2
                        case 9:     case 10:    case 11:    textLineShift = ROW_SEPARATION *  1;    break;      // Line 3
                        case 12:    case 13:    case 14:    textLineShift = ROW_SEPARATION *  2;    break;      // Line 4
                        case 15:    case 16:    case 17:    textLineShift = ROW_SEPARATION *  3;    break;      // Line 5
                        case 18:    case 19:    case 20:    textLineShift = ROW_SEPARATION *  4;    break;      // Line 6
                        default:                            textLineShift = 0;                      break;
                        }
                    }
                    #endif


                    Implementation_TransmitImage(bitPattern);

                    // And Start a new transmission
                    xPos += bitPatternIndex;
                    bitPatternIndex = 0;
                }
            }
        }
        // All characters processed - transmit image
        numBytesToWrite = bitPatternIndex;

        #if defined (TYPE_4_IMPLEMENTATION) || defined(TYPE_5_IMPLEMENTATION)
        if ((functionCode != WRITE_FRAME_TITLE_EUROPE) && (functionCode != WRITE_FRAME_TITLE_ASIAN))
        {
            switch (yPos)
            {
            case 0:     case 1:     case 2:     textLineShift = -4;     break;      // Line 0
            case 3:     case 4:     case 5:     textLineShift = -2;     break;      // Line 1
            case 6:     case 7:     case 8:     textLineShift =  0;     break;      // Line 2
            case 9:     case 10:    case 11:    textLineShift =  2;     break;      // Line 3
            case 12:    case 13:    case 14:    textLineShift =  4;     break;      // Line 4
            case 15:    case 16:    case 17:    textLineShift =  6;     break;      // Line 5
            case 18:    case 19:    case 20:    textLineShift =  8;     break;      // Line 6
            default:                            textLineShift =  0;     break;
            }
        }
        #endif

        Implementation_TransmitImage(bitPattern);

        // Start the next page
		xPos = startingXpos;
        yPos ++;
        pagesToWrite --;
    }
#if defined (TYPE_4_IMPLEMENTATION) || defined(TYPE_5_IMPLEMENTATION)
    writeText = eFALSE;
    AsianString = eFALSE;
#endif

}
#endif // end of #if defined(_NONVOL_IMPLEMENTATION) || defined(EMBEDDED_ASIAN)



#ifndef _MSP430
// this function not size optimized beyond medium
#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
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
#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
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
// WRITE_STRING_EUROPE:     LATER VERSION                                           EARLY VERSION
// --------------------     --------------                                          -------------
                            // [0] function code                                    // [0] function code
                            // [1] len                                              // [1] len
                            // [2] xPos byte (blank line bit has been cleared)      // [2] xPos byte (blank line bit has been cleared)
                            // [3] textInfo byte (invert,font,yPos)                 // [3] textInfo byte (invert,font,yPos)
  // dataPtr points here -> // [4] additional info byte                             // [4] first byte of string     <- dataPtr points here
                            // [5] first byte of string


    packet[4] = LL_EXTENDED_CHARACTER_PAGE_EUROPEAN;    // This will be overwritten if EARLY VERSION is detected

	temp = (TUSIGN8)strlen((const char*)str);
    if (temp > 20)
    {
        temp = 20;
    }

    j = 5;                      // j = position in packet[] of first byte of string (allowing for additional info byte)

    #ifdef TYPE_1_IMPLEMENTATION
    if (mainHMI_type <= 20302)
    {
        j --;                  // EARLY VERSION - does not support additional info byte so no need to allow for it
    }
    #endif


    strncpy((char *)&packet[j],(const char *)str,(size_t)temp);

    // Function Code
	packet[0] = WRITE_STRING_EUROPE;

    // Length
    packet[1] = temp + (j - 2);         // EARLY VERSION - (j = 4) so length = length of string + 2    LATER VERSION - (j = 5) so length = length of string + 3

    // xPosition
	packet[2] = xPosition;

    // textInfo
	packet[3] = txtInfo;

    // additional info byte
    // already entered (and overwritten if EARLY VERSION is detected)

    // string data
    // already entered

    // Checksum
    packet[temp + j] = 1;
	for(i = 0; i < (temp + j); i ++)
    {
		packet[temp + j] += packet[i];
    }

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
#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
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
#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
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
#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
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


#endif	// #if defined(TYPE_5_IMPLEMENTATION) || defined(TYPE_4_IMPLEMENTATION) || defined(TYPE_3_IMPLEMENTATION) || defined(TYPE_2_IMPLEMENTATION) || ( defined(TYPE_1_IMPLEMENTATION) && defined(_MSP430))
