#ifndef COUNTEDLIST_H
#define COUNTEDLIST_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

#include "List.h"

typedef struct{
    List list;
    U nCount;
}CountedList;

static inline void CountedList_Reset(CountedList *pList){
    List_Reset((List*)pList);
    pList->nCount = 0;
}

static inline void CountedList_Initialize(CountedList *pList){
    CountedList_Reset(pList);
}

static inline B CountedList_Empty(const CountedList *pList){
    return List_Empty((List*)pList);
}

static inline B CountedList_NotEmpty(const CountedList *pList){
    return List_NotEmpty((List*)pList);
}

static inline U CountedList_GetCount(const CountedList *pList){
    return pList->nCount;
}

static inline B CountedList_First(const CountedList *pList, const DoubleNode *pNode){
    return List_First((List*)pList, pNode);
}

static inline B CountedList_NotFirst(const CountedList *pList, const DoubleNode *pNode){
    return List_NotFirst((List*)pList, pNode);
}

static inline B CountedList_Last(const CountedList *pList, const DoubleNode *pNode){
    return List_Last((List*)pList, pNode);
}

static inline B CountedList_NotLast(const CountedList *pList, const DoubleNode *pNode){
    return List_NotLast((List*)pList, pNode);
}

static inline void CountedList_Insert(CountedList *pList, DoubleNode *pNode, DoubleNode *pPrev, DoubleNode *pNext){
    List_Insert(pNode, pPrev, pNext);
    ++pList->nCount;
}

static inline void CountedList_Remove(CountedList *pList, DoubleNode *pNode){
    List_Remove(pNode);
    --pList->nCount;
}

//push back
static inline void CountedList_Push(CountedList *pList, DoubleNode *pNode){
    List_Push((List*)pList, pNode);
    ++pList->nCount;
}

//pop front
static inline DoubleNode *CountedList_Pop(CountedList *pList){
    DoubleNode *pNode = List_Pop((List*)pList);
    --pList->nCount;
    return pNode;
}

static inline void CountedList_PushFront(CountedList *pList, DoubleNode *pNode){
    List_PushFront((List*)pList, pNode);
    ++pList->nCount;
}

static inline DoubleNode *CountedList_PopBack(CountedList *pList){
    DoubleNode *pNode = List_PopBack((List*)pList);
    --pList->nCount;
    return pNode;
}

static inline void CountedList_MoveNodeToBack(CountedList *pList, DoubleNode *pNode){
    List_MoveNodeToBack((List*)pList, pNode);
}

static inline void CountedList_MoveNodeToFront(CountedList *pList, DoubleNode *pNode){
    List_MoveNodeToFront((List*)pList, pNode);
}

//move to back
static inline void CountedList_MoveTo(CountedList *pList, CountedList *pTargetList){
    List_MoveTo((List*)pList, (List*)pTargetList);
    pTargetList->nCount += pList->nCount;
    pList->nCount = 0;
}

static inline void CountedList_MoveToFront(CountedList *pList, CountedList *pTargetList){
    List_MoveToFront((List*)pList, (List*)pTargetList);
    pTargetList->nCount += pList->nCount;
    pList->nCount = 0;
}

static inline void CountedList_Swap(CountedList *pList, CountedList *pTargetList){
    List_Swap ((List*)pList, (List*)pTargetList);
    U nTmp = pList->nCount;
    pList->nCount = pTargetList->nCount;
    pTargetList->nCount = nTmp;
}

#endif //COUNTEDLIST_H
