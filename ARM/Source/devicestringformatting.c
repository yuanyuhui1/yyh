//--------------------------------------------------------------------------------------------------
/*!
 Copyright              Copyright ABB, 2006.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         EMF
 Module         devicestringformatting.c
 Description    String formatting functions used by the Device subsystem.

 Remarks

*/
//--------------------------------------------------------------------------------------------------
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "devicestringformatting.h"
#include "../../embos/interface/rtos.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "System/Interface/assert.h"
// Hmi files included for date/time definitions and types
#include "Hmi/Source/keystateDefinitions.h"
#include "Hmi/Source/compileTimeDefinitions.h"
#include "Hmi/Source/FrameTableDefinitions.h"
#include "Hmi/Source/Adjuster.h"
#include "T_Unit/Interface/t_unit.h"
#include "Hmi/Interface/HMI.h"
// For device subsystem based definitions
//#include "..\interface\device.h"
// String handling
#include <stdio.h>

// Fix for the non-working 32 bit print formatting function.
// Two tables are used in the decimal to ascii conversion.
// The table max32BitColumnValue is based on the maximum value for an internal representation of a 32bit number,
// 4294967295. As the conversion starts from the left hand side and highest value column, the maximum allowable
// value for the final column is limited to four. All other columns are allowed to range between 0 and 9. The values
// in the column valueSteps32Bit, are the the base 10 values of each column of the 32bit number.
// These are mulitplied against the values in the former table. If the value being converted
// is greater than the product, the next character is found. The product is then subtracted from the number being converted.
// This continues until all characters are converted.

// Private function declarations
TUSIGN8 Get32BitCharacterCountDeviceStringFormatting(TUSIGN32 *pValue32) ;
TUSIGN16 AddTwoCharacterCodeDeviceStringFormatting(TUSIGN8 characterCode, TUSIGN8 *pBuffer) ;
TUSIGN16 StringFormatHmiTimeDeviceStringFormatting(TIMEANDDATE *pTimeAndDate, TUSIGN8 *pBuffer) ;

// Industrial only private function declarations
#ifdef _EMF_INDUSTRIAL
TUSIGN16 StringFormat_DATE(TDate *pDate, TUSIGN8 *pBuffer) ;
#endif

// Data arrays used during string conversion
const TUSIGN32 valueSteps32Bit[] =   {
                                                    1000000000 ,            // 10
                                                    100000000 ,             // 9
                                                    10000000 ,              // 8
                                                    1000000 ,               // 7
                                                    100000 ,                // 6
                                                    10000 ,                 // 5
                                                    1000 ,                  // 4
                                                    100 ,                   // 3
                                                    10 ,                    // 2
                                                    1 ,                     // 1
                                                } ;

// Table of max character values when the uppermost character is unknown
const TUSIGN8 max32BitColumnValue[] = {
                                                        4,                  // 10
                                                        9,                  // 9
                                                        9 ,                 // 8
                                                        9 ,                 // 7
                                                        9 ,                 // 6
                                                        9 ,                 // 5
                                                        9 ,                 // 4
                                                        9 ,                 // 3
                                                        9 ,                 // 2
                                                        9                   // 1
                                                    } ;

// Private definitions
#define MAX_32BIT_CHARACTERS 10

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Gets the number of ascii characters in a thirty two bit number.
 \author GL
 \date   2006-02-27
 \param  pValue32 - pointer to the thirty two bit number having its characters counted.
 \return > 0 The number of characters, = 0 failed.
 <pre>
    > 0 success
    = 0 failed
 </pre>
 \warning
 \test
 \n by: GL
 \n intention: 1. complete code covarage
 \n result module test: 2006-10-03
 \n result Lint Level 3: // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN8 Get32BitCharacterCountDeviceStringFormatting(TUSIGN32 *pValue32)
{
    // The possible number of characters that the passed value has been checked against
    TUSIGN8 checkedCharacters = 0 ;
    // Set when the number of characters is determined
    TBOOL result = eFALSE ;

    do
    {
        if (*pValue32 >= valueSteps32Bit[checkedCharacters])
        {
		    result = eTRUE ;
        }
        else
        {
            checkedCharacters++ ;
        }
    }
    while (checkedCharacters < ( MAX_32BIT_CHARACTERS - 1 ) && result == eFALSE) ;
    return(MAX_32BIT_CHARACTERS - checkedCharacters) ;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Converts a thirty two bit number to ascii data. Calls get character count to initialise the character count
 \brief  variable, then uses the tables of data valueSteps32Bit and max32BitColumnValue
 \brief  to calculate the ascii values for each column. This was written to replace the IAR sprintf call
 \brief  which doesn't work with whole numbers above sixteen bits.
 \author GL
 \date   2006-02-27
 \param  pValue32 - Pointer to the 32 bit number being string formatted.
 \param  pBuffer - The string buffer to be loaded with the formatted 32bit information
 \return > 0 The number of characters added to the buffer, = 0 failed.
 <pre>
    > 0 success
    = 0 failed
 </pre>
 \warning
 \test
 \n by: GL
 \n intention: 1. complete code covarage
 \n result module test: 2006-10-03
 \n result Lint Level 3: // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN8 StringFormat32BitDeviceStringFormatting(TUSIGN32 *pValue32, TUSIGN8 *pBuffer, TBOOL nullTerminate)
{
    TUSIGN8 characterCount ;
    TUSIGN8 charactersAdded = 0 ;
    TUSIGN8 valuesChecked = 0 ;
    TUSIGN32 tempValue32 ;
    TUSIGN8 characterPosition = 0 ;
    TBOOL newChar = eFALSE ;

    VIP_ASSERT(pValue32) ;
    VIP_ASSERT(pBuffer) ;

    // Get the number of characters for the string equivalent of the 32 bit number
    characterCount = Get32BitCharacterCountDeviceStringFormatting(pValue32) ;
    // And zero the destination value
    tempValue32 = *pValue32 ;

    // Don't proceed if no characters need to be added
    if (characterCount > 0)
    {
        // Get the position of the first character for this id - this will be the highest power of ten for the id
        characterPosition = MAX_32BIT_CHARACTERS - characterCount ;
        // Add each character in descending order of powers of ten
        for (charactersAdded = 0 ; charactersAdded < characterCount ; charactersAdded++)
        {
            // Assume no character added initially
            newChar = eFALSE ;
            // Multiply each possible value for the character by the appropriate power of ten for this column
            for (valuesChecked = max32BitColumnValue[characterPosition] ; valuesChecked > 0 ; valuesChecked--)
            {
                // If the value is greater than or equal to the number being converted . . .
				if (tempValue32 >= (valuesChecked * valueSteps32Bit[characterPosition]))
                {
                    // The character value is the next value in the table
					newChar = eTRUE ;
                    pBuffer[charactersAdded] = valuesChecked + 0X30 ;
                    tempValue32 -= valuesChecked * valueSteps32Bit[characterPosition] ;
                    break ;
                }
            }
            // If no character is found, a zero is added in its place
			if (newChar == eFALSE)
            {
                pBuffer[charactersAdded] = 0X30 ;
            }
            // Look at the next character column
			characterPosition++ ;
        }
        // Check if a null character is required at the end of the string
        if (nullTerminate == eTRUE)
        {
            // Terminate the string with a null character after the conversion is complete
            pBuffer[charactersAdded] = '\0' ;
        }
    }
    return(charactersAdded) ;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Converts an ascii format character string into a thirty-two bit number. Any leading zeroes
 \brief  are rejected, and a total length of 11 bytes is checked (10 characters max for 0xFFFFFFFF).
 \brief  Should any spaces find their way into the string after the first valid character, these will
 \brief  be converted to zeroes.
 \author GL
 \date   2007-02-7
 \param  pBuffer - The string buffer to be loaded with the formatted 32bit information
 \param  pValue32 - Pointer to the 32 bit number being string formatted.
 \param  Length - If non-zero, a non-null terminated string is assumed. Only the characters within the
 \param  length are then converted.
 \return Framework code, OK or otherwise
 <pre>
    OK success
    <> OK failed
 </pre>
 \warning
 \test
 \n by: GL
 \n intention: 1. complete code covarage
 \n result module test: 2006-10-03
 \n result Lint Level 3: // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 NumberFormat32BitDeviceStringFormatting(TUSIGN8 *pBuffer, TUSIGN32 *pValue32, TUSIGN8 length)
{
    TUSIGN16 result = OK ;
    TUSIGN8 characterCount = 0 ;
    TBOOL validCharacterFound = eFALSE ;
    TUSIGN8 decimalPlaces = 0 ;
    TUSIGN8 charactersProcessed ;
    TUSIGN8 decimalValue ;
    TUSIGN8 power ;
    TUSIGN8 charactersToProcess ;

    TUSIGN8 Buffer[20] ;
    memcpy(Buffer,pBuffer,10) ;

    VIP_ASSERT(pBuffer) ;
    VIP_ASSERT(pValue32) ;

    // Zero the result
    *pValue32 = 0 ;

    // Check if a null character is present
    if (length)
    {
        charactersToProcess = length ;
    }
    else
    {
        charactersToProcess = MAX_32BIT_CHARACTERS ;
    }

    // Omit any leading zeroes or spaces . . .
    for (characterCount = 0 ; characterCount < charactersToProcess && validCharacterFound == eFALSE ;)
    {
        if (pBuffer[characterCount] != 0x30 && pBuffer[characterCount] != 0x20)
        {
            validCharacterFound = eTRUE ;
        }
        else
        {
            // Only point at the next character if a zero is found - this index will be used elsewhere otherwise
            characterCount++ ;
        }
    }
    // Check if we have a number other than zero . . .
    if (validCharacterFound == eTRUE)
    {
        // Starting with the first valid character, work out how many characters are in the string form number
        // This determines how many decimal places will be used
        if (length)
        {
            decimalPlaces = length - characterCount ;
        }
        else
        {
            decimalPlaces = strlen((const char *)&pBuffer[characterCount]) ;
        }
        // Now multiply the value in each element by its appropriate power of ten
        for (charactersProcessed = 0 ; charactersProcessed < decimalPlaces ; charactersProcessed++)
        {
            // Assume the next character is invalid
            decimalValue = 0x0 ;
            // Ensure that a valid ascii value is used each time. If the value is a space, set it as a zero
            if (pBuffer[characterCount] >= 0x30 && pBuffer[characterCount] <= 0x39)
            {
                // Valid character
                decimalValue = pBuffer[characterCount] - 0x30 ;
            }
            // Use the power variable to get the location in the powers of ten table
            power = MAX_32BIT_CHARACTERS - (decimalPlaces - charactersProcessed) ;
            // Now add this to the total result
            *pValue32 += decimalValue * valueSteps32Bit[power] ;
            // Do the next character
            characterCount++ ;
        }
    }
    return(result) ;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Converts an HMI format date (a TUSIGN32 with the time in seconds from a certain date).
 \brief  All this code is lifted directly from the HMI as it cannot be accessed via a function call.
 \author GL
 \date   2007-02-7
 \param  hmiFormatDate - The date in HMI format
 \param  pBuffer - Pointer to the string where the formatted output will be placed.
 \return TUSIGN16 - number of bytes added, 0 signifies an error
 <pre>
    OK success
    <> OK failed
 </pre>
 \warning Expects the passed string buffer to be of size defined by DEVICE_STRING_SIZE.
 \test
 \n by: GL
 \n intention: 1. complete code covarage
 \n result module test: 2006-10-03
 \n result Lint Level 3: // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 StringFormatHmiDateDeviceStringFormatting(TUSIGN32 hmiFormatDate, TUSIGN8 *pBuffer, TUSIGN8 hmiDateTimeFormat)
{
    // Local time/date format object
    TIMEANDDATE timeAndDate ;
    // Number of characters added to the passed string buffer
    TUSIGN8 addedCharacters = 0 ;

    // Don't use an invalid string buffer
    VIP_ASSERT(pBuffer) ;
    // Convert the seconds information to date data
 //   AdjusterConvertSecondsToDate(hmiFormatDate,&timeAndDate) ;  			//del for temps
    // Blank the target buffer, assume it's a device hart string
    memset(pBuffer,0x0,DEVICE_STRING_SIZE) ;

    // Then format to a string
    switch(hmiDateTimeFormat)
    {
        case(HMI_DATE_FORMAT_DD_MM_YYYY):
            // Add the time information first
            addedCharacters = StringFormatHmiTimeDeviceStringFormatting(&timeAndDate,pBuffer) ;
            // Now add the day
            AddTwoCharacterCodeDeviceStringFormatting(timeAndDate.date,&pBuffer[addedCharacters]) ;
            // Separate the date characters with a backslash
            pBuffer[11] = '/' ;
            // Three characters added
            addedCharacters += 3 ;
            // Add the month
            AddTwoCharacterCodeDeviceStringFormatting(timeAndDate.month,&pBuffer[addedCharacters]) ;
            // Separate the date characters with a backslash
            pBuffer[14] = '/' ;
            // Three characters added
            addedCharacters += 3 ;
            // Finally add the year
            sprintf((char *)&pBuffer[addedCharacters],"%d",timeAndDate.year) ;
        break ;
        case(HMI_DATE_FORMAT_MM_DD_YYYY):
            // Add the time information first
            addedCharacters = StringFormatHmiTimeDeviceStringFormatting(&timeAndDate,pBuffer) ;
            // Add the month
            AddTwoCharacterCodeDeviceStringFormatting(timeAndDate.month,&pBuffer[addedCharacters]) ;
            // Separate the date characters with a backslash
            pBuffer[11] = '/' ;
            // Three characters added
            addedCharacters += 3 ;
            // Now add the day
            AddTwoCharacterCodeDeviceStringFormatting(timeAndDate.date,&pBuffer[addedCharacters]) ;
            // Separate the date characters with a backslash
            pBuffer[14] = '/' ;
            // Three characters added
            addedCharacters += 3 ;
            // Finally add the year
            sprintf((char *)&pBuffer[addedCharacters],"%d",timeAndDate.year) ;
        break ;
        case(HMI_DATE_FORMAT_YYYY_MM_DD):
            // Add the time information first
            addedCharacters = StringFormatHmiTimeDeviceStringFormatting(&timeAndDate,pBuffer) ;
            // Add the year
            sprintf((char *)&pBuffer[addedCharacters],"%d",timeAndDate.year) ;
            // Separate the date characters with a backslash
            pBuffer[13] = '/' ;
            // Five characters added
            addedCharacters += 5 ;
            // Add the month
            AddTwoCharacterCodeDeviceStringFormatting(timeAndDate.month,&pBuffer[addedCharacters]) ;
            // Separate the date characters with a backslash
            pBuffer[16] = '/' ;
            // Three characters added
            addedCharacters += 3 ;
            // Now add the day
            AddTwoCharacterCodeDeviceStringFormatting(timeAndDate.date,&pBuffer[addedCharacters]) ;
        break ;
    }
    // Now return with the number of characters added
    addedCharacters = strlen((char const *)pBuffer) ;
    return(addedCharacters) ;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief   Converts the passed integer value into a two character code and adds to the passed
 \brief   string buffer. If the number exceeds 99, no characters are added, and the function exits
 \brief   with zero.
 \author  GL
 \date    2006-02-21
 \param   CharacterCode - number to be added.
 \param   pBuffer - Pointer to the target buffer for formatting
 \return  TUSIGN16 - Number of characters added to the target string
 <pre>
          > 0 - Operation was successful
 </pre>
 \warning
 \test      Pass
 \n by:     GL
 \n intention: 1. complete code covarage
 \n result module test: 2006-02-21
 \n result Lint Level 3: // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 AddTwoCharacterCodeDeviceStringFormatting(TUSIGN8 characterCode, TUSIGN8 *pBuffer)
{
    TUSIGN8 charactersAdded = 0 ;

    if (characterCode <= 9)
    {
        charactersAdded = sprintf((char*)pBuffer,"0%d",characterCode) ;
    }
    else
    {
        if (characterCode <= 99)
        {
            charactersAdded = sprintf((char*)pBuffer,"%d",characterCode) ;
        }
    }
    return(charactersAdded) ;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief   Private function to format the time portion of an hmi format date/time object into a string.
 \author  GL
 \date    2006-02-21
 \param   pTimeAndDate - pointer to a TIMEANDDATE structure holding the date / time information.
 \param   pBuffer - Pointer to the target buffer for formatting
 \return  TUSIGN16 - Number of characters added to the target string
 <pre>
          > 0 - Operation was successful
 </pre>
 \warning
 \test      Pass
 \n by:     GL
 \n intention: 1. complete code covarage
 \n result module test: 2006-02-21
 \n result Lint Level 3: // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 StringFormatHmiTimeDeviceStringFormatting(TIMEANDDATE *pTimeAndDate, TUSIGN8 *pBuffer)
{
    TUSIGN8 addedCharacters = 0 ;

    // Ensure the time and date pointer and buffer pointer are OK
    VIP_ASSERT(pTimeAndDate) ;
    VIP_ASSERT(pBuffer) ;
    // If these are OK, we can go ahead and format the time
    // Add the hour first
    AddTwoCharacterCodeDeviceStringFormatting(pTimeAndDate->hour,pBuffer) ;
    // Dividing character for the hour and minute
    pBuffer[2] = ':' ;
    // Three characters added
    addedCharacters += 3 ;
    // The minute
    AddTwoCharacterCodeDeviceStringFormatting(pTimeAndDate->minute,&pBuffer[addedCharacters]) ;
    // Dividing character for the minute and second
    pBuffer[5] = ':' ;
    // Another three characters added
    addedCharacters += 3 ;
    // The second
    AddTwoCharacterCodeDeviceStringFormatting(pTimeAndDate->second,&pBuffer[addedCharacters]) ;
    // Add a space to separate the time from the date
    pBuffer[8] = ' ' ;
    // Another three characters added
    addedCharacters += 3 ;
    // Exit with the number of characters added
    return(addedCharacters) ;
}

#ifdef _EMF_INDUSTRIAL
//--------------------------------------------------------------------------------------------------
/*!
 \brief   Industrial only function for converting NE53 objects into string data. This object type is no
 \brief   longer used by the WW variant, but is still implemented in the IND bootloader, so still supported
 \brief   for that system
 \author  GL
 \date    2006-02-21
 \param   pTimeAndDate - pointer to a TIMEANDDATE structure holding the date / time information.
 \param   pBuffer - Pointer to the target buffer for formatting
 \return  TUSIGN16 - Number of characters added to the target string
 <pre>
          > 0 - Operation was successful
 </pre>
 \warning
 \test      Pass
 \n by:     GL
 \n intention: 1. complete code covarage
 \n result module test: 2006-02-21
 \n result Lint Level 3: // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 StringFormatNE53VERSIONDeviceStringFormatting(TNE53VERSION *me, TUSIGN8 *pBuffer)
{
    TUSIGN8 dateString[TDATE_STRING_SIZE] ;
    TBOOL dateIncluded = eFALSE ;
    TDate tempDate ;
    // Buffer for converted two character codes, the extra is for the null character
    TUSIGN8 twoCharacterBuffer[3] ;
    TUSIGN16 charactersAdded = 0 ;

    VIP_ASSERT(pBuffer) ;
    // Read the date information, if any, into a local date structure
    tempDate.DateYear = me->DateYear ;
    tempDate.DateMonth = me->DateMonth ;
    tempDate.DateDay = me->DateDay ;
    // Check whether date information should be added to the version string
    if (StringFormat_DATE(&tempDate, dateString) > 0)
    {
        // Valid date information available
        dateIncluded = eTRUE ;
    }
    // The version string can now be formatted
    // Each of the version codes have to be entered as a double number code, so 0 is 00.
    // Major version code
    charactersAdded = AddTwoCharacterCodeDeviceStringFormatting(me->MajorVersionNumber, pBuffer) ;
    if (charactersAdded > 0)
    {
        // Separating dot character
        strcat((char*)pBuffer,".") ;
        // Minor version code
        charactersAdded = AddTwoCharacterCodeDeviceStringFormatting(me->MinorVersionNumber, twoCharacterBuffer) ;
        if (charactersAdded > 0)
        {
            // Add the minor version code
            strcat((char*)pBuffer,(char*)twoCharacterBuffer) ;
            // Separating dot character
            strcat((char*)pBuffer,".") ;
            // Sub-minor version code
            charactersAdded = AddTwoCharacterCodeDeviceStringFormatting(me->SubMinorVersionNumber, twoCharacterBuffer) ;
            if (charactersAdded > 0)
            {
                // Add the sub minor version code
                strcat((char*)pBuffer,(char*)twoCharacterBuffer) ;
                // Check if either date or special version information was included. If not, the string ends here
                if (dateIncluded == eTRUE)
                {
                    // Add a space between the version and date
                    strcat((char*)pBuffer," ") ;
                    // Then add the date string
                    strcat((char*)pBuffer,(char*)dateString) ;
                }
            }
        }
    }
    if (charactersAdded > 0)
    {
        // Get the total number of characters added
        charactersAdded = strlen((char*)pBuffer) ;
    }
    return(charactersAdded) ;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief   Formats the passed string with the passed date object data.
 \author  GL
 \date    2006-02-21
 \param   pDate - Pointer to a simple date object
 \param   DateTimeFormat - The required format of the version code date time data
 \param   pBuffer - Pointer to the target buffer for formatting
 \return  TUSIGN16 - Number of characters added to the target string
 <pre>
          > 0 - Operation was successful
 </pre>
 \warning
 \test      Pass
 \n by:     GL
 \n intention: 1. complete code covarage
 \n result module test: 2006-02-21
 \n result Lint Level 3: // approved warnings and approved infos are suppresed
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 StringFormat_DATE(TDate *pDate, TUSIGN8 *pBuffer)
{
    // Temporary buffer for year information - An extra character is left for the null terminator
    TUSIGN8 yearBuffer[5] ;
    // Temporary buffer for two character converted strings - extra byte for null character
    TUSIGN8 twoCharBuffer[3] ;
    // The number of characters added to the passed string buffer
    TUSIGN16 characterLength = 0 ;
    // Set to true if a standard date/time format is performed OK,
    // set to false if an error occurs or a HART date/time is requested
    TBOOL result = eTRUE ;

    if (pDate && pBuffer)
    {
        // Check all the date values are less than the maximum values
        if (pDate->DateDay <= DATE_MAX_DAYS_PER_MONTH &&
            pDate->DateMonth <= DATE_MAX_MONTHS_PER_YEAR &&
              pDate->DateYear <= DATE_MAX_YEAR)
        {
            // And greater than the minimum values
            if (pDate->DateDay >= DATE_MIN_DAYS_PER_MONTH &&
                pDate->DateMonth >= DATE_MIN_MONTHS_PER_YEAR &&
                  pDate->DateYear >= DATE_MIN_YEAR)
            {
                // First convert the year information
                sprintf((char*)yearBuffer,"%d",pDate->DateYear) ;
                // Assume the date format is DDMMYYYY
                AddTwoCharacterCodeDeviceStringFormatting(pDate->DateDay, pBuffer) ;
                (void)sprintf((char*)twoCharBuffer,"%c",SEPARATOR_CHARACTER) ;
                (void)strcat((char*)pBuffer,(char*)twoCharBuffer) ;
                AddTwoCharacterCodeDeviceStringFormatting(pDate->DateMonth, twoCharBuffer) ;
                (void)strcat((char*)pBuffer,(char*)twoCharBuffer) ;
                (void)sprintf((char*)twoCharBuffer,"%c",SEPARATOR_CHARACTER) ;
                (void)strcat((char*)pBuffer,(char*)twoCharBuffer) ;
                (void)strcat((char*)pBuffer,(char*)yearBuffer) ;
                if (result == eTRUE)
                {
                    // Terminated string, so return the length with strlen
                    characterLength = strlen((char*)pBuffer) ;
                }
            }
        }
    }
    return(characterLength) ;
}
#endif

#ifdef MODULETEST_DEVICESTRINGFORMATTING

#include <string.h>

TUSIGN16 ModuleTestDeviceStringFormatting(void)
{
    // Make the buffer size the maximum allowable number of characters + 1 for the terminating null character
    TUSIGN8 testBuf[MAX_32BIT_CHARACTERS + 1] ;
    TUSIGN32 testValue32 = 0 ;
    TUSIGN16 result = 0 ;
    TUSIGN8 lengthResult ;

    // Test the string formatting function
    testValue32 = 0XFFFFFFFF ;
    lengthResult = StringFormat32BitDeviceStringFormatting(&testValue32, testBuf, eTRUE) ;
    // Buffer should now be set to 4294967295
    if (strcmp((char*)testBuf,"4294967295"))
    {
        // String formatting not working
        result = 1 ;
    }
    assert(result == 0) ;
    // Check the resultant length - should be 10
    if (lengthResult != 10)
    {
        // Length not being reported properly
        result = 2 ;
    }
    assert(result == 0) ;
    // All ones
    testValue32 = 1111111111 ;
    lengthResult = StringFormat32BitDeviceStringFormatting(&testValue32, testBuf, eTRUE) ;
    // Buffer should now be set to 1111111111
    if (strcmp((char*)testBuf,"1111111111"))
    {
        // String formatting not working
        result = 1 ;
    }
    assert(result == 0) ;
    // Check length
    if (lengthResult != 10)
    {
        // Length not being reported properly
        result = 2 ;
    }
    assert(result == 0) ;
    // Zero
    testValue32 = 0 ;
    lengthResult = StringFormat32BitDeviceStringFormatting(&testValue32, testBuf, eTRUE) ;
    // Buffer should now be set to 0
    if (strcmp((char*)testBuf,"0"))
    {
        // String formatting not working
        result = 1 ;
    }
    assert(result == 0) ;
    // And length should be 1
    if (lengthResult != 1)
    {
        // Bad length returned
        result = 2 ;
    }
    assert(result == 0) ;
    // Random values . . .
    // 1234567890
    testValue32 = 1234567890 ;
    lengthResult = StringFormat32BitDeviceStringFormatting(&testValue32, testBuf, eTRUE) ;
    // Check the string buffer content
    if (strcmp((char*)testBuf,"1234567890"))
    {
        // String formatting not working
        result = 1 ;
    }
    assert(result == 0) ;
    // Test the formatting for ascii to numeric
    strcpy((char *)testBuf,"1234567890") ;
    // Get the numeric representation
    NumberFormat32BitDeviceStringFormatting(testBuf, &testValue32, 0) ;
    // Check the result
    if (testValue32 != 1234567890)
    {
        // Numeric formatting failed
        result = 2 ;
    }
    assert(result == 0) ;
    strcpy((char *)testBuf,"   001 2") ;
    // Should ignore any leading spaces, then convert the space in the data to a zero
    NumberFormat32BitDeviceStringFormatting(testBuf, &testValue32, 0) ;
    if (testValue32 != 102)
    {
        // Failed to catch leading spaces or space in data
        result = 3 ;
    }
    assert(result == 0) ;
    // Invalid string content - should return 0
    strcpy((char *)testBuf,"ABCDEFG") ;
    NumberFormat32BitDeviceStringFormatting(testBuf, &testValue32, 0) ;
    if (testValue32 != 0)
    {
        // Failed to catch invalid data
        result = 4 ;
    }
    assert(result == 0) ;
    return(result);
}
#endif
