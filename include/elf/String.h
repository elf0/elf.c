#ifndef STRING_H
#define STRING_H

//License: Public Domain
//Author: elf
//EMail:

#include "Char.h"

typedef void (*String_EventHandler)(void *pContext, const C *pBegin, const C *pEnd);

static const C *String_Find(const C *pBegin, const C *pEnd, const C *pKey, const C *pKeyEnd, U32 uKey);
inline static B String_StartsWith(const C *pBegin, U32 nSize, const C *pPrefix, U32 nPrefixSize);
inline static B String_StartsWith2(const C *pBegin, U32 nSize, const C *pPrefix);
inline static B String_StartsWith4(const C *pBegin, U32 nSize, const C *pPrefix);
inline static B String_StartsWith8(const C *pBegin, U32 nSize, const C *pPrefix);
inline static B String_EndsWith(const C *pBegin, U32 nSize, const C *pSuffix, U32 nSuffixSize);
inline static B String_EndsWith2(const C *pEnd, U32 nSize, const C *pSuffix);
inline static B String_EndsWith4(const C *pEnd, U32 nSize, const C *pSuffix);
inline static B String_EndsWith8(const C *pEnd, U32 nSize, const C *pSuffix);
inline static const C *String_Skip(const C *p, const C value);
inline static const C *String_SkipBackward(const C *p, C value);
inline static const C *String_SkipBackwardUntil(const C *p, C value);
inline static const C *String_SkipDigit(const C *p);
inline static const C *String_SkipUpper(const C *p);
inline static const C *String_SkipLower(const C *p);
inline static const C *String_SkipAlpha(const C *p);
inline static const C *String_TrimEnd(const C *pBegin, C *pEnd, C value);
inline static void String_Split(const C *pBegin, const C *pEnd, C cSplitter, String_EventHandler onSubString, void *pContext);

inline static B String_Equal2(const C *pLeft, const C *pRight);
inline static B String_Equal4(const C *pLeft, const C *pRight);
inline static B String_Equal6(const C *pLeft, const C *pRight4, const C *pRight2);
inline static B String_Equal8(const C *pLeft, const C *pRight);
inline static B String_Equal(const C *pLeft, U32 uLeft, const C *pRight, const C *pRightEnd, U32 uRight);
inline static B String_EqualCI(const C *pLeft, U32 uLeft, const C *pRight, const C *pRightEnd, U32 uRight);

inline static C *String_Hex8(U8 uValue, C *pBuffer);
inline static C *String_Hex16(U16 uValue, C *pBuffer);
inline static C *String_Hex32(U32 uValue, C *pBuffer);
inline static C *String_Hex64(U64 uValue, C *pBuffer);

////////////////////////////////////////////////////////////////
inline static const C *String_Find1(const C *pBegin, const C *pEnd, C v) {
    do {
        if(*pBegin == v)
            return pBegin;
    } while (++pBegin != pEnd);
    return 0;
}

inline static const C *String_Find1CI(const C *pBegin, const C *pEnd, C lv) {
    do {
        if((*pBegin | 0x20) == lv)
            return pBegin;
    } while (++pBegin != pEnd);
    return 0;
}

inline static const C *String_FindLast1(const C *pRBegin, const C *pREnd, C v) {
    do {
        if (*pRBegin == v)
            return pRBegin;
    } while (--pRBegin != pREnd);
    return 0;
}

inline
    static const C *String_FindLast1CI(const C *pBegin, const C *pEnd, C lv) {
    do {
        if((*pRBegin | 0x20) == lv)
            return pRBegin;
    } while (--pRBegin != pREnd);
    return 0;
}

inline static const C *String_Find2(const C *pBegin, const C *pEnd, U16 v) {
    do {
        if(*(U16*)pBegin == v)
            return pBegin;
    } while (++pBegin != pEnd);
    return 0;
}

inline static const C *String_Find2CI(const C *pBegin, const C *pEnd, U16 lv) {
    do {
        if((*(U16*)pBegin | 0x2020) == lv)
            return pBegin;
    } while (++pBegin != pEnd);
    return 0;
}

inline static const C *String_FindLast2(const C *pRBegin, const C *pREnd, U16 v) {
    do {
        if (*(U16*)pRBegin == v)
            return pRBegin;
    } while (--pRBegin != pREnd);
    return 0;
}

inline static const C *String_FindLast2CI(const C *pBegin, const C *pEnd, U16 lv) {
    do {
        if((*(U16*)pRBegin | 0x2020) == lv)
            return pRBegin;
    } while (--pRBegin != pREnd);
    return 0;
}

inline static const C *String_Find3(const C *pBegin, const C *pEnd, U16 v0, C v1) {
    do {
        if(*(U16*)pBegin == v0 && *(pBegin + 2) == v1)
            return pBegin;
    } while (++pBegin != pEnd);
    return 0;
}

inline static const C *String_Find3CI(const C *pBegin, const C *pEnd, U16 lv0, C lv1) {
    do {
        if((*(U16*)pBegin | 0x2020) == lv0 && (*(pBegin + 2) | 0x20)== lv1)
            return pBegin;
    } while (++pBegin != pEnd);
    return 0;
}

inline static const C *String_Find4(const C *pBegin, const C *pEnd, U32 v) {
    do {
        if(*(U32*)pBegin == v)
            return pBegin;
    } while (++pBegin != pEnd);
    return 0;
}

inline static const C *String_Find4CI(const C *pBegin, const C *pEnd, U32 lv) {
    do {
        if((*(U32*)pBegin | 0x20202020) == lv)
            return pBegin;
    } while (++pBegin != pEnd);
    return 0;
}

inline static const C *String_FindLast4(const C *pRBegin, const C *pREnd, U32 v) {
    do {
        if (*(U32*)pRBegin == v)
            return pRBegin;
    } while (--pRBegin != pREnd);
    return 0;
}

inline static const C *String_FindLast4CI(const C *pBegin, const C *pEnd, U32 lv) {
    do {
        if((*(U32*)pRBegin | 0x20202020) == lv)
            return pRBegin;
    } while (--pRBegin != pREnd);
    return 0;
}

inline static const C *String_Find5(const C *pBegin, const C *pEnd, U32 v0, C v1) {
    do {
        if(*(U32*)pBegin == v0 && *(pBegin + 4) == v1)
            return pBegin;
    } while (++pBegin != pEnd);
    return 0;
}

inline static const C *String_Find5CI(const C *pBegin, const C *pEnd, U32 lv0, C lv1) {
    do {
        if((*(U32*)pBegin | 0x20202020) == lv0 && (*(pBegin + 4) | 0x20)== lv1)
            return pBegin;
    } while (++pBegin != pEnd);
    return 0;
}

inline static const C *String_Find6(const C *pBegin, const C *pEnd, U32 v0, U16 v1) {
    do {
        if(*(U32*)pBegin == v0 && *(U16*)(pBegin + 4) == v1)
            return pBegin;
    } while (++pBegin != pEnd);
    return 0;
}

inline static const C *String_Find6CI(const C *pBegin, const C *pEnd, U32 lv0, U16 lv1) {
    do {
        if((*(U32*)pBegin | 0x20202020) == lv0 && (*(U16*)(pBegin + 4) | 0x2020)== lv1)
            return pBegin;
    } while (++pBegin != pEnd);
    return 0;
}

inline static const C *String_FindLast6(const C *pRBegin, const C *pREnd, U32 v0, U16 v1) {
    do {
        if (*(U32*)pRBegin == v0 && *(U16*)(pRBegin + 4) == v1)
            return pRBegin;
    } while (--pRBegin != pREnd);
    return 0;
}

inline static const C *String_Find7(const C *pBegin, const C *pEnd, U32 v0, U16 v1, C v2) {
    do {
        if(*(U32*)pBegin == v0 && *(U16*)(pBegin + 4) == v1 && *(pBegin + 6) == v2)
            return pBegin;
    } while (++pBegin != pEnd);
    return 0;
}

inline static const C *String_Find7CI(const C *pBegin, const C *pEnd, U32 lv0, U16 lv1, C lv2) {
    do {
        if((*(U32*)pBegin | 0x20202020) == lv0 && (*(U16*)(pBegin + 4) | 0x2020) == lv1 && (*(pBegin + 6) | 0x20) == lv2)
            return pBegin;
    } while (++pBegin != pEnd);
    return 0;
}

inline static const C *String_FindLast7(const C *pRBegin, const C *pREnd, U32 v0, U16 v1, C v2) {
    do {
        if (*(U32*)pRBegin == v0 && *(U16*)(pRBegin + 4) == v1 && *(pRBegin + 6) == v2)
            return pRBegin;
    } while (--pRBegin != pREnd);
    return 0;
}

inline static const C *String_Find8(const C *pBegin, const C *pEnd, U64 v) {
    do {
        if(*(U64*)pBegin == v)
            return pBegin;
    } while (++pBegin != pEnd);
    return 0;
}

inline static const C *String_Find8CI(const C *pBegin, const C *pEnd, U64 lv) {
    do {
        if((*(U64*)pBegin | 0x2020202020202020) == lv)
            return pBegin;
    } while (++pBegin != pEnd);
    return 0;
}

inline static const C *String_FindLast8(const C *pRBegin, const C *pREnd, U64 v) {
    do {
        if (*(U64*)pRBegin == v)
            return pRBegin;
    } while (--pRBegin != pREnd);
    return 0;
}

inline static const C *String_FindLast8CI(const C *pBegin, const C *pEnd, U64 lv) {
    do {
        if((*(U64*)pRBegin | 0x2020202020202020) == lv)
            return pRBegin;
    } while (--pRBegin != pREnd);
    return 0;
}

inline static const C *String_Find9(const C *pBegin, const C *pEnd, U64 v0, C v1) {
    do {
        if(*(U64*)pBegin == v0 && *(pBegin + 8) == v1)
            return pBegin;
    } while (++pBegin != pEnd);
    return 0;
}

inline static const C *String_Find9CI(const C *pBegin, const C *pEnd, U64 lv0, C lv1) {
    do {
        if((*(U64*)pBegin | 0x2020202020202020) == lv0 && (*(pBegin + 8) | 0x20) == lv1)
            return pBegin;
    } while (++pBegin != pEnd);
    return 0;
}

inline static const C *String_Find10(const C *pBegin, const C *pEnd, U64 v0, U16 v1) {
    do {
        if(*(U64*)pBegin == v0 && *(U16*)(pBegin + 8) == v1)
            return pBegin;
    } while (++pBegin != pEnd);
    return 0;
}

inline static const C *String_Find10CI(const C *pBegin, const C *pEnd, U64 lv0, U16 lv1) {
    do {
        if((*(U64*)pBegin | 0x2020202020202020) == lv0 && (*(U16*)(pBegin + 8) | 0x2020) == lv1)
            return pBegin;
    } while (++pBegin != pEnd);
    return 0;
}

inline static const C *String_Find12(const C *pBegin, const C *pEnd, U64 v0, U32 v1) {
    do {
        if(*(U64*)pBegin == v0 && *(U32*)(pBegin + 8) == lv1)
            return pBegin;
    } while (++pBegin != pEnd);
    return 0;
}

inline static const C *String_Find12CI(const C *pBegin, const C *pEnd, U64 lv0, U32 lv1) {
    do {
        if((*(U64*)pBegin | 0x2020202020202020) == lv0 && (*(U32*)(pBegin + 8) | 0x20202020) == lv1)
            return pBegin;
    } while (++pBegin != pEnd);
    return 0;
}

inline static const C *String_Find16(const C *pBegin, const C *pEnd, U64 v0, U64 v1) {
    do {
        if(*(U64*)pBegin == v0 && *(U64*)(pBegin + 8) == lv1)
            return pBegin;
    } while (++pBegin != pEnd);
    return 0;
}

inline static const C *String_Find16CI(const C *pBegin, const C *pEnd, U64 lv0, U64 lv1) {
    do {
        if((*(U64*)pBegin | 0x2020202020202020) == lv0 && (*(U64*)(pBegin + 8) | 0x2020202020202020) == lv1)
            return pBegin;
    } while (++pBegin != pEnd);
    return 0;
}


inline static const C *String_Find28(const C *pBegin, const C *pEnd, U64 v0, U64 v1, U64 v2, U32 v3) {
    do {
        if(*(U64*)pBegin == v0 && *(U64*)(pBegin + 8) == v1 && *(U64*)(pBegin + 16) == v2 && *(U32*)(pBegin + 24) == v3)
            return pBegin;
    } while (++pBegin != pEnd);
    return 0;
}

inline static const C *String_Find32(const C *pBegin, const C *pEnd, U64 v0, U64 v1, U64 v2, U64 v3) {
    do {
        if(*(U64*)pBegin == v0 && *(U64*)(pBegin + 8) == v1 && *(U64*)(pBegin + 16) == v2 && *(U64*)(pBegin + 24) == v3)
            return pBegin;
    } while (++pBegin != pEnd);
    return 0;
}


//(pEnd - pBegin) >= uKey && uKey >  0
inline static const C *String_Find(const C *p, const C *pEnd, const C *pKeyword, const C *pKeywordEnd, U32 uKeyword) {
    pEnd -= uKeyword - 1;
    C k0 = *pKeyword++;
NEXT:
    while (p < pEnd) {
        if (*p++ == k0) {
            const C *pS = p;
            const C *pK = pKeyword;
            while (pK != pKeywordEnd) {
                if (*pS++ != *pK++)
                    goto NEXT;
            }
            return p - 1;
        }
    }
    return NULL;
}

inline static const C *String_FindCI(const C *p, const C *pEnd, const C *pKeyword, const C *pKeywordEnd, U32 uKeyword) {
    pEnd -= uKeyword - 1;
    C k0 = *pKeyword++ | 0x20;
NEXT:
    while (p < pEnd) {
        if ((*p++ | 0x20) == k0) {
            const C *pS = p;
            const C *pK = pKeyword;
            while (pK != pKeywordEnd) {
                if ((*pS++ | 0x20) != (*pK++ | 0x20))
                    goto NEXT;
            }
            return p - 1;
        }
    }
    return NULL;
}

inline static const C *String_FindCIL(const C *p, const C *pEnd, const C *pKeyword, const C *pKeywordEnd, U32 uKeyword) {
    pEnd -= uKeyword - 1;
    C k0 = *pKeyword++;
NEXT:
    while (p < pEnd) {
        if ((*p++ | 0x20) == k0) {
            const C *pS = p;
            const C *pK = pKeyword;
            while (pK != pKeywordEnd) {
                if ((*pS++ | 0x20) != *pK++)
                    goto NEXT;
            }
            return p - 1;
        }
    }
    return NULL;
}

inline static B String_StartsWith(const C *pBegin, U32 nSize, const C *pPrefix, U32 nPrefixSize) {
    if(nPrefixSize > nSize)
        return false;

    const C *pPrefixEnd = pPrefix + nPrefixSize;
    const C *pStr = pBegin;
    const C *pPre = pPrefix;
    while (pPre != pPrefixEnd) {
        if(*pStr != *pPre)
            return false;
        ++pStr;
        ++pPre;
    }
    return true;
}

inline static B String_StartsWith2(const C *pBegin, U32 nSize, const C *pPrefix) {
    return nSize > 1 && String_Equal2(pBegin, pPrefix);
}

inline static B String_StartsWith4(const C *pBegin, U32 nSize, const C *pPrefix) {
    return nSize > 3 && String_Equal4(pBegin, pPrefix);
}

inline static B String_StartsWith8(const C *pBegin, U32 nSize, const C *pPrefix) {
    return nSize > 7 && String_Equal8(pBegin, pPrefix);
}

inline static B String_EndsWith(const C *pBegin, U32 nSize, const C *pSuffix, U32 nSuffixSize) {
    if(nSuffixSize > nSize)
        return false;

    const C *pSuffixEnd = pSuffix + nSuffixSize;
    const C *pStr = pBegin + (nSize - nSuffixSize);
    const C *pSuf = pSuffix;
    while (pSuf != pSuffixEnd) {
        if(*pStr != *pSuf)
            return false;
        ++pStr;
        ++pSuf;
    }
    return true;
}

inline static B String_EndsWith2(const C *pEnd, U32 nSize, const C *pSuffix) {
    return nSize > 1 && String_Equal2(pEnd - 2, pSuffix);
}

inline static B String_EndsWith4(const C *pEnd, U32 nSize, const C *pSuffix) {
    return nSize > 3 && String_Equal4(pEnd - 4, pSuffix);
}

inline static B String_EndsWith8(const C *pEnd, U32 nSize, const C *pSuffix) {
    return nSize > 7 && String_Equal8(pEnd - 8, pSuffix);
}


inline static const C *String_Skip(const C *p, const C value) {
    while (*p == value)
        ++p;
    return p;
}

inline static const C *String_SkipBackward(const C *p, const C value) {
    while (*p == value)
        --p;
    return p;
}

inline static const C *String_SkipUntil(const C *p, C value) {
    while (*p != value)
        ++p;

    return p;
}

inline static const C *String_SkipUntilEnd(const C *p, C value) {
    while (*p++ != value);
    return p;
}

inline static const C *String_SkipBackwardUntil(const C *p, C value) {
    while (*p != value)
        --p;
    return p;
}

inline static const C *String_SkipDigit(const C *p) {
    while (C_IsDigit(*p))
        ++p;
    return p;
}

inline static const C *String_SkipUpper(const C *p) {
    while (C_IsUpper(*p))
        ++p;
    return p;
}

inline static const C *String_SkipLower(const C *p) {
    while (C_IsLower(*p))
        ++p;
    return p;
}

inline static const C *String_SkipAlpha(const C *p) {
    p = String_SkipLower(p);
    while (C_IsAlpha(*p))
        ++p;
    return p;
}

inline static C *String_SkipAndLowerAlpha(C *pBegin, C *pEnd) {
    C c;
    C *p = pBegin;
    while (p != pEnd) {
        c = *p;
        switch(c) {
        default:
            return p;
        case CASE_CHAR_UPPER:
            *p = c + ('a' - 'A');
            break;
        case CASE_CHAR_LOWER:
            break;
        }
        ++p;
    }
    return p;
}

inline static const C *String_TrimEnd(const C *pBegin, C *pEnd, C value) {
    const C *pREnd = pBegin - 1;
    const C *p = pEnd - 1;
    while (p != pREnd) {
        if(*p != value)
            break;
        --p;
    }
    return ++p;
}

//*pEnd MUST equal cSplitter
inline static void String_Split(const C *pBegin, const C *pEnd, C cSplitter, String_EventHandler onSubString, void *pContext) {
    const C *pSubString;
    const C *p = pBegin;
    while (p < pEnd) {
        pSubString = p;
        p = String_SkipUntil(p, cSplitter);
        onSubString(pContext, pSubString, p);
        ++p;
    }
}

inline static B String_Equal2(const C *pLeft, const C *pRight) {
    return *(const U16*)pLeft == *(const U16*)pRight;
}

inline static B String_Equal4(const C *pLeft, const C *pRight) {
    return *(const U32*)pLeft == *(const U32*)pRight;
}

inline static B String_Equal6(const C *pLeft, const C *pRight4, const C *pRight2) {
    return String_Equal4(pLeft, pRight4) && String_Equal2(pLeft + 4, pRight2);
}

inline static B String_Equal8(const C *pLeft, const C *pRight) {
    return *(const U64*)pLeft == *(const U64*)pRight;
}

inline static B String_Equal(U32 uLeft, const C *pLeft, U32 uRight, const C *pRight, const C *pRightEnd) {
    if (uLeft != uRight)
        return 0;

    const C *pREnd = pRight + (uRight & 0xFFFFFFF8);
    const C *pR = pRight;
    while (pR != pREnd) {
        if (*(U64*)pLeft != *(U64*)pR)
            return 0;

        pLeft += 8;
        pR += 8;
    }
    if (pR + 4 <= pRightEnd) {
        if (*(U32*)pLeft != *(U32*)pR)
            return 0;

        pLeft += 4;
        pR += 4;
    }
    if (pR + 2 <= pRightEnd) {
        if (*(U16*)pLeft != *(U16*)pR)
            return 0;

        pLeft += 2;
        pR += 2;
    }
    if (pR != pRightEnd) {
        if (*pLeft++ != *pR++)
            return 0;
    }
    return 1;
}

inline static B String_EqualCI(U32 uLeft, const C *pLeft, U32 uRight, const C *pRight, const C *pRightEnd) {
    if (uLeft != uRight)
        return 0;

    const C *pREnd = pRight + (uRight & 0xFFFFFFF8);
    const C *pR = pRight;
    while (pR != pREnd) {
        if ((*(U64*)pLeft | 0x2020202020202020) != (*(U64*)pR | 0x2020202020202020))
            return 0;

        pLeft += 8;
        pR += 8;
    }
    if (pR + 4 <= pRightEnd) {
        if ((*(U32*)pLeft | 0x20202020) != (*(U32*)pR | 0x20202020))
            return 0;

        pLeft += 4;
        pR += 4;
    }
    if (pR + 2 <= pRightEnd) {
        if ((*(U16*)pLeft | 0x2020) != (*(U16*)pR | 0x2020))
            return 0;

        pLeft += 2;
        pR += 2;
    }
    if (pR != pRightEnd) {
        if ((*pLeft++ | 0x20) != (*pR++ | 0x20))
            return 0;
    }
    return 1;
}

#define HEXCHARS "0123456789ABCDEF"

inline static C *String_Hex8(U8 uValue, C *pBuffer) {
    *pBuffer++ = HEXCHARS[uValue >> 4];
    *pBuffer++ = HEXCHARS[uValue & 0x0F];
    return pBuffer;
}

inline static C *String_Hex16(U16 uValue, C *pBuffer) {
    *pBuffer++ = HEXCHARS[uValue >> 12];
    *pBuffer++ = HEXCHARS[(uValue >> 8) & 0x0F];
    *pBuffer++ = HEXCHARS[(uValue >> 4) & 0x0F];
    *pBuffer++ = HEXCHARS[uValue & 0x0F];
    return pBuffer;
}

inline static C *String_Hex32(U32 uValue, C *pBuffer) {
    *pBuffer++ = HEXCHARS[uValue >> 28];
    *pBuffer++ = HEXCHARS[(uValue >> 24) & 0x0F];
    *pBuffer++ = HEXCHARS[(uValue >> 20) & 0x0F];
    *pBuffer++ = HEXCHARS[(uValue >> 16) & 0x0F];
    *pBuffer++ = HEXCHARS[(uValue >> 12) & 0x0F];
    *pBuffer++ = HEXCHARS[(uValue >> 8) & 0x0F];
    *pBuffer++ = HEXCHARS[(uValue >> 4) & 0x0F];
    *pBuffer++ = HEXCHARS[uValue & 0x0F];
    return pBuffer;
}

inline static C *String_Hex64(U64 uValue, C *pBuffer) {
    *pBuffer++ = HEXCHARS[uValue >> 60];
    *pBuffer++ = HEXCHARS[(uValue >> 56) & 0x0F];
    *pBuffer++ = HEXCHARS[(uValue >> 52) & 0x0F];
    *pBuffer++ = HEXCHARS[(uValue >> 48) & 0x0F];
    *pBuffer++ = HEXCHARS[(uValue >> 44) & 0x0F];
    *pBuffer++ = HEXCHARS[(uValue >> 40) & 0x0F];
    *pBuffer++ = HEXCHARS[(uValue >> 36) & 0x0F];
    *pBuffer++ = HEXCHARS[(uValue >> 32) & 0x0F];
    *pBuffer++ = HEXCHARS[(uValue >> 28) & 0x0F];
    *pBuffer++ = HEXCHARS[(uValue >> 24) & 0x0F];
    *pBuffer++ = HEXCHARS[(uValue >> 20) & 0x0F];
    *pBuffer++ = HEXCHARS[(uValue >> 16) & 0x0F];
    *pBuffer++ = HEXCHARS[(uValue >> 12) & 0x0F];
    *pBuffer++ = HEXCHARS[(uValue >> 8) & 0x0F];
    *pBuffer++ = HEXCHARS[(uValue >> 4) & 0x0F];
    *pBuffer++ = HEXCHARS[uValue & 0x0F];
    return pBuffer;
}

#endif // STRING_H

