#ifndef FIXEDWORKERTHREAD_H
#define FIXEDWORKERTHREAD_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "Type.h"
#include "List.h"
#include "Thread.h"
#include "ThreadCondition.h"

#define MAX_RUNNING_TASKS 64

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
    Task *szRunnings[MAX_RUNNING_TASKS];
    Task **ppRunningsEnd;
    Task **ppRunningsMax;
    List lstWaitings;
}WorkerThread;

static void *FixedWorkerThread_Entry(WorkerThread *pThread);

static inline void FixedWorkerThread_Initialize(WorkerThread *pThread){
    Thread_Initialize((Thread*)pThread, (ThreadEntry)FixedWorkerThread_Entry);
    ThreadLock_Initialize(&pThread->lock);
    ThreadCondition_Initialize(&pThread->condition);
    pThread->ppRunningsEnd = pThread->szRunnings;
    pThread->ppRunningsMax = pThread->szRunnings + MAX_RUNNING_TASKS;
    List_Initialize(&pThread->lstWaitings);
}

static inline void FixedWorkerThread_Finalize(WorkerThread *pThread){
    ThreadCondition_Finalize(&pThread->condition);
    ThreadLock_Finalize(&pThread->lock);
}

static inline Bool FixedWorkerThread_Run(WorkerThread *pThread){
    return Thread_Run((Thread*)pThread);
}

static inline void FixedWorkerThread_Post(WorkerThread *pThread, Task *pTask){
    ThreadLock_Lock(&pThread->lock);
    List_Push(&pThread->lstWaitings, (DoubleNode*)pTask);
    ThreadLock_Unlock(&pThread->lock);

    ThreadCondition_Signal(&pThread->condition);
}

//Do not post Empty task list
static inline void FixedWorkerThread_PostTasks(WorkerThread *pThread, List *pTasks){
    ThreadLock_Lock(&pThread->lock);
    List_MoveTo(pTasks, &pThread->lstWaitings);
    ThreadLock_Unlock(&pThread->lock);

    ThreadCondition_Signal(&pThread->condition);
}

//Internal
static inline Bool FixedWorkerThread_HaveWaitings(List *pTasks){
    return List_NotEmpty(pTasks);
}

static inline Bool FixedWorkerThread_NoTasks(List *pTasks){
    return List_Empty(pTasks);
}

static inline void FixedWorkerThread_FillTasks(WorkerThread *pThread){
    Task **ppRunningsMax = pThread->ppRunningsMax;
    List *pWaitings = &pThread->lstWaitings;

    while(pThread->ppRunningsEnd != ppRunningsMax && FixedWorkerThread_HaveWaitings(pWaitings)){
        Task *pTask = (Task *)List_Pop(pWaitings);
        *pThread->ppRunningsEnd++ = pTask;
    }
}

static inline void FixedWorkerThread_WaitTasks(WorkerThread *pThread){
    ThreadLock_Lock(&pThread->lock);

    FixedWorkerThread_FillTasks(pThread);

    if(pThread->ppRunningsEnd == pThread->szRunnings){
        ThreadCondition_Wait(&pThread->condition, &pThread->lock);
        FixedWorkerThread_FillTasks(pThread);
    }

    ThreadLock_Unlock(&pThread->lock);
}

static inline void FixedWorkerThread_RunTasks(WorkerThread *pThread){
    Task **ppTask = pThread->szRunnings;
    Task **ppEnd = pThread->ppRunningsEnd;
    while(ppTask != ppEnd){
        Task *pTask = *ppTask;
        if(pTask->Entry(pTask)){
            pTask->Finalize(pTask);
            *ppTask = *--ppEnd;
            pThread->ppRunningsEnd= ppEnd;
        }
        else
            ++ppTask;
    }
}

static void *FixedWorkerThread_Entry(WorkerThread *pThread){
    while(true){
        FixedWorkerThread_WaitTasks(pThread);
        FixedWorkerThread_RunTasks(pThread);
    }
    return null;
}
#endif //FIXEDWORKERTHREAD_H

