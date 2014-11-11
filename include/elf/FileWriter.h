#ifndef FILEWRITER_H
#define FILEWRITER_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

typedef struct FileWriter FileWriter;
typedef I32 (*FileWriteFunction)(FileWriter *pWriter, const Byte *pData, U32 nSize);

struct FileWriter{
    int fd;
    struct stat meta;
    U64 nOffset;
    FileWriteFunction Write;
};

static inline void FileWriter_Close(FileWriter *pWriter);
static inline Bool FileWriter_SeekToEnd(FileWriter *pWriter);
static inline I32 FileWriter_DefaultWrite(FileWriter *pWriter, const Byte *pData, U32 nSize);

static inline Bool FileWriter_OpenForAppend(FileWriter *pWriter, const Char *pszPathName){
    pWriter->fd = open((const char*)pszPathName, O_WRONLY, 0644);
    if(pWriter->fd == -1)
        return false;

    if(!FileWriter_SeekToEnd(pWriter)){
        FileWriter_Close(pWriter);
        return false;
    }

    pWriter->Write = File_DefaultWrite;
    return true;
}

static inline void FileWriter_Close(FileWriter *pWriter){
    close(pWriter->fd);
    pWriter->fd = -1;
}

static inline Bool FileWriter_ReadMeta(FileWriter *pWriter){
    return fstat(pWriter->fd, &pWriter->meta) != -1;
}

static inline void FileWriter_Seek(FileWriter *pWriter, U64 nOffset){
    pWriter->nOffset = nOffset;
}

static inline Bool FileWriter_SeekToEnd(FileWriter *pWriter){
    if(!FileWriter_ReadMeta(pWriter))
        return false;

    pWriter->nOffset = pWriter->meta.st_size;
    return true;
}

static inline I32 FileWriter_Write(FileWriter *pWriter, const Byte *pData, U32 nSize){
    return pWriter->Write(pWriter, pData, nSize);
}

static inline I32 FileWriter_WriteAt(const FileWriter *pWriter, const Byte *pData, U32 nSize, U64 nOffset){
    return pwrite(pWriter->file.fd, pData, nSize, nOffset);
}

static inline I32 FileWriter_DefaultWrite(FileWriter *pWriter, const Byte *pData, U32 nSize){
    ssize_t nResult = FileWriter_WriteAt(pWriter, pData, nSize, pFile->nOffset);
    if(nResult == -1)
        return -1;

    pFile->nOffset += nResult;
    return nResult;
}
#endif // FILEWRITER_H
