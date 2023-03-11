/* 7zFile.c -- File IO
2017-04-03 : Igor Pavlov : Public domain */

#include "Services/SoftwareUpdate/Decompression/Interface/7zFile.h"
#include "FS_interface.h"

FS_fileSystem_t* pFileSystem = NULL;

void File_Init() {
  (void)FS_GetInstance(FS_instanceMain, &pFileSystem);
}

void File_Construct(CSzFile *p) {
  p->file = NULL;
}

static WRes File_Open(CSzFile *p, const char *name, int writeMode) {
  int32_t retVal = FS_FileOpen(pFileSystem, name, writeMode ? FS_modeWrite : FS_modeRead, &p->file);
  return (retVal != FS_OK) ? 1 : 0;
}

WRes InFile_Open(CSzFile *p, const char *name) { 
  return File_Open(p, name, 0); 
}

WRes OutFile_Open(CSzFile *p, const char *name) { 
  return File_Open(p, name, 1); 
}

WRes File_Close(CSzFile *p) {
  if (p->file != NULL) {
    int res = FS_FileClose(pFileSystem, &p->file);
    if (res != 0) {
      return res;
    }
    p->file = NULL;
  }
  
  return 0;
}

WRes File_Read(const CSzFile *p, void *data, size_t *size) {
  size_t originalSize = *size;
  if (originalSize == 0) {
    return 0;
  }

  *size = (size_t)FS_FileRead(pFileSystem, p->file, data, originalSize);

  return (*size == originalSize) ? 0 : 1;
}

WRes File_Write(const CSzFile *p, const void *data, size_t *size) {
  size_t originalSize = *size;
  if (originalSize == 0) {
    return 0;
  }
 
  *size = (size_t)FS_FileWrite(pFileSystem, p->file, data, originalSize);
  if (*size == originalSize) {
    return 0;
  }
  return 1;
  
}

WRes File_Seek(const CSzFile *p, Int64 *pos, ESzSeek origin) {  
  FS_whence_t moveMethod;
  WRes res;

  switch (origin) {
    case SZ_SEEK_SET: moveMethod = FS_seekSet; break;
    case SZ_SEEK_CUR: moveMethod = FS_seekCur; break;
    case SZ_SEEK_END: moveMethod = FS_seekEnd; break;
    default: 
      return 1;
  }

  res = (WRes)FS_FileSeek(pFileSystem, p->file, (int32_t)*pos, moveMethod);
  *pos = (Int64)FS_FileTell(pFileSystem, p->file);
  return res;
}

WRes File_GetLength(const CSzFile *p, UInt64 *length) { 
  long pos = FS_FileTell(pFileSystem, p->file);
  int res = FS_FileSeek(pFileSystem, p->file, 0, FS_seekEnd);
  *length = (UInt64)FS_FileTell(pFileSystem, p->file);
  (void)FS_FileSeek(pFileSystem, p->file, pos, FS_seekSet);
  return res;
}

/* ---------- FileSeqInStream ---------- */
static SRes FileSeqInStream_Read(const ISeqInStream *pp, void *buf, size_t *size) {
  CFileSeqInStream *p = CONTAINER_FROM_VTBL(pp, CFileSeqInStream, vt);
  return File_Read(&p->file, buf, size) == 0 ? SZ_OK : SZ_ERROR_READ;
}

void FileSeqInStream_CreateVTable(CFileSeqInStream *p) {
  p->vt.Read = FileSeqInStream_Read;
}


/* ---------- FileInStream ---------- */
static SRes FileInStream_Read(const ISeekInStream *pp, void *buf, size_t *size) {
  CFileInStream *p = CONTAINER_FROM_VTBL(pp, CFileInStream, vt);
  return (File_Read(&p->file, buf, size) == 0) ? SZ_OK : SZ_ERROR_READ;
}

static SRes FileInStream_Seek(const ISeekInStream *pp, Int64 *pos, ESzSeek origin) {
  CFileInStream *p = CONTAINER_FROM_VTBL(pp, CFileInStream, vt);
  return File_Seek(&p->file, pos, origin);
}

void FileInStream_CreateVTable(CFileInStream *p) {
  p->vt.Read = FileInStream_Read;
  p->vt.Seek = FileInStream_Seek;
}

/* ---------- FileOutStream ---------- */
static size_t FileOutStream_Write(const ISeqOutStream *pp, const void *data, size_t size) {
  CFileOutStream *p = CONTAINER_FROM_VTBL(pp, CFileOutStream, vt);
  File_Write(&p->file, data, &size);
  return size;
}

void FileOutStream_CreateVTable(CFileOutStream *p) {
  p->vt.Write = FileOutStream_Write;
}
