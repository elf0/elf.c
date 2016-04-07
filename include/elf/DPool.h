#ifndef DPOOL_H
#define DPOOL_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

#include "Type.h"
#include "Stack.h"

typedef struct{
    Stack stack;
    U32 size;
}DPool;

static inline void DPool_Initialize(DPool *pPool, U32 size){
    Stack_Initialize((Stack*)pPool);
    pPool->size = size;
}

static inline void DPool_Clear(DPool *pPool){
    StackNode *pNode = Stack_Top((Stack*)pPool);
    StackNode *pFreeNode;
    while(pNode != (StackNode*)pPool){
        pFreeNode = pNode;
        pNode = pNode->pNext;
        free(pFreeNode);
    }

    Stack_Clear((Stack*)pPool);
}

static inline void DPool_Finalize(DPool *pPool){
    DPool_Clear(pPool);
    pPool->size = 0;
}

static inline U32 DPool_Size(DPool *pPool){
    return pPool->size;
}

static inline Byte *DPool_Pop(DPool *pPool){
    if(Stack_NotEmpty((Stack*)pPool))
        return (Byte*)Stack_Pop((Stack*)pPool) + sizeof(StackNode);
    else
        return (Byte*)malloc(sizeof(StackNode) + pPool->size) + sizeof(StackNode);
}

static inline void DPool_Push(DPool *pPool, Byte *pBuffer){
    Stack_Push((Stack*)pPool, (StackNode*)(pBuffer - sizeof(StackNode)));
}

#endif //DPOOL_H
