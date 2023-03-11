//----------------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2004.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System					Low Level HMI

 Description			Global data is declared here. This method allows us to
 						place all global data into one file, eliminating the need to
 						scatter the code with 'extern' references (using globals.h).

 Author					Martin Ashford
 Remarks
 $Id $
 </pre>
*/
//----------------------------------------------------------------------------------------------------------
#include "Hmi/Source/Implementation.h"

#ifdef TYPE_1_IMPLEMENTATION
#include "../../common.h"
#endif

TUSIGN8 bitPattern[MAX_BIT_PATTERNS];			// Patterns generated from font table
#if defined (TYPE_4_IMPLEMENTATION) || defined(TYPE_5_IMPLEMENTATION)
TUSIGN16 numBytesToWrite=0;						// Number of bytes to be transmitted to display
TUSIGN16 xPos=0;								// Horizontal starting position of string (in pixels)
TBOOL writeText = eFALSE;
    #if defined (TYPE_4_IMPLEMENTATION) || defined (TYPE_5_IMPLEMENTATION)
    TINT16 characterInvertStartIndex = -1;
    TINT16 characterInvertEndIndex = -1;
    TBOOL drawIcon = eFALSE;
    TBOOL drawTitleBar = eFALSE;
    TBOOL drawRepeat = eFALSE;
    TBOOL configHeaderInit = eFALSE;
    TUSIGN8 configHeaderType = 0;
    TBOOL AsianString = eFALSE;
    #endif
TINT16  textLineShift = 0;
#else
TUSIGN8 numBytesToWrite=0;						// Number of bytes to be transmitted to display
TUSIGN8 xPos=0;									// Horizontal starting position of string (in pixels)
#endif
TUSIGN8 bitPatternIndex=0;						// Index used for array bitPattern
TUSIGN8 numChars=0;								// Number of characters within a string
TUSIGN8 *string=0;								// String to be displayed
TUSIGN8 charPos=0;								// Position of character within string
TUSIGN8 scaleWidth=0;							// Multiples of standard width
TUSIGN8 pagesToWrite=0;							// Number of pages for font
TUSIGN8 srcBitPattern=0;						// An assigned element of the font table
TUSIGN8 scaledBitPattern=0;						// Bit manipulated copy of srcBitPattern
TUSIGN8 invert=0;								// Inversion options
TUSIGN8 maxCharsPerLine=0;						// Max. number of characters for a given font
TUSIGN8 lengthOfData=0;							// Length of data within packet being processed
TUSIGN8 yPos=0;									// Vertical position of data (in pages)
TUSIGN8 checksum=0;								// Checksum received in packet
TUSIGN8 temp=0;									// Temporary variable
TUSIGN8 processPacketStartPos=0;				// Index of packet that is being processed
TUSIGN16  fontTableIndex=0;						// Index used to access font table
TUSIGN8  i=0;									// Loop increment/decrement var
TUSIGN8  j=0;									// Loop increment/decrement var
TUSIGN8 *dataPtr;								// Pointer to information within the packet
TUSIGN8 numPacketsReceived = 0;					// Number of packets received
TUSIGN8 numPacketsProcessed = 0;				// Number of packets processed
TUSIGN8 functionCode=0;							
TUSIGN8 numRepeats=0;							// Used in DrawAndRepeat
TUSIGN16 calcRomCS=0;							// Calculated ROM checksum
TUSIGN16 address=0x1000;						// ROM address for checksum calculation
_reply_packet replyPacket;						// Reply packet info.
_text_info	  textInfo;							// Y position, font size & invert
_blank_area   blankAreaInfo;
_setup_display_info	setupDispInfo;
TINT16	mainHMI_type = MAIN_HMI_REV_1;			// Type of Main HMI (Default = 'old' style)

#if defined(TYPE_2_IMPLEMENTATION)
TUSIGN8 ucWriteData = 0;
#endif

#ifdef TYPE_5_IMPLEMENTATION
TUSIGN16 colourTable[] = {  0x0000,0xF800,0xFFE0,0x07E0,0x07FF,0x001F,0xF81F,0xFFFF,
                            0x030B,0xAD75,0x600B,0x6B6D,0x6300,0x0300,0x000B,0x6000
};
    #ifdef DISPLAY_SIZE_D
    TBOOL blankForTopSoftkeyRedraw_LLHMI;
    #endif
#endif

// Variables used in interrupt (asynchronously accessed)
// Ensure that modification of these vars in interrupt does not affect code flow elsewhere!!!!
//											// MOD. INTERRUPT		MOD. ELSEWHERE		READ
_intrpt_errors intrptErrors;				// Yes					Yes					Main
TUSIGN8 rxByteNum=0;						// Yes					Yes					No
TUSIGN8 rxPacketStartPos=0;					// Yes					Yes					No
TUSIGN8 rxComplete=0;						// Yes					Yes					No
TUSIGN8 packet[MAX_BUFFER_SIZE];			// Yes					Yes					Main

#if(HARDWARE_REVISION==HW_NORMAL_B_CAP_SWITCH)
TUSIGN16 TAR_soft=0;                        // Yes                  Yes                 No
TUSIGN8 keys_mask=1;                            // toggle mask for SYNC signal of key sensors
TUSIGN8 keys_power=0;                           // power bits for latch data
TUSIGN8 keys_cnt=0;                             // state machine counter
TUSIGN8 keys_data=0xF0;                         // latch data
#endif
