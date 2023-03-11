//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2020.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Helper Functions
 Module         Software Update
 Description    Implementation of decompression methods

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------

#include "Services/SoftwareUpdate/Decompression/Interface/HelperFunctions.h"
#include "Services/SoftwareUpdate/Decompression/Interface/7zTypes.h"
#include "FS_interface.h"

static const ISzAlloc g_Alloc = { SzAlloc, SzFree };

/*lint -esym(526,  pFileSystem) pFileSystem is defined in "7zFile.c" */
extern FS_fileSystem_t* pFileSystem;

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Creates directory based on Utf16 name
 \author    Michal Kochel
 \date      2020-05-27
 \param		dest Buffer
 \param		size Expected size
 \test
 \n test-date:           
 \n by:                  Michal Kochel
 \n environment:         Unit tests
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
WRes MyCreateDir(const UInt16* name) {
	CBuf buf;
	WRes res;
	Buf_Init(&buf);
	RINOK(Utf16_To_Char(&buf, name));

	res = FS_DirCreate(pFileSystem, (const char*)buf.data);

	if (res == FS_EXIST) {
		res = FS_OK;
	}

	Buf_Free(&buf, &g_Alloc);

	return res;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Check if destination buffer is big enough
 \author    Michal Kochel
 \date      2020-05-27
 \param		dest Buffer
 \param		size Expected size
 \test
 \n test-date:           
 \n by:                  Michal Kochel
 \n environment:         Unit tests
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
int Buf_EnsureSize(CBuf* dest, size_t size) {
	if (dest->size >= size) {
		return 1;
	}

	Buf_Free(dest, &g_Alloc);
	return Buf_Create(dest, size, &g_Alloc);
}

#define _UTF8_START(n) (0x100 - (1 << (7 - (n))))
#define _UTF8_RANGE(n) (((UInt32)1) << ((n) * 5 + 6))
#define _UTF8_HEAD(n, val) ((Byte)(_UTF8_START(n) + (val >> (6 * (n)))))
#define _UTF8_CHAR(n, val) ((Byte)(0x80 + (((val) >> (6 * (n))) & 0x3F)))

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Get output buffer size when converting Utf16 to Utf8
 \author    Michal Kochel
 \date      2020-05-27
 \param		src Source buffer
 \param		srcLim End of source buffer
 \test
 \n test-date:           
 \n by:                  Michal Kochel
 \n environment:         Unit tests
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
size_t Utf16_To_Utf8_Calc(const UInt16 *src, const UInt16 *srcLim)
{
	size_t size = 0;
	for (;;)
	{
		UInt32 val;
		if (src == srcLim)
			return size;

		size++;
		val = *src++;

		if (val < 0x80) {
			continue;
		}

		if (val < _UTF8_RANGE(1)) {
			size++;
			continue;
		}

		if (val >= 0xD800 && val < 0xDC00 && src != srcLim) {
			UInt32 c2 = *src;
			if (c2 >= 0xDC00 && c2 < 0xE000) {
				src++;
				size += 3;
				continue;
			}
		}

		size += 2;
	}
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Convert Utf16 to Utf8 and ensure buffer is big enough
 \author    Michal Kochel
 \date      2020-05-27
 \param     dest Destination buffer
 \param		src Source buffer
 \param		srcLen End of source buffer
 \test
 \n test-date:           
 \n by:                  Michal Kochel
 \n environment:         Unit tests
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
SRes Utf16_To_Utf8Buf(CBuf *dest, const UInt16 *src, size_t srcLen)
{
	size_t destLen = Utf16_To_Utf8_Calc(src, src + srcLen) + 1;

	if (!Buf_EnsureSize(dest, destLen)) {
		return SZ_ERROR_MEM;
	}

	Byte* bufferEnd = Utf16_To_Utf8(dest->data, src, src + srcLen);

	*bufferEnd = '\0';

	return SZ_OK;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Convert Utf16 to Char
 \author    Michal Kochel
 \date      2020-05-27
 \param     buf Destination buffer
 \param		sourceBuffer Source buffer
 \test
 \n test-date:           
 \n by:                  Michal Kochel
 \n environment:         Unit tests
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
SRes Utf16_To_Char(CBuf* buf, const UInt16* sourceBuffer) {
	unsigned len;

	for (len = 0; sourceBuffer[len] != 0; len++) { }

	return Utf16_To_Utf8Buf(buf, sourceBuffer, len);
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Convert Utf16 to Utf8
 \author    Michal Kochel
 \date      2020-05-27
 \param     dest Destination buffer
 \param		src Source buffer
 \param		srcLim End of source buffer
 \test
 \n test-date:           
 \n by:                  Michal Kochel
 \n environment:         Unit tests
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
Byte* Utf16_To_Utf8(Byte *dest, const UInt16 *src, const UInt16 *srcLim)
{
	for (;;)
	{
		UInt32 val;
		if (src == srcLim)
			return dest;

		val = *src++;

		if (val < 0x80) {
			*dest++ = (char)val;
			continue;
		}

		if (val < _UTF8_RANGE(1)) {
			dest[0] = _UTF8_HEAD(1, val);
			//lint -e{845,835} correct calculation
			dest[1] = _UTF8_CHAR(0, val);
			dest += 2;
			continue;
		}

		if (val >= 0xD800 && val < 0xDC00 && src != srcLim) {
			UInt32 c2 = *src;
			if (c2 >= 0xDC00 && c2 < 0xE000) {
				src++;
				val = (((val - 0xD800) << 10) | (c2 - 0xDC00)) + 0x10000;
				dest[0] = _UTF8_HEAD(3, val);
				dest[1] = _UTF8_CHAR(2, val);
				dest[2] = _UTF8_CHAR(1, val);
				//lint -e{845,835} correct calculation
				dest[3] = _UTF8_CHAR(0, val);
				dest += 4;
				continue;
			}
		}

		dest[0] = _UTF8_HEAD(2, val);
		dest[1] = _UTF8_CHAR(1, val);
		//lint -e{845,835} correct calculation
		dest[2] = _UTF8_CHAR(0, val);
		dest += 3;
	}
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Open file using Utf16 file path
 \author    Michal Kochel
 \date      2020-05-27
 \param     p Memory where key is stored
 \param		name File path in Utf16
 \test
 \n test-date:           
 \n by:                  Michal Kochel
 \n environment:         Unit tests
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
WRes OutFile_OpenUtf16(CSzFile* pFile, const UInt16* name) {
	CBuf buf;
	WRes res;

	Buf_Init(&buf);
	(void)Utf16_To_Char(&buf, name);
	res = OutFile_Open(pFile, (const char*)buf.data);
	Buf_Free(&buf, &g_Alloc);

	return res;
}
