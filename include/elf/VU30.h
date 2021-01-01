#ifndef VU30_H
#define VU30_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

// Range: [0, 0x3FFFFFFF]

inline
static U8 VU30_TailBytes(const Byte *pVU30) {
  return *pVU30 >> 6;
}

inline
static U8 VU30_Bytes(const Byte *pVU30) {
  return 1 + (*pVU30 >> 6);
}

inline
static U8 VU30_NeedBytes(U32 u30) {
  return u30 < 0x40? 1 : u30 < 0x4000? 2 : u30 < 0x400000? 3 : 4;
}

inline
static U32 VU30_Read(const Byte **ppVU30) {
  const Byte *pVU30 = *ppVU30;
  U32 u30 = *pVU30++;
  if (u30 > 0x3F) {
    U32 uHead = u30;
    u30 &= 0x3F;
    if (uHead > 0x7F) {
      if (uHead > 0xBF) {
        u30 <<= 8;
        u30 |= *pVU30++;
      }
      u30 <<= 8;
      u30 |= *pVU30++;
    }
    u30 <<= 8;
    u30 |= *pVU30++;
  }
  *ppVU30 = pVU30;
  return u30;
}

inline
static B VU30_Valid(U32 u32) {
  return u32 < 0x40000000;
}

inline
static B VU30_Invalid(U32 u32) {
  return u32 > 0x3FFFFFFF;
}

//u6 must in range[0, 0x3F]. Check it youself!
inline
static Byte *VU30_FromU6(Byte *pVU30, U8 u6) {
  *pVU30++ = u6;
  return pVU30;
}

inline
static Byte *VU30_FromU8(Byte *pVU30, U8 u8) {
  if (u8 > 0x3F)
    *pVU30++ = 0x40;
  *pVU30++ = u8;
  return pVU30;
}

inline
static Byte *VU30_FromU16(Byte *pVU30, U16 u16) {
  if (u16 > 0x3F) {
    if (u16 < 0x4000)
      *pVU30++ = (u16 >> 8) | 0x40;
    else {
      *pVU30++ = 0x80;
      *pVU30++ = u16 >> 8;
    }
  }
  *pVU30++ = u16;
  return pVU30;
}

//u30 must in range[0, 0x3FFFFFFF]. Check it youself!
inline
static Byte *VU30_FromU30(Byte *pVU30, U32 u30) {
  if (u30 > 0x3F) {
    if (u30 < 0x4000)
      *pVU30++ = (u30 >> 8) | 0x40;
    else {
      if (u30 < 0x400000)
        *pVU30++ = (u30 >> 16) | 0x80;
      else {
        *pVU30++ = (u30 >> 24) | 0xC0;
        *pVU30++ = u30 >> 16;
      }
      *pVU30++ = u30 >> 8;
    }
  }
  *pVU30++ = u30;
  return pVU30;
}

//(pEnd - pBegin) > 0
//(pEnd - pBegin) SHOULD <= 0x404040
inline
static Byte *VU30_Sum(Byte *pVU30, const Byte *pBegin, const Byte *pEnd) {
  U32 uSum = *pBegin++;
  while (pBegin != pEnd)
    uSum += *pBegin++;
  if (uSum > 0x3F) {
    if (uSum < 0x4000)
      *pVU30++ = (uSum >> 8) | 0x40;
    else {
      if (uSum < 0x400000)
        *pVU30++ = (uSum >> 16) | 0x80;
      else {
        *pVU30++ = (uSum >> 24) | 0xC0;
        *pVU30++ = uSum >> 16;
      }
      *pVU30++ = uSum >> 8;
    }
  }
  *pVU30++ = u30;
  return pVU30;
}

#endif //VU30_H

