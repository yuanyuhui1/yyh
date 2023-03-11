
#include "Services/SoftwareUpdate/Decompression/Interface/7z.h"
#include "Services/SoftwareUpdate/Decompression/Interface/7zAlloc.h"
#include "Services/SoftwareUpdate/Decompression/Interface/7zFile.h"
#include "Services/SoftwareUpdate/Decompression/Interface/7zCrc.h"
#include "Services/SoftwareUpdate/Decompression/Interface/7zBuf.h"

WRes MyCreateDir(const UInt16* name);
WRes OutFile_OpenUtf16(CSzFile* p, const UInt16* name);
size_t Utf16_To_Utf8_Calc(const UInt16 *src, const UInt16 *srcLim);
SRes Utf16_To_Utf8Buf(CBuf *dest, const UInt16 *src, size_t srcLen);
SRes Utf16_To_Char(CBuf* buf, const UInt16* s);
Byte* Utf16_To_Utf8(Byte *dest, const UInt16 *src, const UInt16 *srcLim);
WRes OutFile_OpenUtf16(CSzFile* p, const UInt16* name);
