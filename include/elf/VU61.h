#ifndef VU61_H
#define VU61_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

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

static inline Bool VU61_Valid(U64 u64){
  return u64 < 0x2000000000000000;
}

static inline Bool VU61_Invalid(U64 u64){
  return u64 > 0x1FFFFFFFFFFFFFFF;
}

//u64 must in range[0, 0x1FFFFFFFFFFFFFFF]. Check it youself!
static inline const Byte *VU61_FromU64(Byte *pVU61, U64 u64){
  U8 uShift = 56;
  while(u64 >> uShift == 0){
    if(!(uShift -= 8))
      break;
  }


  U8 *p = pVU61;
  *p++ = uShift << 2 | u64 >> uShift;
  while(uShift)
    *p++ = u64 >> (uShift -= 8);

  return p;
}

#endif //VU61_H

