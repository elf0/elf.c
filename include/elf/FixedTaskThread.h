#ifndef FIXEDTASKTHREAD_H
#define FIXEDTASKTHREAD_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "Type.h"
#include "List.h"
#include "Thread.h"
#include "ThreadCondition.h"

#define MAX_RUNNING_TASKS 64

typedef struct structTask Task;
typedef Bool (*TaskHandler)(Task *pTask);
typedef void (*TaskFinalize)(Task *pTask);

struct structTask{
    DoubleNode node;
    TaskHandler Perform;
    TaskFinalize Finalize;
};

static inline void Task_Initialize(Task *pTask, TaskHandler Perform, TaskFinalize Finalize){
    pTask->Perform = Perform;
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
}TaskThread;

static void *FixedTaskThread_Entry(TaskThread *pThread);

static inline void FixedTaskThread_Initialize(TaskThread *pThread){
    Thread_Initialize((Thread*)pThread, (ThreadEntry)FixedTaskThread_Entry);
    ThreadLock_Initialize(&pThread->lock);
    ThreadCondition_Initialize(&pThread->condition);
    pThread->ppRunningsEnd = pThread->szRunnings;
    pThread->ppRunningsMax = pThread->szRunnings + MAX_RUNNING_TASKS;
    List_Initialize(&pThread->lstWaitings);
}

static inline void FixedTaskThread_Finalize(TaskThread *pThread){
    ThreadCondition_Finalize(&pThread->condition);
    ThreadLock_Finalize(&pThread->lock);
}

static inline Bool FixedTaskThread_Run(TaskThread *pThread){
    return Thread_Run((Thread*)pThread);
}

static inline void FixedTaskThread_Post(TaskThread *pThread, Task *pTask){
    ThreadLock_Lock(&pThread->lock);
    List_Push(&pThread->lstWaitings, (DoubleNode*)pTask);
    ThreadLock_Unlock(&pThread->lock);

    ThreadCondition_Signal(&pThread->condition);
}

//Do not post Empty task list
static inline void FixedTaskThread_PostTasks(TaskThread *pThread, List *pTasks){
    ThreadLock_Lock(&pThread->lock);
    List_MoveTo(pTasks, &pThread->lstWaitings);
    ThreadLock_Unlock(&pThread->lock);

    ThreadCondition_Signal(&pThread->condition);
}

//Internal
static inline Bool FixedTaskThread_HaveWaitings(List *pTasks){
    return List_NotEmpty(pTasks);
}

static inline Bool FixedTaskThread_NoTasks(List *pTasks){
    return List_Empty(pTasks);
}

static inline void FixedTaskThread_FillTasks(TaskThread *pThread){
    Task **ppRunningsMax = pThread->ppRunningsMax;
    List *pWaitings = &pThread->lstWaitings;

    while(pThread->ppRunningsEnd != ppRunningsMax && FixedTaskThread_HaveWaitings(pWaitings)){
        Task *pTask = (Task *)List_Pop(pWaitings);
        *pThread->ppRunningsEnd++ = pTask;
    }
}

static inline void FixedTaskThread_WaitTasks(TaskThread *pThread){
    ThreadLock_Lock(&pThread->lock);

    FixedTaskThread_FillTasks(pThread);

    if(pThread->ppRunningsEnd == pThread->szRunnings){
        ThreadCondition_Wait(&pThread->condition, &pThread->lock);
        FixedTaskThread_FillTasks(pThread);
    }

    ThreadLock_Unlock(&pThread->lock);
}

static inline void FixedTaskThread_RunTasks(TaskThread *pThread){
    Task **ppTask = pThread->szRunnings;
    Task **ppEnd = pThread->ppRunningsEnd;
    while(ppTask != ppEnd){
        Task *pTask = *ppTask;
        if(pTask->Perform(pTask)){
            pTask->Finalize(pTask);
            *ppTask = *--ppEnd;
            pThread->ppRunningsEnd= ppEnd;
        }
        else
            ++ppTask;
    }
}

static void *FixedTaskThread_Entry(TaskThread *pThread){
    while(true){
        FixedTaskThread_WaitTasks(pThread);
        FixedTaskThread_RunTasks(pThread);
    }
    return null;
}
#endif //FIXEDTASKTHREAD_H

