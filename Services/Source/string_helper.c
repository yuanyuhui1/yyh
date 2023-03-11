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
#include "System/Interface/common_type.h"

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "Services/Interface/string_helper.h"
#include "Services/Interface/math_helper.h"

//-------------------------------------------------------------------------
/*! 
 \brief Shifts buffer in string view
 \author  
 \date  
 \param pStringView String view in which to perform shift
 \param by Value by which to shift string view
 \warning  
 \test  
 \n test-date: 
 \n by: 
 \n environment:  
 \n intention:  
 \n result module test:  
 \n result Lint check:  
 \bug   
*/ 
//--------------------------------------------------------------------------
void string_view_shift(T_STRING_VIEW* pStringView, TINT16 by) {
    if (by < 0) {
        pStringView->length += (TUSIGN32)abs(by);
    }
    else {
        pStringView->length -= (TUSIGN32)abs(by);
    }

    pStringView->buffer += by;
}

//-------------------------------------------------------------------------
/*! 
 \brief Copy string view to char buffer
 \author Michal Kochel
 \date   21.05.2020
 \param pStringView String view as a copy source
 \param outputBuffer Output buffer
 \warning  
 \test  
 \n test-date: 
 \n by: 
 \n environment:  
 \n intention:  
 \n result module test:  
 \n result Lint check:  
 \bug   
*/ 
//-------------------------------------------------------------------------- 
void string_view_to_buffer(const T_STRING_VIEW* pStringView, TCHAR* outputBuffer) {
    (void)strcpy_safe(outputBuffer, pStringView->buffer, pStringView->length + 1);
}

//-------------------------------------------------------------------------
/*! 
 \brief Copy string view to char buffer
 \author Michal Kochel
 \date   21.05.2020
 \param pStringView String view as a copy source
 \param outputBuffer Output buffer
 \param outputBufferSize Max size of output buffer
 \warning  
 \test  
 \n test-date: 
 \n by: 
 \n environment:  
 \n intention:  
 \n result module test:  
 \n result Lint check:  
 \bug   
*/ 
//-------------------------------------------------------------------------- 
void string_view_to_n_buffer(const T_STRING_VIEW* pStringView, TCHAR* outputBuffer, size_t outputBufferSize) {
    (void)strcpy_safe(outputBuffer, pStringView->buffer, min_tusign32(pStringView->length + 1, (TUSIGN32)outputBufferSize));
}


//-------------------------------------------------------------------------
/*! 
 \brief Extract int from string view
 \author Michal Kochel
 \date   21.05.2020
 \param pStringView String view as a source
 \param base Numeric base for int convertions
 \return TINT32 extracted int
 \warning  
 \test  
 \n test-date: 
 \n by: 
 \n environment:  
 \n intention:  
 \n result module test:  
 \n result Lint check:  
 \bug   
*/ 
//-------------------------------------------------------------------------- 
TINT32 string_view_to_int(const T_STRING_VIEW* pStringView, TINT32 base) {
    if (pStringView->length > 0) {
        return strtol((const char*)pStringView->buffer, NULL, base);
    }
    else {
        return 0;
    }
}

//-------------------------------------------------------------------------
/*! 
 \brief Extract token from string view and copy it to buffer
 \author Michal Kochel
 \date   21.05.2020
 \param pStringView String view as a source
 \param sep Character which seperates tokens
 \param outputBuffer Output buffer
 \return TINT32 extracted int
 \warning  
 \test  
 \n test-date: 
 \n by: 
 \n environment:  
 \n intention:  
 \n result module test:  
 \n result Lint check:  
 \bug   
*/ 
//-------------------------------------------------------------------------- 
TBOOL string_view_tok_string(T_STRING_VIEW* pStringView, TCHAR sep, TCHAR* outputBuffer) {
    T_STRING_VIEW extractedToken;

    TBOOL endReached = strntokloc_view(pStringView, &extractedToken, sep);

    string_view_to_buffer(&extractedToken, outputBuffer);

    return endReached;
}

//-------------------------------------------------------------------------
/*! 
 \brief Extract token from string view and convert it to int
 \author Michal Kochel
 \date   21.05.2020
 \param pStringView String view as a source
 \param sep Character which seperates tokens
 \param outputValue Output value
 \param base Numeric base for int convertions
 \return TBOOL Is source string view end reached 
 \warning  
 \test  
 \n test-date: 
 \n by: 
 \n environment:  
 \n intention:  
 \n result module test:  
 \n result Lint check:  
 \bug   
*/ 
//-------------------------------------------------------------------------- 
TBOOL string_view_tok_int(T_STRING_VIEW* pStringView, TCHAR sep, TINT32* outputValue, TINT32 base) {
    T_STRING_VIEW extractedToken;

    TBOOL endReached = strntokloc_view(pStringView, &extractedToken, sep);

    *outputValue = string_view_to_int(&extractedToken, base);

    return endReached;
}

//-------------------------------------------------------------------------
/*! 
 \brief Extract token from string view as hex string and convert it data buffer 
 \author Michal Kochel
 \date   21.05.2020
 \param pStringView String view as a source
 \param sep Character which seperates tokens
 \param outputBuffer Output value
 \param outputSize Size of output buffer
 \return TBOOL Is source string view end reached 
 \warning  
 \test  
 \n test-date: 
 \n by: 
 \n environment:  
 \n intention:  
 \n result module test:  
 \n result Lint check:  
 \bug   
*/ 
//-------------------------------------------------------------------------- 
TBOOL string_view_tok_strhex(T_STRING_VIEW* pStringView, TCHAR sep, TUSIGN8* outputBuffer, size_t outputSize) {
    T_STRING_VIEW extractedToken;

    TBOOL endReached = strntokloc_view(pStringView, &extractedToken, sep);

    size_t extractBytes = min_size_t(outputSize * 2, extractedToken.length);

    (void)HexStringToBuffer(extractedToken.buffer, extractBytes, outputBuffer);

    return endReached;
}
//-------------------------------------------------------------------------
/*! 
 \brief Performs safe string copy with NULL terminator
 \author  
 \date  
 \param pDestBuffer Destination string buffer  
 \param pSrcBuffer Source string buffer
 \param maxlen Max number of character that will be copied
 \return int chars copied 
 \warning  
 \test  
 \n test-date: 
 \n by: 
 \n environment:  
 \n intention:  
 \n result module test:  
 \n result Lint check:  
 \bug   
*/ 
//-------------------------------------------------------------------------- 
size_t strcpy_safe(TCHAR* pDestBuffer, const TCHAR* pSrcBuffer, size_t maxlen) {
    size_t strLen = strlen((const char*)pSrcBuffer) + 1;

    if (strLen > maxlen) {
        strLen = maxlen;
    }
    strncpy((char*)pDestBuffer, (const char*)pSrcBuffer, strLen);
    pDestBuffer[strLen - 1] = '\0';

    return strLen;
}

//-------------------------------------------------------------------------
/*! 
 \brief Finds character in string 
 \author  
 \date  
 \param str String to be searched
 \param ch Charachter which is searched for
 \param maxlen Max number of character in string that will be checked
 \return char* Pointer to place where charachter was found or NULL if not found
 \warning  
 \test  
 \n test-date: 
 \n by: 
 \n environment:  
 \n intention:  
 \n result module test:  
 \n result Lint check:  
 \bug   
*/ 
//-------------------------------------------------------------------------- 
const TCHAR* strnchr(const TCHAR* str, TCHAR ch, size_t maxlen) {
    size_t strSize = strnlen((const char*)str, maxlen);

    for (size_t offset = 0; offset < strSize; ++offset, ++str) {
        if (*str == ch) {
            return str;
        }
    }
    return NULL;
}

//-------------------------------------------------------------------------
/*! 
 \brief Finds string in string 
 \author  
 \date  
 \param str1 String to be searched
 \param str2 String which is searched for
 \param maxlen Max number of character in string that will be checked
 \return char* Pointer to place where string was found or NULL if not found
 \warning  
 \test  
 \n test-date: 
 \n by: 
 \n environment:  
 \n intention:  
 \n result module test:  
 \n result Lint check:  
 \bug   
*/ 
//-------------------------------------------------------------------------- 
const TCHAR* strnstr(const TCHAR* str1, const TCHAR* str2, size_t maxlen) {
    size_t str1size = strnlen((const char*)str1, maxlen);
    size_t str2size = strnlen((const char*)str2, maxlen);
    if (str1size < str2size) {
        return NULL;
    }

    for (size_t offset = 0; offset <= str1size - str2size; ++offset, ++str1) {
        if (strncmp((const char*)str1, (const char*)str2, str2size) == 0) {
            return str1;
        }
    }
    return NULL;
}

int strncasecmp(char const* str1, char const* str2, size_t maxLen)
{
    int diff = (int)(strnlen(str1, maxLen) - strnlen(str2, maxLen));

    for (; diff == 0 && maxLen > 0; str1++, str2++, maxLen--) {
        diff = tolower((int)*str1) - tolower((int)*str2);
    }

    return diff;
}

const TCHAR* strncasestr(const TCHAR* str1, const TCHAR* str2, size_t maxlen) {
    size_t str1size = strnlen((const char*)str1, maxlen);
    size_t str2size = strnlen((const char*)str2, maxlen);
    if (str1size < str2size) {
        return NULL;
    }

    for (size_t offset = 0; offset <= str1size - str2size; ++offset, ++str1) {
        if (strncasecmp((const char*)str1, (const char*)str2, str2size) == 0) {
            return str1;
        }
    }
    return NULL;
}

//-------------------------------------------------------------------------
/*! 
 \brief Converts a char in hex to decimal representaion as a number
 \author  
 \date  
 \param hexChar Hex char 
 \return char Hex char numeric value 
 \warning  
 \test  
 \n test-date: 
 \n by: 
 \n environment:  
 \n intention:  
 \n result module test:  
 \n result Lint check:  
 \bug   
*/ 
//-------------------------------------------------------------------------- 
TUSIGN8 HexCharToValue(TCHAR hexChar) {
    if (hexChar >= '0' && hexChar <= '9') {
        return hexChar - '0';
    }
    else if (hexChar >= 'a' && hexChar <= 'f') {
        return 10 + (hexChar - 'a');
    }
    else if (hexChar >= 'A' && hexChar <= 'F') {
        return 10 + (hexChar - 'A');
    }
    return 0;
}

//-------------------------------------------------------------------------
/*! 
 \brief Converts a hex in decimal representaion as a number to ASCII char
 \author  
 \date  
 \param vlaue Hex char numeric value 
 \return char Hex char 
 \warning  
 \test  
 \n test-date: 
 \n by: 
 \n environment:  
 \n intention:  
 \n result module test:  
 \n result Lint check:  
 \bug   
*/ 
//-------------------------------------------------------------------------- 
TCHAR ValueToHexChar(TUSIGN8 value) {
    if (value <= 9) {
        return '0' + value;
    }
    else if (value >= 0xA && value <= 0xF) {
        return 'A' + (value - 0xA);
    }
    else {
        return '0';
    }
}

//-------------------------------------------------------------------------
/*! 
 \brief Finds token with given seperation character and size limit
 \author  
 \date  
 \param string String from which to extract token
 \param max Max size of token
 \param sep Character which seperates tokens
 \return char Hex char numeric value 
 \warning  
 \test  
 \n test-date: 
 \n by: 
 \n environment:  
 \n intention:  
 \n result module test:  
 \n result Lint check:  
 \bug   
*/ 
//--------------------------------------------------------------------------
size_t strntokloc(const TCHAR* string, size_t max, TCHAR sep) {
    const TCHAR* tokLocation = strnchr(string, sep, max);

    if (tokLocation == NULL) {
        return max;
    }
    else {
        return (size_t)(tokLocation - string);
    }
}

//-------------------------------------------------------------------------
/*! 
 \brief Finds token with given seperation character within string view
 \author  
 \date  
 \param stringSource String view from which to extract token
 \param stringResult Resulting string view
 \param sep Character which seperates tokens
 \return TBOOL Is source string view end reached 
 \warning  
 \test  
 \n test-date: 
 \n by: 
 \n environment:  
 \n intention:  
 \n result module test:  
 \n result Lint check:  
 \bug   
*/ 
//--------------------------------------------------------------------------
TBOOL strntokloc_view(T_STRING_VIEW* stringSource, T_STRING_VIEW* stringResult, TCHAR sep) {
    TBOOL endReached;

    size_t extractedResult = strntokloc(stringSource->buffer, stringSource->length, sep);

    endReached = (stringSource->length == extractedResult) ? eTRUE : eFALSE;

    stringResult->buffer = stringSource->buffer;
    stringResult->length = extractedResult;

    // Skip separator
    if (endReached == eFALSE) {
        extractedResult++;
    }

    string_view_shift(stringSource, (TINT16)extractedResult);

    return endReached;
}

//-------------------------------------------------------------------------
/*! 
 \brief Locate last occurrence of character in string starting from max len
 \author  
 \date  
 \param str String in which to search
 \param character Character to search
 \param maxlen String size to search in
 \return const TCHAR* A pointer to the last occurrence of character in str
 \warning  
 \test  
 \n test-date: 
 \n by: 
 \n environment:  
 \n intention:  
 \n result module test:  
 \n result Lint check:  
 \bug   
*/ 
//--------------------------------------------------------------------------
const TCHAR* strrnchr(const TCHAR* str, int character, size_t maxlen) {
    const TCHAR* currentChar = str + maxlen - 1;
    for (size_t index = 0; index < maxlen; ++index, --currentChar) {
        if (*currentChar == character) {
            return currentChar;
        }
    }

    return NULL;
}

//-------------------------------------------------------------------------
/*! 
 \brief Converts data buffer into hax string representation
 \author  
 \date  
 \param buffer Buffer which will be converted
 \param bufferSize Size of buffer to convert
 \param output Hex string buffer in which to write output
 \return TCHAR* End of string in which hex buffer was written
 \warning  
 \test  
 \n test-date: 
 \n by: 
 \n environment:  
 \n intention:  
 \n result module test:  
 \n result Lint check:  
 \bug   
*/ 
//--------------------------------------------------------------------------
TCHAR* BufferToHexString(const TUSIGN8* buffer, size_t bufferSize, TCHAR* output) {
    for (size_t i = 0; i < bufferSize; ++i) {
        *output =  ValueToHexChar((buffer[i] >> 4) & 0x0F);
        output++;
        *output =  ValueToHexChar(buffer[i] & 0x0F);
        output++;
    }
    return output;
}

//-------------------------------------------------------------------------
/*! 
 \brief Converts hax string representation into data buffer
 \author  
 \date  
 \param hexString Hex string
 \param hexStringSize Hex string size
 \param output Buffer in which to write output
 \return size_t Number of converted bytes
 \warning  
 \test  
 \n test-date: 
 \n by: 
 \n environment:  
 \n intention:  
 \n result module test:  
 \n result Lint check:  
 \bug   
*/ 
//--------------------------------------------------------------------------
size_t HexStringToBuffer(const TCHAR* hexString, size_t hexStringSize, TUSIGN8* output) {
    TUSIGN8* outputBeg = output;

    if (hexStringSize % 2 == 1) {
        *output = (HexCharToValue(*hexString));
        hexString++;
        output++;
    }

    for (size_t i = 0; i < hexStringSize / 2; ++i) {
        *output = (HexCharToValue(*hexString) << 4) & 0xFF;
        hexString++;

        *output |= (HexCharToValue(*hexString));
        hexString++;

        output++;
    }

    return (size_t)(output - outputBeg);
}

//-------------------------------------------------------------------------
/*! 
 \brief Converts hax string representation into data buffer
 \author  
 \date  
 \param hexString Hex string
 \param hexStringSize Hex string size
 \param output Buffer in which to write output
 \return size_t Number of converted bytes
 \warning  
 \test  
 \n test-date: 
 \n by: 
 \n environment:  
 \n intention:  
 \n result module test:  
 \n result Lint check:  
 \bug   
*/ 
//--------------------------------------------------------------------------
size_t HexStringToNBuffer(const TCHAR* hexString, size_t hexStringSize, TUSIGN8* output, size_t outputSize) {
    TUSIGN8* outputBeg = output;

    if (outputSize > 0) {
        if (hexStringSize % 2 == 1) {
            *output = (HexCharToValue(*hexString));
            hexString++;
            output++;
            outputSize--;
        }
    }

    for (size_t i = 0; (i < hexStringSize / 2) && (outputSize > 0); ++i) {
        *output = (HexCharToValue(*hexString) << 4) & 0xFF;
        hexString++;

        *output |= (HexCharToValue(*hexString));
        hexString++;

        output++;
        outputSize--;
    }

    return (size_t)(output - outputBeg);
}

//-------------------------------------------------------------------------
/*! 
 \brief Checks if string is printable
 \author  
 \date  
 \param pString String to check
 \return TBOOL If string is printable
 \warning  
 \test  
 \n test-date: 
 \n by: 
 \n environment:  
 \n intention:  
 \n result module test:  
 \n result Lint check:  
 \bug   
*/ 
//--------------------------------------------------------------------------
TBOOL IsStringPrintable(const TCHAR* pString) {
    while (*pString != '\0') {
        if (isprint(*pString) == 0) {
            return eFALSE;
        }
        pString++;
    }
    return eTRUE;
}

//-------------------------------------------------------------------------
/*! 
 \brief Find and replace char in string
 \author  
 \date  
 \param pString String in which to find char
 \param len String length
 \param find Char to find
 \param replace Char to replace with
 \return TBOOL If string is printable
 \warning  
 \test  
 \n test-date: 
 \n by: 
 \n environment:  
 \n intention:  
 \n result module test:  
 \n result Lint check:  
 \bug   
*/ 
//--------------------------------------------------------------------------
void ReplaceChar(TCHAR* pString, size_t len, TCHAR find, TCHAR replace) {
    for ( ; len > 0 && *pString != '\0'; --len, pString++) {
        if (*pString == find) {
            *pString = replace;
        }
    }
}

// Based on: https://www.iar.com/support/tech-notes/compiler/linker-error-undefined-external-for-inline-functions/
//lint -e{762} Needed to properly handle inlining in IAR
extern inline TCHAR* strappendch(TCHAR* str, TCHAR ch);
//lint -e{762} Needed to properly handle inlining in IAR
extern inline TCHAR* strappendstr(TCHAR* strDest, const TCHAR* strSrc);
//lint -e{762} Needed to properly handle inlining in IAR
extern inline TCHAR* strappendchnull(TCHAR* str, TCHAR ch);
