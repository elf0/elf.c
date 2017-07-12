#ifndef FIXEDTASKTHREAD_H
#define FIXEDTASKTHREAD_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

#include "Type.h"
#include "List.h"
#include "Thread.h"
#include "ThreadCondition.h"
#include "Task.h"

#define MAX_RUNNING_TASKS 64

typedef struct{
    Thread thread;
    ThreadLock lock;
    ThreadCondition condition;
    Task *szRunnings[MAX_RUNNING_TASKS];
    Task **ppRunningsEnd;
    Task **ppRunningsMax;
    List pendings;
}FixedTaskThread;

static void *FixedTaskThread_Entry(FixedTaskThread *pThread);

static inline void FixedTaskThread_Initialize(FixedTaskThread *pThread){
    Thread_Initialize((Thread*)pThread, (ThreadEntry)FixedTaskThread_Entry);
    ThreadLock_Initialize(&pThread->lock);
    ThreadCondition_Initialize(&pThread->condition);
    pThread->ppRunningsEnd = pThread->szRunnings;
    pThread->ppRunningsMax = pThread->szRunnings + MAX_RUNNING_TASKS;
    List_Initialize(&pThread->pendings);
}

static inline void FixedTaskThread_Finalize(FixedTaskThread *pThread){
    ThreadCondition_Finalize(&pThread->condition);
    ThreadLock_Finalize(&pThread->lock);
}

static inline B FixedTaskThread_Run(FixedTaskThread *pThread){
    return Thread_Run((Thread*)pThread);
}

static inline void FixedTaskThread_Post(FixedTaskThread *pThread, Task *pTask){
    ThreadLock_Lock(&pThread->lock);
    List_Push(&pThread->pendings, (DoubleNode*)pTask);
    ThreadLock_Unlock(&pThread->lock);

    ThreadCondition_Signal(&pThread->condition);
}

//Do not post Empty task list
static inline void FixedTaskThread_PostTasks(FixedTaskThread *pThread, List *pTasks){
    ThreadLock_Lock(&pThread->lock);
    List_MoveTo(pTasks, &pThread->pendings);
    ThreadLock_Unlock(&pThread->lock);

    ThreadCondition_Signal(&pThread->condition);
}

//Internal
static inline B FixedTaskThread_HaveTasks(List *pTasks){
    return List_NotEmpty(pTasks);
}

static inline B FixedTaskThread_NoTasks(List *pTasks){
    return List_Empty(pTasks);
}

static inline void FixedTaskThread_WaitTasks(FixedTaskThread *pThread){
    Task **ppRunningsMax = pThread->ppRunningsMax;
    List *pPendings = &pThread->pendings;

    ThreadLock_Lock(&pThread->lock);

    if(FixedTaskThread_HaveTasks(pPendings)){
        while(pThread->ppRunningsEnd != ppRunningsMax){
            Task *pTask = (Task *)List_Pop(pPendings);
            *pThread->ppRunningsEnd++ = pTask;
            if(FixedTaskThread_NoTasks(pPendings))
                break;
        }
    }
    else if(pThread->ppRunningsEnd == pThread->szRunnings){
        ThreadCondition_Wait(&pThread->condition, &pThread->lock);
        while(pThread->ppRunningsEnd != ppRunningsMax){
            Task *pTask = (Task *)List_Pop(pPendings);
            *pThread->ppRunningsEnd++ = pTask;
            if(FixedTaskThread_NoTasks(pPendings))
                break;
        }
    }

    ThreadLock_Unlock(&pThread->lock);
}

static inline void FixedTaskThread_RunTasks(FixedTaskThread *pThread){
    Task **ppTask = pThread->szRunnings;
    Task **ppEnd = pThread->ppRunningsEnd;
    while(ppTask != ppEnd){
        Task *pTask = *ppTask;
        if(pTask->bCancel || pTask->Perform(pTask)){
            pTask->Finalize(pTask);
            *ppTask = *--ppEnd;
            pThread->ppRunningsEnd= ppEnd;
        }
        else
            ++ppTask;
    }
}

static void *FixedTaskThread_Entry(FixedTaskThread *pThread){
    while(true){
        FixedTaskThread_WaitTasks(pThread);
        FixedTaskThread_RunTasks(pThread);
    }
    return null;
}
#endif //FIXEDTASKTHREAD_H


