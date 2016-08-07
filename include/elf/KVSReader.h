#ifndef KVSREADER_H
#define KVSREADER_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

//Structure: "Key0:Value0\nKey1:Value0\n:Value1\n:\n"
//Buffer must ends with ":\n"

#include "String.h"

#ifndef KVSReader_KEY_END_CHAR
#define KVSReader_KEY_END_CHAR ':'
#endif

#ifndef KVSReader_VALUE_END_CHAR
#define KVSReader_VALUE_END_CHAR '\n'
#endif

#ifndef KVSReader_VALUE_CONTINUE_CHAR
#define KVSReader_VALUE_CONTINUE_CHAR '.'
#endif

typedef struct KVSReader KVSReader;
typedef E8 (*KVSReader_Handler)(KVSReader *pReader, const C *pBegin, const C *pEnd);

struct KVSReader{
  void *pContext;
  KVSReader_Handler onKey;
  KVSReader_Handler onValue;
};

static inline void KVSReader_Initialize(KVSReader *pReader
                                       , void *pContext
                                       , KVSReader_Handler onKey
                                       , KVSReader_Handler onValue){
  pReader->pContext = pContext;
  pReader->onKey = onKey;
  pReader->onValue = onValue;
}

static inline E8 KVSReader_Parse(KVSReader *pReader, const C *pBegin, const C *pEnd){
  size_t nSize = pEnd - pBegin;
  if(nSize < 2)
    return 1;

  if(*(U16*)(pEnd - 2) != *(U16*)":\n")
    return 1;

  E8 err;
  const C *pData;
  const C *p = pBegin;
  while(1){
    p = String_SkipUntil(pData = p, (C)KVSReader_KEY_END_CHAR);
    if(err = pReader->onKey(pReader, pData, p++))
      return err;

    while(1){
      p = String_SkipUntil(pData = p, (C)KVSReader_VALUE_END_CHAR);
      if(err = pReader->onValue(pReader, pData, p++))
        return err;

      if(p == pEnd)
        return 0;

      if(*p != (C)KVSReader_VALUE_CONTINUE_CHAR)
        break;

      ++p;
    }
  }
}

#endif // KVSREADER_H
