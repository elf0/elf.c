#ifndef VU30_H
#define VU30_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

#include "Type.h"

inline
static U8 VU30_TailBytes(const Byte *pVU30){
  return *pVU30 >> 6;
}

inline
static U8 VU30_Bytes(const Byte *pVU30){
  return 1 + VU30_TailBytes(pVU30);
}

//range: [0, 0x3FFFFFFF]
inline
static const Byte *VU30_ToU32(const Byte *pVU30, U32 *pU32){
  const U8 *p = pVU30;
  U32 uValue = *p++;
  if(uValue > 0x3F){
    U8 uTail = uValue >> 6;
    uValue &= 0x3F;
    do{
      uValue <<= 8;
      uValue |= *p++;
    }while(--uTail);
  }
  *pU32 = uValue;
  return p;
}

inline
static B VU30_Valid(U32 u32){
  return u32 < 0x40000000;
}

inline
static B VU30_Invalid(U32 u32){
  return u32 > 0x3FFFFFFF;
}

//u6 must in range[0, 0x3F]. Check it youself!
inline
static Byte *VU30_FromU6(Byte *pVU30, U8 u6){
  U8 *p = pVU30;
  *p++ = u6;
  return p;
}

inline
static Byte *VU30_FromU8(Byte *pVU30, U8 u8){
  U8 *p = pVU30;

  if(u8 > 0x3F)
    *p++ = 0x40;

  *p++ = u8;
  return p;
}

inline
static Byte *VU30_FromU16(Byte *pVU30, U16 u16){
  U8 u8 = u16 >> 8;
  U8 *p = pVU30;
  if(u16 > 0x3FFF){
    *p++ = 0x80;
    *p++ = u8;
  }
  else  if(u16 > 0x3F)
    *p++ = u8 | 0x40;

  *p++ = u16;
  return p;
}

//u30 must in range[0, 0x3FFFFFFF]. Check it youself!
inline
static Byte *VU30_FromU30(Byte *pVU30, U32 u30){
  U8 u8 = u30 >> 8;
  U8 *p = pVU30;
  if(u30 > 0x3FFF){
    if(u30 > 0x3FFFFF){
      *p++ = u30 >> 24 | 0xC0;
      *p++ = u30 >> 16;
    }
    else
      *p++ = u30 >> 16 | 0x80;

    *p++ = u8;
  }
  else  if(u30 > 0x3F)
    *p++ = u8 | 0x40;

  *p++ = u30;
  return p;
}

//(pEnd - pBegin) > 0
//(pEnd - pBegin) SHOULD <= 0x404040
inline
static Byte *VU30_Sum(Byte *pVU30, const Byte *pBegin, const Byte *pEnd){
  U32 uSum = 0;
  const Byte *p = pBegin;
  do
    uSum += *p++;
  while(p != pEnd);

  return VU30_FromU30(pVU30, uSum);
}

#endif //VU30_H

