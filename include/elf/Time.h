#ifndef TIME_H
#define TIME_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

#ifdef __linux__

inline
static U64 Time_Tick() {
    U64 uTick;
    asm volatile(
        "mfence\n\t"
        "rdtsc\n\t"
        "lfence\n\t"
        "shl $32, %%rdx\n\t"
        "or %%rdx, %0"
        : "=a" (uTick)
        :
        : "rdx");
    return uTick;
} 

#include <time.h>

#define MILLISECONDS_PER_SECOND 1000
#define MICROSECONDS_PER_SECOND 1000000
#define NANOSECONDS_PER_SECOND 1000000000

#define NANOSECONDS_PER_MILLISECOND 1000000
#define NANOSECONDS_PER_MICROSECOND 1000

typedef struct timespec Time;

inline
static void Time_Real(Time *pTime) {
    clock_gettime(CLOCK_REALTIME, pTime);
}

inline
static void Time_Boot(Time *pTime) {
    clock_gettime(CLOCK_BOOTTIME, pTime);
}

inline
static void Time_Process(Time *pTime) {
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, pTime);
}

inline
static void Time_Thread(Time *pTime) {
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, pTime);
}

inline
static void Time_Raw(Time *pTime) {
    clock_gettime(CLOCK_MONOTONIC_RAW, pTime);
}

inline
static void Time_Coarse(Time *pTime) {
    clock_gettime(CLOCK_MONOTONIC_COARSE, pTime);
}


inline
static U64 Time_Seconds(const Time *pTime) {
    return pTime->tv_sec;
}

inline
static U64 Time_Milliseconds(const Time *pTime) {
    return pTime->tv_sec * MILLISECONDS_PER_SECOND + pTime->tv_nsec / NANOSECONDS_PER_MILLISECOND;
}

inline
static U64 Time_Microseconds(const Time *pTime) {
    return pTime->tv_sec * MICROSECONDS_PER_SECOND + pTime->tv_nsec / NANOSECONDS_PER_MICROSECOND;
}

inline
static U64 Time_Nanoseconds(const Time *pTime) {
    return pTime->tv_sec * NANOSECONDS_PER_SECOND + pTime->tv_nsec;
}

inline
static U64 Time_ElapsedSeconds(const Time *ptBegin, const Time *ptEnd) {
    return ptEnd->tv_sec - ptBegin->tv_sec;
}

inline
static U64 Time_ElapsedMilliseconds(const Time *ptBegin, const Time *ptEnd) {
    return (ptEnd->tv_sec - ptBegin->tv_sec) * MILLISECONDS_PER_SECOND + (ptEnd->tv_nsec - ptBegin->tv_nsec)/NANOSECONDS_PER_MILLISECOND;
}

inline
static U64 Time_ElapsedMicroseconds(const Time *ptBegin, const Time *ptEnd) {
    return (ptEnd->tv_sec - ptBegin->tv_sec) * MICROSECONDS_PER_SECOND + (ptEnd->tv_nsec - ptBegin->tv_nsec)/NANOSECONDS_PER_MICROSECOND;
}

inline
static U64 Time_ElapsedNanoseconds(const Time *ptBegin, const Time *ptEnd) {
    return (ptEnd->tv_sec - ptBegin->tv_sec) * NANOSECONDS_PER_SECOND + (ptEnd->tv_nsec - ptBegin->tv_nsec);
}

#else
#endif

#endif //TIME_H
