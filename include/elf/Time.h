#ifndef TIME_H
#define TIME_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#define MILLISECONDS_PER_SECOND 1000
#define MICROSECONDS_PER_SECOND 1000000
#define NANOSECONDS_PER_SECOND 1000000000

#define NANOSECONDS_PER_MILLISECOND 1000000
#define NANOSECONDS_PER_MICROSECOND 1000

typedef struct{
  struct timespec tsTime;
}Time;

static inline void Time_Boot(Time *pTime){
  clock_gettime(CLOCK_BOOTTIME, &pTime->tsTime);
}

static inline void Time_Process(Time *pTime){
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &pTime->tsTime);
}

static inline void Time_Thread(Time *pTime){
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &pTime->tsTime);
}

static inline void Time_Begin(Time *pTime){
  clock_gettime(CLOCK_MONOTONIC, &pTime->tsTime);
}

static inline void Time_End(Time *pTime){
  struct timespec tsEnd;
  clock_gettime(CLOCK_MONOTONIC, &tsEnd);

  I64 nSeconds = tsEnd.tv_sec - pTime->tsTime.tv_sec;
  I64 nNanoSeconds = tsEnd.tv_nsec - pTime->tsTime.tv_nsec;
  if(nNanoSeconds < 0){
    --nSeconds;
    nNanoSeconds += NANOSECONDS_PER_SECOND;
  }

  pTime->tsTime.tv_sec = nSeconds;
  pTime->tsTime.tv_nsec = nNanoSeconds;
}

static inline U64 Time_Seconds(Time *pTime){
  return pTime->tsTime.tv_sec;
}

static inline U64 Time_Milliseconds(Time *pTime){
  U64 nSecondPart = pTime->tsTime.tv_sec;
  nSecondPart *= MILLISECONDS_PER_SECOND;
  U64 nNanoPart = pTime->tsTime.tv_nsec;
  nNanoPart /= NANOSECONDS_PER_MILLISECOND;
  return nSecondPart + nNanoPart;
}

static inline U64 Time_Microseconds(Time *pTime){
  U64 nSecondPart = pTime->tsTime.tv_sec;
  nSecondPart *= MICROSECONDS_PER_SECOND;
  U64 nNanoPart = pTime->tsTime.tv_nsec;
  nNanoPart /= NANOSECONDS_PER_MICROSECOND;
  return nSecondPart + nNanoPart;
}

static inline U64 Time_Nanoseconds(Time *pTime){
  U64 nNanoseconds = pTime->tsTime.tv_sec;
  nNanoseconds *= NANOSECONDS_PER_SECOND;
  nNanoseconds += pTime->tsTime.tv_nsec;
  return nNanoseconds;
}

#endif //TIME_H
