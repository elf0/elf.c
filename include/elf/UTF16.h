#ifndef UTF16_H
#define UTF16_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

// Unicode: [0, 0x10FFFF]
// Surrogate: [0xD800, 0xDFFF]
// High surrogate: [0xD800, 0xDBFF]
// Low surrogate: [0xDC00, 0xDFFF]

inline
static B UTF16_IsBasic(C16 c) {
    return (c >> 11) != 0x1B;
}

inline
static B UTF16_IsSurrogate(C16 c) {
    return (c >> 11) == 0x1B;
}

inline
static B UTF16_IsLead(C16 c) {
    return (c >> 10) == 0x36;
}

inline
static B UTF16_NotLead(C16 c) {
    return (c >> 10) != 0x36;
}

inline
static B UTF16_IsTrail(C16 c) {
    return (c >> 10) == 0x37;
}

inline
static B UTF16_NotTrail(C16 c) {
    return (c >> 10) != 0x37;
}

inline
static C32 UTF16_Surrogate(C32 lead, C16 trail) {
    lead &= 0x03FF;
    lead <<= 10;
    trail &= 0x03FF;
    return 0x10000 + (lead | trail);
}

inline
static U64 UTF16_Count(const C16 *p, const C16 *pEnd) {
    U64 uCount = 0;
    while (p != pEnd) {
        ++uCount;
        if ((*p++ >> 11) == 0x1B)
            ++p;
    }
    return uCount;
}

inline
static B UTF16_Valid(const C16 *p, const C16 *pEnd) {
    while(p != pEnd) {
        C16 c16 = *p++;
        if ((c16 >> 11) == 0x1B) {
            if (c16 > 0xDBFF || p == pEnd || (*p++ >> 10) != 0x37)
                return 0;
        }
    }
    return 1;
}

inline
static C32 UTF16_Parse(const C16 **ppBegin, const C16 *pEnd) {
    const C16 *p = *ppBegin;
    C32 c32 = *p++;
    if ((c32 >> 11) == 0x1B) {
        if (c32 > 0xDBFF || p == pEnd)
            c32 |= 0x80000000;
        else {
            C16 c16 = *p++;
            if ((c16 >> 10) == 0x37)
                c32 = UTF16_Surrogate(c32, c16);
            else {
                --p;
                c32 |= 0x80000000;
            }
        }
    }
    *ppBegin = p;
    return c32;
}
inline
static U64 UTF16_ParseCount(const C16 **ppBegin, const C16 *pEnd) {
    U64 uCount = 0;
    const C16 *p = *ppBegin;
    while (p != pEnd) {
        C32 c32 = *p++;
        if ((c32 >> 11) == 0x1B) {
            if (c32 > 0xDBFF || p == pEnd) {
                --p;
                uCount |= 0x8000000000000000;
                break;
            }
            else if ((*p++ >> 10) != 0x37) {
                p -= 2;
                uCount |= 0x8000000000000000;
                break;
            }
        }
        ++uCount;
    }
    *ppBegin = p;
    return uCount;
}

//caller must input valid utf-16 string
inline
static C32 UTF16_Read(const C16 **ppString) {
    const C16 *p = *ppString;
    C32 c32 = *p++;
    if ((c32 >> 11) == 0x1B)
        c32 = UTF16_Surrogate(c32, *p++);
    *ppString = p;
    return c32;
}

inline
static U8 UTF16_Bytes(C32 value) {
    return value < 0x10000? 2 : 4;
}

//caller must input valid utf value
inline
static C16 *UTF16_Write(C16 *pBuffer, C32 value) {
    if (value < 0x10000) {
        //        assert(value < 0xD800 || value > 0xDFFF);
        *pBuffer = value;
    }
    else{
        //        assert(value < 0x110000);
        value -= 0x10000;
        *pBuffer++ = 0xD800 | (value >> 10);
        *pBuffer = 0xDC00 | (value & 0x03FF);
    }

    return ++pBuffer;
}

#endif //UTF16_H
