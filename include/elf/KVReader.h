#ifndef KVREADER_H
#define KVREADER_H

//License: Public Domain
//Author: elf
//EMail:


#ifndef KVReader_KEY_END_CHAR
#define KVReader_KEY_END_CHAR ':'
#endif

#ifndef KVReader_VALUE_END_CHAR
#define KVReader_VALUE_END_CHAR '\n'
#endif


typedef E8 (*KVReader_Handler)(void *pContext, const C *pKey, const C *pKeyEnd, const C *pValue, const C *pValueEnd);

static inline E8 KVReader_Parse(void *pContext, const C *pBegin, const C *pEnd, KVReader_Handler onKV){
    const C *pREnd = pBegin - 1;
    const C *pLastKeyEnd = --pEnd;
    while (1) {
        if (pEnd == pREnd)
            return 1;

        if (*pEnd == KVReader_VALUE_END_CHAR)
            break;

        --pEnd;
    }

    while (1) {
        if (pLastKeyEnd == pREnd)
            return 1;

        if (*pLastKeyEnd == KVReader_KEY_END_CHAR)
            break;

        --pLastKeyEnd;
    }

    if (pEnd <= pLastKeyEnd)
        return 1;

    E8 e;
    const C *pKey, *pKeyEnd, *pValue;
    const C *p = pBegin;
    do {
        pKey = p;
        while (*p != KVReader_KEY_END_CHAR)
            ++p;

        pKeyEnd = p++;
        pValue = p;
        while (*p != KVReader_VALUE_END_CHAR)
            ++p;

        e = onKV(pContext, pKey, pKeyEnd, pValue, p);
        if(e)
            return e;
    } while (p++ != pEnd);
    return 0;
}

#endif // KVREADER_H
