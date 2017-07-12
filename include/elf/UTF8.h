#ifndef UTF8_H
#define UTF8_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com


static inline C *UTF8_GetValidValue(C *pBegin, C *pEnd, C32 *pValue){
  C *p = pBegin;
  C32 value = *p++;
  if(value < 0x80);//0xxxxxxx
  else if(value < 0xC2)
    return null;
  else{
    if(value < 0xE0)//110xxxxx 10xxxxxx
      value = (value & 0x001F) << 6;
    else{
      if(value < 0xF0)//1110xxxx 10xxxxxx 10xxxxxx
        value = (value & 0x0F) << 12;
      else if(value < 0xF5){//11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        value = (value & 0x07) << 18;
        if(p == pEnd || ((*p & 0xC0) != 0x80))
          return null;
        value |= (*p++ & 0x3F) << 12;
      }
      else
        return null;

      if(p == pEnd || ((*p & 0xC0) != 0x80))
        return null;
      value |= (*p++ & 0x3F) << 6;
    }

    if(p == pEnd || ((*p & 0xC0) != 0x80))
      return null;
    value |= *p++ & 0x3F;
  }

  *pValue = value;
  return p;
}

//caller must input valid utf-8 string
static inline C *UTF8_GetMultibytesValue(C *pString, C32 *pValue){
  C *p = pString;
  C32 value = *p++;

  if(value < 0xE0)//110xxxxx 10xxxxxx
    value = (value & 0x001F) << 6;
  else{
    if(value < 0xF0)//1110xxxx 10xxxxxx 10xxxxxx
      value = (value & 0x0F) << 12;
    else{//11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
      value = (value & 0x07) << 18;
      value |= (*p++ & 0x3F) << 12;
    }

    value |= (*p++ & 0x3F) << 6;
  }

  value |= *p++ & 0x3F;


  *pValue = value;
  return p;
}

//caller must input valid utf-8 string
static inline C *UTF8_GetValue(C *pString, C32 *pValue){
  C *p = pString;
  C32 value = *p++;
  if(value < 0x80);//0xxxxxxx
  else{
    if(value < 0xE0)//110xxxxx 10xxxxxx
      value = (value & 0x001F) << 6;
    else{
      if(value < 0xF0)//1110xxxx 10xxxxxx 10xxxxxx
        value = (value & 0x0F) << 12;
      else{//11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        value = (value & 0x07) << 18;
        value |= (*p++ & 0x3F) << 12;
      }

      value |= (*p++ & 0x3F) << 6;
    }

    value |= *p++ & 0x3F;
  }

  *pValue = value;
  return p;
}

//pBuffer size should >= 4 bytes
//caller must input valid utf value
static inline C *UTF_To8(C32 value, C *pBuffer){
  C *p = pBuffer;
  if(value < 0x80){//0xxxxxxx
    *p = value;
    return ++p;
  }

  if(value < 0x0800)//110xxxxx 10xxxxxx
    *p = 0xC0 | (value >> 6);
  else{
    if(value < 0x10000){//1110xxxx 10xxxxxx 10xxxxxx
      assert(value < 0xD800 || value >= 0xE000);
      *p = 0xE0 | (value >> 12);
    }
    else{//11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
      assert(value < 0x110000);
      *p = 0xF0 | (value >> 18);
      ++p;
      *p = 0x80 | ((value >> 12) & 0x3F);
    }

    ++p;
    *p = 0x80 | ((value >> 6) & 0x3F);
  }

  ++p;
  *p = 0x80 | (value & 0x3F);
  return ++p;
}

#endif //UTF8_H

