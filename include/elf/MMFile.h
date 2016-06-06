#ifndef MAPPEDFILE_H
#define MAPPEDFILE_H

#include "File.h"
#include <sys/mman.h>

typedef struct{
  File file;
  Byte *pBegin;
}MMFile;

static inline Byte *MMFile_Open(MMFile *pFile, const C *szPath){
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

static inline Byte *MMFile_Prepare(MMFile *pFile, const C *szPath){
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

static inline const Byte *MMFile_OpenForRead(MMFile *pFile, const C *szPath){
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

static inline void MMFile_Close(MMFile *pFile){
  if(pFile->pBegin){
    munmap(pFile->pBegin, pFile->file.meta.st_size);
    pFile->pBegin = null;
  }
  File_Close(&pFile->file);
}

static inline Byte *MMFile_Address(MMFile *pFile, U64 offset){
  return pFile->pBegin + offset;
}

static inline U64 MMFile_GetSize(MMFile *pFile){
  return File_GetSize(&pFile->file);
}

static inline Byte *MMFile_SetSize(MMFile *pFile, U64 nSize){
  File *pfFile = &pFile->file;
  munmap(pFile->pBegin, pfFile->meta.st_size);
  if(!File_SetSize(pfFile, nSize))
    return pFile->pBegin = null;
  pfFile->meta.st_size = nSize;

  Byte *pBegin = (Byte*)mmap(null, nSize, PROT_READ | PROT_WRITE, MAP_SHARED, pfFile->fd, 0);
  return  pFile->pBegin = pBegin != MAP_FAILED? pBegin: null;
}

static inline B MMFile_Flush(MMFile *pFile){
  return File_Flush(&pFile->file);
}

static inline B MMFile_FlushData(MMFile *pFile){
  return File_FlushData(&pFile->file);
}

static inline B MMFile_FlushRange(MMFile *pFile, Byte *pBegin, U64 uOffset){
//  long lPageSize = sysconf(_SC_PAGESIZE);
  return msync(pBegin, uOffset, MS_SYNC) == 0;
}

#endif // MAPPEDFILE_H

