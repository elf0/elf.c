#pragma once

#include "UTF8.h"
#include "UTF16.h"

#define UTF16_IsHan(c) (c >= 0x4E00 && c < 0xA000)

//Out buffer bytes should >= 2 * In buffer bytes;
//caller must input valid utf-16 string
static inline UTF8 *UTF_16To8(UTF16 *p16Begin, UTF16 *p16End, UTF8 *p8Buffer){
 U32 value;
 UTF16 *p16 = p16Begin;
 UTF8 *p8 = p8Buffer;
 while(p16 != p16End){
  p16 = UTF16_GetValue(p16, &value);
  p8 = UTF_To8(value, p8);
 }
 return p8;
}

//Out buffer bytes should >= 2 * In buffer bytes;
//caller must input valid utf-8 string
static inline UTF16 *UTF_8To16(UTF8 *p8Begin, UTF8 *p8End, UTF16 *p16Buffer){
 U32 value;
 UTF8 *p8 = p8Begin;
 UTF16 *p16 = p16Buffer;
 while(p8 != p8End){
  p8 = UTF8_GetValue(p8, &value);
  p16 = UTF_To16(value, p16);
 }

 return p16;
}
