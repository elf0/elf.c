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

inline static E8 String_ParseU8_Max( U8 *pValue, const C **ppTail, U8 uMax ) {
    E8 e = 0;
    U8 uValue = *pValue;
    const C *p = *ppTail;
    const U8 uDiv = uMax / 10;
    const U8 uMod = uMax % 10;
    const U8 uSub = uMax - uMod;
    U8 uRange;
    while ((uRange = *p - '0') < 10) {
        if (uValue < uDiv)
            uValue = uValue * 10 + uRange;
        else if (uValue == uDiv && uRange <= uMod)
            uValue = uSub + uRange;
        else {
            e = 1;
            break;
        }
        ++p;
    }
    *pValue = uValue;
    *ppTail = p;
    return e;
}

inline static E8 String_ParseU8_Max9( U8 *pValue, const C **ppTail, U8 uMax ) {
    E8 e = 0;
    U8 uValue = *pValue;
    const C *p = *ppTail;
    const U8 uDiv = uMax / 10;
    const U8 uMod = uMax % 10;
    const U8 uSub = uMax - uMod;
    U8 uRange;
    while ((uRange = *p - '0') < 10) {
        if (uValue <= uDiv)
            uValue = uValue * 10 + uRange;
        else {
            e = 1;
            break;
        }
        ++p;
    }
    *pValue = uValue;
    *ppTail = p;
    return e;
}

inline static U8 String_ParseU8(U8 uHead, const C **ppTail) {
    const C *p = *ppTail;
    U8 uRange;
    while ((uRange = *p - '0') < 10) {
        if (uHead < 0x19)
            uHead = uHead * 10 + uRange;
        else if (uHead == 0x19 && uRange < 6)
            uHead = 0xFA + uRange;
        else {
            *ppTail = p;
            return 0;
        }
        ++p;
    }
    *ppTail = p;
    return uHead;
}

//Parse '0x' prefix youself
inline static E8 String_ParseU8_16(U8 *puValue, const C **ppTail) {
    E8 e = 0;
    U8 uValue = *puValue;
    const C *p = *ppTail;
    while (1) {
        U8 uRange = *p - '0';
        if (uRange >= 10) {
            uRange &= 0xDF;
            uRange -= 0x11;
            if (uRange >= 6)
                break;

            uRange += 10;
        }

        if (uValue >= 0x10) {
            e = 1;
            break;
        }
        uValue = (uValue << 4) | uRange;
        ++p;
    }

    *puValue = uValue;
    *ppTail = p;
    return e;
}

#endif // STRING_TO_U8_H
