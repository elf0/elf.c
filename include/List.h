#pragma once
//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

//Double linked list

typedef struct struct_List_Node{
 struct struct_List_Node *pNext;
 struct struct_List_Node *pPrev;
}List_Node;

static inline void List_Initialize(List_Node *pEntry){
 pEntry->pNext = pEntry;
 pEntry->pPrev = pEntry;
}

static inline Bool List_Empty(List_Node *pEntry){
 return pEntry->pNext == pEntry;
}

static inline Bool List_NotEmpty(List_Node *pEntry){
 return pEntry->pNext != pEntry;
}

static inline void List_Remove(List_Node *pNode){
 pNode->pPrev->pNext = pNode->pNext;
 pNode->pNext->pPrev = pNode->pPrev;
}

static inline void List_Insert(List_Node *pPosition, List_Node *pNode){
 pNode->pPrev = pPosition->pPrev;
 pNode->pNext = pPosition;

 pPosition->pPrev->pNext = pNode;
 pPosition->pPrev = pNode;
}

static inline void List_Append(List_Node *pTarget, List_Node *pNode){
 pNode->pPrev = pTarget;
 pNode->pNext = pTarget->pNext;

 pTarget->pNext->pPrev = pNode;
 pTarget->pNext = pNode;
}

static inline List_Node *List_Pop(List_Node *pEntry){
 List_Node *pNode = pEntry->pNext;
 List_Remove(pNode);
 return pNode;
}

static inline void List_Push(List_Node *pEntry, List_Node *pNode){
 List_Append(pEntry, pNode);
}
