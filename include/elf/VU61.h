#ifndef VU61_H
#define VU61_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

#include "Type.h"

static inline U8 VU61_TailBytes(const Byte *pVU61){
  return *pVU61 >> 5;
}

static inline U8 VU61_Bytes(const Byte *pVU61){
  return 1 + VU61_TailBytes(pVU61);
}

//range: [0, 0x1FFFFFFFFFFFFFFF]
static inline const Byte *VU61_ToU64(const Byte *pVU61, U64 *pU64){
  const U8 *p = pVU61;
  U64 uValue = *p & 0x1F;
  U8 uTail = VU61_TailBytes(p++);

  while(uTail--)
    uValue = (uValue << 8) | *p++;

  *pU64 = uValue;
  return p;
}

static inline B VU61_Valid(U64 u64){
  return u64 < 0x2000000000000000;
}

static inline B VU61_Invalid(U64 u64){
  return u64 > 0x1FFFFFFFFFFFFFFF;
}

static inline Byte *VU61_FromU8(Byte *pVU61, U8 u8){
  U8 *p = pVU61;

  if(u8 & 0xE0)
    *p++ = 0x20;

  *p++ = u8;
  return p;
}

static inline Byte *VU61_FromU16(Byte *pVU61, U16 u16){
  U8 *p = pVU61;

  if(u16 & 0xFF00){
    if(u16 & 0xE000){
      *p++ = 0x40;
      *p++ = u16 >> 8;
    }
    else
      *p++ = u16 >> 8 | 0x20;
  }
  else if(u16 & 0xE0)
    *p++ = 0x20;

  *p++ = u16;
  return p;
}

static inline Byte *VU61_FromU32(Byte *pVU61, U32 u32){
  U8 *p = pVU61;
  if(u32 & 0xFFFF0000){
    if(u32 & 0xFF000000){
      if(u32 & 0xE0000000){
        *p++ = 0x80;
        *p++ = u32 >> 24;
      }
      else
        *p++ = u32 >> 24 | 0x60;

      *p++ = u32 >> 16;
    }
    else{
      if(u32 & 0xE00000){
        *p++ = 0x60;
        *p++ = u32 >> 16;
      }
      else
        *p++ = u32 >> 16 | 0x40;
    }

    *p++ = u32 >> 8;
  }
  else if(u32 & 0xFF00){
    if(u32 & 0xE000){
      *p++ = 0x40;
      *p++ = u32 >> 8;
    }
    else
      *p++ = u32 >> 8 | 0x20;
  }
  else if(u32 & 0xE0)
    *p++ = 0x20;

  *p++ = u32;
  return p;
}

//u64 must in range[0, 0x1FFFFFFFFFFFFFFF]. Check it youself!
static inline Byte *VU61_FromU64(Byte *pVU61, U64 u64){
  U8 *p = pVU61;
  if(u64 & 0xFFFFFFFF00000000){
    if(u64 & 0xFFFF000000000000){
      if(u64 & 0xFF00000000000000){
        *p++ = u64 >> 56 | 0xE0;
      }
      else{
        if(u64 & 0xE0000000000000){
          *p++ = 0xE0;
          *p++ = u64 >> 48;
        }
        else
          *p++ = u64 >> 48 | 0xC0;
      }

      *p++ = u64 >> 40;
      *p++ = u64 >> 32;
    }
    else if(u64 & 0xFF0000000000){
      if(u64 & 0xE00000000000){
        *p++ = 0xC0;
        *p++ = u64 >> 40;
      }
      else
        *p++ = u64 >> 40 | 0xA0;

      *p++ = u64 >> 32;
    }
    else{
      if(u64 & 0xE000000000){
        *p++ = 0xA0;
        *p++ = u64 >> 32;
      }
      else
        *p++ = u64 >> 32 | 0x80;
    }

    *p++ = u64 >> 24;
    *p++ = u64 >> 16;
    *p++ = u64 >> 8;
  }
  else if(u64 & 0xFFFF0000){
    if(u64 & 0xFF000000){
      if(u64 & 0xE0000000){
        *p++ = 0x80;
        *p++ = u64 >> 24;
      }
      else
        *p++ = u64 >> 24 | 0x60;

      *p++ = u64 >> 16;
    }
    else{
      if(u64 & 0xE00000){
        *p++ = 0x60;
        *p++ = u64 >> 16;
      }
      else
        *p++ = u64 >> 16 | 0x40;
    }

    *p++ = u64 >> 8;
  }
  else if(u64 & 0xFF00){
    if(u64 & 0xE000){
      *p++ = 0x40;
      *p++ = u64 >> 8;
    }
    else
      *p++ = u64 >> 8 | 0x20;
  }
  else if(u64 & 0xE0)
    *p++ = 0x20;

  *p++ = u64;
  return p;
}

#endif //VU61_H

