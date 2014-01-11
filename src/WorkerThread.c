//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "WorkerThread.h"

static inline Bool HaveTasks(ListNode *pEntry){
 return ListNode_NotAlone(pEntry);
}

static inline Bool NoTasks(ListNode *pEntry){
 return ListNode_Alone(pEntry);
}

static inline void MoveTasks(ListNode *pPendingTasks, ListNode *pTasks){
 ListNode_MoveBuddiesTo(pPendingTasks, pTasks);
}

static inline void WaitTasks(WorkerThread *pThread){
  ListNode *pTasks = &pThread->tasks;
  ListNode *pPendingTasks = &pThread->pendingTasks;

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
 ListNode *pEntry = &pThread->tasks;
 ListNode *pNode = pEntry->pNext;
 Task *pTask;
 while(pNode != pEntry){
  pTask = (Task*)pNode;
  if(pTask->Entry(pTask)){
   ListNode_Unlink(pNode);
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

Bool WorkerThread_Run(WorkerThread *pThread){
 if(pthread_create(&pThread->thread, null, (void*(*)(void*))WorkerThread_Entry, pThread) != 0)
  return false;
 return true;
}

