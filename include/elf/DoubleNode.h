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

inline
static void DoubleNode_LinkPrev(DoubleNode *pNode, DoubleNode *pPrev){
    pNode->pPrev = pPrev;
    pPrev->pNext = pNode;
}

inline
static void DoubleNode_LinkNext(DoubleNode *pNode, DoubleNode *pNext){
    pNode->pNext = pNext;
    pNext->pPrev = pNode;
}

inline
static void DoubleNode_Link(DoubleNode *pNode, DoubleNode *pPrev, DoubleNode *pNext){
    DoubleNode_LinkPrev(pNode, pPrev);
    DoubleNode_LinkNext(pNode, pNext);
}

inline
static void DoubleNode_Unlink(DoubleNode *pNode){
    DoubleNode *pNext = pNode->pNext;
    DoubleNode *pPrev = pNode->pPrev;
    pPrev->pNext = pNext;
    pNext->pPrev = pPrev;
}

inline
static void DoubleNode_Reset(DoubleNode *pNode){
    pNode->pPrev = pNode->pNext = pNode;
}

inline
static B DoubleNode_Alone(const DoubleNode *pNode){
    return pNode->pNext == pNode;
}

inline
static B DoubleNode_NotAlone(const DoubleNode *pNode){
    return pNode->pNext != pNode;
}

inline
static void DoubleNode_MoveBuddiesTo(DoubleNode *pNode, DoubleNode *pTargetNode){
    DoubleNode_LinkPrev(pNode->pNext, pTargetNode->pPrev);
    DoubleNode_LinkNext(pNode->pPrev, pTargetNode);
    DoubleNode_Reset(pNode);
}

inline
static void DoubleNode_MoveBuddiesToFront(DoubleNode *pNode, DoubleNode *pTargetNode){
    DoubleNode_LinkNext(pNode->pPrev, pTargetNode->pNext);
    DoubleNode_LinkPrev(pNode->pNext, pTargetNode);
    DoubleNode_Reset(pNode);
}

inline
static void DoubleNode_Swap(DoubleNode *pNode, DoubleNode *pTargetNode){
    pNode->pNext->pPrev = pTargetNode
    pNode->pPrev->pNext = pTargetNode

    pTargetNode->pNext->pPrev = pNode
    pTargetNode->pPrev->pNext = pNode

    DoubleNode *pTmp = pNode->pNext
    pNode->pNext = pTargetNode->pNext
    pTargetNode->pNext = pTmp

    pTmp = pNode->pPrev
    pNode->pPrev = pTargetNode->pPrev
    pTargetNode->pPrev = tmp
}

#endif //DOUBLENODE_H
