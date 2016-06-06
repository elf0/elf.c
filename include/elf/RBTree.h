#ifndef RBTREE
#define RBTREE

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

//Note: User must define RBTreeKey before this file
//Example:
//#define RBTreeKey I32
//#include "RBTree.h"
#ifndef RBTreeKey
#error "User must define RBTreeKey before this file"
#endif

//API
typedef struct RBTree  RBTree;
typedef struct RBTreeNode RBTreeNode;

static inline void RBTree_Initialize(RBTree *pTree);
static inline RBTreeNode *RBTree_Find(RBTree *pTree, RBTreeKey nKey);
static inline B RBTree_Insert(RBTree *pTree, RBTreeNode *pNew);

//Internal
struct RBTreeNode{
    //pParent must be first
    RBTreeNode *pParent;
    RBTreeNode *pLeft;
    RBTreeNode *pRight;
    U8                 bRed;
    RBTreeKey nKey;
};

#define RBTREE_ROOT(pTree) (*(RBTreeNode**)pTree)
#define RBTREE_SET_ROOT(pTree, pNode) (RBTREE_ROOT(pTree) = pNode)

#define RBTREE_NODE_PARENT(pNode) (*(RBTreeNode**)pNode)
#define RBTREE_NODE_SET_PARENT(pNode, pParent) (RBTREE_NODE_PARENT(pNode) = pParent)

typedef B (*RBTree_Insert_Function) (RBTree *pTree, RBTreeNode *pNode);

static B RBTree_InsertRoot(RBTree *pTree, RBTreeNode *pNew);
static B RBTree_InsertChild(RBTree *pTree, RBTreeNode *pNew);
static inline void RBTree_Balance(RBTree *pTree, RBTreeNode *pNode, RBTreeNode *pSentinel);
static inline void RBTree_LeftRotate(RBTree *pTree, RBTreeNode *pNode, RBTreeNode *pSentinel);
static inline void RBTree_RightRotate(RBTree *pTree, RBTreeNode *pNode, RBTreeNode *pSentinel);

struct RBTree{
    //pRoot must be first
    RBTreeNode     *pRoot;
    RBTree_Insert_Function   fInsert;
    RBTreeNode     ndSentinel;
};

static inline void RBTree_Initialize(RBTree *pTree){
    RBTreeNode *pSentinel = &pTree->ndSentinel;
    pSentinel->bRed = false;

    pTree->pRoot = pSentinel;
    pTree->fInsert = RBTree_InsertRoot;
}

static inline RBTreeNode *RBTree_Find(RBTree *pTree, RBTreeKey nKey){
    RBTreeNode  *pSentinel = &pTree->ndSentinel;
    RBTreeNode *pNode = RBTREE_ROOT(pTree);
    while(pNode != pSentinel){
        RBTreeKey nRightKey = pNode->nKey;
        if(nKey < nRightKey)
            pNode = pNode->pLeft;
        else if(nKey > nRightKey)
            pNode = pNode->pRight;
        else
            return pNode;
    }

    return null;
}

static inline B RBTree_Insert(RBTree *pTree, RBTreeNode *pNew){
    return pTree->fInsert(pTree, pNew);
}

static B RBTree_InsertRoot(RBTree *pTree, RBTreeNode *pNew){
    //    Assert(offsetof(RBTree, pRoot) == 0);
    //    Assert(offsetof(RBTreeNode, pParent) == 0);

    RBTreeNode  *pSentinel = &pTree->ndSentinel;
    pNew->pLeft = pSentinel;
    pNew->pRight = pSentinel;
    RBTREE_NODE_SET_PARENT(pNew, null);
    pNew->bRed = false;

    RBTREE_SET_ROOT(pTree, pNew);
    pTree->fInsert = RBTree_InsertChild;
    return true;
}

static B RBTree_InsertChild(RBTree *pTree, RBTreeNode *pNew){
    RBTreeNode *pSentinel = &pTree->ndSentinel;
    RBTreeNode *pNode = RBTREE_ROOT(pTree);

    RBTreeNode *pParent;
    while(true){
        RBTreeKey nRightKey = pNode->nKey;
        if(pNew->nKey < nRightKey){
            pParent = pNode;
            pNode = pNode->pLeft;
            if(pNode == pSentinel){
                pParent->pLeft = pNew;
                break;
            }
        }
        else if(pNew->nKey > nRightKey){
            pParent = pNode;
            pNode = pNode->pRight;
            if(pNode == pSentinel){
                pParent->pRight = pNew;
                break;
            }
        }
        else
            return false;
    }

    RBTREE_NODE_SET_PARENT(pNew, pParent);
    pNew->pLeft = pSentinel;
    pNew->pRight = pSentinel;
    pNew->bRed = true;

    RBTree_Balance(pTree, pNew, pSentinel);
    return true;
}

//Not implement yet
void RBTree_Delete(RBTree *pTree, RBTreeNode *pNode);

static inline void RBTree_Balance(RBTree *pTree, RBTreeNode *pNode, RBTreeNode *pSentinel){
    RBTreeNode *pGrandparent;
    RBTreeNode *pUncle;
    RBTreeNode *pParent;
    while((pParent = RBTREE_NODE_PARENT(pNode))->bRed){
        pGrandparent = RBTREE_NODE_PARENT(pParent);
        if(pParent == pGrandparent->pLeft){
            pUncle = pGrandparent->pRight;
            if(pUncle->bRed){
                pUncle->bRed = false;
                if(pGrandparent == RBTREE_ROOT(pTree)){
                    pParent->bRed = false;
                    return;
                }
                pNode = pGrandparent;
            }
            else{
                if(pNode == pParent->pRight){
                    pNode->bRed = false;
                    RBTree_LeftRotate(pTree, pParent, pSentinel);
                    RBTree_RightRotate(pTree, pGrandparent, pSentinel);
                    pGrandparent->bRed = true;
                    break;
                }
                RBTree_RightRotate(pTree, pGrandparent, pSentinel);
            }
        }
        else{
            pUncle = pGrandparent->pLeft;
            if(pUncle->bRed){
                pUncle->bRed = false;
                if(pGrandparent == RBTREE_ROOT(pTree)){
                    pParent->bRed = false;
                    return;
                }
                pNode = pGrandparent;
            }
            else{
                if(pNode == pParent->pLeft){
                    pNode->bRed = false;
                    RBTree_RightRotate(pTree, pParent, pSentinel);
                    RBTree_LeftRotate(pTree, pGrandparent, pSentinel);
                    pGrandparent->bRed = true;
                    break;
                }
                RBTree_LeftRotate(pTree, pGrandparent, pSentinel);
            }
        }
        pParent->bRed = false;
        pGrandparent->bRed = true;
    }
}

static inline void RBTree_LeftRotate(RBTree *pTree, RBTreeNode *pNode, RBTreeNode *pSentinel){
    RBTreeNode  *pParent = RBTREE_NODE_PARENT(pNode);
    RBTreeNode  *pRight = pNode->pRight;
    RBTreeNode  *pRightLeft = pRight->pLeft;

    RBTREE_NODE_PARENT(pNode) = pRight;
    //pNode->pLeft = pNode->pLeft;
    pNode->pRight = pRightLeft;

    RBTREE_NODE_SET_PARENT(pRight, pParent);
    pRight->pLeft = pNode;
    //pRight->pRight = pRight->pRight;

    if(pRightLeft != pSentinel)
        RBTREE_NODE_SET_PARENT(pRightLeft, pNode);

    if(pNode == RBTREE_ROOT(pTree))
        RBTREE_SET_ROOT(pTree, pRight);
    else if(pNode == pParent->pLeft)
        pParent->pLeft = pRight;
    else
        pParent->pRight = pRight;
}

static inline void RBTree_RightRotate(RBTree *pTree, RBTreeNode *pNode, RBTreeNode *pSentinel){
    RBTreeNode  *pParent = RBTREE_NODE_PARENT(pNode);
    RBTreeNode  *pLeft= pNode->pLeft;
    RBTreeNode  *pLeftRight = pLeft->pRight;

    RBTREE_NODE_PARENT(pNode) = pLeft;
    pNode->pLeft = pLeftRight;
    //pNode->pRight = pNode->pRight;

    RBTREE_NODE_SET_PARENT(pLeft, pParent);
    //pLeft->pLeft = pLeft->pLeft;
    pLeft->pRight = pNode;

    if(pLeftRight != pSentinel)
        RBTREE_NODE_SET_PARENT(pLeftRight, pNode);

    if(pNode == RBTREE_ROOT(pTree))
        RBTREE_SET_ROOT(pTree, pLeft);
    else if(pNode == pParent->pRight)
        pParent->pRight = pLeft;
    else
        pParent->pLeft = pLeft;
}
#endif // RBTREE

