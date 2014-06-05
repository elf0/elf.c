#ifndef PSTACK_H
#define PSTACK_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

//pointer stack

#include <assert.h>

typedef struct{
 Byte **ppBegin;
 Byte **ppEnd;
 Byte **ppCur;
}PStack;

static inline void PStack_Initialize(PStack *pStack, Byte **ppBegin, Byte **ppEnd){
 assert(ppBegin < ppEnd);
 pStack->ppBegin = ppBegin;
 pStack->ppEnd = pStack->ppCur = ppEnd;
}

static inline size_t PStack_Size(PStack *pStack){
 return pStack->ppEnd - pStack->ppBegin;
}

static inline size_t PStack_Count(PStack *pStack){
 return pStack->ppEnd - pStack->ppCur;
}

static inline Bool PStack_Full(PStack *pStack){
 return pStack->ppCur == pStack->ppBegin;
}

static inline Bool PStack_Empty(PStack *pStack){
 return pStack->ppCur == pStack->ppEnd;
}

static inline Bool PStack_NotEmpty(PStack *pStack){
 return pStack->ppCur != pStack->ppEnd;
}

static inline void *PStack_Pop(PStack *pStack){
 assert(pStack->ppCur < pStack->ppEnd);
 return *pStack->ppCur++;
}

static inline void PStack_Push(PStack *pStack, void *pObject){
 assert(pStack->ppCur > pStack->ppBegin);
 *--pStack->ppCur = pObject;
}

#endif //PSTACK_H
