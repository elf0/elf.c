#ifndef FILEREADER_H
#define FILEREADER_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "File.h"

typedef struct{
    File file;
    U64 nOffset;
}FileReader;

static inline Bool FileReader_Open(FileReader *pReader, const Char *pszPathName){
    pReader->file.fd = open((const char*)pszPathName, O_RDONLY);
    if(pReader->file.fd == -1)
        return false;

    pReader->nOffset = 0;
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
    ssize_t nResult = File_ReadFrom(&pReader->file, pReader->nOffset, pBuffer, nSize);
    if(nResult == -1)
        return -1;

    pReader->nOffset += nResult;
    return nResult;
}

static inline I32 FileReader_ReadFrom(FileReader *pReader, U64 nOffset, Byte *pBuffer, U32 nSize){
    return File_ReadFrom(&pReader->file, nOffset, pBuffer, nSize);
}

static inline Byte *FileReader_Map(FileReader *pReader, const Char *pszPathName){
    if(!FileReader_Open(pReader, pszPathName))
        return null;

    if(!FileReader_ReadMeta(pReader)){
        FileReader_Close(pReader);
        return null;
    }

    Byte *pBegin = (Byte*)mmap(0, pReader->file.meta.st_size, PROT_READ, MAP_SHARED, pReader->file.fd, 0);
    FileReader_Close(pReader);
    if(pBegin == MAP_FAILED)
        return null;

    pReader->nOffset = 0;
    return pReader->file.pBegin = pBegin;
}

static inline void FileReader_Unmap(FileReader *pReader){
    File_Unmap(&pReader->file);
}

#endif // FILEREADER_H
