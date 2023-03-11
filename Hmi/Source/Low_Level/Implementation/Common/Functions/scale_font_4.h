#ifndef _SCALE_FONT_4_H_
#define _SCALE_FONT_4_H_

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
Function:		ScaleFont4
Parameters:		None	
Returns:		None
Description:	Scales the font from 7x5 to 21x15 (height x width)
//------------------------------------------------------------------------------*/
static void CommonFunctions_ScaleFont4(void)
{
	// Starting from bit1, all bits of each column of the bitmap are duplicated
	// three times to triple the height.
	if(pagesToWrite == 1)
	{
		scaledBitPattern |= (srcBitPattern&0x40);
		scaledBitPattern |= ((srcBitPattern&0x40)>>1);
		scaledBitPattern |= ((srcBitPattern&0x40)>>2);
		scaledBitPattern |= ((srcBitPattern&0x20)>>2);
		scaledBitPattern |= ((srcBitPattern&0x20)>>3);
		scaledBitPattern |= ((srcBitPattern&0x20)>>4);
		scaledBitPattern |= ((srcBitPattern&0x10)>>4);
	}
	else if(pagesToWrite == 2)
	{		
		scaledBitPattern |= ((srcBitPattern&0x10)<<3);
		scaledBitPattern |= ((srcBitPattern&0x10)<<2);
		scaledBitPattern |= ((srcBitPattern&0x08)<<2);
		scaledBitPattern |= ((srcBitPattern&0x08)<<1);
		scaledBitPattern |= (srcBitPattern&0x08);
		scaledBitPattern |= (srcBitPattern&0x04);
		scaledBitPattern |= ((srcBitPattern&0x04)>>1);
		scaledBitPattern |= ((srcBitPattern&0x04)>>2);
	}
	else
	{
		scaledBitPattern |= ((srcBitPattern&0x02)<<6);
		scaledBitPattern |= ((srcBitPattern&0x02)<<5);
		scaledBitPattern |= ((srcBitPattern&0x02)<<4);
		scaledBitPattern |= ((srcBitPattern&0x01)<<4);
		scaledBitPattern |= ((srcBitPattern&0x01)<<3);
		scaledBitPattern |= ((srcBitPattern&0x01)<<2);
	}
	// Copy each modified column to triple the width
	if(invert) {scaledBitPattern = ~scaledBitPattern;}
	for(temp=i;temp<(i+3);temp++)
	{
	  bitPattern[bitPatternIndex+temp] = scaledBitPattern;
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
Function:		ScaleFont4
Parameters:		None	
Returns:		None
Description:	Scales the font from 7x5 to 21x15 (height x width)
//------------------------------------------------------------------------------*/
static void CommonFunctions_ScaleFont4TFT(void)
{
	// Starting from bit1, all bits of each column of the bitmap are duplicated
	// three times to triple the height.
	if(pagesToWrite == 1)
	{
		scaledBitPattern |= (srcBitPattern&0x80);
		scaledBitPattern |= ((srcBitPattern&0x80)>>1);
		scaledBitPattern |= ((srcBitPattern&0x80)>>2);
		scaledBitPattern |= ((srcBitPattern&0x40)>>2);
		scaledBitPattern |= ((srcBitPattern&0x40)>>3);
		scaledBitPattern |= ((srcBitPattern&0x40)>>4);
		scaledBitPattern |= ((srcBitPattern&0x20)>>4);
		scaledBitPattern |= ((srcBitPattern&0x20)>>5);
	}
	else if(pagesToWrite == 2)
	{		
		scaledBitPattern |= ((srcBitPattern&0x20)<<2);
		scaledBitPattern |= ((srcBitPattern&0x10)<<2);
		scaledBitPattern |= ((srcBitPattern&0x10)<<1);
		scaledBitPattern |= ((srcBitPattern&0x10)<<0);
		scaledBitPattern |= ((srcBitPattern&0x08));
		scaledBitPattern |= ((srcBitPattern&0x08)>>1);
		scaledBitPattern |= ((srcBitPattern&0x08)>>2);
		scaledBitPattern |= ((srcBitPattern&0x04)>>2);		
	}
	else
	{
		scaledBitPattern |= ((srcBitPattern&0x04)<<5);
		scaledBitPattern |= ((srcBitPattern&0x04)<<4);
		scaledBitPattern |= ((srcBitPattern&0x02)<<4);
		scaledBitPattern |= ((srcBitPattern&0x02)<<3);
		scaledBitPattern |= ((srcBitPattern&0x02)<<2);
		scaledBitPattern |= ((srcBitPattern&0x01)<<2);		
		scaledBitPattern |= ((srcBitPattern&0x01)<<1);		
		scaledBitPattern |= ((srcBitPattern&0x01)<<0);				
	}
	// Copy each modified column to triple the width
	if(invert) {scaledBitPattern = ~scaledBitPattern;}
	for(temp=i;temp<(i+3);temp++)
	{
	  bitPattern[bitPatternIndex+temp] = scaledBitPattern;
	}
}

#else
    #error Implementation not defined
#endif  // #if defined(TYPE_4_IMPLEMENTATION) || defined(TYPE_5_IMPLEMENTATION)

#endif  // #ifndef _SCALE_FONT_4_H_
