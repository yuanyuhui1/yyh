#ifndef _DRIVER_HW_TYPE_1_H_
#define _DRIVER_HW_TYPE_1_H_

#define Implementation_ResetController()        \
{                                               \
	/* Hardware Reset */                        \
	P2OUT |= 0x04;                              \
                                                \
	ENABLE_LCD;                                 \
	COMMAND_DATA;                               \
}
#define Implementation_SetSplashStartPosition(xpos) \
{                                               \
    COMMAND_DATA;                               \
    LcdWrite(SET_PAGE_ADDR|yPos);		        \
    LcdWrite(SET_COLUMN_ADDR_MS_NIB);           \
    LcdWrite(SET_COLUMN_ADDR_LS_NIB);           \
    DISPLAY_DATA;                               \
}

#define Implementation_SetTransmitImageStartPosition(xpos)  \
{                                                       \
    ENABLE_LCD;                                         \
    COMMAND_DATA;                                       \
    LcdWrite(SET_PAGE_ADDR|yPos);                       \
    LcdWrite(SET_COLUMN_ADDR_MS_NIB | ((xPos&0xF0)>>4));\
    LcdWrite(SET_COLUMN_ADDR_LS_NIB | (xPos&0x0F));     \
    DISPLAY_DATA;	                                    \
}

#define Implementation_LcdWrite()   \
{                                   \
	P2OUT |= 0x10;                  \
	P1OUT = data;                   \
	P2OUT &= 0xEF;                  \
}

#define Implementation_EnableLCD()              {ENABLE_LCD;COMMAND_DATA;}
#define Implementation_DisableLCD()             DISABLE_LCD

#ifdef _SWITCHES_CAPACITIVE
    #define Implementation_ReadSwitches()       {temp = P3IN;}
#elif defined _SWITCHES_NOT_FITTED
    #define Implementation_ReadSwitches()       {temp = 0;}
#else
    #define Implementation_ReadSwitches()       \
    {                                           \
        volatile unsigned char delay=0;         \
        P3OUT |= 0x40;                          \
                                                \
        /* Allow for port line to change state */\
        for(delay=0;delay<3;delay++);           \
                                                \
        temp = ~(P3IN & 0x0F);                  \
        temp &= 0x0F;                           \
        P3OUT &= 0xB0;                          \
    }
#endif	

// Program NV build
#if defined (_NONVOL_IMPLEMENTATION) && defined (_PROGRAM_NONVOL)
    #define Implementation_GetSplashPattern()
    #define Implementation_InitControllerExit()     CommonMacros_InitControllerExit()
    #define Implementation_DrawIcon()

    #define Implementation_DisplayStartMessage()
    #define Implementation_ShowRevisionInfo()       StaticTextWrite(revBuff,0x00,0,eFALSE);
    #define Implementation_DrawSplashScreen()           \
        /* Blank Screen */                              \
   	    numBytesToWrite = MAX_COMMON_HMI_WIDTH/2;       \
       	xPos=0;                                         \
        for (yPos=0;yPos<8;yPos++)                      \
        {                                               \
            Implementation_SetSplashStartPosition(0);   \
                                                        \
        	for(xPos=0; xPos<MAX_COMMON_HMI_WIDTH; xPos += MAX_COMMON_HMI_WIDTH/2)  \
		    {		                                    \
			    i=0;                                    \
    			while(i < MAX_COMMON_HMI_WIDTH/2)       \
	    		{                                       \
		    		bitPattern[i] = 0x00;               \
			    	i++;                                \
    			}                                       \
	    		Implementation_TransmitImage(bitPattern);   \
		    }                                           \
        }                                               \
        fontTableIndex=0;




    #define Implementation_AssembleHardwareRevision()

// NV build specific
#elif defined (_NONVOL_IMPLEMENTATION)

    #define Implementation_GetSplashPattern()                                           \
        TUSIGN16 readAddr;                                                              \
        TUSIGN8  nonVolData[BLOCK_INFO_ALLOCATED_SIZE];                                 \
                                                                                        \
        /* Get the Block Info */                                                        \
        readAddr = GetBlockInfo(BLOCK_ID_LOGO, nonVolData);                             \
                                                                                        \
        if (readAddr)                                                                   \
        {                                                                               \
          /* Extract the start address of the BLOCK_DATA (BLOCK_START) */               \
          readAddr = ((TUSIGN16)nonVolData[NONVOL_BLOCK_INFO__BLOCK_START_MSBYTE] << 8) \
                   +            nonVolData[NONVOL_BLOCK_INFO__BLOCK_START_LSBYTE];      \
                                                                                        \
          /* Find the address of the required section of the Logo */                    \
          readAddr += fontTableIndex;                                                   \
                                                                                        \
          /* Read the icon data */                                                      \
   		  ReadNonVolData(readAddr, bitPattern, numBytesToWrite);                        \
        }


    #define Implementation_InitControllerExit()     return VerifyNonVolData(0)
    #define Implementation_DrawIcon()               Implementation_DrawIcon_Type1_NV()

// Normal build
#else
    #define Implementation_GetSplashPattern()       CommonMacros_GetSplashPattern()
    #define Implementation_InitControllerExit()     CommonMacros_InitControllerExit()
    #define Implementation_DrawIcon()               CommonFunctions_DrawIcon()
#endif

// Common to NV & non-NV builds
#ifndef _PROGRAM_NONVOL
    #define Implementation_DisplayStartMessage()        CommonMacros_DisplayStartMessage()
    #define Implementation_ShowRevisionInfo()           CommonMacros_ShowRevisionInfo()
    #define Implementation_DrawSplashScreen()           CommonMacros_DrawSplashScreen()
    #define Implementation_AssembleHardwareRevision()           \
    {                                                           \
	    TUSIGN8	hwRevCopy = HARDWARE_REVISION;                  \
        if(hwRevCopy/100)                                       \
        {                                                       \
            revBuff[revBuffPtr++] = '0' + (hwRevCopy/100);      \
            while(hwRevCopy/100) {hwRevCopy-=100;}              \
        }                                                       \
        if(hwRevCopy/10)                                        \
        {                                                       \
            revBuff[revBuffPtr++] = '0' + (hwRevCopy/10);       \
            while(hwRevCopy/10) {hwRevCopy-=10;}                \
        }                                                       \
        revBuff[revBuffPtr++] = '0' + hwRevCopy;                \
        revBuff[revBuffPtr++] = '.';                            \
    }
#endif  // #ifndef _PROGRAM_NONVOL

// Common to all type 1 builds
#define Implementation_DrawSliderBarLowerDigit()    CommonMacros_DrawSliderBarLowerDigit()
#define Implementation_DrawSliderBarUpperDigit()    CommonMacros_DrawSliderBarUpperDigit()

#define Implementation_ScaleFont2()                 CommonFunctions_ScaleFont2()
#define Implementation_ScaleFont4()                 CommonFunctions_ScaleFont4()
#define Implementation_ScaleFont5()                 CommonFunctions_ScaleFont5()
#define Implementation_BuildImageEuropean()         CommonFunctions_BuildImageEuropean()
#define Implementation_GetFontPattern(index)        CommonFunctions_GetFontPattern(index)
#define Implementation_TransmitImage(ptr_image)     CommonFunctions_TransmitImage(ptr_image)
#define Implementation_BlankArea()                  CommonMacros_BlankArea()
#define Implementation_DrawSliderBar()              CommonFunctions_DrawSliderBar()
#define Implementation_WriteFrameTitle()            CommonMacros_WriteFrameTitle()

#define IMPLEMENTATION_MAX_WRITE_BYTES              (MAX_COMMON_HMI_WIDTH/2)
#define IMPLEMENTATION_MAX_WRITES                   2
#define IMPLEMENTATION_INC_Y_POS                    temp

#endif  // #ifndef _DRIVER_HW_TYPE_1_H_

