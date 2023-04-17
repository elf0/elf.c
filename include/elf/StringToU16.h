#ifndef STRING_TO_U16_H
#define STRING_TO_U16_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

#include "Type.h"

//String to U16

//From 'pBegin' to 'pEnd' MUST be a valid number string.
inline
static U16 String_ToU16_2(const C *pBegin, const C *pEnd) {
  const C *p = pBegin;
  U16 uValue = *p++ - (C)'0';
  while (p != pEnd) {
    uValue <<= 1;
    uValue |= *p++ - (C)'0';
  }

  return uValue;
}

inline
static U16 String_ToU16_4(const C *pBegin, const C *pEnd) {
  const C *p = pBegin;
  U16 uValue = *p++ - (C)'0';
  while (p != pEnd) {
    uValue <<= 2;
    uValue |= *p++ - (C)'0';
  }

  return uValue;
}

inline
static U16 String_ToU16_8(const C *pBegin, const C *pEnd) {
  const C *p = pBegin;
  U16 uValue = *p++ - (C)'0';
  while (p != pEnd) {
    uValue <<= 3;
    uValue |= *p++ - (C)'0';
  }

  return uValue;
}

inline
static U16 String_ToU16_10(const C *pBegin, const C *pEnd) {
  const C *p = pBegin;
  U16 uValue = *p++ - (C)'0';
  while (p != pEnd) {
    uValue *= 10;
    uValue += *p++ - (C)'0';
  }

  return uValue;
}

inline
static U16 String_ToU16_16(const C *pBegin, const C *pEnd) {
  const C *p = pBegin;
  U16 uValue = *p++ - (C)'0';
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
static E8 String_ParseU16_2(U16 uValue, const C **ppTail, U16 *puValue) {
  U8 uRange;
  while ((uRange = *p - '0') < 2) {
    if (uValue > 0x7FFF) {
      *ppTail = p;
      return 1;
    }

    uValue <<= 1;
    uValue |= uRange;
    ++p;
  }

  *ppTail = p;
  *puValue = uValue;
  return 0;type
}

inline
static E8 String_ParseU16_4(U16 uValue, const C **ppTail, U16 *puValue) {
  U8 uRange;
  while ((uRange = *p - '0') < 4) {
    if (uValue > 0x3FFF) {
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
static E8 String_ParseU16_8(U16 uValue, const C **ppTail, U16 *puValue) {
  const C *p = *ppTail;
  U8 uRange;
  while ((uRange = *p - '0') < 8) {
    if (uValue > 0x1FFF) {
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

inline
static E8 String_ParseU16_10(U16 uValue, const C **ppTail, U16 *puValue) {
  const C *p = *ppTail;
  U8 uRange;
  while ((uRange = *p - '0') < 10) {
    if (uValue < 0x1999) {
      uValue *= 10;
      uValue += uRange;
    }
    else if (uValue > 0x1999) {
      *ppTail = p;
      return 1;
    }
    else {
      if (uRange > 5) {
        *ppTail = p;
        return 1;
      }
      uValue = 0xFFFA;
      uValue += uRange;
    }
    ++p;
  }
  *ppTail = p;
  *puValue = uValue;
  return 0;
}

//Parse '0x' prefix youself
inline
static E8 String_ParseU16_16(U16 uValue, const C **ppTail, U16 *puValue) {
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

    if (uValue > 0xFFF) {
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

#endif // STRING_TO_U16_H
