#ifndef IKVREADER_H
#define IKVREADER_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

//Structure: "Key0:Value0\n.Key1:Value1\n..Key2:Value2\n:\n"
//Buffer must ends with ":\n"

#include "String.h"

#ifndef IKVReader_KEY_END_CHAR
#define IKVReader_KEY_END_CHAR ':'
#endif

#ifndef IKVReader_VALUE_END_CHAR
#define IKVReader_VALUE_END_CHAR '\n'
#endif

#ifndef IKVReader_INDENT_CHAR
#define IKVReader_INDENT_CHAR '.'
#endif

typedef E8 (*IKVReader_Handler)(void *pContext, U8 uIndent, const C *pKey, const C *pKeyEnd, const C *pValue, const C *pValueEnd);

static inline E8 IKVReader_Parse(void *pContext, const C *pBegin, const C *pEnd, IKVReader_Handler onKV){
  size_t nSize = pEnd - pBegin;
  if(nSize < 2 || *(U16*)(pEnd - 2) != *(U16*)":\n")
    return 1;

  E8 e;
  U8 uIndent;
  const C *pKey, *pKeyEnd, *pToken;
  const C *p = pBegin;
  do{
    p = String_Skip(pToken = p, (C)IKVReader_INDENT_CHAR);
    uIndent = (U8)(p - pToken);
    p = String_SkipUntil(pKey = p, (C)IKVReader_KEY_END_CHAR);
    pKeyEnd = p++;
    p = String_SkipUntil(pToken = p, (C)IKVReader_VALUE_END_CHAR);

    if(e = onKV(pContext, uIndent, pKey, pKeyEnd, pToken, p++))
      return e;
  }while(p != pEnd);

  return 0;
}

#endif // IKVREADER_H
