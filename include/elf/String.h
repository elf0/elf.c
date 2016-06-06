#ifndef STRING_H
#define STRING_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

#include "Char.h"

typedef void (*String_EventHandler)(void *pContext, const Char *pBegin, const Char *pEnd);

static inline const Char *String_Find(const Char *pBegin, const Char *pEnd, Char value);
static inline const Char *String_FindLast(const Char *pBegin, const Char *pEnd, Char value);
static inline const Char *String_FindSub(const Char *pBegin, const Char *pEnd, const Char *pSub, U32 nSubSize);
static inline B String_StartsWith(const Char *pBegin, U32 nSize, const Char *pPrefix, U32 nPrefixSize);
static inline B String_StartsWith2(const Char *pBegin, U32 nSize, const Char *pPrefix);
static inline B String_StartsWith4(const Char *pBegin, U32 nSize, const Char *pPrefix);
static inline B String_StartsWith8(const Char *pBegin, U32 nSize, const Char *pPrefix);
static inline B String_EndsWith(const Char *pBegin, U32 nSize, const Char *pSuffix, U32 nSuffixSize);
static inline B String_EndsWith2(const Char *pEnd, U32 nSize, const Char *pSuffix);
static inline B String_EndsWith4(const Char *pEnd, U32 nSize, const Char *pSuffix);
static inline B String_EndsWith8(const Char *pEnd, U32 nSize, const Char *pSuffix);
static inline const Char *String_Skip(const Char *p, const Char value);
static inline const Char *String_SkipBackward(const Char *p, Char value);
static inline const Char *String_SkipUntil(const Char *p, Char value);
static inline const Char *String_SkipBackwardUntil(const Char *p, Char value);
static inline const Char *String_SkipDigit(const Char *p);
static inline const Char *String_SkipUpper(const Char *p);
static inline const Char *String_SkipLower(const Char *p);
static inline const Char *String_SkipAlpha(const Char *p);
static inline const Char *String_TrimEnd(const Char *pBegin, Char *pEnd, Char value);
static inline void String_Split(const Char *pBegin, const Char *pEnd, Char cSplitter, String_EventHandler onSubString, void *pContext);

static inline B String_Equal2(const Char *pLeft, const Char *pRight);
static inline B String_Equal4(const Char *pLeft, const Char *pRight);
static inline B String_Equal6(const Char *pLeft, const Char *pRight4, const Char *pRight2);
static inline B String_Equal8(const Char *pLeft, const Char *pRight);


////////////////////////////////////////////////////////////////
static inline const Char *String_Find(const Char *pBegin, const Char *pEnd, Char value){
    const Char *p = pBegin;
    while(p != pEnd){
        if(*p == value)
            return p;
        ++p;
    }
    return null;
}

static inline const Char *String_FindLast(const Char *pBegin, const Char *pEnd, Char value){
    const Char *pREnd = pBegin - 1;
    const Char *p = pEnd - 1;
    while(p != pREnd){
        if(*p == value)
            return p;
        --p;
    }
    return null;
}

static inline const Char *String_FindSub(const Char *pBegin, const Char *pEnd, const Char *pSub, U32 nSubSize){
    const Char *pS = pSub + 1;
    U32 nS = nSubSize - 1;
    const Char *p = pBegin;
    while(p != pEnd){
        if(*p++ == *pSub && String_StartsWith(p, pEnd - p, pS, nS))
            return p - 1;
    }
    return null;
}

static inline B String_StartsWith(const Char *pBegin, U32 nSize, const Char *pPrefix, U32 nPrefixSize){
    if(nPrefixSize > nSize)
        return false;

    const Char *pPrefixEnd = pPrefix + nPrefixSize;
    const Char *pStr = pBegin;
    const Char *pPre = pPrefix;
    while(pPre != pPrefixEnd){
        if(*pStr != *pPre)
            return false;
        ++pStr;
        ++pPre;
    }
    return true;
}

static inline B String_StartsWith2(const Char *pBegin, U32 nSize, const Char *pPrefix){
    return nSize > 1 && String_Equal2(pBegin, pPrefix);
}

static inline B String_StartsWith4(const Char *pBegin, U32 nSize, const Char *pPrefix){
    return nSize > 3 && String_Equal4(pBegin, pPrefix);
}

static inline B String_StartsWith8(const Char *pBegin, U32 nSize, const Char *pPrefix){
    return nSize > 7 && String_Equal8(pBegin, pPrefix);
}

static inline B String_EndsWith(const Char *pBegin, U32 nSize, const Char *pSuffix, U32 nSuffixSize){
    if(nSuffixSize > nSize)
        return false;

    const Char *pSuffixEnd = pSuffix + nSuffixSize;
    const Char *pStr = pBegin + (nSize - nSuffixSize);
    const Char *pSuf = pSuffix;
    while(pSuf != pSuffixEnd){
        if(*pStr != *pSuf)
            return false;
        ++pStr;
        ++pSuf;
    }
    return true;
}

static inline B String_EndsWith2(const Char *pEnd, U32 nSize, const Char *pSuffix){
    return nSize > 1 && String_Equal2(pEnd - 2, pSuffix);
}

static inline B String_EndsWith4(const Char *pEnd, U32 nSize, const Char *pSuffix){
    return nSize > 3 && String_Equal4(pEnd - 4, pSuffix);
}

static inline B String_EndsWith8(const Char *pEnd, U32 nSize, const Char *pSuffix){
    return nSize > 7 && String_Equal8(pEnd - 8, pSuffix);
}


static inline const Char *String_Skip(const Char *p, const Char value){
    while(*p == value)
        ++p;
    return p;
}

static inline const Char *String_SkipBackward(const Char *p, const Char value){
    while(*p == value)
        --p;
    return p;
}

static inline const Char *String_SkipUntil(const Char *p, Char value){
    while(*p != value)
        ++p;
    return p;
}

static inline const Char *String_SkipBackwardUntil(const Char *p, Char value){
    while(*p != value)
        --p;
    return p;
}

static inline const Char *String_SkipDigit(const Char *p){
    while(Char_IsDigit(*p))
        ++p;
    return p;
}

static inline const Char *String_SkipUpper(const Char *p){
    while(Char_IsUpper(*p))
        ++p;
    return p;
}

static inline const Char *String_SkipLower(const Char *p){
    while(Char_IsLower(*p))
        ++p;
    return p;
}

static inline const Char *String_SkipAlpha(const Char *p){
    while(Char_IsAlpha(*p))
        ++p;
    return p;
}

static inline Char *String_SkipAndLowerAlpha(Char *pBegin, Char *pEnd){
    Char *p = pBegin;
    while(p != pEnd){
        switch(*p){
        default:
            return p;
        case CASE_CHAR_UPPER:
            *p = *p + ('a' - 'A');
            break;
        case CASE_CHAR_LOWER:
            break;
        }
        ++p;
    }
    return p;
}

static inline const Char *String_TrimEnd(const Char *pBegin, Char *pEnd, Char value){
    const Char *pREnd = pBegin - 1;
    const Char *p = pEnd - 1;
    while(p != pREnd){
        if(*p != value)
            break;
        --p;
    }
    return ++p;
}

//*pEnd MUST equal cSplitter
static inline void String_Split(const Char *pBegin, const Char *pEnd, Char cSplitter, String_EventHandler onSubString, void *pContext){
    const Char *pSubString;
    const Char *p = pBegin;
    while(p < pEnd){
        pSubString = p;
        p = String_SkipUntil(p, cSplitter);
        onSubString(pContext, pSubString, p);
        ++p;
    }
}

static inline B String_Equal2(const Char *pLeft, const Char *pRight){
    return *(const U16*)pLeft == *(const U16*)pRight;
}

static inline B String_Equal4(const Char *pLeft, const Char *pRight){
    return *(const U32*)pLeft == *(const U32*)pRight;
}

static inline B String_Equal6(const Char *pLeft, const Char *pRight4, const Char *pRight2){
    return String_Equal4(pLeft, pRight4) && String_Equal2(pLeft + 4, pRight2);
}

static inline B String_Equal8(const Char *pLeft, const Char *pRight){
    return *(const U64*)pLeft == *(const U64*)pRight;
}

#endif // STRING_H

