#ifndef RBTREE_H
#define RBTREE_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

//API
typedef struct RBTree  RBTree;
typedef struct RBTree_Node RBTree_Node;

typedef I8 (*RBTree_Compare_Function)(RBTree_Node *pLeft, RBTree_Node *pRight);

static inline void RBTree_Initialize(RBTree *pTree, RBTree_Compare_Function fCompare);
static inline RBTree_Node *RBTree_Find(RBTree *pTree, RBTree_Node *pKey);
static inline B RBTree_Insert(RBTree *pTree, RBTree_Node *pNew);

//Internal
struct RBTree_Node{
  //pParent must be first
  RBTree_Node *pParent;
  RBTree_Node *pLeft;
  RBTree_Node *pRight;
  B bRed;
};

#define RBTREE_ROOT(pTree) (*(RBTree_Node**)pTree)
#define RBTREE_SET_ROOT(pTree, pNode) (RBTREE_ROOT(pTree) = pNode)

#define RBTREE_NODE_PARENT(pNode) (*(RBTree_Node**)pNode)
#define RBTREE_NODE_SET_PARENT(pNode, pParent) (RBTREE_NODE_PARENT(pNode) = pParent)

typedef E8 (*RBTree_Insert_Function) (RBTree *pTree, RBTree_Node *pNode);

static inline E8 RBTree_InsertRoot(RBTree *pTree, RBTree_Node *pNew);
static inline E8 RBTree_InsertChild(RBTree *pTree, RBTree_Node *pNew);
static inline void RBTree_Balance(RBTree *pTree, RBTree_Node *pNode);
static inline void RBTree_LeftRotate(RBTree *pTree, RBTree_Node *pNode);
static inline void RBTree_RightRotate(RBTree *pTree, RBTree_Node *pNode);

struct RBTree{
  //pRoot must be first
  RBTree_Node     *pRoot;
  RBTree_Insert_Function   fInsert;
  RBTree_Compare_Function fCompare;
  U32 uCount;
};

static inline void RBTree_Initialize(RBTree *pTree, RBTree_Compare_Function fCompare){
  pTree->pRoot = 0;
  pTree->fInsert = RBTree_InsertRoot;
  pTree->fCompare = fCompare;
  pTree->uCount = 0;
}

static inline RBTree_Node *RBTree_Find(RBTree *pTree, RBTree_Node *pKey){
  RBTree_Node *pNode = RBTREE_ROOT(pTree);
  I8 iCompare;
  while(pNode){
    iCompare = pTree->fCompare(pKey, pNode);
    if(iCompare < 0)
      pNode = pNode->pLeft;
    else if(iCompare > 0)
      pNode = pNode->pRight;
    else
      return pNode;
  }

  return 0;
}

static inline E8 RBTree_Insert(RBTree *pTree, RBTree_Node *pNew){
  return pTree->fInsert(pTree, pNew);
}

static inline E8 RBTree_InsertRoot(RBTree *pTree, RBTree_Node *pNew){
  pNew->pLeft = 0;
  pNew->pRight = 0;
  RBTREE_NODE_SET_PARENT(pNew, 0);
  pNew->bRed = 0;

  RBTREE_SET_ROOT(pTree, pNew);
  pTree->fInsert = RBTree_InsertChild;
  pTree->uCount = 1;
  return 0;
}

static inline E8 RBTree_InsertChild(RBTree *pTree, RBTree_Node *pNew){
  RBTree_Node *pNode = RBTREE_ROOT(pTree);
  RBTree_Compare_Function Compare = pTree->fCompare;
  I8 iCompare;
  while(1){
    iCompare = Compare(pNew, pNode);
    if(iCompare < 0){
      if(pNode->pLeft)
        pNode = pNode->pLeft;
      else{
        pNode->pLeft = pNew;
        break;
      }
    }
    else if(iCompare > 0){
      if(pNode->pRight)
        pNode = pNode->pRight;
      else{
        pNode->pRight = pNew;
        break;
      }
    }
    else
      return 1;
  }

  RBTREE_NODE_SET_PARENT(pNew, pNode);
  pNew->pLeft = 0;
  pNew->pRight = 0;
  pNew->bRed = 1;

  RBTree_Balance(pTree, pNew);
  ++pTree->uCount;
  return 0;
}

//Not implement yet
void RBTree_Delete(RBTree *pTree, RBTree_Node *pNode);

static inline void RBTree_Balance(RBTree *pTree, RBTree_Node *pNode){
  RBTree_Node *pGreatGrandparent;
  RBTree_Node *pGrandparent;
  RBTree_Node *pUncle;
  RBTree_Node *pParent;
  RBTree_Node *pLeft;
  RBTree_Node *pRight;
  while((pParent = RBTREE_NODE_PARENT(pNode))->bRed){
    pGrandparent = RBTREE_NODE_PARENT(pParent);
    if(pParent == pGrandparent->pLeft){
      pUncle = pGrandparent->pRight;
      if(pUncle && pUncle->bRed){
        pUncle->bRed = 0;
        if(pGrandparent == RBTREE_ROOT(pTree)){
          pParent->bRed = 0;
          break;
        }
        pNode = pGrandparent;
      }
      else{
        if(pNode == pParent->pRight){
          //Set pGreatGrandparent
          pGreatGrandparent = pGrandparent->pParent;
          if(pGrandparent == RBTREE_ROOT(pTree))
            RBTREE_SET_ROOT(pTree, pNode);
          else if(pGrandparent == pGreatGrandparent->pLeft)
            pGreatGrandparent->pLeft = pNode;
          else
            pGreatGrandparent->pRight = pNode;

          //Set pGrandparent
          pGrandparent->bRed = 1;
          RBTREE_NODE_SET_PARENT(pGrandparent, pNode);
          pRight = pNode->pRight;
          pGrandparent->pLeft = pRight;
          if(pRight)
            RBTREE_NODE_SET_PARENT(pRight, pGrandparent);

          //Set pParent
          RBTREE_NODE_SET_PARENT(pParent, pNode);
          pLeft = pNode->pLeft;
          pParent->pRight = pLeft;
          if(pLeft)
            RBTREE_NODE_SET_PARENT(pLeft, pParent);

          //Set pNode
          pNode->bRed = 0;
          RBTREE_NODE_SET_PARENT(pNode, pGreatGrandparent);
          pNode->pLeft = pParent;
          pNode->pRight = pGrandparent;

          break;
        }
        RBTree_RightRotate(pTree, pGrandparent);
      }
    }
    else{
      pUncle = pGrandparent->pLeft;
      if(pUncle && pUncle->bRed){
        pUncle->bRed = 0;
        if(pGrandparent == RBTREE_ROOT(pTree)){
          pParent->bRed = 0;
          break;
        }
        pNode = pGrandparent;
      }
      else{
        if(pNode == pParent->pLeft){
          //Set pGreatGrandparent
          pGreatGrandparent = pGrandparent->pParent;
          if(pGrandparent == RBTREE_ROOT(pTree))
            RBTREE_SET_ROOT(pTree, pNode);
          else if(pGrandparent == pGreatGrandparent->pLeft)
            pGreatGrandparent->pLeft = pNode;
          else
            pGreatGrandparent->pRight = pNode;

          //Set pGrandparent
          pGrandparent->bRed = 1;
          RBTREE_NODE_SET_PARENT(pGrandparent, pNode);
          pLeft = pNode->pLeft;
          pGrandparent->pRight = pLeft;
          if(pLeft)
            RBTREE_NODE_SET_PARENT(pLeft, pGrandparent);

          //Set pParent
          RBTREE_NODE_SET_PARENT(pParent, pNode);
          pRight = pNode->pRight;
          pParent->pLeft = pRight;
          if(pRight)
            RBTREE_NODE_SET_PARENT(pRight, pParent);

          //Set pNode
          pNode->bRed = 0;
          RBTREE_NODE_SET_PARENT(pNode, pGreatGrandparent);
          pNode->pLeft = pGrandparent;
          pNode->pRight = pParent;

          break;
        }
        RBTree_LeftRotate(pTree, pGrandparent);
      }
    }
    pParent->bRed = 0;
    pGrandparent->bRed = 1;
  }
}

static inline void RBTree_LeftRotate(RBTree *pTree, RBTree_Node *pNode){
  RBTree_Node  *pParent = RBTREE_NODE_PARENT(pNode);
  RBTree_Node  *pRight = pNode->pRight;
  RBTree_Node  *pRightLeft = pRight->pLeft;

  RBTREE_NODE_SET_PARENT(pNode, pRight);
  pNode->pRight = pRightLeft;

  RBTREE_NODE_SET_PARENT(pRight, pParent);
  pRight->pLeft = pNode;

  if(pRightLeft)
    RBTREE_NODE_SET_PARENT(pRightLeft, pNode);

  if(pNode == RBTREE_ROOT(pTree))
    RBTREE_SET_ROOT(pTree, pRight);
  else if(pNode == pParent->pLeft)
    pParent->pLeft = pRight;
  else
    pParent->pRight = pRight;
}

static inline void RBTree_RightRotate(RBTree *pTree, RBTree_Node *pNode){
  RBTree_Node  *pParent = RBTREE_NODE_PARENT(pNode);
  RBTree_Node  *pLeft= pNode->pLeft;
  RBTree_Node  *pLeftRight = pLeft->pRight;

  RBTREE_NODE_SET_PARENT(pNode, pLeft);
  pNode->pLeft = pLeftRight;

  RBTREE_NODE_SET_PARENT(pLeft, pParent);
  pLeft->pRight = pNode;

  if(pLeftRight)
    RBTREE_NODE_SET_PARENT(pLeftRight, pNode);

  if(pNode == RBTREE_ROOT(pTree))
    RBTREE_SET_ROOT(pTree, pLeft);
  else if(pNode == pParent->pRight)
    pParent->pRight = pLeft;
  else
    pParent->pLeft = pLeft;
}
#endif // RBTREE_H

