#ifndef TASKTHREAD_H
#define TASKTHREAD_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

#include "Type.h"
#include "List.h"
#include "Thread.h"
#include "ThreadCondition.h"
#include "Task.h"

typedef struct{
    Thread thread;
    List tasks;
    List *pPendings;
    ThreadLock *pLock;
    ThreadCondition *pCondition;
    Bool bContinue;
}TaskThread;

static void *TaskThread_Entry(TaskThread *pThread);

static inline void TaskThread_Initialize(TaskThread *pThread, ThreadLock *pLock, ThreadCondition *pCondition, List *pPendingTasks){
    Thread_Initialize((Thread*)pThread, (ThreadEntry)TaskThread_Entry);
    List_Initialize(&pThread->tasks);
    pThread->pLock = pLock;
    pThread->pCondition = pCondition;
    pThread->pPendings = pPendingTasks;
    pThread->bContinue = true;
}

static inline void TaskThread_Finalize(TaskThread *pThread){
    pThread->bContinue = false;
}

static inline Bool TaskThread_Run(TaskThread *pThread){
    return Thread_Run((Thread*)pThread);
}

static inline void TaskThread_Post(TaskThread *pThread, Task *pTask){
    ThreadLock *pLock = pThread->pLock;
    ThreadLock_Lock(pLock);
    List_Push(pThread->pPendings, (DoubleNode*)pTask);
    ThreadLock_Unlock(pLock);

    ThreadCondition_Signal(pThread->pCondition);
}

//Do not post Empty task list
static inline void TaskThread_PostTasks(TaskThread *pThread, List *pTasks){
    ThreadLock *pLock = pThread->pLock;
    ThreadLock_Lock(pLock);
    List_MoveTo(pTasks, pThread->pPendings);
    ThreadLock_Unlock(pLock);

    ThreadCondition_Signal(pThread->pCondition);
}

//Internal
static inline Bool TaskThread_HaveTasks(List *pTasks){
    return List_NotEmpty(pTasks);
}

static inline Bool TaskThread_NoTasks(List *pTasks){
    return List_Empty(pTasks);
}

static inline void TaskThread_MoveTasks(List *pPendings, List *pTasks){
    List_MoveTo(pPendings, pTasks);
}

static inline void TaskThread_RunTasks(TaskThread *pThread){
    DoubleNode *pEntry = (DoubleNode*)&pThread->tasks;
    DoubleNode *pNode = pEntry->pNext;
    Task *pTask;
    while(pNode != pEntry){
        pTask = (Task*)pNode;
        if(pTask->Perform(pTask)){
            List_Remove(pNode);
            pNode = pNode->pNext;
            pTask->Finalize(pTask);
        }
        else
            pNode = pNode->pNext;
    }
}

static void *TaskThread_Entry(TaskThread *pThread){
    List *pTasks = &pThread->tasks;
    List *pPendings = pThread->pPendings;
    ThreadLock *pLock = pThread->pLock;
    ThreadCondition *pCondition = pThread->pCondition;

    while(pThread->bContinue){
        ThreadLock_Lock(pLock);

        if(TaskThread_HaveTasks(pPendings))
            TaskThread_MoveTasks(pPendings, pTasks);
        else if(TaskThread_NoTasks(pTasks)){
            ThreadCondition_Wait(pCondition, pLock);
            if(pThread->bContinue)
                TaskThread_MoveTasks(pPendings, pTasks);
            else{
                ThreadLock_Unlock(pLock);
                break;
            }
        }

        ThreadLock_Unlock(pLock);

        TaskThread_RunTasks(pThread);
    }
    return null;
}
#endif //TASKTHREAD_H


