#ifndef _DRIVER_HW_TYPE_2_H_
#define _DRIVER_HW_TYPE_2_H_

#define Implementation_ResetController()                    \
    TUSIGN8 ucColumnOffset;                                 \
                                                            \
    /* CS_TO_DISPLAY - High - No chip select */             \
    Layer2_Display_CS_high();                               \
                                                            \
    /*DC_TO_DISPLAY - Low  - A0 set to command*/            \
    Layer2_Display_A0_command();                            \
                                                            \
    /* XRES - Low  - Reset Controller */                    \
    Layer2_Display_ActivateHWreset();                       \
    Delay100uS();                                           \
                                                            \
    /* XRES - High - End of reset pulse */                  \
    Layer2_Display_DeactivateHWreset();                     \
    Delay100uS();                                           \
                                                            \
    /* set flag LOW to indicate write of display command */ \
    ucWriteData = 0;




#define Implementation_SetSplashStartPosition(xpos)                                 \
{                                                                                   \
    /* flag LOW to indicate write of display command */                             \
    ucWriteData = 0;                                                                \
                                                                                    \
    LcdWrite((TUSIGN8)(SET_PAGE_ADDR|yPos));		                                \
    ucColumnOffset = COLUMN_OFFSET + xpos;	                                        \
    LcdWrite((TUSIGN8)(SET_COLUMN_ADDR_MS_NIB + ((ucColumnOffset & 0xF0) >> 4) ));  \
    LcdWrite((TUSIGN8)(SET_COLUMN_ADDR_LS_NIB +  (ucColumnOffset & 0x0F)       ));  \
                                                                                    \
    /* set flag HIGH to indicate write of display data */                           \
    ucWriteData = 1;                                                                \
}

#ifdef TYPE_2_IMPLEMENTATION_PARALLEL_KEYS

#ifdef LAYER_2_OPTIMIZED_FOR_SPEED

//--------------------------------------------------------------------------------------------------
// This macro used when keys are read from parallel port and layer 2 is optimized for speed
#define Implementation_ReadSwitches()       \
{                                           \
    volatile TUSIGN8 rawSw;                 \
    Layer2_ReadSwitches(rawSw);             \
    Layer2_ArrangeSwitches(rawSw,temp);     \
}
#else
//--------------------------------------------------------------------------------------------------
// This macro used when keys are read from parallel port but layer 2 is not optimized for speed
#define Implementation_ReadSwitches()       \
{                                           \
    temp = Layer2_ReadSwitches();           \
    temp = Layer2_ArrangeSwitches(temp);    \
}
#endif

#else

#ifdef LAYER_2_OPTIMIZED_FOR_SPEED
//--------------------------------------------------------------------------------------------------
// This macro used when keys are read serially and layer 2 is optimized for speed
#define Implementation_ReadSwitches()       \
{	                                        \
    volatile TUSIGN8 delay=0;               \
    Layer2_Display_CS_high();               \
    Layer2_Keyboard_EnableLoad();           \
                                            \
    /* Send dummy & wait */                 \
    Layer2_TransmitByte(0);                 \
    for (delay=0; delay<5; delay++)         \
    {                                       \
        nop_instruction();                  \
    }                                       \
                                            \
    Layer2_Keyboard_EnableShift();          \
                                            \
    /* Send dummy & wait */                 \
    Layer2_TransmitByte(0);                 \
    for (delay=0; delay<5; delay++)         \
    {                                       \
        nop_instruction();                  \
    }                                       \
                                            \
    Layer2_ReceiveByte(temp);               \
                                            \
    Layer2_Display_A0_data();               \
    Layer2_Display_CS_low();                \
                                            \
    /* keyboard present? */                 \
    if ((temp&0xF0) != 0x80)                \
    {                                       \
        temp = 0x00;                        \
    }                                       \
    else                                    \
    {                                       \
        delay = temp;                       \
        Layer2_ArrangeSwitches(delay,temp); \
    }                                       \
}

#else

//--------------------------------------------------------------------------------------------------
// This macro used when keys are read serially and layer 2 is not optimized for speed
#define Implementation_ReadSwitches()       \
{	                                        \
    volatile TUSIGN8 delay=0;               \
    Layer2_Display_CS_high();               \
    Layer2_Keyboard_EnableLoad();           \
                                            \
    /* Send dummy & wait */                 \
    Layer2_TransmitByte(0);                 \
    for (delay=0; delay<5; delay++)         \
    {                                       \
        nop_instruction();                  \
    }                                       \
                                            \
    Layer2_Keyboard_EnableShift();          \
                                            \
    /* Send dummy & wait */                 \
    Layer2_TransmitByte(0);                 \
    for (delay=0; delay<5; delay++)         \
    {                                       \
        nop_instruction();                  \
    }                                       \
                                            \
    temp = Layer2_ReceiveByte();            \
                                            \
    Layer2_Display_A0_data();               \
    Layer2_Display_CS_low();                \
                                            \
    /* keyboard present? */                 \
    if ((temp&0xF0) != 0x80)                \
    {                                       \
        temp = 0x00;                        \
    }                                       \
    else                                    \
    {                                       \
        temp = Layer2_ArrangeSwitches(temp);\
    }                                       \
}
#endif
#endif

#define Implementation_LcdWrite()   \
{                                   \
    TUSIGN16 ui;                    \
                                    \
    if (ucWriteData)                \
    {                               \
        /* Send display data to transmit register */    \
        Layer2_TransmitByte(data);  \
        for (ui=0; ui<5; ui++)      \
        {                           \
          asm("nop");               \
        }                           \
    }                               \
    else                            \
    {                               \
        Layer2_Display_CS_high();   \
        Layer2_Display_A0_command();\
        Layer2_Display_CS_low();    \
                                    \
        Layer2_TransmitByte(data);  \
        for (ui=0; ui<5; ui++)      \
        {                           \
          nop_instruction();        \
        }                           \
                                    \
        Layer2_Display_A0_data();   \
    }                               \
}

#define Implementation_SetTransmitImageStartPosition(xpos)  \
    Implementation_SetSplashStartPosition(xpos)

#define Implementation_GetSplashPattern()           CommonMacros_GetSplashPattern()
#define Implementation_EnableLCD()                  {ucWriteData = 0;}
#define Implementation_DisableLCD()                 {asm("nop");}
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

#endif  // #ifndef _DRIVER_HW_TYPE_2_H_
