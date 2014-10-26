#ifndef FILE_H
#define FILE_H

#include <unistd.h>
#include <fcntl.h>
#include "Type.h"

typedef struct File File;

struct File{
    I32 fd;
    U64 nOffset;
    I32 (*Read)(File *pFile, Byte *pBuffer, U32 nSize);
    I32 (*Write)(File *pFile, const Byte *pData, U32 nSize);
};

static inline I32 File_PRead(File *pFile, Byte *pBuffer, U32 nSize);
static inline I32 File_PWrite(File *pFile, const Byte *pData, U32 nSize);
static inline I32 File_SRead(File *pFile, Byte *pBuffer, U32 nSize);
static inline I32 File_SWrite(File *pFile, const Byte *pData, U32 nSize);

static inline Bool File_Create(File *pFile, const Char *pszPathName){
    pFile->fd = open((const char*)pszPathName, O_CREAT | O_TRUNC | O_RDWR, 0644);
    if(pFile->fd == -1)
        return false;
    pFile->nOffset = 0;
    pFile->Read = File_PRead;
    pFile->Write = File_PWrite;
    return true;
}

static inline Bool File_CreateForWrite(File *pFile, const Char *pszPathName){
    pFile->fd = open((const char*)pszPathName, O_CREAT | O_TRUNC | O_WRONLY, 0644);
    if(pFile->fd == -1)
        return false;
    pFile->nOffset = 0;
    pFile->Read = 0;
    pFile->Write = File_PWrite;
    return true;
}

static inline Bool File_Open(File *pFile, const Char *pszPathName){
    pFile->fd = open((const char*)pszPathName, O_RDWR);
    if(pFile->fd == -1)
        return false;
    pFile->nOffset = 0;
    pFile->Read = File_PRead;
    pFile->Write = File_PWrite;
    return true;
}

static inline void File_OpenStdIn(File *pFile){
    pFile->fd = 0;
    pFile->nOffset = 0;
    pFile->Read = File_SRead;
    pFile->Write = 0;
}

static inline Bool File_OpenForRead(File *pFile, const Char *pszPathName){
    pFile->fd = open((const char*)pszPathName, O_RDONLY);
    if(pFile->fd == -1)
        return false;

    pFile->nOffset = 0;
    pFile->Read = File_PRead;
    pFile->Write = 0;
    return true;
}

static inline void File_Close(File *pFile){
    close(pFile->fd);
    pFile->fd = -1;
}

static inline I32 File_Read(File *pFile, Byte *pBuffer, U32 nSize){
    return pFile->Read(pFile, pBuffer, nSize);
}

static inline I32 File_Write(File *pFile, const Byte *pData, U32 nSize){
    return pFile->Write(pFile, pData, nSize);
}

static inline I32 File_SRead(File *pFile, Byte *pBuffer, U32 nSize){
    ssize_t nResult = read(pFile->fd, pBuffer, nSize);
    if(nResult == -1)
        return -1;

    pFile->nOffset += nResult;
    return nResult;
}

static inline I32 File_PRead(File *pFile, Byte *pBuffer, U32 nSize){
    ssize_t nResult = pread(pFile->fd, pBuffer, nSize, pFile->nOffset);
    if(nResult == -1)
        return -1;

    pFile->nOffset += nResult;
    return nResult;
}

static inline I32 File_ReadAt(const File *pFile, Byte *pBuffer, U32 nSize, U64 nOffset){
    return pread(pFile->fd, pBuffer, nSize, nOffset);
}

static inline I32 File_SWrite(File *pFile, const Byte *pData, U32 nSize){
    ssize_t nResult = write(pFile->fd, pData, nSize);
    if(nResult == -1)
        return -1;

    pFile->nOffset += nResult;
    return nResult;
}

static inline I32 File_PWrite(File *pFile, const Byte *pData, U32 nSize){
    ssize_t nResult = pwrite(pFile->fd, pData, nSize, pFile->nOffset);
    if(nResult == -1)
        return -1;

    pFile->nOffset += nResult;
    return nResult;
}

static inline I32 File_WriteAt(const File *pFile, const Byte *pData, U32 nSize, U64 nOffset){
    return pwrite(pFile->fd, pData, nSize, nOffset);
}

#endif // FILE_H
