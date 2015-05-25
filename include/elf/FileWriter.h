#ifndef FILEWRITER_H
#define FILEWRITER_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "File.h"

typedef struct{
    File file;
    U64 nOffset;
}FileWriter;

static inline Bool FileWriter_Create(FileWriter *pWriter, const Char *pszPathName);
static inline Bool FileWriter_Open(FileWriter *pWriter, const Char *pszPathName);
static inline Bool FileWriter_Prepare(FileWriter *pWriter, const Char *pszPathName);
static inline Bool FileWriter_PrepareForAppend(FileWriter *pWriter, const Char *pszPathName);
static inline void FileWriter_Close(FileWriter *pWriter);
static inline Bool FileWriter_ReadMeta(FileWriter *pWriter);
static inline void FileWriter_Seek(FileWriter *pWriter, U64 nOffset);
static inline Bool FileWriter_SeekToEnd(FileWriter *pWriter);
static inline I32 FileWriter_Write(FileWriter *pWriter, const Byte *pData, U32 nSize);
static inline I32 FileWriter_WriteTo(FileWriter *pWriter, U64 nOffset, const Byte *pData, U32 nSize);

static inline Bool FileWriter_Create(FileWriter *pWriter, const Char *pszPathName){
    pWriter->file.fd = open((const char*)pszPathName, O_CREAT | O_TRUNC | O_WRONLY, 0644);
    if(pWriter->file.fd == -1)
        return false;

    pWriter->nOffset = 0;
    return true;
}

static inline Bool FileWriter_Open(FileWriter *pWriter, const Char *pszPathName){
    pWriter->file.fd = open((const char*)pszPathName, O_WRONLY);
    if(pWriter->file.fd == -1)
        return false;

    pWriter->nOffset = 0;
    return true;
}

static inline Bool FileWriter_Prepare(FileWriter *pWriter, const Char *pszPathName){
    pWriter->file.fd = open((const char*)pszPathName, O_WRONLY);
    if(pWriter->file.fd == -1){
        if(errno != ENOENT)
            return false;

        pWriter->file.fd = open((const char*)pszPathName, O_CREAT | O_WRONLY, 0644);
        if(pWriter->file.fd == -1)
            return false;
    }

    pWriter->nOffset = 0;
    return true;
}

static inline Bool FileWriter_PrepareForAppend(FileWriter *pWriter, const Char *pszPathName){
    if(!FileWriter_Prepare(pWriter, pszPathName))
        return false;

    if(FileWriter_SeekToEnd(pWriter))
        return true;

    FileWriter_Close(pWriter);
    return false;
}

static inline void FileWriter_Close(FileWriter *pWriter){
    File_Close(&pWriter->file);
}

static inline Bool FileWriter_ReadMeta(FileWriter *pWriter){
    return File_ReadMeta(&pWriter->file);
}

static inline void FileWriter_Seek(FileWriter *pWriter, U64 nOffset){
    pWriter->nOffset = nOffset;
}

static inline Bool FileWriter_SeekToEnd(FileWriter *pWriter){
    if(!File_ReadMeta(&pWriter->file))
        return false;

    pWriter->nOffset = pWriter->file.meta.st_size;
    return true;
}

static inline I32 FileWriter_Write(FileWriter *pWriter, const Byte *pData, U32 nSize){
    ssize_t nResult = File_WriteTo(&pWriter->file, pWriter->nOffset, pData, nSize);
    if(nResult == -1)
        return -1;

    pWriter->nOffset += nResult;
    return nResult;
}

static inline I32 FileWriter_WriteTo(FileWriter *pWriter, U64 nOffset, const Byte *pData, U32 nSize){
    return File_WriteTo(&pWriter->file, nOffset, pData, nSize);
}

#endif // FILEWRITER_H
