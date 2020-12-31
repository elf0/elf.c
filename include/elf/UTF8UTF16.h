#ifndef UTF8UTF16_H
#define UTF8UTF16_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

#include "UTF8.h"
#include "UTF16.h"

static C16 *UTF_8To16(C16 *p16, const C16 *p16End, const C **pp8, const C *p8End) {
    const C *p8 = *pp8;
    while (p8 != p8End) {
        const C *p8Begin = p8;
        C32 code = UTF8_Read(&p8);
        if (p16End < p16 + UTF16_Bytes(code)) {
            *pp8 = p8Begin;
            break;
        }
        p16 = UTF16_Write(p16, code);
    }
    *pp8 = p8;
    return p16;
}

inline
static C *UTF_16To8(C *p8, const C *p8End, const C16 **pp16, const C16 *p16End) {
    const C16 *p16 = *pp16;
    while (p16 != p16End) {
        const C16 *p16Begin = p16;
        C32 code = UTF16_Read(&p16);
        U8 uBytes = UTF8_Bytes(code);
        if (p8End < p8 + uBytes) {
            *pp16 = p16Begin;
            return p8;
        }
        p8 = UTF8_Write(p8, code);
    }
    *pp16 = p16;
    return p8;
}

#endif //UTF8UTF16_H

