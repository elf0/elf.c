#ifndef STRING_H
#define STRING_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "Char.h"

typedef void (*String_EventHandler)(void *pContext, const Char *pBegin, const Char *pEnd);

static inline const Char *String_Find(const Char *pBegin, const Char *pEnd, Char value);
static inline const Char *String_FindLast(const Char *pBegin, const Char *pEnd, Char value);
static inline const Char *String_FindSub(const Char *pBegin, const Char *pEnd, const Char *pSub, U32 nSubSize);
static inline Bool String_StartsWith(const Char *pBegin, U32 nSize, const Char *pPrefix, U32 nPrefixSize);
static inline Bool String_StartsWith2(const Char *pBegin, U32 nSize, const Char *pPrefix);
static inline Bool String_StartsWith4(const Char *pBegin, U32 nSize, const Char *pPrefix);
static inline Bool String_StartsWith8(const Char *pBegin, U32 nSize, const Char *pPrefix);
static inline Bool String_EndsWith(const Char *pBegin, U32 nSize, const Char *pSuffix, U32 nSuffixSize);
static inline Bool String_EndsWith2(const Char *pEnd, U32 nSize, const Char *pSuffix);
static inline Bool String_EndsWith4(const Char *pEnd, U32 nSize, const Char *pSuffix);
static inline Bool String_EndsWith8(const Char *pEnd, U32 nSize, const Char *pSuffix);
static inline const Char *String_Skip(const Char *p, const Char value);
static inline const Char *String_SkipUntil(const Char *p, Char value);
static inline const Char *String_SkipForwardUntil(const Char *p, Char value);
static inline const Char *String_SkipDigit(const Char *p);
static inline const Char *String_SkipUpper(const Char *p);
static inline const Char *String_SkipLower(const Char *p);
static inline const Char *String_SkipAlpha(const Char *p);
static inline const Char *String_TrimEnd(const Char *pBegin, Char *pEnd, Char value);
static inline void String_Split(const Char *pBegin, const Char *pEnd, Char cSplitter, String_EventHandler onSubString, void *pContext);

//Digits string
static inline U32 String_ToU32(const Char *pBegin, const Char *pEnd);
static inline U64 String_ToU64(const Char *pBegin, const Char *pEnd);
static inline Bool String_ParseU32(const Char **ppszNumber, U32 *puValue);
static inline Bool String_ParseU64(const Char **ppszNumber, U64 *puValue);
//*piValue MUST be initialized
//Parse '+' and '-' youself
static inline const Char *String_ParseI32(const Char *pszNumber, I32 *piValue);
static inline const Char *String_ParseI64(const Char *pszNumber, I64 *piValue);
//Parse '+' youself
static inline const Char *String_ParseI32_Positive(const Char *pszNumber, I32 *piValue, Bool *pbOverflow);
static inline const Char *String_ParseI64_Positive(const Char *pszNumber, I64 *piValue, Bool *pbOverflow);
//Parse '-' youself
static inline const Char *String_ParseI32_Negative(const Char *pszNumber, I32 *piValue, Bool *pbOverflow);
static inline const Char *String_ParseI64_Negative(const Char *pszNumber, I64 *piValue, Bool *pbOverflow);

static inline Bool String_Equal2(const Char *pLeft, const Char *pRight);
static inline Bool String_Equal4(const Char *pLeft, const Char *pRight);
static inline Bool String_Equal6(const Char *pLeft, const Char *pRight4, const Char *pRight2);
static inline Bool String_Equal8(const Char *pLeft, const Char *pRight);

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

static inline Bool String_StartsWith(const Char *pBegin, U32 nSize, const Char *pPrefix, U32 nPrefixSize){
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

static inline Bool String_StartsWith2(const Char *pBegin, U32 nSize, const Char *pPrefix){
    return nSize > 1 && String_Equal2(pBegin, pPrefix);
}

static inline Bool String_StartsWith4(const Char *pBegin, U32 nSize, const Char *pPrefix){
    return nSize > 3 && String_Equal4(pBegin, pPrefix);
}

static inline Bool String_StartsWith8(const Char *pBegin, U32 nSize, const Char *pPrefix){
    return nSize > 7 && String_Equal8(pBegin, pPrefix);
}

static inline Bool String_EndsWith(const Char *pBegin, U32 nSize, const Char *pSuffix, U32 nSuffixSize){
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

static inline Bool String_EndsWith2(const Char *pEnd, U32 nSize, const Char *pSuffix){
    return nSize > 1 && String_Equal2(pEnd - 2, pSuffix);
}

static inline Bool String_EndsWith4(const Char *pEnd, U32 nSize, const Char *pSuffix){
    return nSize > 3 && String_Equal4(pEnd - 4, pSuffix);
}

static inline Bool String_EndsWith8(const Char *pEnd, U32 nSize, const Char *pSuffix){
    return nSize > 7 && String_Equal8(pEnd - 8, pSuffix);
}


static inline const Char *String_Skip(const Char *p, const Char value){
    while(*p == value)
        ++p;
    return p;
}

static inline const Char *String_SkipUntil(const Char *p, Char value){
    while(*p != value)
        ++p;
    return p;
}

static inline const Char *String_SkipForwardUntil(const Char *p, Char value){
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

static inline U32 String_ToU32(const Char *pBegin, const Char *pEnd){
    const Char *p = pBegin;
    U32 uValue = *p++ - '0';

    while(p != pEnd){
        uValue = 10 * uValue + (*p - '0');
        ++p;
    }

    return uValue;
}

static inline U64 String_ToU64(const Char *pBegin, const Char *pEnd){
    const Char *p = pBegin;
    U64 uValue = *p++ - '0';

    while(p != pEnd){
        uValue = 10 * uValue + (*p++ - '0');
//        ++p;
    }

    return uValue;
}

static inline Bool String_ParseU32(const Char **ppszNumber, U32 *puValue){
    const Char *p = *ppszNumber;
    U32 uValue = *p++ - '0';

//MAX: 4294967295
#define U32_OVERFLOW_BEFORE_MUL 429496729U
#define U32_OVERFLOW_BEFORE_ADD 4294967290U

    while(true){
     I32 iRange = *p - '0';
     if(iRange < 0 || iRange > 9)
      break;
     ++p;


     if(uValue < U32_OVERFLOW_BEFORE_MUL)
      uValue = uValue * 10 + iRange;
     else if(uValue > U32_OVERFLOW_BEFORE_MUL || iRange > 5){
      *ppszNumber = --p;
      *puValue = uValue;
      return true;
     }else
      uValue = U32_OVERFLOW_BEFORE_ADD + iRange;
    }

    *ppszNumber = p;
    *puValue = uValue;
    return false;
}

static inline Bool String_ParseU64(const Char **ppszNumber, U64 *puValue){
    const Char *p = *ppszNumber;
    U64 uValue = *p++ - '0';

//MAX: 18446744073709551615
#define U64_OVERFLOW_BEFORE_MUL 1844674407370955161LLU
#define U64_OVERFLOW_BEFORE_ADD 18446744073709551610LLU

    while(true){
     I32 iRange = *p - '0';
     if(iRange < 0 || iRange > 9)
      break;
     ++p;

     if(uValue < U64_OVERFLOW_BEFORE_MUL)
      uValue = uValue * 10 + iRange;
     else if(uValue > U64_OVERFLOW_BEFORE_MUL || iRange > 5){
      *ppszNumber = --p;
      *puValue = uValue;
      return true;
     }else
      uValue = U64_OVERFLOW_BEFORE_ADD + iRange;
    }

    *ppszNumber = p;
    *puValue = uValue;
    return false;
}

static inline const Char *String_ParseI32(const Char *pszNumber, I32 *piValue){
    I32 iValue = *piValue;
    const Char *p = pszNumber;

    while(Char_IsDigit(*p)){
        iValue = 10 * iValue + (*p - '0');
        ++p;
    }

    *piValue = iValue;
    return p;
}

static inline const Char *String_ParseI64(const Char *pszNumber, I64 *piValue){
    I64 iValue = *piValue;
    const Char *p = pszNumber;

    while(Char_IsDigit(*p)){
        iValue = 10 * iValue + (*p - '0');
        ++p;
    }

    *piValue = iValue;
    return p;
}

static inline const Char *String_ParseI32_Positive(const Char *pszNumber, I32 *piValue, Bool *pbOverflow){
    I32 iNewValue;
    I32 iValue = *piValue;
    const Char *p = pszNumber;

    while(Char_IsDigit(*p)){
        iNewValue = 10 * iValue + (*p - '0');
        if(iNewValue < iValue){
            *piValue = iValue;
            *pbOverflow = true;
            return p;
        }
        iValue = iNewValue;
        ++p;
    }

    *piValue = iValue;
    return p;
}

static inline const Char *String_ParseI64_Positive(const Char *pszNumber, I64 *piValue, Bool *pbOverflow){
    I64 iNewValue;
    I64 iValue = *piValue;
    const Char *p = pszNumber;

    while(Char_IsDigit(*p)){
        iNewValue = 10 * iValue + (*p - '0');
        if(iNewValue < iValue){
            *piValue = iValue;
            *pbOverflow = true;
            return p;
        }
        iValue = iNewValue;
        ++p;
    }

    *piValue = iValue;
    return p;
}

static inline const Char *String_ParseI32_Negative(const Char *pszNumber, I32 *piValue, Bool *pbOverflow){
    I32 iNewValue;
    I32 iValue = *piValue;
    const Char *p = pszNumber;

    while(Char_IsDigit(*p)){
        iNewValue = 10 * iValue + (*p - '0');
        if(iNewValue > iValue){
            *piValue = iValue;
            *pbOverflow = true;
            return p;
        }
        iValue = iNewValue;
        ++p;
    }

    *piValue = iValue;
    return p;
}

static inline const Char *String_ParseI64_Negative(const Char *pszNumber, I64 *piValue, Bool *pbOverflow){
    I64 iNewValue;
    I64 iValue = *piValue;
    const Char *p = pszNumber;

    while(Char_IsDigit(*p)){
        iNewValue = 10 * iValue + (*p - '0');
        if(iNewValue > iValue){
            *piValue = iValue;
            *pbOverflow = true;
            return p;
        }
        iValue = iNewValue;
        ++p;
    }

    *piValue = iValue;
    return p;
}

static inline const Char *String_ParseHexU32(const Char *pszNumber, U32 *puValue){
    U32 u32 = *puValue;
    const Char *p = pszNumber;

    while(true){
        switch(*p){
        default:
            *puValue = u32;
            return p;
        case CASE_CHAR_DIGIT:
            u32 = (u32 << 4) | (*p - '0');
            break;
        case CASE_CHAR_HEX_LETTER_UPPER:
            u32 = (u32 << 4) | (10 + (*p - 'A'));
            break;
        case CASE_CHAR_HEX_LETTER_LOWER:
            u32 = (u32 << 4) | (10 + (*p - 'a'));
            break;
        }
        ++p;
    }
}

static inline const Char *String_ParseHexU64(const Char *pszNumber, U64 *puValue){
    U64 u64 = *puValue;
    const Char *p = pszNumber;

    while(true){
        switch(*p){
        default:
            *puValue = u64;
            return p;
        case CASE_CHAR_DIGIT:
            u64 = (u64 << 4) | (*p - '0');
            break;
        case CASE_CHAR_HEX_LETTER_UPPER:
            u64 = (u64 << 4) | (10 + (*p - 'A'));
            break;
        case CASE_CHAR_HEX_LETTER_LOWER:
            u64 = (u64 << 4) | (10 + (*p - 'a'));
            break;
        }
        ++p;
    }
}

static inline Bool String_Equal2(const Char *pLeft, const Char *pRight){
    return *(const U16*)pLeft == *(const U16*)pRight;
}

static inline Bool String_Equal4(const Char *pLeft, const Char *pRight){
    return *(const U32*)pLeft == *(const U32*)pRight;
}

static inline Bool String_Equal6(const Char *pLeft, const Char *pRight4, const Char *pRight2){
    return String_Equal4(pLeft, pRight4) && String_Equal2(pLeft + 4, pRight2);
}

static inline Bool String_Equal8(const Char *pLeft, const Char *pRight){
    return *(const U64*)pLeft == *(const U64*)pRight;
}

static inline const Char *String_ParseIp(const Char *pIp, U32 *pnIp){
    const Char *p = pIp;
    U32 nIp = 0;
    if(!Char_IsDigit(*p))
        return null;

    if(String_ParseU32(&p, &nIp))
     return null;

    if(*p != '.')
     return null;
    ++p;

    if(!Char_IsDigit(*p))
        return null;

    U32 uValue = 0;
    if(String_ParseU32(&p, &uValue))
     return null;
    nIp = (nIp << 8) | uValue;

    if(*p != '.')
        return 0;
    ++p;

    if(!Char_IsDigit(*p))
        return null;

    uValue = 0;
    if(String_ParseU32(&p, &uValue))
     return null;
    nIp = (nIp << 8) | uValue;

    if(*p != '.')
        return 0;
    ++p;

    if(!Char_IsDigit(*p))
        return null;

    uValue = 0;
    if(String_ParseU32(&p, &uValue))
     return null;
    nIp = (nIp << 8) | uValue;

    *pnIp = nIp;
    return p;
}

#endif // STRING_H

