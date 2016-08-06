#ifndef DSTACK_H
#define DSTACK_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

#include "Type.h"

#ifndef DStack_ITEM_SIZE
#define DStack_ITEM_SIZE 1
#endif

typedef struct{
  Byte *pBegin;
  Byte *pEnd;
  Byte *pTop;
}DStack;

static inline E8 DStack_Initialize(DStack *pStack, U32 uBytes){
  Byte *pBuffer = (Byte*)malloc(uBytes);
  if(!pBuffer)
    return 1;
  pStack->pBegin = pBuffer;
  pStack->pTop = pStack->pEnd = pBuffer + uBytes;
  return 0;
}

static inline void DStack_Finalize(DStack *pStack){
  free(pStack->pBegin);
}

static inline U32 DStack_Size(DStack *pStack){
  return pStack->pEnd - pStack->pBegin;
}

static inline B DStack_Empty(DStack *pStack){
  return pStack->pTop == pStack->pEnd;
}

static inline B DStack_NotEmpty(DStack *pStack){
  return pStack->pTop != pStack->pEnd;
}

static inline B DStack_Full(DStack *pStack){
  return pStack->pTop == pStack->pBegin;
}

static inline B DStack_NotFull(DStack *pStack){
  return pStack->pTop != pStack->pBegin;
}

static inline Byte *DStack_Top(DStack *pStack){
  return pStack->pTop;
}

static inline Byte *DStack_Push(DStack *pStack){
  pStack->pTop -= DStack_ITEM_SIZE;
  return pStack->pTop;
}

static inline Byte *DStack_Pop(DStack *pStack){
  Byte *pTop = pStack->pTop;
  pStack->pTop += DStack_ITEM_SIZE;
  return pTop;
}

static inline E8 DStack_Increase(DStack *pStack, U32 uBytes){
  Byte *pBegin = pStack->pBegin;
  Byte *pEnd = pStack->pEnd;
  uBytes += pEnd - pBegin;
  Byte *pBuffer = (Byte*)malloc(uBytes);
  if(!pBuffer)
    return 1;

  Byte *pData = pStack->pTop;
  U32 uData = pEnd - pData;
  pEnd = pBuffer + uBytes;
  Byte *pTop = pEnd - uData;
  memcpy(pTop, pData, uData);
  free(pBegin);

  pStack->pBegin = pBuffer;
  pStack->pEnd = pEnd;
  pStack->pTop = pTop;
  return 0;
}
#endif // DSTACK_H
