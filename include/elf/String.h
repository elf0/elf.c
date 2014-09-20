#ifndef STRING_H
#define STRING_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "Char.h"

typedef void (*String_EventHandler)(void *pContext, const Char *pBegin, const Char *pEnd);

static inline const Char *String_Skip(const Char *p, const Char value);
static inline const Char *String_SkipUntil(const Char *p, Char value);
static inline const Char *String_SkipDigit(const Char *p);
static inline const Char *String_SkipUpper(const Char *p);
static inline const Char *String_SkipLower(const Char *p);
static inline const Char *String_SkipAlpha(const Char *p);
static inline Char *String_TrimEnd(Char *pBegin, Char *pEnd, Char value);
static inline void String_Split(const Char *pBegin, const Char *pEnd, Char cSplitter, String_EventHandler onSubString, void *pContext);
static inline  U32 String_ToU32(const Char *pBegin, const Char *pEnd);
static inline Char *String_ParseU32(Char *pNumber, U32 *pValue);
static inline Char *String_ParseI32(Char *pNumber, I32 *pValue);
static inline const Char *String_ParseU64(const Char *pNumber, U64 *pValue);
static inline Char *String_ParseI64(Char *pNumber, I64 *pValue);
static inline  Char *String_ParseDecimal32(Char *pNumber, F32 *pValue);
static inline  const Char *String_ParseDecimal64(const Char *pNumber, F64 *pValue);
static inline  const Char *String_ParseF32(const Char *pNumber, F32 *pValue);
static inline  const Char *String_ParseF64(const Char *pNumber, F64 *pValue);

static inline Bool String_Equal2(const Char *pLeft, const Char *pRight);
static inline Bool String_Equal4(const Char *pLeft, const Char *pRight);
static inline Bool String_Equal6(const Char *pLeft, const Char *pRight4, const Char *pRight2);
static inline Bool String_Equal8(const Char *pLeft, const Char *pRight);

static inline const Char *String_Skip(const Char *p, const Char value){
    while(*p == value)
        ++p;
    return p;
}

static inline const Char *String_SkipUntil(const Char *p, Char value){
    while(*p != value)
        ++p;
    return p;
}

static inline const Char *String_SkipDigit(const Char *p){
    while(Char_IsDigit(*p))
        ++p;
    return p;
}

static inline const Char *String_SkipUpper(const Char *p){
    while(Char_IsUpper(*p))
        ++p;
    return p;
}

static inline const Char *String_SkipLower(const Char *p){
    while(Char_IsLower(*p))
        ++p;
    return p;
}

static inline const Char *String_SkipAlpha(const Char *p){
    while(Char_IsAlpha(*p))
        ++p;
    return p;
}

static inline Char *String_TrimEnd(Char *pBegin, Char *pEnd, Char value){
    Char *pREnd = pBegin - 1;
    Char *p = pEnd - 1;
    while(p != pREnd){
        if(*p != value)
            break;
        --p;
    }
    return ++p;
}

//*pEnd MUST equal cSplitter
static inline void String_Split(const Char *pBegin, const Char *pEnd, Char cSplitter, String_EventHandler onSubString, void *pContext){
    const Char *pSubString;
    const Char *p = pBegin;
    while(p < pEnd){
        pSubString = p;
        p = String_SkipUntil(p, cSplitter);
        onSubString(pContext, pSubString, p);
        ++p;
    }
}

static inline  U32 String_ToU32(const Char *pBegin, const Char *pEnd){
    const Char *p = pBegin;
    U32 nValue = *p - '0';
    ++p;

    while(p != pEnd){
        nValue = 10 * nValue + (*p - '0');
        ++p;
    }

    return nValue;
}

static inline Char *String_ParseU32(Char *pNumber, U32 *pValue){
    Char *p = pNumber;
    if(!Char_IsDigit(*p))
        return p;

    U32 nValue = *p - '0';
    ++p;

    while(Char_IsDigit(*p)){
        nValue = 10 * nValue + (*p - '0');
        ++p;
    }

    *pValue = nValue;
    return p;
}

//parse '+', '-' youself
static inline Char *String_ParseI32(Char *pNumber, I32 *pValue){
    Char *p = pNumber;
    if(!Char_IsDigit(*p))
        return p;

    I32 nValue = *p - '0';
    ++p;

    while(Char_IsDigit(*p)){
        nValue = 10 * nValue + (*p - '0');
        ++p;
    }

    *pValue = nValue;
    return p;
}

static inline const Char *String_ParseU64(const Char *pNumber, U64 *pValue){
    const Char *p = pNumber;
    if(!Char_IsDigit(*p))
        return p;

    U64 nValue = *p - '0';
    ++p;

    while(Char_IsDigit(*p)){
        nValue = 10 * nValue + (*p - '0');
        ++p;
    }

    *pValue = nValue;
    return p;
}

//parse '+', '-' youself
static inline Char *String_ParseI64(Char *pNumber, I64 *pValue){
    Char *p = pNumber;
    if(!Char_IsDigit(*p))
        return p;

    I64 nValue = *p - '0';
    ++p;

    while(Char_IsDigit(*p)){
        nValue = 10 * nValue + (*p - '0');
        ++p;
    }

    *pValue = nValue;
    return p;
}

//Decimal part
//FIXME: Precision problem
static inline  Char *String_ParseDecimal32(Char *pNumber, F32 *pValue){
    Char *p = pNumber;
    if(!Char_IsDigit(*p))
        return p;

    U32 nDelta = 10;
    U32 nValue = *p - '0';
    ++p;

    while(Char_IsDigit(*p)){
        nDelta *= 10;
        nValue = 10 * nValue + (*p - '0');
        ++p;
    }

    *pValue = (F32)nValue / (F32)nDelta;
    return p;
}

//Decimal part
static inline  const Char *String_ParseDecimal64(const Char *pNumber, F64 *pValue){
    const Char *p = pNumber;
    if(!Char_IsDigit(*p))
        return p;

    U64 nDelta = 10;
    U64 nValue = *p - '0';
    ++p;

    while(Char_IsDigit(*p)){
        nDelta *= 10;
        nValue = 10 * nValue + (*p - '0');
        ++p;
    }

    *pValue = (F64)nValue / (F64)nDelta;
    return p;
}

//Only "0.0" is valid
//parse '+', '-' youself
//FIXME: Precision problem
static inline  Char *String_ParseF32(Char *pNumber, F32 *pValue){
    Char *p = pNumber;

    U32 nInteger;
    p = String_ParseU32(pNumber, &nInteger);
    if(p == pNumber)
        return p;

    F32 fInteger = nInteger;

    if(*p != '.'){
        *pValue = fInteger;
        return p;
    }

    Char *pDecimal = p + 1;
    F32 fDecimal;
    p = String_ParseDecimal32(pDecimal, &fDecimal);
    if(p == pDecimal){
        *pValue = fInteger;
        return p;
    }

    *pValue = fInteger + fDecimal;
    return p;
}

//Only "0.0" is valid
//parse '+', '-' youself
static inline const Char *String_ParseF64(const Char *pNumber, F64 *pValue){
    const Char *p = pNumber;

    U64 nInteger;
    p = String_ParseU64(pNumber, &nInteger);
    if(p == pNumber)
        return p;

    F64 fInteger = nInteger;

    if(*p != '.'){
        *pValue = fInteger;
        return p;
    }

    const Char *pDecimal = p + 1;
    F64 fDecimal;
    p = String_ParseDecimal64(pDecimal, &fDecimal);
    if(p == pDecimal){
        *pValue = fInteger;
        return p;
    }

    *pValue = fInteger + fDecimal;
    return p;
}

static inline Bool String_Equal2(const Char *pLeft, const Char *pRight){
    return *(U16*)pLeft == *(U16*)pRight;
}

static inline Bool String_Equal4(const Char *pLeft, const Char *pRight){
    return *(U32*)pLeft == *(U32*)pRight;
}

static inline Bool String_Equal6(const Char *pLeft, const Char *pRight4, const Char *pRight2){
    return String_Equal4(pLeft, pRight4) && String_Equal2(pLeft + 4, pRight2);
}

static inline Bool String_Equal8(const Char *pLeft, const Char *pRight){
    return *(U64*)pLeft == *(U64*)pRight;
}

#endif // STRING_H




