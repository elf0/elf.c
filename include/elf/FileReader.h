#ifndef FILEREADER_H
#define FILEREADER_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

typedef struct FileReader FileReader;
typedef I32 (*FileReadFunction)(FileReader *pReader, Byte *pBuffer, U32 nSize);

struct FileReader{
    int fd;
    struct stat meta;
    U64 nOffset;
    FileReadFunction Read;
};

static inline I32 FileReader_DefaultRead(FileReader *pReader, Byte *pBuffer, U32 nSize);

static inline Bool FileReader_Open(FileReader *pReader, const Char *pszPathName){
    int fd = open((const char*)pszPathName, O_RDONLY);
    if(fd == -1)
        return false;

    pReader->fd = fd;
    pReader->nOffset = 0;
    pReader->Read = FileReader_DefaultRead;
    return true;
}

static inline void FileReader_Close(FileReader *pReader){
    close(pReader->fd);
    pReader->fd = -1;
}

static inline void FileReader_Seek(FileReader *pReader, U64 nOffset){
    pReader->nOffset = nOffset;
}

static inline Bool FileReader_ReadMeta(FileReader *pReader){
    return fstat(pReader->fd, &pReader->meta) != -1;
}

static inline I32 FileReader_Read(FileReader *pReader, Byte *pBuffer, U32 nSize){
    return pReader->Read(pReader, pBuffer, nSize);
}

static inline I32 FileReader_ReadAt(const FileReader *pReader, Byte *pBuffer, U32 nSize, U64 nOffset){
    return pread(pReader->fd, pBuffer, nSize, nOffset);
}

static inline I32 FileReader_DefaultRead(FileReader *pReader, Byte *pBuffer, U32 nSize){
    I32 nResult = FileReader_ReadAt(pReader, pBuffer, nSize, pReader->nOffset);
    if(nResult == -1)
        return -1;

    pReader->nOffset += nResult;
    return nResult;
}

#endif // FILEREADER_H
