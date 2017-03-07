#ifndef VU61_H
#define VU61_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

#include "Type.h"

inline
static U8 VU61_TailBytes(const Byte *pVU61){
  return *pVU61 >> 5;
}

inline
static U8 VU61_Bytes(const Byte *pVU61){
  return 1 + VU61_TailBytes(pVU61);
}

//range: [0, 0x1FFFFFFFFFFFFFFF]
inline
static const Byte *VU61_ToU64(const Byte *pVU61, U64 *pU64){
  const U8 *p = pVU61;
  U64 uValue = *p++;
  if(uValue > 0x1F){
    U8 uTail = uValue >> 5;
    uValue &= 0x1F;
    do{
      uValue <<= 8;
      uValue |= *p++;
    }while(--uTail);
  }
  *pU64 = uValue;
  return p;
}

inline
static B VU61_Valid(U64 u64){
  return u64 < 0x2000000000000000;
}

inline
static B VU61_Invalid(U64 u64){
  return u64 > 0x1FFFFFFFFFFFFFFF;
}

//u5 must in range[0, 0x1F]. Check it youself!
inline
static Byte *VU61_FromU5(Byte *pVU61, U8 u5){
  U8 *p = pVU61;
  *p++ = u5;
  return p;
}

inline
static Byte *VU61_FromU8(Byte *pVU61, U8 u8){
  U8 *p = pVU61;
  if(u8 > 0x1F)
    *p++ = 0x20;

  *p++ = u8;
  return p;
}

inline
static Byte *VU61_FromU16(Byte *pVU61, U16 u16){
  U8 *p = pVU61;
  if(u16 > 0xFF){
    U8 u8 = u16 >> 8;
    if(u16 > 0x1FFF){
      *p++ = 0x40;
      *p++ = u8;
    }
    else
      *p++ = u8 | 0x20;
  }
  else if(u16 > 0x1F)
    *p++ = 0x20;

  *p++ = u16;
  return p;
}

inline
static Byte *VU61_FromU32(Byte *pVU61, U32 u32){
  U8 u8 = u32 >> 8;
  U8 *p = pVU61;
  if(u32 > 0x1FFF){
    if(u32 > 0x1FFFFF){
      if(u32 > 0x1FFFFFFF){
        *p++ = 0x80;
        *p++ = u32 >> 24;
      }
      else
        *p++ = u32 >> 24 | 0x60;

      *p++ = u32 >> 16;
    }
    else
      *p++ = u32 >> 16 | 0x40;

    *p++ = u8;
  }
  else  if(u32 > 0x1F)
    *p++ = u8 | 0x20;

  *p++ = u32;
  return p;
}

//u61 must in range[0, 0x1FFFFFFFFFFFFFFF]. Check it youself!
inline
static Byte *VU61_FromU61(Byte *pVU61, U64 u61){
  U8 u8 = u61 >> 8;
  U8 *p = pVU61;
  if(u61 > 0x1FFFFFFFULL){
    if(u61 > 0x1FFFFFFFFFFFULL){
      if(u61 > 0x1FFFFFFFFFFFFFULL){
        *p++ = u61 >> 56 | 0xE0;
        *p++ = u61 >> 48;
      }
      else
        *p++ = u61 >> 48 | 0xC0;

      *p++ = u61 >> 40;
      *p++ = u61 >> 32;
      *p++ = u61 >> 24;
      *p++ = u61 >> 16;
      *p++ = u8;
    }
    else{
      if(u61 > 0x1FFFFFFFFFULL){
        *p++ = u61 >> 40 | 0xA0;
        *p++ = u61 >> 32;
      }
      else
        *p++ = u61 >> 32 | 0x80;

      *p++ = u61 >> 24;
      *p++ = u61 >> 16;
      *p++ = u8;
    }
  }
  else{
    if(u61 > 0x1FFFULL){
      if(u61 > 0x1FFFFFULL){
        *p++ = u61 >> 24 | 0x60;
        *p++ = u61 >> 16;
      }
      else
        *p++ = u61 >> 16 | 0x40;

      *p++ = u8;
    }
    else  if(u61 > 0x1FULL)
      *p++ = u8 | 0x20;
  }
  *p++ = u61;
  return p;
}

//(pEnd - pBegin) > 0
//(pEnd - pBegin) SHOULD <= 0x20202020202020
inline
static Byte *VU61_Sum(Byte *pVU61, const Byte *pBegin, const Byte *pEnd){
  U64 uSum = 0;
  const Byte *p = pBegin;
  do
    uSum += *p++;
  while(p != pEnd);

  return VU61_FromU61(pVU61, uSum);
}

#endif //VU61_H

