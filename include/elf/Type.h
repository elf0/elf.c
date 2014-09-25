#ifndef TYPE_H
#define TYPE_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

typedef unsigned char Bool;
#define false 0
#define true  1

typedef void* Pointer;
#define null  0

typedef unsigned char      Byte;
typedef unsigned char      Char;

typedef unsigned long      U;
typedef unsigned char      U8;
typedef unsigned short     U16;
typedef unsigned int       U32;
typedef unsigned long long U64;

static inline U32 U32_Add(U32 uLeft, U32 uRight, U32 *puOverflow){
    register U32 uOverflow = 0;
    register U32 uResult;

    asm volatile("addl %[uRight], %%eax\n\t"
                 "jnb 0f\n\t"
                 "movl $1, %[uOverflow]\n\t"
                 "0:\n\t"
                 : [uResult] "=a" (uResult), [uOverflow] "+r" (uOverflow)
                 : [uLeft] "a" (uLeft), [uRight] "r" (uRight)
                 : "cc");

    *puOverflow = uOverflow;
    return uResult;
}

static inline U64 U64_Add(U64 uLeft, U64 uRight, U64 *puOverflow){
    register U64 uOverflow = 0;
    register U64 uResult;

    asm volatile("addq %[uRight], %%rax\n\t"
                 "jnb 0f\n\t"
                 "movq $1, %[uOverflow]\n\t"
                 "0:\n\t"
                 : [uResult] "=a" (uResult), [uOverflow] "+r" (uOverflow)
                 : [uLeft] "a" (uLeft), [uRight] "r" (uRight)
                 : "cc");

    *puOverflow = uOverflow;
    return uResult;
}

static inline U32 U32_Mul(U32 uLeft, U32 uRight, U32 *puOverflow){
    register U32 uOverflow;
    register U32 uResult;

    asm volatile(
                "mull %[uRight]\n\t"
                : [uResult] "=a" (uResult), [uOverflow] "=d" (uOverflow)
                : [uLeft] "a" (uLeft), [uRight] "r" (uRight)
                : "cc");

    *puOverflow = uOverflow;
    return uResult;
}

typedef signed long        I;
typedef signed char        I8;
typedef signed short       I16;
typedef signed int         I32;
typedef signed long long  I64;

static inline I32 I32_Add(I32 iLeft, I32 iRight, I32 *piOverflow){
    register I32 iOverflow = 0;
    register I32 iResult;

    asm volatile("addl %[iRight], %%eax\n\t"
                 "jno 0f\n\t"
                 "movl $1, %[iOverflow]\n\t"
                 "0:\n\t"
                 : [iResult] "=a" (iResult), [iOverflow] "+r" (iOverflow)
                 : [iLeft] "a" (iLeft), [iRight] "r" (iRight)
                 : "cc");

    *piOverflow = iOverflow;
    return iResult;
}

static inline I64 I64_Add(I64 iLeft, I64 iRight, I64 *piOverflow){
    register I64 iOverflow = 0;
    register I64 iResult;

    asm volatile("addq %[iRight], %%rax\n\t"
                 "jno 0f\n\t"
                 "movq $1, %[iOverflow]\n\t"
                 "0:\n\t"
                 : [iResult] "=a" (iResult), [iOverflow] "+r" (iOverflow)
                 : [iLeft] "a" (iLeft), [iRight] "r" (iRight)
                 : "cc");

    *piOverflow = iOverflow;
    return iResult;
}

typedef float              F32;

#define F32_BINARY_POSITIVE_INFINITY  0x7F800000
#define F32_BINARY_NEGATIVE_INFINITY 0xFF800000

static inline F32 F32_PositiveInfinity(){
    U32 value = F32_BINARY_POSITIVE_INFINITY;
    return *(F32*)&value;
}

#define F32_POSITIVE_INFINITY F32_PositiveInfinity()

static inline F32 F32_NegativeInfinity(){
    U32 value = F32_BINARY_NEGATIVE_INFINITY;
    return *(F32*)&value;
}

#define F32_NEGATIVE_INFINITY F32_NegativeInfinity()

static inline Bool F32_IsNan(F32 fValue){
    return fValue != fValue;
}

static inline Bool F32_IsPositiveInfinity(F32 fValue){
    return *(U32*)&fValue == F32_BINARY_POSITIVE_INFINITY;
}

static inline Bool F32_IsNegativeInfinity(F32 fValue){
    return *(U32*)&fValue == F32_BINARY_NEGATIVE_INFINITY;
}

typedef double             F64;

#define F64_BINARY_POSITIVE_INFINITY  0x7FF0000000000000
#define F64_BINARY_NEGATIVE_INFINITY 0xFFF0000000000000

static inline F64 F64_PositiveInfinity(){
    F64 value = F64_BINARY_POSITIVE_INFINITY;
    return *(F64*)&value;
}

#define F64_POSITIVE_INFINITY F64_PositiveInfinity()

static inline F64 F64_NegativeInfinity(){
    F64 value = F64_BINARY_NEGATIVE_INFINITY;
    return *(F64*)&value;
}

#define F64_NEGATIVE_INFINITY F64_NegativeInfinity()

static inline Bool F64_IsNan(F64 fValue){
    return fValue != fValue;
}

static inline Bool F64_IsPositiveInfinity(F64 fValue){
    return *(U64*)&fValue == F64_BINARY_POSITIVE_INFINITY;
}

static inline Bool F64_IsNegativeInfinity(F64 fValue){
    return *(U64*)&fValue == F64_BINARY_NEGATIVE_INFINITY;
}

#endif //TYPE_H
