#ifndef LINEREADER_H
#define LINEREADER_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

//Buffer must ends with ":\n"
//Line: "Key:Value\n"

#include "String.h"

typedef E8 (*LineReader_Handler)(void *pContext, const C *pLine, const C *pLineEnd);

static inline E8 LineReader_Parse(void *pContext, const C *pBegin, const C *pEnd, LineReader_Handler onLine){
  size_t nSize = pEnd - pBegin;
  if(nSize < 1)
    return 1;

  --pEnd;
  if(*pEnd != 0x0A)
    return 1;

  E8 err;
  const C *pLine;
  const C *p = pBegin;
  while(true){
    p = String_SkipUntil(pLine = p, 0x0A);
    if(p == pEnd)
      break;

    err = onLine(pContext, pLine, p++);
    if(err)
      return err;
  }

  return 0;
}

#endif // LINEREADER_H
