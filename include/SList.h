#pragma once
//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

//Single linked list

typedef struct struct_SList_Node{
 struct struct_SList_Node *pNext;
}SList_Node;

static inline void SList_Initialize(SList_Node *pEntry){
 pEntry->pNext = pEntry;
}

static inline Bool SList_Empty(SList_Node *pEntry){
 return pEntry->pNext == pEntry;
}

static inline Bool SList_NotEmpty(SList_Node *pEntry){
 return pEntry->pNext != pEntry;
}

static inline SList_Node *SList_Next(SList_Node *pNode){
 return pNode->pNext;
}

static inline void SList_Append(SList_Node *pTarget, SList_Node *pNode){
 pNode->pNext = pTarget->pNext;
 pTarget->pNext = pNode;
}

static inline void SList_Remove(SList_Node *pNode, SList_Node *pPrev){
 pPrev->pNext = pNode->pNext;
}

static inline SList_Node *SList_Pop(SList_Node *pEntry){
 SList_Node *pNode = pEntry->pNext;
 SList_Remove(pNode, pEntry);
 return pNode;
}

static inline void SList_Push(SList_Node *pEntry, SList_Node *pNode){
 SList_Append(pEntry, pNode);
}
