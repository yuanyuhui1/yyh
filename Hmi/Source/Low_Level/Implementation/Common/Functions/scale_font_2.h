#ifndef _SCALE_FONT_2_H_
#define _SCALE_FONT_2_H_

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
Function:		ScaleFont2
Parameters:		None	
Returns:		None
Description:	Scales the font from 7x5 to 14x5 (height x width)
//------------------------------------------------------------------------------*/
static void CommonFunctions_ScaleFont2(void)
{
	// Starting from bit1, all bits of each column of the bitmap are duplicated
	// two times to double the height.
	if(pagesToWrite == 1)
	{
		scaledBitPattern |= (srcBitPattern&0x40);
		scaledBitPattern |= ((srcBitPattern&0x40)>>1);
		scaledBitPattern |= ((srcBitPattern&0x20)>>1);
		scaledBitPattern |= ((srcBitPattern&0x20)>>2);
		scaledBitPattern |= ((srcBitPattern&0x10)>>2);
		scaledBitPattern |= ((srcBitPattern&0x10)>>3);
		scaledBitPattern |= ((srcBitPattern&0x08)>>3);
	}
	else
	{					
		scaledBitPattern |= ((srcBitPattern&0x08)<<4);
		scaledBitPattern |= ((srcBitPattern&0x04)<<4);
		scaledBitPattern |= ((srcBitPattern&0x04)<<3);
		scaledBitPattern |= ((srcBitPattern&0x02)<<3);
		scaledBitPattern |= ((srcBitPattern&0x02)<<2);
		scaledBitPattern |= ((srcBitPattern&0x01)<<2);
		scaledBitPattern |= ((srcBitPattern&0x01)<<1);
	}

	if(invert)  {scaledBitPattern = ~scaledBitPattern;}
	
	bitPattern[bitPatternIndex+i] = scaledBitPattern;
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
Function:		ScaleFont2
Parameters:		None	
Returns:		None
Description:	Scales the font from 7x5 to 14x5 (height x width)
//------------------------------------------------------------------------------*/
static void CommonFunctions_ScaleFont2TFT(void)
{
	// Starting from bit1, all bits of each column of the bitmap are duplicated
	// two times to double the height.
	if(pagesToWrite == 1)
	{
		scaledBitPattern |= (srcBitPattern&0x80);
		scaledBitPattern |= ((srcBitPattern&0x80)>>1);
		scaledBitPattern |= ((srcBitPattern&0x40)>>1);
		scaledBitPattern |= ((srcBitPattern&0x40)>>2);
		scaledBitPattern |= ((srcBitPattern&0x20)>>2);
		scaledBitPattern |= ((srcBitPattern&0x20)>>3);
		scaledBitPattern |= ((srcBitPattern&0x10)>>3);
		scaledBitPattern |= ((srcBitPattern&0x10)>>4);		
	}
	else
	{					
		scaledBitPattern |= ((srcBitPattern&0x08)<<4);
		scaledBitPattern |= ((srcBitPattern&0x08)<<3);		
		scaledBitPattern |= ((srcBitPattern&0x04)<<3);
		scaledBitPattern |= ((srcBitPattern&0x04)<<2);
		scaledBitPattern |= ((srcBitPattern&0x02)<<2);
		scaledBitPattern |= ((srcBitPattern&0x02)<<1);
		scaledBitPattern |= ((srcBitPattern&0x01)<<1);
		scaledBitPattern |= (srcBitPattern&0x01);
	}

	if(invert)  {scaledBitPattern = ~scaledBitPattern;}
	
	bitPattern[bitPatternIndex+i] = scaledBitPattern;
}

#else
    #error Implementation not defined
#endif  // #if defined(TYPE_4_IMPLEMENTATION) || defined(TYPE_5_IMPLEMENTATION)

#endif  // #ifndef _SCALE_FONT_2_H_
