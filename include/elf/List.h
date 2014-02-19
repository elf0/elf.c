#pragma once
//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "DoubleNode.h"

typedef struct{
    elf_DoubleNode entry;
}elf_List;

static inline void elf_List_Reset (elf_List *pList){
    elf_DoubleNode_Reset(&pList->entry);
}

static inline Bool elf_List_Empty(const elf_List *pList){
    return elf_DoubleNode_Alone(&pList->entry);
}

static inline Bool elf_List_NotEmpty(const elf_List *pList){
    return elf_DoubleNode_NotAlone(&pList->entry);
}

static inline void elf_List_Insert (elf_DoubleNode *pNode, elf_DoubleNode *pPrev, elf_DoubleNode *pNext){
    elf_DoubleNode_Link(pNode, pPrev, pNext);
}

static inline void elf_List_Remove (elf_DoubleNode *pNode){
    elf_DoubleNode_Unlink(pNode);
}

//push back
static inline void elf_List_Push (elf_List *pList, elf_DoubleNode *pNode){
    elf_DoubleNode_Link(pNode, pList->entry.pPrev, &pList->entry);
}

//pop front
static inline elf_DoubleNode *elf_List_Pop (elf_List *pList){
    elf_DoubleNode *pNode = pList->entry.pNext;
    elf_DoubleNode_Unlink(pNode);
    return pNode;
}

static inline void elf_List_PushFront (elf_List *pList, elf_DoubleNode *pNode){
    elf_DoubleNode_Link(pNode, &pList->entry, pList->entry.pNext);
}

static inline elf_DoubleNode *elf_List_PopBack (elf_List *pList){
    elf_DoubleNode *pNode = pList->entry.pPrev;
    elf_DoubleNode_Unlink(pNode);
    return pNode;
}

//move to back
static inline void elf_List_MoveTo (elf_List *pList, elf_List *pTargetList){
    elf_DoubleNode_MoveBuddiesTo(&pList->entry, &pTargetList->entry);
}

static inline void elf_List_MoveToFront (elf_List *pList, elf_List *pTargetList){
    elf_DoubleNode_MoveBuddiesToFront(&pList->entry, &pTargetList->entry);
}

static inline void elf_List_Swap (elf_List *pList, elf_List *pTargetList){
    elf_DoubleNode_SwapBuddies(&pList->entry, &pTargetList->entry);
}
