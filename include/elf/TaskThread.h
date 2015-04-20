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
    List pendingTasks;
}TaskThread;

static void *TaskThread_Entry(TaskThread *pProcessor);

static inline void TaskThread_Initialize(TaskThread *pProcessor){
    Thread_Initialize((Thread*)pProcessor, (ThreadEntry)TaskThread_Entry);
    ThreadLock_Initialize(&pProcessor->lock);
    ThreadCondition_Initialize(&pProcessor->condition);
    List_Initialize(&pProcessor->tasks);
    List_Initialize(&pProcessor->pendingTasks);
}

static inline void TaskThread_Finalize(TaskThread *pProcessor){
    ThreadCondition_Finalize(&pProcessor->condition);
    ThreadLock_Finalize(&pProcessor->lock);
}

static inline Bool TaskThread_Run(TaskThread *pProcessor){
    return Thread_Run((Thread*)pProcessor);
}

static inline void TaskThread_Post(TaskThread *pProcessor, Task *pTask){
    ThreadLock_Lock(&pProcessor->lock);
    List_Push(&pProcessor->pendingTasks, (DoubleNode*)pTask);
    ThreadLock_Unlock(&pProcessor->lock);

    ThreadCondition_Signal(&pProcessor->condition);
}

//Do not post Empty task list
static inline void TaskThread_PostTasks(TaskThread *pProcessor, List *pTasks){
    ThreadLock_Lock(&pProcessor->lock);
    List_MoveTo(pTasks, &pProcessor->pendingTasks);
    ThreadLock_Unlock(&pProcessor->lock);

    ThreadCondition_Signal(&pProcessor->condition);
}

//Internal
static inline Bool TaskThread_HaveTasks(List *pTasks){
    return List_NotEmpty(pTasks);
}

static inline Bool TaskThread_NoTasks(List *pTasks){
    return List_Empty(pTasks);
}

static inline void TaskThread_MoveTasks(List *pPendingTasks, List *pTasks){
    List_MoveTo(pPendingTasks, pTasks);
}

static inline void TaskThread_WaitTasks(TaskThread *pProcessor){
    List *pTasks = &pProcessor->tasks;
    List *pPendingTasks = &pProcessor->pendingTasks;

    ThreadLock_Lock(&pProcessor->lock);

    if(TaskThread_HaveTasks(pPendingTasks))
        TaskThread_MoveTasks(pPendingTasks, pTasks);

    if(TaskThread_NoTasks(pTasks)){
        ThreadCondition_Wait(&pProcessor->condition, &pProcessor->lock);
        TaskThread_MoveTasks(pPendingTasks, pTasks);
    }

    ThreadLock_Unlock(&pProcessor->lock);
}

static inline void TaskThread_RunTasks(TaskThread *pProcessor){
    DoubleNode *pEntry = (DoubleNode*)&pProcessor->tasks;
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

static void *TaskThread_Entry(TaskThread *pProcessor){
    while(true){
        TaskThread_WaitTasks(pProcessor);
        TaskThread_RunTasks(pProcessor);
    }
    return null;
}
#endif //TASKTHREAD_H


