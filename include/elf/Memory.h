#ifndef MEMORY_H
#define MEMORY_H

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

//asm
static inline void Memory_SetPositiveDirection(){
    asm("cld\n\t");
}

static inline void Memory_SetNegativeDirection(){
    asm("std\n\t");
}

//From Ivy Bridge microarchitecture
static inline void Memory_FastCopy32(Pointer pDest, Pointer pSrc, U32 nBytes){
    asm(
    "rep movsb\n\t"
    :
    :"D" (pDest), "S" (pSrc), "c" (nBytes)
    :"memory");
}

static inline void Memory_Copy32(Pointer pDest, Pointer pSrc, U32 nBytes){
    asm volatile(
                "movl %%ecx, %%edx\n\t"
                "andl $7, %%edx\n\t"
                "shrl $3, %%ecx\n\t"
                "rep movsq\n\t"
                "movl %%edx, %%ecx\n\t"
                "rep movsb\n\t"
                :
                : "D" (pDest), "S" (pSrc), "c" (nBytes)
                : "%edx", "memory"
                );
}

static inline void Memory_Clear(Pointer pAddress, U32 nBytes){
    asm(
    "movl %%ecx, %%edx\n\t"
    "andl $7, %%edx\n\t"
    "shrl $3, %%ecx\n\t"
    "xorl %%eax, %%eax\n\t"
    "rep stosq\n\t"
    "movl %%edx, %%ecx\n\t"
    "rep stosb\n\t"
    :
    :"D" (pAddress), "c" (nBytes)
    : "%rax", "%edx", "memory"
    );
}

//From Ivy Bridge microarchitecture
static inline void Memory_FastClear(Pointer pAddress, U32 nBytes){
    asm(
    "xorb %%al, %%al\n\t"
    "rep stosb\n\t"
    :
    :"D" (pAddress), "c" (nBytes)
    : "%al", "memory");
}

//From Ivy Bridge microarchitecture
static inline void Memory_FastSet(Pointer pAddress, U8 nValue, U32 nBytes){
    asm(
    "rep stosb\n\t"
    :
    : "D" (pAddress), "a" (nValue), "c" (nBytes)
    : "memory"
    );
}
#endif //MEMORY_H
