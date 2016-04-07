#ifndef QUEUE_H
#define QUEUE_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

//Queue

typedef struct QueueNode{
 struct QueueNode *pNext;
}QueueNode;

typedef struct Queue{
 QueueNode *pFirst;
 QueueNode *pLast;
}Queue;


static inline void Queue_Initialize(Queue *pQueue){
 pQueue->pFirst = (QueueNode*)pQueue;
 pQueue->pLast = (QueueNode*)pQueue;
}

static inline Bool Queue_Empty(Queue *pQueue){
 return pQueue->pFirst == (QueueNode*)pQueue;
}

static inline Bool Queue_NotEmpty(Queue *pQueue){
 return pQueue->pFirst != (QueueNode*)pQueue;
}

static inline void Queue_Push(Queue *pQueue, QueueNode *pNode){
 pNode->pNext = (QueueNode*)pQueue;
 pQueue->pLast->pNext = pNode;
 pQueue->pLast = pNode;
}

static inline QueueNode *Queue_Pop(Queue *pQueue){
 QueueNode *pNode = pQueue->pFirst;
 pQueue->pFirst = pNode->pNext;
 if(pQueue->pFirst == (QueueNode*)pQueue)
   pQueue->pLast = (QueueNode*)pQueue;
 return pNode;
}

#endif //QUEUE_H
