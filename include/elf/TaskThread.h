#ifndef TASKTHREAD_H
#define TASKTHREAD_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "Type.h"
#include "List.h"
#include "Thread.h"
#include "ThreadCondition.h"

typedef struct structTask Task;
typedef Bool (* TaskHandler)(Task *pTask);
typedef void (*TaskFinalize)(Task *pTask);

struct structTask{
    DoubleNode node;
    TaskHandler Perform;
    TaskFinalize Finalize;
};

static inline void Task_Initialize(Task *pTask,  TaskHandler Perform, TaskFinalize Finalize){
    pTask->Perform = Perform;
    pTask->Finalize = Finalize;
}


typedef struct{
    Thread thread;
    ThreadLock lock;
    ThreadCondition condition;
    List tasks;
    List pendings;
}TaskThread;

static void *TaskThread_Entry(TaskThread *pThread);

static inline void TaskThread_Initialize(TaskThread *pThread){
    Thread_Initialize((Thread*)pThread, (ThreadEntry)TaskThread_Entry);
    ThreadLock_Initialize(&pThread->lock);
    ThreadCondition_Initialize(&pThread->condition);
    List_Initialize(&pThread->tasks);
    List_Initialize(&pThread->pendings);
}

static inline void TaskThread_Finalize(TaskThread *pThread){
    ThreadCondition_Finalize(&pThread->condition);
    ThreadLock_Finalize(&pThread->lock);
}

static inline Bool TaskThread_Run(TaskThread *pThread){
    return Thread_Run((Thread*)pThread);
}

static inline void TaskThread_Post(TaskThread *pThread, Task *pTask){
    ThreadLock_Lock(&pThread->lock);
    List_Push(&pThread->pendings, (DoubleNode*)pTask);
    ThreadLock_Unlock(&pThread->lock);

    ThreadCondition_Signal(&pThread->condition);
}

//Do not post Empty task list
static inline void TaskThread_PostTasks(TaskThread *pThread, List *pTasks){
    ThreadLock_Lock(&pThread->lock);
    List_MoveTo(pTasks, &pThread->pendings);
    ThreadLock_Unlock(&pThread->lock);

    ThreadCondition_Signal(&pThread->condition);
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

static inline void TaskThread_WaitTasks(TaskThread *pThread){
    List *pTasks = &pThread->tasks;
    List *pPendings = &pThread->pendings;

    ThreadLock_Lock(&pThread->lock);

    if(TaskThread_HaveTasks(pPendings))
        TaskThread_MoveTasks(pPendings, pTasks);

    if(TaskThread_NoTasks(pTasks)){
        ThreadCondition_Wait(&pThread->condition, &pThread->lock);
        TaskThread_MoveTasks(pPendings, pTasks);
    }

    ThreadLock_Unlock(&pThread->lock);
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
    while(true){
        TaskThread_WaitTasks(pThread);
        TaskThread_RunTasks(pThread);
    }
    return null;
}
#endif //TASKTHREAD_H


