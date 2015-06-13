#ifndef CHAR_H
#define CHAR_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "Type.h"

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

static inline Bool Char_IsUpper(Char c){
    return c >= 'A' && c <= 'Z';
}

static inline Bool Char_IsLower(Char c){
    return c >= 'a' && c <= 'z';
}

static inline Bool Char_IsAlpha(Char c){
    switch(c){
    case CASE_CHAR_UPPER: case CASE_CHAR_LOWER:
        return true;
    }
    return false;
}

static inline Bool Char_IsDigit(Char c){
    return c >= '0' && c <= '9';
}

static inline Bool Char_IsNotDigit(Char c){
    return c < '0' || c > '9';
}

static inline Bool Char_IsHexLetterUpper(Char c){
    switch(c){
    case CASE_CHAR_HEX_LETTER_UPPER:
        return true;
    }
    return false;
}

static inline Bool Char_IsHexUpper(Char c){
    switch(c){
    case CASE_CHAR_DIGIT: case CASE_CHAR_HEX_LETTER_UPPER:
        return true;
    }
    return false;
}

static inline Bool Char_IsHexLetterLower(Char c){
    switch(c){
    case CASE_CHAR_HEX_LETTER_LOWER:
        return true;
    }
    return false;
}

static inline Bool Char_IsHexLower(Char c){
    switch(c){
    case CASE_CHAR_DIGIT: case CASE_CHAR_HEX_LETTER_LOWER:
        return true;
    }
    return false;
}

static inline Bool Char_IsHex(Char c){
    switch(c){
    case CASE_CHAR_DIGIT: case CASE_CHAR_HEX_LETTER:
        return true;
    }
    return false;
}

static inline U8 Char_HexValue(Char c){
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


