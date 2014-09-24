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

typedef signed long        I;
typedef signed char        I8;
typedef signed short       I16;
typedef signed int         I32;
typedef signed long long  I64;

typedef float              F32;

#define F32_BINARY_POSITIVE_INFINITY  0x7F800000
#define F32_BINARY_NEGATIVE_INFINITY 0xFF800000

static inline F32 F32_PositiveInfinity(){
    U32 value = F32_BINARY_POSITIVE_INFINITY;
    return *(F32*)&value;
}

static inline F32 F32_NegativeInfinity(){
    U32 value = F32_BINARY_NEGATIVE_INFINITY;
    return *(F32*)&value;
}

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
static inline F64 F64_NegativeInfinity(){
    F64 value = F64_BINARY_NEGATIVE_INFINITY;
    return *(F64*)&value;
}

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
