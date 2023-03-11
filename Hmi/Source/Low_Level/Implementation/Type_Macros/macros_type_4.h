#ifndef _DRIVER_HW_TYPE_4_H_
#define _DRIVER_HW_TYPE_4_H_

#ifndef _COMPILETIMEDEFINITIONS
    #include "Hmi/Source/compileTimeDefinitions.h"
#endif
#include "Hmi/Source/FrameTableDefinitions.h"
#include "Hmi/Source/FrameTable.h"

#define Implementation_ResetController()                        \
{                                                               \
    Layer2_Display_ActivateHWreset();                           \
    Delay100uS();                                               \
    Layer2_Display_DeactivateHWreset();                         \
    Delay100uS();                                               \
	                                                            \
	/* Init SSD1906	graphic controller & HIMAX controller */    \
	if(!init_tft_controller())                                  \
    {                                                           \
	    return eFALSE;                                          \
    }                                                           \
}

#define Implementation_ReadSwitches()       \
{                                           \
    temp = Layer2_ReadSwitches();           \
}
#ifndef _OEM_RELEASE
    #define Implementation_DrawSplashScreen()                                                               \
    {                                                                                                       \
        /* Blank Entire screen */                                                                           \
        TUSIGN16 MAX_COLS = GDISPW;                                                                         \
        TUSIGN16 MAX_ROWS = GDISPH;                                                                         \
        TUSIGN32 addrOffset = 0;                                                                            \
        unsigned char const * psplash = ABB_splash_definition.ABB_splash_Address;                           \
        for(TUSIGN32 j=0; j<MAX_ROWS; j++)                                                                  \
        {                                                                                                   \
            for(TUSIGN32 i=0; i<MAX_COLS/2; i++)                                                            \
            {                                                                                               \
                DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+addrOffset+i,0x77);                 \
            }                                                                                               \
            addrOffset += BYTES_PER_ROW;                                                                    \
        }                                                                                                   \
        /* Draw ABB splash */                                                                               \
        MAX_COLS = ABB_splash_definition.ABB_splash_Width;                                                  \
        MAX_ROWS = ABB_splash_definition.ABB_splash_Height;                                                 \
        addrOffset = ABB_splash_definition.ABB_splash_Start;                                                \
        TUSIGN16 counter=0;                                                                                 \
        for(TUSIGN32 j=0; j<MAX_ROWS; j++)                                                                  \
        {                                                                                                   \
            for(TUSIGN32 i=0; i<MAX_COLS/2; i++)                                                            \
            {                                                                                               \
                DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+addrOffset+i,psplash[counter++]);   \
            }                                                                                               \
            addrOffset += BYTES_PER_ROW;                                                                    \
        }                                                                                                   \
    }
#else
    #define Implementation_DrawSplashScreen()                                                               \
    {                                                                                                       \
        /* Blank Entire screen */                                                                           \
        TUSIGN16 MAX_COLS = GDISPW;                                                                         \
        TUSIGN16 MAX_ROWS = GDISPH;                                                                         \
        TUSIGN32 addrOffset = 0;                                                                            \
        for(TUSIGN32 j=0; j<MAX_ROWS; j++)                                                                  \
        {                                                                                                   \
            for(TUSIGN32 i=0; i<MAX_COLS/2; i++)                                                            \
            {                                                                                               \
                DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+addrOffset+i,0x77);                 \
            }                                                                                               \
            addrOffset += BYTES_PER_ROW;                                                                    \
        }                                                                                                   \
    }
#endif

#define Implementation_WriteFrameTitle()                                                                                \
{                                                                                                                       \
    diagnosticHistory = eFALSE;                                                                                         \
    manualAdjust = eFALSE;                                                                                              \
    stringParam = eFALSE;                                                                                               \
                                                                                                                        \
	/* Write Title String at left of display (middle) */                                                                \
    drawTitleBar = eTRUE;                                                                                               \
    string = dataPtr;                                                                                                   \
  	textInfo.field.font = 0;                                                                                            \
	textInfo.field.invert = 0;                                                                                          \
	xPos = 0;                                                                                                           \
    yPos = 0;                                                                                                           \
                                                                                                                        \
    /* WriteStringEuropean();*/                                                                                         \
    Implementation_WriteString();                                                                                       \
                                                                                                                        \
    drawTitleBar = eFALSE;                                                                                              \
                                                                                                                        \
    TUSIGN8 armSecurityPassed;                                                                                          \
    T_UNIT const SLOW *pSub = GetSubsystemPtr(TFT_IDX);                                                                 \
    pSub->Get(pSub, TFT_IDX_armSecurityPassed,0,(void FAST *)&armSecurityPassed);                                       \
    if(armSecurityPassed)                                                                                               \
    {                                                                                                                   \
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
        if(configHeaderInit == eFALSE)                                                                                  \
        {                                                                                                               \
            /* Draw icon */                                                                                             \
            TUSIGN32 addrOffset = TITLEBAR_ICON_OFFSET;                                                                 \
            const TUSIGN8 MAX_COLS = TITLEBAR_ICON_WIDTH;                                                               \
            const TUSIGN8 MAX_ROWS = TITLEBAR_ICON_HEIGHT;                                                              \
            const TUSIGN16 tableOffset = configHeaderType*(TITLEBAR_ICON_WIDTH*TITLEBAR_ICON_HEIGHT/2);                 \
            for(TUSIGN8 row=0; row < MAX_ROWS; row++)                                                                   \
            {                                                                                                           \
                for(TUSIGN8 col=0; col<MAX_COLS/2; col++)                                                               \
                {                                                                                                       \
                    TUSIGN8 byte = titlebar_icons[tableOffset + col + (row*MAX_COLS/2)];                                \
                    DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+addrOffset+col,byte);                       \
                }                                                                                                       \
                addrOffset += BYTES_PER_ROW;                                                                            \
            }                                                                                                           \
        }                                                                                                               \
    }                                                                                                                   \
}

#define Implementation_DrawIcon()     Implementation_DrawIcon_Type4();

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
#define Implementation_TransmitImage(ptr_image)     Implementation_TransmitImage_Type4(ptr_image)
#define Implementation_BlankArea()                  Implementation_BlankArea_Type4()
#define Implementation_DrawSliderBar()              Implementation_DrawSliderBar_Type4()

#define IMPLEMENTATION_MAX_WRITE_BYTES              MAX_COMMON_HMI_WIDTH
#define IMPLEMENTATION_MAX_WRITES                   1
#define IMPLEMENTATION_INC_Y_POS                    1

//--------------------------------------------------------------------------------------------------
// Implementation specific macros
//--------------------------------------------------------------------------------------------------
#define HOR_NON_DISPLAY 	32    	// Horizontal non display period (increment in multiple of 32)
#define VER_NON_DISPLAY 	21    	// Vertical non display period
#define HOR_OFFSET 			64   	// Adjusting horizontal position of start data
#define VER_OFFSET 			18   	// Adjusting vertical   position of start data

#define BITS_PER_PIXEL 		4		// Bits per pixel

#define FOREGROUND_COL		0//0x99
#define	BACKGROUND_COL	    0x99
#define FORE_COL_HI_NIB		(FOREGROUND_COL&0xF0)
#define FORE_COL_LO_NIB		(FOREGROUND_COL&0x0F)
#define	BACK_COL_HI_NIB		(BACKGROUND_COL&0xF0)
#define	BACK_COL_LO_NIB		(BACKGROUND_COL&0x0F)

//#define DISPLAY_START_ADDR		0x40000		// TFT display - start address of display RAM
//#define DISPLAY_REG_ADDR   		0x80000    	// TFT display - register base address

// TFT controller register address offsets
#define LUT_BLUE_WRITE_DATA_REG     0x08
#define LUT_GREEN_WRITE_DATA_REG    0x09
#define LUT_RED_WRITE_DATA_REG      0x0A
#define LUT_WRITE_ADDR_REG          0x0B
#define GPIO_STATUS_CONTROL1_REG    0xaD
#define DISPLAY_MODE_REG            0x70
#define REVISION_CODE_REG       	0x00

#define CONTR_SIGNATURE				0x28	// Signature for SSD1906 controller (0x14 for SSD1905)

#define PAGE_PIXEL_HEIGHT		8			// TFT display - page height
#define DISPLAY_WRITE(addr,data)		((*(TUSIGN8 volatile*)(addr)) = (data))
#define DISPLAY_READ(addr)				(*((TUSIGN8 volatile*)(addr)))

// Common HMI colours
#define INVERT_COL_FORE         0xEE    // Dark blue
#define INVERT_COL_BACK         0x77    // White
#define INVERT_COL_FORE_LO      0x0E    // Dark blue
#define INVERT_COL_FORE_HI      0xE0
#define INVERT_COL_BACK_LO      0x07    // White
#define INVERT_COL_BACK_HI      0x70
#define DEFAULT_COL_FORE        0x00    // Black
#define DEFAULT_COL_BACK        0x99    // Light grey
#define SOFTKEY_COL_FORE        0xBB    // Dark grey
#define TITLEBAR_COL_FORE       0x77    // White
#define TITLEBAR_COL_BACK       0x55    // Blue
#define DRAWREPEAT_COL_FORE     0xEE    // Dark blue

#define ANIMATION_ICON_WIDTH    100
#define ANIMATION_ICON_HEIGHT   48
#define MAX_CONFIG_HEADERS      15
#define COMMON_HMI_WIDTH_MID    64
#endif  // #ifndef _DRIVER_HW_TYPE_4_H_
