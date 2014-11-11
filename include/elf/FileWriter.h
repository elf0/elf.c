#ifndef FILEWRITER_H
#define FILEWRITER_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "File.h"

typedef struct FileWriter FileWriter;
typedef I32 (*FileWriteFunction)(FileWriter *pWriter, const Byte *pData, U32 nSize);

struct FileWriter{
    File file;
    U64 nOffset;
    FileWriteFunction Write;
};

static inline Bool FileWriter_SeekToEnd(FileWriter *pWriter);
static inline I32 FileWriter_DefaultWrite(FileWriter *pWriter, const Byte *pData, U32 nSize);

static inline Bool FileWriter_OpenForAppend(FileWriter *pWriter, const Char *pszPathName){
    if(!File_OpenForWrite(&pWriter->file, pszPathName))
        return false;

    if(!FileWriter_SeekToEnd(pWriter)){
        File_Close(&pWriter->file);
        return false;
    }

    pWriter->Write = File_DefaultWrite;
    return true;
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
    if(!File_ReadMeta(pWriter))
        return false;

    pWriter->nOffset = pWriter->meta.st_size;
    return true;
}

static inline I32 FileWriter_Write(FileWriter *pWriter, const Byte *pData, U32 nSize){
    return pWriter->Write(pWriter, pData, nSize);
}

static inline I32 FileWriter_DefaultWrite(FileWriter *pWriter, const Byte *pData, U32 nSize){
    ssize_t nResult = File_WriteAt(&pWriter->file, pData, nSize, pWriter->nOffset);
    if(nResult == -1)
        return -1;

    pWriter->nOffset += nResult;
    return nResult;
}
#endif // FILEWRITER_H
