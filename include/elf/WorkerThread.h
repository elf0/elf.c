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

void WorkerThread_Initialize(WorkerThread *pThread);
void WorkerThread_Finalize(WorkerThread *pThread);
Bool WorkerThread_Run(WorkerThread *pThread);

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

#endif //WORKERTHREAD_H
