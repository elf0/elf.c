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


#include "_String.h"

#endif // STRING_H

