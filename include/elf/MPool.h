#pragma once
//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

//memory pool

#include <assert.h>
#include "Memory.h"

typedef struct struct_MPool_Node{
 struct struct_MPool_Node *pNext;
}MPool_Node;

typedef struct{
 size_t nObjectSize;
 Byte *pBegin;
 Byte *pEnd;
 Byte *pLast;
 MPool_Node *pUsedEntry;
 Byte *pForAlignment;
}MPool;

static inline void MPool_Initialize(MPool *pPool, Byte *pObjectBegin, Byte *pObjectEnd, U32 nObjects, size_t nObjectSize){
 assert(Memory_Aligned((size_t)pPool));
 assert(Memory_Aligned((size_t)pObjectBegin));
 assert(Memory_Aligned((size_t)pObjectEnd));
 assert(Memory_Aligned(nObjectSize));
 assert((pObjectBegin + nObjects * nObjectSize) == pObjectEnd);

 pPool->nObjectSize = nObjectSize;
 pPool->pBegin = pPool->pLast = pObjectBegin;
 pPool->pEnd = pObjectEnd;
 pPool->pUsedEntry = null;
}

//memory peak(bytes)
static inline size_t MPool_Peak(MPool *pPool){
 return pPool->pLast - pPool->pBegin;
}

static inline Bool MPool_UnusedEmpty(MPool *pPool){
 return pPool->pLast == pPool->pEnd;
}

static inline Bool MPool_UnusedNotEmpty(MPool *pPool){
 return pPool->pLast != pPool->pEnd;
}

static inline void *MPool_PopUnused(MPool *pPool){
  void *pObject = pPool->pLast;
  pPool->pLast += pPool->nObjectSize;
 return pObject;
}

static inline Bool MPool_UsedEmpty(MPool *pPool){
 return pPool->pUsedEntry == null;
}

static inline Bool MPool_UsedNotEmpty(MPool *pPool){
 return pPool->pUsedEntry != null;
}

static inline void *MPool_PopUsed(MPool *pPool){
  void *pObject = pPool->pUsedEntry;
  pPool->pUsedEntry = pPool->pUsedEntry->pNext;
 return pObject;
}

static inline Bool MPool_Empty(MPool *pPool){
 return MPool_UsedEmpty(pPool) && MPool_UnusedEmpty(pPool);
}

static inline Bool MPool_NotEmpty(MPool *pPool){
 return MPool_UsedNotEmpty(pPool) || MPool_UnusedNotEmpty(pPool);
}

static inline void *MPool_Pop(MPool *pPool){
 if(MPool_UsedNotEmpty(pPool))
  return MPool_PopUsed(pPool);
 else if(MPool_UnusedNotEmpty(pPool))
  return MPool_PopUnused(pPool);

 return null;
}

static inline void MPool_Push(MPool *pPool, void *pObject){
 MPool_Node *pNode = (MPool_Node*)pObject;
 pNode->pNext = pPool->pUsedEntry;
 pPool->pUsedEntry = pNode;
}
