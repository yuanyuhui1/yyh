/* 7zFile.h -- File IO
2017-04-03 : Igor Pavlov : Public domain */

#ifndef __7Z_FILE_H
#define __7Z_FILE_H


#include "7zTypes.h"
#include "FS_interface.h"

EXTERN_C_BEGIN

/* ---------- File ---------- */

typedef struct
{
  FS_file_t *file;
} CSzFile;

void File_Init(void);
void File_Construct(CSzFile *p);
WRes InFile_Open(CSzFile *p, const char *name);
WRes OutFile_Open(CSzFile *p, const char *name);
WRes File_Close(CSzFile *p);

/* reads max(*size, remain file's size) bytes */
WRes File_Read(const CSzFile *p, void *data, size_t *size);

/* writes *size bytes */
WRes File_Write(const CSzFile *p, const void *data, size_t *size);

WRes File_Seek(const CSzFile *p, Int64 *pos, ESzSeek origin);
WRes File_GetLength(const CSzFile *p, UInt64 *length);


/* ---------- FileInStream ---------- */

typedef struct
{
  ISeqInStream vt;
  CSzFile file;
} CFileSeqInStream;

void FileSeqInStream_CreateVTable(CFileSeqInStream *p);


typedef struct
{
  ISeekInStream vt;
  CSzFile file;
} CFileInStream;

void FileInStream_CreateVTable(CFileInStream *p);


typedef struct
{
  ISeqOutStream vt;
  CSzFile file;
} CFileOutStream;

void FileOutStream_CreateVTable(CFileOutStream *p);

EXTERN_C_END

#endif
