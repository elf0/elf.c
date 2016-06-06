#ifndef FLOAT_H
#define FLOAT_H

#include "Type.h"

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

static inline B F32_IsNan(F32 fValue){
    return fValue != fValue;
}

static inline B F32_IsPositiveInfinity(F32 fValue){
    return *(U32*)&fValue == F32_BINARY_POSITIVE_INFINITY;
}

static inline B F32_IsNegativeInfinity(F32 fValue){
    return *(U32*)&fValue == F32_BINARY_NEGATIVE_INFINITY;
}

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

static inline B F64_IsNan(F64 fValue){
    return fValue != fValue;
}

static inline B F64_IsPositiveInfinity(F64 fValue){
    return *(U64*)&fValue == F64_BINARY_POSITIVE_INFINITY;
}

static inline B F64_IsNegativeInfinity(F64 fValue){
    return *(U64*)&fValue == F64_BINARY_NEGATIVE_INFINITY;
}
#endif // FLOAT_H
