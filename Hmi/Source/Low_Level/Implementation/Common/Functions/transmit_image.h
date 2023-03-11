#ifndef _TRANSMIT_IMAGE_H_
#define _TRANSMIT_IMAGE_H_

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
Function:			TransmitImage
Parameters:		
	imageByte		Pointer to the data to be written to the display
Returns:			None
Description:		Writes data to the display.
//------------------------------------------------------------------------------*/
static void CommonFunctions_TransmitImage(TUSIGN8 *imageByte)
{		
#if defined (TYPE_2_IMPLEMENTATION) || defined(TYPE_3_IMPLEMENTATION)
    TUSIGN8 ucColumnOffset;
#endif

	// Error checking
	if(numBytesToWrite > (MAX_COMMON_HMI_WIDTH-xPos))
	{
		numBytesToWrite = MAX_COMMON_HMI_WIDTH-xPos;
		replyPacket.bit.procError = 1;
	}
	if(yPos >= MAX_PAGES)
	{
		yPos = 0;
		replyPacket.bit.procError = 1;
	}	
	if(xPos >= MAX_COMMON_HMI_WIDTH)
	{
		xPos = 0;
		replyPacket.bit.procError = 1;
	}		
	
    Implementation_SetTransmitImageStartPosition(xPos);

    i=0;
    while(i < numBytesToWrite)
    {
        LcdWrite(imageByte[i]);
        i++;
    }

    Implementation_DisableLCD();
}

#endif  // #if defined(TYPE_1_IMPLEMENTATION) || defined(TYPE_2_IMPLEMENTATION) || defined(TYPE_3_IMPLEMENTATION)

#endif  // #ifndef _TRANSMIT_IMAGE_H_
