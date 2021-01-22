#ifndef VU61_H
#define VU61_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

#include "Type.h"

inline
static U8 VU61_TailBytes(const Byte *pVU61) {
  return *pVU61 >> 5;
}

inline
static U8 VU61_Bytes(const Byte *pVU61) {
  return 1 + VU61_TailBytes(pVU61);
}

//range: [0, 0x1FFFFFFFFFFFFFFF]
inline
static U64 VU61_Read(const Byte **ppVU61) {
  const Byte *p = *ppVU61;
  U64 uValue = *p++;
  if (uValue > 0x1F) {
    const Byte *pEnd = p + (uValue >> 5);
    uValue &= 0x1F;
    do {
      uValue <<= 8;
      uValue |= *p++;
    } while (p != pEnd);
  }
  *ppVU61 = p;
  return uValue;
}

inline
static B VU61_Valid(U64 u64) {
  return u64 < 0x2000000000000000;
}

inline
static B VU61_Invalid(U64 u64) {
  return u64 > 0x1FFFFFFFFFFFFFFF;
}

//u5 must in range[0, 0x1F]. Check it youself!
inline
static Byte *VU61_FromU5(Byte *pVU61, U8 u5) {
  U8 *p = pVU61;
  *p++ = u5;
  return p;
}

inline
static Byte *VU61_FromU8(Byte *pVU61, U8 u8) {
  U8 *p = pVU61;
  if(u8 > 0x1F)
    *p++ = 0x20;

  *p++ = u8;
  return p;
}

inline
static Byte *VU61_FromU16(Byte *pVU61, U16 u16) {
  U8 *p = pVU61;
  if(u16 > 0xFF) {
    U8 u8_8 = u16 >> 8;
    if(u16 > 0x1FFF)
      *p++ = 0x40;
    else
      u8_8 |= 0x20;

    *p++ = u8_8;
  }
  else if(u16 > 0x1F)
    *p++ = 0x20;

  *p++ = u16;
  return p;
}

inline
static Byte *VU61_FromU32(Byte *pVU61, U32 u32) {
  U8 *p = pVU61;
  if(u32 > 0x1FFF) {
    U8 u8_16 = u32 >> 16;
    if(u32 > 0x1FFFFF) {
      U8 u8_24 = u32 >> 24;
      if(u32 > 0x1FFFFFFF)
        *p++ = 0x80;
      else
        u8_24 |= 0x60;

      *p++ = u8_24;
    }
    else
      u8_16 |= 0x40;

    *p++ = u8_16;
    *p++ = u32 >> 8;
  }
  else if(u32 > 0x1F)
    *p++ = 0x20 | (u32 >> 8);

  *p++ = u32;
  return p;
}

//u61 must in range[0, 0x1FFFFFFFFFFFFFFF]. Check it youself!
inline
static Byte *VU61_FromU61(Byte *pVU61, U64 u61) {
  U8 *p = pVU61;
  if(u61 > 0x1FFFFFFFULL) {
    U8 u8_32 = u61 >> 32;
    if(u61 > 0x1FFFFFFFFFFFULL) {
      U8 u8_48 = u61 >> 48;
      if(u61 > 0x1FFFFFFFFFFFFFULL)
        *p++ = u61 >> 56 | 0xE0;
      else
        u8_48 |= 0xC0;

      *p++ = u8_48;
      *p++ = u61 >> 40;
    }
    else if(u61 > 0x1FFFFFFFFFULL)
      *p++ = 0xA0 | (u61 >> 40);
    else
      u8_32 |= 0x80;

    *p++ = u8_32;
    *p++ = u61 >> 24;
    *p++ = u61 >> 16;
    *p++ = u61 >> 8;
  }
  else if(u61 > 0x1FFFULL) {
    U8 u8_16 = u61 >> 16;
    if(u61 > 0x1FFFFFULL)
      *p++ = u61 >> 24 | 0x60;
    else
      u8_16 |= 0x40;

    *p++ = u8_16;
    *p++ = u61 >> 8;
  }
  else if(u61 > 0x1FULL)
    *p++ = 0x20 | (u61 >> 8);

  *p++ = u61;
  return p;
}

//(pEnd - pBegin) > 0
//(pEnd - pBegin) SHOULD <= 0x20202020202020
inline
static Byte *VU61_Sum(Byte *pVU61, const Byte *pBegin, const Byte *pEnd) {
  U64 uSum = 0;
  const Byte *p = pBegin;
  do
    uSum += *p++;
  while(p != pEnd);

  return VU61_FromU61(pVU61, uSum);
}

#endif //VU61_H

