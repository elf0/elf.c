#ifndef UTF8UTF16_H
#define UTF8UTF16_H

//License: Public Domain
//Author: elf
//EMail: elf000@zoho.com

#include "UTF8.h"
#include "UTF16.h"

inline static U16 UTF_Bytes8To16(const C *p, const C *pEnd) {
    U32 uBytes = 0;
    while (p != pEnd) {
        C c = *p;
        if (c < 0xF0) {
            uBytes += 2;
            if (c < 0x80) // 0xxxxxxx
                ++p;
            else if (c < 0xE0) // 110xxxxx 10xxxxxx
                p += 2;
            else if (c < 0xF0) // 1110xxxx 10xxxxxx 10xxxxxx
                p += 3;
        }
        else {
            uBytes += 4;
            p += 4; // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        }
    }
    return uBytes;
}

inline static C16 *UTF_8To16(C16 *p16, const C16 *p16End, const C **pp8, const C *p8End) {
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

inline static C *UTF_16To8(C *p8, const C *p8End, const C16 **pp16, const C16 *p16End) {
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

