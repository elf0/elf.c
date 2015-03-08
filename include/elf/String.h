#ifndef STRING_H
#define STRING_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "Char.h"

typedef void (*String_EventHandler)(void *pContext, const Char *pBegin, const Char *pEnd);

static inline Bool String_StartsWith(const Char *pBegin, U32 nSize, const Char *pPrefix, U32 nPrefixSize);
static inline const Char *String_Skip(const Char *p, const Char value);
static inline const Char *String_SkipUntil(const Char *p, Char value);
static inline const Char *String_SkipDigit(const Char *p);
static inline const Char *String_SkipUpper(const Char *p);
static inline const Char *String_SkipLower(const Char *p);
static inline const Char *String_SkipAlpha(const Char *p);
static inline const Char *String_TrimEnd(const Char *pBegin, Char *pEnd, Char value);
static inline void String_Split(const Char *pBegin, const Char *pEnd, Char cSplitter, String_EventHandler onSubString, void *pContext);

//Digits string
static inline U32 String_ToU32(const Char *pBegin, const Char *pEnd);
static inline U64 String_ToU64(const Char *pBegin, const Char *pEnd);
//*puValue MUST be initialized
static inline const Char *String_ParseU32(const Char *pszNumber, U32 *puValue);
static inline const Char *String_ParseU64(const Char *pszNumber, U64 *puValue);
static inline const Char *String_ParseU32_Overflow(const Char *pszNumber, U32 *puValue, Bool *pbOverflow);
static inline const Char *String_ParseU64_Overflow(const Char *pszNumber, U64 *puValue, Bool *pbOverflow);
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
    U32 uValue = *p - '0';
    ++p;

    while(p != pEnd){
        uValue = 10 * uValue + (*p - '0');
        ++p;
    }

    return uValue;
}

static inline U64 String_ToU64(const Char *pBegin, const Char *pEnd){
    const Char *p = pBegin;
    U64 uValue = *p - '0';
    ++p;

    while(p != pEnd){
        uValue = 10 * uValue + (*p - '0');
        ++p;
    }

    return uValue;
}

static inline const Char *String_ParseU32(const Char *pszNumber, U32 *puValue){
    U32 uValue = *puValue;
    const Char *p = pszNumber;

    while(Char_IsDigit(*p)){
        uValue = 10 * uValue + (*p - '0');
        ++p;
    }

    *puValue = uValue;
    return p;
}

static inline const Char *String_ParseU64(const Char *pszNumber, U64 *puValue){
    U64 nValue = *puValue;
    const Char *p = pszNumber;

    while(Char_IsDigit(*p)){
        nValue = 10 * nValue + (*p - '0');
        ++p;
    }

    *puValue = nValue;
    return p;
}

static inline const Char *String_ParseU32_Overflow(const Char *pszNumber, U32 *puValue, Bool *pbOverflow){
    U32 uNewValue;
    U32 uValue = *puValue;
    const Char *p = pszNumber;

    while(Char_IsDigit(*p)){
        uNewValue = 10 * uValue + (*p - '0');
        if(uNewValue < uValue){
            *puValue = uValue;
            *pbOverflow = true;
            return p;
        }
        uValue = uNewValue;
        ++p;
    }

    *puValue = uValue;
    *pbOverflow = false;
    return p;
}

static inline const Char *String_ParseU64_Overflow(const Char *pszNumber, U64 *puValue, Bool *pbOverflow){
    U64 uNewValue;
    U64 uValue = *puValue;
    const Char *p = pszNumber;

    while(Char_IsDigit(*p)){
        uNewValue = 10 * uValue + (*p - '0');
        if(uNewValue < uValue){
            *puValue = uValue;
            *pbOverflow = true;
            return p;
        }
        uValue = uNewValue;
        ++p;
    }

    *puValue = uValue;
    *pbOverflow = false;
    return p;
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

#endif // STRING_H

