#ifndef UTF16_H
#define UTF16_H

//License: Public Domain
//Author: elf
//EMail: elf000@zoho.com

// Unicode: [0, 0x10FFFF]
// Surrogate: [0xD800, 0xDFFF]
// High surrogate: [0xD800, 0xDBFF]
// Low surrogate: [0xDC00, 0xDFFF]
// BOM:
// BE: FE FF
// LE: FF FE
inline
    static B UTF16_IsBasic(C32 c) {
  return c >> 11 != 0x1B;
}

inline
    static B UTF16_IsHigh(C32 c) {
  return c >> 10 == 0x36;
}

inline
    static B UTF16_NotHigh(C32 c) {
  return c >> 10 != 0x36;
}

inline
    static B UTF16_IsLow(C32 c) {
  return c >> 10 == 0x37;
}

inline
    static B UTF16_NotLow(C32 c) {
  return c >> 10 != 0x37;
}

inline
    static bool UTF16_IsSurrogate(C32 c) {
  return c >> 11 == 0x1B;
}

inline
    static C32 UTF16_Surrogate(C32 c0, C32 c1) {
  return c0 << 10 ^ c1 ^ 0x360DC00;
}

inline
    static U64 UTF16_Count(const C16 *p, const C16 *pEnd) {
  U64 uCount = 0;
  while (p != pEnd) {
      ++uCount;
      if (*p++ >> 11 == 0x1B)
        ++p;
    }
  return uCount;
}

inline
    static B UTF16_Valid(const C16 *p, const C16 *pEnd) {
  while(p != pEnd) {
      C16 c16 = *p++;
      if (c16 >> 11 == 0x1B) {
          if (c16 > 0xDBFF || p == pEnd || *p++ >> 10 != 0x37)
            return 0;
        }
      else if (c16 == 0x0A00)
        return 0;
    }
  return 1;
}

inline
    static C32 UTF16_Parse(const C16 **ppBegin, const C16 *pEnd) {
  const C16 *p = *ppBegin;
  C32 c = *p++;
  if (c >> 11 == 0x1B) {
      if (c > 0xDBFF || p == pEnd)
        c |= 0x80000000;
      else {
          C32 c1 = *p++;
          if (c1 >> 10 == 0x37)
            c = UTF16_Surrogate(c, c1);
          else {
              --p;
              c |= 0x80000000;
            }
        }
    }
  else if (c == 0x0A00)
    c |= 0x80000000;
  *ppBegin = p;
  return c;
}
inline
    static U64 UTF16_ParseCount(const C16 **ppBegin, const C16 *pEnd) {
  U64 uCount = 0;
  const C16 *p = *ppBegin;
  while (p != pEnd) {
      C32 c32 = *p++;
      if (c32 >> 11 == 0x1B) {
          if (c32 > 0xDBFF || p == pEnd) {
              --p;
              uCount |= 0x8000000000000000;
              break;
            }
          else if (*p++ >> 10 != 0x37) {
              p -= 2;
              uCount |= 0x8000000000000000;
              break;
            }
        }
      else if (c32 == 0x0A00) {
          --p;
          uCount |= 0x8000000000000000;
          break;
        }
      ++uCount;
    }
  *ppBegin = p;
  return uCount;
}

//caller must input valid utf-16 string
inline
    static C32 UTF16_Read(const C16 **ppString) {
  const C16 *p = *ppString;
  C32 c = *p++;
  if (c >> 11 == 0x1B)
    c = UTF16_Surrogate(c, *p++);
  *ppString = p;
  return c;
}

inline
    static C32 UTF16_ReadLE(const C **ppString) {
  const C *p = *ppString;
  C32 c = *p++;
  c |= *p++ << 8;
  if (c >> 11 == 0x1B)
    c = UTF16_Surrogate(c, *p++ | *p++ << 8);
  *ppString = p;
  return c;
}

inline
    static C32 UTF16_ReadBE(const C **ppString) {
  const C *p = *ppString;
  C32 c = *p++ << 8;
  c |= *p++;
  if (c >> 11 == 0x1B)
    c = UTF16_Surrogate(c, *p++ << 8 | *p++);
  *ppString = p;
  return c;
}

inline
    static U8 UTF16_Bytes(C32 value) {
  return value < 0x10000? 2 : 4;
}

//caller must input valid utf value
inline
    static C16 *UTF16_Write(C16 *pBuffer, C32 value) {
  if (value < 0x10000) {
      //        assert(value < 0xD800 || value > 0xDFFF);
      *pBuffer = value;
    }
  else{
      //        assert(value < 0x110000);
      value -= 0x10000;
      *pBuffer++ = 0xD800 | value >> 10;
      *pBuffer = 0xDC00 | value & 0x03FF;
    }

  return ++pBuffer;
}

#endif //UTF16_H
