#ifndef UTF_8_16_H
#define UTF_8_16_H

//License: Public Domain
//Author: elf
//EMail: elf000@zoho.com

#include "UTF8.h"
#include "UTF16.h"

inline static U32 UTF8_BytesFrom16(const C16 *p, const C16 *pEnd) {
  U32 uBytes = 0;
  while (p != pEnd) {
      C16 c0 = *p++;
      uBytes += c0 >= 0x800? (c0 < 0x10000? 3 :  (++p, 4)) : (c0 >= 0x80? 2 : 1);
    }
  return uBytes;
}

inline static C32 UTF16_Read_UTF8_Bytes(const C16 **ppString, U8 *puUTF8Bytes) {
  const C16 *p = *ppString;
  C16 c0 = *p++;
  if (c0 >> 11 != 0x1B) {
      *ppString = p;
      *puUTF8Bytes = c0 >= 0x800? 3 : (c0 >= 0x80? 2 : 1);
      return c0;
    }
  C32 c32 = UTF16_Surrogate(c0, *p++);
  *ppString = p;
  *puUTF8Bytes = 4;
  return c32;
}

inline static C8 *UTF8_From16(C8 *p8, const C8 *p8End, const C16 **pp16, const C16 *p16End) {
  const C16 *p16 = *pp16;
  while (p16 != p16End) {
      const C16 *p16Begin = p16;
      U8 uUtf8Bytes;
      C32 code = UTF16_Read_UTF8_Bytes(&p16, &uUtf8Bytes);
      if (p8 + uUtf8Bytes > p8End) {
          *pp16 = p16Begin;
          return p8;
        }
      p8 = UTF8_Write(p8, code);
    }
  *pp16 = p16;
  return p8;
}

inline static U32 UTF16_BytesFrom8(const C *p, const C *pEnd) {
  U32 uBytes = 0;
  while (p != pEnd) {
      C c = *p;
      if (c < 0xF0) {
          uBytes += 2;
          if (c < 0x80) // 0xxxxxxx
            ++p;
          else if (c < 0xE0) // 110xxxxx 10xxxxxx
            p += 2;
          else if (c < 0xF0) // 1110xxxx 10xxxxxx 10xxxxxx
            p += 3;
        }
      else {
          uBytes += 4;
          p += 4; // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        }
    }
  return uBytes;
}

inline static C32 UTF8_Read_UTF16_Bytes(const C8 **ppString, U8 *puUTF16Bytes) {
  C32 c32;
  U8 uBytes;
  const C8 *p = *ppString;
  C c = *p++;
  if (c < 0x80) { // 0xxxxxxx
      c32 = c;
      uBytes = 2;
    }
  else {
      if (c < 0xE0) { // 110xxxxx 10xxxxxx
          c32 = c & 0x1F;
          uBytes = 2;
        }
      else {
          if (c < 0xF0) { // 1110xxxx 10xxxxxx 10xxxxxx
              c32 = c & 0x0F;
              uBytes = 2;
            }
          else { // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
              c32 = c & 0x07;
              c32 <<= 6;
              c32 |= *p++ & 0x3F;
              uBytes = 4;
            }
          c32 <<= 6;
          c32 |= *p++ & 0x3F;
        }
      c32 <<= 6;
      c32 |= *p++ & 0x3F;
    }
  *ppString = p;
  *puUTF8Bytes = uBytes;
  return c32;
}

inline static C32 UTF8_Read_UTF16_Size(const C8 **ppString, U8 *puUTF16) {
  C32 c32;
  U8 uSize;
  const C8 *p = *ppString;
  C c = *p++;
  if (c < 0x80) { // 0xxxxxxx
      c32 = c;
      uSize = 1;
    }
  else {
      if (c < 0xE0) { // 110xxxxx 10xxxxxx
          c32 = c & 0x1F;
          uSize = 1;
        }
      else {
          if (c < 0xF0) { // 1110xxxx 10xxxxxx 10xxxxxx
              c32 = c & 0x0F;
              uSize = 1;
            }
          else { // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
              c32 = c & 0x07;
              c32 <<= 6;
              c32 |= *p++ & 0x3F;
              uSize = 2;
            }
          c32 <<= 6;
          c32 |= *p++ & 0x3F;
        }
      c32 <<= 6;
      c32 |= *p++ & 0x3F;
    }
  *ppString = p;
  *puUTF16 = uSize;
  return c32;
}

inline static C16 *UTF16_From8(C16 *p16, const C16 *p16End, const C **pp8, const C *p8End) {
  const C *p8 = *pp8;
  while (p8 != p8End) {
      const C *p8Begin = p8;
      U8 uUtf16;
      C32 code = UTF8_Read_UTF16_Size(&p8, &uUtf16);
      if (p16 + uUtf16 > p16End) {
          *pp8 = p8Begin;
          return p16;
        }
      p16 = UTF16_Write(p16, code);
    }
  *pp8 = p8;
  return p16;
}

#endif //UTF_8_16_H
