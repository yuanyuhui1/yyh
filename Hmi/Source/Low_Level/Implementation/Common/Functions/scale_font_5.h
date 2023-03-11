#ifndef _SCALE_FONT_5_H_
#define _SCALE_FONT_5_H_

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
Function:		ScaleFont5
Parameters:		None	
Returns:		None
Description:	Scales the font from 7x5 to 28x20 (height x width)
//------------------------------------------------------------------------------*/
static void CommonFunctions_ScaleFont5(void)
{
	// Starting from bit1, all bits of each column of the bitmap are duplicated
	// four times to quadruple the height.
	if(pagesToWrite==1)
	{		
		for(temp=0;temp<4;temp++)
		{
			scaledBitPattern |= ((srcBitPattern&0x40)>>temp);
		}					
		for(temp=3;temp<6;temp++)
		{
			scaledBitPattern |= ((srcBitPattern&0x20)>>temp);
		}		
	}
	else if(pagesToWrite==2)
	{	
		scaledBitPattern |= ((srcBitPattern&0x20)<<2);
		for(temp=0;temp<3;temp++)
		{
			scaledBitPattern |= ((srcBitPattern&0x10)<<temp);
		}		
		scaledBitPattern |= ((srcBitPattern&0x10)>>1);
		for(temp=1;temp<4;temp++)
		{
			scaledBitPattern |= ((srcBitPattern&0x08)>>temp);
		}		
	}
	else if(pagesToWrite==3)
	{	
		scaledBitPattern |= ((srcBitPattern&0x08)<<4);		
		for(temp=1;temp<5;temp++)
		{
			scaledBitPattern |= ((srcBitPattern&0x04)<<temp);
		}		
		scaledBitPattern |= ((srcBitPattern&0x02)<<1);
		scaledBitPattern |= (srcBitPattern&0x02);
		scaledBitPattern |= ((srcBitPattern&0x02)>>1);
	}
	else
	{
		scaledBitPattern |= ((srcBitPattern&0x02)<<6);
		for(temp=3;temp<7;temp++)
		{
			scaledBitPattern |= ((srcBitPattern&0x01)<<temp);
		}		
	}
	
	// Copy each modified column to quadruple the width
	if(invert){scaledBitPattern = ~scaledBitPattern;}
	for(temp=i;temp<(i+4);temp++)
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
Function:		ScaleFont5
Parameters:		None	
Returns:		None
Description:	Scales the font from 7x5 to 28x20 (height x width)
//------------------------------------------------------------------------------*/
static void CommonFunctions_ScaleFont5TFT(void)
{
	// Starting from bit1, all bits of each column of the bitmap are duplicated
	// four times to quadruple the height.
	if(pagesToWrite==1)
	{		
		scaledBitPattern |= (srcBitPattern&0x80);
		scaledBitPattern |= ((srcBitPattern&0x80)>>1);
		scaledBitPattern |= ((srcBitPattern&0x80)>>2);
		scaledBitPattern |= ((srcBitPattern&0x80)>>3);
		scaledBitPattern |= ((srcBitPattern&0x40)>>3);
		scaledBitPattern |= ((srcBitPattern&0x40)>>4);
		scaledBitPattern |= ((srcBitPattern&0x40)>>5);
		scaledBitPattern |= ((srcBitPattern&0x40)>>6);
	}
	else if(pagesToWrite==2)
	{	
		scaledBitPattern |= ((srcBitPattern&0x20)<<2);
		scaledBitPattern |= ((srcBitPattern&0x20)<<1);
		scaledBitPattern |= ((srcBitPattern&0x20)>>0);
		scaledBitPattern |= ((srcBitPattern&0x20)>>1);
		scaledBitPattern |= ((srcBitPattern&0x10)>>1);
		scaledBitPattern |= ((srcBitPattern&0x10)>>2);
		scaledBitPattern |= ((srcBitPattern&0x10)>>3);
		scaledBitPattern |= ((srcBitPattern&0x10)>>4);
	}
	else if(pagesToWrite==3)
	{	
		scaledBitPattern |= ((srcBitPattern&0x08)<<4);
		scaledBitPattern |= ((srcBitPattern&0x08)<<3);
		scaledBitPattern |= ((srcBitPattern&0x08)<<2);
		scaledBitPattern |= ((srcBitPattern&0x08)<<1);
		scaledBitPattern |= ((srcBitPattern&0x04)<<1);
		scaledBitPattern |= ((srcBitPattern&0x04)<<0);
		scaledBitPattern |= ((srcBitPattern&0x04)>>1);
		scaledBitPattern |= ((srcBitPattern&0x04)>>2);
	}
	else
	{
		scaledBitPattern |= ((srcBitPattern&0x02)<<6);
		scaledBitPattern |= ((srcBitPattern&0x02)<<5);
		scaledBitPattern |= ((srcBitPattern&0x02)<<4);
		scaledBitPattern |= ((srcBitPattern&0x02)<<3);
		scaledBitPattern |= ((srcBitPattern&0x01)<<3);
		scaledBitPattern |= ((srcBitPattern&0x01)<<2);
		scaledBitPattern |= ((srcBitPattern&0x01)<<1);
		scaledBitPattern |= ((srcBitPattern&0x01)<<0);
	}
	
	// Copy each modified column to quadruple the width
	if(invert){scaledBitPattern = ~scaledBitPattern;}
	for(temp=i;temp<(i+4);temp++)
	{
	  bitPattern[bitPatternIndex+temp] = scaledBitPattern;
	}
}

#else
    #error Implementation not defined
#endif  // #if defined(TYPE_4_IMPLEMENTATION) || defined(TYPE_5_IMPLEMENTATION)

#endif  // #ifndef _SCALE_FONT_5_H_
