#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

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
    List pendingDatas;
    DataHandler onData;
}DataProcessor;

struct Data{
    DoubleNode node;
};

static void *DataProcessor_Entry(DataProcessor *pThread);

static inline void DataProcessor_Initialize(DataProcessor *pThread, DataHandler onData){
    Thread_Initialize((Thread*)pThread, (ThreadEntry)DataProcessor_Entry);
    ThreadLock_Initialize(&pThread->lock);
    ThreadCondition_Initialize(&pThread->condition);
    List_Initialize(&pThread->datas);
    List_Initialize(&pThread->pendingDatas);
    pThread->onData = onData;
}

static inline void DataProcessor_Finalize(DataProcessor *pThread){
    ThreadCondition_Finalize(&pThread->condition);
    ThreadLock_Finalize(&pThread->lock);
}

static inline Bool DataProcessor_Run(DataProcessor *pThread){
    return Thread_Run((Thread*)pThread);
}

static inline void DataProcessor_Post(DataProcessor *pThread, Data *pData){
    ThreadLock_Lock(&pThread->lock);
    List_Push(&pThread->pendingDatas, (DoubleNode*)pData);
    ThreadLock_Unlock(&pThread->lock);

    ThreadCondition_Signal(&pThread->condition);
}

//Do not post Empty task list
static inline void DataProcessor_PostDatas(DataProcessor *pThread, List *pDatas){
    ThreadLock_Lock(&pThread->lock);
    List_MoveTo(pDatas, &pThread->pendingDatas);
    ThreadLock_Unlock(&pThread->lock);

    ThreadCondition_Signal(&pThread->condition);
}

//Internal
static inline Bool DATAPROCESSOR_HaveDatas(List *pDatas){
    return List_NotEmpty(pDatas);
}

static inline Bool DataProcessor_NoDatas(List *pDatas){
    return List_Empty(pDatas);
}

static inline void DataProcessor_MoveDatas(List *pPendingDatas, List *pDatas){
    List_MoveTo(pPendingDatas, pDatas);
}

static inline void DataProcessor_WaitDatas(DataProcessor *pThread){
    List *pDatas = &pThread->datas;
    List *pPendingDatas = &pThread->pendingDatas;

    ThreadLock_Lock(&pThread->lock);

    if(DATAPROCESSOR_HaveDatas(pPendingDatas))
        DataProcessor_MoveDatas(pPendingDatas, pDatas);

    if(DataProcessor_NoDatas(pDatas)){
        ThreadCondition_Wait(&pThread->condition, &pThread->lock);
        DataProcessor_MoveDatas(pPendingDatas, pDatas);
    }

    ThreadLock_Unlock(&pThread->lock);
}

static inline void  DataProcessor_ProcessDatas(DataProcessor *pThread){
    List *pDatas = &pThread->datas;
    while(List_NotEmpty(pDatas)){
        Data *pData = (Data*)List_Pop(pDatas);
        pThread->onData(pData);
    }
}

static void *DataProcessor_Entry(DataProcessor *pThread){
    while(true){
        DataProcessor_WaitDatas(pThread);
        DataProcessor_ProcessDatas(pThread);
    }
    return null;
}
#endif //DATAPROCESSOR_H


