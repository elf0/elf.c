#ifndef U128_H
#define U128_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

#include "Type.h"

#if (__GNUC__ > 4 && __GNUC_MINOR__ > 1) || defined(__clang__)
#define U128_USE_BUILTIN
#endif

inline
static E8 U128_Add(U64 uLeftH, U64 uLeftL, U64 uRightH, U64 uRightL, U64 *puResultH, U64 *puResultL){
  U64 uResultH, uResultL;
#ifdef U128_USE_BUILTIN
  //  fprintf(stderr, "__builtin_uaddll_overflow\n");
  if(__builtin_uaddll_overflow(uLeftH, uRightH, &uResultH))
    return 1;

  if(__builtin_uaddll_overflow(uLeftL, uRightL, &uResultL)){
    if(__builtin_uaddll_overflow(uResultH, 1, &uResultH))
      return 1;
  }
#else
  uResultH = uLeftH + uRightH;
  if(uResultH < uLeftH)
    return 1;

  uResultL = uLeftL + uRightL;
  if(uResultL < uLeftL){
    if(uResultH == 0xFFFFFFFFFFFFFFFFULL)
      return 1;
    ++uResultH;
  }
#endif
  *puResultH = uResultH;
  *puResultL = uResultL;
  return 0;
}

inline
static E8 U128_Sub(U64 uLeftH, U64 uLeftL, U64 uRightH, U64 uRightL, U64 *puResultH, U64 *puResultL){
  U64 uResultH, uResultL;
#ifdef U128_USE_BUILTIN
  //  fprintf(stderr, "__builtin_usubll_overflow\n");
  if(__builtin_usubll_overflow(uLeftH, uRightH, &uResultH))
    return 1;

  if(__builtin_usubll_overflow(uLeftL, uRightL, &uResultL)){
    if(uResultH){
      --uResultH;
      ++uResultL;
      uResultL += 0xFFFFFFFFFFFFFFFF;
    }
    else
      return 1;
  }
#else
  if(uLeftH < uRightH)
    return 1;

  uResultH = uLeftH - uRightH;
  uResultL = uLeftL - uRightL;
  if(uLeftL < uRightL){
    if(uResultH){
      --uResultH;
      ++uResultL;
    }
    else
      return 1;
    uResultL += 0xFFFFFFFFFFFFFFFF;
  }
#endif
  *puResultH = uResultH;
  *puResultL = uResultL;
  return 0;
}
#endif //U128_H
