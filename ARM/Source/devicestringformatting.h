//--------------------------------------------------------------------------------------------------
/*!
 Copyright              Copyright ABB, 2006.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         EMF
 Module         devicestringformatting.h
 Description    Public methods for Device string formatting functions.

 Remarks

*/
//--------------------------------------------------------------------------------------------------
#ifndef __DEVICESTRINGFORMATTING_H__
#define __DEVICESTRINGFORMATTING_H__

// Types used by the IND variant for formatting the bootloader software version number
#ifdef _EMF_INDUSTRIAL
#define TDATE_STRING_SIZE 11


typedef TUSIGN8 TDateDay ;
typedef TUSIGN8 TDateMonth ;
typedef TUSIGN16 TDateYear ;

typedef enum _TNE53VERSIONSUBTYPE
{
    TNE53VERSIONSUBTYPE_None = 0 ,
    TNE53VERSIONSUBTYPE_Pre = 1 ,
    TNE53VERSIONSUBTYPE_Special = 2,
    TNE53VERSIONSUBTYPE_Count
}TNE53VERSIONSUBTYPE ;

// Define the underlying data type for the ne53 version number
typedef struct _TNE53VERSION
{
    TUSIGN8 MajorVersionNumber ;
    TUSIGN8 MinorVersionNumber ;
    TUSIGN8 SubMinorVersionNumber ;
    TUSIGN8 VersionSubType ;
    TUSIGN8 VersionSubTypeNumber ;
    TDateYear DateYear ;
    TDateMonth DateMonth ;
    TDateDay DateDay ;
}TNE53VERSION ;

// Also the indices of the object attributes
typedef enum _TNE53VERSION_IDX
{
    TNE53VERSION_IDX_MajorVersionNumber = 0 ,
    TNE53VERSION_IDX_MinorVersionNumber = 1 ,
    TNE53VERSION_IDX_SubMinorVersionNumber = 2 ,
    TNE53VERSION_IDX_VersionSubType = 3 ,
    TNE53VERSION_IDX_VersionSubTypeNumber = 4 ,
    TNE53VERSION_IDX_DateYear = 5 ,
    TNE53VERSION_IDX_DateMonth = 6 ,
    TNE53VERSION_IDX_DateDay = 7 ,
    TNE53VERSION_IDX_Count
}TNE53VERSION_IDX ;

// Simple date time data type
typedef struct _TDate
{
    TDateDay DateDay ;
    TDateMonth DateMonth ;
    TDateYear DateYear ;
}TDate ;

// Limits for date information
#define DATE_MAX_DAYS_PER_MONTH         31
#define DATE_MIN_DAYS_PER_MONTH         1
#define DATE_MAX_MONTHS_PER_YEAR        12
#define DATE_MIN_MONTHS_PER_YEAR        1
#define DATE_MAX_YEAR                   9999
#define DATE_MIN_YEAR                   1
#define DATE_FEBRUARY_LEAP_YEAR_DAYS    29
#define DATE_FEBRUARY_NORMAL_YEAR_DAYS  28
#define DATE_HART_YEAR_CONVERSION       1900
#define DATE_HART_SIZE                  3

// Characters used during string formatting
#define SEPARATOR_CHARACTER '/'
#endif

//--------------------------------------------------------------------------------------------------
/*!
 \brief  Converts a thirty two bit number to ascii data. Calls get character count to initialise the character count
 \brief  variable, then uses the tables of data ValueSteps32Bit and Max32BitColumnValue
 \brief  to calculate the ascii values for each column. This was written to replace the IAR sprintf call
 \brief  which doesn't work with whole numbers above sixteen bits.
 \author GL
 \date   2006-02-27
 \param  pValue32 - Pointer to the 32 bit number being string formatted.
 \param  pBuffer - The string buffer to be loaded with the formatted 32bit number information
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
TUSIGN8 StringFormat32BitDeviceStringFormatting(TUSIGN32 *pValue32, TUSIGN8 *pBuffer, TBOOL nullTerminate) ;

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
TUSIGN16 NumberFormat32BitDeviceStringFormatting(TUSIGN8 *pBuffer, TUSIGN32 *pValue32, TUSIGN8 length) ;

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
//TUSIGN16 StringFormatHmiDateDeviceStringFormatting(TUSIGN32 hmiFormatDate, TUSIGN8 *pBuffer, TUSIGN8 hmiDateTimeFormat) ;

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
TUSIGN16 StringFormatNE53VERSIONDeviceStringFormatting(TNE53VERSION *me, TUSIGN8 *pBuffer) ;
#endif

//#define MODULETEST_DEVICESTRINGFORMATTING
#ifdef MODULETEST_DEVICESTRINGFORMATTING
    TUSIGN16 ModuleTestDeviceStringFormatting(void) ;
#endif

#endif

#define DEVICE_STRING_SIZE 20

