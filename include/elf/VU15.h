#ifndef VU15_H
#define VU15_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

#include "Type.h"

inline
static U8 VU15_TailBytes(const Byte *pVU15){
  return *pVU15 >> 7;
}

inline
static U8 VU15_Bytes(const Byte *pVU15){
  return 1 + VU15_TailBytes(pVU15);
}

//range: [0, 0x7FFF]
inline
static const Byte *VU15_ToU16(const Byte *pVU15, U16 *pU16){
  const U8 *p = pVU15;
  U16 uValue = *p++;
  if(uValue > 0x7F){
    uValue &= 0x7F;
    uValue <<= 8;
    uValue |= *p++;
  }

  *pU16 = uValue;
  return p;
}

inline
static B VU15_Valid(U16 u16){
  return u16 < 0x8000;
}

inline
static B VU15_Invalid(U16 u16){
  return u16 > 0x7FFF;
}

//u7 must in range[0, 0x7F]. Check it youself!
inline
static Byte *VU15_FromU7(Byte *pVU15, U8 u7){
  U8 *p = pVU15;
  *p++ = u7;
  return p;
}

inline
static Byte *VU15_FromU8(Byte *pVU15, U8 u8){
  U8 *p = pVU15;
  if(u8 > 0x7F)
    *p++ = 0x80;

  *p++ = u8;
  return p;
}

//u15 must in range[0, 0x7FFF]. Check it youself!
inline
static Byte *VU15_FromU15(Byte *pVU15, U16 u15){
  U8 *p = pVU15;
  if(u15 > 0x7F)
    *p++ = (u15 >> 8) | 0x80;

  *p++ = u15;
  return p;
}

//(pEnd - pBegin) > 0
//(pEnd - pBegin) SHOULD <= 0x80
inline
static Byte *VU15_Sum(Byte *pVU15, const Byte *pBegin, const Byte *pEnd){
  U16 uSum = 0;
  const Byte *p = pBegin;
  do
    uSum += *p++;
  while(p != pEnd);

  return VU15_FromU15(pVU15, uSum);
}

#endif //VU15_H

