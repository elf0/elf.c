#ifndef STRING_H
#define STRING_H
//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "Char.h"

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

static inline U64 String_ToU64(Char *p){
    U64 nValue = 0;

    while(Char_IsDigit(*p)){
        nValue = 10 * nValue + (*p - '0');
        ++p;
    }

    return nValue;
}

//"+xxx" not supported
static inline I64 String_ToI64(Char *p){
    Bool bNegtive;
    if(*p != '-')
        bNegtive = false;
    else{
        bNegtive = true;
        ++p;
    }

    I64 nValue = 0;
    while(Char_IsDigit(*p)){
        nValue = 10 * nValue + (*p - '0');
        ++p;
    }

    if(bNegtive)
        nValue = -nValue;

    return nValue;
}
#endif // STRING_H

