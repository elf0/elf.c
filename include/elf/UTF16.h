#ifndef UTF16_H
#define UTF16_H

//License: Public Domain
//Author: elf
//EMail: elf000@zoho.com

// Unicode: [0, 0x10FFFF]
// Surrogate: [0xD800, 0xDFFF]
// Lead/High/C0 surrogate: [0xD800, 0xDBFF]
// Trail/Low/C1 surrogate: [0xDC00, 0xDFFF]
// BOM:
// BE: FE FF
// LE: FF FE

// c < 0xD800 || c >= 0xE000
inline static B UTF16_IsBasic(C16 c) {
  return c >> 11 != 0x1B;
}

// Lead/High
// c >= 0xD800 && c < 0xDC00
inline static B UTF16_IsC0(C16 c) {
  return c >> 10 == 0x36;
}

inline static B UTF16_NotC0(C16 c) {
  return c >> 10 != 0x36;
}

// Trail/Low
// c >= 0xDC00 && c < 0xE000
inline static B UTF16_IsC1(C16 c) {
  return c >> 10 == 0x37;
}

inline static B UTF16_NotC1(C16 c) {
  return c >> 10 != 0x37;
}

// c >= 0xD800 && c < 0xE000
inline static B UTF16_IsSurrogate(C16 c) {
  return c >> 11 == 0x1B;
}

inline static B UTF16_NotSurrogate(C16 c) {
  return c >> 11 != 0x1B;
}

inline static C32 UTF16_Surrogate(C16 c0, C16 c1) {
  return (((C32)c0 - 0xD7F7) << 10) + (C32)c1;
}

inline static U64 UTF16_Count(const C16 *p, const C16 *pEnd) {
  U64 uCount = 0;
  while (p != pEnd) {
      if (*p >> 11 != 0x1B)
        ++p;
      else
        p += 2;
      ++uCount;
    }
  return uCount;
}

inline static B UTF16_Valid(const C16 *p, const C16 *pEnd) {
  while(p != pEnd) {
      C16 c0 = *p++ >> 10;
      if (c0 == 0x36) {
          if (p == pEnd || *p++ >> 10 != 0x37)
            return 0;
        }
      else if (c0 == 0x37)
        return 0;
    }
  return 1;
}

inline static C32 UTF16_Parse(const C16 **ppBegin, const C16 *pEnd) {
  const C16 *p = *ppBegin;
  C16 c0 = *p++;
  if (c0 >> 11 != 0x1B) {
      *ppBegin = p;
      return (C32)c0;
    }
  if (c0 < 0xDC00 && p != pEnd) {
      C16 c1 = *p++;
      if (c1 >> 10 == 0x37) {
          *ppBegin = p;
          return UTF16_Surrogate(c0, c1);
        }
    }
  *ppBegin = p;
  return 0x80000000;
}

inline static U64 UTF16_ParseCount(const C16 **ppBegin, const C16 *pEnd) {
  U64 uCount = 0;
  const C16 *p = *ppBegin;
  while (p != pEnd) {
      C16 c0 = *p++;
      if (c0 >> 11 == 0x1B) {
          if (c0 < 0xDC00 || p != pEnd) {
              if (*p++ >> 10 != 0x37)
                uCount |= 0x8000000000000000;
            }
          else
            uCount |= 0x8000000000000000;
        }
      ++uCount;
    }
  *ppBegin = p;
  return uCount;
}

//caller must input valid utf-16 string
inline static C32 UTF16_Read(const C16 **ppString) {
  const C16 *p = *ppString;
  C16 c0 = *p++;
  if (c0 >> 11 != 0x1B) {
      *ppString = p;
      return c0;
    }
  C32 c32 = UTF16_Surrogate(c0, *p++);
  *ppString = p;
  return c32;
}

inline static C32 UTF16_ReadLE(const C **ppString) {
  const C *p = *ppString;
  C16 c0 = U16_LE(p);
  p += 2;
  if (c0 >> 11 != 0x1B) {
      *ppString = p;
      return c0;
    }
  C32 c32 = UTF16_Surrogate(c0, U16_LE(p));
  *ppString = p + 2;
  return c32;
}

inline static C32 UTF16_ReadBE(const C **ppString) {
  const C *p = *ppString;
  C16 c0 = U16_BE(p);
  p += 2;
  if (c0 >> 11 != 0x1B) {
      *ppString = p;
      return c0;
    }
  C32 c32 = UTF16_Surrogate(c0, U16_BE(p));
  *ppString = p + 2;
  return c32;
}

inline static U8 UTF16_Bytes(C32 value) {
  return value < 0x10000? 2 : 4;
}

//caller must input valid utf value
inline static C16 *UTF16_Write(C16 *pBuffer, C32 value) {
  if (value < 0x10000)
    *pBuffer = value;
  else {
      //        assert(value < 0x110000);
      *pBuffer++ = (value >> 10) + 0xD7C0;
      *pBuffer = value & 0x3FF | 0xDC00;
    }
  return ++pBuffer;
}

#endif //UTF16_H
