#pragma once
//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

//OS memory management
//TODO: Move this file to 'os'/'platform' directory, use conditional compilation.

#include <assert.h>
#include <sys/mman.h>
#include "Type.h"

typedef struct{
 Byte *pBegin;
 Byte *pEnd;
}Memory;

static inline Byte *Memory_Allocate(Memory *pMemory, size_t nSize){
 Byte *p = (Byte*)mmap(0, nSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
 if(p == MAP_FAILED)
// Byte *p = (Byte*)malloc(nSize);
// if(p == NULL)
  return NULL;

 pMemory->pBegin = p;
 pMemory->pEnd    = p + nSize;
 return p;
}

static inline void Memory_Free(Memory *pMemory){
 munmap(pMemory->pBegin, pMemory->pEnd - pMemory->pBegin);
// free(pMemory->pBegin);
}

//16 bytes alignment
#define PLATFORM_ALIGNMENT 16

static inline Bool Memory_NotAligned(size_t nSize){
 return nSize & (PLATFORM_ALIGNMENT - 1);
}

static inline Bool Memory_Aligned(size_t nSize){
 return !Memory_NotAligned(nSize);
}

static inline size_t Memory_Align(size_t nSize){
 assert(Memory_NotAligned(nSize));
 return (nSize & ~(PLATFORM_ALIGNMENT - 1)) + PLATFORM_ALIGNMENT;
}
