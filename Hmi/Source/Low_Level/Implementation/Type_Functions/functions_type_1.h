#ifndef _FUNCTIONS_TYPE_1_H_
#define _FUNCTIONS_TYPE_1_H_

/*------------------------------------------------------------------------------
Function:				DrawIcon
Parameters:				None	
Returns:				None
Description:			Draws an icon that is held in memory
						
Packet Format:
FUNCTION CODE, LENGTH, XPOS, YPOS, ICON NUMBER, CHECKSUM
//------------------------------------------------------------------------------*/
static void Implementation_DrawIcon_Type1_NV(void)
{
    TUSIGN16 readAddr;

#if (BLOCK_INFO_ALLOCATED_SIZE >= SMALL_ICON_ALLOCATED_SIZE) && (BLOCK_INFO_ALLOCATED_SIZE >= ANIMATED_ICON_ALLOCATED_SIZE)
    TUSIGN8  nonVolData[BLOCK_INFO_ALLOCATED_SIZE];
#elif (SMALL_ICON_ALLOCATED_SIZE >= BLOCK_INFO_ALLOCATED_SIZE) && (SMALL_ICON_ALLOCATED_SIZE >= ANIMATED_ICON_ALLOCATED_SIZE)
    TUSIGN8  nonVolData[SMALL_ICON_ALLOCATED_SIZE];
#else
    TUSIGN8  nonVolData[ANIMATED_ICON_ALLOCATED_SIZE];
#endif

	if(*dataPtr <= ICON_PART_MAGNIFY_C2)
	{
        // animated Icons
		numBytesToWrite = LARGE_ICON_SPLIT_WIDTH;
        // Get the Block Info
        readAddr = GetBlockInfo(BLOCK_ID_ANIMATED_ICONS, nonVolData);
    }
	else if((*dataPtr >= ICON_NUMBER_EDIT) && (*dataPtr <= ICON_LOCK))
    {
        // small Icons
		numBytesToWrite = SMALL_ICON_WIDTH;
        // Get the Block Info
        readAddr = GetBlockInfo(BLOCK_ID_SMALL_ICONS, nonVolData);
    }
    else
    {
        // invalid icon ID
        readAddr = 0;
	}

    if (readAddr)
    {
        // Extract the start address of the BLOCK_DATA (BLOCK_START)
        readAddr  = ((TUSIGN16)nonVolData[NONVOL_BLOCK_INFO__BLOCK_START_MSBYTE] << 8) + nonVolData[NONVOL_BLOCK_INFO__BLOCK_START_LSBYTE];


        // Increment address to the start of the required Icon
        // NOTE "NONVOL_BLOCK_INFO__FIRST_CHAR_MSBYTE" is not allowed for in calculation as it is assumed to be zero
        readAddr  += ((*dataPtr - nonVolData[NONVOL_BLOCK_INFO__FIRST_CHAR_LSBYTE]) * (TUSIGN16)nonVolData[NONVOL_BLOCK_INFO__CHAR_WIDTH_BYTE]);
        // Read the icon data
   		ReadNonVolData(readAddr, bitPattern, numBytesToWrite);
        // Write the icon to the display
		Implementation_TransmitImage(bitPattern);		
    }
    else
    {
		replyPacket.bit.procError = 1;
    }
}

#endif  // #ifndef _FUNCTIONS_TYPE_1_H_
