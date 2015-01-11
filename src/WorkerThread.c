//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "elf/WorkerThread.h"

static void *WorkerThread_Entry(WorkerThread *pThread);

void WorkerThread_Initialize(WorkerThread *pThread){
 Thread_Initialize((Thread*)pThread, (ThreadEntry)WorkerThread_Entry);
 ThreadLock_Initialize(&pThread->lock);
 ThreadCondition_Initialize(&pThread->condition);
 DoubleNode_Reset(&pThread->tasks);
 DoubleNode_Reset(&pThread->pendingTasks);
}

void WorkerThread_Finalize(WorkerThread *pThread){
 ThreadCondition_Finalize(&pThread->condition);
 ThreadLock_Finalize(&pThread->lock);
}

Bool WorkerThread_Run(WorkerThread *pThread){
 return Thread_Run((Thread*)pThread);
}

static inline Bool HaveTasks(DoubleNode *pEntry){
 return DoubleNode_NotAlone(pEntry);
}

static inline Bool NoTasks(DoubleNode *pEntry){
 return DoubleNode_Alone(pEntry);
}

static inline void MoveTasks(DoubleNode *pPendingTasks, DoubleNode *pTasks){
 DoubleNode_MoveBuddiesTo(pPendingTasks, pTasks);
}

static inline void WaitTasks(WorkerThread *pThread){
  DoubleNode *pTasks = &pThread->tasks;
  DoubleNode *pPendingTasks = &pThread->pendingTasks;

  ThreadLock_Lock(&pThread->lock);

  if(HaveTasks(pPendingTasks))
   MoveTasks(pPendingTasks, pTasks);

  if(NoTasks(pTasks)){
   ThreadCondition_Wait(&pThread->condition, &pThread->lock);
   MoveTasks(pPendingTasks, pTasks);
  }

  ThreadLock_Unlock(&pThread->lock);
}

static inline void RunTasks(WorkerThread *pThread){
 DoubleNode *pEntry = &pThread->tasks;
 DoubleNode *pNode = pEntry->pNext;
 Task *pTask;
 while(pNode != pEntry){
  pTask = (Task*)pNode;
  if(pTask->Entry(pTask)){
   DoubleNode_Unlink(pNode);
   pNode = pNode->pNext;
   pTask->Finalize(pTask);
  }
  else
   pNode = pNode->pNext;
 }
}

static void *WorkerThread_Entry(WorkerThread *pThread){
 while(true){
  WaitTasks(pThread);
  RunTasks(pThread);
 }
 return null;
}
