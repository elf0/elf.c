#ifndef MAPPEDFILE_H
#define MAPPEDFILE_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

#include "File.h"

#ifdef __linux__
#include <sys/mman.h>
#else
#endif
/*
#define PAGE_SHIFT 12
//4096
#define PAGE_SIZE (1 << PAGE_SHIFT)
#define PAGE_SIZE_MASK_L (PAGE_SIZE - 1)
#define PAGE_SIZE_MASK (~PAGE_SIZE_MASK_L)
#define PAGE_ALIGN(s) ((s + PAGE_SIZE_MASK_L) & PAGE_SIZE_MASK)
#define PAGES_TO_SIZE(pages) (pages << PAGE_SHIFT)
*/

typedef struct{
  File file;
  Byte *pBegin;
#ifdef __linux__
#else
  HANDLE hMap;
#endif

}MMFile;

static inline E8 MMFile_Adjust(MMFile *pFile, I32 iSize);

static inline E8 MMFile_Create(MMFile *pFile, const C *szPath, U32 uSize){
  File *pfFile = &pFile->file;
  if(!File_Create(pfFile, szPath))
    return 1;
  pFile->pBegin = null;

  if(!File_SetSize(pfFile, uSize)){
    File_Close(pfFile);
    return 2;
  }

  if(!File_ReadMeta(pfFile)){
    File_Close(pfFile);
    return 3;
  }

  Byte *pBegin = (Byte*)mmap(null, uSize, PROT_READ | PROT_WRITE, MAP_SHARED, pfFile->fd, 0);
  if(pBegin == MAP_FAILED){
    File_Close(pfFile);
    return 4;
  }
  pFile->pBegin = pBegin;
  return 0;
}

static inline E8 MMFile_Open(MMFile *pFile, const C *szPath){
  File *pfFile = &pFile->file;
  if(!File_Open(pfFile, szPath))
    return 1;
  pFile->pBegin = null;

  if(!File_ReadMeta(pfFile)){
    File_Close(pfFile);
    return 2;
  }

  Byte *pBegin = (Byte*)mmap(null, pfFile->meta.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, pfFile->fd, 0);
  if(pBegin == MAP_FAILED){
    File_Close(pfFile);
    return 3;
  }

  pFile->pBegin = pBegin;
  return 0;
}

static inline E8 MMFile_Prepare(MMFile *pFile, const C *szPath, U32 uMinSize){
  File *pfFile = &pFile->file;
  if(!File_Prepare(pfFile, szPath))
    return 1;
  pFile->pBegin = null;

  if(!File_ReadMeta(pfFile)){
    File_Close(pfFile);
    return 2;
  }

  U64 uSize = File_GetSize(pfFile);
  if(uSize < uMinSize){
   E8 e = MMFile_Adjust(pFile, uMinSize - uSize);
   if(e){
    File_Close(pfFile);
    return e;
   }
  }

  Byte *pBegin = (Byte*)mmap(null, pfFile->meta.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, pfFile->fd, 0);
  if(pBegin == MAP_FAILED){
    File_Close(pfFile);
    return 3;
  }
  pFile->pBegin = pBegin;
  return 0;
}

static inline E8 MMFile_OpenForRead(MMFile *pFile, const C *szPath){
  File *pfFile = &pFile->file;
  if(!File_OpenForRead(pfFile, szPath))
    return 1;

  pFile->pBegin = null;
#ifdef __linux__
  if(!File_ReadMeta(pfFile)){
    File_Close(pfFile);
    return 2;
  }

  Byte *pBegin = (Byte*)mmap(null, pfFile->meta.st_size, PROT_READ, MAP_SHARED, pfFile->fd, 0);
  if(pBegin == MAP_FAILED){
    File_Close(pfFile);
    return 3;
  }
#else
  HANDLE hMap = CreateFileMapping(pfFile->handle, 0, PAGE_READONLY, 0, 0, 0);
  if(hMap == NULL){
    File_Close(pfFile);
    return 2;
  }
  
  Byte *pBegin = MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);
  if(pBegin == NULL){
    CloseHandle(hMap);
    File_Close(pfFile);
    return 3;
  }
  
  pFile->hMap = hMap;
#endif
  pFile->pBegin = pBegin;
  return 0;
}

static inline void MMFile_Close(MMFile *pFile){
  if(pFile->pBegin){
#ifdef __linux__
    munmap(pFile->pBegin, pFile->file.meta.st_size);
#else
    UnmapViewOfFile(pFile->pBegin);
    CloseHandle(pFile->hMap);
#endif
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

static inline E8 MMFile_Adjust(MMFile *pFile, I32 iSize){
  File *pfFile = &pFile->file;
  U64 uFileSize = MMFile_GetSize(pFile);
  munmap(pFile->pBegin, uFileSize);

  uFileSize += iSize;
  if(!File_SetSize(pfFile, uFileSize)){
    pFile->pBegin = null;
    return 1;
  }
  pfFile->meta.st_size = uFileSize;

  Byte *pBegin = (Byte*)mmap(null, uFileSize, PROT_READ | PROT_WRITE, MAP_SHARED, pfFile->fd, 0);
  if(pBegin == MAP_FAILED){
    pFile->pBegin = null;
    return 2;
  }

  pFile->pBegin = pBegin;
  return 0;
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

