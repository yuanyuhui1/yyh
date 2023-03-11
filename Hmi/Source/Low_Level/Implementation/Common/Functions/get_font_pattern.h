#ifndef _GET_FONT_PATTERN_H_
#define _GET_FONT_PATTERN_H_

#if defined(TYPE_1_IMPLEMENTATION) || defined(TYPE_2_IMPLEMENTATION) || defined(TYPE_3_IMPLEMENTATION)

#ifdef _NONVOL_IMPLEMENTATION

#ifndef _MSP430
// this function not size optimized beyond medium
#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
#endif
/*------------------------------------------------------------------------------
Function:			GetFontPattern
Parameters:		
	tableIndex		Index of the font pattern, as if the table was sequential
Returns:			The bit pattern required for index
Description:		Gets the correct font pattern.
					NOTE: Table split into two tables for asian, one in info
					memory & one in main memory. Table is sequential for non-asian
					implementation
//------------------------------------------------------------------------------*/
static TUSIGN8 CommonFunctions_GetFontPattern(TUSIGN16 tableIndex)
{
	// Info memory (max = 256/5 = 51 chars)
	return (tableIndex < 255) ? FontTableLower[tableIndex] : FontTableUpper[tableIndex-255];
}

#else   // else condition for  #ifdef _NONVOL_IMPLEMENTATION

/*------------------------------------------------------------------------------
Function:			GetFontPattern
Parameters:		
	tableIndex		Index of the font pattern, as if the table was sequential
Returns:			The bit pattern required for index
Description:		Gets the correct font pattern.
					NOTE: Table split into two tables for asian, one in info
					memory & one in main memory. Table is sequential for non-asian
					implementation
//------------------------------------------------------------------------------*/
static TUSIGN8 CommonFunctions_GetFontPattern(TUSIGN16 tableIndex)
{
	return	FontTable[tableIndex];
}

#endif  // end of else condition for  #ifdef _NONVOL_IMPLEMENTATION

#endif  // #if defined(TYPE_1_IMPLEMENTATION) || defined(TYPE_2_IMPLEMENTATION) || defined(TYPE_3_IMPLEMENTATION)

#endif  // #ifndef _GET_FONT_PATTERN_H_
