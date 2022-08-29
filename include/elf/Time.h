#ifndef TIME_H
#define TIME_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

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
static void Time_Elapsed(const Time *ptBegin, const Time *ptEnd, Time *ptElapsed) {
    I64 nSeconds = ptEnd->tv_sec - ptBegin->tv_sec;
    I64 nNanoSeconds = ptEnd->tv_nsec - ptBegin->tv_nsec;
    if(nNanoSeconds < 0) {
        --nSeconds;
        nNanoSeconds += NANOSECONDS_PER_SECOND;
    }
    ptElapsed->tv_sec = nSeconds;
    ptElapsed->tv_nsec = nNanoSeconds;
}

inline
static U64 Time_Seconds(const Time *pTime) {
    return pTime->tv_sec;
}

inline
static U64 Time_Milliseconds(const Time *pTime) {
    U64 nSecondPart = pTime->tv_sec;
    nSecondPart *= MILLISECONDS_PER_SECOND;
    U64 nNanoPart = pTime->tv_nsec;
    nNanoPart /= NANOSECONDS_PER_MILLISECOND;
    return nSecondPart + nNanoPart;
}

inline
static U64 Time_Microseconds(const Time *pTime) {
    U64 nSecondPart = pTime->tv_sec;
    nSecondPart *= MICROSECONDS_PER_SECOND;
    U64 nNanoPart = pTime->tv_nsec;
    nNanoPart /= NANOSECONDS_PER_MICROSECOND;
    return nSecondPart + nNanoPart;
}

inline
static U64 Time_Nanoseconds(const Time *pTime) {
    U64 nNanoseconds = pTime->tv_sec;
    nNanoseconds *= NANOSECONDS_PER_SECOND;
    nNanoseconds += pTime->tv_nsec;
    return nNanoseconds;
}

inline
static U64 Time_ElapsedSeconds(const Time *ptBegin, const Time *ptEnd) {
    I64 nSeconds = ptEnd->tv_sec - ptBegin->tv_sec;
    if(ptEnd->tv_nsec < ptBegin->tv_nsec)
        --nSeconds;
    return nSeconds;
}

inline
static U64 Time_ElapsedMilliseconds(const Time *ptBegin, const Time *ptEnd) {
    I64 iMilliseconds = (ptEnd->tv_sec - ptBegin->tv_sec) * MILLISECONDS_PER_SECOND;
    iMilliseconds += (ptEnd->tv_nsec - ptBegin->tv_nsec) / NANOSECONDS_PER_MILLISECOND;
    return iMilliseconds;
}

inline
static U64 Time_ElapsedMicroseconds(const Time *ptBegin, const Time *ptEnd) {
    I64 iMicroseconds = (ptEnd->tv_sec - ptBegin->tv_sec) * MICROSECONDS_PER_SECOND;
    iMicroseconds += (ptEnd->tv_nsec - ptBegin->tv_nsec) / NANOSECONDS_PER_MICROSECOND;
    return iMicroseconds;
}

inline
static U64 Time_ElapsedNanoseconds(const Time *ptBegin, const Time *ptEnd) {
    I64 iNanoseconds = (ptEnd->tv_sec - ptBegin->tv_sec) * NANOSECONDS_PER_SECOND;
    iNanoseconds += ptEnd->tv_nsec - ptBegin->tv_nsec;
    return iNanoseconds;
}

#endif //TIME_H
