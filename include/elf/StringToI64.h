#ifndef STRING_TO_I64_H
#define STRING_TO_I64_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

#include "Type.h"

//String to I64

inline
static E8 String_ParsePI64_10(I64 *piValue, const C **ppTail) {
  I64 iValue = *piValue;
  const C *p = *ppTail;
  U8 uRange;
  while ((uRange = *p - (C)'0') < 10) {
    if (iValue < 0x0CCCCCCCCCCCCCCC)
      iValue = iValue * 10 + uRange;
    else if (iValue == 0x0CCCCCCCCCCCCCCC && uRange < 8)
      iValue = 0x7FFFFFFFFFFFFFF8 + uRange;
    else {
      *ppTail = p;
      return 1;
    }
    ++p;
  }
  *ppTail = p;
  *piValue = iValue;
  return 0;
}

inline
static E8 String_ParseNI64_10(I64 *piValue, const C **ppTail) {
  I64 iValue = *piValue;
  const C *p = *ppTail;
  U8 uRange;
  while ((uRange = *p - (C)'0') < 10) {
    if (iValue > -0x0CCCCCCCCCCCCCCC)
      iValue = iValue * 10 - uRange;
    else if (iValue == -0x0CCCCCCCCCCCCCCC && uRange < 9)
      iValue = -0x7FFFFFFFFFFFFFF8 - (I64)uRange;
    else {
      *piValue = iValue;
      *ppTail = p;
      return 1;
    }
    ++p;
  }
  *piValue = iValue;
  *ppTail = p;
  return 0;
}

//Parse '[+/-]0x' prefix youself
inline
static E8 String_ParsePI64_16(I64 *piValue, const C **ppTail) {
  I64 iValue = *piValue;
  const C *p = *ppTail;
  while (1) {
    U8 uRange = *p - (C)'0';
    if (uRange > 9) {
      uRange &= 0xDF;
      uRange -= 0x11;
      if (uRange > 5)
        break;

      uRange += 10;
    }

    if (iValue > 0x07FFFFFFFFFFFFFF) {
      *ppTail = p;
      return 1;
    }

    iValue <<= 4;
    iValue |= uRange;
    ++p;
  }

  *ppTail = p;
  *piValue = iValue;
  return 0;
}

inline
static E8 String_ParseNI64_16(I64 *piValue, const C **ppTail) {
  I64 iValue = *piValue;
  const C *p = *ppTail;
  while (1) {
    U8 uRange = *p - (C)'0';
    if (uRange > 9) {
      uRange &= 0xDF;
      uRange -= 0x11;
      if (uRange > 5)
        break;

      uRange += 10;
    }

    if (iValue > -0x0800000000000000) {
      iValue <<= 4;
      iValue |= uRange;
    }
    else if (iValue == -0x0800000000000000 && !uRange)
      iValue = -0x8000000000000000;
    else {
      *piValue = iValue;
      *ppTail = p;
      return 1;
    }
    ++p;
  }

  *piValue = iValue;
  *ppTail = p;
  return 0;
}

#endif // STRING_TO_I64_H
