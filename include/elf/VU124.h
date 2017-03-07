#ifndef VU124_H
#define VU124_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

#include "Type.h"

inline
static U8 VU124_TailBytes(const Byte *pVU124){
  return *pVU124 >> 4;
}

inline
static U8 VU124_Bytes(const Byte *pVU124){
  return 1 + VU124_TailBytes(pVU124);
}

//VU124 range: [0, 0xFFFFFFFFFFFFFFFF]. Check it youself!
inline
static const Byte *VU124_ToU64(const Byte *pVU124, U64 *pU64){
  const U8 *p = pVU124;
  U64 uValue = *p++;
  if(uValue > 0xF){
    U8 uTail = uValue >> 4;
    uValue &= 0xF;
    do{
      uValue <<= 8;
      uValue |= *p++;
    }while(--uTail);
  }

  *pU64 = uValue;
  return p;
}

//range: [0, 0x0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF]
inline
static const Byte *VU124_ToU124(const Byte *pVU124, U64 *puHigh, U64 *puLow){
  const U8 *p = pVU124;
  U64 uValue = *p++;
  if(uValue > 0xF){
    U8 uTail = uValue >> 4;
    uValue &= 0xF;
    if(uTail > 7){
      U8 uHT = uTail - 8;
      uTail = 8;
      while(uHT--){
        uValue <<= 8;
        uValue |= *p++;
      }
      *puHigh = uValue;
      uValue = 0;
    }

    do{
      uValue <<= 8;
      uValue |= *p++;
    }while(--uTail);
  }
  else
    *puHigh = 0;

  *puLow = uValue;
  return p;
}

inline
static B VU124_Valid(U64 uHigh){
  return uHigh < 0x10000000;
}

inline
static B VU124_Invalid(U64 uHigh){
  return uHigh > 0x0FFFFFFF;
}

//u4 must in range[0, 0x0F]. Check it youself!
inline
static Byte *VU124_FromU4(Byte *pVU124, U8 u4){
  U8 *p = pVU124;
  *p++ = u4;
  return p;
}

inline
static Byte *VU124_FromU8(Byte *pVU124, U8 u8){
  U8 *p = pVU124;
  if(u8 > 0x0F)
    *p++ = 0x10;

  *p++ = u8;
  return p;
}

inline
static Byte *VU124_FromU16(Byte *pVU124, U16 u16){
  U8 *p = pVU124;
  if(u16 > 0xFF){
    U8 u8 = u16 >> 8;
    if(u16 > 0x0FFF){
      *p++ = 0x20;
      *p++ = u8;
    }
    else
      *p++ = u8 | 0x10;
  }
  else if(u16 > 0x0F)
    *p++ = 0x10;

  *p++ = u16;
  return p;
}

inline
static Byte *VU124_FromU32(Byte *pVU124, U32 u32){
  U8 u8 = u32 >> 8;
  U8 *p = pVU124;
  if(u32 > 0x0FFF){
    if(u32 > 0x0FFFFF){
      if(u32 > 0x0FFFFFFF){
        *p++ = 0x40;
        *p++ = u32 >> 24;
      }
      else
        *p++ = u32 >> 24 | 0x30;

      *p++ = u32 >> 16;
    }
    else
      *p++ = u32 >> 16 | 0x20;

    *p++ = u8;
  }
  else  if(u32 > 0x0F)
    *p++ = u8 | 0x10;

  *p++ = u32;
  return p;
}

inline
static Byte *VU124_FromU64(Byte *pVU124, U64 u64){
  U8 u8 = u64 >> 8;
  U8 *p = pVU124;
  if(u64 > 0x0FFFFFFF){
    if(u64 > 0x0FFFFFFFFFFF){
      if(u64 > 0x0FFFFFFFFFFFFF){
        if(u64 > 0x0FFFFFFFFFFFFFFF){
          *p++ = 0x80;
          *p++ = u64 >> 56;
        }
        else
          *p++ = u64 >> 56 | 0x70;

        *p++ = u64 >> 48;
      }
      else
        *p++ = u64 >> 48 | 0x60;

      *p++ = u64 >> 40;
      *p++ = u64 >> 32;
    }
    else{
      if(u64 > 0x0FFFFFFFFF){
        *p++ = u64 >> 40 | 0x50;
        *p++ = u64 >> 32;
      }
      else
        *p++ = u64 >> 32 | 0x40;
    }

    *p++ = u64 >> 24;
    *p++ = u64 >> 16;
    *p++ = u8;
  }
  else{
    if(u64 > 0x0FFF){
      if(u64 > 0x0FFFFF){
        *p++ = u64 >> 24 | 0x30;
        *p++ = u64 >> 16;
      }
      else
        *p++ = u64 >> 16 | 0x20;

      *p++ = u8;
    }
    else  if(u64 > 0x0F)
      *p++ = u8 | 0x10;
  }
  *p++ = u64;
  return p;
}

//uH60 must in range[0, 0x0FFFFFFFFFFFFFFF]. Check it youself!
inline
static Byte *VU124_FromU124(Byte *pVU124, U64 uH60, U64 uL64){
  U8 u8;
  U8 *p = pVU124;
  if(uH60){
    u8 = uH60 >> 8;
    if(uH60 > 0x0FFFFFFFULL){
      if(uH60 > 0x0FFFFFFFFFFFULL){
        if(uH60 > 0x0FFFFFFFFFFFFFULL){
          *p++ = uH60 >> 56 | 0xF0;
          *p++ = uH60 >> 48;
        }
        else
          *p++ = uH60 >> 48 | 0xE0;

        *p++ = uH60 >> 40;
        *p++ = uH60 >> 32;
        *p++ = uH60 >> 24;
        *p++ = uH60 >> 16;
        *p++ = u8;
      }
      else{
        if(uH60 > 0x0FFFFFFFFFULL){
          *p++ = uH60 >> 40 | 0xD0;
          *p++ = uH60 >> 32;
        }
        else
          *p++ = uH60 >> 32 | 0xC0;

        *p++ = uH60 >> 24;
        *p++ = uH60 >> 16;
        *p++ = u8;
      }
      *p++ = uH60;
    }
    else{
      if(uH60 > 0x0FFFULL){
        if(uH60 > 0x0FFFFFULL){
          *p++ = uH60 >> 24 | 0xB0;
          *p++ = uH60 >> 16;
        }
        else
          *p++ = uH60 >> 16 | 0xA0;

        *p++ = u8;
        *p++ = uH60;
      }
      else  if(uH60 > 0x0FULL){
        *p++ = u8 | 0x90;
        *p++ = uH60;
      }
      else if(uH60)
        *p++ = uH60 | 0x80;
    }
    *p++ = uL64 >> 56;
    *p++ = uL64 >> 48;
    *p++ = uL64 >> 40;
    *p++ = uL64 >> 32;
    *p++ = uL64 >> 24;
    *p++ = uL64 >> 16;
    *p++ = uL64 >> 8;
  }
  else{
    u8 = uL64 >> 8;
    if(uL64 > 0x0FFFFFFF){
      if(uL64 > 0x0FFFFFFFFFFF){
        if(uL64 > 0x0FFFFFFFFFFFFF){
          if(uL64 > 0x0FFFFFFFFFFFFFFF){
            *p++ = 0x80;
            *p++ = uL64 >> 56;
          }
          else
            *p++ = uL64 >> 56 | 0x70;

          *p++ = uL64 >> 48;
        }
        else
          *p++ = uL64 >> 48 | 0x60;

        *p++ = uL64 >> 40;
        *p++ = uL64 >> 32;
      }
      else{
        if(uL64 > 0x0FFFFFFFFF){
          *p++ = uL64 >> 40 | 0x50;
          *p++ = uL64 >> 32;
        }
        else
          *p++ = uL64 >> 32 | 0x40;
      }

      *p++ = uL64 >> 24;
      *p++ = uL64 >> 16;
      *p++ = u8;
    }
    else{
      if(uL64 > 0x0FFF){
        if(uL64 > 0x0FFFFF){
          *p++ = uL64 >> 24 | 0x30;
          *p++ = uL64 >> 16;
        }
        else
          *p++ = uL64 >> 16 | 0x20;

        *p++ = u8;
      }
      else  if(uL64 > 0x0F)
        *p++ = u8 | 0x10;
    }
  }
  *p++ = uL64;
  return p;
}

#endif //VU124_H
