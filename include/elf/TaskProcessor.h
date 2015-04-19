#ifndef TASKPROCESSOR_H
#define TASKPROCESSOR_H

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
}TaskProcessor;

static void *TaskProcessor_Entry(TaskProcessor *pProcessor);

static inline void TaskProcessor_Initialize(TaskProcessor *pProcessor){
    Thread_Initialize((Thread*)pProcessor, (ThreadEntry)TaskProcessor_Entry);
    ThreadLock_Initialize(&pProcessor->lock);
    ThreadCondition_Initialize(&pProcessor->condition);
    List_Initialize(&pProcessor->tasks);
    List_Initialize(&pProcessor->pendingTasks);
}

static inline void TaskProcessor_Finalize(TaskProcessor *pProcessor){
    ThreadCondition_Finalize(&pProcessor->condition);
    ThreadLock_Finalize(&pProcessor->lock);
}

static inline Bool TaskProcessor_Run(TaskProcessor *pProcessor){
    return Thread_Run((Thread*)pProcessor);
}

static inline void TaskProcessor_Post(TaskProcessor *pProcessor, Task *pTask){
    ThreadLock_Lock(&pProcessor->lock);
    List_Push(&pProcessor->pendingTasks, (DoubleNode*)pTask);
    ThreadLock_Unlock(&pProcessor->lock);

    ThreadCondition_Signal(&pProcessor->condition);
}

//Do not post Empty task list
static inline void TaskProcessor_PostTasks(TaskProcessor *pProcessor, List *pTasks){
    ThreadLock_Lock(&pProcessor->lock);
    List_MoveTo(pTasks, &pProcessor->pendingTasks);
    ThreadLock_Unlock(&pProcessor->lock);

    ThreadCondition_Signal(&pProcessor->condition);
}

//Internal
static inline Bool TaskProcessor_HaveTasks(List *pTasks){
    return List_NotEmpty(pTasks);
}

static inline Bool TaskProcessor_NoTasks(List *pTasks){
    return List_Empty(pTasks);
}

static inline void TaskProcessor_MoveTasks(List *pPendingTasks, List *pTasks){
    List_MoveTo(pPendingTasks, pTasks);
}

static inline void TaskProcessor_WaitTasks(TaskProcessor *pProcessor){
    List *pTasks = &pProcessor->tasks;
    List *pPendingTasks = &pProcessor->pendingTasks;

    ThreadLock_Lock(&pProcessor->lock);

    if(TaskProcessor_HaveTasks(pPendingTasks))
        TaskProcessor_MoveTasks(pPendingTasks, pTasks);

    if(TaskProcessor_NoTasks(pTasks)){
        ThreadCondition_Wait(&pProcessor->condition, &pProcessor->lock);
        TaskProcessor_MoveTasks(pPendingTasks, pTasks);
    }

    ThreadLock_Unlock(&pProcessor->lock);
}

static inline void TaskProcessor_RunTasks(TaskProcessor *pProcessor){
    DoubleNode *pEntry = (DoubleNode*)&pProcessor->tasks;
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

static void *TaskProcessor_Entry(TaskProcessor *pProcessor){
    while(true){
        TaskProcessor_WaitTasks(pProcessor);
        TaskProcessor_RunTasks(pProcessor);
    }
    return null;
}
#endif //TASKPROCESSOR_H


