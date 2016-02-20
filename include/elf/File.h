#ifndef FILE_H
#define FILE_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include "Type.h"

typedef struct{
  I32 fd;
  struct stat meta;
}File;

static inline Bool File_Exists(const Char *szPath) {
  return access((const char*)szPath, F_OK) == 0;
}

static inline Bool File_Delete(const Char *szPath){
  return unlink((const char*)szPath) == 0;
}

static inline Bool File_Link(const Char *pszOldName, const Char *pszNewName){
  return link((const char*)pszOldName, (const char*)pszNewName) == 0;
}

static inline Bool File_Rename(const Char *pszOldName, const Char *pszNewName){
  if(!File_Link(pszOldName, pszNewName))
    return false;
  return File_Delete(pszOldName);
}

static inline Bool File_Create(File *pFile, const Char *szPath){
  pFile->fd = open((const char*)szPath, O_CREAT | O_TRUNC | O_RDWR, 0644);
  return pFile->fd != -1;
}

static inline Bool File_Open(File *pFile, const Char *szPath){
  pFile->fd = open((const char*)szPath, O_RDWR);
  return pFile->fd != -1;
}

static inline Bool File_OpenForRead(File *pFile, const Char *szPath){
  pFile->fd = open((const char*)szPath, O_RDONLY);
  return pFile->fd != -1;
}

static inline Bool File_CreateForWrite(File *pFile, const Char *szPath){
  pFile->fd = open((const char*)szPath, O_CREAT | O_TRUNC | O_WRONLY, 0644);
  return pFile->fd != -1;
}

static inline Bool File_OpenForWrite(File *pFile, const Char *szPath){
  pFile->fd = open((const char*)szPath, O_WRONLY);
  return pFile->fd != -1;
}

static inline Bool File_PrepareForWrite(File *pFile, const Char *szPath){
  pFile->fd = open((const char*)szPath, O_WRONLY);
  if(pFile->fd == -1){
    if(errno != ENOENT)
      return false;

    pFile->fd = open((const char*)szPath, O_CREAT | O_WRONLY, 0644);
    if(pFile->fd == -1)
      return false;
  }

  return pFile->fd != -1;
}

static inline Bool File_OpenForAppending(File *pFile, const Char *szPath){
  pFile->fd = open((const char*)szPath, O_APPEND);
  return pFile->fd != -1;
}

static inline Bool File_PrepareForAppending(File *pFile, const Char *szPath){
  pFile->fd = open((const char*)szPath, O_APPEND);
  if(pFile->fd == -1){
    if(errno != ENOENT)
      return false;

    pFile->fd = open((const char*)szPath, O_CREAT | O_APPEND, 0644);
    if(pFile->fd == -1)
      return false;
  }

  return pFile->fd != -1;
}

static inline Bool File_Prepare(File *pFile, const Char *szPath){
  pFile->fd = open((const char*)szPath, O_RDWR);
  if(pFile->fd != -1)
    return true;

  if(errno != ENOENT)
    return false;

  pFile->fd = open((const char*)szPath, O_CREAT | O_RDWR, 0644);
  return pFile->fd != -1;
}

static inline void File_OpenStdIn(File *pFile){
  pFile->fd = 0;
}

static inline void File_OpenStdOut(File *pFile){
  pFile->fd = 1;
}

static inline void File_OpenStdError(File *pFile){
  pFile->fd = 2;
}

static inline void File_Close(File *pFile){
  close(pFile->fd);
  pFile->fd = -1;
}

static inline I64 File_Seek(File *pFile, U64 uOffset){
  return lseek(pFile->fd, uOffset, SEEK_SET);
}

static inline I64 File_Offset(File *pFile){
  return lseek(pFile->fd, 0, SEEK_CUR);
}

static inline I64 File_SeekToEnd(File *pFile){
  return lseek(pFile->fd, 0, SEEK_END);
}

static inline Bool File_ReadMeta(File *pFile){
  return fstat(pFile->fd, &pFile->meta) == 0;
}

static inline U64 File_GetSize(File *pFile){
  return pFile->meta.st_size;
}

static inline Bool File_SetSize(File *pFile, U64 nSize){
  return ftruncate(pFile->fd, nSize) == 0;
}

static inline Bool File_Flush(File *pFile){
  return fsync(pFile->fd) == 0;
}

static inline Bool File_FlushData(File *pFile){
  return fdatasync(pFile->fd) == 0;
}

static inline I32 File_Read(const File *pFile, Byte *pBuffer, U32 nSize){
  return read(pFile->fd, pBuffer, nSize);
}

static inline I32 File_ReadFrom(const File *pFile, U64 uOffset, Byte *pBuffer, U32 nSize){
  return pread(pFile->fd, pBuffer, nSize, uOffset);
}

static inline I32 File_Write(const File *pFile, const Byte *pData, U32 nSize){
  return write(pFile->fd, pData, nSize);
}

static inline I32 File_WriteTo(const File *pFile, U64 uOffset, const Byte *pData, U32 nSize){
  return pwrite(pFile->fd, pData, nSize, uOffset);
}

typedef struct{
  Byte *pData;
  size_t uBytes;
}FileData;

static inline I32 File_ReadDatas(const File *pFile, const FileData *szBuffers, U32 uBuffers){
  return readv(pFile->fd, szBuffers, uBuffers);
}

static inline I32 File_ReadDatasFrom(const File *pFile, U64 uOffset, const FileData *szBuffers, U32 uBuffers){
  return preadv(pFile->fd, szBuffers, uBuffers, uOffset);
}

static inline I32 File_WriteDatas(const File *pFile, const FileData *szDatas, U32 uDatas){
  return writev(pFile->fd, szDatas, uDatas);
}

static inline I32 File_WriteDatasTo(const File *pFile, U64 uOffset, const FileData *szDatas, U32 uDatas){
  return pwritev(pFile->fd, szDatas, uDatas, uOffset);
}
#endif // FILE_H

