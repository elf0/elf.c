#ifndef STRING_TO_U64_H
#define STRING_TO_U64_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

#include "Type.h"

//String to U64

//From 'pBegin' to 'pEnd' MUST be a valid number string.
inline
static U64 String_ToU64_2(const C *p, const C *pEnd) {
  U64 uValue = *p++ - (C)'0';
  while (p != pEnd) {
    uValue <<= 1;
    uValue |= *p++ - (C)'0';
  }

  return uValue;
}

inline
static U64 String_ToU64_4(const C *p, const C *pEnd) {
  U64 uValue = *p++ - (C)'0';
  while (p != pEnd) {
    uValue <<= 2;
    uValue |= *p++ - (C)'0';
  }

  return uValue;
}

inline
static U64 String_ToU64_8(const C *p, const C *pEnd) {
  U64 uValue = *p++ - (C)'0';
  while (p != pEnd) {
    uValue <<= 3;
    uValue |= *p++ - (C)'0';
  }

  return uValue;
}

inline
static U64 String_ToU64_10(const C *p, const C *pEnd) {
  U64 uValue = *p++ - (C)'0';
  while (p != pEnd) {
    uValue *= 10;
    uValue += *p++ - (C)'0';
  }

  return uValue;
}

inline
static U64 String_ToU64_16(const C *p, const C *pEnd) {
  U64 uValue = *p++ - (C)'0';
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
static E8 String_ParseU64_2(U64 *puValue, const C **ppTail) {
  U64 uValue = *puValue;
  const C *p = *ppTail;
  U8 uRange;
  while ((uRange = *p - (C)'0') < 2) {
    if (uValue > 0x7FFFFFFFFFFFFFFF) {
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
static E8 String_ParseU64_4(U64 *puValue, const C **ppTail) {
  U64 uValue = *puValue;
  const C *p = *ppTail;
  U8 uRange;
  while ((uRange = *p - (C)'0') < 4) {
    if (uValue > 0x3FFFFFFFFFFFFFFF) {
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
static E8 String_ParseU64_8(U64 *puValue, const C **ppTail) {
  U64 uValue = *puValue;
  const C *p = *ppTail;
  U8 uRange;
  while ((uRange = *p - (C)'0') < 8) {
    if (uValue > 0x1FFFFFFFFFFFFFFF) {
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
static E8 String_ParseU64_10(U64 *puValue, const C **ppTail) {
  U64 uValue = *puValue;
  const C *p = *ppTail;
  U8 uRange;
  while ((uRange = *p - (C)'0') < 10) {
    if (uValue < 0x1999999999999999)
      uValue = uValue * 10 + uRange;
    else if (uValue == 0x1999999999999999 && uRange < 6)
      uValue = 0xFFFFFFFFFFFFFFFA + uRange;
    else {
      *ppTail = p;
      return 1;
    }
    ++p;
  }
  *ppTail = p;
  *puValue = uValue;
  return 0;
}

//Parse '0x' prefix youself
inline
static E8 String_ParseU64_16(U64 *puValue, const C **ppTail) {
  U64 uValue = *puValue;
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

    if (uValue > 0x0FFFFFFFFFFFFFFF) {
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

// return store bits
inline
static U8 U64_Bits(U64 uValue) {
  U8 uBits;
  if (uValue > 0xFFFFFFFF) {
    if (uValue > 0xFFFFFFFFFFFF) {
      if (uValue > 0xFFFFFFFFFFFFFF)
        uBits = 63;
      else
        uBits = 55;
    }
    else if (uValue > 0xFFFFFFFFFF)
      uBits = 47;
    else
      uBits = 39;
  }
  else if (uValue > 0xFFFF) {
    if (uValue > 0xFFFFFF)
      uBits = 31;
    else
      uBits = 23;
  }
  else if (uValue > 0xFF)
    uBits = 15;
  else
    uBits = 7;

  do {
    if (uValue >> uBits)
      return uBits + 1;
  } while (--uBits);
  return 1;
}

inline
static U8 String_ParseU64Bits_2(U64 uValue, const C **ppTail, U64 *puValue) {
  if (String_ParseU64_2(uValue, ppTail, puValue))
    return 255;

  return U64_Bits(*puValue);
}

inline
static U8 String_ParseU64Bits_4(U64 uValue, const C **ppTail, U64 *puValue) {
  if (String_ParseU64_4(uValue, ppTail, puValue))
    return 255;

  return U64_Bits(*puValue);
}

inline
static U8 String_ParseU64Bits_8(U64 uValue, const C **ppTail, U64 *puValue) {
  if (String_ParseU64_8(uValue, ppTail, puValue))
    return 255;

  return U64_Bits(*puValue);
}

inline
static U8 String_ParseU64Bits_10(U64 uValue, const C **ppTail, U64 *puValue) {
  if (String_ParseU64_10(uValue, ppTail, puValue))
    return 255;

  return U64_Bits(*puValue);
}

inline
static U8 String_ParseU64Bits_16(U64 uValue, const C **ppTail, U64 *puValue) {
  if (String_ParseU64_16(uValue, ppTail, puValue))
    return 255;

  return U64_Bits(*puValue);
}

#endif // STRING_TO_U64_H
