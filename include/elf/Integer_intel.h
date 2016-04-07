#ifndef INTEGER_H
#define INTEGER_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

#include "Type.h"

static inline U32 U32_Add(U32 uLeft, U32 uRight, U32 *puOverflow){
    register U32 uOverflow = 0;
    register U32 uResult;

    asm volatile("addl %[uRight], %[uLeft]\n\t"
                 "jnb 0f\n\t"
                 "movl $1, %[uOverflow]\n\t"
                 "0:\n\t"
                   : [uResult] "=a" (uResult), [uOverflow] "+d" (uOverflow)
                 : [uLeft] "a" (uLeft), [uRight] "c" (uRight)
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
                 : [uResult] "=a" (uResult), [uOverflow] "+d" (uOverflow)
                 : [uLeft] "a" (uLeft), [uRight] "c" (uRight)
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
                : [uLeft] "a" (uLeft), [uRight] "d" (uRight)
                : "cc");

    *puOverflow = uOverflow;
    return uResult;
}

static inline U64 U64_Mul(U64 uLeft, U64 uRight, U64 *puOverflow){
    register U64 uOverflow;
    register U64 uResult;

    asm volatile(
                "mulq %[uRight]\n\t"
                : [uResult] "=a" (uResult), [uOverflow] "=d" (uOverflow)
                : [uLeft] "a" (uLeft), [uRight] "d" (uRight)
                : "cc");

    *puOverflow = uOverflow;
    return uResult;
}

static inline U32 U32_MulAdd(U32 uLeft, U32 uRight, U32 uAdd, U32 *puOverflow){
    register U32 uOverflow;
    register U32 uResult;

    asm volatile(
                "mull %[uRight]\n\t"
                "addl %[uAdd], %%eax\n\t"
                "jnb 0f\n\t"
                "incl %[uOverflow]\n\t"
                "0:\n\t"
                : [uResult] "=a" (uResult), [uOverflow] "=d" (uOverflow)
                : [uLeft] "a" (uLeft), [uRight] "d" (uRight), [uAdd] "c" (uAdd)
                : "cc");

    *puOverflow = uOverflow;
    return uResult;
}

static inline U64 U64_MulAdd(U64 uLeft, U64 uRight, U64 uAdd, U64 *puOverflow){
    register U64 uOverflow;
    register U64 uResult;

    asm volatile(
                "mulq %[uRight]\n\t"
                "addq %[uAdd], %%rax\n\t"
                "jnb 0f\n\t"
                "incq %[uOverflow]\n\t"
                "0:\n\t"
                : [uResult] "=a" (uResult), [uOverflow] "=d" (uOverflow)
                : [uLeft] "a" (uLeft), [uRight] "d" (uRight), [uAdd] "c" (uAdd)
                : "cc");

    *puOverflow = uOverflow;
    return uResult;
}

static inline I32 I32_Add(I32 iLeft, I32 iRight, I32 *piOverflow){
    register I32 iOverflow = 0;
    register I32 iResult;

    asm volatile("addl %[iRight], %%eax\n\t"
                 "jno 0f\n\t"
                 "movl $1, %[iOverflow]\n\t"
                 "0:\n\t"
                 : [iResult] "=a" (iResult), [iOverflow] "+d" (iOverflow)
                 : [iLeft] "a" (iLeft), [iRight] "c" (iRight)
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
                 : [iResult] "=a" (iResult), [iOverflow] "+d" (iOverflow)
                 : [iLeft] "a" (iLeft), [iRight] "c" (iRight)
                 : "cc");

    *piOverflow = iOverflow;
    return iResult;
}

static inline I32 I32_Mul(I32 iLeft, I32 iRight, I32 *piOverflow){
    register I32 iOverflow;
    register I32 iResult;

    asm volatile(
                "imull %[iRight]\n\t"
                : [iResult] "=a" (iResult), [iOverflow] "=d" (iOverflow)
                : [iLeft] "a" (iLeft), [iRight] "d" (iRight)
                : "cc");

    *piOverflow = iOverflow;
    return iResult;
}

#endif // INTEGER_H
