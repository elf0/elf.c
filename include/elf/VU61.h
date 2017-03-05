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
    if(u16 > 0x1FFF){
      *p++ = 0x40;
      *p++ = u16 >> 8;
    }
    else
      *p++ = u16 >> 8 | 0x20;
  }
  else if(u16 > 0x1F)
    *p++ = 0x20;

  *p++ = u16;
  return p;
}

inline
static Byte *VU61_FromU32(Byte *pVU61, U32 u32){
  U8 *p = pVU61;
  if(u32 > 0xFFFF){
    if(u32 > 0xFFFFFF){
      if(u32 > 0x1FFFFFFF){
        *p++ = 0x80;
        *p++ = u32 >> 24;
      }
      else
        *p++ = u32 >> 24 | 0x60;

      *p++ = u32 >> 16;
    }
    else{
      if(u32 > 0x1FFFFF){
        *p++ = 0x60;
        *p++ = u32 >> 16;
      }
      else
        *p++ = u32 >> 16 | 0x40;
    }

    *p++ = u32 >> 8;
  }
  else if(u32 > 0xFF){
    if(u32 > 0x1FFF){
      *p++ = 0x40;
      *p++ = u32 >> 8;
    }
    else
      *p++ = u32 >> 8 | 0x20;
  }
  else if(u32 > 0x1F)
    *p++ = 0x20;

  *p++ = u32;
  return p;
}

//u61 must in range[0, 0x1FFFFFFFFFFFFFFF]. Check it youself!
inline
static Byte *VU61_FromU61(Byte *pVU61, U64 u61){
  U8 u8;
  U8 *p = pVU61;
  if(u61 > 0xFFFFFFFF){
    if(u61 > 0xFFFFFFFFFFFF){
      u8 = u61 >> 48;
      if(u61 > 0xFFFFFFFFFFFFFF){
        *p++ = u61 >> 56 | 0xE0;
        *p++ = u8;
      }
      else{
        if(u61 > 0x1FFFFFFFFFFFFF){
          *p++ = 0xE0;
          *p++ = u8;
        }
        else
          *p++ = u8 | 0xC0;
      }

      *p++ = u61 >> 40;
      *p++ = u61 >> 32;
    }
    else{
      if(u61 > 0xFFFFFFFFFF){
        u8 = u61 >> 40;
        if(u61 > 0x1FFFFFFFFFFF){
          *p++ = 0xC0;
          *p++ = u8;
        }
        else
          *p++ = u8 | 0xA0;

        *p++ = u61 >> 32;
      }
      else{
        u8 = u61 >> 32;
        if(u61 > 0x1FFFFFFFFF){
          *p++ = 0xA0;
          *p++ = u8;
        }
        else
          *p++ = u8 | 0x80;
      }
    }

    *p++ = u61 >> 24;
    *p++ = u61 >> 16;
    *p++ = u61 >> 8;
  }
  else if(u61 > 0xFFFF){
    if(u61 > 0xFFFFFF){
      u8 = u61 >> 24;
      if(u61 > 0x1FFFFFFF){
        *p++ = 0x80;
        *p++ = u8;
      }
      else
        *p++ = u8 | 0x60;

      *p++ = u61 >> 16;
    }
    else{
      u8 = u61 >> 16;
      if(u61 > 0x1FFFFF){
        *p++ = 0x60;
        *p++ = u8;
      }
      else
        *p++ = u8 | 0x40;
    }

    *p++ = u61 >> 8;
  }
  else if(u61 > 0xFF){
    u8 = u61 >> 8;
    if(u61 > 0x1FFF){
      *p++ = 0x40;
      *p++ = u8;
    }
    else
      *p++ = u8 | 0x20;
  }
  else if(u61 > 0x1F)
    *p++ = 0x20;

  *p++ = u61;
  return p;
}

//(pEnd - pBegin) SHOULD <= 0x20202020202020
inline
static Byte *VU61_Sum(Byte *pVU61, const Byte *pBegin, const Byte *pEnd){
  U64 uSum = 0;
  const Byte *p = pBegin;
  while(p != pEnd)
    uSum += *p++;

  return VU61_FromU61(pVU61, uSum);
}

#endif //VU61_H

