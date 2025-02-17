#ifndef UTF8_H
#define UTF8_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

// Utf-8 max: F4 8F BF BF
// Utf-8 end: F4 90 80 80
// Unicode max: 0x10FFFF
// Unicode end: 0x110000
// Surrogate: [0xD800, 0xDFFF]
// BOM: EF BB BF

#define UNICODE_MAX 0x10FFFF
#define UNICODE_INVALID(c32) (c32 > 0x10FFFF)

//#define ERROR_UTF8 1
#ifndef ERROR_UTF8
#error "ERROR_UTF8 must be defined"
#endif
#define ERROR_UTF8_PARTIAL (ERROR_UTF8 + 0)
#define ERROR_UTF8_HEAD (ERROR_UTF8 + 1)
#define ERROR_UTF8_TAIL (ERROR_UTF8 + 2)
#define ERROR_UTF8_RANGE (ERROR_UTF8 + 3)
#define ERROR_UTF8_END (ERROR_UTF8 + 4)

inline static B UTF8_ValidTail(C tail) {
    return (tail & 0xC0) == 0x80;
}

inline static B UTF8_InvalidTail(C tail) {
    return (tail & 0xC0) != 0x80;
}

inline static U32 UTF8_Code2(C c0, C c1) {
    U32 c32;
    c32 = c0 & 0x1F;
    c32 <<= 6;
    c32 |= c1 & 0x3F;
    return c32;
}

inline static U32 UTF8_Code3(C c0, C c1, C c2) {
    U32 c32 = U32(c0 & 0x0F);
    c32 <<= 6;
    c32 |= U32(c1 & 0x3F);
    c32 <<= 6;
    c32 |= U32(c2 & 0x3F);
    return c32;
}

inline static U32 UTF8_Code4(C c0, C c1, C c2, C c3) {
    U32 c32 = U32(c0 & 0x0F);
    c32 <<= 6;
    c32 |= U32(c1 & 0x3F);
    c32 <<= 6;
    c32 |= U32(c2 & 0x3F);
    c32 <<= 6;
    c32 |= U32(c3 & 0x3F);
    return c32;
}

inline
static const C *UTF8_Skip(const C *p) {
  U8 u8 = *p;
  if (u8 < 0x80) // 0xxxxxxx
    return p + 1;

  if (u8 < 0xE0) // 110xxxxx 10xxxxxx
    return p + 2;

  if (u8 < 0xF0) // 1110xxxx 10xxxxxx 10xxxxxx
    return p + 3;

  return p + 4; // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
}

inline
static const C *UTF8_SkipTail(C cHead, const C *pTail) {
  if (cHead < 0xE0) // 110xxxxx 10xxxxxx
    return pTail + 1;

  if (cHead < 0xF0) // 1110xxxx 10xxxxxx 10xxxxxx
    return pTail + 2;

  return pTail + 3; // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
}

inline
static U64 UTF8_Count(const C *p, const C *pEnd) {
  U64 uCount = 0;
  while (p != pEnd) {
    ++uCount;
    C c = *p;
    if (c < 0x80) // 0xxxxxxx
      ++p;
    else if (c < 0xE0) // 110xxxxx 10xxxxxx
      p += 2;
    else if (c < 0xF0) // 1110xxxx 10xxxxxx 10xxxxxx
      p += 3;
    else
      p += 4; // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
  }
  return uCount;
}

//caller must input valid utf-8 string
inline
static C32 UTF8_ReadTail(C32 cHead, const C **ppTail) {
  const C *p = *ppTail;
  if (cHead < 0xE0) // 110xxxxx 10xxxxxx
    cHead &= 0x1F;
  else {
    if (cHead < 0xF0) // 1110xxxx 10xxxxxx 10xxxxxx
      cHead &= 0x0F;
    else { // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
      cHead &= 0x07;
      cHead <<= 6;
      cHead |= *p++ & 0x3F;
    }
    cHead <<= 6;
    cHead |= *p++ & 0x3F;
  }
  cHead <<= 6;
  cHead |= *p++ & 0x3F;
  *ppTail = p;
  return cHead;
}

//caller must input valid utf-8 string
inline
static C32 UTF8_Read(const C **ppString) {
  const C *p = *ppString;
  C32 c32;
  C c = *p++;
  if (c < 0x80) // 0xxxxxxx
    c32 = c;
  else {
    if (c < 0xE0) // 110xxxxx 10xxxxxx
      c32 = c & 0x1F;
    else {
      if (c < 0xF0) // 1110xxxx 10xxxxxx 10xxxxxx
        c32 = c & 0x0F;
      else { // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        c32 = c & 0x07;
        c32 <<= 6;
        c32 |= *p++ & 0x3F;
      }
      c32 <<= 6;
      c32 |= *p++ & 0x3F;
    }
    c32 <<= 6;
    c32 |= *p++ & 0x3F;
  }
  *ppString = p;
  return c32;
}

inline static C32 UTF8_LowerRead(const C **ppString) {
    const C *p = *ppString;
    C32 c32;
    C c = *p++;
    if (c < 0x80) { // 0xxxxxxx
        U8 r = c - 'A';
        if (r < 26)
            c |= 0x20;

        c32 = c;
    }
    else {
        if (c < 0xE0) // 110xxxxx 10xxxxxx
            c32 = c & 0x1F;
        else {
            if (c < 0xF0) // 1110xxxx 10xxxxxx 10xxxxxx
                c32 = c & 0x0F;
            else { // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
                c32 = c & 0x07;
                c32 <<= 6;
                c32 |= *p++ & 0x3F;
            }
            c32 <<= 6;
            c32 |= *p++ & 0x3F;
        }
        c32 <<= 6;
        c32 |= *p++ & 0x3F;
    }
    *ppString = p;
    return c32;
}

inline
static C32 UTF8_Max(const C *p, const C *pEnd) {
  C32 cMax = 0;
  while (p != pEnd) {
    C32 c32 = UTF8_Read(&p);
    if (c32 > cMax)
      cMax = c32;
  }
  return cMax;
}

inline
static U8 UTF8_NeedBytes(C32 value) {
  return value < 0x80? 1 : value < 0x0800? 2 : value < 0x10000? 3 : 4;
}

//pBuffer size should >= 4 bytes
//caller must input valid utf c32
inline
static C *UTF8_Write(C *pBuffer, C32 value) {
  C *p = pBuffer;
  if (value < 0x80) { // 0xxxxxxx
    *p++ = value;
    return p;
  }

  if (value < 0x0800) // 110xxxxx 10xxxxxx
    *p++ = 0xC0 | (value >> 6);
  else {
    if (value < 0x10000) {//1110xxxx 10xxxxxx 10xxxxxx
      //            assert(value < 0xD800 || value > 0xDFFF);
      *p++ = 0xE0 | (value >> 12);
    }
    else {//11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
      //            assert(value < 0x110000);
      *p++ = 0xF0 | (value >> 18);
      *p++ = 0x80 | ((value >> 12) & 0x3F);
    }
    *p++ = 0x80 | ((value >> 6) & 0x3F);
  }
  *p++ = 0x80 | (value & 0x3F);
  return p;
}

inline
static B UTF8_Valid(const C *p, const C *pEnd) {
  while (p != pEnd) {
    C32 c32 = *p++;
    // 0xxxxxxx
    if (c32 > 0x7F) {
      if (c32 < 0xE0) { // 110xxxxx 10xxxxxx
        if (c32 < 0xC2 || p == pEnd)
          return 0;
      }
      else {
        if (c32 < 0xF0) { // 1110xxxx 10xxxxxx 10xxxxxx
          if (p + 1 >= pEnd)
            return 0;
        }
        else { // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
          if (p + 2 >= pEnd)
            return 0;

          C t = *p++ - 0x80;
          if (t > 0x3F)
            return 0;

          if (((c32 << 6) | t) > 0x3D0F)
            return 0;
        }
        if ((C)(*p++ - 0x80) > 0x3F)
          return 0;
      }
      if ((C)(*p++ - 0x80) > 0x3F)
        return 0;
    }
  }
  return 1;
}

inline static C32 UTF8_Parse(C **pp, C *pEnd) {
    C *p = *pp;
    C32 uCode = *p++;
    if (uCode >= 0x80) {
        if (uCode < 0xE0) { // 110xxxxx 10xxxxxx
            if (uCode < 0xC2)
                return ERROR_UTF8_HEAD;

            uCode &= 0x1F;
        }
        else {
            if (uCode < 0xF0) // 1110xxxx 10xxxxxx 10xxxxxx
                uCode &= 0x0F;
            else { // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
                if (uCode >= 0xF8)
                    return ERROR_UTF8_HEAD;

                uCode &= 0x07;
                if (p == pEnd)
                    return ERROR_UTF8_PARTIAL;

                C t = *p++ - 0x80;
                if (t >= 0x40)
                    return ERROR_UTF8_TAIL;

                uCode <<= 6;
                uCode |= t;
                if (uCode >= 0x110)
                    return ERROR_UTF8_RANGE;
            }
            if (p == pEnd)
                return ERROR_UTF8_PARTIAL;

            C t = *p++ - 0x80;
            if (t >= 0x40)
                return ERROR_UTF8_TAIL;

            uCode <<= 6;
            uCode |= t;
        }
        if (p == pEnd)
            return ERROR_UTF8_PARTIAL;

        C t = *p++ - 0x80;
        if (t >= 0x40)
            return ERROR_UTF8_TAIL;

        uCode <<= 6;
        uCode |= t;
    }
    *pp = p;
    return uCode;
}

inline
static U64 UTF8_ParseCount(const C **ppBegin, const C *pEnd) {
  U64 uCount = 0;
  const C *p = *ppBegin;
  while (p != pEnd) {
    C32 c32 = *p++;
    // 0xxxxxxx
    if (c32 > 0x7F) {
      if (c32 < 0xE0) { // 110xxxxx 10xxxxxx
        if (c32 < 0xC2 || p == pEnd) {
          uCount |= 0x8000000000000000;
          break;
        }
      }
      else {
        if (c32 < 0xF0) { // 1110xxxx 10xxxxxx 10xxxxxx
          if (p + 1 >= pEnd) {
            uCount |= 0x8000000000000000;
            break;
          }
        }
        else { // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
          if (c32 > 0xF4 || p + 2 >= pEnd) {
            uCount |= 0x8000000000000000;
            break;
          }
          c32 &= 0x07;
          C t = *p++ - 0x80;
          if (t > 0x3F) {
            uCount |= 0x8000000000000000;
            break;
          }
          c32 <<= 6;
          c32 |= t;
          if (c32 > 0x10F) {
            uCount |= 0x8000000000000000;
            break;
          }
        }
        C t = *p++ - 0x80;
        if (t > 0x3F) {
          uCount |= 0x8000000000000000;
          break;
        }
      }
      C t = *p++ - 0x80;
      if (t > 0x3F) {
        uCount |= 0x8000000000000000;
        break;
      }
    }
    ++uCount;
  }
  *ppBegin = p;
  return uCount;
}

#endif //UTF8_H


