#ifndef STACK_H
#define STACK_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

//Stack

typedef struct StackNode{
 struct StackNode *pNext;
}StackNode;

typedef struct Stack{
 StackNode *pTop;
}Stack;


static inline void Stack_Initialize(Stack *pStack){
 pStack->pTop = (StackNode*)pStack;
}

static inline Bool Stack_Empty(Stack *pStack){
 return pStack->pTop == (StackNode*)pStack;
}

static inline Bool Stack_NotEmpty(Stack *pStack){
 return pStack->pTop != (StackNode*)pStack;
}

static inline StackNode *Stack_Top(Stack *pStack){
 return (StackNode*)pStack;
}

static inline void Stack_Push(Stack *pStack, StackNode *pNode){
 pNode->pNext = pStack->pTop;
 pStack->pTop = pNode;
}

static inline StackNode *Stack_Pop(Stack *pStack){
 StackNode *pNode = (StackNode*)pStack;
 pStack->pTop = pNode->pNext;
 return pNode;
}

#endif //STACK_H
