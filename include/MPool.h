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
 MPool_Node *pUsedList;
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
 pPool->pUsedList = NULL;
}

//memory peak(bytes)
static inline size_t MPool_Peak(MPool *pPool){
 return pPool->pLast - pPool->pBegin;
}

static inline void *MPool_Pop(MPool *pPool){
 if(pPool->pUsedList){
  void *pObject = pPool->pUsedList;
  pPool->pUsedList = pPool->pUsedList->pNext;
  return pObject;
 }
 else if(pPool->pLast < pPool->pEnd){
  void *pObject = pPool->pLast;
  pPool->pLast += pPool->nObjectSize;
  return pObject;
 }

 return NULL;
}

static inline void MPool_Push(MPool *pPool, void *pObject){
 MPool_Node *pNode = (MPool_Node*)pObject;
 pNode->pNext = pPool->pUsedList;
 pPool->pUsedList = pNode;
}

