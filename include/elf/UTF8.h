#ifndef UTF8_H
#define UTF8_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

// Max value: 0x10FFFF
// Max utf8: F4 8F BF BF

inline
static const C *UTF8_Skip(const C *p) {
    U8 u8 = *p++;
    if (u8 < 0x80)
        return p;

    ++p;
    if (u8 < 0xE0)//110xxxxx 10xxxxxx
        return p;

    ++p;
    if (u8 < 0xF0)//1110xxxx 10xxxxxx 10xxxxxx
        return p;

    return ++p;
}

inline
static const C *UTF8_SkipTail(C cHead, const C *pTail) {
    ++pTail;
    if (cHead < 0xE0)//110xxxxx 10xxxxxx
        return pTail;

    ++pTail;
    if (cHead < 0xF0)//1110xxxx 10xxxxxx 10xxxxxx
        return pTail;

    return ++pTail;
}

inline
static B UTF8_Valid(const C *p, const C *pEnd) {
    while(p != pEnd) {
        C c = *p++;
        if (c < 0x80)//0xxxxxxx
            continue;

        if (c < 0xC0)//110xxxxx 10xxxxxx
            return 0;

        if (c > 0xDF) {//1110xxxx 10xxxxxx 10xxxxxx
            if (c > 0xEF) {//11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
                if (c > 0xF4)
                    return 0;

                if (p == pEnd || (*p++ & 0xC0) != 0x80)
                    return 0;
            }
            if (p == pEnd || (*p++ & 0xC0) != 0x80)
                return 0;
        }
        if (p == pEnd || (*p++ & 0xC0) != 0x80)
            return 0;
    }
    return 1;
}

inline
static U UTF8_Count(const C *p, const C *pEnd) {
    U uCount = 0;
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

inline
static const C *UTF8_ParseCount(const C *pBegin, const C *pEnd, U *puCount) {
    U uCount = 0;
    const C *p = pBegin;
    while (p != pEnd) {
        pBegin = p;
        C c = *p++;
        if (c > 0x7F) { //0xxxxxxx
            if (c < 0xC0) {//110xxxxx 10xxxxxx
                *puCount = uCount;
                return pBegin;
            }

            if (c > 0xDF) {//1110xxxx 10xxxxxx 10xxxxxx
                if (c > 0xEF) {//11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
                    if (c > 0xF4) {
                        *puCount = uCount;
                        return pBegin;
                    }

                    if (p == pEnd || (*p++ & 0xC0) != 0x80) {
                        *puCount = uCount;
                        return pBegin;
                    }
                }
                if (p == pEnd || (*p++ & 0xC0) != 0x80) {
                    *puCount = uCount;
                    return pBegin;
                }
            }
            if (p == pEnd || (*p++ & 0xC0) != 0x80) {
                *puCount = uCount;
                return pBegin;
            }
        }
        ++uCount;
    }
    *puCount = uCount;
    return p;
}

inline
static const C *UTF8_Parse(const C *pBegin, const C *pEnd, C32 *pValue) {
    const C *p = pBegin;
    C32 value = *p++;
    //0xxxxxxx
    if (value > 0x7F) {
        if (value < 0xC0)
            return pBegin;
        else {
            if (value < 0xE0)//110xxxxx 10xxxxxx
                value = (value & 0x1F) << 6;
            else {
                if (value < 0xF0)//1110xxxx 10xxxxxx 10xxxxxx
                    value = (value & 0x0F) << 12;
                else if (value < 0xF5) {//11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
                    value = (value & 0x07) << 18;
                    if (p == pEnd || ((*p & 0xC0) != 0x80))
                        return pBegin;

                    value |= (*p++ & 0x3F) << 12;
                }
                else
                    return pBegin;

                if (p == pEnd || ((*p & 0xC0) != 0x80))
                    return pBegin;

                value |= (*p++ & 0x3F) << 6;
            }

            if (p == pEnd || ((*p & 0xC0) != 0x80))
                return pBegin;

            value |= *p++ & 0x3F;
        }
    }

    *pValue = value;
    return p;
}

//caller must input valid utf-8 string
inline
static const C *UTF8_ReadMultibytes(const C *pString, C32 *pValue) {
    const C *p = pString;
    C32 value = *p++;
    if (value < 0xE0)//110xxxxx 10xxxxxx
        value = (value & 0x001F) << 6;
    else {
        if (value < 0xF0)//1110xxxx 10xxxxxx 10xxxxxx
            value = (value & 0x0F) << 12;
        else {//11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
            value = (value & 0x07) << 18;
            value |= (*p++ & 0x3F) << 12;
        }

        value |= (*p++ & 0x3F) << 6;
    }

    value |= *p++ & 0x3F;
    *pValue = value;
    return p;
}

//caller must input valid utf-8 string
inline
static const C *UTF8_Read(const C *pString, C32 *pValue) {
    const C *p = pString;
    C32 value = *p++;
    if (value > 0x7F) {//0xxxxxxx
        if (value < 0xE0)//110xxxxx 10xxxxxx
            value = (value & 0x1F) << 6;
        else {
            if (value < 0xF0)//1110xxxx 10xxxxxx 10xxxxxx
                value = (value & 0x0F) << 12;
            else {//11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
                value = (value & 0x07) << 18;
                value |= (*p++ & 0x3F) << 12;
            }

            value |= (*p++ & 0x3F) << 6;
        }

        value |= *p++ & 0x3F;
    }

    *pValue = value;
    return p;
}

//pBuffer size should >= 4 bytes
//caller must input valid utf value
inline
static C *UTF8_Write(C *pBuffer, C32 value) {
    C *p = pBuffer;
    if (value < 0x80) {//0xxxxxxx
        *p = value;
        return ++p;
    }

    if (value < 0x0800)//110xxxxx 10xxxxxx
        *p = 0xC0 | (value >> 6);
    else{
        if (value < 0x10000) {//1110xxxx 10xxxxxx 10xxxxxx
            //            assert(value < 0xD800 || value >= 0xE000);
            *p = 0xE0 | (value >> 12);
        }
        else{//11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
            //            assert(value < 0x110000);
            *p = 0xF0 | (value >> 18);
            ++p;
            *p = 0x80 | ((value >> 12) & 0x3F);
        }

        ++p;
        *p = 0x80 | ((value >> 6) & 0x3F);
    }

    ++p;
    *p = 0x80 | (value & 0x3F);
    return ++p;
}

#endif //UTF8_H

