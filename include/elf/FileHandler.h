#ifndef FILEHANDLER_H
#define FILEHANDLER_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "File.h"

typedef struct FileHandler FileHandler;
typedef I32 (*FileHandler_ReadFunction)(FileHandler *pHandler, Byte *pBuffer, U32 nSize);
typedef I32 (*FileHandler_WriteFunction)(FileHandler *pHandler, const Byte *pData, U32 nSize);

struct FileHandler{
    File file;
    U64 nReadOffset;
    U64 nWriteOffset;
    FileHandler_ReadFunction Read;
    FileHandler_WriteFunction Write;
};

static inline I32 FileHandler_DefaultRead(FileHandler *pHandler, Byte *pBuffer, U32 nSize);
static inline I32 FileHandler_DefaultWrite(FileHandler *pHandler, const Byte *pData, U32 nSize);

static inline Bool FileHandler_Create(FileHandler *pHandler, const Char *pszPathName){
    if(!File_Create(&pHandler->file, pszPathName))
        return false;

    pHandler->nReadOffset = 0;
    pHandler->nWriteOffset = 0;
    pHandler->Read = FileHandler_DefaultRead;
    pHandler->Write = FileHandler_DefaultWrite;
    return true;
}

static inline Bool FileHandler_Open(FileHandler *pHandler, const Char *pszPathName){
    if(!File_Open(&pHandler->file, pszPathName))
        return false;

    pHandler->nReadOffset = 0;
    pHandler->nWriteOffset = 0;
    pHandler->Read = FileHandler_DefaultRead;
    pHandler->Write = FileHandler_DefaultWrite;
    return true;
}

static inline Bool FileHandler_Prepare(FileHandler *pHandler, const Char *pszPathName){
    if(!File_Prepare(&pHandler->file, pszPathName))
        return false;

    pHandler->nReadOffset = 0;
    pHandler->nWriteOffset = 0;
    pHandler->Read = FileHandler_DefaultRead;
    pHandler->Write = FileHandler_DefaultWrite;
    return true;
}

static inline void FileHandler_Close(FileHandler *pHandler){
    File_Close(&pHandler->file);
}

static inline Bool FileHandler_ReadMeta(FileHandler *pHandler){
    return File_ReadMeta(&pHandler->file);
}

static inline void FileHandler_SeekRead(FileHandler *pHandler, U64 nOffset){
    pHandler->nReadOffset = nOffset;
}

static inline void FileHandler_SeekWrite(FileHandler *pHandler, U64 nOffset){
    pHandler->nWriteOffset = nOffset;
}

static inline Bool FileHandler_SeekWriteToEnd(FileHandler *pHandler){
    if(!FileHandler_ReadMeta(pHandler))
        return false;

    pHandler->nWriteOffset = pHandler->file.meta.st_size;
    return true;
}

static inline I32 FileHandler_Read(FileHandler *pHandler, Byte *pBuffer, U32 nSize){
    return pHandler->Read(pHandler, pBuffer, nSize);
}

static inline I32 FileHandler_DefaultRead(FileHandler *pHandler, Byte *pBuffer, U32 nSize){
    I32 nResult = File_ReadAt(&pHandler->file, pBuffer, nSize, pHandler->nReadOffset);
    if(nResult == -1)
        return -1;

    pHandler->nReadOffset += nResult;
    return nResult;
}

static inline I32 FileHandler_Write(FileHandler *pHandler, const Byte *pData, U32 nSize){
    return pHandler->Write(pHandler, pData, nSize);
}

static inline I32 FileHandler_DefaultWrite(FileHandler *pHandler, const Byte *pData, U32 nSize){
    ssize_t nResult = File_WriteAt(&pHandler->file, pData, nSize, pHandler->nWriteOffset);
    if(nResult == -1)
        return -1;

    pHandler->nWriteOffset += nResult;
    return nResult;
}
#endif // FILEHANDLER_H
