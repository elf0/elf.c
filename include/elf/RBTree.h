#ifndef RBTREE_H
#define RBTREE_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

#include "RBTree_Node.h"

//API
typedef struct RBTree  RBTree;

//User must implement "RBTree_Node_Allocate()", "RBTree_Node_Compare()" and "RBTree_Node_Free()"
inline
static RBTree_Node *RBTree_Node_Allocate(void *pContext, const Byte *pKey, U32 uKey);
inline
static void RBTree_Node_Free(void *pContext, RBTree_Node *pNode);
inline
static I8 RBTree_Node_Compare(const Byte *pLeft, U32 uLeft, const RBTree_Node *pNode);

inline
static void RBTree_Initialize(RBTree *pTree);
inline
static void RBTree_Finalize(RBTree *pTree);
inline
static RBTree_Node *RBTree_Find(RBTree *pTree, const Byte *pKey, U32 uKey);
inline
static B RBTree_Add(RBTree *pTree, const Byte *pKey, U32 uKey, void *pContext, RBTree_Node **ppNode);

//Internal


#define RBTREE_ROOT(pTree) (*(RBTree_Node**)pTree)
#define RBTREE_SET_ROOT(pTree, pNode) (RBTREE_ROOT(pTree) = pNode)

#define RBTREE_NODE_PARENT(pNode) (*(RBTree_Node**)pNode)
#define RBTREE_NODE_SET_PARENT(pNode, pParent) (RBTREE_NODE_PARENT(pNode) = pParent)

typedef E8 (*RBTree_Add_f) (RBTree *pTree, const Byte *pKey, U32 uKey, void *pContext, RBTree_Node **ppNode);

inline
static E8 RBTree_AddRoot(RBTree *pTree, const Byte *pKey, U32 uKey, void *pContext, RBTree_Node **ppNode);
inline
static E8 RBTree_AddChild(RBTree *pTree, const Byte *pKey, U32 uKey, void *pContext, RBTree_Node **ppNode);
inline
static void RBTree_Balance(RBTree *pTree, RBTree_Node *pNode);

struct RBTree{
  RBTree_Node     *pRoot;
  RBTree_Add_f   fAdd;
};

inline
static void RBTree_Initialize(RBTree *pTree){
  pTree->pRoot = 0;
  pTree->fAdd = RBTree_AddRoot;
}

inline
static void RBTree_Free(RBTree_Node *pNode, void *pContext){
  if(pNode->pLeft)
    RBTree_Free(pNode->pLeft, pContext);

  if(pNode->pRight)
    RBTree_Free(pNode->pRight, pContext);

  RBTree_Node_Free(pContext, pNode);
}

inline
static void RBTree_Finalize(RBTree *pTree, void *pContext){
  RBTree_Node *pNode = RBTREE_ROOT(pTree);
  if(pNode){
    RBTree_Free(pNode, pContext);
    RBTREE_SET_ROOT(pTree, 0);
  }
}

inline
static RBTree_Node *RBTree_Find(RBTree *pTree, const Byte *pKey, U32 uKey){
  RBTree_Node *pNode = RBTREE_ROOT(pTree);
  I8 iCompare;
  while(pNode){
    iCompare = RBTree_Node_Compare(pKey, uKey, pNode);
    if(iCompare < 0)
      pNode = pNode->pLeft;
    else if(iCompare > 0)
      pNode = pNode->pRight;
    else
      return pNode;
  }

  return 0;
}

inline
static E8 RBTree_Add(RBTree *pTree, const Byte *pKey, U32 uKey
                            , void *pContext, RBTree_Node **ppNode){
  return pTree->fAdd(pTree, pKey, uKey, pContext, ppNode);
}

inline
static E8 RBTree_AddRoot(RBTree *pTree, const Byte *pKey, U32 uKey
                                , void *pContext, RBTree_Node **ppNode){
  RBTree_Node *pNew = RBTree_Node_Allocate(pContext, pKey, uKey);
  pNew->pLeft = 0;
  pNew->pRight = 0;
  RBTREE_NODE_SET_PARENT(pNew, 0);
  pNew->bRed = 0;

  RBTREE_SET_ROOT(pTree, pNew);
  pTree->fAdd = RBTree_AddChild;

  *ppNode = pNew;
  return 0;
}

inline
static E8 RBTree_AddChild(RBTree *pTree, const Byte *pKey, U32 uKey
                                 , void *pContext, RBTree_Node **ppNode){
  RBTree_Node *pNew;
  RBTree_Node *pNode = RBTREE_ROOT(pTree);
  I8 iCompare;
  while(1){
    iCompare = RBTree_Node_Compare(pKey, uKey, pNode);
    if(iCompare < 0){
      if(pNode->pLeft)
        pNode = pNode->pLeft;
      else{
        pNode->pLeft = pNew = RBTree_Node_Allocate(pContext, pKey, uKey);
        break;
      }
    }
    else if(iCompare > 0){
      if(pNode->pRight)
        pNode = pNode->pRight;
      else{
        pNode->pRight = pNew = RBTree_Node_Allocate(pContext, pKey, uKey);
        break;
      }
    }
    else{
      *ppNode = pNode;
      return 1;
    }
  }

  RBTREE_NODE_SET_PARENT(pNew, pNode);
  pNew->pLeft = 0;
  pNew->pRight = 0;
  pNew->bRed = 1;

  RBTree_Balance(pTree, pNew);
  *ppNode = pNew;
  return 0;
}

//Not implement yet
inline
static void RBTree_Remove(RBTree *pTree, RBTree_Node *pNode){
}

inline
static void RBTree_Balance(RBTree *pTree, RBTree_Node *pNode){
  RBTree_Node *pGreatGrandparent;
  RBTree_Node *pGrandparent;
  RBTree_Node *pUncle;
  RBTree_Node *pParent;
  RBTree_Node *pLeft;
  RBTree_Node *pRight;
  while((pParent = RBTREE_NODE_PARENT(pNode))->bRed){
    pGrandparent = RBTREE_NODE_PARENT(pParent);
    pGreatGrandparent = pGrandparent->pParent;
    if(pParent == pGrandparent->pLeft){
      pUncle = pGrandparent->pRight;
      if(pUncle && pUncle->bRed){
        pUncle->bRed = 0;
        if(pGreatGrandparent)
          pNode = pGrandparent;
        else{
          pParent->bRed = 0;
          break;
        }
      }
      else{
        RBTree_Node  *pParentRight = pParent->pRight;
        if(pNode == pParentRight){
          if(pGreatGrandparent){
            if(pGrandparent == pGreatGrandparent->pLeft)
              pGreatGrandparent->pLeft = pNode;
            else
              pGreatGrandparent->pRight = pNode;
          }
          else
            RBTREE_SET_ROOT(pTree, pNode);

          RBTREE_NODE_SET_PARENT(pGrandparent, pNode);
          pRight = pNode->pRight;
          pGrandparent->pLeft = pRight;
          if(pRight)
            RBTREE_NODE_SET_PARENT(pRight, pGrandparent);

          RBTREE_NODE_SET_PARENT(pParent, pNode);
          pLeft = pNode->pLeft;
          pParent->pRight = pLeft;
          if(pLeft)
            RBTREE_NODE_SET_PARENT(pLeft, pParent);

          pNode->bRed = 0;
          RBTREE_NODE_SET_PARENT(pNode, pGreatGrandparent);
          pNode->pLeft = pParent;
          pNode->pRight = pGrandparent;
        }
        else{
          RBTREE_NODE_SET_PARENT(pGrandparent, pParent);
          pGrandparent->pLeft = pParentRight;

          if(pParentRight)
            RBTREE_NODE_SET_PARENT(pParentRight, pGrandparent);

          RBTREE_NODE_SET_PARENT(pParent, pGreatGrandparent);
          pParent->pRight = pGrandparent;

          if(pGreatGrandparent){
            if(pGrandparent == pGreatGrandparent->pLeft)
              pGreatGrandparent->pLeft = pParent;
            else
              pGreatGrandparent->pRight = pParent;
          }
          else
            RBTREE_SET_ROOT(pTree, pParent);

          pParent->bRed = 0;
        }
        pGrandparent->bRed = 1;
        break;
      }
    }
    else{
      pUncle = pGrandparent->pLeft;
      if(pUncle && pUncle->bRed){
        pUncle->bRed = 0;
        if(pGreatGrandparent)
          pNode = pGrandparent;
        else{
          pParent->bRed = 0;
          break;
        }
      }
      else{
        RBTree_Node  *pParentLeft = pParent->pLeft;
        if(pNode == pParentLeft){
          if(pGreatGrandparent){
            if(pGrandparent == pGreatGrandparent->pLeft)
              pGreatGrandparent->pLeft = pNode;
            else
              pGreatGrandparent->pRight = pNode;
          }
          else
            RBTREE_SET_ROOT(pTree, pNode);

          RBTREE_NODE_SET_PARENT(pGrandparent, pNode);
          pLeft = pNode->pLeft;
          pGrandparent->pRight = pLeft;
          if(pLeft)
            RBTREE_NODE_SET_PARENT(pLeft, pGrandparent);

          RBTREE_NODE_SET_PARENT(pParent, pNode);
          pRight = pNode->pRight;
          pParent->pLeft = pRight;
          if(pRight)
            RBTREE_NODE_SET_PARENT(pRight, pParent);

          pNode->bRed = 0;
          RBTREE_NODE_SET_PARENT(pNode, pGreatGrandparent);
          pNode->pLeft = pGrandparent;
          pNode->pRight = pParent;

        }
        else{
          RBTREE_NODE_SET_PARENT(pGrandparent, pParent);
          pGrandparent->pRight = pParentLeft;

          if(pParentLeft)
            RBTREE_NODE_SET_PARENT(pParentLeft, pGrandparent);

          RBTREE_NODE_SET_PARENT(pParent, pGreatGrandparent);
          pParent->pLeft = pGrandparent;

          if(pGreatGrandparent){
            if(pGrandparent == pGreatGrandparent->pRight)
              pGreatGrandparent->pRight = pParent;
            else
              pGreatGrandparent->pLeft = pParent;
          }
          else
            RBTREE_SET_ROOT(pTree, pParent);

          pParent->bRed = 0;
        }
        pGrandparent->bRed = 1;
        break;
      }
    }
    pParent->bRed = 0;
    pGrandparent->bRed = 1;
  }
}

#endif // RBTREE_H

