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
static void RBTree_Finalize(RBTree *pTree, void *pContext);
inline
static RBTree_Node *RBTree_Find(RBTree *pTree, const Byte *pKey, U32 uKey);
inline
static B RBTree_Add(RBTree *pTree, const Byte *pKey, U32 uKey, void *pContext, RBTree_Node **ppNode);

//Internal

typedef E8 (*RBTree_Add_f) (RBTree *pTree, const Byte *pKey, U32 uKey, void *pContext, RBTree_Node **ppNode);

inline
static E8 RBTree_AddRoot(RBTree *pTree, const Byte *pKey, U32 uKey, void *pContext, RBTree_Node **ppNode);
inline
static E8 RBTree_AddChild(RBTree *pTree, const Byte *pKey, U32 uKey, void *pContext, RBTree_Node **ppNode);
inline
static void RBTree_Balance(RBTree *pTree, RBTree_Node *pNode);

struct RBTree {
  RBTree_Node leaf;
  RBTree_Add_f   fAdd;
};

inline
static void RBTree_Initialize(RBTree *pTree) {
  RBTree_Node *pLeaf = &pTree->leaf;
  pLeaf->pParent = pLeaf->pLeft = pLeaf->pRight = pLeaf;
  pLeaf->bRed = 0;
  pTree->fAdd = RBTree_AddRoot;
}

inline
static void RBTree_Free(RBTree_Node *pNode, void *pContext) {
  if(pNode->pLeft)
    RBTree_Free(pNode->pLeft, pContext);

  if(pNode->pRight)
    RBTree_Free(pNode->pRight, pContext);

  RBTree_Node_Free(pContext, pNode);
}

inline
static void RBTree_Finalize(RBTree *pTree, void *pContext) {
  RBTree_Node *pNode = RBTREE_ROOT(pTree);
  if(pNode) {
    RBTree_Free(pNode, pContext);
    RBTREE_SET_ROOT(pTree, 0);
  }
}

inline
static RBTree_Node *RBTree_Find(RBTree *pTree, const Byte *pKey, U32 uKey) {
  RBTree_Node *pNode = pTree->leaf.pParent;
  while (pNode) {
    I8 iResult = RBTree_Node_Compare(pKey, uKey, pNode);
    if (iResult < 0)
      pNode = pNode->pLeft;
    else if (iResult)
      pNode = pNode->pRight;
    else
      return pNode;
  }
  return 0;
}

inline
static E8 RBTree_Add(RBTree *pTree, const Byte *pKey, U32 uKey
                     , void *pContext, RBTree_Node **ppNode) {
  return pTree->fAdd(pTree, pKey, uKey, pContext, ppNode);
}

inline
static E8 RBTree_AddRoot(RBTree *pTree, const Byte *pKey, U32 uKey
                         , void *pContext, RBTree_Node **ppNode) {
  RBTree_Node *pLeaf = &pTree->leaf;
  RBTree_Node *pNew = RBTree_Node_Allocate(pContext, pKey, uKey);
  pNew->pParent = pLeaf;
  pNew->pLeft = pLeaf;
  pNew->pRight = pLeaf;
  pNew->bRed = 0;
  pLeaf->pParent = pNew;
  *ppNode = pNew;
  pTree->fAdd = RBTree_AddChild;
  return 0;
}

inline
static E8 RBTree_AddChild(RBTree *pTree, const Byte *pKey, U32 uKey
                          , void *pContext, RBTree_Node **ppNode) {
  //  Find inserted node and slot
  RBTree_Node *pLeaf = &pTree->leaf;
  RBTree_Node **ppSlot = &pLeaf->pParent;
  RBTree_Node *pNode = *ppSlot;
  while (1) {
    I8 iResult = RBTree_Node_Compare(pKey, uKey, pNode);
    if (iResult < 0) {
      RBTree_Node *pLeft = pNode->pLeft;
      if (pLeft != pLeaf)
        pNode = pLeft;
      else {
        ppSlot = &pNode->pLeft;
        break;
      }
    }
    else if (iResult) {
      RBTree_Node *pRight = pNode->pRight;
      if (pRight != pLeaf)
        pNode = pRight;
      else {
        ppSlot = &pNode->pRight;
        break;
      }
    }
    else {
      *ppNode = pNode;
      return 1;
    }
  }

  RBTree_Node *pNew =  RBTree_Node_Allocate(pContext, pKey, uKey);
  pNew->pParent = pNode;
  pNew->pLeft = pLeaf;
  pNew->pRight = pLeaf;
  pNew->bRed = 1;
  *ppNode = pNew;
  *ppSlot = pNew;

  // parent(black) -> node(red) -> child(red)
  RBTree_Balance(pTree, pNode, pNew);
  return 0;
}

//Not implement yet
inline
static E8 RBTree_Remove(RBTree *pTree, RBTree_Node *pNode) {
  RBTree_Node *pParent = RBTREE_NODE_PARENT(pNode);
  if (pNode->bRed) {
    if (!pNode->pLeft && !pNode->pRight) {
      if (pNode == pParent->pLeft)
        pParent->pLeft = 0;
      else
        pParent->pRight = 0;

      RBTree_Node_Free(0, pNode);
      return 0;
    }
  }
  else {
    if (!pNode->pLeft && !pNode->pRight) {
      if (pParent->bRed) {
        pParent->bRed = 0;
        if (pNode == pParent->pLeft) {
          pParent->pLeft = 0;
          pParent->pRight->bRed = 1;
        }
        else {
          pParent->pRight = 0;
          pParent->pLeft->bRed = 1;
        }
        RBTree_Node_Free(0, pNode);
        return 0;
      }
      else {
        if (pNode == pParent->pLeft) {
          pParent->pLeft = 0;
          RBTree_Node *pSibling = pParent->pRight;
          if (pSibling->bRed) {
            return 1;
          }
          else
            pSibling->bRed = 1;
        }
        else {
          pParent->pRight = 0;
          RBTree_Node *pSibling = pParent->pLeft;
          if (pSibling->bRed) {
            return 1;
          }
          else
            pSibling->bRed = 1;
        }
        RBTree_Node_Free(0, pNode);
        return 0;
      }

      //      if (pParent->pLeft == pNode) {
      //        pParent->pLeft = 0;
      //        if (pParent->pRight)
      //          pParent->pRight->bRed = 1;
      //      }
      //      else {
      //        pParent->pRight = 0;
      //        if (pParent->pLeft)
      //          pParent->pLeft->bRed = 1;
      //      }
    }
  }
  return 1;
}

inline
static void RBTree_Balance(RBTree *pTree, RBTree_Node *pNode, RBTree_Node *pChild) {
  RBTree_Node *pLeaf = &pTree->leaf;
  while (pNode->bRed) {
    RBTree_Node *pParent = pNode->pParent;
    RBTree_Node *pGrandparent = pParent->pParent;
    RBTree_Node *pSibling;
    if (pNode == pParent->pLeft) {
      pSibling = pParent->pRight;
      if (!pSibling->bRed) {
        RBTree_Node *pNodeRight = pNode->pRight;
        if (pChild == pNodeRight) {
          pNode->pParent = pChild;
          RBTree_Node *pChildLeft = pChild->pLeft;
          pNode->pRight = pChildLeft;

          if (pChildLeft != pLeaf)
            pChildLeft->pParent = pNode;

          pChild->pLeft = pNode;

          pNode = pChild;
          pNodeRight = pChild->pRight;
        }
        if (pNodeRight != pLeaf)
          pNodeRight->pParent = pParent;

        pNode->pParent = pGrandparent;
        pNode->pRight = pParent;
        pNode->bRed = 0;

        pParent->pParent = pNode;
        pParent->pLeft = pNodeRight;
        pParent->bRed = 1;

        if (pGrandparent != pLeaf) {
          if (pParent == pGrandparent->pLeft)
            pGrandparent->pLeft = pNode;
          else
            pGrandparent->pRight = pNode;
        }
        else
          pLeaf->pParent = pNode;

        break;
      }
    }
    else {
      pSibling = pParent->pLeft;
      if (!pSibling->bRed) {
        RBTree_Node *pNodeLeft = pNode->pLeft;
        if (pChild == pNodeLeft) {
          pNode->pParent = pChild;
          RBTree_Node *pChildRight = pChild->pRight;
          pNode->pLeft = pChildRight;

          if (pChildRight != pLeaf)
            pChildRight->pParent = pNode;

          pChild->pRight = pNode;

          pNode = pChild;
          pNodeLeft = pChild->pLeft;
        }
        if (pNodeLeft != pLeaf)
          pNodeLeft->pParent = pParent;

        pNode->pParent = pGrandparent;
        pNode->pLeft = pParent;
        pNode->bRed = 0;

        pParent->pParent = pNode;
        pParent->pRight = pNodeLeft;
        pParent->bRed = 1;

        if (pGrandparent != pLeaf) {
          if (pParent == pGrandparent->pLeft)
            pGrandparent->pLeft = pNode;
          else
            pGrandparent->pRight = pNode;
        }
        else
          pLeaf->pParent = pNode;

        break;
      }
    }
    pSibling->bRed = 0;
    pNode->bRed = 0;
    if (pGrandparent != pLeaf) {
      pChild = pParent;
      pChild->bRed = 1;
      pNode = pGrandparent;
    }
    else
      break;
  }
}


#endif // RBTREE_H

