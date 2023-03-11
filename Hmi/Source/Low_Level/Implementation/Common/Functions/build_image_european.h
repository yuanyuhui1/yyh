#ifndef _BUILD_IMAGE_EUROPEAN_H_
#define _BUILD_IMAGE_EUROPEAN_H_

#ifndef _IMPLEMENTATIONDEFINITIONS
  #error implementation.h must be included
#endif

#if defined(TYPE_2_IMPLEMENTATION) || defined(TYPE_3_IMPLEMENTATION) || defined(TYPE_4_IMPLEMENTATION) || defined(TYPE_5_IMPLEMENTATION)
    #ifndef _COMPILETIMEDEFINITIONS
        #error compileTimeDefinitions.h must be included
    #endif
#endif

// File holds definition's of BuildImageEuropean() that are common to two or more implementations

#if defined(TYPE_1_IMPLEMENTATION) || defined(TYPE_2_IMPLEMENTATION) || defined(TYPE_3_IMPLEMENTATION)

#ifndef _MSP430
// this function not size optimized beyond medium
#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
#endif
/*------------------------------------------------------------------------------
Function:		CommonFunctions_BuildImageEuropean
Parameters:		None	
Returns:		None
Description:	Called by WriteStringEurope(). Assembles the data to send to the
				display.
//------------------------------------------------------------------------------*/

// WRITE_FRAME_TITLE:       LATER VERSION                                           EARLY VERSION
// -----------------        --------------                                          -------------
                            // [0] function code                                    // [0] function code
                            // [1] len                                              // [1] len
  // dataPtr points here -> // [2] extendedCharacterPage                            // [2] first byte of string     <- dataPtr points here  <-string points here
  // string points here --> // [3] first byte of string


// WRITE_STRING_EUROPE:     LATER VERSION                                           EARLY VERSION
// --------------------     --------------                                          -------------
                            // [0] function code                                    // [0] function code
                            // [1] len                                              // [1] len
                            // [2] xPos byte (blank line bit has been cleared)      // [2] xPos byte (blank line bit has been cleared)
                            // [3] textInfo byte (invert,font,yPos)                 // [3] textInfo byte (invert,font,yPos)
  // dataPtr points here -> // [4] additional info byte                             // [4] first byte of string     <- dataPtr points here  <-string points here
  // string points here --> // [5] first byte of string

static void CommonFunctions_BuildImageEuropean(void)
{		
	TUSIGN8 startingXpos = xPos;		
	numBytesToWrite = numChars*FONT1_WIDTH*scaleWidth;		
	
    #if defined(TYPE_2_IMPLEMENTATION) || defined(TYPE_3_IMPLEMENTATION)
    #ifndef DISABLE_SMOOTH_FONT

	TBOOL smoothFont = eFALSE;

    // Check if string contains only characters where bit patterns can be extracted from true font tables
    if ((textInfo.field.invert == 0)                    // true font can only be used if no character inversion required
     && ((textInfo.field.font == FONT2)                 // true font can only be used for fonts 2,4 & 5
      || (textInfo.field.font == FONT4)
      || (textInfo.field.font == FONT5)))
    {
        charPos=0;
        smoothFont = eTRUE;                                 // assume smoothed font can be used

        while ((charPos < numChars) && (smoothFont == eTRUE))
        {
            // true fonts are available for thhe following characters: " -./0123456789"
            if (((string[charPos] < '-') || (string[charPos] > '9'))
             && (string[charPos] != ' '))
            {
                smoothFont = eFALSE;                        // un supported character found - smoothed font cannot be used
            }

            if ((*dataPtr & INVCHAR_MASK) == charPos+1)
            {
                smoothFont = eFALSE;                        // character inversion found - smoothed font cannot be used
            }

            charPos ++;
        }
    }

    if (smoothFont == eTRUE)
    {
        TUSIGN8 charIndex,lineIndex,columnIndex;

        // bit patterns can be extracted from smoothed font tables (rather than using scaled font)
        while (pagesToWrite)
        {
            lineIndex = pagesToWrite - 1;
            bitPatternIndex = 0;

            if (textInfo.field.font == FONT2)
            {
                for (charPos = 0; charPos < numChars; charPos++)
                {
                    charIndex = string[charPos];
                    if ((charIndex >= '-') && (charIndex <= '9'))
                    {
                        charIndex -= '-';
                        for (columnIndex = 0; columnIndex < FONT2_WIDTH; columnIndex++)
                        {
                            bitPattern[bitPatternIndex] = TrueFont_2[charIndex][lineIndex][columnIndex];
                            bitPatternIndex ++;
                        }
                    }
                    else
                    {
                        for (columnIndex = 0; columnIndex < FONT2_WIDTH; columnIndex++)
                        {
                            bitPattern[bitPatternIndex] = 0x00;
                            bitPatternIndex ++;
                        }
                    }
                }
            }
            else if (textInfo.field.font == FONT4)
            {
                for (charPos = 0; charPos < numChars; charPos++)
                {
                    charIndex = string[charPos];
                    if ((charIndex >= '-') && (charIndex <= '9'))
                    {
                        charIndex -= '-';
                        for (columnIndex = 0; columnIndex < FONT4_WIDTH; columnIndex++)
                        {
                            bitPattern[bitPatternIndex] = TrueFont_4[charIndex][lineIndex][columnIndex];
                            bitPatternIndex ++;
                        }
                    }
                    else
                    {
                        for (columnIndex = 0; columnIndex < FONT4_WIDTH; columnIndex++)
                        {
                            bitPattern[bitPatternIndex] = 0x00;
                            bitPatternIndex ++;
                        }
                    }
                }
            }
            else
            {
                for (charPos = 0; charPos < numChars; charPos++)
                {
                    charIndex = string[charPos];
                    if ((charIndex >= '-') && (charIndex <= '9'))
                    {
                        charIndex -= '-';
                        for (columnIndex = 0; columnIndex < FONT5_WIDTH; columnIndex++)
                        {
                            bitPattern[bitPatternIndex] = TrueFont_5[charIndex][lineIndex][columnIndex];
                            bitPatternIndex ++;
                        }
                    }
                    else
                    {
                        for (columnIndex = 0; columnIndex < FONT5_WIDTH; columnIndex++)
                        {
                            bitPattern[bitPatternIndex] = 0x00;
                            bitPatternIndex ++;
                        }
                    }
                }
            }
			numBytesToWrite = numChars * columnIndex;
			Implementation_TransmitImage(bitPattern);

			yPos++;					
            pagesToWrite --;
        }
	    smoothFont = eFALSE;
    }
    else
    #endif  // #ifndef DISABLE_SMOOTH_FONT
    #endif  // #if defined(TYPE_2_IMPLEMENTATION) || defined(TYPE_3_IMPLEMENTATION)

    {
        while(pagesToWrite)
	    {
            bitPatternIndex = 0;
			for(charPos=0;charPos < numChars;charPos++)
			{						
				// Check for string or line inversion			
				invert = textInfo.field.invert;
	
                // Get the character to be processed
                i = string[charPos];

                #ifdef TYPE_1_IMPLEMENTATION
                if (mainHMI_type <= 20302)
                {                                                   // EARLY VERSION - Character Inversion based upon MSBit of character code
                    if((i&0x80) != 0)                               // IF  MSBit of character is set
                    {                                               // THEN
                        invert |= INVERT_CHAR;                      //     Set Invert flag
                        i &= 0x7F;                                  //     Clear MSBit
                    }
                }
                else
                #endif
                {                                                   // LATER VERSION - The additional info byte contains character inversion info
                    if ((*dataPtr & INVCHAR_MASK) == charPos+1)     // IF  additional info byte identifies current character
                    {                                               // THEN
					    invert |= INVERT_CHAR;                      //     Set Invert flag
                    }
                }

	            #if defined(TYPE_2_IMPLEMENTATION) || defined(TYPE_3_IMPLEMENTATION)

                    // Get index for the start of the character within the font table
                    if (0)
                    {
                    }
                    #ifdef INCLUDE_LATIN1_PATTERNS
                    else if (((*dataPtr & EXTENDED_PAGE_MASK) == LL_EXTENDED_CHARACTER_PAGE_LATIN1) && (i >= FIRST_LATIN1) && (i <= LAST_LATIN1))
                    {
                        fontTableIndex =  (i - FIRST_LATIN1) + START_LATIN1;
                    }
                    #endif
                    #ifdef INCLUDE_KATAKANA_PATTERNS
                    else if (((*dataPtr & EXTENDED_PAGE_MASK) == LL_EXTENDED_CHARACTER_PAGE_KATAKANA) && (i >= FIRST_KATAKANA) && (i <= LAST_KATAKANA))
                    {
                        fontTableIndex =  (i - FIRST_KATAKANA) + START_KATAKANA;
                    }
                    #endif
                    #ifdef INCLUDE_TURKISH_PATTERNS
                    else if (((*dataPtr & EXTENDED_PAGE_MASK) == LL_EXTENDED_CHARACTER_PAGE_TURKISH) && (i >= FIRST_TURKISH) && (i <= LAST_TURKISH))
                    {
                        fontTableIndex = (i - FIRST_TURKISH) + START_TURKISH;
                    }
                    #endif
                    #ifdef INCLUDE_RUSSIAN_PATTERNS
                    else if (((*dataPtr & EXTENDED_PAGE_MASK) == LL_EXTENDED_CHARACTER_PAGE_RUSSIAN) && (i >= FIRST_RUSSIAN) && (i <= LAST_RUSSIAN))
                    {
                        fontTableIndex = (i - FIRST_RUSSIAN) + START_RUSSIAN;
                    }
                    #endif
                    else if ((i >= FIRST_EUROPEAN) && (i <= LAST_EUROPEAN))
                    {
                        fontTableIndex = (i - FIRST_EUROPEAN) + START_EUROPEAN;
                    }
                    else
                    {
                        fontTableIndex = ('?' - FIRST_EUROPEAN) + START_EUROPEAN;
                    }
                    fontTableIndex *= (FONT1_WIDTH-1);

                #else   // else condition for #if defined(TYPE_2_IMPLEMENTATION) || defined(TYPE_3_IMPLEMENTATION)

                    #if defined(_NONVOL_IMPLEMENTATION)

                    TUSIGN16 readAddr = 0;

                    #if (BLOCK_INFO_ALLOCATED_SIZE >= EUROPEAN_CHARACTER_ALLOCATED_SIZE)
                        TUSIGN8  nonVolData[BLOCK_INFO_ALLOCATED_SIZE];
                    #else
                        TUSIGN8  nonVolData[EUROPEAN_CHARACTER_ALLOCATED_SIZE];
                    #endif

                    if (mainHMI_type > 20302)
                    {                                                   // LATER VERSION - which includes the extended character page
                        TUSIGN8 blockID;
                        switch (*dataPtr & EXTENDED_PAGE_MASK)
                        {
                        case LL_EXTENDED_CHARACTER_PAGE_KATAKANA:   blockID = BLOCK_ID_KATAKANA;            break;
                        case LL_EXTENDED_CHARACTER_PAGE_TURKISH:    blockID = BLOCK_ID_TURKISH;             break;
                        case LL_EXTENDED_CHARACTER_PAGE_RUSSIAN:    blockID = BLOCK_ID_RUSSIAN;             break;
                        default:                                    blockID = BLOCK_ID_EXTENDED_EUROPEAN;   break;
                        }
                        readAddr = GetBlockInfo(blockID, nonVolData);

                        if (readAddr)
                        {
                            // Extract the start address of the BLOCK_DATA (BLOCK_START)
                            readAddr  = ((TUSIGN16)nonVolData[NONVOL_BLOCK_INFO__BLOCK_START_MSBYTE] << 8) + nonVolData[NONVOL_BLOCK_INFO__BLOCK_START_LSBYTE];
                        }

                        if ((string[charPos] < nonVolData[NONVOL_BLOCK_INFO__FIRST_CHAR_LSBYTE])    // string[charPos] < FIRST_CHAR
                         || (string[charPos] > nonVolData[NONVOL_BLOCK_INFO__LAST_CHAR_LSBYTE])     // string[charPos] > LAST_CHAR
                         || (nonVolData[NONVOL_BLOCK_INFO__FIRST_CHAR_MSBYTE] != 0)                 // nonvol supports characters > 0xFF
                         || (nonVolData[NONVOL_BLOCK_INFO__LAST_CHAR_MSBYTE]  != 0)                 // nonvol supports characters > 0xFF
                         || (readAddr == 0))                                                        // Block could not be found
                        {
                            if ((string[charPos] >= FONT_TABLE_FIRST_CHARACTER) && (string[charPos] <= FONT_TABLE_LAST_CHARACTER))
                            {
                                // Get Bit Pattern from ROM (FontTable[])
                                fontTableIndex = string[charPos] - FONT_TABLE_FIRST_CHARACTER;
                            }
                            else
                            {
                                // Display '?'
                                fontTableIndex = '?' - FONT_TABLE_FIRST_CHARACTER;
                            }

                            // index in FontTable = character position in FontTable * width of each character
                            fontTableIndex *= (FONT1_WIDTH-1);
                            // address in NONVOL is not valid
                            readAddr = 0;
                        }
                        else
                        {
                            // address in NONVOL = start address of the BLOCK_DATA + (character position in BLOCK_DATA * width of each character)
                            readAddr += (string[charPos] - nonVolData[NONVOL_BLOCK_INFO__FIRST_CHAR_LSBYTE]) * nonVolData[NONVOL_BLOCK_INFO__CHAR_WIDTH_BYTE];

                            // read the character from the NONVOL
                            ReadNonVolData(readAddr, nonVolData, nonVolData[NONVOL_BLOCK_INFO__CHAR_WIDTH_BYTE]);
                        }
                    }
                    else
                    #endif  // end of else condition for #if defined(_NONVOL_IMPLEMENTATION)
                    {
                        if ((i < FONT_TABLE_FIRST_CHARACTER) || (i > FONT_TABLE_LAST_CHARACTER))
                        {
                            fontTableIndex = '?' - FONT_TABLE_FIRST_CHARACTER;
                        }
                        else
                        {
                            fontTableIndex = i - FONT_TABLE_FIRST_CHARACTER;
                        }
                        fontTableIndex *= (FONT1_WIDTH-1);
                    }

                #endif  // end of else condition for #if defined(TYPE_2_IMPLEMENTATION) || defined(TYPE_3_IMPLEMENTATION)

                // Add bit patterns to bitPattern[]
				for(i=0, j=0; j <= (FONT1_WIDTH-1); i += scaleWidth, j++)				
                {
                    if (j == (FONT1_WIDTH-1))
                    {
                        srcBitPattern = 0x00;
                    }
                    #if defined(_NONVOL_IMPLEMENTATION)
                    else if (readAddr)
                    {
                        srcBitPattern = nonVolData[j];
                    }
                    #endif
                    else
                    {
                        srcBitPattern = Implementation_GetFontPattern((TUSIGN16)(fontTableIndex+j));
                    }
                    scaledBitPattern=0;
                    switch (textInfo.field.font)
                    {
                    #ifndef _MODULE_TESTING
                    #ifndef _PROGRAM_NONVOL
                    case FONT5:     Implementation_ScaleFont5();	                                            break;
                    case FONT4:     Implementation_ScaleFont4();	                                            break;
                    #endif  // end of #ifndef _PROGRAM_NONVOL
                    case FONT2:     Implementation_ScaleFont2();                                                break;
                    #endif  // end of #ifndef _MODULE_TESTING
                    default:        bitPattern[bitPatternIndex + j] = invert ? ~srcBitPattern : srcBitPattern;  break;
                    }
                }
                bitPatternIndex += FONT1_WIDTH*scaleWidth;

                if (charPos + 1 < numChars)
                {
                    // character that has been processed is not the last character so more bytes will be added to bitPattern[]
                    if((bitPatternIndex+(FONT1_WIDTH*scaleWidth)) > MAX_BIT_PATTERNS)
//                    if((bitPatternIndex+(2*FONT1_WIDTH*scaleWidth)) > MAX_BIT_PATTERNS)
//                    if((bitPatternIndex+(2*FONT1_WIDTH*scaleWidth)-1) >= MAX_BIT_PATTERNS)
                    {
                        // Next character will not fit into bitPattern[] - So transmit what we have
                        numBytesToWrite = bitPatternIndex;
                        Implementation_TransmitImage(bitPattern);
                        // And Start a new transmission
                        xPos += bitPatternIndex;
                        bitPatternIndex = 0;
                    }
                }
            }

            // All characters processed - transmit image
            numBytesToWrite = bitPatternIndex;
            Implementation_TransmitImage(bitPattern);

            // Start the next page
            xPos = startingXpos;
            yPos ++;
            pagesToWrite --;
        }
    }
}

#elif defined(TYPE_4_IMPLEMENTATION) || defined(TYPE_5_IMPLEMENTATION)

#ifndef _MSP430
// this function not size optimized beyond medium
#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
#endif
/*------------------------------------------------------------------------------
Function:		CommonFunctions_BuildImageEuropeanTFT
Parameters:		None	
Returns:		None
Description:	Called by WriteStringEurope(). Assembles the data to send to the
				display.
//------------------------------------------------------------------------------*/
static void CommonFunctions_BuildImageEuropeanTFT(void)
{		
	TUSIGN8 startingXpos = xPos;		
	numBytesToWrite = numChars*FONT1_WIDTH*scaleWidth;		
	TUSIGN8 startYpos = yPos*3;
	TUSIGN8 maxPages = pagesToWrite;
	numBytesToWrite *= 2;
	xPos *= 2;	
	startingXpos = xPos;
	writeText = eTRUE;

    #if defined (TYPE_5_IMPLEMENTATION) && defined(DISPLAY_SIZE_E)
    himax_reset_auto_increment();
    #endif

    // Find inverted character span
    characterInvertStartIndex = -1, characterInvertEndIndex = -1;
    if(invert = textInfo.field.invert)
    {
	    // String or line inversion			
        characterInvertStartIndex = 0;
        characterInvertEndIndex = numChars*FONT1_WIDTH*scaleWidth*2-1;
    }
    else
    {
        // Find start of invert
        for(charPos=0;charPos < numChars;charPos++)
        {
            // Check 'invert character' bit
            if ((*dataPtr & INVCHAR_MASK) == charPos+1)
            {
                characterInvertStartIndex = charPos*FONT1_WIDTH*scaleWidth*2;
                break;
            }
        }

        if(charPos < numChars)
        {
            // Find end of invert
            characterInvertEndIndex = characterInvertStartIndex;
            characterInvertEndIndex += FONT1_WIDTH*scaleWidth*2-1;
        }
    }

    while(pagesToWrite)
    {
	   for(TUSIGN8 tableIndex=0;tableIndex<3;tableIndex++)
	   {
			for(charPos=0;charPos < numChars;charPos++)
			{		
				bitPatternIndex = charPos*FONT1_WIDTH*scaleWidth*2;
						
				// Check for string or line inversion			
				invert = textInfo.field.invert;
	
				// Check 'invert character' bit
                if ((*dataPtr & INVCHAR_MASK) == charPos+1)
				{
					invert |= INVERT_CHAR;
				}

				// Get index for the start of the character within the font table
                if (0)
                {
                }
                #ifdef INCLUDE_LATIN1_PATTERNS
                else if (((*dataPtr & EXTENDED_PAGE_MASK) == LL_EXTENDED_CHARACTER_PAGE_LATIN1) && (i >= FIRST_LATIN1) && (i <= LAST_LATIN1))
                {
                    fontTableIndex =  (i - FIRST_LATIN1) + START_LATIN1;
                }
                #endif
                #ifdef INCLUDE_KATAKANA_PATTERNS
                else if (((*dataPtr & EXTENDED_PAGE_MASK) == LL_EXTENDED_CHARACTER_PAGE_KATAKANA) && (string[charPos] >= FIRST_KATAKANA) && (string[charPos] <= LAST_KATAKANA))
                {
                    fontTableIndex =  (string[charPos] - FIRST_KATAKANA) + START_KATAKANA;
                }
                #endif
                #ifdef INCLUDE_TURKISH_PATTERNS
                else if (((*dataPtr & EXTENDED_PAGE_MASK) == LL_EXTENDED_CHARACTER_PAGE_TURKISH) && (string[charPos] >= FIRST_TURKISH) && (string[charPos] <= LAST_TURKISH))
                {
                    fontTableIndex = (string[charPos] - FIRST_TURKISH) + START_TURKISH;
                }
                #endif
                #ifdef INCLUDE_RUSSIAN_PATTERNS
                else if (((*dataPtr & EXTENDED_PAGE_MASK) == LL_EXTENDED_CHARACTER_PAGE_RUSSIAN) && (string[charPos] >= FIRST_RUSSIAN) && (string[charPos] <= LAST_RUSSIAN))
                {
                    fontTableIndex = (string[charPos] - FIRST_RUSSIAN) + START_RUSSIAN;
                }
                #endif
                else if ((string[charPos] >= FIRST_EUROPEAN) && (string[charPos] <= LAST_EUROPEAN))
                {
                    fontTableIndex = (string[charPos] - FIRST_EUROPEAN) + START_EUROPEAN;
                }
                else
                {
                    fontTableIndex = ('?' - FIRST_EUROPEAN) + START_EUROPEAN;
                }
				fontTableIndex *= ((FONT1_WIDTH*2)-2);
				
                // Get bit patterns directly from font table
				if(textInfo.field.font == FONT1)
				{			
					for(i=0;i<((FONT1_WIDTH*2)-2);i++)
					{		
						if(tableIndex==2)
						{
							bitPattern[bitPatternIndex+i] = invert ? ~FontLower[fontTableIndex+i] : FontLower[fontTableIndex+i];
						}
						else if(tableIndex==1)
						{
							bitPattern[bitPatternIndex+i] = invert ? ~FontMid[fontTableIndex+i] : FontMid[fontTableIndex+i];					
						}
                        else
						{
							bitPattern[bitPatternIndex+i] = invert ? ~FontUpper[fontTableIndex+i] : FontUpper[fontTableIndex+i];					
						}
					}
					// Add spacer to character
					bitPattern[bitPatternIndex+i] = invert ? 0xFF : 0x00;
					// Add spacer to character
					bitPattern[bitPatternIndex+i+1] = invert ? 0xFF : 0x00;
				}
	#ifndef _MODULE_TESTING
				// Perform bit manipulation of font table elements
				else
				{		
					for(i=0,j=0;j<((FONT1_WIDTH*2)-2);i+=scaleWidth,j++)
					{	
						if(tableIndex==2)
						{
							srcBitPattern= FontLower[fontTableIndex+j];
						}
						else if(tableIndex==1)
						{
							srcBitPattern= FontMid[fontTableIndex+j];
						}
                        else
						{
							srcBitPattern= FontUpper[fontTableIndex+j];
						}

						scaledBitPattern=0;
						switch(textInfo.field.font)
						{
							case FONT2:
								Implementation_ScaleFont2();
								break;
							case FONT4:
								Implementation_ScaleFont4();							
								break;
							case FONT5:
								Implementation_ScaleFont5();					
								break;
                            default:
                                break;
						}
					}
	
					// Add spacer to character
					switch(textInfo.field.font)
					{
						case FONT2:
							bitPattern[bitPatternIndex+i] = invert ? 0xFF : 0x00;
							bitPattern[bitPatternIndex+i+1] = invert ? 0xFF : 0x00;
							break;
						case FONT4:
							for(temp=i;temp<(i+6);temp++)
							{
								bitPattern[bitPatternIndex+temp] = invert ? 0xFF : 0x00;
							}
							break;
						case FONT5:
							for(temp=i;temp<(i+8);temp++)
							{
								bitPattern[bitPatternIndex+temp] = invert ? 0xFF : 0x00;
							}		
							break;
                        default:
                            break;

					}
				}
	#endif		// #ifndef _MODULE_TESTING
				
				// Routine will add another x bytes to the transmit buffer, so
				// we need to ensure that the array limits will not be exceeded
                if(charPos+1 == numChars)
                {
                    numBytesToWrite = bitPatternIndex+(FONT1_WIDTH*scaleWidth*2);	
                    if(tableIndex==0)
                    {
                        yPos = startYpos + maxPages - pagesToWrite;
                    }
                    else if(tableIndex==1)
                    {
                        yPos = startYpos + (2*maxPages) - pagesToWrite;
                    }
                    else
                    {
                        yPos = startYpos + (3*maxPages) - pagesToWrite;								
                    }

                    textLineShift = 0;

                    if ((maxPages == 1) && (functionCode != WRITE_FRAME_TITLE_EUROPE) && (functionCode != WRITE_FRAME_TITLE_ASIAN))
                    {
                        switch (startYpos)
                        {
                        case 0:     case 1:     case 2:     textLineShift = ROW_SEPARATION * -2;    break;      // Line 0
                        case 3:     case 4:     case 5:     textLineShift = ROW_SEPARATION * -1;    break;      // Line 1
                        case 6:     case 7:     case 8:     textLineShift = ROW_SEPARATION *  0;    break;      // Line 2
                        case 9:     case 10:    case 11:    textLineShift = ROW_SEPARATION *  1;    break;      // Line 3
                        case 12:    case 13:    case 14:    textLineShift = ROW_SEPARATION *  2;    break;      // Line 4
                        case 15:    case 16:    case 17:    textLineShift = ROW_SEPARATION *  3;    break;      // Line 5
                        case 18:    case 19:    case 20:    textLineShift = ROW_SEPARATION *  4;    break;      // Line 6
                        default:                            textLineShift =  0;                     break;
                        }
                    }
                    Implementation_TransmitImage(bitPattern);
                }
			}
			xPos = startingXpos;		
			yPos++;					
		}
		pagesToWrite--;			
	}
    characterInvertEndIndex = -1;
    characterInvertStartIndex = -1;
	writeText = eFALSE;
}


#else
    #error Implementation not defined
#endif  // #if defined(TYPE_4_IMPLEMENTATION) || defined(TYPE_5_IMPLEMENTATION)


#endif  // #ifndef _BUILD_IMAGE_EUROPEAN_H_
