#ifndef POOL_H
#define POOL_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

//object pool

#include <assert.h>
#include "Memory.h"
#include "PStack.h"

typedef struct{
 size_t nObjectSize;
 Byte *pBegin;
 Byte *pEnd;
 Byte *pLast;
 PStack used;
 Byte *pForAlignment;
}Pool;

static inline void Pool_Initialize(Pool *pPool, Byte **ppUsedBegin, Byte **ppUsedEnd, Byte *pObjectBegin, Byte *pObjectEnd, U32 nObjects, size_t nObjectSize){
 assert(Memory_Aligned((size_t)pPool));
 assert(Memory_Aligned((size_t)ppUsedBegin));
 assert(Memory_Aligned((size_t)ppUsedEnd));
 assert(Memory_Aligned((size_t)pObjectBegin));
 assert(Memory_Aligned((size_t)pObjectEnd));
 assert(Memory_Aligned(nObjectSize));
 assert((pObjectBegin + nObjects * nObjectSize) == pObjectEnd);
 assert((ppUsedBegin + nObjects) == ppUsedEnd);

 pPool->nObjectSize = nObjectSize;
 pPool->pLast = pPool->pBegin = pObjectBegin;
 pPool->pEnd = pObjectEnd;
 PStack_Initialize(&pPool->used, ppUsedBegin, ppUsedEnd);
}

//memory peak(bytes)
static inline size_t Pool_Peak(Pool *pPool){
 return pPool->pLast - pPool->pBegin;
}

//for memory allocation
static inline size_t Pool_UnusedCount(Pool *pPool){
 return (pPool->pEnd - pPool->pLast)/pPool->nObjectSize;
}

static inline Bool Pool_UnusedEmpty(Pool *pPool){
 return pPool->pLast == pPool->pEnd;
}

static inline Bool Pool_UnusedNotEmpty(Pool *pPool){
 return pPool->pLast != pPool->pEnd;
}

static inline void *Pool_PopUnused(Pool *pPool){
 assert(Pool_UnusedNotEmpty(pPool));
 void *pObject = pPool->pLast;
 pPool->pLast += pPool->nObjectSize;
 return pObject;
}

//for active object(thread, connection..)
static inline size_t Pool_UsedCount(Pool *pPool){
 return PStack_Count(&pPool->used);
}

static inline Bool Pool_UsedFull(Pool *pPool){
 return PStack_Full(&pPool->used);
}

static inline Bool Pool_UsedEmpty(Pool *pPool){
 return PStack_Empty(&pPool->used);
}

static inline Bool Pool_UsedNotEmpty(Pool *pPool){
 return PStack_NotEmpty(&pPool->used);
}

static inline void *Pool_PopUsed(Pool *pPool){
 return PStack_Pop(&pPool->used);
}

//for general purpose
static inline size_t Pool_Size(Pool *pPool){
 return PStack_Size(&pPool->used);
}

static inline Bool Pool_Empty(Pool *pPool){
 return Pool_UsedEmpty(pPool) && Pool_UnusedEmpty(pPool);
}

static inline Bool Pool_NotEmpty(Pool *pPool){
 return Pool_UsedNotEmpty(pPool) || Pool_UnusedNotEmpty(pPool);
}

static inline void *Pool_Pop(Pool *pPool){
 if(Pool_UsedNotEmpty(pPool))
  return Pool_PopUsed(pPool);
 else if(Pool_UnusedNotEmpty(pPool))
  return Pool_PopUnused(pPool);
 return NULL;
}

static inline void Pool_Push(Pool *pPool, void *pObject){
 PStack_Push(&pPool->used, pObject);
}

#endif //POOL_H
