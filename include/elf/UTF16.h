#ifndef UTF16_H
#define UTF16_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

typedef U16 UTF16;

//static inline B UTF16_IsSurrogate(UTF16 c){
// return (c >> 11) == 0x1B;
//}

static inline B UTF16_IsBasic(UTF16 c){
 return (c >> 11) != 0x1B;
}

static inline B UTF16_IsLead(UTF16 c){
 return (c >> 10) == 0x36;
}

static inline B UTF16_IsTrail(UTF16 c){
 return (c >> 10) == 0x37;
}

static inline U32 UTF16Extra_GetValue(UTF16 lead, UTF16 trail){
 U32 nHigh = (U32)lead & 0x03FF;
 U32 nLow = (U32)trail & 0x03FF;
 return 0x10000 + ((nHigh << 10) | nLow);
}

static inline UTF16 *UTF16_GetValidValue(UTF16 *pBegn, UTF16 *pEnd, U32 *pValue){
 UTF16 *p16 = pBegn;
 UTF16 v16 = *p16++;
  if(UTF16_IsBasic(v16))
   *pValue = v16;
  else if(UTF16_IsLead(v16)){
   UTF16 v16Trail = *p16;
   if(p16 == pEnd || !UTF16_IsTrail(v16Trail))
    return null;
   *pValue = UTF16Extra_GetValue(v16, v16Trail);
   ++p16;
  }
  else
   return null;
 return p16;
}

//caller must input valid utf-16 string
static inline UTF16 *UTF16_GetValue(UTF16 *pString, U32 *pValue){
 UTF16 *p16 = pString;
 UTF16 v16 = *p16++;
 if(UTF16_IsBasic(v16))
  *pValue = v16;
 else
  *pValue = UTF16Extra_GetValue(v16, *p16++);
 return p16;
}

//caller must input valid utf value
static inline UTF16 *UTF_To16(U32 value, UTF16 *pBuffer){
 UTF16 *p = pBuffer;
 if(value < 0x10000){
  assert(value < 0xD800 || value >= 0xE000);
  *p = value;
 }
 else{
  assert(value < 0x110000);
  value -= 0x10000;
  *p++ = 0xD800 | (value >> 10);
  *p = 0xDC00 | (value & 0x03FF);
 }

 return ++p;
}

#endif //UTF16_H
