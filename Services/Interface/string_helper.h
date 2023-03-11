//--------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2019
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Common Framework
 Module         COMMON
 Description    Functions for string handling
 Remarks

*/
//--------------------------------------------------------------------------------------------------

#ifndef __STRING_HELPER_H__
#define __STRING_HELPER_H__

#include <stddef.h>
#include <stdint.h>
#include <string.h>

typedef struct _T_STRING_VIEW {
    const TCHAR* buffer;
    TUSIGN32 length;
} T_STRING_VIEW;

void string_view_shift(T_STRING_VIEW* pStringView, TINT16 by);

void string_view_to_buffer(const T_STRING_VIEW* pStringView, TCHAR* outputBuffer);

void string_view_to_n_buffer(const T_STRING_VIEW* pStringView, TCHAR* outputBuffer, size_t outputBufferSize);

TINT32 string_view_to_int(const T_STRING_VIEW* pStringView, TINT32 base);

TBOOL string_view_tok_string(T_STRING_VIEW* pStringView, TCHAR sep, TCHAR* outputBuffer);

TBOOL string_view_tok_int(T_STRING_VIEW* pStringView, TCHAR sep, TINT32* outputValue, TINT32 base);

TBOOL string_view_tok_strhex(T_STRING_VIEW* pStringView, TCHAR sep, TUSIGN8* outputBuffer, size_t outputSize);

size_t strcpy_safe(TCHAR* pDestBuffer, const TCHAR* pSrcBuffer, size_t maxlen);

const TCHAR* strnchr(const TCHAR* str, TCHAR ch, size_t maxlen);

const TCHAR* strnstr(const TCHAR* str1, const TCHAR* str2, size_t maxlen);
const TCHAR* strncasestr(const TCHAR* str1, const TCHAR* str2, size_t maxlen);

TUSIGN8 HexCharToValue(TCHAR hexChar);

TCHAR ValueToHexChar(TUSIGN8 value);

size_t strntokloc(const TCHAR* string, size_t max, TCHAR sep);

TBOOL strntokloc_view(T_STRING_VIEW* stringSource, T_STRING_VIEW* stringResult, TCHAR sep);

const TCHAR* strrnchr(const TCHAR * str, int character, size_t maxlen);

TCHAR* BufferToHexString(const TUSIGN8* buffer, size_t bufferSize, TCHAR* output);

size_t HexStringToBuffer(const TCHAR* hexString, size_t hexStringSize, TUSIGN8* output);
size_t HexStringToNBuffer(const TCHAR* hexString, size_t hexStringSize, TUSIGN8* output, size_t outputSize);

TBOOL IsStringPrintable(const TCHAR* pString);

void ReplaceChar(TCHAR* pString, size_t len, TCHAR find, TCHAR replace);

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Appends char to given string
 \author    Michal Kochel
 \date      2019-11-19
 \param     str String to which char will be appended
 \param     ch Character which will be appended
 \return    TCHAR* Pointer to string after appended char
 \test
 \n test-date:           
 \n by:                  Michal Kochel
 \n environment:         Unit tests
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
inline TCHAR* strappendch(TCHAR* str, TCHAR ch) {
    *str = ch;
    return ++str;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Appends char to given string with null char at the end
 \author    Michal Kochel
 \date      2019-11-19
 \param     str String to which char will be appended
 \param     ch Character which will be appended
 \return    TCHAR* Pointer to string after appended char
 \test
 \n test-date:           
 \n by:                  Michal Kochel
 \n environment:         Unit tests
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
inline TCHAR* strappendchnull(TCHAR* str, TCHAR ch) {
    *str = ch;
    ++str;
    *str = '\0';
    return str;
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief     Appends string to given string
 \author    Michal Kochel
 \date      2019-11-19
 \param     strDest String to which char will be appended
 \param     strSrc String which will be appended
 \return    TCHAR* Pointer to string after appended string
 \test
 \n test-date:           
 \n by:                  Michal Kochel
 \n environment:         Unit tests
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
inline TCHAR* strappendstr(TCHAR* strDest, const TCHAR* strSrc) {
    while (*strDest++ = *strSrc++);
    return --strDest;
}

#endif