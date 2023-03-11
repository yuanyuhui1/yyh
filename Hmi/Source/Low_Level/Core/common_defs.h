//----------------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2004.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System					Low Level HMI

 Description			Declares all #defines that are common to two or more files

 Author					Martin Ashford
 Remarks
 $Id $
 </pre>
*/
//----------------------------------------------------------------------------------------------------------

#ifndef	_COMMON_DEFS_H
#define _COMMON_DEFS_H

#ifndef _IMPLEMENTATIONDEFINITIONS
    #error implementation.h must be included
#endif

#include "Hmi/Source/Low_Level/Implementation/Version/version_control.h"

#define DEFAULT_CONTRAST			32
#define MAIN_HMI_REV_1				-1

/* -------------------------- PACKET CODES ---------------------------- */
#define WRITE_STRING_EUROPE			1
#define WRITE_ARRAY					2
#define SETUP_DISPLAY				3
#define BLANK_AREA					4
#define READ_SWITCHES				5
#define SET_CONTRAST				6
#define DRAW_SLIDER_BAR				8
#define WRITE_FRAME_TITLE_EUROPE	9
#define DRAW_AND_REPEAT				10
#define DRAW_ICON					11
#define WRITE_STRING_ASIAN			12
#define WRITE_FRAME_TITLE_ASIAN		13
#define COMMS_V2_INIT				14
#define LOW_LEVEL_INFO				15

#define INITIALISE_HMI				50



/* -------------------------- LIMITS ---------------------------- */
#define PRE_DEF_TABLE_SIZE			16
#define LANGUAGE_TABLE_SIZE			(PRE_DEF_TABLE_SIZE/2)

#if defined(TYPE_5_IMPLEMENTATION) || defined (TYPE_4_IMPLEMENTATION)
	#define MAX_BUFFER_SIZE				120
	#define MAX_BIT_PATTERNS			256	// should be 128 if smoothed font is NOT used

#elif defined(TYPE_3_IMPLEMENTATION)
    #define MAX_BUFFER_SIZE             240
	#define MAX_BIT_PATTERNS			128

#elif defined(TYPE_2_IMPLEMENTATION)
	#define MAX_BUFFER_SIZE				120
	#define MAX_BIT_PATTERNS			128

#else       // (TYPE_1_IMPLEMENTATION)
	#define MAX_BUFFER_SIZE				60
	#define MAX_BIT_PATTERNS			66

#endif

#define MAX_DATA_LENGTH				MAX_BUFFER_SIZE-3	// -function code byte, -length byte, -checksum byte


#define MAX_WRITE_STRING_LEN		24
#define MIN_WRITE_STRING_LEN		4

#define MAX_WRITE_FRAME_TITLE_LEN	22
#define MIN_WRITE_FRAME_TITLE_LEN	2


#define MAX_WRITE_ARRAY_LEN			MAX_DATA_LENGTH
#define MIN_WRITE_ARRAY_LEN			3

#define MAX_SETUP_DISP_LEN			1
#define MIN_SETUP_DISP_LEN			1

#define MAX_BLANK_AREA_LEN			1
#define MIN_BLANK_AREA_LEN			1

#define MAX_READ_SWITCHES_LEN		1
#define MIN_READ_SWITCHES_LEN		1
#define READ_SWITCHES_REPLY_LEN		1

#define MAX_SET_CONTRAST_LEN		1
#define MIN_SET_CONTRAST_LEN		1

#define MAX_DRAW_SLIDER_BAR_LEN		8
#define MIN_DRAW_SLIDER_BAR_LEN		5


#define MAX_DRAW_AND_REPEAT_LEN		MAX_DATA_LENGTH
#define MIN_DRAW_AND_REPEAT_LEN		4

#define MAX_INIT_HMI_LEN			3
#define MIN_INIT_HMI_LEN			3

#define MAX_DRAW_ICON_LEN			3
#define MIN_DRAW_ICON_LEN			3


#define MAX_LOW_LEVEL_INFO_LEN      1
#define MIN_LOW_LEVEL_INFO_LEN      1
#define	LOW_LEVEL_INFO_REPLY_LEN	3

#define MAX_COMMS_V2_INIT_LEN		3
#define MIN_COMMS_V2_INIT_LEN		3
#define	COMMS_V2_INIT_REPLY_LEN		9


// NONVOL Data BLOCK_ID values
#define BLOCK_ID_EXTENDED_EUROPEAN  0
#define BLOCK_ID_KATAKANA           1
#define BLOCK_ID_TURKISH            2
#define BLOCK_ID_RUSSIAN            3
#define BLOCK_ID_ANIMATED_ICONS     4
#define BLOCK_ID_SMALL_ICONS        5
#define BLOCK_ID_LOGO               6
#define BLOCK_ID_ASIAN              7

#define BLOCK_INFO_ALLOCATED_SIZE               11                          // Size (in Bytes) allocated for reading a BLOCK_INFO block from NONVOL
#define SMALL_ICON_ALLOCATED_SIZE               SMALL_ICON_WIDTH            // Size (in Bytes) allocated for reading a small icon from NONVOL
#define ANIMATED_ICON_ALLOCATED_SIZE            LARGE_ICON_SPLIT_WIDTH      // Size (in Bytes) allocated for reading an animated icon from NONVOL
#define EUROPEAN_CHARACTER_ALLOCATED_SIZE       FONT1_WIDTH                 // Size (in Bytes) allocated for reading a European, Katakana, Turkish or Russian character from NONVOL
#define ASIAN_CHARACTER_ALLOCATED_SIZE          FONT_ASIAN_WIDTH            // Size (in Bytes) allocated for reading an Asian character from NONVOL

// NONVOL Data NV_HEADER
#define NONVOL_HEADER_ADDR                              0   // Address in NONVOL of the header
#define NONVOL_HEADER_SIZE                              5   // Size (in Bytes) of the NONVOL header

#define NONVOL_HEADER__DATA_SIZE_MSBYTE                 0
#define NONVOL_HEADER__DATA_SIZE_LSBYTE                 1
#define NONVOL_HEADER__BLOCK_INFO_SIZE_BYTE             2
#define NONVOL_HEADER__BLOCK_INFO_VERSION_BYTE          3
#define NONVOL_HEADER__NUMBER_OF_BLOCKS_BYTE            4


// NONVOL Data BLOCK_INFO items
#define NONVOL__FIRST_BLOCK__BLOCK_ID_ADDR              NONVOL_HEADER_SIZE  // Address in NONVOL of the 1st BLOCK INFO block

#define NONVOL_BLOCK_INFO__BLOCK_ID_BYTE                0
#define NONVOL_BLOCK_INFO__BLOCK_DATA_VERSION_MSBYTE    1
#define NONVOL_BLOCK_INFO__BLOCK_DATA_VERSION_LSBYTE    2
#define NONVOL_BLOCK_INFO__CHAR_WIDTH_BYTE              3
#define NONVOL_BLOCK_INFO__CHAR_HEIGHT_BYTE             4
#define NONVOL_BLOCK_INFO__FIRST_CHAR_MSBYTE            5
#define NONVOL_BLOCK_INFO__FIRST_CHAR_LSBYTE            6
#define NONVOL_BLOCK_INFO__LAST_CHAR_MSBYTE             7
#define NONVOL_BLOCK_INFO__LAST_CHAR_LSBYTE             8
#define NONVOL_BLOCK_INFO__BLOCK_START_MSBYTE           9
#define NONVOL_BLOCK_INFO__BLOCK_START_LSBYTE           10

/* -------------------------- FONT INFO. ---------------------------- */
#define	FONT1		0	
#define	FONT2		1	
#define	FONT3		2	
#define	FONT4		3	
#define	FONT5		4	
#define	FONT6		5	

/* -------------------------- MISC ---------------------------- */
#define NO_ERRORS					0

#ifdef TYPE_1_IMPLEMENTATION
	#define TRANSMIT_BYTE(x)			while (!(IFG2 & UTXIFG0));TXBUF0=x
#else
	#define TRANSMIT_BYTE(x)            ()
#endif

#ifndef	NULL
	#define NULL						0
#endif

#if(HARDWARE_REVISION==HW_NORMAL_B_CAP_SWITCH)||(HARDWARE_REVISION==HW_EEPROM_B_CAP_SWITCH)

    #define TIMER_A                     TAR_soft
    #define COMS_TIMEOUT_1              8       // ~ 138ms
    #define COMS_TIMEOUT_2              171     // ~ 3 Seconds

    #if(HARDWARE_REVISION==HW_NORMAL_B_CAP_SWITCH)
        // Clock Pulse for latch is on Port 3.7
        #define SET_CLOCK_PULSE_FOR_LATCH   P3OUT |= 0x80
        #define RESET_CLOCK_PULSE_FOR_LATCH P3OUT &= ~0x80
    #endif

    #if(HARDWARE_REVISION==HW_EEPROM_B_CAP_SWITCH)
        // Clock Pulse for latch is on Port 3.6
        #define SET_CLOCK_PULSE_FOR_LATCH   P3OUT |= 0x40
        #define RESET_CLOCK_PULSE_FOR_LATCH P3OUT &= ~0x40
    #endif

#elif((HARDWARE_REVISION==HW_NORMAL_X_X)||(HARDWARE_REVISION==HW_EEPROM_X_X))

    #define TIMER_A                     TAR
    #define COMS_TIMEOUT_1              491     // ~ 120ms
    #define COMS_TIMEOUT_2              40960   // ~ 10 seconds

#else

    #define TIMER_A                     TAR
    #define COMS_TIMEOUT_1              491     // ~ 120ms
    #define COMS_TIMEOUT_2              12288   // ~ 3 seconds

#endif

#endif
