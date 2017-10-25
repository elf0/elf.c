#ifndef LIST_H
#define LIST_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

#include "DoubleNode.h"

typedef struct{
    DoubleNode entry;
}List;

inline
static void List_Clear(List *pList){
    DoubleNode_Reset((DoubleNode*)pList);
}

inline
static void List_Initialize(List *pList){
    List_Clear(pList);
}

inline
static B List_Empty(const List *pList){
    return DoubleNode_Alone((DoubleNode*)pList);
}

inline
static B List_NotEmpty(const List *pList){
    return DoubleNode_NotAlone((DoubleNode*)pList);
}

inline
static DoubleNode *List_Front(const List *pList){
    return pList->entry.pNext;
}

inline
static DoubleNode *List_Back(const List *pList){
    return pList->entry.pPrev;
}

inline
static B List_First(const List *pList, const DoubleNode *pNode){
    return pNode->pPrev == ((DoubleNode*)pList);
}

inline
static B List_NotFirst(const List *pList, const DoubleNode *pNode){
    return pNode->pPrev != ((DoubleNode*)pList);
}

inline
static B List_Last(const List *pList, const DoubleNode *pNode){
    return pNode->pNext == ((DoubleNode*)pList);
}

inline
static B List_NotLast(const List *pList, const DoubleNode *pNode){
    return pNode->pNext != ((DoubleNode*)pList);
}

inline
static void List_Insert(DoubleNode *pNode, DoubleNode *pPrev, DoubleNode *pNext){
    DoubleNode_Link(pNode, pPrev, pNext);
}

inline
static void List_Remove(DoubleNode *pNode){
    DoubleNode_Unlink(pNode);
}

//push back
inline
static void List_Push(List *pList, DoubleNode *pNode){
    DoubleNode_Link(pNode, ((DoubleNode*)pList)->pPrev, (DoubleNode*)pList);
}

//pop front
inline
static DoubleNode *List_Pop(List *pList){
    DoubleNode *pNode = ((DoubleNode*)pList)->pNext;
    DoubleNode_Unlink(pNode);
    return pNode;
}

inline
static void List_PopTo(List *pList, List *pTarget){
    List_Push(pTarget, List_Pop(pList));
}

inline
static void List_PushFront(List *pList, DoubleNode *pNode){
    DoubleNode_Link(pNode, (DoubleNode*)pList, ((DoubleNode*)pList)->pNext);
}

inline
static void List_PopToFront(List *pList, List *pTarget){
    List_PushFront(pTarget, List_Pop(pList));
}

inline
static DoubleNode *List_PopBack (List *pList){
    DoubleNode *pNode = ((DoubleNode*)pList)->pPrev;
    DoubleNode_Unlink(pNode);
    return pNode;
}

inline
static void List_MoveNodeToBack(List *pList, DoubleNode *pNode){
    List_Remove(pNode);
    List_Push(pList, pNode);
}

inline
static void List_MoveNodeToFront(List *pList, DoubleNode *pNode){
    List_Remove(pNode);
    List_PushFront(pList, pNode);
}

//move to back
inline
static void List_MoveTo(List *pList, List *pTargetList){
    DoubleNode_MoveBuddiesTo((DoubleNode*)pList, (DoubleNode*)pTargetList);
}

inline
static void List_MoveToFront(List *pList, List *pTargetList){
    DoubleNode_MoveBuddiesToFront((DoubleNode*)pList, (DoubleNode*)pTargetList);
}

inline
static void List_Swap(List *pList, List *pTargetList){
    DoubleNode_Swap((DoubleNode*)pList, (DoubleNode*)pTargetList);
}

#endif //LIST_H

