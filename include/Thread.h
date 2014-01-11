#pragma once

#include <pthread.h>

typedef struct structThread Thread;
typedef void *(*ThreadEntry)(Thread *pThread);

struct structThread{
 pthread_t thread;
 ThreadEntry Entry;
};

static inline void Thread_Initialize(Thread *pThread, ThreadEntry Entry){
 pThread->Entry = Entry;
}

static inline Bool Thread_Run(Thread *pThread){
 return pthread_create(&pThread->thread, NULL, (void*(*)(void*))pThread->Entry, pThread) == 0;
}
