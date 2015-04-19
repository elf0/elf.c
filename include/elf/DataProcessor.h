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
    DataHandler Handle;
}DataProcessor;

struct Data{
    DoubleNode node;
};

static void *DataProcessor_Entry(DataProcessor *pProcessor);

static inline void DataProcessor_Initialize(DataProcessor *pProcessor, DataHandler Handle){
    Thread_Initialize((Thread*)pProcessor, (ThreadEntry)DataProcessor_Entry);
    ThreadLock_Initialize(&pProcessor->lock);
    ThreadCondition_Initialize(&pProcessor->condition);
    List_Initialize(&pProcessor->datas);
    List_Initialize(&pProcessor->pendingDatas);
    pProcessor->Handle = Handle;
}

static inline void DataProcessor_Finalize(DataProcessor *pProcessor){
    ThreadCondition_Finalize(&pProcessor->condition);
    ThreadLock_Finalize(&pProcessor->lock);
}

static inline Bool DataProcessor_Run(DataProcessor *pProcessor){
    return Thread_Run((Thread*)pProcessor);
}

static inline void DataProcessor_Post(DataProcessor *pProcessor, Data *pData){
    ThreadLock_Lock(&pProcessor->lock);
    List_Push(&pProcessor->pendingDatas, (DoubleNode*)pData);
    ThreadLock_Unlock(&pProcessor->lock);

    ThreadCondition_Signal(&pProcessor->condition);
}

//Do not post Empty task list
static inline void DataProcessor_PostDatas(DataProcessor *pProcessor, List *pDatas){
    ThreadLock_Lock(&pProcessor->lock);
    List_MoveTo(pDatas, &pProcessor->pendingDatas);
    ThreadLock_Unlock(&pProcessor->lock);

    ThreadCondition_Signal(&pProcessor->condition);
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

static inline void DataProcessor_WaitDatas(DataProcessor *pProcessor){
    List *pDatas = &pProcessor->datas;
    List *pPendingDatas = &pProcessor->pendingDatas;

    ThreadLock_Lock(&pProcessor->lock);

    if(DATAPROCESSOR_HaveDatas(pPendingDatas))
        DataProcessor_MoveDatas(pPendingDatas, pDatas);

    if(DataProcessor_NoDatas(pDatas)){
        ThreadCondition_Wait(&pProcessor->condition, &pProcessor->lock);
        DataProcessor_MoveDatas(pPendingDatas, pDatas);
    }

    ThreadLock_Unlock(&pProcessor->lock);
}

static inline void  DataProcessor_ProcessDatas(DataProcessor *pProcessor){
    List *pDatas = &pProcessor->datas;
    while(List_NotEmpty(pDatas)){
        Data *pData = (Data*)List_Pop(pDatas);
        pProcessor->Handle(pData);
    }
}

static void *DataProcessor_Entry(DataProcessor *pProcessor){
    while(true){
        DataProcessor_WaitDatas(pProcessor);
        DataProcessor_ProcessDatas(pProcessor);
    }
    return null;
}
#endif //DATAPROCESSOR_H


