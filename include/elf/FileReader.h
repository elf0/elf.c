#ifndef FILEREADER_H
#define FILEREADER_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "File.h"

typedef struct FileReader FileReader;
typedef I32 (*FileReadFunction)(FileReader *pReader, Byte *pBuffer, U32 nSize);

struct FileReader{
    File file;
    U64 nOffset;
    FileReadFunction Read;
};

static inline I32 FileReader_DefaultRead(FileReader *pReader, Byte *pBuffer, U32 nSize);

static inline Bool FileReader_Open(FileReader *pReader, const Char *pszPathName){
    if(!File_OpenForRead(&pReader->file, pszPathName))
        return false;

    pReader->nOffset = 0;
    pReader->Read = FileReader_DefaultRead;
    return true;
}

static inline void FileReader_Close(FileReader *pReader){
    File_Close(&pReader->file);
}

static inline void FileReader_Seek(FileReader *pReader, U64 nOffset){
    pReader->nOffset = nOffset;
}

static inline Bool FileReader_ReadMeta(FileReader *pReader){
    return File_ReadMeta(&pReader->file);
}

static inline I32 FileReader_Read(FileReader *pReader, Byte *pBuffer, U32 nSize){
    return pReader->Read(pReader, pBuffer, nSize);
}

static inline I32 FileReader_DefaultRead(FileReader *pReader, Byte *pBuffer, U32 nSize){
    I32 nResult = File_ReadAt(&pReader->file, pBuffer, nSize, pReader->nOffset);
    if(nResult == -1)
        return -1;

    pReader->nOffset += nResult;
    return nResult;
}

#endif // FILEREADER_H
