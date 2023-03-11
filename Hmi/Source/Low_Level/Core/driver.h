//----------------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2004.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System					Low Level HMI

 Description			Declares all defines that are specific to driver.c.
 						Forward function declarations also here

 Author					Martin Ashford
 Remarks
 $Id $
 </pre>
*/
//----------------------------------------------------------------------------------------------------------
#ifndef		_DRIVER_H
#define		_DRIVER_H

#ifndef _IMPLEMENTATIONDEFINITIONS
  #error implementation.h must be included
#endif

#if defined(TYPE_2_IMPLEMENTATION) || defined(TYPE_3_IMPLEMENTATION)
    #ifndef _COMPILETIMEDEFINITIONS
        #error compileTimeDefinitions.h must be included
    #endif
#endif

#ifdef _MSP430
#include  <msp430x12x.h>
#endif

/* --------------------------LCD COMMANDS ---------------------------- */
#define	SET_DISPLAY_ON					0xAF      // (1) same
#define	SET_DISPLAY_OFF					0xAE      // (1) same
#define SET_DISPLAY						0xAE		
#define	SET_START_LINE					0x40      // (2) same
#define	SET_PAGE_ADDR					0xB0      // (3) same

#define SET_COLUMN_ADDR_LS_NIB	        0x00      // (4.2) same
#define	SET_COLUMN_ADDR_MS_NIB	        0x10      // (4.1) same

#define READ_STATUS                               // (5)
#define WRITE_DISPLAY_DATA                        // (6)
#define READ_DISPLAY_DATA                         // (7)
#define	SET_COLUMN_MIRROR_ON	        0xA1      // (8) ADC Select
#define	SET_COLUMN_MIRROR_OFF	        0xA0      // (8) ADC Select
#define	SET_INVERSE_DISPLAY_ON	        0xA7      // (9) reverse display
#define	SET_INVERSE_DISPLAY_OFF	        0xA6      // (9) normal display
#define SET_INVERSE_DISPLAY				0xA6
#define	SET_ALL_PIXELS_ON				0xA5      // (10) same
#define	SET_ALL_PIXELS_OFF				0xA4      // (10) same
#define SET_ALL_PIXELS					0xA4
#define	SET_LCD_BIAS_RATIO_HIGH			0xA3      // (11) different values
#define	SET_LCD_BIAS_RATIO_LOW			0xA2      // (11) different values
#define START_INC_COLUMN_ADDR           0xE0      // (12) new
#define END_INC_COLUMN_ADDR             0xEE      // (13) new
#define	SYSTEM_RESET					0xE2      // (14) same
#define COMMMON_OP_MODE_SEL_HIGH        0xC8      // (15) new (SET_LCD_RAM_MAPPING?)
#define COMMMON_OP_MODE_SEL_LOW         0xC0      // (15) new (SET_LCD_RAM_MAPPING?)
#define	SET_POWER_CTRL					0x28      // (16) same
#define	REGULATOR_RESISTOR_SELECT		0x20      // (17) was vlcd to vpm gain
#define	SET_VREF_PM						0x81      // (18) same
#define SET_STAT_INDICATOR_ON           0xAD      // (19) new
#define SET_STAT_INDICATOR_OFF          0xAC      // (19) new
#define POWER_SAVE                                // (20) new
#define	NOP								0xE3      // (21) same
#define TEST_COMMAND                    0xF0      // (22) Do not use
#define TEST_MODE_RESET                 0xF0      // (23)

// For use with command 16
#define VC_OFF	0x00
#define VR_OFF	0x00
#define VF_OFF	0x00
#define VC_ON	0x04
#define VR_ON	0x02
#define VF_ON	0x01

/* -------------------------- FONT INFO. ---------------------------- */
#define FONT_CHINESE	0
#define FONT_JAPANESE	1
#define FONT1_WIDTH				6
#define FONT2_WIDTH				6
#define FONT3_WIDTH				12
#define FONT4_WIDTH				18
#define FONT5_WIDTH				24
#define FONT6_WIDTH				30
#define FONT_ASIAN_WIDTH		12      // 11 pixels for character + 1 pixel gap
#define FONT_ASIAN_WIDTH_TFT    23      // 22 pixels for character + 1 pixel gap

#define FONT1_MAX_CHARS			MAX_COMMON_HMI_WIDTH/FONT1_WIDTH
#define FONT2_MAX_CHARS			MAX_COMMON_HMI_WIDTH/FONT2_WIDTH
#define FONT3_MAX_CHARS			MAX_COMMON_HMI_WIDTH/FONT3_WIDTH
#define FONT4_MAX_CHARS			MAX_COMMON_HMI_WIDTH/FONT4_WIDTH
#define FONT5_MAX_CHARS			MAX_COMMON_HMI_WIDTH/FONT5_WIDTH
#define FONT6_MAX_CHARS			MAX_COMMON_HMI_WIDTH/FONT6_WIDTH
#define ASIAN_MAX_CHARS			10

#ifdef SINGLE_BYTE_ASIAN
#define FONT_ASIAN_WIDTH_2      FONT_ASIAN_WIDTH
#define ASIAN_MAX_CHARS_2		ASIAN_MAX_CHARS
#else
#define FONT_ASIAN_WIDTH_2      6
#define ASIAN_MAX_CHARS_2		20
#endif

#if defined(DISPLAY_SIZE_D) && defined (TYPE_4_IMPLEMENTATION)
// Can fit more titlebar characters on the TFT landscape version of the display
#define MAX_TITLBAR_CHARS   19
#else
#define MAX_TITLBAR_CHARS   17
#endif

/* -------------------------- SPECIAL CHAR VALS ---------------------------- */

#if defined(TYPE_1_IMPLEMENTATION) && defined (_MSP430)
#define	FONT_TABLE_FIRST_CHARACTER	0x06	// Up Arrow
#define	FONT_TABLE_LAST_CHARACTER	0x7F	// '~'
#endif

#define LL_EXTENDED_CHARACTER_PAGE_EUROPEAN      0x00
#define LL_EXTENDED_CHARACTER_PAGE_KATAKANA      0x20
#define LL_EXTENDED_CHARACTER_PAGE_TURKISH       0x40
#define LL_EXTENDED_CHARACTER_PAGE_RUSSIAN       0x60
#define LL_EXTENDED_CHARACTER_PAGE_LATIN1        0x80
#define LL_EXTENDED_CHARACTER_PAGE_CHINESE       0xA0

// ------- EUROPEAN CHARACTERS -------
#define START_EUROPEAN      0x00
#define FIRST_EUROPEAN      0x01
#define LAST_EUROPEAN       0xFD
#define NUMBER_OF_EUROPEAN  ((LAST_EUROPEAN - FIRST_EUROPEAN) + 1)

// ------- TURKISH CHARACTERS -------
#define START_TURKISH       (START_EUROPEAN + NUMBER_OF_EUROPEAN)
#define FIRST_TURKISH       0xA6
#define LAST_TURKISH        0xAB
#ifdef INCLUDE_TURKISH_PATTERNS
    #define NUMBER_OF_TURKISH   ((LAST_TURKISH - FIRST_TURKISH) + 1)
#else
    #define NUMBER_OF_TURKISH   0
#endif

// ------- KATAKANA CHARACTERS -------
#define START_KATAKANA          (START_TURKISH + NUMBER_OF_TURKISH)
#define FIRST_KATAKANA          0xA1
#define LAST_KATAKANA           0xDF
#ifdef INCLUDE_KATAKANA_PATTERNS
    #define NUMBER_OF_KATAKANA  ((LAST_KATAKANA - FIRST_KATAKANA) + 1)
#else
    #define NUMBER_OF_KATAKANA  0
#endif

// ------- RUSSIAN CHARACTERS -------
#define START_RUSSIAN           (START_KATAKANA + NUMBER_OF_KATAKANA)
#define FIRST_RUSSIAN           0x8A
#define LAST_RUSSIAN            0x9A
#ifdef INCLUDE_RUSSIAN_PATTERNS
    #define NUMBER_OF_RUSSIAN   ((LAST_RUSSIAN - FIRST_RUSSIAN) + 1)
#else
    #define NUMBER_OF_RUSSIAN   0
#endif

// ------- LATIN1 CHARACTERS -------
#define START_LATIN1            (START_RUSSIAN + NUMBER_OF_RUSSIAN)
#define FIRST_LATIN1            0xA0
#define LAST_LATIN1             0xFF
#ifdef INCLUDE_LATIN1_PATTERNS
    #define NUMBER_OF_LATIN1    ((LAST_LATIN1 - FIRST_LATIN1) + 1)
#else
    #define NUMBER_OF_LATIN1    0
#endif



#if defined(TYPE_1_IMPLEMENTATION) && defined (_MSP430)

#define ASIAN_FIRST			1
#if((HARDWARE_REVISION==HW_NORMAL_X_X)||(HARDWARE_REVISION==HW_EEPROM_X_X))
#define ASIAN_LAST			113
#else
#define ASIAN_LAST			115
#endif

#define ASIAN_QUESTION_MARK	96

#endif

/* ------------------------------ TEXT ACTIONS ----------------------------- */
#define INVERT_TEXT		1
#define INVERT_LINE		2
#define INVERT_CHAR		3

/* ----------------------------- SCREEN LIMITS ---------------------------- */
#define SCROLLBAR_WIDTH			3
#define MAX_CONFIG_WIDTH		MAX_COMMON_HMI_WIDTH-SCROLLBAR_WIDTH
#define MAX_PAGES				8
#ifdef DISPLAY_SIZE_E
    #define	MAX_COMMON_HMI_WIDTH	120		// Maximum width (in pixels) of the common HMI area
#else
    #define	MAX_COMMON_HMI_WIDTH	128		// Maximum width (in pixels) of the common HMI area
#endif

/* ------------------------------- CONTRAST ------------------------------- */
#define MIN_CONTRAST			0
#define MAX_CONTRAST			63

/* -------------------------------- MASKS --------------------------------- */
//#ifdef LOW_LEVEL_USES_EXTENDED_CHARACTER_SET
//#define CHAR_MASK				0xFF
//#else
//#define CHAR_MASK				0x7F
//#endif

#define INVCHAR_MASK            0x1F
#define EXTENDED_PAGE_MASK      0xE0

/* --------------------------------- MISC --------------------------------- */
#if defined(DISPLAY_ROTATE_180)
#define COLUMN_OFFSET           0
#else
#define COLUMN_OFFSET           4
#endif
#define SPLASH_WIDTH			64
#define LARGE_ICON_SPLIT_WIDTH	25
#define LARGE_ICON_WIDTH		50
#define SMALL_ICON_WIDTH		12

/* --------------------------- Lcd Hardware setup ------------------------- */
#ifdef _MSP430
#define ENABLE_LCD      P2OUT &= 0xFE;P2OUT &= 0xF7;	// CS Low, WR = low
#define DISABLE_LCD     P2OUT |= 0x01					// CS = high

// Set A0 line HIGH to indicate write of display data
#define DISPLAY_DATA    P2OUT |= 0x02

// Set A0 line LOW to indicate write of command data
#define COMMAND_DATA    P2OUT &= 0xFD
#endif

/* ---------------------------  Internal (private) functions ------------------------- */

#if defined(TYPE_5_IMPLEMENTATION) || defined(TYPE_4_IMPLEMENTATION) || defined(TYPE_3_IMPLEMENTATION) || defined(TYPE_2_IMPLEMENTATION) || defined(_MSP430)

#if defined (_NONVOL_IMPLEMENTATION) || (defined(EMBEDDED_ASIAN))
    static void BuildImageAsian(void);
#endif
static void LcdWrite(TUSIGN8 data);
static void CheckLimits(void);
static void RunTestPattern(TUSIGN8 patternNum);

#endif

/* ---------------------------  External (public) functions ------------------------- */

#if defined(TYPE_5_IMPLEMENTATION) || defined(TYPE_4_IMPLEMENTATION) || defined(TYPE_3_IMPLEMENTATION) || defined(TYPE_2_IMPLEMENTATION) || (defined(TYPE_1_IMPLEMENTATION) && defined (_MSP430))

void WriteStringEuropean(void);
#if defined (_NONVOL_IMPLEMENTATION) || (defined(EMBEDDED_ASIAN))
    void WriteStringAsian(void);
#endif
void WriteArray(void);
void SetupDisplay(void);
void SetContrast(void);
void BlankArea(void);
void DrawSliderBar(void);
void WriteFrameTitle(void);
void WritePreDefString(void);
void DrawAndRepeat(void);
TBOOL InitController(TBOOL displayConnectMessage);
void ReadSwitches(void);
void DrawIcon(void);
void StaticTextWrite(const TINT8 *str, TUSIGN8 txtInfo, TUSIGN8 xPosition, TBOOL blankScreen);

#endif

#if defined(TYPE_5_IMPLEMENTATION) || defined(TYPE_4_IMPLEMENTATION) || defined(TYPE_3_IMPLEMENTATION) || defined(TYPE_2_IMPLEMENTATION)
    static void Delay100uS(void);
#endif


#endif	// #ifndef		_DRIVER_H
