#ifndef DataThread_H
#define DataThread_H

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

static void *DataThread_Entry(DataThread *pThread);

static inline void DataThread_Initialize(DataThread *pThread, DataHandler Handle){
    Thread_Initialize((Thread*)pThread, (ThreadEntry)DataThread_Entry);
    ThreadLock_Initialize(&pThread->lock);
    ThreadCondition_Initialize(&pThread->condition);
    List_Initialize(&pThread->datas);
    List_Initialize(&pThread->pendingDatas);
    pThread->Handle = Handle;
}

static inline void DataThread_Finalize(DataThread *pThread){
    ThreadCondition_Finalize(&pThread->condition);
    ThreadLock_Finalize(&pThread->lock);
}

static inline Bool DataThread_Run(DataThread *pThread){
    return Thread_Run((Thread*)pThread);
}

static inline void DataThread_Post(DataThread *pThread, Data *pData){
    ThreadLock_Lock(&pThread->lock);
    List_Push(&pThread->pendingDatas, (DoubleNode*)pData);
    ThreadLock_Unlock(&pThread->lock);

    ThreadCondition_Signal(&pThread->condition);
}

//Do not post Empty data list
static inline void DataThread_PostDatas(DataThread *pThread, List *pDatas){
    ThreadLock_Lock(&pThread->lock);
    List_MoveTo(pDatas, &pThread->pendingDatas);
    ThreadLock_Unlock(&pThread->lock);

    ThreadCondition_Signal(&pThread->condition);
}

//Internal
static inline Bool DataThread_HaveDatas(List *pDatas){
    return List_NotEmpty(pDatas);
}

static inline Bool DataThread_NoDatas(List *pDatas){
    return List_Empty(pDatas);
}

static inline void DataThread_MoveDatas(List *pPendingDatas, List *pDatas){
    List_MoveTo(pPendingDatas, pDatas);
}

static inline void DataThread_WaitDatas(DataThread *pThread){
    List *pDatas = &pThread->datas;
    List *pPendingDatas = &pThread->pendingDatas;

    ThreadLock_Lock(&pThread->lock);

    if(DataThread_HaveDatas(pPendingDatas))
        DataThread_MoveDatas(pPendingDatas, pDatas);

    if(DataThread_NoDatas(pDatas)){
        ThreadCondition_Wait(&pThread->condition, &pThread->lock);
        DataThread_MoveDatas(pPendingDatas, pDatas);
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
#endif //DataThread_H


