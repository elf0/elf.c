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

//Parse '0b', '+0b', '-0b' prefix youself
static inline Bool String_ParseBinaryI32_Positive(const Char **ppszNumber, I32 *piValue);
static inline Bool String_ParseBinaryI32_Negative(const Char **ppszNumber, I32 *piValue);
static inline Bool String_ParseBinaryI64_Positive(const Char **ppszNumber, I64 *piValue);
static inline Bool String_ParseBinaryI64_Negative(const Char **ppszNumber, I64 *piValue);

//Parse '0o', '+0o', '-0o' prefix youself
static inline Bool String_ParseOctalI32_Positive(const Char **ppszNumber, I32 *piValue);
static inline Bool String_ParseOctalI32_Negative(const Char **ppszNumber, I32 *piValue);
static inline Bool String_ParseOctalI64_Positive(const Char **ppszNumber, I64 *piValue);
static inline Bool String_ParseOctalI64_Negative(const Char **ppszNumber, I64 *piValue);

//Parse '0x', '+0x', '-0x' prefix youself
static inline Bool String_ParseHexI32_Positive(const Char **ppszNumber, I32 *piValue);
static inline Bool String_ParseHexI32_Negative(const Char **ppszNumber, I32 *piValue);
static inline Bool String_ParseHexI64_Positive(const Char **ppszNumber, I64 *piValue);
static inline Bool String_ParseHexI64_Negative(const Char **ppszNumber, I64 *piValue);

static inline Bool String_ParseIp(const Char **ppIp, U32 *puIp);

#include "_String.h"

#endif // STRING_H

