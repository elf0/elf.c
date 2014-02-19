#pragma once
//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "DoubleNode.h"

typedef struct{
    elf_DoubleNode entry;
}elf_List;

static inline void elf_List_Reset(elf_List *pList){
    elf_DoubleNode_Reset((elf_DoubleNode*)pList);
}

static inline void elf_List_Initialize(elf_List *pList){
    elf_List_Reset(pList);
}

static inline Bool elf_List_Empty(const elf_List *pList){
    return elf_DoubleNode_Alone((elf_DoubleNode*)pList);
}

static inline Bool elf_List_NotEmpty(const elf_List *pList){
    return elf_DoubleNode_NotAlone((elf_DoubleNode*)pList);
}

static inline void elf_List_Insert(elf_DoubleNode *pNode, elf_DoubleNode *pPrev, elf_DoubleNode *pNext){
    elf_DoubleNode_Link(pNode, pPrev, pNext);
}

static inline void elf_List_Remove(elf_DoubleNode *pNode){
    elf_DoubleNode_Unlink(pNode);
}

//push back
static inline void elf_List_Push(elf_List *pList, elf_DoubleNode *pNode){
    elf_DoubleNode_Link(pNode, ((elf_DoubleNode*)pList)->pPrev, (elf_DoubleNode*)pList);
}

//pop front
static inline elf_DoubleNode *elf_List_Pop(elf_List *pList){
    elf_DoubleNode *pNode = ((elf_DoubleNode*)pList)->pNext;
    elf_DoubleNode_Unlink(pNode);
    return pNode;
}

static inline void elf_List_PushFront(elf_List *pList, elf_DoubleNode *pNode){
    elf_DoubleNode_Link(pNode, (elf_DoubleNode*)pList, ((elf_DoubleNode*)pList)->pNext);
}

static inline elf_DoubleNode *elf_List_PopBack (elf_List *pList){
    elf_DoubleNode *pNode = ((elf_DoubleNode*)pList)->pPrev;
    elf_DoubleNode_Unlink(pNode);
    return pNode;
}

//move to back
static inline void elf_List_MoveTo(elf_List *pList, elf_List *pTargetList){
    elf_DoubleNode_MoveBuddiesTo((elf_DoubleNode*)pList, (elf_DoubleNode*)pTargetList);
}

static inline void elf_List_MoveToFront(elf_List *pList, elf_List *pTargetList){
    elf_DoubleNode_MoveBuddiesToFront((elf_DoubleNode*)pList, (elf_DoubleNode*)pTargetList);
}

static inline void elf_List_Swap(elf_List *pList, elf_List *pTargetList){
    elf_DoubleNode_SwapBuddies((elf_DoubleNode*)pList, (elf_DoubleNode*)pTargetList);
}

