#ifndef MAPPEDFILE_H
#define MAPPEDFILE_H

#include "File.h"

typedef struct{
  File file;
  Byte *pBegin;
}MappedFile;

static inline Byte *MappedFile_Open(MappedFile *pFile, const Char *szPath){
  File *pfFile = &pFile->file;
  if(!File_Open(pfFile, szPath))
    return null;

  if(!File_ReadMeta(pfFile)){
    File_Close(pfFile);
    return null;
  }

  Byte *pBegin = (Byte*)mmap(null, pfFile->meta.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, pfFile->fd, 0);
  if(pBegin == MAP_FAILED){
    File_Close(pfFile);
    return pFile->pBegin = null;
  }
  return pFile->pBegin = pBegin;
}

static inline Byte *MappedFile_Prepare(MappedFile *pFile, const Char *szPath){
  File *pfFile = &pFile->file;
  if(!File_Prepare(pfFile, szPath))
    return null;

  if(!File_ReadMeta(pfFile)){
    File_Close(pfFile);
    return null;
  }

  Byte *pBegin = (Byte*)mmap(null, pfFile->meta.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, pfFile->fd, 0);
  if(pBegin == MAP_FAILED){
    File_Close(pfFile);
    return pFile->pBegin = null;
  }
  return pFile->pBegin = pBegin;
}

static inline const Byte *MappedFile_OpenForRead(MappedFile *pFile, const Char *szPath){
  File *pfFile = &pFile->file;
  if(!File_OpenForRead(pfFile, szPath))
    return null;

  if(!File_ReadMeta(pfFile)){
    File_Close(pfFile);
    return null;
  }

  Byte *pBegin = (Byte*)mmap(null, pfFile->meta.st_size, PROT_READ, MAP_SHARED, pfFile->fd, 0);
  if(pBegin == MAP_FAILED){
    File_Close(pfFile);
    return pFile->pBegin = null;
  }
  return pFile->pBegin = pBegin;
}

static inline void MappedFile_Close(MappedFile *pFile){
  if(pFile->pBegin){
    munmap(pFile->pBegin, pFile->file.meta.st_size);
    pFile->pBegin = null;
  }
  File_Close(&pFile->file);
}

static inline Byte *MappedFile_Address(MappedFile *pFile, U64 offset){
  return pFile->pBegin + offset;
}

static inline U64 MappedFile_GetSize(MappedFile *pFile){
  return File_GetSize(&pFile->file);
}

static inline Byte *MappedFile_SetSize(MappedFile *pFile, U64 nSize){
  File *pfFile = &pFile->file;
  munmap(pFile->pBegin, pfFile->meta.st_size);
  if(!File_SetSize(pfFile, nSize))
    return pFile->pBegin = null;
  pfFile->meta.st_size = nSize;

  Byte *pBegin = (Byte*)mmap(null, nSize, PROT_READ | PROT_WRITE, MAP_SHARED, pfFile->fd, 0);
  return  pFile->pBegin = pBegin != MAP_FAILED? pBegin: null;
}

static inline Bool MappedFile_Flush(MappedFile *pFile){
  return File_Flush(&pFile->file);
}

static inline Bool MappedFile_FlushData(MappedFile *pFile){
  return File_FlushData(&pFile->file);
}

static inline Bool MappedFile_FlushRange(MappedFile *pFile, Byte *pBegin, U64 uOffset){
//  long lPageSize = sysconf(_SC_PAGESIZE);
  return msync(pBegin, uOffset, MS_SYNC) == 0;
}

#endif // MAPPEDFILE_H

