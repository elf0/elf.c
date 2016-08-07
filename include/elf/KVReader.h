#ifndef KVREADER_H
#define KVREADER_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

//Structure: "Key0:Value0\nKey1:Value1\nKey2:Value2\n:\n"
//Buffer must ends with ":\n"

#include "String.h"

#ifndef KVReader_KEY_END_CHAR
#define KVReader_KEY_END_CHAR ':'
#endif

#ifndef KVReader_VALUE_END_CHAR
#define KVReader_VALUE_END_CHAR '\n'
#endif


typedef E8 (*KVReader_Handler)(void *pContext, const C *pKey, const C *pKeyEnd, const C *pValue, const C *pValueEnd);

static inline E8 KVReader_Parse(void *pContext, const C *pBegin, const C *pEnd, KVReader_Handler onKV){
  size_t nSize = pEnd - pBegin;
  if(nSize < 2 || *(U16*)(pEnd - 2) != *(U16*)":\n")
    return 1;

  E8 e;
  U8 uIndent;
  const C *pKey, *pKeyEnd, *pValue;
  const C *p = pBegin;
  do{
    p = String_SkipUntil(pKey = p, (C)KVReader_KEY_END_CHAR);
    pKeyEnd = p++;
    p = String_SkipUntil(pValue = p, (C)KVReader_VALUE_END_CHAR);

    if(e = onKV(pContext, pKey, pKeyEnd, pValue, p++))
      return e;
  }while(p != pEnd);

  return 0;
}

#endif // KVREADER_H
