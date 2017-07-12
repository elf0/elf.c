#ifndef UTF16_H
#define UTF16_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com


//static inline B UTF16_IsSurrogate(C16 c){
// return (c >> 11) == 0x1B;
//}

static inline B UTF16_IsBasic(C16 c){
 return (c >> 11) != 0x1B;
}

static inline B UTF16_IsLead(C16 c){
 return (c >> 10) == 0x36;
}

static inline B UTF16_IsTrail(C16 c){
 return (c >> 10) == 0x37;
}

static inline C32 UTF16Extra_GetValue(C16 lead, C16 trail){
 C32 nHigh = (C32)lead & 0x03FF;
 C32 nLow = (C32)trail & 0x03FF;
 return 0x10000 + ((nHigh << 10) | nLow);
}

static inline C16 *UTF16_GetValidValue(C16 *pBegn, C16 *pEnd, C32 *pValue){
 C16 *p16 = pBegn;
 C16 v16 = *p16++;
  if(UTF16_IsBasic(v16))
   *pValue = v16;
  else if(UTF16_IsLead(v16)){
   C16 v16Trail = *p16;
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
static inline C16 *UTF16_GetValue(C16 *pString, C32 *pValue){
 C16 *p16 = pString;
 C16 v16 = *p16++;
 if(UTF16_IsBasic(v16))
  *pValue = v16;
 else
  *pValue = UTF16Extra_GetValue(v16, *p16++);
 return p16;
}

//caller must input valid utf value
static inline C16 *UTF_To16(C32 value, C16 *pBuffer){
 C16 *p = pBuffer;
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
