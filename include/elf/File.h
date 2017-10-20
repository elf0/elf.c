#ifndef FILE_H
#define FILE_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

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

inline
static E8 File_Exists(const C *szPath){
#ifdef __linux__
  return access((const char*)szPath, F_OK) != 0;
#else
#endif
}

inline
static E8 File_Delete(const C *szPath){
#ifdef __linux__
  return unlink((const char*)szPath) != 0;
#else
  return !DeleteFileA((LPCSTR)szPath);
#endif
}

inline
static E8 File_Link(const C *pszOldName, const C *pszNewName){
#ifdef __linux__
  return link((const char*)pszOldName, (const char*)pszNewName) != 0;
#else
#endif
}

inline
static E8 File_Rename(const C *pszOldName, const C *pszNewName){
#ifdef __linux__
  E8 e = File_Link(pszOldName, pszNewName);
  if(e)
    return e;
  return File_Delete(pszOldName);
#else
#endif
}

inline
static E8 File_Create(File *pFile, const C *szPath){
#ifdef __linux__
  int fd = open((const char*)szPath, O_CREAT | O_TRUNC | O_RDWR, 0644);
  if(fd < 0)
    return errno;

  pFile->fd = fd;
  return 0;
#else
  pFile->handle = CreateFileA((LPCSTR)szPath, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  return pFile->handle == INVALID_HANDLE_VALUE;
#endif
}

inline
static E8 File_CreateForWrite(File *pFile, const C *szPath){
#ifdef __linux__
  int fd = open((const char*)szPath, O_CREAT | O_TRUNC | O_WRONLY, 0644);
  if(fd < 0)
    return errno;

  pFile->fd = fd;
  return 0;
#else
  pFile->handle = CreateFileA((LPCSTR)szPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  return pFile->handle == INVALID_HANDLE_VALUE;
#endif
}

inline
static E8 File_Prepare(File *pFile, const C *szPath){
#ifdef __linux__
  int fd = open((const char*)szPath, O_RDWR);
  if(fd < 0){
    E8 e = errno;
    if(e != ENOENT)
      return e;

    fd = open((const char*)szPath, O_CREAT | O_RDWR, 0644);
    if(fd < 0)
      return errno;
  }

  pFile->fd = fd;
  return 0;
#else
  pFile->handle = CreateFileA((LPCSTR)szPath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  return pFile->handle == INVALID_HANDLE_VALUE;
#endif
}

inline
static E8 File_PrepareForAppending(File *pFile, const C *szPath){
#ifdef __linux__
  int fd = open((const char*)szPath, O_APPEND);
  if(fd < 0){
    E8 e = errno;
    if(e != ENOENT)
      return e;

    fd = open((const char*)szPath, O_CREAT | O_APPEND, 0644);
    if(fd < 0)
      return errno;
  }

  pFile->fd = fd;
  return 0;
#else
#endif
}

inline
static E8 File_PrepareForWrite(File *pFile, const C *szPath){
#ifdef __linux__
  int fd = open((const char*)szPath, O_WRONLY);
  if(fd < 0){
    E8 e = errno;
    if(e != ENOENT)
      return e;

    fd = open((const char*)szPath, O_CREAT | O_WRONLY, 0644);
  }

  pFile->fd = fd;
  return 0;
#else
  pFile->handle = CreateFileA((LPCSTR)szPath, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  return pFile->handle == INVALID_HANDLE_VALUE;
#endif
}

inline
static E8 File_Open(File *pFile, const C *szPath){
#ifdef __linux__
  int fd = open((const char*)szPath, O_RDWR);
  if(fd < 0)
    return errno;

  pFile->fd = fd;
  return 0;
#else
  pFile->handle = CreateFileA((LPCSTR)szPath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  return pFile->handle == INVALID_HANDLE_VALUE;
#endif
}

inline
static E8 File_OpenForAppending(File *pFile, const C *szPath){
#ifdef __linux__
  int fd = open((const char*)szPath, O_APPEND);
  if(fd < 0)
    return errno;

  pFile->fd = fd;
  return 0;
#else
#endif
}

inline
static E8 File_OpenForRead(File *pFile, const C *szPath){
#ifdef __linux__
  int fd = open((const char*)szPath, O_RDONLY);
  if(fd < 0)
    return errno;

  pFile->fd = fd;
  return 0;
#else
  pFile->handle = CreateFileA((LPCSTR)szPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  return pFile->handle == INVALID_HANDLE_VALUE;
#endif
}

inline
static E8 File_OpenForWrite(File *pFile, const C *szPath){
#ifdef __linux__
  int fd = open((const char*)szPath, O_WRONLY);
  if(fd < 0)
    return errno;

  pFile->fd = fd;
  return 0;
#else
  pFile->handle = CreateFileA((LPCSTR)szPath, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  return pFile->handle == INVALID_HANDLE_VALUE;
#endif
}

inline
static void File_OpenStdIn(File *pFile){
#ifdef __linux__
  pFile->fd = 0;
#else
#endif
}

inline
static void File_OpenStdOut(File *pFile){
#ifdef __linux__
  pFile->fd = 1;
#else
#endif
}

inline
static void File_OpenStdError(File *pFile){
#ifdef __linux__
  pFile->fd = 2;
#else
#endif
}

inline
static void File_Close(File *pFile){
#ifdef __linux__
  close(pFile->fd);
  pFile->fd = -1;
#else
  CloseHandle(pFile->handle);
  pFile->handle = INVALID_HANDLE_VALUE;
#endif
}

inline
static I64 File_Seek(File *pFile, U64 uOffset){
#ifdef __linux__
  return lseek(pFile->fd, uOffset, SEEK_SET);
#else
#endif
}

inline
static I64 File_Offset(File *pFile){
#ifdef __linux__
  return lseek(pFile->fd, 0, SEEK_CUR);
#else
#endif
}

inline
static I64 File_SeekToEnd(File *pFile){
#ifdef __linux__
  return lseek(pFile->fd, 0, SEEK_END);
#else
#endif
}

inline
static E8 File_ReadMeta(File *pFile){
#ifdef __linux__
  if(fstat(pFile->fd, &pFile->meta) != 0)
    return errno;

  return 0;
#else
#endif
}

inline
static U64 File_GetSize(File *pFile){
#ifdef __linux__
  return pFile->meta.st_size;
#else
#endif
}

inline
static E8 File_SetSize(File *pFile, U64 uSize){
#ifdef __linux__
  if(ftruncate(pFile->fd, uSize) != 0)
    return errno;

  pFile->meta.st_size = uSize;
  return 0;
#else
#endif
}

inline
static E8 File_Flush(File *pFile){
#ifdef __linux__
  if(fsync(pFile->fd) != 0)
    return errno;

  return 0;
#else
#endif
}

inline
static E8 File_FlushData(File *pFile){
#ifdef __linux__
  if(fdatasync(pFile->fd) != 0)
    return errno;

  return 0;
#else
#endif
}

inline
static R64 File_Read(const File *pFile, Byte *pBuffer, U64 uSize){
#ifdef __linux__
  R64 rRead = read(pFile->fd, pBuffer, uSize);
  if(rRead < 0)
    return -errno;

  return rRead;
#else
  DWORD uBytes;
  if(ReadFile(pFile->handle, pBuffer, uSize, (LPDWORD)&uBytes, NULL))
    return uBytes;
  return -1;
#endif
}

inline
static R64 File_ReadFrom(const File *pFile, U64 uOffset, Byte *pBuffer, U64 uSize){
#ifdef __linux__
  R64 rRead = pread(pFile->fd, pBuffer, uSize, uOffset);
  if(rRead < 0)
    return -errno;

  return rRead;
#else
#endif
}

inline
static R64 File_Write(const File *pFile, const Byte *pData, U64 uSize){
#ifdef __linux__
  R64 rWrote = write(pFile->fd, pData, uSize);
  if(rWrote < 0)
    return -errno;

  return rWrote;
#else
  DWORD uBytes;
  if(WriteFile(pFile->handle, pData, uSize, (LPDWORD)&uBytes, NULL))
    return uBytes;
  return -1;
#endif
}

inline
static R64 File_WriteTo(const File *pFile, U64 uOffset, const Byte *pData, U64 uSize){
#ifdef __linux__
  R64 rWrote = pwrite(pFile->fd, pData, uSize, uOffset);
  if(rWrote < 0)
    return -errno;

  return rWrote;
#else
#endif
}

typedef struct{
  P pData;
  U uBytes;
}FileData;

inline
static R64 File_ReadDatas(const File *pFile, const FileData *szBuffers, U32 uBuffers){
#ifdef __linux__
  R64 rRead = readv(pFile->fd, (struct iovec*)szBuffers, uBuffers);
  if(rRead < 0)
    return -errno;

  return rRead;
#else
#endif
}

inline
static R64 File_ReadDatasFrom(const File *pFile, U64 uOffset, const FileData *szBuffers, U32 uBuffers){
#ifdef __linux__
  R64 rRead = preadv(pFile->fd, (struct iovec*)szBuffers, uBuffers, uOffset);
  if(rRead < 0)
    return -errno;

  return rRead;
#else
#endif
}

inline
static R64 File_WriteDatas(const File *pFile, const FileData *szDatas, U32 uDatas){
#ifdef __linux__
  R64 rWrote = writev(pFile->fd, (struct iovec*)szDatas, uDatas);
  if(rWrote < 0)
    return -errno;

  return rWrote;
#else
#endif
}

inline
static R64 File_WriteDatasTo(const File *pFile, U64 uOffset, const FileData *szDatas, U32 uDatas){
#ifdef __linux__
  R64 rWrote = pwritev(pFile->fd, (struct iovec*)szDatas, uDatas, uOffset);
  if(rWrote < 0)
    return -errno;

  return rWrote;
#else
#endif
}

#endif // FILE_H

