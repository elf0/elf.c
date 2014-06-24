#ifndef STRING_H
#define STRING_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "Char.h"

static inline Char *String_Skip(Char *p, Char value);
static inline Char *String_SkipUntil(Char *p, Char value);
static inline Char *String_SkipDigit(Char *p);
static inline Char *String_SkipUpper(Char *p);
static inline Char *String_SkipLower(Char *p);
static inline Char *String_SkipAlpha(Char *p);
static inline Char *String_TrimEnd(Char *pBegin, Char *pEnd, Char value);
static inline Char *String_ToU32(Char *pNumber, U32 *pValue);
static inline Char *String_ToI32(Char *pNumber, I32 *pValue);
static inline Char *String_ToU64(Char *pNumber, U64 *pValue);
static inline Char *String_ToI64(Char *pNumber, I64 *pValue);
static inline  Char *String_ToDecimal32(Char *pNumber, F32 *pValue);
static inline  Char *String_ToDecimal64(Char *pNumber, F64 *pValue);
static inline  Char *String_ToF32(Char *pNumber, F32 *pValue);
static inline  Char *String_ToF64(Char *pNumber, F64 *pValue);

static inline Char *String_Skip(Char *p, Char value){
    while(*p == value)
        ++p;
    return p;
}

static inline Char *String_SkipUntil(Char *p, Char value){
    while(*p != value)
        ++p;
    return p;
}

static inline Char *String_SkipDigit(Char *p){
    while(Char_IsDigit(*p))
        ++p;
    return p;
}

static inline Char *String_SkipUpper(Char *p){
    while(Char_IsUpper(*p))
        ++p;
    return p;
}

static inline Char *String_SkipLower(Char *p){
    while(Char_IsLower(*p))
        ++p;
    return p;
}

static inline Char *String_SkipAlpha(Char *p){
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

static inline Char *String_ToU32(Char *pNumber, U32 *pValue){
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
static inline Char *String_ToI32(Char *pNumber, I32 *pValue){
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

static inline Char *String_ToU64(Char *pNumber, U64 *pValue){
    Char *p = pNumber;
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
static inline Char *String_ToI64(Char *pNumber, I64 *pValue){
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
static inline  Char *String_ToDecimal32(Char *pNumber, F32 *pValue){
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
static inline  Char *String_ToDecimal64(Char *pNumber, F64 *pValue){
    Char *p = pNumber;
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
static inline  Char *String_ToF32(Char *pNumber, F32 *pValue){
    Char *p = pNumber;

    U32 nInteger;
    p = String_ToU32(pNumber, &nInteger);
    if(p == pNumber)
        return p;

    F32 fInteger = nInteger;

    if(*p != '.'){
        *pValue = fInteger;
        return p;
    }

    Char *pDecimal = p + 1;
    F32 fDecimal;
    p = String_ToDecimal32(pDecimal, &fDecimal);
    if(p == pDecimal){
        *pValue = fInteger;
        return p;
    }

    *pValue = fInteger + fDecimal;
    return p;
}

//Only "0.0" is valid
//parse '+', '-' youself
static inline  Char *String_ToF64(Char *pNumber, F64 *pValue){
    Char *p = pNumber;

    U64 nInteger;
    p = String_ToU64(pNumber, &nInteger);
    if(p == pNumber)
        return p;

    F64 fInteger = nInteger;

    if(*p != '.'){
        *pValue = fInteger;
        return p;
    }

    Char *pDecimal = p + 1;
    F64 fDecimal;
    p = String_ToDecimal64(pDecimal, &fDecimal);
    if(p == pDecimal){
        *pValue = fInteger;
        return p;
    }

    *pValue = fInteger + fDecimal;
    return p;
}

#endif // STRING_H


