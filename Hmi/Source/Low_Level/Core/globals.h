//----------------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2004.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System					Low Level HMI

 Description			Global data externs declared here. This method allows us to
 						place all global data into one file, eliminating the need to
 						scatter the code with 'extern' references.

 Author					Martin Ashford
 Remarks
 $Id $
 </pre>
*/
//----------------------------------------------------------------------------------------------------------
#ifndef _GLOBALS_H
#define _GLOBALS_H

#ifdef TYPE_1_IMPLEMENTATION
#include "common.h"
#endif

extern TUSIGN8 bitPattern[MAX_BIT_PATTERNS];
#if defined (TYPE_4_IMPLEMENTATION) || defined(TYPE_5_IMPLEMENTATION)
extern TUSIGN16 numBytesToWrite;
extern TUSIGN16 xPos;
extern TBOOL writeText;
    #if defined (TYPE_4_IMPLEMENTATION) || defined (TYPE_5_IMPLEMENTATION)
    extern TINT16 characterInvertStartIndex;
    extern TINT16 characterInvertEndIndex;
    extern TBOOL drawIcon;
    extern TBOOL drawTitleBar;
    extern TBOOL drawRepeat;
    extern TBOOL configHeaderInit;
    extern TUSIGN8 configHeaderType;
    extern TBOOL AsianString;
    #endif
extern TINT16  textLineShift;

#else
extern TUSIGN8 numBytesToWrite;
extern TUSIGN8 xPos;
#endif
extern TUSIGN8 bitPatternIndex;
extern TUSIGN8 numChars;
extern TUSIGN8 *string;
extern TUSIGN8 charPos;
extern TUSIGN8 scaleWidth;
extern TUSIGN8 pagesToWrite;
extern TUSIGN8 srcBitPattern;
extern TUSIGN8 scaledBitPattern;
extern TUSIGN8 invert;
extern TUSIGN8 maxCharsPerLine;
extern TUSIGN8 lengthOfData;
extern TUSIGN8 yPos;
extern TUSIGN8 checksum;
extern TUSIGN8 temp;
extern TUSIGN8 processPacketStartPos;
extern TUSIGN16  fontTableIndex;
extern TUSIGN8  i;
extern TUSIGN8  j;
extern TUSIGN8 *dataPtr;
extern TUSIGN8 numPacketsReceived ;
extern TUSIGN8 numPacketsProcessed ;
extern TUSIGN8 functionCode;
extern TUSIGN8 numRepeats;
extern TUSIGN16 calcRomCS;
extern TUSIGN16 address;

extern _reply_packet replyPacket;
extern _text_info	  textInfo;
extern _blank_area   blankAreaInfo;
extern _setup_display_info	setupDispInfo;
extern TINT16	mainHMI_type;

#if defined(TYPE_2_IMPLEMENTATION)
extern TUSIGN8 ucWriteData;
#endif

#ifdef TYPE_5_IMPLEMENTATION
extern TUSIGN16 colourTable[];
    #ifdef DISPLAY_SIZE_D
    extern TBOOL blankForTopSoftkeyRedraw_LLHMI;
    #endif
#endif

// Variables used in interrupt (asynchronously accessed)
// Ensure that modification of these vars in interrupt does not affect code flow elsewhere!!!!
//											// MOD. INTERRUPT		MOD. ELSEWHERE		READ
extern _intrpt_errors intrptErrors;
extern TUSIGN8 rxByteNum;
extern TUSIGN8 rxPacketStartPos;
extern TUSIGN8 packet[MAX_BUFFER_SIZE];
extern TUSIGN8 rxComplete;

#if(HARDWARE_REVISION==HW_NORMAL_B_CAP_SWITCH)||(HARDWARE_REVISION==HW_EEPROM_B_CAP_SWITCH)
extern TUSIGN16 TAR_soft;
extern TUSIGN8 keys_mask;
extern TUSIGN8 keys_power;
extern TUSIGN8 keys_cnt;
extern TUSIGN8 keys_data;
#endif

#endif
