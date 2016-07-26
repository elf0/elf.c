#ifndef TYPE_H
#define TYPE_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

#if defined(__GNUC__) \
    && defined(__BYTE_ORDER__)

#if defined(__ORDER_BIG_ENDIAN__) \
    && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#define ELF_BIG_ENDIAN
#elif defined(__ORDER_LITTLE_ENDIAN__) \
    && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#define ELF_LITTLE_ENDIAN
#endif

#endif

typedef unsigned char      U8;
typedef unsigned short     U16;
typedef unsigned int       U32;
typedef unsigned long long U64;

typedef signed char        I8;
typedef signed short       I16;
typedef signed int         I32;
typedef signed long long  I64;

typedef float              F32;
typedef double             F64;

#if defined(__x86_64__) || defined(_WIN64)
typedef U64 U;
typedef I64 I;
typedef F64 F;
#else
typedef U32 U;
typedef I32 I;
typedef F32 F;
#endif

//Character
typedef U8 C;
typedef U16 C16;
typedef U32 C32;

//Boolean
typedef U8 B;
#define false 0
#define true  1

typedef U8 Byte;

typedef Byte* P;
#define null  0

//Error code
typedef U8 E8;
typedef U16 E16;
typedef U32 E32;
typedef U64 E64;
typedef U E;

//Result code
typedef I8 R8;
typedef I16 R16;
typedef I32 R32;
typedef I64 R64;
typedef I R;

#define ContainerOf(pointer, Member, Container) \
    ((Container*)((P)(pointer) - (P)&((Container*)0)->Member))

#endif //TYPE_H
