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

static inline Bool String_Equal2(const Char *pLeft, const Char *pRight);
static inline Bool String_Equal4(const Char *pLeft, const Char *pRight);
static inline Bool String_Equal6(const Char *pLeft, const Char *pRight4, const Char *pRight2);
static inline Bool String_Equal8(const Char *pLeft, const Char *pRight);

//Digits string

//From 'pBegin' to 'pEnd' MUST be a valid U32 number.
static inline U32 String_ToU32(const Char *pBegin, const Char *pEnd);

//From 'pBegin' to 'pEnd' MUST be a valid U64 number.
static inline U64 String_ToU64(const Char *pBegin, const Char *pEnd);

static inline Bool String_ParseU8(const Char **ppszNumber, U8 *puValue);
static inline Bool String_ParseU16(const Char **ppszNumber, U16 *puValue);
static inline Bool String_ParseU32(const Char **ppszNumber, U32 *puValue);
static inline Bool String_ParseU64(const Char **ppszNumber, U64 *puValue);

//Parse '+' youself.
static inline Bool String_ParseI32_Positive(const Char **ppszNumber, I32 *piValue);
static inline Bool String_ParseI64_Positive(const Char **ppszNumber, I64 *piValue);

//Parse '-' youself
static inline Bool String_ParseI32_Negative(const Char **ppszNumber, I32 *piValue);
static inline Bool String_ParseI64_Negative(const Char **ppszNumber, I64 *piValue);

//Parse '0b' prefix youself
static inline Bool String_ParseBinaryU32(const Char **ppszNumber, U32 *puValue);
static inline Bool String_ParseBinaryU64(const Char **ppszNumber, U64 *puValue);

//Parse '0o' prefix youself
static inline Bool String_ParseOctalU32(const Char **ppszNumber, U32 *puValue);
static inline Bool String_ParseOctalU64(const Char **ppszNumber, U64 *puValue);

//Parse '0x' prefix youself
static inline Bool String_ParseHexU32(const Char **ppszNumber, U32 *puValue);
static inline Bool String_ParseHexU64(const Char **ppszNumber, U64 *puValue);

//Parse '0x', '+0x', '-0x' prefix youself
static inline Bool String_ParseHexI32_Positive(const Char **ppszNumber, I32 *piValue);
static inline Bool String_ParseHexI32_Negative(const Char **ppszNumber, I32 *piValue);
static inline Bool String_ParseHexI64_Positive(const Char **ppszNumber, I64 *piValue);
static inline Bool String_ParseHexI64_Negative(const Char **ppszNumber, I64 *piValue);

static inline Bool String_ParseIp(const Char **ppIp, U32 *puIp);

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
        uValue = 10 * uValue + (*p++ - '0');
        //        ++p;
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

static inline Bool String_ParseU8(const Char **ppszNumber, U8 *puValue){
    const Char *p = String_Skip(*ppszNumber, '0');
    U8 uValue = 0;
    //Max: 255
#define U8_OVERFLOW_BEFORE_MUL 25U
#define U8_OVERFLOW_BEFORE_ADD 250U

    while(true){
        I32 iRange = *p - '0';
        if(iRange < 0 || iRange > 9)
            break;
        ++p;


        if(uValue < U8_OVERFLOW_BEFORE_MUL)
            uValue = uValue * 10 + iRange;
        else if(uValue > U8_OVERFLOW_BEFORE_MUL || iRange > 5){
            *ppszNumber = --p;
            *puValue = uValue;
            return true;
        }else
            uValue = U8_OVERFLOW_BEFORE_ADD + iRange;
    }

    *ppszNumber = p;
    *puValue = uValue;
    return false;
}

static inline Bool String_ParseU16(const Char **ppszNumber, U16 *puValue){
    const Char *p = String_Skip(*ppszNumber, '0');
    U16 uValue = 0;
    //Max: 65535
#define U16_OVERFLOW_BEFORE_MUL 6553U
#define U16_OVERFLOW_BEFORE_ADD 65530U

    while(true){
        I32 iRange = *p - '0';
        if(iRange < 0 || iRange > 9)
            break;
        ++p;


        if(uValue < U16_OVERFLOW_BEFORE_MUL)
            uValue = uValue * 10 + iRange;
        else if(uValue > U16_OVERFLOW_BEFORE_MUL || iRange > 5){
            *ppszNumber = --p;
            *puValue = uValue;
            return true;
        }else
            uValue = U16_OVERFLOW_BEFORE_ADD + iRange;
    }

    *ppszNumber = p;
    *puValue = uValue;
    return false;
}

static inline Bool String_ParseU32(const Char **ppszNumber, U32 *puValue){
    const Char *p = String_Skip(*ppszNumber, '0');
    U32 uValue = 0;
    //Max: 4294967295
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
    const Char *p = String_Skip(*ppszNumber, '0');
    U64 uValue = 0;
    //Max: 18446744073709551615
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

static inline Bool String_ParseI32_Positive(const Char **ppszNumber, I32 *piValue){
    const Char *p = String_Skip(*ppszNumber, '0');
    I32 iValue = 0;
    //Max: 2147483647
#define I32_POSITIVE_OVERFLOW_BEFORE_MUL 214748364
#define I32_POSITIVE_OVERFLOW_BEFORE_ADD 2147483640

    while(true){
        I32 iRange = *p - '0';
        if(iRange < 0 || iRange > 9)
            break;
        ++p;


        if(iValue < I32_POSITIVE_OVERFLOW_BEFORE_MUL)
            iValue = iValue * 10 + iRange;
        else if(iValue > I32_POSITIVE_OVERFLOW_BEFORE_MUL || iRange > 7){
            *ppszNumber = --p;
            *piValue = iValue;
            return true;
        }else
            iValue = I32_POSITIVE_OVERFLOW_BEFORE_ADD + iRange;
    }

    *ppszNumber = p;
    *piValue = iValue;
    return false;
}

static inline Bool String_ParseI64_Positive(const Char **ppszNumber, I64 *piValue){
    const Char *p = String_Skip(*ppszNumber, '0');
    I64 iValue = 0;
    //Max: 9223372036854775807
#define I64_POSITIVE_OVERFLOW_BEFORE_MUL 922337203685477580LL
#define I64_POSITIVE_OVERFLOW_BEFORE_ADD 9223372036854775800LL

    while(true){
        I32 iRange = *p - '0';
        if(iRange < 0 || iRange > 9)
            break;
        ++p;


        if(iValue < I64_POSITIVE_OVERFLOW_BEFORE_MUL)
            iValue = iValue * 10 + iRange;
        else if(iValue > I64_POSITIVE_OVERFLOW_BEFORE_MUL || iRange > 7){
            *ppszNumber = --p;
            *piValue = iValue;
            return true;
        }else
            iValue = I64_POSITIVE_OVERFLOW_BEFORE_ADD + iRange;
    }

    *ppszNumber = p;
    *piValue = iValue;
    return false;
}

static inline Bool String_ParseI32_Negative(const Char **ppszNumber, I32 *piValue){
    const Char *p = String_Skip(*ppszNumber, '0');
    if(!Char_IsDigit(*p)){
        *ppszNumber = p;
        *piValue = 0;
        return false;
    }
    I32 iValue = -(*p++ - '0');
    //Min: -2147483648
#define I32_NEGATIVE_OVERFLOW_BEFORE_MUL -214748364
#define I32_NEGATIVE_OVERFLOW_BEFORE_ADD -2147483640

    while(true){
        I32 iRange = *p - '0';
        if(iRange < 0 || iRange > 9)
            break;
        ++p;


        if(iValue > I32_NEGATIVE_OVERFLOW_BEFORE_MUL)
            iValue = iValue * 10 - iRange;
        else if(iValue < I32_NEGATIVE_OVERFLOW_BEFORE_MUL || iRange > 8){
            *ppszNumber = --p;
            *piValue = iValue;
            return true;
        }else
            iValue = I32_NEGATIVE_OVERFLOW_BEFORE_ADD - iRange;
    }

    *ppszNumber = p;
    *piValue = iValue;
    return false;
}

static inline Bool String_ParseI64_Negative(const Char **ppszNumber, I64 *piValue){
    const Char *p = String_Skip(*ppszNumber, '0');
    if(!Char_IsDigit(*p)){
        *ppszNumber = p;
        *piValue = 0;
        return false;
    }
    I64 iValue = -(*p++ - '0');
    //Min: -9223372036854775808
#define I64_NEGATIVE_OVERFLOW_BEFORE_MUL -922337203685477580LL
#define I64_NEGATIVE_OVERFLOW_BEFORE_ADD -9223372036854775800LL

    while(true){
        I32 iRange = *p - '0';
        if(iRange < 0 || iRange > 9)
            break;
        ++p;


        if(iValue > I64_NEGATIVE_OVERFLOW_BEFORE_MUL)
            iValue = iValue * 10 - iRange;
        else if(iValue < I64_NEGATIVE_OVERFLOW_BEFORE_MUL || iRange > 8){
            *ppszNumber = --p;
            *piValue = iValue;
            return true;
        }else
            iValue = I64_NEGATIVE_OVERFLOW_BEFORE_ADD - iRange;
    }

    *ppszNumber = p;
    *piValue = iValue;
    return false;
}

static inline Bool String_ParseBinaryU32(const Char **ppszNumber, U32 *puValue){
    const Char *p = String_Skip(*ppszNumber, '0');
    U32 uValue = 0;
    //Max: 11111111111111111111111111111111
#define BINARY_U32_OVERFLOW_BEFORE_MUL 0x7FFFFFFFU

    while(true){
        I32 iRange = *p - '0';
        if(iRange < 0 || iRange > 1)
            break;
        ++p;

        if(uValue > BINARY_U32_OVERFLOW_BEFORE_MUL){
            *ppszNumber = --p;
            *puValue = uValue;
            return true;
        }
        else
            uValue = (uValue << 1) + iRange;
    }

    *ppszNumber = p;
    *puValue = uValue;
    return false;
}

static inline Bool String_ParseBinaryU64(const Char **ppszNumber, U64 *puValue){
    const Char *p = String_Skip(*ppszNumber, '0');
    U64 uValue = 0;
    //Max: 1111111111111111111111111111111111111111111111111111111111111111
#define BINARY_U64_OVERFLOW_BEFORE_MUL 0x7FFFFFFFFFFFFFFFLLU

    while(true){
        I32 iRange = *p - '0';
        if(iRange < 0 || iRange > 1)
            break;
        ++p;

        if(uValue > BINARY_U64_OVERFLOW_BEFORE_MUL){
            *ppszNumber = --p;
            *puValue = uValue;
            return true;
        }
        else
            uValue = (uValue << 1) + iRange;
    }

    *ppszNumber = p;
    *puValue = uValue;
    return false;
}

static inline Bool String_ParseOctalU32(const Char **ppszNumber, U32 *puValue){
    const Char *p = String_Skip(*ppszNumber, '0');
    U32 uValue = 0;
    //Max: 37777777777
#define OCTAL_U32_OVERFLOW_BEFORE_MUL 03777777777U

    while(true){
        I32 iRange = *p - '0';
        if(iRange < 0 || iRange > 7)
            break;
        ++p;

        if(uValue > OCTAL_U32_OVERFLOW_BEFORE_MUL){
            *ppszNumber = --p;
            *puValue = uValue;
            return true;
        }
        else
            uValue = (uValue << 3) + iRange;
    }

    *ppszNumber = p;
    *puValue = uValue;
    return false;
}

static inline Bool String_ParseOctalU64(const Char **ppszNumber, U64 *puValue){
    const Char *p = String_Skip(*ppszNumber, '0');
    U64 uValue = 0;
    //Max: 1777777777777777777777
#define OCTAL_U64_OVERFLOW_BEFORE_MUL 0177777777777777777777LLU

    while(true){
        I32 iRange = *p - '0';
        if(iRange < 0 || iRange > 7)
            break;
        ++p;

        if(uValue > OCTAL_U64_OVERFLOW_BEFORE_MUL){
            *ppszNumber = --p;
            *puValue = uValue;
            return true;
        }
        else
            uValue = (uValue << 3) + iRange;
    }

    *ppszNumber = p;
    *puValue = uValue;
    return false;
}

static inline Bool String_ParseHexU32(const Char **ppszNumber, U32 *puValue){
    const Char *p = String_Skip(*ppszNumber, '0');
    U32 uValue = 0;
    //Max: FFFFFFFF
#define HEX_U32_OVERFLOW_BEFORE_MUL 0xFFFFFFFU

    while(true){
        switch(*p){
        default:
            *ppszNumber = p;
            *puValue = uValue;
            return false;
        case CASE_CHAR_DIGIT:
            if(uValue > HEX_U32_OVERFLOW_BEFORE_MUL){
                *ppszNumber = p;
                *puValue = uValue;
                return true;
            }
            uValue = (uValue << 4) | (*p - '0');
            break;
        case CASE_CHAR_HEX_LETTER_UPPER:
            if(uValue > HEX_U32_OVERFLOW_BEFORE_MUL){
                *ppszNumber = p;
                *puValue = uValue;
                return true;
            }
            uValue = (uValue << 4) | (10 + (*p - 'A'));
            break;
        case CASE_CHAR_HEX_LETTER_LOWER:
            if(uValue > HEX_U32_OVERFLOW_BEFORE_MUL){
                *ppszNumber = p;
                *puValue = uValue;
                return true;
            }
            uValue = (uValue << 4) | (10 + (*p - 'a'));
            break;
        }
        ++p;
    }
}

static inline Bool String_ParseHexU64(const Char **ppszNumber, U64 *puValue){
    const Char *p = String_Skip(*ppszNumber, '0');
    U64 uValue = 0;
    //Max: FFFFFFFFFFFFFFFF
#define HEX_U64_OVERFLOW_BEFORE_MUL 0xFFFFFFFFFFFFFFFLLU

    while(true){
        switch(*p){
        default:
            *ppszNumber = p;
            *puValue = uValue;
            return false;
        case CASE_CHAR_DIGIT:
            if(uValue > HEX_U64_OVERFLOW_BEFORE_MUL){
                *ppszNumber = p;
                *puValue = uValue;
                return true;
            }
            uValue = (uValue << 4) | (*p - '0');
            break;
        case CASE_CHAR_HEX_LETTER_UPPER:
            if(uValue > HEX_U64_OVERFLOW_BEFORE_MUL){
                *ppszNumber = p;
                *puValue = uValue;
                return true;
            }
            uValue = (uValue << 4) | (10 + (*p - 'A'));
            break;
        case CASE_CHAR_HEX_LETTER_LOWER:
            if(uValue > HEX_U64_OVERFLOW_BEFORE_MUL){
                *ppszNumber = p;
                *puValue = uValue;
                return true;
            }
            uValue = (uValue << 4) | (10 + (*p - 'a'));
            break;
        }
        ++p;
    }
}

static inline Bool String_ParseHexI32_Positive(const Char **ppszNumber, I32 *piValue){
    const Char *p = String_Skip(*ppszNumber, '0');
    I32 iValue = 0;
    //Max: 7FFFFFFF
#define HEX_POSITIVE_I32_OVERFLOW_BEFORE_MUL 0x7FFFFFF

    while(true){
        switch(*p){
        default:
            *ppszNumber = p;
            *piValue = iValue;
            return false;
        case CASE_CHAR_DIGIT:
            if(iValue > HEX_POSITIVE_I32_OVERFLOW_BEFORE_MUL){
                *ppszNumber = p;
                *piValue = iValue;
                return true;
            }
            iValue = (iValue << 4) | (*p - '0');
            break;
        case CASE_CHAR_HEX_LETTER_UPPER:
            if(iValue > HEX_POSITIVE_I32_OVERFLOW_BEFORE_MUL){
                *ppszNumber = p;
                *piValue = iValue;
                return true;
            }
            iValue = (iValue << 4) | (10 + (*p - 'A'));
            break;
        case CASE_CHAR_HEX_LETTER_LOWER:
            if(iValue > HEX_POSITIVE_I32_OVERFLOW_BEFORE_MUL){
                *ppszNumber = p;
                *piValue = iValue;
                return true;
            }
            iValue = (iValue << 4) | (10 + (*p - 'a'));
            break;
        }
        ++p;
    }
}

static inline Bool String_ParseHexI32_Negative(const Char **ppszNumber, I32 *piValue){
    const Char *p = String_Skip(*ppszNumber, '0');
    I32 iValue;
    switch(*p){
    default:
        *ppszNumber = p;
        *piValue = 0;
        return false;
    case CASE_CHAR_DIGIT:
        iValue = '0' - *p;
        break;
    case CASE_CHAR_HEX_LETTER_UPPER:
        iValue = -10 + ('A' - *p);
        break;
    case CASE_CHAR_HEX_LETTER_LOWER:
        iValue = -10 + ('a' - *p);
        break;
    }
    ++p;

#define HEX_NEGATIVE_I32_OVERFLOW_MAX -0x80000000
#define HEX_NEGATIVE_I32_OVERFLOW_BEFORE_MUL -0x8000000

    while(true){
        switch(*p){
        default:
            *ppszNumber = p;
            *piValue = iValue;
            return false;
        case CASE_CHAR_DIGIT:
            if(iValue > HEX_NEGATIVE_I32_OVERFLOW_BEFORE_MUL)
                iValue = (iValue << 4) + ('0' - *p);
            else if(iValue < HEX_NEGATIVE_I32_OVERFLOW_BEFORE_MUL || *p != '0'){
                *ppszNumber = p;
                *piValue = iValue;
                return true;
            }
            else
                iValue = HEX_NEGATIVE_I32_OVERFLOW_MAX;
            break;
        case CASE_CHAR_HEX_LETTER_UPPER:
            if(iValue > HEX_NEGATIVE_I32_OVERFLOW_BEFORE_MUL)
                iValue = (iValue << 4) + (10 + ('A' - *p));
            else{
                *ppszNumber = p;
                *piValue = iValue;
                return true;
            }
            break;
        case CASE_CHAR_HEX_LETTER_LOWER:
            if(iValue > HEX_NEGATIVE_I32_OVERFLOW_BEFORE_MUL)
                iValue = (iValue << 4) + (10 + ('a' - *p));
            else{
                *ppszNumber = p;
                *piValue = iValue;
                return true;
            }
            break;
        }
        ++p;
    }
}

static inline Bool String_ParseHexI64_Positive(const Char **ppszNumber, I64 *piValue){
    const Char *p = String_Skip(*ppszNumber, '0');
    I64 iValue = 0;
    //Max: 0x7FFFFFFFFFFFFFFF
#define HEX_POSITIVE_I64_OVERFLOW_BEFORE_MUL 0x7FFFFFFFFFFFFFFLL

    while(true){
        switch(*p){
        default:
            *ppszNumber = p;
            *piValue = iValue;
            return false;
        case CASE_CHAR_DIGIT:
            if(iValue > HEX_POSITIVE_I64_OVERFLOW_BEFORE_MUL){
                *ppszNumber = p;
                *piValue = iValue;
                return true;
            }
            iValue = (iValue << 4) | (*p - '0');
            break;
        case CASE_CHAR_HEX_LETTER_UPPER:
            if(iValue > HEX_POSITIVE_I64_OVERFLOW_BEFORE_MUL){
                *ppszNumber = p;
                *piValue = iValue;
                return true;
            }
            iValue = (iValue << 4) | (10 + (*p - 'A'));
            break;
        case CASE_CHAR_HEX_LETTER_LOWER:
            if(iValue > HEX_POSITIVE_I64_OVERFLOW_BEFORE_MUL){
                *ppszNumber = p;
                *piValue = iValue;
                return true;
            }
            iValue = (iValue << 4) | (10 + (*p - 'a'));
            break;
        }
        ++p;
    }
}

static inline Bool String_ParseHexI64_Negative(const Char **ppszNumber, I64 *piValue){
    const Char *p = String_Skip(*ppszNumber, '0');
    I64 iValue;
    switch(*p){
    default:
        *ppszNumber = p;
        *piValue = 0;
        return false;
    case CASE_CHAR_DIGIT:
        iValue = '0' - *p;
        break;
    case CASE_CHAR_HEX_LETTER_UPPER:
        iValue = -10 + ('A' - *p);
        break;
    case CASE_CHAR_HEX_LETTER_LOWER:
        iValue = -10 + ('a' - *p);
        break;
    }
    ++p;

#define HEX_NEGATIVE_I64_OVERFLOW_MAX -0x8000000000000000LL
#define HEX_NEGATIVE_I64_OVERFLOW_BEFORE_MUL -0x800000000000000LL

    while(true){
        switch(*p){
        default:
            *ppszNumber = p;
            *piValue = iValue;
            return false;
        case CASE_CHAR_DIGIT:
            if(iValue > HEX_NEGATIVE_I64_OVERFLOW_BEFORE_MUL)
                iValue = (iValue << 4) + ('0' - *p);
            else if(iValue < HEX_NEGATIVE_I64_OVERFLOW_BEFORE_MUL || *p != '0'){
                *ppszNumber = p;
                *piValue = iValue;
                return true;
            }
            else
                iValue = HEX_NEGATIVE_I64_OVERFLOW_MAX;
            break;
        case CASE_CHAR_HEX_LETTER_UPPER:
            if(iValue > HEX_NEGATIVE_I64_OVERFLOW_BEFORE_MUL)
                iValue = (iValue << 4) + (-10 + ('A' - *p));
            else{
                *ppszNumber = p;
                *piValue = iValue;
                return true;
            }
            break;
        case CASE_CHAR_HEX_LETTER_LOWER:
            if(iValue > HEX_NEGATIVE_I64_OVERFLOW_BEFORE_MUL)
                iValue = (iValue << 4) + (-10 + ('a' - *p));
            else{
                *ppszNumber = p;
                *piValue = iValue;
                return true;
            }
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

static inline Bool String_ParseIp(const Char **ppIp, U32 *puIp){
    const Char *p = *ppIp;

    if(!Char_IsDigit(*p))
        return false;

    union{
        U32 uIp;
        U8 szIp[4];
    }ip;

    if(String_ParseU8(&p, &ip.szIp[0])){
        *ppIp = p;
        return false;
    }

    if(*p != '.'){
        *ppIp = p;
        return false;
    }
    ++p;

    if(!Char_IsDigit(*p)){
        *ppIp = p;
        return false;
    }

    if(String_ParseU8(&p, &ip.szIp[1])){
        *ppIp = p;
        return false;
    }

    if(*p != '.'){
        *ppIp = p;
        return false;
    }
    ++p;

    if(!Char_IsDigit(*p)){
        *ppIp = p;
        return false;
    }

    if(String_ParseU8(&p, &ip.szIp[2])){
        *ppIp = p;
        return false;
    }

    if(*p != '.'){
        *ppIp = p;
        return false;
    }
    ++p;

    if(!Char_IsDigit(*p)){
        *ppIp = p;
        return false;
    }

    if(String_ParseU8(&p, &ip.szIp[3])){
        *ppIp = p;
        return false;
    }

    *ppIp = p;
    *puIp = ip.uIp;
    return true;
}

#endif // STRING_H

