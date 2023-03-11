/*------------------------------------------------------------------------------
Function:				DrawSliderBar
Parameters:				None	
Returns:				None
Description:			Draw a slider bar, at a given position, on the right hand
						edge of the screen. A user defined number is drawn above
						the slider bar (top right of screen).
						
Packet Format:
FUNCTION CODE, LENGTH, NUMBER, YPOS, PIXEL PATTERNS...(3 or 6), CHECKSUM
//------------------------------------------------------------------------------*/
static void CommonFunctions_DrawSliderBar(void)
{
    // Draw vertical line
	numBytesToWrite = 3;
	xPos = MAX_CONFIG_WIDTH;
	for(yPos=2; yPos<6; yPos++)
	{
		bitPattern[0] = bitPattern[2] = 0x00;
		bitPattern[1] = 0xFF;
		Implementation_TransmitImage(bitPattern);
	}	
	bitPattern[0] = bitPattern[2] = 0x00;
	bitPattern[1] = 0xFE;
	yPos = 1;
	Implementation_TransmitImage(bitPattern);
	bitPattern[0] = bitPattern[2] = 0x00;
	bitPattern[1] = 0x7F;
	yPos = 6;
	Implementation_TransmitImage(bitPattern);

	// Draw scroll bar
	pagesToWrite = (lengthOfData-2)/3;
	numBytesToWrite = 3;
	yPos = dataPtr[1];
	xPos = MAX_CONFIG_WIDTH;	
	for(i=0;i<numBytesToWrite;i++)
	{
		bitPattern[i]=dataPtr[2+i];
	}
	Implementation_TransmitImage(bitPattern);	
	if(pagesToWrite==2)
	{
		for(i=0;i<numBytesToWrite;i++)
		{
			bitPattern[i]=dataPtr[5+i];
		}
		yPos++;
		Implementation_TransmitImage(bitPattern);
	}
	
	// Draw number in top right
	if(dataPtr[0] > 99)
	{
		replyPacket.bit.procError = 1;
	}
	else
	{
		bitPattern[0] = 0;
		numBytesToWrite = 1;
		yPos=0;
		xPos = MAX_COMMON_HMI_WIDTH-FONT1_WIDTH+1;
		Implementation_TransmitImage(bitPattern);	

		fontTableIndex = '0' + (dataPtr[0]%10);

        #if defined(TYPE_2_IMPLEMENTATION) || defined(TYPE_3_IMPLEMENTATION)
		fontTableIndex -= FIRST_EUROPEAN;
        #else
		fontTableIndex -= FONT_TABLE_FIRST_CHARACTER;
        #endif

        Implementation_DrawSliderBarLowerDigit();
	
		if(dataPtr[0] > 9)
		{
			fontTableIndex = '0' + (dataPtr[0]/10);

            #if defined(TYPE_2_IMPLEMENTATION) || defined(TYPE_3_IMPLEMENTATION)
            fontTableIndex -= FIRST_EUROPEAN;
            #else
            fontTableIndex -= FONT_TABLE_FIRST_CHARACTER;
            #endif

            Implementation_DrawSliderBarUpperDigit();
		}
		else
		{
		  	 // Blank x10 area (bug moving from two digit item to single digit item)
			for(i=0;i<FONT1_WIDTH;i++)
			{
				bitPattern[i] = 0x00;
			}
			numBytesToWrite = FONT1_WIDTH;
			xPos = MAX_COMMON_HMI_WIDTH-(FONT1_WIDTH*2);
			yPos=0;
			Implementation_TransmitImage(bitPattern);	
		}
	}
}
