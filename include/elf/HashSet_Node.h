#ifndef HASHSET_NODE_H
#define HASHSET_NODE_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

#include "RBTree_Node.h"

typedef struct HashSet_Node HashSet_Node;

struct HashSet_Node{
  RBTree_Node node;
};

#endif // HASHSET_NODE_H
