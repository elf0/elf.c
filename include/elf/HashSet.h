#ifndef HASHSET_H
#define HASHSET_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

#include "RBTree.h"

//API
typedef struct HashSet  HashSet;
typedef struct HashSet_Node HashSet_Node;

typedef I8 (*HashSet_FindCompare_f)(Byte *pKey, U32 uKey, HashSet_Node *pNode);
typedef I8 (*HashSet_InsertCompare_f)(HashSet_Node *pNew, HashSet_Node *pNode);

static inline void HashSet_Initialize(HashSet *pSet);
static inline HashSet_Node *HashSet_Find(HashSet *pSet, Byte *pKey, U32 uKey, HashSet_FindCompare_f fCompare);
static inline B HashSet_Insert(HashSet *pSet, HashSet_Node *pNew, HashSet_InsertCompare_f fCompare);

//Internal
struct HashSet_Node{
  RBTree_Node node;
  U32 uData;
  Byte szData[4];
};

//SET_HASH_WIDTH must be 2, 4, 8, 16 , 32, ..
#ifndef SET_HASH_WIDTH
#define SET_HASH_WIDTH 16
#endif

#define SET_HASH_INDEX(uData) (uData & (SET_HASH_WIDTH - 1))

struct HashSet{
  RBTree trees[SET_HASH_WIDTH];
  U32 uCount;
};

static inline void HashSet_Initialize(HashSet *pSet){
  RBTree *pTree = pSet->trees;
  RBTree *ptEnd = pTree + SET_HASH_WIDTH;
  while(pTree != ptEnd)
    RBTree_Initialize(pTree++);

  pSet->uCount = 0;
}

static inline HashSet_Node *HashSet_Find(HashSet *pSet, Byte *pKey, U32 uKey, HashSet_FindCompare_f fCompare){
  return (HashSet_Node*)RBTree_Find(&pSet->trees[SET_HASH_INDEX(uKey)], pKey, uKey, (RBTree_FindCompare_f)fCompare);
}

static inline E8 HashSet_Insert(HashSet *pSet, HashSet_Node *pNew, HashSet_InsertCompare_f fCompare){
  RBTree *pTree = &pSet->trees[SET_HASH_INDEX(pNew->uData)];
  E8 e = pTree->fAdd(pTree, (RBTree_Node*)pNew, (RBTree_AddCompare_f)fCompare);
  if(e)
    return e;

  ++pSet->uCount;
  return 0;
}

//RBTree_Remove not implement yet
static inline void HashSet_Remove(HashSet *pSet, HashSet_Node *pNode){
  RBTree *pTree = &pSet->trees[SET_HASH_INDEX(pNode->uData)];
  RBTree_Remove(pTree, (RBTree_Node*)pNode);
  --pSet->uCount;
}


#endif // HASHSET_H

