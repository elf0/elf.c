#ifndef DATATHREAD_H
#define DATATHREAD_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

#include "Type.h"
#include "List.h"
#include "Thread.h"
#include "ThreadCondition.h"

typedef struct Data Data;
typedef void (*DataHandler)(Data *pData);

typedef struct{
    Thread thread;
    ThreadLock lock;
    ThreadCondition condition;
    List datas;
    List pendings;
    DataHandler Handle;
}DataThread;

struct Data{
    DoubleNode node;
};

static void *DataThread_Entry(DataThread *pThread);

static inline void DataThread_Initialize(DataThread *pThread, DataHandler Handle){
    Thread_Initialize((Thread*)pThread, (ThreadEntry)DataThread_Entry);
    ThreadLock_Initialize(&pThread->lock);
    ThreadCondition_Initialize(&pThread->condition);
    List_Initialize(&pThread->datas);
    List_Initialize(&pThread->pendings);
    pThread->Handle = Handle;
}

static inline void DataThread_Finalize(DataThread *pThread){
    ThreadCondition_Finalize(&pThread->condition);
    ThreadLock_Finalize(&pThread->lock);
}

static inline B DataThread_Run(DataThread *pThread){
    return Thread_Run((Thread*)pThread);
}

static inline void DataThread_Post(DataThread *pThread, Data *pData){
    ThreadLock_Lock(&pThread->lock);
    List_Push(&pThread->pendings, (DoubleNode*)pData);
    ThreadLock_Unlock(&pThread->lock);

    ThreadCondition_Signal(&pThread->condition);
}

//Do not post Empty data list
static inline void DataThread_PostDatas(DataThread *pThread, List *pDatas){
    ThreadLock_Lock(&pThread->lock);
    List_MoveTo(pDatas, &pThread->pendings);
    ThreadLock_Unlock(&pThread->lock);

    ThreadCondition_Signal(&pThread->condition);
}

//Internal
static inline B DATATHREAD_HaveDatas(List *pDatas){
    return List_NotEmpty(pDatas);
}

static inline B DataThread_NoDatas(List *pDatas){
    return List_Empty(pDatas);
}

static inline void DataThread_MoveDatas(List *pPendings, List *pDatas){
    List_MoveTo(pPendings, pDatas);
}

static inline void DataThread_WaitDatas(DataThread *pThread){
    List *pDatas = &pThread->datas;
    List *pPendings = &pThread->pendings;

    ThreadLock_Lock(&pThread->lock);

    if(DATATHREAD_HaveDatas(pPendings))
        DataThread_MoveDatas(pPendings, pDatas);
    else if(DataThread_NoDatas(pDatas)){
        ThreadCondition_Wait(&pThread->condition, &pThread->lock);
        DataThread_MoveDatas(pPendings, pDatas);
    }

    ThreadLock_Unlock(&pThread->lock);
}

static inline void  DataThread_ProcessDatas(DataThread *pThread){
    List *pDatas = &pThread->datas;
    while(List_NotEmpty(pDatas)){
        Data *pData = (Data*)List_Pop(pDatas);
        pThread->Handle(pData);
    }
}

static void *DataThread_Entry(DataThread *pThread){
    while(true){
        DataThread_WaitDatas(pThread);
        DataThread_ProcessDatas(pThread);
    }
    return null;
}
#endif //DATATHREAD_H


