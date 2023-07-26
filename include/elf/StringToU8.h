#ifndef STRING_TO_U8_H
#define STRING_TO_U8_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

#include "Type.h"

//String to U8

//From 'pBegin' to 'pEnd' MUST be a valid number string.
inline
    static U8 String_ToU8_2(const C *pBegin, const C *pEnd) {
    const C *p = pBegin;
    U8 uValue = *p++ - '0';
    while (p != pEnd) {
        uValue <<= 1;
        uValue |= *p++ - '0';
    }

    return uValue;
}

inline
    static U8 String_ToU8_4(const C *pBegin, const C *pEnd) {
    const C *p = pBegin;
    U8 uValue = *p++ - '0';
    while (p != pEnd) {
        uValue <<= 2;
        uValue |= *p++ - '0';
    }

    return uValue;
}

inline
    static U8 String_ToU8_8(const C *pBegin, const C *pEnd) {
    const C *p = pBegin;
    U8 uValue = *p++ - '0';
    while (p != pEnd) {
        uValue <<= 3;
        uValue |= *p++ - '0';
    }

    return uValue;
}

inline
    static U8 String_ToU8_10(const C *pBegin, const C *pEnd) {
    const C *p = pBegin;
    U8 uValue = *p++ - '0';
    while (p != pEnd) {
        uValue *= 10;
        uValue += *p++ - '0';
    }

    return uValue;
}

inline
    static U8 String_ToU8_16(const C *pBegin, const C *pEnd) {
    const C *p = pBegin;
    U8 uValue = *p++ - '0';
    if (uValue > 9) {
        uValue &= 0xDF;
        uValue -= 0x11;
    }

    while (p != pEnd) {
        uValue <<= 4;
        U8 u8 = *p++ - '0';
        if (u8 > 9) {
            u8 &= 0xDF;
            u8 -= 0x11;
        }

        uValue |= u8;

    }

    return uValue;
}
//Parse '0b' prefix youself
inline
    static E8 String_ParseU8_2(U8 uValue, const C **ppTail, U8 *puValue) {
    const C *p = *ppTail;
    U8 uRange;
    while ((uRange = *p - '0') < 2) {
        if (uValue > 0x7F) {
            *ppTail = p;
            return 1;
        }

        uValue <<= 1;
        uValue |= uRange;
        ++p;
    }

    *ppTail = p;
    *puValue = uValue;
    return 0;
}

inline
    static E8 String_ParseU8_4(U8 uValue, const C **ppTail, U8 *puValue) {
    const C *p = *ppTail;
    U8 uRange;
    while ((uRange = *p - '0') < 4) {
        if (uValue > 0x3F) {
            *ppTail = p;
            return 1;
        }

        uValue <<= 2;
        uValue |= uRange;
        ++p;
    }

    *ppTail = p;
    *puValue = uValue;
    return 0;
}

//Parse '0o' prefix youself
inline
    static E8 String_ParseU8_8(U8 uValue, const C **ppTail, U8 *puValue) {
    const C *p = *ppTail;
    U8 uRange;
    while ((uRange = *p - '0') < 8) {
        if (uValue > 0x1F) {
            *ppTail = p;
            return 1;
        }

        uValue <<= 3;
        uValue |= uRange;
        ++p;
    }

    *ppTail = p;
    *puValue = uValue;
    return 0;
}

// First 2 digits won't overflow
inline
    static U8 String_ParseU8_10_Head2(const C **ppHead) {
    const C *p = *ppHead;
    U8 uRange =  *p - '0';
    if (uRange > 9)
        return 0;

    U8 uValue = uRange;
    uRange = *++p - (C)'0';
    if (uRange < 10) {
        ++p;
        uValue *= 10;
        uValue += uRange;
    }
    *ppHead = p;
    return uValue;
}

inline static E8 String_ParseU8(U8 *puHead, const C **ppTail) {
    U8 uValue = *puHead;
    const C *p = *ppTail;
    U8 u8;
    while ((u8 = *p - '0') < 10) {
#define U8_10_LIMIT (U8_MAX / 10)
        if (uValue < U8_10_LIMIT)
            uValue = uValue * 10 + uRange;
        else if (uValue == U8_10_LIMIT && uRange < 6)
            uValue = U8_MAX - 5 + uRange;
        else {
            *puHead = uValue;
            *ppTail = p;
            return 1;
        }
        ++p;
    }
    *puHead = u16;
    *ppTail = p;
    return 0;
}

//Parse '0x' prefix youself
inline
    static E8 String_ParseU8_16(U8 uValue, const C **ppTail, U8 *puValue) {
    const C *p = *ppTail;
    while (1) {
        U8 uRange = *p - '0';
        if (uRange > 9) {
            uRange &= 0xDF;
            uRange -= 0x11;
            if (uRange > 5)
                break;

            uRange += 10;
        }

        if (uValue > 0xF) {
            *ppTail = p;
            return 1;
        }

        uValue <<= 4;
        uValue |= uRange;
        ++p;
    }

    *ppTail = p;
    *puValue = uValue;
    return 0;
}

#endif // STRING_TO_U8_H
