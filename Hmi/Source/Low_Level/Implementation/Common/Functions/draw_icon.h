#ifndef _DRAW_ICON_H_
#define _DRAW_ICON_H_

// Only include for builds that do not support NV
#ifndef _NONVOL_IMPLEMENTATION

#ifndef _MSP430
// this function not size optimized beyond medium
#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
#endif
/*------------------------------------------------------------------------------
Function:				DrawIcon
Parameters:				None	
Returns:				None
Description:			Draws an icon that is held in memory
						
Packet Format:
FUNCTION CODE, LENGTH, XPOS, YPOS, ICON NUMBER, CHECKSUM
//------------------------------------------------------------------------------*/
static void CommonFunctions_DrawIcon(void)
{
	// Get icon number
	if(*dataPtr <= ICON_PART_MAGNIFY_C2)
	{
		numBytesToWrite = LARGE_ICON_SPLIT_WIDTH;
		for(i=0;i<numBytesToWrite;i++)
		{
			bitPattern[i] = animatedIconTable[*dataPtr][i];
		}
		Implementation_TransmitImage(bitPattern);		
	}
	else if((*dataPtr >= ICON_NUMBER_EDIT) && (*dataPtr <= ICON_LOCK))
	{
		numBytesToWrite = SMALL_ICON_WIDTH;
		for(i=0;i<numBytesToWrite;i++)
		{
			bitPattern[i] = smallIconTable[*dataPtr-ICON_NUMBER_EDIT][i];
		}		
		Implementation_TransmitImage(bitPattern);
	}
	else
	{
		replyPacket.bit.procError = 1;
	}
}

#endif  // #ifndef _NONVOL_IMPLEMENTATION

#endif  // #ifndef _DRAW_ICON_H_
