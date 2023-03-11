#ifndef __CHECKSUM_H__
#define __CHECKSUM_H__

TBOOL GetFileChecksum(const TCHAR* filePath, TUSIGN8* checksum, TUSIGN8 checksumSize);

#endif