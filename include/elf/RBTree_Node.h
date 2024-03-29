#ifndef RBTREE_NODE_H
#define RBTREE_NODE_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

#include "Type.h"

typedef struct RBTree_Node RBTree_Node;

struct RBTree_Node {
  RBTree_Node *pParent;
  RBTree_Node *pLeft;
  RBTree_Node *pRight;
  B bRed;
  int value;
};

#endif // RBTREE_NODE_H
