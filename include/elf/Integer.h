#ifndef INTEGER
#define INTEGER

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "Type.h"

static inline U8 VU61_Bytes(const Byte *pVU61){
  return (*pVU61 >> 5) + 1;
}

//range: [0, 0x1FFFFFFFFFFFFFFF]
static inline const Byte *VU61_ToU64(const Byte *pVU61, U64 *pU64){
  const U8 *p = pVU61;
  U64 nValue = *p & 0x1F;
  U8 nTail = *p++ >> 5;
  while(nTail){
    nValue = (nValue << 8) | *p++;
    --nTail;
  }
  *pU64 = nValue;
  return p;
}

static inline Bool VU61_Valid(U64 u64){
  return u64 < 0x2000000000000000;
}

static inline Bool VU61_Invalid(U64 u64){
  return u64 > 0x1FFFFFFFFFFFFFFF;
}

//u64 must in range[0, 0x1FFFFFFFFFFFFFFF]. Check it youself!
static inline const Byte *VU61_FromU64(Byte *pVU61, U64 u64){
  U8 *p = pVU61;
  U64 nValue = u64;
  if(nValue < 0x200000){
    if(nValue > 0x1F){
      if(nValue < 0x2000){
        *p++ = (nValue >> 8) | 0x20;
      }
      else{
        *p++ = (nValue >> 16) | 0x40;
        *p++ = nValue >> 8;
      }
    }
  }
  else{
    if(nValue < 0x2000000000){
      if(nValue < 0x20000000){
        *p++ = (nValue >> 24) | 0x60;
      }
      else{
        *p++ = (nValue >> 32) | 0x80;
        *p++ = nValue >> 24;
      }
    }
    else{
      if(nValue < 0x20000000000000){
        if(nValue < 0x200000000000){
          *p++ = (nValue >> 40) | 0xA0;
        }
        else{
          *p++ = (nValue >> 48) | 0xC0;
          *p++ = nValue >> 40;
        }
      }
      else{
        *p++ = (nValue >> 56) | 0xE0;
        *p++ = nValue >> 48;
        *p++ = nValue >> 40;
      }
      *p++ = nValue >> 32;
      *p++ = nValue >> 24;
    }
    *p++ = nValue >> 16;
    *p++ = nValue >> 8;
  }

  *p++ = nValue;
  return p;
}
#endif // INTEGER

