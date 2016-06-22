#ifndef THREAD_H
#define THREAD_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

#ifdef __linux__
#include <pthread.h>
#else
#endif

typedef struct structThread Thread;
typedef void *(*ThreadEntry)(Thread *pThread);

struct structThread{
#ifdef __linux__
 pthread_t thread;
#else
#endif
 ThreadEntry Entry;
};

static inline void Thread_Initialize(Thread *pThread, ThreadEntry Entry){
 pThread->Entry = Entry;
}

static inline B Thread_Run(Thread *pThread){
#ifdef __linux__
 return pthread_create(&pThread->thread, NULL, (void*(*)(void*))pThread->Entry, pThread) == 0;
#else
#endif
}

static inline void *Thread_Join(Thread *pThread){
#ifdef __linux__
  void *pResult;
  pthread_join(&pThread->thread, &pResult);
  return pResult;
#else
#endif
}

#endif //THREAD_H
