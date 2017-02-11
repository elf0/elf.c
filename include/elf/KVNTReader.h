#ifndef KVNTREADER_H
#define KVNTREADER_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

//Structure: "Key0:Value0\nKey1:Value1\nKey2:Value2\n"

#include "String.h"

#ifndef KVNTReader_KEY_END_CHAR
#define KVNTReader_KEY_END_CHAR ':'
#endif

#ifndef KVNTReader_VALUE_END_CHAR
#define KVNTReader_VALUE_END_CHAR '\n'
#endif

typedef E8 (*KVNTReader_Handler)(void *pContext, const C *pKey, const C *pKeyEnd, const C *pValue, const C *pValueEnd);

static inline E8 KVNTReader_Parse(void *pContext, const C *pBegin, const C *pEnd, KVNTReader_Handler onKV){
  if((pEnd - pBegin) < 2
     || *--pEnd != KVNTReader_VALUE_END_CHAR
     || !String_FindLast(pBegin, pEnd, KVNTReader_KEY_END_CHAR))
    return 1;

  E8 e;
  const C *pKey, *pKeyEnd, *pValue;
  const C *p = pBegin;
  do{
#ifndef KVNTReader_FORBID_EMPTY_RECORD
    p = String_Skip(p, (C)KVNTReader_VALUE_END_CHAR);
#endif

    p = String_SkipUntil(pKey = p, (C)KVNTReader_KEY_END_CHAR);
    pKeyEnd = p++;

    p = String_SkipUntil(pValue = p, (C)KVNTReader_VALUE_END_CHAR);

    e = onKV(pContext, pKey, pKeyEnd, pValue, p);
    if(e)
      return e;
  }while(p++ != pEnd);

  return 0;
}

#endif // KVNTREADER_H
