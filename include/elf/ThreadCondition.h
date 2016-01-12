#ifndef THREADCONDITION_H
#define THREADCONDITION_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "ThreadLock.h"

typedef struct{
    pthread_cond_t condition;
}ThreadCondition;

static inline void ThreadCondition_Initialize(ThreadCondition *pCondition){
    pCondition->condition = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
}

static inline void ThreadCondition_Finalize(ThreadCondition *pCondition){
    pthread_cond_destroy(&pCondition->condition);
}

static inline void ThreadCondition_Wait(ThreadCondition *pCondition, ThreadLock *pLock){
    pthread_cond_wait(&pCondition->condition, &pLock->mutex);
}

static inline void ThreadCondition_Signal(ThreadCondition *pCondition){
    pthread_cond_signal(&pCondition->condition);
}

static inline void ThreadCondition_SignalAll(ThreadCondition *pCondition){
    pthread_cond_broadcast(&pCondition->condition);
}

#endif //THREADCONDITION_H
