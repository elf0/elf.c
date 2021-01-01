#ifndef VU15_H
#define VU15_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

// Range: [0, 0x7FFF]

inline
static U8 VU15_TailBytes(const Byte *pVU15) {
  return *pVU15 >> 7;
}

inline
static U8 VU15_Bytes(const Byte *pVU15) {
  return 1 + (*pVU15 >> 7);
}

inline
static U8 VU15_NeedBytes(U16 u15) {
  return u15 < 0x7F? 1 : 2;
}

inline
static U16 VU15_Read(const Byte **ppVU15) {
  const Byte *pVU15 = *ppVU15;
  U16 u15 = *pVU15++;
  if (u15 > 0x7F) {
    u15 &= 0x7F;
    u15 <<= 8;
    u15 |= *pVU15++;
  }
  *ppVU15 = pVU15;
  return u15;
}

inline
static B VU15_Valid(U16 u16) {
  return u16 < 0x8000;
}

inline
static B VU15_Invalid(U16 u16) {
  return u16 > 0x7FFF;
}

//u7 must in range[0, 0x7F]. Check it youself!
inline
static Byte *VU15_FromU7(Byte *pVU15, U8 u7) {
  *pVU15++ = u7;
  return pVU15;
}

inline
static Byte *VU15_FromU8(Byte *pVU15, U8 u8) {
  if (u8 > 0x7F)
    *pVU15++ = 0x80;
  *pVU15++ = u8;
  return pVU15;
}

//u15 must in range[0, 0x7FFF]. Check it youself!
inline
static Byte *VU15_FromU15(Byte *pVU15, U16 u15) {
  if (u15 > 0x7F)
    *pVU15++ = (u15 >> 8) | 0x80;
  *pVU15++ = u15;
  return pVU15;
}

//(pEnd - pBegin) > 0
inline
static Byte *VU15_Sum(Byte *pVU15, const Byte *pBegin, const Byte *pEnd) {
  U16 uSum = *pBegin++;
  while (pBegin != pEnd)
    uSum += *pBegin++;
  if (uSum > 0x7F)
    *pVU15++ = (uSum >> 8) | 0x80;
  *pVU15++ = uSum;
  return pVU15;
}

#endif //VU15_H

