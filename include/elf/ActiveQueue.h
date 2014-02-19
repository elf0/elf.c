#pragma once
//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "CountedList.h"

typedef struct elf_struct_ActiveObject elf_ActiveObject;
struct elf_struct_ActiveObject{
    elf_DoubleNode dnNode;
    U64 nTime;
};

typedef struct elf_struct_ActiveQueue elf_ActiveQueue;
typedef void (*elf_event_ActiveQueue_Alarm)(void *pContext, elf_ActiveObject *pSelf);
struct elf_struct_ActiveQueue{
    elf_CountedList lstObjects;
};

static inline void elf_ActiveQueue_Initialize(elf_ActiveQueue *pQueue){
    elf_CountedList_Initialize((elf_CountedList*)pQueue);
}

static inline Bool elf_ActiveQueue_Empty(const elf_ActiveQueue *pQueue){
    return elf_CountedList_Empty((elf_CountedList*)pQueue);
}

static inline Bool elf_ActiveQueue_NotEmpty(const elf_ActiveQueue *pQueue){
    return elf_CountedList_NotEmpty((elf_CountedList*)pQueue);
}

static inline U elf_ActiveQueue_GetCount(const elf_ActiveQueue *pQueue){
    return elf_CountedList_GetCount((elf_CountedList*)pQueue);
}

static inline void elf_ActiveQueue_Add(elf_ActiveQueue *pQueue, elf_ActiveObject *pObject, U64 nTime){
    pObject->nTime = nTime;
    elf_CountedList_PushFront((elf_CountedList*)pQueue, (elf_DoubleNode*)pObject);
}

static inline void elf_ActiveQueue_Active(elf_ActiveQueue *pQueue, elf_ActiveObject *pObject, U64 nTime){
    elf_CountedList *pList = (elf_CountedList*)pQueue;
    elf_DoubleNode *pNode = (elf_DoubleNode*)pObject;
    if(elf_CountedList_NotFirst(pList, pNode))
        elf_CountedList_MoveNodeToFront(pList, pNode);

    pObject->nTime = nTime;
}

static inline elf_ActiveObject *elf_ActiveQueue_PopLaziest(elf_ActiveQueue *pQueue){
    return (elf_ActiveObject*)elf_CountedList_PopBack((elf_CountedList*)pQueue);
}

static inline elf_ActiveObject *elf_ActiveQueue_AlarmLazyObjects(elf_ActiveQueue *pQueue, U64 nTimeSpan, U64 nNow, elf_event_ActiveQueue_Alarm onAlarm, void *pAlarmContext){
    elf_ActiveObject *pFirstLazy = null;
    elf_DoubleNode *pEntryNode = (elf_DoubleNode*)pQueue;
    elf_DoubleNode *pNode = pEntryNode->pPrev;
    while(pNode != pEntryNode){
        elf_ActiveObject *pObject = (elf_ActiveObject*)pNode;
        if(nNow < pObject->nTime + nTimeSpan)
            break;
        pFirstLazy = (elf_ActiveObject*)pNode;
        onAlarm(pAlarmContext, pFirstLazy);
        pNode = pNode->pPrev;
    }
    return pFirstLazy;
}

