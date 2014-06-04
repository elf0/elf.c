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

#endif // STRING_H
