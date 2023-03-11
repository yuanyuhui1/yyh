#ifndef _DRIVER_HW_TYPE_5_H_
#define _DRIVER_HW_TYPE_5_H_

#ifndef _COMPILETIMEDEFINITIONS
    #include "Hmi/Source/compileTimeDefinitions.h"
#endif
#ifndef _HMI_LAYER2_H
    #include "Hmi/Source/layer2.h"
#endif

#include "Hmi/Source/FrameTableDefinitions.h"
#include "Hmi/Source/FrameTable.h"

#define Implementation_ResetController()        \
{                                               \
	init_tft_controller();                      \
}

#define Implementation_ReadSwitches()       \
{                                           \
    temp = Layer2_ReadSwitches();           \
}

#define Implementation_WriteFrameTitle()                    \
{                                                           \
    diagnosticHistory = eFALSE;                             \
    manualAdjust = eFALSE;                                                           \
    stringParam = eFALSE;                                                            \
                                                            \
    /* Write Title String at left of display (middle) */    \
    drawTitleBar = eTRUE;                                   \
    string = dataPtr;                                       \
  	textInfo.field.font = 0;                                \
	textInfo.field.invert = 0;                              \
	xPos = FRAME_TITLE_START_X;                             \
    yPos = 0;                                               \
                                                            \
    /*WriteStringEuropean();*/		                            \
    Implementation_WriteString();                           \
                                                            \
    drawTitleBar = eFALSE;                                  \
                                                            \
    TUSIGN8 armSecurityPassed;                                   \
    T_UNIT const SLOW *pSub = GetSubsystemPtr(TFT_IDX);     \
    pSub->Get(pSub, TFT_IDX_armSecurityPassed,0,(void FAST *)&armSecurityPassed);\
    if(armSecurityPassed)                                        \
    {                                                       \
        /* Draw menu icon */                                                                                            \
        const FRAME_FORMAT_TYPE * pCurrDisplayFrame = FrameTableGetCurrentFrame();                                      \
        TUSIGN8 frameType = pCurrDisplayFrame->frameType;                                                               \
        TUSIGN8 spec_menu = pCurrDisplayFrame->frameDefinition.menuFrame.specialMenuFlag;                               \
                                                                                                                        \
        configHeaderInit = (TBOOL)(frameType == FRAME_TYPE_CONFIG_HEADER);                                              \
        diagnosticHistory = (TBOOL)((frameType == FRAME_TYPE_MENU) && ((spec_menu == 5) || (spec_menu == 6)));          \
        manualAdjust = (TBOOL)(frameType == FRAME_TYPE_MANUAL_ADJUST);                                                  \
        stringParam = (TBOOL)((frameType == FRAME_TYPE_STRING_PARAM) || (frameType == FRAME_TYPE_STRING_PARAM_PACKED)); \
                                                                                                                        \
        if(configHeaderInit == eFALSE)                          \
        {                                                       \
            /* Draw icon */                                     \
            TUSIGN32 addrOffset = TITLEBAR_ICON_OFFSET;         \
            const TUSIGN8 MAX_COLS = TITLEBAR_ICON_WIDTH;       \
            const TUSIGN8 MAX_ROWS = TITLEBAR_ICON_HEIGHT;      \
            const TUSIGN16 tableOffset = configHeaderType*(TITLEBAR_ICON_WIDTH*TITLEBAR_ICON_HEIGHT/2);\
            /* Draw icon */                                     \
            TUSIGN16 counter = 0;                                           \
            for(TUSIGN16 row=0; row < MAX_ROWS; row++)                      \
            {                                                               \
                for(TUSIGN16 col=0; col<MAX_COLS; col+=2)                   \
                {                                                           \
                    TUSIGN8 byte = titlebar_icons[tableOffset + counter];   \
                    for(TUSIGN8 nib = 0; nib < 4; nib+=2)                   \
                    {                                                       \
                        TUSIGN8 tableIndex = nib ? (byte & 0x0F) : ((byte & 0xF0) >> 4);    \
                        DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+addrOffset,colourTable[tableIndex]);\
                    }                                                       \
                    counter++;                                              \
                }                                                           \
                addrOffset += BYTES_PER_ROW;                                \
            }                                                               \
        }                                                                   \
    }                                                                       \
}

#define Implementation_DrawSplashScreen()   \
{                                           \
    /* Blank Entire screen */               \
    TUSIGN16 MAX_COLS = GDISPW;             \
    TUSIGN16 MAX_ROWS = GDISPH;             \
    unsigned char const * psplash = ABB_splash_definition.ABB_splash_Address; \
    for(TUSIGN16 j=0; j<MAX_ROWS; j++)      \
    {                                       \
        for(TUSIGN32 i=0; i<MAX_COLS; i++)   \
        {                                       \
            DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET,0xFFFF); \
        }                                       \
    }                                           \
    /* Draw ABB splash */                       \
    MAX_COLS = ABB_splash_definition.ABB_splash_Width;                \
    MAX_ROWS = ABB_splash_definition.ABB_splash_Height;               \
    TUSIGN32 addrOffset = ABB_splash_definition.ABB_splash_Start;     \
    TUSIGN16 counter=0;                         \
    for(TUSIGN16 row=0; row < MAX_ROWS; row++)                      \
    {                                                               \
        for(TUSIGN16 col=0; col<MAX_COLS; col+=2)                   \
        {                                                           \
            TUSIGN8 byte = psplash[counter];                     \
            for(TUSIGN8 nib = 0; nib < 4; nib+=2)                   \
            {                                                       \
                TUSIGN8 tableIndex = nib ? (byte & 0x0F) : ((byte & 0xF0) >> 4);    \
                DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+addrOffset,colourTable[tableIndex]);\
            }                                                       \
            counter++;                                              \
        }                                                           \
        addrOffset += BYTES_PER_ROW;                                \
    }                                                               \
}

#define Implementation_SetTransmitImageStartPosition(xpos)
#define Implementation_SetSplashStartPosition(xpos)
#define Implementation_GetSplashPattern()           CommonMacros_GetSplashPattern()
#define Implementation_EnableLCD()
#define Implementation_DisableLCD()
#define Implementation_InitControllerExit()         CommonMacros_InitControllerExitTFT()
#define Implementation_DisplayStartMessage()
#define Implementation_ShowRevisionInfo()
#define Implementation_AssembleHardwareRevision()
#define Implementation_DrawSliderBarLowerDigit()    CommonMacros_DrawSliderBarLowerDigitTFT()
#define Implementation_DrawSliderBarUpperDigit()    CommonMacros_DrawSliderBarUpperDigitTFT()
#define Implementation_LcdWrite()

#define Implementation_ScaleFont2()                 CommonFunctions_ScaleFont2TFT()
#define Implementation_ScaleFont4()                 CommonFunctions_ScaleFont4TFT()
#define Implementation_ScaleFont5()                 CommonFunctions_ScaleFont5TFT()
#define Implementation_BuildImageEuropean()         CommonFunctions_BuildImageEuropeanTFT()
#define Implementation_GetFontPattern(index)
#define Implementation_TransmitImage(ptr_image)     Implementation_TransmitImage_Type5(ptr_image)
#define Implementation_DrawIcon()                   Implementation_DrawIcon_Type5()
#define Implementation_BlankArea()                  Implementation_BlankArea_Type5()
#define Implementation_DrawSliderBar()              Implementation_DrawSliderBar_Type5()

#define IMPLEMENTATION_MAX_WRITE_BYTES              MAX_COMMON_HMI_WIDTH
#define IMPLEMENTATION_MAX_WRITES                   1
#define IMPLEMENTATION_INC_Y_POS                    1


//--------------------------------------------------------------------------------------------------
// Implementation specific macros
//--------------------------------------------------------------------------------------------------
// Port Configuration
#define reset       P9_bit.P9_5
#define rs          P9_bit.P9_6

#define _16_BPP			// 16 bpp
//#define DISPLAY_START_ADDR	0x40000		// TFT display - dummy address
                                        // Use External space0

#define FOREGROUND_COL  	0xF800      // Red colour
#define BACKGROUND_COL      0xFFFF      // White colour

#define PAGE_PIXEL_HEIGHT	       	8	// TFT display - page height

#define DISPLAY_WRITE(addr,data)		(himax_write((TUSIGN32)(DISPLAY_START_ADDR-addr+GDISP_ADDR_OFFSET), data))
#define DISPLAY_READ(addr)		        (*((TUSIGN16 volatile*)(addr)))

// Common HMI colours
#define INVERT_COL_FORE         0x000B    // Dark blue
#define INVERT_COL_BACK         0xFFFF    // White
#define DEFAULT_COL_FORE        0x0000    // Black
#define DEFAULT_COL_BACK        0xAD75    // Light grey
#define SOFTKEY_COL_FORE        0x6B6D    // Dark grey
#define TITLEBAR_COL_FORE       0xFFFF    // White
#define TITLEBAR_COL_BACK       0x001F    // Blue
#define DRAWREPEAT_COL_FORE     0x000B    // Dark blue

/*
  #define G_BLACK       0x0000
  #define G_RED         0xF800
  #define G_YELLOW      0xFFE0
  #define G_GREEN       0x07E0
  #define G_CYAN        0x07FF
  #define G_BLUE        0x001F
  #define G_MAGENTA     0xF81F
  #define G_WHITE       0xFFFF

  #define G_DARKCYAN    0x030B
  #define G_LIGHTGREY   0xAD75
  #define G_DARKMAGENTA 0x600B
  #define G_DARKGREY    0x6B6D
  #define G_DARKYELLOW  0x6300
  #define G_DARKGREEN   0x0300
  #define G_DARKBLUE    0x000B
  #define G_DARKRED     0x6000
*/

#define ANIMATION_ICON_WIDTH    100
#define ANIMATION_ICON_HEIGHT   48
#define MAX_CONFIG_HEADERS      15
#define SOFTKEY_START_PAGE      21
#define COMMON_HMI_WIDTH_MID    64

#define ghw_wr_no_auto_inc(val)  \
/* Upper byte of row start address  */  \
rs = 0;  *(TUSIGN16 volatile*)(DISPLAY_START_ADDR) = 0x0006;    \
rs = 1;  *(TUSIGN16 volatile*)(DISPLAY_START_ADDR) = (lty&0xFF00)>>8;   \
    \
/* Lower byte of row start address */   \
rs = 0;  *(TUSIGN16 volatile*)(DISPLAY_START_ADDR) = 0x0007;    \
rs = 1;  *(TUSIGN16 volatile*)(DISPLAY_START_ADDR) = lty+Y_ROW_OFFSET;   \
    \
/* Upper byte of column start address */    \
rs = 0;  *(TUSIGN16 volatile*)(DISPLAY_START_ADDR) = 0x0002;    \
rs = 1;  *(TUSIGN16 volatile*)(DISPLAY_START_ADDR) = (x&0xFF00)>>8; \
    \
/* Lower byte of column start address */    \
rs = 0;  *(TUSIGN16 volatile*)(DISPLAY_START_ADDR) = 0x0003;    \
rs = 1;  *(TUSIGN16 volatile*)(DISPLAY_START_ADDR) = x; \
    \
/* Latch data */    \
rs = 0;  *(TUSIGN16 volatile*)(DISPLAY_START_ADDR) = 0x0022;    \
rs = 1; \
*(TUSIGN16 volatile*)(DISPLAY_START_ADDR) = val;

#endif  // #ifndef _DRIVER_HW_TYPE_5_H_
