#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "Type.h"
#include "List.h"
#include "Thread.h"
#include "ThreadCondition.h"

typedef struct structTask Task;
typedef Bool (*TaskEntry)(Task *pTask);
typedef void (*TaskFinalize)(Task *pTask);

struct structTask{
 DoubleNode node;
 TaskEntry Entry;
 TaskFinalize Finalize;
};

static inline void Task_Initialize(Task *pTask, TaskEntry Entry, TaskFinalize Finalize){
 pTask->Entry = Entry;
 pTask->Finalize = Finalize;
}


typedef struct{
 Thread thread;
 ThreadLock lock;
 ThreadCondition condition;
 List tasks;
 List pendingTasks;
}WorkerThread;

static void *WorkerThread_Entry(WorkerThread *pThread);

static inline void WorkerThread_Initialize(WorkerThread *pThread){
 Thread_Initialize((Thread*)pThread, (ThreadEntry)WorkerThread_Entry);
 ThreadLock_Initialize(&pThread->lock);
 ThreadCondition_Initialize(&pThread->condition);
 List_Initialize(&pThread->tasks);
 List_Initialize(&pThread->pendingTasks);
}

static inline void WorkerThread_Finalize(WorkerThread *pThread){
 ThreadCondition_Finalize(&pThread->condition);
 ThreadLock_Finalize(&pThread->lock);
}

static inline Bool WorkerThread_Run(WorkerThread *pThread){
 return Thread_Run((Thread*)pThread);
}

static inline void WorkerThread_Post(WorkerThread *pThread, Task *pTask){
 ThreadLock_Lock(&pThread->lock);
 List_Push(&pThread->pendingTasks, (DoubleNode*)pTask);
 ThreadLock_Unlock(&pThread->lock);

 ThreadCondition_Signal(&pThread->condition);
}

//Do not post Empty task list
static inline void WorkerThread_PostTasks(WorkerThread *pThread, List *pTasks){
 ThreadLock_Lock(&pThread->lock);
 List_MoveTo(pTasks, &pThread->pendingTasks);
 ThreadLock_Unlock(&pThread->lock);

 ThreadCondition_Signal(&pThread->condition);
}

//Internal
static inline Bool WorkerThread_HaveTasks(List *pTasks){
 return List_NotEmpty(pTasks);
}

static inline Bool WorkerThread_NoTasks(List *pTasks){
 return List_Empty(pTasks);
}

static inline void WorkerThread_MoveTasks(List *pPendingTasks, List *pTasks){
 List_MoveTo(pPendingTasks, pTasks);
}

static inline void WorkerThread_WaitTasks(WorkerThread *pThread){
  List *pTasks = &pThread->tasks;
  List *pPendingTasks = &pThread->pendingTasks;

  ThreadLock_Lock(&pThread->lock);

  if(WorkerThread_HaveTasks(pPendingTasks))
   WorkerThread_MoveTasks(pPendingTasks, pTasks);

  if(WorkerThread_NoTasks(pTasks)){
   ThreadCondition_Wait(&pThread->condition, &pThread->lock);
   WorkerThread_MoveTasks(pPendingTasks, pTasks);
  }

  ThreadLock_Unlock(&pThread->lock);
}

static inline void WorkerThread_RunTasks(WorkerThread *pThread){
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
  WorkerThread_WaitTasks(pThread);
  WorkerThread_RunTasks(pThread);
 }
 return null;
}
#endif //WORKERTHREAD_H

