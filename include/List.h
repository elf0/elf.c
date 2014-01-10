#pragma once
//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

//Double linked list

//Node
typedef struct struct_ListNode ListNode;
typedef struct struct_ListNode{
 ListNode *pNext;
 ListNode *pPrev;
};

static inline void ListNode_LinkPrev(ListNode *pNode, ListNode *pPrev){
 pNode->pPrev = pPrev;
 pPrev->pNext = pNode;
}

static inline void ListNode_LinkNext(ListNode *pNode, ListNode *pNext){
 pNode->pNext = pNext;
 pNext->pPrev = pNode;
}

static inline void ListNode_Link(ListNode *pNode, ListNode *pPrev, ListNode *pNext){
 ListNode_LinkPrev(pNode, pPrev);
 ListNode_LinkNext(pNode, pNext);
}

static inline void ListNode_Unlink(ListNode *pNode){
 pNode->pPrev->pNext = pNode->pNext;
 pNode->pNext->pPrev = pNode->pPrev;
}

static inline void ListNode_Reset(ListNode *pNode){
 pNode->pPrev = pNode->pNext = pNode;
}

static inline Bool ListNode_Alone(const ListNode *pNode){
 return pNode->pNext == pNode;
}

static inline Bool ListNode_NotAlone(const ListNode *pNode){
 return pNode->pNext != pNode;
}

static inline void ListNode_MoveBuddiesTo(ListNode *pNode, ListNode *pTargetNode){
 ListNode_LinkPrev(pNode->pNext, pTargetNode->pPrev);
 ListNode_LinkNext(pNode->pPrev, pTargetNode);
 ListNode_Reset(pNode);
}

static inline void ListNode_MoveBuddiesToFront(ListNode *pNode, ListNode *pTargetNode){
 ListNode_LinkPrev(pNode->pNext, pTargetNode);
 ListNode_LinkNext(pNode->pPrev, pTargetNode->pNext);
 ListNode_Reset(pNode);
}

static inline void ListNode_SwapBuddies(ListNode *pNode, ListNode *pTargetNode){
 ListNode tmp;
 ListNode_Reset(&tmp);
 ListNode_MoveBuddiesTo(pNode, &tmp);
 ListNode_MoveBuddiesTo(pTargetNode, pNode);
 ListNode_MoveBuddiesTo(&tmp, pTargetNode);
}
//List
//If you do not want counting, use ListNode directly.
typedef struct{
 Node entry;
 size_t nCount;
}List;

static inline void List_Reset(List *pList){
 ListNode_Reset(&pList->entry);
 pList->nCount = 0;
}

static inline Bool List_Empty(const List *pList){
 return ListNode_Alone(&pList->entry);
}

static inline Bool List_NotEmpty(const List *pList){
 return ListNode_NotAlone(&pList->entry);
}

static inline void List_Insert(List *pList, ListNode *pNode, ListNode *pPrev, ListNode *pNext){
 ListNode_Link(pNode, pPrev, pNext);
 ++pList->nCount;
}

static inline void List_Remove(List *pList, ListNode *pNode){
 ListNode_Unlink(pNode);
 --pList->nCount;
}

//push back
static inline void List_Push(List *pList, ListNode *pNode){
 ListNode_Link(pNode, pList->entry.pPrev, &pList->entry);
 ++pList->nCount;
}

//pop front
static inline ListNode *List_Pop(List *pList){
 ListNode *pNode = pList->entry.pNext;
 ListNode_Unlink(pNode);
 --pList->nCount;
 return pNode;
}

static inline void List_PushFront(List *pList, ListNode *pNode){
 ListNode_Link(pNode, &pList->entry, pList->entry.pNext);
 ++pList->nCount;
}

static inline ListNode *List_PopBack(List *pList){
 ListNode *pNode = pList->entry.pPrev;
 ListNode_Unlink(pNode);
 --pList->nCount;
 return pNode;
}

//move to back
static inline void List_MoveTo(List *pList, List *pTargetList){
 ListNode_MoveBuddiesTo(&pList->entry, &pTargetList->entry);
 pTargetList->nCount += pList->nCount;
 pList->nCount = 0;
}

static inline void List_MoveToFront(List *pList, List *pTargetList){
 ListNode_MoveBuddiesToFront(&pList->entry, &pTargetList->entry);
 pTargetList->nCount += pList->nCount;
 pList->nCount = 0;
}

static inline void List_Swap(List *pList, List *pTargetList){
 ListNode_SwapBuddies(&pList->entry, &pTargetList->entry);
 size_t nTmp = pList->nCount;
 pList->nCount = pTargetList->nCount;
 pTargetList->nCount = nTmp;
}
