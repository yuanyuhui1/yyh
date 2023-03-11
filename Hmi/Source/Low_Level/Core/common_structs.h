// M Ashford - Suppress MISRA Diagnostics
// MISRA Rule 13 (The basic types of char, int, short, long, float, and double
// should not be used, but specific-length equivalents should be typedef’d for the
// specific compiler, and these type names used in the code.
#pragma diag_suppress=Pm011


//----------------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2004.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System					Low Level HMI

 Description			Defines all structures used within the code

 Author					Martin Ashford
 Remarks
 $Id $
 </pre>
*/
//----------------------------------------------------------------------------------------------------------

#ifndef	_COMMON_STRUCTS_H
#define _COMMON_STRUCTS_H

/* ----------------------- STRUCTURE DEFS ------------------------- */
typedef union	{
	struct
	{
		unsigned char	yPos:	3;
		unsigned char	font:	3;
		unsigned char	invert:	2;
	}field;
	unsigned char	byte;	
}_text_info;


typedef union	{
	struct
	{
		unsigned char	startPage:	3;
		unsigned char	endPage:	3;
		unsigned char	error:		2;
	}field;
	unsigned char	byte;	
}_blank_area;

typedef union	{
	struct
	{
		unsigned char	commsError:		1;
		unsigned char	dispError:		1;
		unsigned char	switchError:	1;
		unsigned char	procError:		1;
		unsigned char	sw0:			1;
		unsigned char	sw1:			1;
		unsigned char	sw2:			1;
		unsigned char	sw3:			1;						
	}bit;
	struct
	{
		unsigned char	errors:			4;
		unsigned char	switches:		4;
	}field;	
	unsigned char	byte;	
}_reply_packet;

typedef union	{
	struct
	{
		unsigned char	invertDisplay:		1;
		unsigned char	displayOn:			1;
		unsigned char	powerSave:			1;
		unsigned char	backlightOn:		1;
		unsigned char	allPixelsOn:		1;
		unsigned char	testDisplay:		1;
		unsigned char	defaultCont:		1;
		unsigned char	resetDisplay:		1;		
	}bit;
	unsigned char	byte;	
}_setup_display_info;


// DO NOT CHANGE ORDER OF THIS UNION!!!!!!
typedef union	{
	struct
	{
		unsigned char	buffOverflow:	1;
		unsigned char	funcCodeError:	1;
		unsigned char	lengthError:	1;
		unsigned char	synchError:		1;		
		unsigned char	brkError:		1;
		unsigned char	overrunError:	1;
		unsigned char	parityError:	1;		// Not used
		unsigned char	framingError:	1;		
	}bit;
	struct
	{
		unsigned char	pktErrors:		4;
		unsigned char	commErrors:		4;		
	}field;
	unsigned char	byte;	
}_intrpt_errors;

#if defined (TYPE_4_IMPLEMENTATION) || defined (TYPE_5_IMPLEMENTATION)
typedef const struct _ICON_INFO
{
    const TUSIGN8 *patternMap;
    TUSIGN8 xs;         // horizontal offset with respect to icon start position
    TUSIGN8 ys;
    TUSIGN8 width;      // vertical offset with respect to icon start position
    TUSIGN8 height;
}ICON_INFO;
#endif

#endif


// M. Ashford - Enable MISRA Diagnostics
#pragma diag_default=Pm011
