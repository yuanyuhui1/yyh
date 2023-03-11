//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2020.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Decompression Service
 Module         Software Update
 Description    Implementation of decompression methods

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>

#include "Services/SoftwareUpdate/Decompression/Interface/7z.h"
#include "Services/SoftwareUpdate/Decompression/Interface/7zAlloc.h"
#include "Services/SoftwareUpdate/Decompression/Interface/7zFile.h"
#include "Services/SoftwareUpdate/Decompression/Interface/7zCrc.h"
#include "Services/SoftwareUpdate/Decompression/Interface/7zBuf.h"
#include "System/Interface/common_type.h"
#include "Services/SoftwareUpdate/Decompression/Interface/DecService.h"
#include "Services/SoftwareUpdate/Decompression/Interface/HelperFunctions.h"
#include "FS_interface.h"

#define kInputBufSize ((size_t)1 << 18)

static const ISzAlloc g_Alloc = { SzAlloc, SzFree };

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Update file name buffer
 \author    Michał Kochel
 \date      2020-08-06
 \param     fileNameInArchive Buffer for file name
 \param		currentFileNameSize Current size of file name buffer
 \param		fileNameInArchiveSize Requested file size in buffer
 \return    UInt16* New buffer pointer or NULL on error
 \warning   none
 \test
 \n test-date:           
 \n by:                  Michał Kochel
 \n environment:         End-to-end
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
static UInt16* UpdateFileNameBuffer(UInt16* fileNameInArchive, size_t currentFileNameSize, size_t* fileNameInArchiveSize) {
	if (fileNameInArchive == NULL || currentFileNameSize > *fileNameInArchiveSize) {
		SzFree(NULL, fileNameInArchive);
		*fileNameInArchiveSize = currentFileNameSize;
		fileNameInArchive = (UInt16*)SzAlloc(NULL, (*fileNameInArchiveSize + 1) * sizeof(fileNameInArchive[0]));
	}
	return fileNameInArchive;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Decompresses manifest
 \author    Piotr Kmiecinski
 \date      2020-02-24
 \param     archName File path to archive name
 \return    E_SUP_RET code
 \warning   none
 \test
 \n test-date:           
 \n by:                  Piotr Kmiecinski
 \n environment:         End-to-end
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
E_SUP_RET decompressManifest(const char* archName)
{
	E_SUP_RET eRetVal = E_SUP_DECOMPRESSION_ERROR;

	ISzAlloc allocImp;
	ISzAlloc allocTempImp;

	allocImp = g_Alloc;
	allocTempImp = g_Alloc;

	CFileInStream archiveStream;
	CLookToRead2 lookStream;

	CSzArEx db;

	File_Init();

	if (InFile_Open(&archiveStream.file, archName)) {
		return E_SUP_CANNOT_OPEN_FILE;
	}

	FileInStream_CreateVTable(&archiveStream);
	LookToRead2_CreateVTable(&lookStream, False);

	lookStream.buf = (Byte*)ISzAlloc_Alloc(&allocImp, kInputBufSize);
	if (lookStream.buf != NULL) {
		lookStream.bufSize = kInputBufSize;
		lookStream.realStream = &archiveStream.vt;
		LookToRead2_Init(&lookStream);

		CrcGenerateTable();

		SzArEx_Init(&db);

		if (SzArEx_Open(&db, &lookStream.vt, &allocImp, &allocTempImp) == SZ_OK) {
			CBuf filename; 
			Buf_Init(&filename);
			UInt32 blockIndex = 0xFFFFFFFF; /* it can have any value before first call (if outBuffer = 0) */
			Byte* outBuffer = 0; /* it must be 0 before first call for each new archive. */
			size_t outBufferSize = 0;  /* it can have any value before first call (if outBuffer = 0) */
			UInt16* fileNameInArchive = NULL;
			size_t fileNameInArchiveSize = 0;

			for (UInt32 fileIndex = 0; fileIndex < db.NumFiles; fileIndex++) {
				size_t offset = 0;
				size_t outSizeProcessed = 0;
				size_t currentFileNameSize;

				// Realocate memory for bigger file name size
				currentFileNameSize = SzArEx_GetFileNameUtf16(&db, fileIndex, NULL);

				fileNameInArchive = UpdateFileNameBuffer(fileNameInArchive, currentFileNameSize, &fileNameInArchiveSize);
				if (fileNameInArchive == NULL) {
					break;
				}

				// Get file name and conver it to char
				(void)SzArEx_GetFileNameUtf16(&db, fileIndex, fileNameInArchive);                                         
				(void)Utf16_To_Char(&filename, fileNameInArchive);

				// Look for manifest file
				if (strcmp("sandbox/manifest.csv", (const char*)filename.data) == 0) {
					if (SzArEx_Extract(&db, &lookStream.vt, fileIndex,
							&blockIndex, &outBuffer, &outBufferSize,
							&offset, &outSizeProcessed,
							&allocImp, &allocTempImp) == SZ_OK) {

						CSzFile outFile;
														
						if (OutFile_Open(&outFile, TEMP_MANIFEST_LOCATION) == 0) {
							size_t processedSize = outSizeProcessed;

							if (File_Write(&outFile, outBuffer + offset, &processedSize) != 0 || processedSize != outSizeProcessed) {
								eRetVal = E_SUP_FS_ERROR;
							}
							else {
								eRetVal = E_SUP_OK;
							}

							(void)File_Close(&outFile);
						}
					}

					if (outBuffer != NULL) {
						ISzAlloc_Free(&allocImp, outBuffer);
					}
					break;
				}
			}
					
			Buf_Free(&filename, &g_Alloc);
			SzFree(NULL, fileNameInArchive);
			SzArEx_Free(&db, &allocImp);
			ISzAlloc_Free(&allocImp, lookStream.buf);
			(void)File_Close(&archiveStream.file);
		}
	}

	return eRetVal; 
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Decompresses whole package
 \author    Piotr Kmiecinski
 \date      2020-02-24
 \param     archName File path to archive name
 \return    E_SUP_RET code
 \warning   none
 \test
 \n test-date:           
 \n by:                  Piotr Kmiecinski
 \n environment:         End-to-end
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
E_SUP_RET decompressSUP(const char* archName)
{
	ISzAlloc allocImp;
	ISzAlloc allocTempImp;

	allocImp = g_Alloc;
	allocTempImp = g_Alloc;

	CFileInStream archiveStream;
	CLookToRead2 lookStream;

	E_SUP_RET eRetVal = E_SUP_DECOMPRESSION_ERROR;
	CSzArEx db;
        
	File_Init();

	if (InFile_Open(&archiveStream.file, archName)) {
		return E_SUP_CANNOT_OPEN_FILE;
	}

	FileInStream_CreateVTable(&archiveStream);
	LookToRead2_CreateVTable(&lookStream, False);

	lookStream.buf = (Byte*)ISzAlloc_Alloc(&allocImp, kInputBufSize);
	if (lookStream.buf != NULL) {
		lookStream.bufSize = kInputBufSize;
		lookStream.realStream = &archiveStream.vt;
		LookToRead2_Init(&lookStream);

		CrcGenerateTable();
		SzArEx_Init(&db);

		if (SzArEx_Open(&db, &lookStream.vt, &allocImp, &allocTempImp) == SZ_OK) {
			eRetVal = E_SUP_OK;

			UInt32 blockIndex = 0xFFFFFFFF; /* it can have any value before first call (if outBuffer = 0) */
			Byte* outBuffer = 0; /* it must be 0 before first call for each new archive. */
			size_t outBufferSize = 0;  /* it can have any value before first call (if outBuffer = 0) */
			UInt16* fileNameInArchive = NULL;
			size_t fileNameInArchiveSize = 0;

			for (UInt32 fileIndex = 0; fileIndex < db.NumFiles; fileIndex++) {
				size_t offset = 0;
				size_t outSizeProcessed = 0;
				size_t currentFileNameSize;
				unsigned isDir = SzArEx_IsDir(&db, fileIndex);

				// Realocate memory for bigger file name size
				currentFileNameSize = SzArEx_GetFileNameUtf16(&db, fileIndex, NULL);

				fileNameInArchive = UpdateFileNameBuffer(fileNameInArchive, currentFileNameSize, &fileNameInArchiveSize);
				if (!fileNameInArchive) {
					eRetVal = E_SUP_DECOMPRESSION_ERROR;
					break;
				}

				(void)SzArEx_GetFileNameUtf16(&db, fileIndex, fileNameInArchive);

				// Create all dirs in hierarchy 
				for (size_t charIndex = 0; fileNameInArchive[charIndex] != 0; charIndex++) {
					if (fileNameInArchive[charIndex] == '/') {
						fileNameInArchive[charIndex] = '\0';
						if (MyCreateDir(fileNameInArchive) != FS_OK) {
							eRetVal = E_SUP_FS_ERROR;
						}
						fileNameInArchive[charIndex] = CHAR_PATH_SEPARATOR;
					}
				}

				if (isDir) {
					if (MyCreateDir(fileNameInArchive) != FS_OK) {
						eRetVal = E_SUP_FS_ERROR;
					}
				}
				else {
					if (SzArEx_Extract(&db, &lookStream.vt, fileIndex,
							&blockIndex, &outBuffer, &outBufferSize,
							&offset, &outSizeProcessed,
							&allocImp, &allocTempImp) == SZ_OK) {

						CSzFile outFile;

						if (OutFile_OpenUtf16(&outFile, fileNameInArchive) == 0) {
							size_t processedSize = outSizeProcessed;

							if (File_Write(&outFile, outBuffer + offset, &processedSize) != 0 || processedSize != outSizeProcessed) {
								eRetVal = E_SUP_FS_ERROR;
							}

							(void)File_Close(&outFile);
						}
						else {
							eRetVal = E_SUP_DECOMPRESSION_ERROR;
						}
					}
				}

				if (eRetVal != E_SUP_OK) {
					break;
				}
			}

			// Perform final cleanup. SzArEx_Extract clears outBuffer on begingin of each run
			if (outBuffer != NULL) {
				ISzAlloc_Free(&allocImp, outBuffer);
			}

			SzFree(NULL, fileNameInArchive);
			SzArEx_Free(&db, &allocImp);
			ISzAlloc_Free(&allocImp, lookStream.buf);
			(void)File_Close(&archiveStream.file);
		}
	}

	return eRetVal;
}
