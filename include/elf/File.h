#ifndef FILE_H
#define FILE_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include "Type.h"

struct File{
    I32 fd;
    struct stat meta;
};

typedef struct File File;

static inline Bool File_Exists(const Char *pszPathName) {
  return access((const char*)pszPathName, F_OK) != -1;
}

static inline Bool File_Create(File *pFile, const Char *pszPathName){
    pFile->fd = open((const char*)pszPathName, O_CREAT | O_TRUNC | O_RDWR, 0644);
    return pFile->fd != -1;
}

static inline Bool File_CreateForWrite(File *pFile, const Char *pszPathName){
    pFile->fd = open((const char*)pszPathName, O_CREAT | O_TRUNC | O_WRONLY, 0644);
    return pFile->fd != -1;
}

static inline Bool File_Open(File *pFile, const Char *pszPathName){
    pFile->fd = open((const char*)pszPathName, O_RDWR);
    return pFile->fd != -1;
}

static inline Bool File_OpenForRead(File *pFile, const Char *pszPathName){
    pFile->fd = open((const char*)pszPathName, O_RDONLY);
    return pFile->fd != -1;
}

static inline Bool File_OpenForWrite(File *pFile, const Char *pszPathName){
    pFile->fd = open((const char*)pszPathName, O_WRONLY);
    return pFile->fd != -1;
}

static inline Bool File_Prepare(File *pFile, const Char *pszPathName){
    pFile->fd = open((const char*)pszPathName, O_RDWR);
    if(pFile->fd != -1)
        return true;

    if(errno != ENOENT)
        return false;

    pFile->fd = open((const char*)pszPathName, O_CREAT | O_RDWR, 0644);
    return pFile->fd != -1;
}

static inline Bool File_PrepareForWrite(File *pFile, const Char *pszPathName){
    pFile->fd = open((const char*)pszPathName, O_WRONLY);
    if(pFile->fd != -1)
        return true;

    if(errno != ENOENT)
        return false;

    pFile->fd = open((const char*)pszPathName, O_CREAT | O_WRONLY, 0644);
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

static inline Bool File_ReadMeta(File *pFile){
    return fstat(pFile->fd, &pFile->meta) == 0;
}

static inline U64 File_GetSize(File *pFile){
    return pFile->meta.st_size;
}

static inline Bool File_SetSize(File *pFile, U64 nSize){
    return ftruncate(pFile->fd, nSize) == 0;
}

static inline Bool File_Commit(File *pFile){
    return fsync(pFile->fd) == 0;
}

static inline I32 File_Read(const File *pFile, Byte *pBuffer, U32 nSize){
    return read(pFile->fd, pBuffer, nSize);
}

static inline I32 File_ReadFrom(const File *pFile, U64 nOffset, Byte *pBuffer, U32 nSize){
    return pread(pFile->fd, pBuffer, nSize, nOffset);
}

static inline I32 File_Write(const File *pFile, const Byte *pData, U32 nSize){
    return write(pFile->fd, pData, nSize);
}

static inline I32 File_WriteTo(const File *pFile, U64 nOffset, const Byte *pData, U32 nSize){
    return pwrite(pFile->fd, pData, nSize, nOffset);
}
#endif // FILE_H
