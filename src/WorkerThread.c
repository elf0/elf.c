//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "elf/WorkerThread.h"

static void *WorkerThread_Entry(WorkerThread *pThread);

void WorkerThread_Initialize(WorkerThread *pThread){
 Thread_Initialize((Thread*)pThread, (ThreadEntry)WorkerThread_Entry);
 ThreadLock_Initialize(&pThread->lock);
 ThreadCondition_Initialize(&pThread->condition);
 List_Initialize(&pThread->tasks);
 List_Initialize(&pThread->pendingTasks);
}

void WorkerThread_Finalize(WorkerThread *pThread){
 ThreadCondition_Finalize(&pThread->condition);
 ThreadLock_Finalize(&pThread->lock);
}

Bool WorkerThread_Run(WorkerThread *pThread){
 return Thread_Run((Thread*)pThread);
}

static inline Bool HaveTasks(List *pTasks){
 return List_NotEmpty(pTasks);
}

static inline Bool NoTasks(List *pTasks){
 return List_Empty(pTasks);
}

static inline void MoveTasks(List *pPendingTasks, List *pTasks){
 List_MoveTo(pPendingTasks, pTasks);
}

static inline void WaitTasks(WorkerThread *pThread){
  List *pTasks = &pThread->tasks;
  List *pPendingTasks = &pThread->pendingTasks;

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
 DoubleNode *pEntry = (DoubleNode*)&pThread->tasks;
 DoubleNode *pNode = pEntry->pNext;
 Task *pTask;
 while(pNode != pEntry){
  pTask = (Task*)pNode;
  if(pTask->Entry(pTask)){
   List_Remove(pNode);
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
