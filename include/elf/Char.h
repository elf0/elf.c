#ifndef CHAR_H
#define CHAR_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

#include "Type.h"

// Can not overflow
inline static U8 C_CountInRange(C cMin, C cMax) {
    return cMax - cMin + 1;
}

inline static U8 C_IndexInRange(C c, C cMin) {
    return c - cMin;
}

inline static B C_InRange(C c, C cMin, C cMax){
    return C_IndexInRange(c, cMin) < C_CountInRange(cMin, cMax);
}

inline static B C_NotInRange(C c, C cMin, U8 cMax){
     return C_IndexInRange(c, cMin) >= C_CountInRange(cMin, cMax);
}

#define CASE_CHAR_HEX_LETTER_UPPER \
    'A': case 'B': case 'C': case 'D': case 'E': case 'F'

#define CASE_CHAR_UPPER \
    CASE_CHAR_HEX_LETTER_UPPER: case 'G': case 'H': \
    case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': \
    case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': \
    case 'Y': case 'Z'

#define CASE_CHAR_HEX_LETTER_LOWER \
    'a': case 'b': case 'c': case 'd': case 'e': case 'f'

#define CASE_CHAR_LOWER \
    CASE_CHAR_HEX_LETTER_LOWER: case 'g': case 'h': \
    case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': \
    case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': \
    case 'y': case 'z'

#define CASE_CHAR_HEX_LETTER \
    CASE_CHAR_HEX_LETTER_UPPER: case CASE_CHAR_HEX_LETTER_LOWER

#define CASE_CHAR_NONZERODIGIT \
    '1': case '2': case '3': case '4': case '5': case '6': case '7': \
    case '8': case '9'

#define CASE_CHAR_DIGIT \
    '0': case CASE_CHAR_NONZERODIGIT

inline static B C_IsUpper(C c){
    return C_InRange(c, 'A', 'Z');
}

inline static B C_IsLower(C c){
    return C_InRange(c, 'a', 'z');
}

inline
static B C_IsAlpha(C c){
    return C_InRange(c | 0x20, 'a', 'z');
}

inline static B C_IsDigit(C c){
    return C_InRange(c, '0', '9');
}

inline static B C_IsNotDigit(C c){
    return C_NotInRange(c, '0', '9');
}

inline
static B C_IsHexLetterUpper(C c){
    switch(c){
    case CASE_CHAR_HEX_LETTER_UPPER:
        return true;
    }
    return false;
}

inline
static B C_IsHexUpper(C c){
    switch(c){
    case CASE_CHAR_DIGIT: case CASE_CHAR_HEX_LETTER_UPPER:
        return true;
    }
    return false;
}

inline
static B C_IsHexLetterLower(C c){
    switch(c){
    case CASE_CHAR_HEX_LETTER_LOWER:
        return true;
    }
    return false;
}

inline
static B C_IsHexLower(C c){
    switch(c){
    case CASE_CHAR_DIGIT: case CASE_CHAR_HEX_LETTER_LOWER:
        return true;
    }
    return false;
}

inline
static B C_IsHex(C c){
    switch(c){
    case CASE_CHAR_DIGIT: case CASE_CHAR_HEX_LETTER:
        return true;
    }
    return false;
}

inline
static U8 C_HexValue(C c){
    switch(c){
    case CASE_CHAR_DIGIT:
        return c - '0';
    case CASE_CHAR_HEX_LETTER_UPPER:
        return 0x0A + (c - 'A');
    case CASE_CHAR_HEX_LETTER_LOWER:
        return 0x0A + (c - 'a');
    }
    return 0xFF;
}

#endif // CHAR_H


