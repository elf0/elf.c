#ifndef CONFIGREADER_H
#define CONFIGREADER_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

//Buffer must ends with ":\n"
//Line: "Key:Value\n"

typedef void (*ConfigReader_Handler)(const C *pKey, const C *pKeyEnd, const C *pValue, const C *pValueEnd);

static inline E8 ConfigReader_Parse(const C *pBegin, const C *pEnd, ConfigReader_Handler onKV){
  size_t nSize = pEnd - pBegin;
  if(nSize < 2)
    return 1;

  const C *pEnd0 = &pEnd[-2];
  const C *pEnd1 = &pEnd[-1];
  if(*pEnd0 != 0x3A || *pEnd1 != 0x0A)
    return 1;

  const C *pKey, *pKeyEnd;
  const C *pValue;
  const C *p = pBegin;
  while(true){
    pKey = p;
    p = String_SkipUntil(p, 0x3A);
    if(p == pEnd0)
      return p == pKey? 0 : 1;
    pKeyEnd = p++;

    pValue = p;
    p = String_SkipUntil(p, 0x0A);
    if(p == pEnd1)
      return 1;

    onKV(pKey, pKeyEnd, pValue, p++);
  }

  return 0;
}

#endif // CONFIGREADER_H
