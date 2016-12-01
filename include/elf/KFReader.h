#ifndef KFREADER_H
#define KFREADER_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

#include "String.h"

#ifndef KFReader_FILE_END
#define KFReader_FILE_END ": .\n"
#endif

#ifndef KFReader_KEY_END_CHAR
#define KFReader_KEY_END_CHAR ((C)':')
#endif

#ifndef KFReader_INTEGER_END_CHAR
#define KFReader_INTEGER_END_CHAR ((C)'.')
#endif

#ifndef KFReader_TAIL_END_CHAR
#define KFReader_TAIL_END_CHAR ((C)'\n')
#endif


typedef E8 (*KFReader_Handler)(void *pContext, const C *pKey, const C *pKeyEnd, const C *pInteger, const C *pIntegerEnd, const C *pTail, const C *pTailEnd);

static inline E8 KFReader_Parse(void *pContext, const C *pBegin, const C *pEnd, KFReader_Handler onKF){
  if((pEnd - pBegin) < 4 || *(U32*)(pEnd - 4) != *(U32*)KFReader_FILE_END)
    return 1;
  --pEnd;

  E8 e;
  const C *pKey, *pKeyEnd, *pInteger, *pIntegerEnd, *pTail;
  const C *p = pBegin;
  while(1){
#ifndef KFReader_FORBID_EMPTY_RECORD
    p = String_Skip(p, KFReader_TAIL_END_CHAR);
#endif

    pKey = p;
    p = String_SkipUntil(p, KFReader_KEY_END_CHAR);
    pKeyEnd = p++;

    if(*p == ' ')
     ++p;

    pInteger = p;
    p = String_SkipUntil(p, KFReader_INTEGER_END_CHAR);
    pIntegerEnd = p++;

    pTail = p;
    p = String_SkipUntil(p, KFReader_TAIL_END_CHAR);
    if(p == pEnd)
      break;

    e = onKF(pContext, pKey, pKeyEnd, pInteger, pIntegerEnd, pTail, p++);
    if(e)
      return e;
  }

  return 0;
}

#endif // KFREADER_H
