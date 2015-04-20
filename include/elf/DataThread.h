#ifndef DATATHREAD_H
#define DATATHREAD_H

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
}DataThread;

struct Data{
    DoubleNode node;
};

static void *DataThread_Entry(DataThread *pProcessor);

static inline void DataThread_Initialize(DataThread *pProcessor, DataHandler Handle){
    Thread_Initialize((Thread*)pProcessor, (ThreadEntry)DataThread_Entry);
    ThreadLock_Initialize(&pProcessor->lock);
    ThreadCondition_Initialize(&pProcessor->condition);
    List_Initialize(&pProcessor->datas);
    List_Initialize(&pProcessor->pendingDatas);
    pProcessor->Handle = Handle;
}

static inline void DataThread_Finalize(DataThread *pProcessor){
    ThreadCondition_Finalize(&pProcessor->condition);
    ThreadLock_Finalize(&pProcessor->lock);
}

static inline Bool DataThread_Run(DataThread *pProcessor){
    return Thread_Run((Thread*)pProcessor);
}

static inline void DataThread_Post(DataThread *pProcessor, Data *pData){
    ThreadLock_Lock(&pProcessor->lock);
    List_Push(&pProcessor->pendingDatas, (DoubleNode*)pData);
    ThreadLock_Unlock(&pProcessor->lock);

    ThreadCondition_Signal(&pProcessor->condition);
}

//Do not post Empty data list
static inline void DataThread_PostDatas(DataThread *pProcessor, List *pDatas){
    ThreadLock_Lock(&pProcessor->lock);
    List_MoveTo(pDatas, &pProcessor->pendingDatas);
    ThreadLock_Unlock(&pProcessor->lock);

    ThreadCondition_Signal(&pProcessor->condition);
}

//Internal
static inline Bool DATATHREAD_HaveDatas(List *pDatas){
    return List_NotEmpty(pDatas);
}

static inline Bool DataThread_NoDatas(List *pDatas){
    return List_Empty(pDatas);
}

static inline void DataThread_MoveDatas(List *pPendingDatas, List *pDatas){
    List_MoveTo(pPendingDatas, pDatas);
}

static inline void DataThread_WaitDatas(DataThread *pProcessor){
    List *pDatas = &pProcessor->datas;
    List *pPendingDatas = &pProcessor->pendingDatas;

    ThreadLock_Lock(&pProcessor->lock);

    if(DATATHREAD_HaveDatas(pPendingDatas))
        DataThread_MoveDatas(pPendingDatas, pDatas);

    if(DataThread_NoDatas(pDatas)){
        ThreadCondition_Wait(&pProcessor->condition, &pProcessor->lock);
        DataThread_MoveDatas(pPendingDatas, pDatas);
    }

    ThreadLock_Unlock(&pProcessor->lock);
}

static inline void  DataThread_ProcessDatas(DataThread *pProcessor){
    List *pDatas = &pProcessor->datas;
    while(List_NotEmpty(pDatas)){
        Data *pData = (Data*)List_Pop(pDatas);
        pProcessor->Handle(pData);
    }
}

static void *DataThread_Entry(DataThread *pProcessor){
    while(true){
        DataThread_WaitDatas(pProcessor);
        DataThread_ProcessDatas(pProcessor);
    }
    return null;
}
#endif //DATATHREAD_H


