#ifndef ACTIVEQUEUE_H
#define ACTIVEQUEUE_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

#include "CountedList.h"

typedef struct struct_ActiveObject ActiveObject;
struct struct_ActiveObject{
    DoubleNode dnNode;
    U64 nTime;
};

typedef struct struct_ActiveQueue ActiveQueue;
typedef void (*event_ActiveQueue_Alarm)(void *pContext, ActiveObject *pSelf);
struct struct_ActiveQueue{
    CountedList lstObjects;
};

static inline void ActiveQueue_Initialize(ActiveQueue *pQueue){
    CountedList_Initialize((CountedList*)pQueue);
}

static inline B ActiveQueue_Empty(const ActiveQueue *pQueue){
    return CountedList_Empty((CountedList*)pQueue);
}

static inline B ActiveQueue_NotEmpty(const ActiveQueue *pQueue){
    return CountedList_NotEmpty((CountedList*)pQueue);
}

static inline U ActiveQueue_GetCount(const ActiveQueue *pQueue){
    return CountedList_GetCount((CountedList*)pQueue);
}

static inline void ActiveQueue_Add(ActiveQueue *pQueue, ActiveObject *pObject, U64 nTime){
    pObject->nTime = nTime;
    CountedList_PushFront((CountedList*)pQueue, (DoubleNode*)pObject);
}

static inline void ActiveQueue_Active(ActiveQueue *pQueue, ActiveObject *pObject, U64 nTime){
    CountedList *pList = (CountedList*)pQueue;
    DoubleNode *pNode = (DoubleNode*)pObject;
    if(CountedList_NotFirst(pList, pNode))
        CountedList_MoveNodeToFront(pList, pNode);

    pObject->nTime = nTime;
}

static inline ActiveObject *ActiveQueue_PopLaziest(ActiveQueue *pQueue){
    return (ActiveObject*)CountedList_PopBack((CountedList*)pQueue);
}

static inline ActiveObject *ActiveQueue_AlarmLazyObjects(ActiveQueue *pQueue, U64 nTimeOut, U64 nNow, event_ActiveQueue_Alarm onAlarm, void *pAlarmContext){
    ActiveObject *pFirstLazy = null;
    DoubleNode *pEntryNode = (DoubleNode*)pQueue;
    DoubleNode *pNode = pEntryNode->pPrev;
    while(pNode != pEntryNode){
        ActiveObject *pObject = (ActiveObject*)pNode;
        if(nNow < pObject->nTime + nTimeOut)
            break;
        pFirstLazy = (ActiveObject*)pNode;
        onAlarm(pAlarmContext, pFirstLazy);
        pNode = pNode->pPrev;
    }
    return pFirstLazy;
}

#endif //ACTIVEQUEUE_H
