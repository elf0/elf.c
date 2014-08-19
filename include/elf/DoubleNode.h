#ifndef DOUBLENODE_H
#define DOUBLENODE_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

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
    pNode->pPrev->pNext = pNode->pNext;
    pNode->pNext->pPrev = pNode->pPrev;
}

static inline void DoubleNode_Reset(DoubleNode *pNode){
    pNode->pPrev = pNode->pNext = pNode;
}

static inline Bool DoubleNode_Alone(const DoubleNode *pNode){
    return pNode->pNext == pNode;
}

static inline Bool DoubleNode_NotAlone(const DoubleNode *pNode){
    return pNode->pNext != pNode;
}

static inline void DoubleNode_MoveBuddiesTo(DoubleNode *pNode, DoubleNode *pTargetNode){
    DoubleNode_LinkPrev(pNode->pNext, pTargetNode->pPrev);
    DoubleNode_LinkNext(pNode->pPrev, pTargetNode);
    DoubleNode_Reset(pNode);
}

static inline void DoubleNode_MoveBuddiesToFront(DoubleNode *pNode, DoubleNode *pTargetNode){
    DoubleNode_LinkPrev(pNode->pNext, pTargetNode);
    DoubleNode_LinkNext(pNode->pPrev, pTargetNode->pNext);
    DoubleNode_Reset(pNode);
}

static inline void DoubleNode_SwapBuddies(DoubleNode *pNode, DoubleNode *pTargetNode){
    DoubleNode tmp;
    DoubleNode_Reset(&tmp);
    DoubleNode_MoveBuddiesTo(pNode, &tmp);
    DoubleNode_MoveBuddiesTo(pTargetNode, pNode);
    DoubleNode_MoveBuddiesTo(&tmp, pTargetNode);
}

#endif //DOUBLENODE_H
