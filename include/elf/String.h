#ifndef STRING_H
#define STRING_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

#include "Char.h"

typedef void (*String_EventHandler)(void *pContext, const C *pBegin, const C *pEnd);

inline
static const C *String_FindChar(const C *pBegin, const C *pEnd, C value);
inline
static const C *String_FindLast(const C *pBegin, const C *pEnd, C value);
inline
static const C *String_Find(const C *pBegin, const C *pEnd, const C *pKey, const C *pKeyEnd, U32 uKey);
inline
static B String_StartsWith(const C *pBegin, U32 nSize, const C *pPrefix, U32 nPrefixSize);
inline
static B String_StartsWith2(const C *pBegin, U32 nSize, const C *pPrefix);
inline
static B String_StartsWith4(const C *pBegin, U32 nSize, const C *pPrefix);
inline
static B String_StartsWith8(const C *pBegin, U32 nSize, const C *pPrefix);
inline
static B String_EndsWith(const C *pBegin, U32 nSize, const C *pSuffix, U32 nSuffixSize);
inline
static B String_EndsWith2(const C *pEnd, U32 nSize, const C *pSuffix);
inline
static B String_EndsWith4(const C *pEnd, U32 nSize, const C *pSuffix);
inline
static B String_EndsWith8(const C *pEnd, U32 nSize, const C *pSuffix);
inline
static const C *String_Skip(const C *p, const C value);
inline
static const C *String_SkipBackward(const C *p, C value);
inline
static const C *String_SkipUntil(const C *p, C value);
inline
static const C *String_SkipBackwardUntil(const C *p, C value);
inline
static const C *String_SkipDigit(const C *p);
inline
static const C *String_SkipUpper(const C *p);
inline
static const C *String_SkipLower(const C *p);
inline
static const C *String_SkipAlpha(const C *p);
inline
static const C *String_TrimEnd(const C *pBegin, C *pEnd, C value);
inline
static void String_Split(const C *pBegin, const C *pEnd, C cSplitter, String_EventHandler onSubString, void *pContext);

inline
static B String_Equal2(const C *pLeft, const C *pRight);
inline
static B String_Equal4(const C *pLeft, const C *pRight);
inline
static B String_Equal6(const C *pLeft, const C *pRight4, const C *pRight2);
inline
static B String_Equal8(const C *pLeft, const C *pRight);

inline
static C *String_Hex8(U8 uValue, C *pBuffer);
inline
static C *String_Hex16(U16 uValue, C *pBuffer);
inline
static C *String_Hex32(U32 uValue, C *pBuffer);
inline
static C *String_Hex64(U64 uValue, C *pBuffer);

////////////////////////////////////////////////////////////////
inline
static const C *String_FindChar(const C *pBegin, const C *pEnd, C value){
    const C *p = pBegin;
    while(p != pEnd){
        if(*p == value)
            return p;
        ++p;
    }
    return null;
}

inline
static const C *String_FindLast(const C *pBegin, const C *pEnd, C value){
    const C *pREnd = pBegin - 1;
    const C *p = pEnd - 1;
    while(p != pREnd){
        if(*p == value)
            return p;
        --p;
    }
    return null;
}

//(pEnd - pBegin) >= uKey && uKey >  0
inline
static const C *String_Find(const C *pBegin, const C *pEnd, const C *pKey, const C *pKeyEnd, U32 uKey){
  pEnd -= uKey;
    ++pEnd;
    C cFirst = *pKey++;
    do{
        if(*pBegin == cFirst){
            const C *pS = pBegin + 1;
            const C *pK = pKey;
            while(pK != pKeyEnd){
                if(*pS++ != *pK++)
                    goto STRING_FIND_NEXT_CHAR;
            }
            return pBegin;
        }
STRING_FIND_NEXT_CHAR:;
    }while(++pBegin != pEnd);
    return null;
}

inline
static B String_StartsWith(const C *pBegin, U32 nSize, const C *pPrefix, U32 nPrefixSize){
    if(nPrefixSize > nSize)
        return false;

    const C *pPrefixEnd = pPrefix + nPrefixSize;
    const C *pStr = pBegin;
    const C *pPre = pPrefix;
    while(pPre != pPrefixEnd){
        if(*pStr != *pPre)
            return false;
        ++pStr;
        ++pPre;
    }
    return true;
}

inline
static B String_StartsWith2(const C *pBegin, U32 nSize, const C *pPrefix){
    return nSize > 1 && String_Equal2(pBegin, pPrefix);
}

inline
static B String_StartsWith4(const C *pBegin, U32 nSize, const C *pPrefix){
    return nSize > 3 && String_Equal4(pBegin, pPrefix);
}

inline
static B String_StartsWith8(const C *pBegin, U32 nSize, const C *pPrefix){
    return nSize > 7 && String_Equal8(pBegin, pPrefix);
}

inline
static B String_EndsWith(const C *pBegin, U32 nSize, const C *pSuffix, U32 nSuffixSize){
    if(nSuffixSize > nSize)
        return false;

    const C *pSuffixEnd = pSuffix + nSuffixSize;
    const C *pStr = pBegin + (nSize - nSuffixSize);
    const C *pSuf = pSuffix;
    while(pSuf != pSuffixEnd){
        if(*pStr != *pSuf)
            return false;
        ++pStr;
        ++pSuf;
    }
    return true;
}

inline
static B String_EndsWith2(const C *pEnd, U32 nSize, const C *pSuffix){
    return nSize > 1 && String_Equal2(pEnd - 2, pSuffix);
}

inline
static B String_EndsWith4(const C *pEnd, U32 nSize, const C *pSuffix){
    return nSize > 3 && String_Equal4(pEnd - 4, pSuffix);
}

inline
static B String_EndsWith8(const C *pEnd, U32 nSize, const C *pSuffix){
    return nSize > 7 && String_Equal8(pEnd - 8, pSuffix);
}


inline
static const C *String_Skip(const C *p, const C value){
    while(*p == value)
        ++p;
    return p;
}

inline
static const C *String_SkipBackward(const C *p, const C value){
    while(*p == value)
        --p;
    return p;
}

inline
static const C *String_SkipUntil(const C *p, C value){
    while(*p != value)
        ++p;
    return p;
}

inline
static const C *String_SkipBackwardUntil(const C *p, C value){
    while(*p != value)
        --p;
    return p;
}

inline
static const C *String_SkipDigit(const C *p){
    while(C_IsDigit(*p))
        ++p;
    return p;
}

inline
static const C *String_SkipUpper(const C *p){
    while(C_IsUpper(*p))
        ++p;
    return p;
}

inline
static const C *String_SkipLower(const C *p){
    while(C_IsLower(*p))
        ++p;
    return p;
}

inline
static const C *String_SkipAlpha(const C *p){
    p = String_SkipLower(p);
    while(C_IsAlpha(*p))
        ++p;
    return p;
}

inline
static C *String_SkipAndLowerAlpha(C *pBegin, C *pEnd){
    C c;
    C *p = pBegin;
    while(p != pEnd){
        c = *p;
        switch(c){
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

inline
static const C *String_TrimEnd(const C *pBegin, C *pEnd, C value){
    const C *pREnd = pBegin - 1;
    const C *p = pEnd - 1;
    while(p != pREnd){
        if(*p != value)
            break;
        --p;
    }
    return ++p;
}

//*pEnd MUST equal cSplitter
inline
static void String_Split(const C *pBegin, const C *pEnd, C cSplitter, String_EventHandler onSubString, void *pContext){
    const C *pSubString;
    const C *p = pBegin;
    while(p < pEnd){
        pSubString = p;
        p = String_SkipUntil(p, cSplitter);
        onSubString(pContext, pSubString, p);
        ++p;
    }
}

inline
static B String_Equal2(const C *pLeft, const C *pRight){
    return *(const U16*)pLeft == *(const U16*)pRight;
}

inline
static B String_Equal4(const C *pLeft, const C *pRight){
    return *(const U32*)pLeft == *(const U32*)pRight;
}

inline
static B String_Equal6(const C *pLeft, const C *pRight4, const C *pRight2){
    return String_Equal4(pLeft, pRight4) && String_Equal2(pLeft + 4, pRight2);
}

inline
static B String_Equal8(const C *pLeft, const C *pRight){
    return *(const U64*)pLeft == *(const U64*)pRight;
}

#define HEXCHARS "0123456789ABCDEF"

inline
static C *String_Hex8(U8 uValue, C *pBuffer){
   *pBuffer++ = HEXCHARS[uValue >> 4];
   *pBuffer++ = HEXCHARS[uValue & 0x0F];
  return pBuffer;
}

inline
static C *String_Hex16(U16 uValue, C *pBuffer){
  *pBuffer++ = HEXCHARS[uValue >> 12];
  *pBuffer++ = HEXCHARS[(uValue >> 8) & 0x0F];
  *pBuffer++ = HEXCHARS[(uValue >> 4) & 0x0F];
  *pBuffer++ = HEXCHARS[uValue & 0x0F];
  return pBuffer;
}

inline
static C *String_Hex32(U32 uValue, C *pBuffer){
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

inline
static C *String_Hex64(U64 uValue, C *pBuffer){
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

