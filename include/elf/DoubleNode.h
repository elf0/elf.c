#ifndef DOUBLENODE_H
#define DOUBLENODE_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

#include "Type.h"

typedef struct struct_DoubleNode DoubleNode;
struct struct_DoubleNode{
    DoubleNode *pNext;
    DoubleNode *pPrev;
};

static inline void DoubleNode_LinkPrev(DoubleNode *pNode, DoubleNode *pPrev){
    pNode->pPrev = pPrev;
    pPrev->pNext = pNode;
}

static inline void DoubleNode_LinkNext(DoubleNode *pNode, DoubleNode *pNext){
    pNode->pNext = pNext;
    pNext->pPrev = pNode;
}

static inline void DoubleNode_Link(DoubleNode *pNode, DoubleNode *pPrev, DoubleNode *pNext){
    DoubleNode_LinkPrev(pNode, pPrev);
    DoubleNode_LinkNext(pNode, pNext);
}

static inline void DoubleNode_Unlink(DoubleNode *pNode){
    DoubleNode *pNext = pNode->pNext;
    DoubleNode *pPrev = pNode->pPrev;
    pPrev->pNext = pNext;
    pNext->pPrev = pPrev;
}

static inline void DoubleNode_Reset(DoubleNode *pNode){
    pNode->pPrev = pNode->pNext = pNode;
}

static inline B DoubleNode_Alone(const DoubleNode *pNode){
    return pNode->pNext == pNode;
}

static inline B DoubleNode_NotAlone(const DoubleNode *pNode){
    return pNode->pNext != pNode;
}

static inline void DoubleNode_MoveBuddiesTo(DoubleNode *pNode, DoubleNode *pTargetNode){
    DoubleNode_LinkPrev(pNode->pNext, pTargetNode->pPrev);
    DoubleNode_LinkNext(pNode->pPrev, pTargetNode);
    DoubleNode_Reset(pNode);
}

static inline void DoubleNode_MoveBuddiesToFront(DoubleNode *pNode, DoubleNode *pTargetNode){
    DoubleNode_LinkNext(pNode->pPrev, pTargetNode->pNext);
    DoubleNode_LinkPrev(pNode->pNext, pTargetNode);
    DoubleNode_Reset(pNode);
}

static inline void DoubleNode_SwapBuddies(DoubleNode *pNode, DoubleNode *pTargetNode){
    DoubleNode tmp = *pNode;
    *pNode = *pTargetNode;
    *pTargetNode = tmp;
}

#endif //DOUBLENODE_H
