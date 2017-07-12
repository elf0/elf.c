#ifndef THREADLOCK_H
#define THREADLOCK_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

typedef struct{
 pthread_mutex_t mutex;
}ThreadLock;

static inline void ThreadLock_Initialize(ThreadLock *pLock){
 pLock->mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
}

static inline void ThreadLock_Finalize(ThreadLock *pLock){
 pthread_mutex_destroy(&pLock->mutex);
}

static inline void ThreadLock_Lock(ThreadLock *pLock){
 pthread_mutex_lock(&pLock->mutex);
}

static inline void ThreadLock_Unlock(ThreadLock *pLock){
 pthread_mutex_unlock(&pLock->mutex);
}

#endif //THREADLOCK_H
