#ifndef STRING_TO_U32_H
#define STRING_TO_U32_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

#include "Type.h"

//String to U32

//From 'pBegin' to 'pEnd' MUST be a valid number string.
inline
static U32 String_ToU32_2(const C *pBegin, const C *pEnd) {
  const C *p = pBegin;
  U32 uValue = *p++ - (C)'0';
  while (p != pEnd) {
    uValue <<= 1;
    uValue |= *p++ - (C)'0';
  }

  return uValue;
}

inline
static U32 String_ToU32_4(const C *pBegin, const C *pEnd) {
  const C *p = pBegin;
  U32 uValue = *p++ - (C)'0';
  while (p != pEnd) {
    uValue <<= 2;
    uValue |= *p++ - (C)'0';
  }

  return uValue;
}

inline
static U32 String_ToU32_8(const C *pBegin, const C *pEnd) {
  const C *p = pBegin;
  U32 uValue = *p++ - (C)'0';
  while (p != pEnd) {
    uValue <<= 3;
    uValue |= *p++ - (C)'0';
  }

  return uValue;
}

inline
static U32 String_ToU32_10(const C *pBegin, const C *pEnd) {
  const C *p = pBegin;
  U32 uValue = *p++ - (C)'0';
  while (p != pEnd) {
    uValue *= 10;
    uValue += *p++ - (C)'0';
  }

  return uValue;
}

inline
static U32 String_ToU32_16(const C *pBegin, const C *pEnd) {
  const C *p = pBegin;
  U32 uValue = *p++ - (C)'0';
  if (uValue > 9) {
    uValue &= 0xDF;
    uValue -= 0x11;
  }

  while (p != pEnd) {
    uValue <<= 4;
    U8 u8 = *p++ - (C)'0';
    if (u8 > 9) {
      u8 &= 0xDF;
      u8 -= 0x11;
    }

    uValue |= u8;

  }

  return uValue;
}

//Parse '0b' prefix youself
inline
static E8 String_ParseU32_2(U32 uValue, const C **ppTail, U32 *puValue) {
  U8 uRange;
  while ((uRange = *p - '0') < 2) {
    if (uValue > 0x7FFFFFFF) {
      *ppTail = p;
      return 1;
    }

    uValue <<= 1;
    uValue |= uRange;
    ++p;
  }

  *ppTail = p;
  *puValue = uValue;
  return 0;
}

inline
static E8 String_ParseU32_4(U32 uValue, const C **ppTail, U32 *puValue) {
  U8 uRange;
  while ((uRange = *p - '0') < 4) {
    if (uValue > 0x3FFFFFFF) {
      *ppTail = p;
      return 1;
    }

    uValue <<= 2;
    uValue |= uRange;
    ++p;
  }

  *ppTail = p;
  *puValue = uValue;
  return 0;
}

//Parse '0o' prefix youself
inline
static E8 String_ParseU32_8(U32 uValue, const C **ppTail, U32 *puValue) {
  const C *p = *ppTail;
  U8 uRange;
  while ((uRange = *p - '0') < 8) {
    if (uValue > 0x1FFFFFFF) {
      *ppTail = p;
      return 1;
    }

    uValue <<= 3;
    uValue |= uRange;
    ++p;
  }

  *ppTail = p;
  *puValue = uValue;
  return 0;
}

inline static E8 String_ParseU32_Max( U32 *pValue, const C **ppTail, U32 uMax ) {
    E8 e = 0;
    U32 uValue = *pValue;
    const C *p = *ppTail;
    const U32 uDiv = uMax / 10;
    const U32 uMod = uMax % 10;
    const U32 uSub = uMax - uMod;
    U8 uRange;
    while ((uRange = *p - '0') < 10) {
        if (uValue < uDiv)
            uValue = uValue * 10 + uRange;
        else if (uValue == uDiv && uRange <= uMod)
            uValue = uSub + uRange;
        else {
            e = 1;
            break;
        }
        ++p;
    }
    *pValue = uValue;
    *ppTail = p;
    return e;
}

inline static E8 String_ParseU32(U32 *puHead, const C **ppTail) {
    return String_ParseU32_Max(pValue, ppTail, U32_MAX);
}

//Parse '0x' prefix and first digit youself
inline
static E8 String_ParseU32_16(U32 *puValue, const C **ppTail) {
  U32 uValue = *puValue;
  const C *p = *ppTail;
  while (1) {
    U8 uRange = *p - '0';
    if (uRange > 9) {
      uRange &= 0xDF;
      uRange -= 0x11;
      if (uRange > 5)
        break;

      uRange += 10;
    }

    if (uValue > 0xFFFFFFF) {
      *ppTail = p;
      return 1;
    }

    uValue <<= 4;
    uValue |= uRange;
    ++p;
  }

  *ppTail = p;
  *puValue = uValue;
  return 0;
}

inline
static E8 String_ParseU32_36(U32 uValue, const C **ppTail, U32 *puValue) {
  const C *p = *ppTail;
  while (1) {
    U8 uRange = *p - '0';
    if (uRange > 9) {
      uRange |= 0x20;
      uRange -= 0x31;
      if (uRange > 25)
        break;

      uRange += 10;
    }

    if (uValue < 0x71C71C7) {
      uValue *= 0x24;
      uValue += uRange;
    }
    else if (uValue > 0x71C71C7) {
      *ppTail = p;
      return 1;
    } else {
      if (uRange > 3) {
        *ppTail = p;
        return 1;
      }
      uValue = 0xFFFFFFFC + uRange;
    }
    ++p;
  }

  *ppTail = p;
  *puValue = uValue;
  return 0;
}

#endif // STRING_TO_U32_H
