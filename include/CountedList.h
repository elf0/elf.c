#pragma once
//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "List.h"

typedef struct{
    elf_List list;
    MU nCount;
}elf_CountedList;

static inline void elf_CountedList_Reset (elf_CountedList *pList){
    elf_List_Reset((elf_List*)pList);
    pList->nCount = 0;
}

static inline Bool elf_CountedList_Empty(const elf_CountedList *pList){
    return elf_List_Empty((elf_List*)pList);
}

static inline Bool elf_CountedList_NotEmpty(const elf_CountedList *pList){
    return elf_List_NotEmpty((elf_List*)pList);
}

static inline void elf_CountedList_Insert(elf_CountedList *pList, elf_DoubleNode *pNode, elf_DoubleNode *pPrev, elf_DoubleNode *pNext){
    elf_List_Insert(pNode, pPrev, pNext);
    ++pList->nCount;
}

static inline void elf_CountedList_Remove (elf_CountedList *pList, elf_DoubleNode *pNode){
    elf_List_Remove(pNode);
    --pList->nCount;
}

//push back
static inline void elf_CountedList_Push (elf_CountedList *pList, elf_DoubleNode *pNode){
    elf_List_Push((elf_List*)pList, pNode);
    ++pList->nCount;
}

//pop front
static inline elf_DoubleNode *elf_CountedList_Pop (elf_CountedList *pList){
    elf_DoubleNode *pNode = elf_List_Pop((elf_List*)pList);
    --pList->nCount;
    return pNode;
}

static inline void elf_CountedList_PushFront (elf_CountedList *pList, elf_DoubleNode *pNode){
    elf_List_PushFront((elf_List*)pList, pNode);
    ++pList->nCount;
}

static inline elf_DoubleNode *CountedList_PopBack(elf_CountedList *pList){
    elf_DoubleNode *pNode = elf_List_PopBack((elf_List*)pList);
    --pList->nCount;
    return pNode;
}

//move to back
static inline void elf_CountedList_MoveTo (elf_CountedList *pList, elf_CountedList *pTargetList){
    elf_List_MoveTo((elf_List*)pList, (elf_List*)pTargetList);
    pTargetList->nCount += pList->nCount;
    pList->nCount = 0;
}

static inline void elf_CountedList_MoveToFront (elf_CountedList *pList, elf_CountedList *pTargetList){
    elf_List_MoveToFront((elf_List*)pList, (elf_List*)pTargetList);
    pTargetList->nCount += pList->nCount;
    pList->nCount = 0;
}

static inline void elf_CountedList_Swap (elf_CountedList *pList, elf_CountedList *pTargetList){
    elf_List_Swap ((elf_List*)pList, (elf_List*)pTargetList);
    MU nTmp = pList->nCount;
    pList->nCount = pTargetList->nCount;
    pTargetList->nCount = nTmp;
}
