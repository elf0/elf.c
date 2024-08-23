#ifndef VU23_H
#define VU23_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

#include "Type.h"

inline
static U8 VU23_TailBytes(const Byte *pVU23) {
  return *pVU23 > 0x7F? 2 : 0;
}

inline
static U8 VU23_Bytes(const Byte *pVU23) {
  return 1 + VU23_TailBytes(pVU23);
}

//range: [0, 0x7FFFFF]
inline
static const Byte *VU23_ToU32(const Byte *pVU23, U32 *pU32) {
  const U8 *p = pVU23;
  U32 uValue = *p++;
  if(uValue > 0x7F) {
    uValue &= 0x7F;
    uValue <<= 8;
    uValue |= *p++;
    uValue <<= 8;
    uValue |= *p++;
  }
  *pU32 = uValue;
  return p;
}

inline
static B VU23_Valid(U32 u32) {
  return u32 < 0x800000;
}

inline
static B VU23_Invalid(U32 u32) {
  return u32 > 0x7FFFFF;
}

//u7 must in range[0, 0x7F]. Check it youself!
inline
static Byte *VU23_FromU7(Byte *pVU23, U8 u7) {
  U8 *p = pVU23;
  *p++ = u7;
  return p;
}

inline
static Byte *VU23_FromU8(Byte *pVU23, U8 u8) {
  U8 *p = pVU23;
  if(u8 > 0x7F) {
    *p++ = 0x80;
    *p++ = 0;
  }

  *p++ = u8;
  return p;
}

inline
static Byte *VU23_FromU16(Byte *pVU23, U16 u16) {
  U8 *p = pVU23;
  if(u16 > 0x7F) {
    *p++ = 0x80;
    *p++ = u16 >> 8;
  }

  *p++ = u16;
  return p;
}

//u23 must in range[0, 0x7FFFFF]. Check it youself!
inline
static Byte *VU23_FromU23(Byte *pVU23, U32 u23) {
  U8 *p = pVU23;
  if(u23 > 0x7F) {
    *p++ = 0x80 | (u23 >> 16);
    *p++ = u23 >> 8;
  }

  *p++ = u23;
  return p;
}

//(pEnd - pBegin) > 0
//(pEnd - pBegin) SHOULD <= 0x8080
inline
static Byte *VU23_Sum(Byte *pVU23, const Byte *pBegin, const Byte *pEnd) {
  U32 uSum = 0;
  const Byte *p = pBegin;
  do
    uSum += *p++;
  while(p != pEnd);

  return VU23_FromU23(pVU23, uSum);
}

#endif //VU23_H

