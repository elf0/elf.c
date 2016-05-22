#ifndef VU30_H
#define VU30_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

#include "Type.h"

static inline U8 VU30_TailBytes(const Byte *pVU30){
  return *pVU30 >> 6;
}

static inline U8 VU30_Bytes(const Byte *pVU30){
  return 1 + VU30_TailBytes(pVU30);
}

//range: [0, 0x3FFFFFFF]
static inline const Byte *VU30_ToU32(const Byte *pVU30, U32 *pU32){
  const U8 *p = pVU30;
  U32 uValue = *p & 0x3F;
  U8 uTail = VU30_TailBytes(p++);

  while(uTail--)
    uValue = (uValue << 8) | *p++;

  *pU32 = uValue;
  return p;
}

static inline Bool VU30_Valid(U32 u32){
  return u32 < 0x40000000;
}

static inline Bool VU30_Invalid(U32 u32){
  return u32 > 0x3FFFFFFF;
}

static inline Byte *VU30_FromU8(Byte *pVU30, U8 u8){
  U8 *p = pVU30;

  if(u8 & 0xC0)
    *p++ = 0x40;

  *p++ = u8;
  return p;
}

static inline Byte *VU30_FromU16(Byte *pVU30, U16 u16){
  U8 *p = pVU30;

  if(u16 & 0xFF00){
    if(u16 & 0xC000){
      *p++ = 0x80;
      *p++ = u16 >> 8;
    }
    else
      *p++ = u16 >> 8 | 0x40;
  }
  else if(u16 & 0xC0)
    *p++ = 0x40;

  *p++ = u16;
  return p;
}

//u32 must in range[0, 0x3FFFFFFF]. Check it youself!
static inline Byte *VU30_FromU32(Byte *pVU30, U32 u32){
  U8 *p = pVU30;
  if(u32 & 0xFFFF0000){
    if(u32 & 0xFF000000){
      *p++ = u32 >> 24 | 0xC0;
      *p++ = u32 >> 16;
    }
    else{
      if(u32 & 0xC00000){
        *p++ = 0xC0;
        *p++ = u32 >> 16;
      }
      else
        *p++ = u32 >> 16 | 0x80;
    }

    *p++ = u32 >> 8;
  }
  else if(u32 & 0xFF00){
    if(u32 & 0xC000){
      *p++ = 0x80;
      *p++ = u32 >> 8;
    }
    else
      *p++ = u32 >> 8 | 0x40;
  }
  else if(u32 & 0xC0)
    *p++ = 0x40;

  *p++ = u32;
  return p;
}

#endif //VU30_H

