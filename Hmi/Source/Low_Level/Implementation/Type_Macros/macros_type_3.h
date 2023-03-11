#ifndef _DRIVER_HW_TYPE_3_H_
#define _DRIVER_HW_TYPE_3_H_

#define Implementation_ResetController()    \
    TUSIGN8 ucColumnOffset;                 \
                                            \
    /* /CSI - High - No chip select */      \
    Layer2_Display_CS_high();               \
                                            \
    /*  AO  - Low  - A0 set to command */   \
    Layer2_Display_A0_command();            \
                                            \
    /* /RST - Low  - Reset Controller */    \
    Layer2_Display_ActivateHWreset();       \
    Delay100uS();                           \
                                            \
    /* /RST - High - End of reset pulse */  \
    Layer2_Display_DeactivateHWreset();     \
    Delay100uS();                           \
                                            \
    Layer2_Display_CS_low();                \
    Layer2_Display_A0_command();            \
    Layer2_Display_WR_low();                \
                                            \
	LcdWrite(SYSTEM_RESET);                 \

#define Implementation_SetSplashStartPosition(xpos)                                 \
{                                                                                   \
    Layer2_Display_A0_command();                                                    \
    LcdWrite((TUSIGN8)(SET_PAGE_ADDR|yPos));                                        \
    ucColumnOffset = COLUMN_OFFSET + xpos;                                          \
    LcdWrite((TUSIGN8)(SET_COLUMN_ADDR_MS_NIB + ((ucColumnOffset & 0xF0) >> 4) ));  \
    LcdWrite((TUSIGN8)(SET_COLUMN_ADDR_LS_NIB +  (ucColumnOffset & 0x0F)       ));  \
    Layer2_Display_A0_data();                                                       \
}


#define Implementation_SetTransmitImageStartPosition(xpos)  \
{                                                           \
    Layer2_Display_CS_low();                                \
    Layer2_Display_WR_low();                                \
    Layer2_Display_A0_command();                            \
    LcdWrite((TUSIGN8)(SET_PAGE_ADDR|yPos));                \
    ucColumnOffset = COLUMN_OFFSET + xPos;                  \
    LcdWrite((TUSIGN8)(SET_COLUMN_ADDR_MS_NIB + ((ucColumnOffset & 0xF0) >> 4) ));\
    LcdWrite((TUSIGN8)(SET_COLUMN_ADDR_LS_NIB +  (ucColumnOffset & 0x0F)       ));\
    Layer2_Display_A0_data();                               \
}

#define Implementation_GetSplashPattern()           CommonMacros_GetSplashPattern()
#define Implementation_EnableLCD()                  \
{                                                   \
    Layer2_Display_CS_low();                        \
    Layer2_Display_WR_low();                        \
    Layer2_Display_A0_command();                    \
}

#ifdef LAYER_2_OPTIMIZED_FOR_SPEED

#define Implementation_ReadSwitches()       \
{                                           \
    volatile TUSIGN8 rawSw;                 \
    Layer2_ReadSwitches(rawSw);             \
    Layer2_ArrangeSwitches(rawSw,temp);     \
}

#else

#define Implementation_ReadSwitches()       \
{                                           \
    temp = Layer2_ReadSwitches();           \
    temp = Layer2_ArrangeSwitches(temp);    \
}

#endif

#define Implementation_LcdWrite()   \
{                                   \
    Layer2_Display_RD_high();       \
    Layer2_OutputByte(data);        \
    Layer2_Display_RD_low();        \
}

#define Implementation_DisableLCD()                 Layer2_Display_CS_high()
#define Implementation_InitControllerExit()         CommonMacros_InitControllerExit()
#define Implementation_DisplayStartMessage()        CommonMacros_DisplayStartMessage()
#define Implementation_ShowRevisionInfo()           CommonMacros_ShowRevisionInfo()
#define Implementation_DrawSplashScreen()           CommonMacros_DrawSplashScreen()
#define Implementation_AssembleHardwareRevision()
#define Implementation_DrawSliderBarLowerDigit()    CommonMacros_DrawSliderBarLowerDigit()
#define Implementation_DrawSliderBarUpperDigit()    CommonMacros_DrawSliderBarUpperDigit()

#define Implementation_ScaleFont2()                 CommonFunctions_ScaleFont2()
#define Implementation_ScaleFont4()                 CommonFunctions_ScaleFont4()
#define Implementation_ScaleFont5()                 CommonFunctions_ScaleFont5()
#define Implementation_BuildImageEuropean()         CommonFunctions_BuildImageEuropean()
#define Implementation_GetFontPattern(index)        CommonFunctions_GetFontPattern(index)
#define Implementation_TransmitImage(ptr_image)     CommonFunctions_TransmitImage(ptr_image)
#define Implementation_DrawIcon()                   CommonFunctions_DrawIcon()
#define Implementation_BlankArea()                  CommonMacros_BlankArea()
#define Implementation_DrawSliderBar()              CommonFunctions_DrawSliderBar()
#define Implementation_WriteFrameTitle()            CommonMacros_WriteFrameTitle()

#define IMPLEMENTATION_MAX_WRITE_BYTES              MAX_COMMON_HMI_WIDTH
#define IMPLEMENTATION_MAX_WRITES                   1
#define IMPLEMENTATION_INC_Y_POS                    1

#endif  //#ifndef _DRIVER_HW_TYPE_3_H_
