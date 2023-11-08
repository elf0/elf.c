#ifndef TYPE_H
#define TYPE_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

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

//Boolean
#if defined(__UINT8_TYPE__)
typedef __UINT8_TYPE__ B;
#else
typedef unsigned char  B;
#endif
#define false 0
#define true  1

#if defined(__UINT8_TYPE__)
typedef __UINT8_TYPE__ U8;
#else
typedef unsigned char  U8;
#endif
#define U8_MAX 0xFF

// Can not overflow
inline static U8 U8_CountInRange(U8 min, U8 max) {
    return max - min + 1;
}

inline static U8 U8_IndexInRange(U8 value, U8 min) {
    return value - min;
}

inline static B U8_InRange(U8 value, U8 min, U8 max){
//    return value - min < max - min + 1; //Bug? Both gcc and clang Will optimize it to: return value < max + 1;
    return U8_IndexInRange(value, min) < U8_CountInRange(min, max);
}

inline static B U8_NotInRange(U8 value, U8 min, U8 max){
     return U8_IndexInRange(value, min) >= U8_CountInRange(min, max);
}

#if defined(__UINT16_TYPE__)
typedef __UINT16_TYPE__ U16;
#else
typedef unsigned short  U16;
#endif
#define U16_MAX 0xFFFF

// Can not overflow
inline static U16 U16_CountInRange(U16 min, U16 max) {
    return max - min + 1;
}

inline static U16 U16_IndexInRange(U16 value, U16 min) {
    return value - min;
}

inline static B U16_InRange(U16 value, U16 min, U16 max){
//    return value - min < max - min + 1; //Bug? Both gcc and clang Will optimize it to: return value < max + 1;
    return U16_IndexInRange(value, min) < U16_CountInRange(min, max);
}

inline static B U16_NotInRange(U16 value, U16 min, U16 max){
     return U16_IndexInRange(value, min) >= U16_CountInRange(min, max);
}

#if defined(__UINT32_TYPE__)
typedef __UINT32_TYPE__ U32;
#else
typedef unsigned int    U32;
#endif
#define U24_MAX 0xFFFFFF
#define U32_MAX 0xFFFFFFFF

// Can not overflow
inline static U32 U32_CountInRange(U32 min, U32 max) {
    return max - min + 1;
}

inline static U32 U32_IndexInRange(U32 value, U32 min) {
    return value - min;
}

inline static B U32_InRange(U32 value, U32 min, U32 max){
//    return value - min < max - min + 1; //Bug? Both gcc and clang Will optimize it to: return value < max + 1;
    return U32_IndexInRange(value, min) < U32_CountInRange(min, max);
}

inline static B U32_NotInRange(U32 value, U32 min, U32 max){
     return U32_IndexInRange(value, min) >= U32_CountInRange(min, max);
}

#if defined(__UINT64_TYPE__)
typedef __UINT64_TYPE__    U64;
#else
typedef unsigned long U64;
#endif
#define U40_MAX 0xFFFFFFFFFF
#define U48_MAX 0xFFFFFFFFFFFF
#define U56_MAX 0xFFFFFFFFFFFFFF
#define U64_MAX 0xFFFFFFFFFFFFFFFF

// Can not overflow
inline static U64 U64_CountInRange(U64 min, U64 max) {
    return max - min + 1;
}

inline static U64 U64_IndexInRange(U64 value, U64 min) {
    return value - min;
}

inline static B U64_InRange(U64 value, U64 min, U64 max){
//    return value - min < max - min + 1; //Bug? Both gcc and clang Will optimize it to: return value < max + 1;
    return U64_IndexInRange(value, min) < U64_CountInRange(min, max);
}

inline static B U64_NotInRange(U64 value, U64 min, U64 max){
     return U64_IndexInRange(value, min) >= U64_CountInRange(min, max);
}


#if defined(__INT8_TYPE__)
typedef __INT8_TYPE__ I8;
#else
typedef signed char   I8;
#endif

#if defined(__INT16_TYPE__)
typedef __INT16_TYPE__ I16;
#else
typedef signed short   I16;
#endif

#if defined(__INT32_TYPE__)
typedef __INT32_TYPE__ I32;
#else
typedef signed int     I32;
#endif

#if defined(__INT64_TYPE__)
typedef __INT64_TYPE__    I64;
#else
typedef signed long I64;
#endif

typedef float              F32;
typedef double             F64;

#if defined(__x86_64__) || defined(_WIN64)
#define POINTER_BITS 64
#define POINTER_BYTES 8
typedef U64 U;
typedef I64 I;
typedef F64 F;
#else
#define POINTER_BITS 32
#define POINTER_BYTES 4
typedef U32 U;
typedef I32 I;
typedef F32 F;
#endif

//Character
typedef U8 C;
typedef U16 C16;
typedef U32 C32;

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

#define LOOP while(1)

#define U16_Get(p) (*(U16*)(p))
#define U32_Get(p) (*(U32*)(p))
#define U64_Get(p) (*(U64*)(p))

#define I16_Get(p) (*(I16*)(p))
#define I32_Get(p) (*(I32*)(p))
#define I64_Get(p) (*(I64*)(p))

#if defined(ELF_BIG_ENDIAN)

#define U16_BE(p) (*(U16*)(p))
#define U32_BE(p) (*(U32*)(p))
#define U64_BE(p) (*(U64*)(p))

#define I16_BE(p) (*(I16*)(p))
#define I32_BE(p) (*(I32*)(p))
#define I64_BE(p) (*(I64*)(p))

#if defined(__GNUC__)

#define U16_LE(p) __builtin_bswap16(*(U16*)(p))
#define U32_LE(p) __builtin_bswap32(*(U32*)(p))
#define U64_LE(p) __builtin_bswap64(*(U64*)(p))

#else

inline
static U16 U16_LE(Byte *p) {
    U16 value = *p++;
    value |= *p << 8;
    return value;
}

inline
static U32 U32_LE(Byte *p) {
    U32 value = *p++;
    value |= *p++ << 8;
    value |= *p++ << 16;
    value |= *p << 24;
    return value;
}

inline
static U64 U64_LE(Byte *p) {
    U64 value = *p++;
    value |= *p++ << 8;
    value |= *p++ << 16;
    value |= *p++ << 24;
    value |= *p++ << 32;
    value |= *p++ << 40;
    value |= *p++ << 48;
    value |= *p << 56;
    return value;
}

#endif

#define I16_LE(p) ((I16)U16_LE(p))
#define I32_LE(p) ((I32)U32_LE(p))
#define I64_LE(p) ((I64)U64_LE(p))

#else

#define U16_LE(p) (*(U16*)(p))
#define U32_LE(p) (*(U32*)(p))
#define U64_LE(p) (*(U64*)(p))

#define I16_LE(p) (*(I16*)(p))
#define I32_LE(p) (*(I32*)(p))
#define I64_LE(p) (*(I64*)(p))

#if defined(__GNUC__)

#define U16_BE(p) __builtin_bswap16(*(U16*)(p))
#define U32_BE(p) __builtin_bswap32(*(U32*)(p))
#define U64_BE(p) __builtin_bswap64(*(U64*)(p))

#else

inline
static U16 U16_BE(Byte *p) {
    U16 value = *p++;
    value <<= 8;
    value |= *p;
    return value;
}

inline
static U32 U32_BE(U8 *p) {
    U32 value = *p++;
    value <<= 8;
    value |= *p++;
    value <<= 8;
    value |= *p++;
    value <<= 8;
    value |= *p;
    return value;
}

inline
static U64 U64_BE(U8 *p) {
  U64 value = *p++;
  value <<= 8;
  value |= *p++;
  value <<= 8;
  value |= *p++;
  value <<= 8;
  value |= *p++;
  value <<= 8;
  value |= *p++;
  value <<= 8;
  value |= *p++;
  value <<= 8;
  value |= *p++;
  value <<= 8;
  value |= *p;
  return value;
}
#endif

#define I16_BE(p) ((I16)U16_BE(p))
#define I32_BE(p) ((I32)U32_BE(p))
#define I64_BE(p) ((I64)U64_BE(p))

#endif

inline
static B Byte2_Equal(const Byte *pLeft, const Byte *pRight){
    return *(const U16*)pLeft == *(const U16*)pRight;
}

inline
static B Byte4_Equal(const Byte *pLeft, const Byte *pRight){
    return *(const U32*)pLeft == *(const U32*)pRight;
}

inline
static B Byte8_Equal(const Byte *pLeft, const Byte *pRight){
    return *(const U64*)pLeft == *(const U64*)pRight;
}

#endif //TYPE_H
