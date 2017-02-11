#ifndef KVREADER_H
#define KVREADER_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

//Structure: "Key0:Value0\nKey1:Value1\nKey2:Value2\n"

#include "String.h"

#ifndef KVReader_KEY_END_CHAR
#define KVReader_KEY_END_CHAR ':'
#endif

#ifndef KVReader_VALUE_END_CHAR
#define KVReader_VALUE_END_CHAR '\n'
#endif

typedef E8 (*KVReader_Handler)(void *pContext, const C *pKey, const C *pKeyEnd, const C *pValue, const C *pValueEnd);

static inline E8 KVReader_Parse(void *pContext, const C *pBegin, const C *pEnd, KVReader_Handler onKV){
  if((pEnd - pBegin) < 2
     || *--pEnd != KVReader_VALUE_END_CHAR
     || !String_FindLast(pBegin, pEnd, KVReader_KEY_END_CHAR))
    return 1;

  E8 e;
  const C *pKey, *pKeyEnd, *pValue;
  const C *p = pBegin;
  do{
#ifndef KVReader_FORBID_EMPTY_RECORD
    p = String_Skip(p, (C)KVReader_VALUE_END_CHAR);
#endif

    p = String_SkipUntil(pKey = p, (C)KVReader_KEY_END_CHAR);
    pKeyEnd = p++;

    p = String_SkipUntil(pValue = p, (C)KVReader_VALUE_END_CHAR);

    e = onKV(pContext, pKey, pKeyEnd, pValue, p);
    if(e)
      return e;
  }while(p++ != pEnd);

  return 0;
}

#endif // KVREADER_H
