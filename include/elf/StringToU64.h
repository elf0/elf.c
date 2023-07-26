#ifndef STRING_TO_U64_H
#define STRING_TO_U64_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

#include "Type.h"

//String to U64

//From 'pBegin' to 'pEnd' MUST be a valid number string.
inline static U64 String_ToU64_2(const C *p, const C *pEnd) {
    U64 uValue = *p++ - (C)'0';
    while (p != pEnd) {
        uValue <<= 1;
        uValue |= *p++ - (C)'0';
    }
    return uValue;
}

inline static U64 String_ToU64_4(const C *p, const C *pEnd) {
    U64 uValue = *p++ - (C)'0';
    while (p != pEnd) {
        uValue <<= 2;
        uValue |= *p++ - (C)'0';
    }
    return uValue;
}

inline static U64 String_ToU64_8(const C *p, const C *pEnd) {
    U64 uValue = *p++ - (C)'0';
    while (p != pEnd) {
        uValue <<= 3;
        uValue |= *p++ - (C)'0';
    }
    return uValue;
}

inline static U64 String_ToU64(const C *p, const C *pEnd) {
    U64 uValue = *p++ - (C)'0';
    while (p != pEnd) {
        uValue *= 10;
        uValue += *p++ - (C)'0';
    }
    return uValue;
}

inline static U64 String_ToU64_16(const C *p, const C *pEnd) {
    U64 uValue = *p++ - (C)'0';
    if (uValue >= 10) {
        uValue &= 0xDF;
        uValue -= 0x11;
    }

    while (p != pEnd) {
        uValue <<= 4;
        U8 u8 = *p++ - '0';
        if (u8 >= 10) {
            u8 &= 0xDF;
            u8 -= 0x11;
        }
        uValue |= u8;
    }
    return uValue;
}

//Parse '0b' prefix youself
inline static U64 String_ParseU64_2(U64 uHead, const C **ppTail) {
    const C *p = *ppTail;
    U8 uRange;
    while ((uRange = *p - '0') < 2) {
        if (uHead >= 0x8000000000000000) {
            *ppTail = p;
            return 0xFFFFFFFFFFFFFFFF;
        }
        uHead = (uHead << 1) | uRange;
        ++p;
    }
    *ppTail = p;
    return uHead;
}

inline static U64 String_ParseU64_4(U64 uHead, const C **ppTail) {
    U64 uValue = *puValue;
    const C *p = *ppTail;
    U8 uRange;
    while ((uRange = *p - '0') < 4) {
        if (uValue >= 0x4000000000000000) {
            *ppTail = p;
            return 0xFFFFFFFFFFFFFFFF;
        }
        uHead = (uHead << 2) | uRange;
        ++p;
    }
    *ppTail = p;
    return uHead;
}

//Parse '0o' prefix youself
inline static U64 String_ParseU64_8(U64 uHead, const C **ppTail) {
    const C *p = *ppTail;
    U8 uRange;
    while ((uRange = *p - '0') < 8) {
        if (uHead >= 0x2000000000000000) {
            *ppTail = p;
            return 0xFFFFFFFFFFFFFFFF;
        }
        uHead = (uHead << 3) | uRange;
        ++p;
    }
    *ppTail = p;
    return uHead;
}

inline static E8 String_ParseU64_Max( U64 *pValue, const C **ppTail, U64 uMax ) {
    E8 e = 0;
    U64 uValue = *pValue;
    const C *p = *ppTail;
    const U64 uDiv = uMax / 10;
    const U64 uMod = uMax % 10;
    const U64 uSub = uMax - uMod;
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

inline static E8 String_ParseU64_Max9( U64 *pValue, const C **ppTail, U64 uMax ) {
    E8 e = 0;
    U64 uValue = *pValue;
    const C *p = *ppTail;
    const U64 uDiv = uMax / 10;
    const U64 uMod = uMax % 10;
    const U64 uSub = uMax - uMod;
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

inline static E8 String_ParseU64( U64 *pValue, const C **ppTail ) {
    return String_ParseU64_Max(pValue, ppTail, U64_MAX);
}

//Parse '0x' prefix youself
inline static U64 String_ParseU64_16(U64 uHead, const U8 **ppTail) {
    const U8 *p = *ppTail;
    while (1) {
        U8 uRange = *p - '0';
        if (uRange >= 10) {
            uRange &= 0xDF;
            uRange -= 0x11;
            if (uRange >= 6)
                break;

            uRange += 10;
        }

        if (uHead >= 0x1000000000000000) {
            *ppTail = p;
            return 0xFFFFFFFFFFFFFFFF;
        }
        uHead = (uHead << 4) | uRange;
        ++p;
    }
    *ppTail = p;
    return uHead;
}

// return store bits
inline static U8 U64_Bits(U64 uValue) {
    U8 uBits;
    if (uValue > 0xFFFFFFFF) {
        if (uValue > 0xFFFFFFFFFFFF) {
            if (uValue > 0xFFFFFFFFFFFFFF)
                uBits = 63;
            else
                uBits = 55;
        }
        else if (uValue > 0xFFFFFFFFFF)
            uBits = 47;
        else
            uBits = 39;
    }
    else if (uValue > 0xFFFF) {
        if (uValue > 0xFFFFFF)
            uBits = 31;
        else
            uBits = 23;
    }
    else if (uValue > 0xFF)
        uBits = 15;
    else
        uBits = 7;

    do {
        if (uValue >> uBits)
            return uBits + 1;
    } while (--uBits);
    return 1;
}

inline static U8 String_ParseU64Bits_2(U64 uValue, const C **ppTail, U64 *puValue) {
    if (String_ParseU64_2(uValue, ppTail, puValue))
        return 255;

    return U64_Bits(*puValue);
}

inline static U8 String_ParseU64Bits_4(U64 uValue, const C **ppTail, U64 *puValue) {
    if (String_ParseU64_4(uValue, ppTail, puValue))
        return 255;

    return U64_Bits(*puValue);
}

inline static U8 String_ParseU64Bits_8(U64 uValue, const C **ppTail, U64 *puValue) {
    if (String_ParseU64_8(uValue, ppTail, puValue))
        return 255;

    return U64_Bits(*puValue);
}

inline static U8 String_ParseU64Bits_10(U64 uValue, const C **ppTail, U64 *puValue) {
    if (String_ParseU64_10(uValue, ppTail, puValue))
        return 255;

    return U64_Bits(*puValue);
}

inline static U8 String_ParseU64Bits_16(U64 uValue, const C **ppTail, U64 *puValue) {
    if (String_ParseU64_16(uValue, ppTail, puValue))
        return 255;

    return U64_Bits(*puValue);
}

#endif // STRING_TO_U64_H
