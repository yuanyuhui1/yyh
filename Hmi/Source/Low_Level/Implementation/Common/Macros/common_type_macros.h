#ifndef _COMMON_MACROS_H_
#define _COMMON_MACROS_H_

#define CommonMacros_GetSplashPattern()             \
{                                                   \
    i=0;                                            \
    while(i < SPLASH_WIDTH)                         \
    {                                               \
        bitPattern[i] = abbLogo[i+fontTableIndex];  \
        i++;                                        \
    }                                               \
}

#define CommonMacros_InitControllerExit()             return eTRUE
#define CommonMacros_InitControllerExitTFT()                    \
{                                                               \
	/* Hold 'ABB connecting...' display for 3 seconds */        \
	unsigned short limt = 20000;                                \
	unsigned short count;                                       \
		for(count=0; count<limt; count++)                       \
			Delay100uS();                                       \
    return eTRUE;                                               \
}

#define CommonMacros_DisplayStartMessage()                                  \
{                                                                           \
    if(displayConnectMessage)                                               \
    {                                                                       \
        StaticTextWrite(connectMessage,0x0E,(TUSIGN8)(revBuffPtr*FONT1_WIDTH),eFALSE);	\
    }                                                                       \
    else                                                                    \
    {                                                                       \
        StaticTextWrite(abbWebMessage,0x0E,(TUSIGN8)(revBuffPtr*FONT1_WIDTH),eFALSE);	\
    }                                                                       \
}

#define CommonMacros_ShowRevisionInfo()           StaticTextWrite(revBuff,0x4E,0,eFALSE)

#define CommonMacros_DrawSplashScreen()             \
{                                                   \
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
    fontTableIndex=0;                               \
                                                    \
    numBytesToWrite = SPLASH_WIDTH;                 \
    xPos = (MAX_COMMON_HMI_WIDTH-SPLASH_WIDTH)/2;   \
    for (yPos=1;yPos<5;yPos++)                      \
    {                                               \
        Implementation_GetSplashPattern();          \
        Implementation_TransmitImage(bitPattern);   \
        fontTableIndex += SPLASH_WIDTH;             \
    }                                               \
}

#define CommonMacros_DrawSliderBarLowerDigit()                      \
{                                                                   \
    fontTableIndex *= (FONT1_WIDTH-1);                              \
    for(i=0;i<FONT1_WIDTH-1;i++)                                    \
    {                                                               \
        bitPattern[i] = Implementation_GetFontPattern((TUSIGN16)(fontTableIndex+i));\
    }                                                               \
    bitPattern[i] = 0x00;                                           \
    numBytesToWrite = FONT1_WIDTH;                                  \
    yPos=0;                                                         \
    xPos = MAX_COMMON_HMI_WIDTH-FONT1_WIDTH;                        \
    Implementation_TransmitImage(bitPattern);                                      \
}

#define CommonMacros_DrawSliderBarUpperDigit()                      \
{                                                                   \
    fontTableIndex *= (FONT1_WIDTH-1);                              \
    for(i=0;i<FONT1_WIDTH-1;i++)                                    \
    {                                                               \
        bitPattern[i] = Implementation_GetFontPattern((TUSIGN16)(fontTableIndex+i));\
    }                                                               \
    bitPattern[i] = 0x00;                                           \
    numBytesToWrite = FONT1_WIDTH;                                  \
    yPos=0;                                                         \
    xPos = MAX_COMMON_HMI_WIDTH-(FONT1_WIDTH*2);                    \
    Implementation_TransmitImage(bitPattern);                                      \
}

#define CommonMacros_DrawSliderBarLowerDigitTFT()           \
{                                                           \
    writeText = eTRUE;                                      \
    yPos=0;                                                 \
    fontTableIndex *= ((FONT1_WIDTH*2)-2);                  \
    for(TUSIGN8 tableIndex=0;tableIndex<2;tableIndex++)     \
    {                                                       \
        for(i=0;i<((FONT1_WIDTH*2)-2);i++)                  \
        {                                                   \
            if(tableIndex)                                  \
            {                                               \
                bitPattern[i] = FontLower[fontTableIndex+i];\
            }                                               \
            else                                            \
            {                                               \
                bitPattern[i] = FontUpper[fontTableIndex+i];\
            }                                               \
        }                                                   \
        bitPattern[i] = 0x00;                               \
        bitPattern[i+1] = 0x00;                             \
        numBytesToWrite = FONT1_WIDTH*2;                    \
        xPos = (MAX_COMMON_HMI_WIDTH-FONT1_WIDTH)*2;        \
        Implementation_TransmitImage(bitPattern);           \
        yPos++;                                             \
    }                                                       \
    writeText = eFALSE;	                                    \
}

#define CommonMacros_DrawSliderBarUpperDigitTFT()           \
{                                                           \
    writeText = eTRUE;                                      \
    yPos=0;                                                 \
    fontTableIndex *= ((FONT1_WIDTH*2)-2);                  \
    for(TUSIGN8 tableIndex=0;tableIndex<2;tableIndex++)     \
    {                                                       \
        for(i=0;i<((FONT1_WIDTH*2)-2);i++)                  \
        {                                                   \
            if(tableIndex)                                  \
            {                                               \
                bitPattern[i] = FontLower[fontTableIndex+i];\
            }                                               \
            else                                            \
            {                                               \
                bitPattern[i] = FontUpper[fontTableIndex+i];\
            }                                               \
        }                                                   \
        bitPattern[i] = 0x00;                               \
        bitPattern[i+1] = 0x00;                             \
        numBytesToWrite = FONT1_WIDTH*2;                    \
        xPos = (MAX_COMMON_HMI_WIDTH-(FONT1_WIDTH*2))*2;    \
        Implementation_TransmitImage(bitPattern);	        \
        yPos++;                                             \
    }                                                       \
    writeText = eFALSE;	                                    \
}

#define CommonMacros_GetFontPattern()       (FontTable[tableIndex])

#define CommonMacros_BlankArea()                            \
{                                                           \
    /* BLANK n pages */                                     \
    numBytesToWrite = IMPLEMENTATION_MAX_WRITE_BYTES;       \
    yPos = blankAreaInfo.field.startPage;                   \
    for(j=0;j<=(blankAreaInfo.field.endPage - blankAreaInfo.field.startPage);j++)   \
    {                                                       \
        for(temp=0;temp<IMPLEMENTATION_MAX_WRITES;temp++)   \
        {                                                   \
            i=0;                                            \
            while(i < numBytesToWrite)                      \
            {                                               \
                bitPattern[i]=0;                            \
                i++;                                        \
            }                                               \
            xPos = temp*(MAX_COMMON_HMI_WIDTH - numBytesToWrite);   \
            Implementation_TransmitImage(bitPattern);       \
            yPos += IMPLEMENTATION_INC_Y_POS;               \
        }                                                   \
    }                                                       \
}

#if defined (_NONVOL_IMPLEMENTATION) || (defined(EMBEDDED_ASIAN))

#define Implementation_WriteString()                            \
{                                                               \
    if(functionCode == WRITE_FRAME_TITLE_ASIAN)                 \
    {                                                           \
        WriteStringAsian();                                     \
    }                                                           \
    else                                                        \
    {                                                           \
        WriteStringEuropean();                                  \
    }                                                           \
}

#else

#define Implementation_WriteString()                            \
{                                                               \
    WriteStringEuropean();                                      \
}

#endif

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

#define CommonMacros_WriteFrameTitle()                                               \
{                                                                                    \
	/* Draw horizontal line across top of display	*/                               \
	numBytesToWrite = (MAX_COMMON_HMI_WIDTH - 14)/2;                                 \
	for(temp = 0; temp < 2; temp++)                                                  \
	{                                                                                \
		for(i = 0; i < numBytesToWrite; i++)                                         \
		{                                                                            \
			bitPattern[i] = 0x10;                                                    \
		}	                                                                         \
		xPos = numBytesToWrite*temp;                                                 \
		yPos = 0;                                                                    \
		Implementation_TransmitImage(bitPattern);                                    \
	}                                                                                \
	                                                                                 \
    /* Calculate x position for text */                                              \
    if (functionCode == WRITE_FRAME_TITLE_EUROPE)                                    \
    {                                                                                \
        /* European: Optimize for best visual effect -                   */          \
        /* If No of chars < 13 :- Text centred relative to display       */          \
        /* If No of chars > 12 :- Text centred relative to writable area */          \
        xPos = (MAX_COMMON_HMI_WIDTH - (numChars * FONT1_WIDTH));                    \
        if(numChars >= 13)                                                           \
        {                                                                            \
            xPos -= 14;                                                              \
        }                                                                            \
        xPos /= 2;                                                                   \
    }                                                                                \
    else                                                                             \
    {                                                                                \
        /* Asian position close to left */                                           \
        xPos = 3;                                                                    \
    }                                                                                \
    temp = xPos;                                                                     \
                                                                                     \
    /* Draw two pixel gap to left of text */                                         \
    bitPattern[0] = 0x00;                                                            \
    bitPattern[1] = 0x00;                                                            \
    numBytesToWrite = 2;                                                             \
    xPos = temp - 2;                                                                 \
    Implementation_TransmitImage(bitPattern);                                        \
                                                                                     \
    /* Draw three pixel gap to right of text */                                      \
    bitPattern[2] = 0x00;                                                            \
    numBytesToWrite = 3;                                                             \
                                                                                     \
    j = functionCode == WRITE_FRAME_TITLE_EUROPE ? FONT1_WIDTH : FONT_ASIAN_WIDTH_2; \
                                                                                     \
    xPos = temp + (numChars*j);                                                      \
    Implementation_TransmitImage(bitPattern);                                        \
                                                                                     \
    /* Write Title String across top of display (middle) */                          \
    string = dataPtr;                                                                \
    textInfo.field.font = 0;                                                         \
    textInfo.field.invert = 0;                                                       \
    xPos = temp;                                                                     \
    Implementation_WriteString();                                                    \
}

#endif  // #ifndef _COMMON_MACROS_H_
