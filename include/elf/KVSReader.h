#ifndef KVSREADER_H
#define KVSREADER_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

//Structure: "Key0:Value0\nKey1:Value0\n:Value1\n:\n"
//Buffer must ends with ":\n"

#include "String.h"

#ifndef KVSReader_FILE_END
#define KVSReader_FILE_END ":\n"
#endif

#ifndef KVSReader_KEY_END_CHAR
#define KVSReader_KEY_END_CHAR ':'
#endif

#ifndef KVSReader_VALUE_END_CHAR
#define KVSReader_VALUE_END_CHAR '\n'
#endif

#ifndef KVSReader_VALUE_CONTINUE_CHAR
#define KVSReader_VALUE_CONTINUE_CHAR ' '
#endif

#ifndef KVSReader_OMIT_EMPTY_LINE
#define KVSReader_OMIT_EMPTY_LINE ' '
#endif

typedef E8 (*KVReader_KVHandler)(void *pContext, const C *pKey, const C *pKeyEnd, const C *pValue, const C *pValueEnd);
typedef E8 (*KVSReader_ValueHandler)(void *pContext, const C *pBegin, const C *pEnd);

static inline E8 KVSReader_Parse(void *pContext, const C *pBegin, const C *pEnd
                                 , KVReader_KVHandler onKV, KVSReader_ValueHandler onValue){
  size_t nSize = pEnd - pBegin;
  if(nSize < 2 || *(U16*)(pEnd - 2) != *(U16*)KVSReader_FILE_END)
    return 1;
  --pEnd;

  E8 e;
  const C *pKey, *pKeyEnd, *pValue;
  const C *p = pBegin;
  while(1){
    p = String_SkipUntil(pKey = p, (C)KVSReader_KEY_END_CHAR);
    pKeyEnd = p++;

    p = String_SkipUntil(pValue = p, (C)KVSReader_VALUE_END_CHAR);
    if(p == pEnd)
      break;

    if(e = onKV(pContext, pKey, pKeyEnd, pValue, p++))
      return e;

    while(*p == (C)KVSReader_VALUE_CONTINUE_CHAR){
      p = String_SkipUntil(pValue = ++p, (C)KVSReader_VALUE_END_CHAR);
      if(p == pEnd)
        return 1;

      if(e = onValue(pContext, pValue, p++))
        return e;
    }

#ifndef KVSReader_FORBID_EMPTY_LINE
    p = String_Skip(p, (C)KVSReader_VALUE_END_CHAR);
#endif
  }

  return 0;
}

#endif // KVSREADER_H
