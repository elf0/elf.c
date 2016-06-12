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
    B bContinue;
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
    ThreadLock *pLock = pThread->pLock;
    ThreadLock_Lock(pLock);
    pThread->bContinue = false;
    ThreadCondition_SignalAll(pThread->pCondition);
    ThreadLock_Unlock(pLock);
    Thread_Join((Thread*)pThread);
}

static inline B TaskThread_Run(TaskThread *pThread){
    return Thread_Run((Thread*)pThread);
}

static inline void TaskThread_Post(TaskThread *pThread, Task *pTask){
    ThreadLock *pLock = pThread->pLock;
    ThreadLock_Lock(pLock);
    List_Push(pThread->pPendings, (DoubleNode*)pTask);
    ThreadCondition_Signal(pThread->pCondition);
    ThreadLock_Unlock(pLock);

}

//Do not post Empty task list
static inline void TaskThread_PostTasks(TaskThread *pThread, List *pTasks){
    ThreadLock *pLock = pThread->pLock;
    ThreadLock_Lock(pLock);
    List_MoveTo(pTasks, pThread->pPendings);
    ThreadCondition_SignalAll(pThread->pCondition);
    ThreadLock_Unlock(pLock);
}

//Internal

static inline void TaskThread_RunTasks(List *pTasks){
    DoubleNode *pEntry = (DoubleNode*)pTasks;
    DoubleNode *pNode = pEntry->pNext;
    Task *pTask;
    while(pNode != pEntry){
        pTask = (Task*)pNode;
        if(pTask->bCancel || pTask->Perform(pTask)){
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
    B *pbContinue = &pThread->bContinue;
    while(true){
        ThreadLock_Lock(pLock);
        if(!*pbContinue)
            break;

        if(List_Empty(pPendings)){
            if(List_Empty(pTasks)){
                ThreadCondition_Wait(pCondition, pLock);
                if(!*pbContinue){
                    ThreadLock_Unlock(pLock);
                    break;
                }

                if(List_NotEmpty(pPendings))
                    List_PopToFront(pPendings, pTasks);
            }
        }
        else
            List_PopToFront(pPendings, pTasks);

        ThreadLock_Unlock(pLock);

        TaskThread_RunTasks(pTasks);
    }
    return null;
}
#endif //TASKTHREAD_H


