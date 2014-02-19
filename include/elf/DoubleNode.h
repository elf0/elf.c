#pragma once
//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

typedef struct elf_struct_DoubleNode elf_DoubleNode;
struct elf_struct_DoubleNode{
    elf_DoubleNode *pNext;
    elf_DoubleNode *pPrev;
};

static inline void elf_DoubleNode_LinkPrev(elf_DoubleNode *pNode, elf_DoubleNode *pPrev){
    pNode->pPrev = pPrev;
    pPrev->pNext = pNode;
}

static inline void elf_DoubleNode_LinkNext(elf_DoubleNode *pNode, elf_DoubleNode *pNext){
    pNode->pNext = pNext;
    pNext->pPrev = pNode;
}

static inline void elf_DoubleNode_Link(elf_DoubleNode *pNode, elf_DoubleNode *pPrev, elf_DoubleNode *pNext){
    elf_DoubleNode_LinkPrev(pNode, pPrev);
    elf_DoubleNode_LinkNext(pNode, pNext);
}

static inline void elf_DoubleNode_Unlink(elf_DoubleNode *pNode){
    pNode->pPrev->pNext = pNode->pNext;
    pNode->pNext->pPrev = pNode->pPrev;
}

static inline void elf_DoubleNode_Reset(elf_DoubleNode *pNode){
    pNode->pPrev = pNode->pNext = pNode;
}

static inline Bool elf_DoubleNode_Alone(const elf_DoubleNode *pNode){
    return pNode->pNext == pNode;
}

static inline Bool elf_DoubleNode_NotAlone(const elf_DoubleNode *pNode){
    return pNode->pNext != pNode;
}

static inline void elf_DoubleNode_MoveBuddiesTo(elf_DoubleNode *pNode, elf_DoubleNode *pTargetNode){
    elf_DoubleNode_LinkPrev(pNode->pNext, pTargetNode->pPrev);
    elf_DoubleNode_LinkNext(pNode->pPrev, pTargetNode);
    elf_DoubleNode_Reset(pNode);
}

static inline void elf_DoubleNode_MoveBuddiesToFront(elf_DoubleNode *pNode, elf_DoubleNode *pTargetNode){
    elf_DoubleNode_LinkPrev(pNode->pNext, pTargetNode);
    elf_DoubleNode_LinkNext(pNode->pPrev, pTargetNode->pNext);
    elf_DoubleNode_Reset(pNode);
}

static inline void elf_DoubleNode_SwapBuddies(elf_DoubleNode *pNode, elf_DoubleNode *pTargetNode){
    elf_DoubleNode tmp;
    elf_DoubleNode_Reset(&tmp);
    elf_DoubleNode_MoveBuddiesTo(pNode, &tmp);
    elf_DoubleNode_MoveBuddiesTo(pTargetNode, pNode);
    elf_DoubleNode_MoveBuddiesTo(&tmp, pTargetNode);
}
