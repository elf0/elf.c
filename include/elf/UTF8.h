#ifndef UTF8_H
#define UTF8_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

// Max utf8: F4 8F BF BF
// Unicode: [0, 0x10FFFF]
// Surrogate: [0xD800, 0xDFFF]

#define UNICODE_INVALID(c32) (c32 > 0x10FFFF)

inline
static const C *UTF8_Skip(const C *p) {
    U8 u8 = *p;
    if (u8 < 0x80) // 0xxxxxxx
        return p + 1;

    if (u8 < 0xE0) // 110xxxxx 10xxxxxx
        return p + 2;

    if (u8 < 0xF0) // 1110xxxx 10xxxxxx 10xxxxxx
        return p + 3;

    return p + 4; // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
}

inline
static const C *UTF8_SkipTail(C cHead, const C *pTail) {
    if (cHead < 0xE0) // 110xxxxx 10xxxxxx
        return pTail + 1;

    if (cHead < 0xF0) // 1110xxxx 10xxxxxx 10xxxxxx
        return pTail + 2;

    return pTail + 3; // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
}

inline
static U64 UTF8_Count(const C *p, const C *pEnd) {
    U64 uCount = 0;
    while (p != pEnd) {
        ++uCount;
        C c = *p++;
        if (c > 0x7F) { //0xxxxxxx
            ++p;
            if (c > 0xDF) { //1110xxxx 10xxxxxx 10xxxxxx
                ++p;
                if (c > 0xEF) //11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
                    ++p;
            }
        }
    }
    return uCount;
}

//caller must input valid utf-8 string
inline
static C32 UTF8_ReadTail(C32 cHead, const C **ppTail) {
    const C *p = *ppTail;
    if (cHead < 0xE0) { // 110xxxxx 10xxxxxx
        cHead &= 0x1F;
        cHead <<= 6;
    }
    else {
        if (cHead < 0xF0) {//1110xxxx 10xxxxxx 10xxxxxx
            cHead &= 0x0F;
            cHead <<= 12;
        }
        else {//11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
            cHead &= 0x07;
            cHead <<= 18;
            cHead |= (*p++ & 0x3F) << 12;
        }
        cHead |= (*p++ & 0x3F) << 6;
    }
    cHead |= *p++ & 0x3F;
    *ppTail = p;
    return cHead;
}

//caller must input valid utf-8 string
inline
static C32 UTF8_Read(const C **ppString) {
    const C *p = *ppString;
    C32 c32 = *p++;
    if (c32 > 0x7F) { // 0xxxxxxx
        if (c32 < 0xE0) { // 110xxxxx 10xxxxxx
            c32 &= 0x1F;
            c32 <<= 6;
        }
        else {
            if (c32 < 0xF0) {//1110xxxx 10xxxxxx 10xxxxxx
                c32 &= 0x0F;
                c32 <<= 12;
            }
            else {//11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
                c32 &= 0x07;
                c32 <<= 18;
                c32 |= (*p++ & 0x3F) << 12;
            }
            c32 |= (*p++ & 0x3F) << 6;
        }
        c32 |= *p++ & 0x3F;
    }
    *ppString = p;
    return c32;
}

//pBuffer size should >= 4 bytes
//caller must input valid utf c32
inline
static C *UTF8_Write(C *pBuffer, C32 value) {
    C *p = pBuffer;
    if (value < 0x80) { // 0xxxxxxx
        *p++ = value;
        return p;
    }

    if (value < 0x0800) // 110xxxxx 10xxxxxx
        *p++ = 0xC0 | (value >> 6);
    else {
        if (value < 0x10000) {//1110xxxx 10xxxxxx 10xxxxxx
            //            assert(value < 0xD800 || value > 0xDFFF);
            *p++ = 0xE0 | (value >> 12);
        }
        else {//11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
            //            assert(value < 0x110000);
            *p++ = 0xF0 | (value >> 18);
            *p++ = 0x80 | ((value >> 12) & 0x3F);
        }
        *p++ = 0x80 | ((value >> 6) & 0x3F);
    }
    *p++ = 0x80 | (value & 0x3F);
    return p;
}

inline
static B UTF8_Valid(const C *p, const C *pEnd) {
    while (p != pEnd) {
        C32 c32 = *p++;
        // 0xxxxxxx
        if (c32 > 0x7F) {
            if (c32 < 0xE0) { // 110xxxxx 10xxxxxx
                if (c32 < 0xC2 || p == pEnd)
                    return 0;
                else {
                    C t = *p++;
                    if (t < 0x80 || t > 0xBF)
                        return 0;
                }
            }
            else if (c32 < 0xF0) { // 1110xxxx 10xxxxxx 10xxxxxx
                if (p + 1 >= pEnd)
                    return 0;
                else {
                    C t0 = *p++;
                    if (t0 < 0x80 || t0 > 0xBF || (c32 == 0xE0 && t0 < 0xA0)
                            || (c32 == 0xED && t0 > 0x9F && t0 < 0xC0))
                        return 0;
                    else {
                        C t1 = *p++;
                        if (t1 < 0x80 || t1 > 0xBF)
                            return 0;
                    }
                }
            }
            else if (c32 < 0xF5) { // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
                if (p + 2 >= pEnd)
                    return 0;
                else {
                    C t0 = *p++;
                    if (t0 < 0x80 || t0 > 0xBF || (t0 > 0x8F && c32 == 0xF4) || (t0 < 0x90 && c32 == 0xF0))
                        return 0;
                    else {
                        C t1 = *p++;
                        if (t1 < 0x80 || t1 > 0xBF)
                            return 0;
                        else {
                            C t2 = *p++;
                            if (t2 < 0x80 || t2 > 0xBF)
                                return 0;
                        }
                    }
                }
            }
            else
                return 0;
        }
    }
    return 1;
}

inline
static C32 UTF8_Parse(const C **ppBegin, const C *pEnd) {
    const C *p = *ppBegin;
    C32 c32 = *p++;
    // 0xxxxxxx
    if (c32 > 0x7F) {
        if (c32 < 0xE0) { // 110xxxxx 10xxxxxx
            if (c32 < 0xC2 || p == pEnd)
                c32 |= 0x80000000;
            else {
                C t = *p++;
                if (t < 0x80 || t > 0xBF) {
                    --p;
                    c32 |= 0x80000000;
                }
                else {
                    t &= 0x3F;
                    c32 &= 0x1F;
                    c32 <<= 6;
                    c32 |= t;
                }
            }
        }
        else if (c32 < 0xF0) { // 1110xxxx 10xxxxxx 10xxxxxx
            if (p + 1 >= pEnd)
                c32 |= 0x80000000;
            else {
                C t0 = *p++;
                if (t0 < 0x80 || t0 > 0xBF || (c32 == 0xE0 && t0 < 0xA0)
                         || (c32 == 0xED && t0 > 0x9F && t0 < 0xC0)) {
                    --p;
                    c32 |= 0x80000000;
                }
                else {
                    C t1 = *p++;
                    if (t1 < 0x80 || t1 > 0xBF) {
                        p -= 2;
                        c32 |= 0x80000000;
                    }
                    else {
                        c32 &= 0x0F;
                        c32 <<= 6;
                        t0 &= 0x3F;
                        c32 |= t0;
                        c32 <<= 6;
                        t1 &= 0x3F;
                        c32 |= t1;
                    }
                }
            }
        }
        else if (c32 < 0xF5) { // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
            if (p + 2 >= pEnd)
                c32 |= 0x80000000;
            else {
                C t0 = *p++;
                if (t0 < 0x80 || t0 > 0xBF || (t0 > 0x8F && c32 == 0xF4) || (t0 < 0x90 && c32 == 0xF0)) {
                    --p;
                    c32 |= 0x80000000;
                }
                else {
                    C t1 = *p++;
                    if (t1 < 0x80 || t1 > 0xBF) {
                        p -= 2;
                        c32 |= 0x80000000;
                    }
                    else {
                        C t2 = *p++;
                        if (t2 < 0x80 || t2 > 0xBF) {
                            p -= 3;
                            c32 |= 0x80000000;
                        }
                        else {
                            c32 &= 0x0F;
                            c32 <<= 6;
                            t0 &= 0x3F;
                            c32 |= t0;
                            c32 <<= 6;
                            t1 &= 0x3F;
                            c32 |= t1;
                            c32 <<= 6;
                            t2 &= 0x3F;
                            c32 |= t2;
                        }
                    }
                }
            }
        }
        else
            c32 |= 0x80000000;
    }
    *ppBegin = p;
    return c32;
}

inline
static U64 UTF8_ParseCount(const C **ppBegin, const C *pEnd) {
    U64 uCount = 0;
    const C *p = *ppBegin;
    while (p != pEnd) {
        C32 c32 = *p++;
        // 0xxxxxxx
        if (c32 > 0x7F) {
            if (c32 < 0xE0) { // 110xxxxx 10xxxxxx
                if (c32 < 0xC2 || p == pEnd) {
                    --p;
                    uCount |= 0x8000000000000000;
                    break;
                }
                else {
                    C t = *p++;
                    if (t < 0x80 || t > 0xBF) {
                        p -= 2;
                        uCount |= 0x8000000000000000;
                        break;
                    }
                }
            }
            else if (c32 < 0xF0) { // 1110xxxx 10xxxxxx 10xxxxxx
                if (p + 1 >= pEnd) {
                    uCount |= 0x8000000000000000;
                    break;
                }
                else {
                    C t0 = *p++;
                    if (t0 < 0x80 || t0 > 0xBF || (c32 == 0xE0 && t0 < 0xA0)
                             || (c32 == 0xED && t0 > 0x9F && t0 < 0xC0)) {
                        p -= 2;
                        uCount |= 0x8000000000000000;
                        break;
                    }
                    else {
                        C t1 = *p++;
                        if (t1 < 0x80 || t1 > 0xBF) {
                            p -= 3;
                            uCount |= 0x8000000000000000;
                            break;
                        }
                    }
                }
            }
            else if (c32 < 0xF5) { // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
                if (p + 2 >= pEnd) {
                    --p;
                    uCount |= 0x8000000000000000;
                    break;
                }
                else {
                    C t0 = *p++;
                    if (t0 < 0x80 || t0 > 0xBF || (t0 > 0x8F && c32 == 0xF4) || (t0 < 0x90 && c32 == 0xF0)) {
                        p -= 2;
                        uCount |= 0x8000000000000000;
                        break;
                    }
                    else {
                        C t1 = *p++;
                        if (t1 < 0x80 || t1 > 0xBF) {
                            p -= 3;
                            uCount |= 0x8000000000000000;
                            break;
                        }
                        else {
                            C t2 = *p++;
                            if (t2 < 0x80 || t2 > 0xBF) {
                                p -= 4;
                                uCount |= 0x8000000000000000;
                                break;
                            }
                        }
                    }
                }
            }
            else {
                --p;
                uCount |= 0x8000000000000000;
                break;
            }
        }
        ++uCount;
    }
    *ppBegin = p;
    return uCount;
}

#endif //UTF8_H

