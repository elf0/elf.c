#ifndef LIST_H
#define LIST_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

#include "DoubleNode.h"

typedef struct{
    DoubleNode entry;
}List;

static inline void List_Clear(List *pList){
    DoubleNode_Reset((DoubleNode*)pList);
}

static inline void List_Initialize(List *pList){
    List_Clear(pList);
}

static inline B List_Empty(const List *pList){
    return DoubleNode_Alone((DoubleNode*)pList);
}

static inline B List_NotEmpty(const List *pList){
    return DoubleNode_NotAlone((DoubleNode*)pList);
}

static inline DoubleNode *List_Front(const List *pList){
    return pList->entry.pNext;
}

static inline DoubleNode *List_Back(const List *pList){
    return pList->entry.pPrev;
}

static inline B List_First(const List *pList, const DoubleNode *pNode){
    return pNode->pPrev == ((DoubleNode*)pList);
}

static inline B List_NotFirst(const List *pList, const DoubleNode *pNode){
    return pNode->pPrev != ((DoubleNode*)pList);
}

static inline B List_Last(const List *pList, const DoubleNode *pNode){
    return pNode->pNext == ((DoubleNode*)pList);
}

static inline B List_NotLast(const List *pList, const DoubleNode *pNode){
    return pNode->pNext != ((DoubleNode*)pList);
}

static inline void List_Insert(DoubleNode *pNode, DoubleNode *pPrev, DoubleNode *pNext){
    DoubleNode_Link(pNode, pPrev, pNext);
}

static inline void List_Remove(DoubleNode *pNode){
    DoubleNode_Unlink(pNode);
}

//push back
static inline void List_Push(List *pList, DoubleNode *pNode){
    DoubleNode_Link(pNode, ((DoubleNode*)pList)->pPrev, (DoubleNode*)pList);
}

//pop front
static inline DoubleNode *List_Pop(List *pList){
    DoubleNode *pNode = ((DoubleNode*)pList)->pNext;
    DoubleNode_Unlink(pNode);
    return pNode;
}

static inline void List_PopTo(List *pList, List *pTarget){
    List_Push(pTarget, List_Pop(pList));
}

static inline void List_PushFront(List *pList, DoubleNode *pNode){
    DoubleNode_Link(pNode, (DoubleNode*)pList, ((DoubleNode*)pList)->pNext);
}

static inline void List_PopToFront(List *pList, List *pTarget){
    List_PushFront(pTarget, List_Pop(pList));
}

static inline DoubleNode *List_PopBack (List *pList){
    DoubleNode *pNode = ((DoubleNode*)pList)->pPrev;
    DoubleNode_Unlink(pNode);
    return pNode;
}

static inline void List_MoveNodeToBack(List *pList, DoubleNode *pNode){
    List_Remove(pNode);
    List_Push(pList, pNode);
}

static inline void List_MoveNodeToFront(List *pList, DoubleNode *pNode){
    List_Remove(pNode);
    List_PushFront(pList, pNode);
}

//move to back
static inline void List_MoveTo(List *pList, List *pTargetList){
    DoubleNode_MoveBuddiesTo((DoubleNode*)pList, (DoubleNode*)pTargetList);
}

static inline void List_MoveToFront(List *pList, List *pTargetList){
    DoubleNode_MoveBuddiesToFront((DoubleNode*)pList, (DoubleNode*)pTargetList);
}

static inline void List_Swap(List *pList, List *pTargetList){
    DoubleNode_SwapBuddies((DoubleNode*)pList, (DoubleNode*)pTargetList);
}

#endif //LIST_H

