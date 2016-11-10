#ifndef FILE_H
#define FILE_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

#ifdef __linux__
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/uio.h>
#else
#endif

#include "Type.h"

typedef struct{
#ifdef __linux__
  I32 fd;
  struct stat meta;
#else
  HANDLE handle;
#endif
}File;

static inline B File_Exists(const C *szPath){
#ifdef __linux__
  return access((const char*)szPath, F_OK) == 0;
#else
#endif
}

static inline B File_Delete(const C *szPath){
#ifdef __linux__
  return unlink((const char*)szPath) == 0;
#else
  return DeleteFileA((LPCSTR)szPath);
#endif
}

static inline B File_Link(const C *pszOldName, const C *pszNewName){
#ifdef __linux__
  return link((const char*)pszOldName, (const char*)pszNewName) == 0;
#else
#endif
}

static inline B File_Rename(const C *pszOldName, const C *pszNewName){
#ifdef __linux__
  if(!File_Link(pszOldName, pszNewName))
    return false;
  return File_Delete(pszOldName);
#else
#endif
}

static inline B File_Create(File *pFile, const C *szPath){
#ifdef __linux__
  pFile->fd = open((const char*)szPath, O_CREAT | O_TRUNC | O_RDWR, 0644);
  return pFile->fd != -1;
#else
  pFile->handle = CreateFileA((LPCSTR)szPath, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  return pFile->handle != INVALID_HANDLE_VALUE;
#endif
}

static inline B File_Open(File *pFile, const C *szPath){
#ifdef __linux__
  pFile->fd = open((const char*)szPath, O_RDWR);
  return pFile->fd != -1;
#else
  pFile->handle = CreateFileA((LPCSTR)szPath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  return pFile->handle != INVALID_HANDLE_VALUE;
#endif
}

static inline B File_OpenForRead(File *pFile, const C *szPath){
#ifdef __linux__
  pFile->fd = open((const char*)szPath, O_RDONLY);
  return pFile->fd != -1;
#else
  pFile->handle = CreateFileA((LPCSTR)szPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  return pFile->handle != INVALID_HANDLE_VALUE;
#endif
}

static inline B File_CreateForWrite(File *pFile, const C *szPath){
#ifdef __linux__
  pFile->fd = open((const char*)szPath, O_CREAT | O_TRUNC | O_WRONLY, 0644);
  return pFile->fd != -1;
#else
  pFile->handle = CreateFileA((LPCSTR)szPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  return pFile->handle != INVALID_HANDLE_VALUE;
#endif
}

static inline B File_OpenForWrite(File *pFile, const C *szPath){
#ifdef __linux__
  pFile->fd = open((const char*)szPath, O_WRONLY);
  return pFile->fd != -1;
#else
  pFile->handle = CreateFileA((LPCSTR)szPath, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  return pFile->handle != INVALID_HANDLE_VALUE;
#endif
}

static inline B File_PrepareForWrite(File *pFile, const C *szPath){
#ifdef __linux__
  pFile->fd = open((const char*)szPath, O_WRONLY);
  if(pFile->fd == -1){
    if(errno != ENOENT)
      return false;

    pFile->fd = open((const char*)szPath, O_CREAT | O_WRONLY, 0644);
  }

  return pFile->fd != -1;
#else
  pFile->handle = CreateFileA((LPCSTR)szPath, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  return pFile->handle != INVALID_HANDLE_VALUE;
#endif
}

static inline B File_OpenForAppending(File *pFile, const C *szPath){
#ifdef __linux__
  pFile->fd = open((const char*)szPath, O_APPEND);
  return pFile->fd != -1;
#else
#endif
}

static inline B File_PrepareForAppending(File *pFile, const C *szPath){
#ifdef __linux__
  pFile->fd = open((const char*)szPath, O_APPEND);
  if(pFile->fd == -1){
    if(errno != ENOENT)
      return false;

    pFile->fd = open((const char*)szPath, O_CREAT | O_APPEND, 0644);
  }

  return pFile->fd != -1;
#else
#endif
}

static inline B File_Prepare(File *pFile, const C *szPath){
#ifdef __linux__
  pFile->fd = open((const char*)szPath, O_RDWR);
  if(pFile->fd != -1)
    return true;

  if(errno != ENOENT)
    return false;

  pFile->fd = open((const char*)szPath, O_CREAT | O_RDWR, 0644);
  return pFile->fd != -1;
#else
  pFile->handle = CreateFileA((LPCSTR)szPath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  return pFile->handle != INVALID_HANDLE_VALUE;
#endif
}

static inline void File_OpenStdIn(File *pFile){
#ifdef __linux__
  pFile->fd = 0;
#else
#endif
}

static inline void File_OpenStdOut(File *pFile){
#ifdef __linux__
  pFile->fd = 1;
#else
#endif
}

static inline void File_OpenStdError(File *pFile){
#ifdef __linux__
  pFile->fd = 2;
#else
#endif
}

static inline void File_Close(File *pFile){
#ifdef __linux__
  close(pFile->fd);
  pFile->fd = -1;
#else
  CloseHandle(pFile->handle);
#endif
}

static inline I64 File_Seek(File *pFile, U64 uOffset){
#ifdef __linux__
  return lseek(pFile->fd, uOffset, SEEK_SET);
#else
#endif
}

static inline I64 File_Offset(File *pFile){
#ifdef __linux__
  return lseek(pFile->fd, 0, SEEK_CUR);
#else
#endif
}

static inline I64 File_SeekToEnd(File *pFile){
#ifdef __linux__
  return lseek(pFile->fd, 0, SEEK_END);
#else
#endif
}

static inline B File_ReadMeta(File *pFile){
#ifdef __linux__
  return fstat(pFile->fd, &pFile->meta) == 0;
#else
#endif
}

static inline U64 File_GetSize(File *pFile){
#ifdef __linux__
  return pFile->meta.st_size;
#else
#endif
}

static inline B File_SetSize(File *pFile, U64 uSize){
#ifdef __linux__
  return ftruncate(pFile->fd, uSize) == 0;
#else
#endif
}

static inline B File_Flush(File *pFile){
#ifdef __linux__
  return fsync(pFile->fd) == 0;
#else
#endif
}

static inline B File_FlushData(File *pFile){
#ifdef __linux__
  return fdatasync(pFile->fd) == 0;
#else
#endif
}

static inline I32 File_Read(const File *pFile, Byte *pBuffer, U32 uSize){
#ifdef __linux__
  return read(pFile->fd, pBuffer, uSize);
#else
  U32 uBytes;
  if(ReadFile(pFile->handle, pBuffer, uSize, (LPDWORD)&uBytes, NULL))
    return uBytes;
  return -1;
#endif
}

static inline I32 File_ReadFrom(const File *pFile, U64 uOffset, Byte *pBuffer, U32 uSize){
#ifdef __linux__
  return pread(pFile->fd, pBuffer, uSize, uOffset);
#else
#endif
}

static inline I32 File_Write(const File *pFile, const Byte *pData, U32 uSize){
#ifdef __linux__
  return write(pFile->fd, pData, uSize);
#else
  U32 uBytes;
  if(WriteFile(pFile->handle, pData, uSize, (LPDWORD)&uBytes, NULL))
    return uBytes;
  return -1;
#endif
}

static inline I32 File_WriteTo(const File *pFile, U64 uOffset, const Byte *pData, U32 uSize){
#ifdef __linux__
  return pwrite(pFile->fd, pData, uSize, uOffset);
#else
#endif
}

typedef struct{
  P pData;
  U uBytes;
}FileData;

static inline I32 File_ReadDatas(const File *pFile, const FileData *szBuffers, U32 uBuffers){
#ifdef __linux__
  return readv(pFile->fd, (struct iovec*)szBuffers, uBuffers);
#else
#endif
}

static inline I32 File_ReadDatasFrom(const File *pFile, U64 uOffset, const FileData *szBuffers, U32 uBuffers){
#ifdef __linux__
  return preadv(pFile->fd, (struct iovec*)szBuffers, uBuffers, uOffset);
#else
#endif
}

static inline I32 File_WriteDatas(const File *pFile, const FileData *szDatas, U32 uDatas){
#ifdef __linux__
  return writev(pFile->fd, (struct iovec*)szDatas, uDatas);
#else
#endif
}

static inline I32 File_WriteDatasTo(const File *pFile, U64 uOffset, const FileData *szDatas, U32 uDatas){
#ifdef __linux__
  return pwritev(pFile->fd, (struct iovec*)szDatas, uDatas, uOffset);
#else
#endif
}

#endif // FILE_H

