#pragma once
//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include <pthread.h>

#include "Type.h"
#include "List.h"
#include "ThreadCondition.h"

typedef struct structTask Task;
typedef Bool (*TaskEntry)(Task *pTask);
typedef void (*TaskFinalize)(Task *pTask);

struct structTask{
 ListNode node;
 TaskEntry Entry;
 TaskFinalize Finalize;
};

static inline void Task_Initialize(Task *pTask, TaskEntry Entry, TaskFinalize Finalize){
 pTask->Entry = Entry;
 pTask->Finalize = Finalize;
}


typedef struct{
 pthread_t thread;
 ThreadLock lock;
 ThreadCondition condition;
 ListNode tasks;
 ListNode pendingTasks;
}WorkerThread;

static inline void WorkerThread_Initialize(WorkerThread *pThread){
 ThreadLock_Initialize(&pThread->lock);
 ThreadCondition_Initialize(&pThread->condition);
 ListNode_Reset(&pThread->tasks);
 ListNode_Reset(&pThread->pendingTasks);
}

static inline void WorkerThread_Finalize(WorkerThread *pThread){
 ThreadCondition_Finalize(&pThread->condition);
 ThreadLock_Finalize(&pThread->lock);
}

Bool WorkerThread_Run(WorkerThread *pThread);

static inline void WorkerThread_Post(WorkerThread *pThread, Task *pTask){
 ThreadLock_Lock(&pThread->lock);
 ListNode_Link(&pTask->node, pThread->pendingTasks.pPrev, &pThread->pendingTasks);
 ThreadLock_Unlock(&pThread->lock);

 ThreadCondition_Signal(&pThread->condition);
}

//Do not post Empty task list
static inline void WorkerThread_PostTasks(WorkerThread *pThread, ListNode *pTasks){
 ThreadLock_Lock(&pThread->lock);
 ListNode_MoveBuddiesTo(pTasks, &pThread->pendingTasks);
 ThreadLock_Unlock(&pThread->lock);

 ThreadCondition_Signal(&pThread->condition);
}
