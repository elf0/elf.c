#ifndef HASHSET_H
#define HASHSET_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

#include "RBTree.h"
#include "HashSet_Node.h"
//API
typedef struct HashSet  HashSet;

//User must implement "HashSet_Node_Allocate()" and "HashSet_Node_Compare()"
inline
static HashSet_Node *HashSet_Node_Allocate(void *pContext, const Byte *pKey, U32 uKey);
inline
static void HashSet_Node_Free(void *pContext, HashSet_Node *pNode);
inline
static I8 HashSet_Node_Compare(const Byte *pLeft, U32 uLeft, const HashSet_Node *pNode);

inline
static void HashSet_Initialize(HashSet *pSet);
inline
static void HashSet_Finalize(HashSet *pSet);
inline
static HashSet_Node *HashSet_Find(HashSet *pSet, const Byte *pKey, U32 uKey);
inline
static E8 HashSet_Add(HashSet *pSet, const Byte *pKey, U32 uKey
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

inline
static void HashSet_Initialize(HashSet *pSet){
  RBTree *pTree = pSet->trees;
  RBTree *ptEnd = pTree + SET_HASH_WIDTH;
  while(pTree != ptEnd)
    RBTree_Initialize(pTree++);

  pSet->uCount = 0;
}

inline
static void HashSet_Finalize(HashSet *pSet){
  RBTree *pTree = pSet->trees;
  RBTree *ptEnd = pTree + SET_HASH_WIDTH;
  while(pTree != ptEnd)
    RBTree_Finalize(pTree++);

  pSet->uCount = 0;
}

inline
static HashSet_Node *HashSet_Find(HashSet *pSet, const Byte *pKey, U32 uKey){
  return (HashSet_Node*)RBTree_Find(&pSet->trees[SET_HASH_INDEX(uKey)], pKey, uKey);
}

inline
static E8 HashSet_Add(HashSet *pSet, const Byte *pKey, U32 uKey
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
inline
static void HashSet_Remove(HashSet *pSet, U32 uKey, HashSet_Node *pNode){
  RBTree *pTree = &pSet->trees[SET_HASH_INDEX(uKey)];
  RBTree_Remove(pTree, (RBTree_Node*)pNode);
  --pSet->uCount;
}

inline
static RBTree_Node *RBTree_Node_Allocate(void *pContext, const Byte *pKey, U32 uKey){
  return (RBTree_Node*)HashSet_Node_Allocate(pContext, pKey, uKey);
}

inline
static void RBTree_Node_Free(void *pContext, RBTree_Node *pNode){
  HashSet_Node_Free(pContext, (HashSet_Node*)pNode);
}

inline
static I8 RBTree_Node_Compare(const Byte *pKey, U32 uKey, const RBTree_Node *pNode){
  return HashSet_Node_Compare(pKey, uKey, (HashSet_Node*)pNode);
}

#endif // HASHSET_H
