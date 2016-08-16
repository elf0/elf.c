#ifndef HASHSET_H
#define HASHSET_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

#include "RBTree.h"
#include "HashSet_Node.h"
//API
typedef struct HashSet  HashSet;

//User must implement "HashSet_Allocate()" and "HashSet_Compare()"
static inline HashSet_Node *HashSet_Allocate(void *pContext, const Byte *pKey, U32 uKey);
static inline I8 HashSet_Compare(const Byte *pLeft, U32 uLeft, const HashSet_Node *pNode);

static inline void HashSet_Initialize(HashSet *pSet);
static inline HashSet_Node *HashSet_Find(HashSet *pSet, const Byte *pKey, U32 uKey);
static inline E8 HashSet_Add(HashSet *pSet, const Byte *pKey, U32 uKey
                             , void *pContext, HashSet_Node **ppNode);

//Internal

//SET_HASH_WIDTH must be 2, 4, 8, 16 , 32, ..
#ifndef SET_HASH_WIDTH
#define SET_HASH_WIDTH 16
#endif

#define SET_HASH_INDEX(size) (size & (SET_HASH_WIDTH - 1))

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

static inline HashSet_Node *HashSet_Find(HashSet *pSet, const Byte *pKey, U32 uKey){
  return (HashSet_Node*)RBTree_Find(&pSet->trees[SET_HASH_INDEX(uKey)], pKey, uKey);
}

static inline E8 HashSet_Add(HashSet *pSet, const Byte *pKey, U32 uKey
                             , void *pContext, HashSet_Node **ppNode){
  RBTree *pTree = &pSet->trees[SET_HASH_INDEX(uKey)];
  E8 e = pTree->fAdd(pTree, pKey, uKey
                     , pContext, (RBTree_Node**)ppNode);
  if(e)
    return e;

  ++pSet->uCount;
  return 0;
}

//RBTree_Remove not implement yet
static inline void HashSet_Remove(HashSet *pSet, U32 uKey, HashSet_Node *pNode){
  RBTree *pTree = &pSet->trees[SET_HASH_INDEX(uKey)];
  RBTree_Remove(pTree, (RBTree_Node*)pNode);
  --pSet->uCount;
}

static inline RBTree_Node *RBTree_Allocate(void *pContext, const Byte *pKey, U32 uKey){
  return (RBTree_Node*)HashSet_Allocate(pContext, pKey, uKey);
}

static inline I8 RBTree_Compare(const Byte *pKey, U32 uKey, const RBTree_Node *pNode){
  return HashSet_Compare(pKey, uKey, (HashSet_Node*)pNode);
}

#endif // HASHSET_H
