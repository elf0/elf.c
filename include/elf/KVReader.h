#ifndef KVREADER_H
#define KVREADER_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

//Structure: "Key0:Value0\nKey1:Value0\n:Value1\n:\n"
//Buffer must ends with ":\n"

#include "String.h"

typedef E8 (*KVReader_Handler)(void *pContext, const C *pBegin, const C *pEnd);

typedef struct{
  void *pContext;
  KVReader_Handler onKey;
  KVReader_Handler onValue;
}KVReader;

static inline void KVReader_Initialize(KVReader *pReader
                                       , void *pContext
                                       , KVReader_Handler onKey
                                       , KVReader_Handler onValue){
  pReader->pContext = pContext;
  pReader->onKey = onKey;
  pReader->onValue = onValue;
}

static inline E8 KVReader_Parse(KVReader *pReader, const C *pBegin, const C *pEnd){
  size_t nSize = pEnd - pBegin;
  if(nSize < 2)
    return 1;

  if(*(U16*)(pEnd - 2) != *(U16*)":\n")
    return 1;

  E8 err;
  const C *pData;
  const C *p = pBegin;
  while(1){
    p = String_SkipUntil(pData = p, (C)':');
    if(err = pReader->onKey(pReader->pContext, pData, p++))
      return err;

    while(1){
      p = String_SkipUntil(pData = p, (C)'\n');
      if(err = pReader->onValue(pReader->pContext, pData, p++))
        return err;

      if(p == pEnd)
        return 0;

      if(*p != (C)':')
        break;

      ++p;
    }
  }
}

#endif // KVREADER_H
