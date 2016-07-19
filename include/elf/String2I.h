#ifndef STRING2I_H
#define STRING2I_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

//String to integer

#define I32_MIN 0x80000000
#define I64_MIN 0x8000000000000000LL

//Parse '+' youself.
static inline B String_ParseI32_Positive(const C **ppszNumber, I32 *piValue);
static inline B String_ParseI64_Positive(const C **ppszNumber, I64 *piValue);

//Parse '-*' youself
static inline B String_ParseI32_Negative(const C **ppszNumber, I32 *piValue);
static inline B String_ParseI64_Negative(const C **ppszNumber, I64 *piValue);


//Parse '0b', '+0b', '-0b*' prefix youself
static inline B String_ParseBinaryI32_Positive(const C **ppszNumber, I32 *piValue);
static inline B String_ParseBinaryI32_Negative(const C **ppszNumber, I32 *piValue);
static inline B String_ParseBinaryI64_Positive(const C **ppszNumber, I64 *piValue);
static inline B String_ParseBinaryI64_Negative(const C **ppszNumber, I64 *piValue);

//Parse '0o', '+0o', '-0o*' prefix youself
static inline B String_ParseOctalI32_Positive(const C **ppszNumber, I32 *piValue);
static inline B String_ParseOctalI32_Negative(const C **ppszNumber, I32 *piValue);
static inline B String_ParseOctalI64_Positive(const C **ppszNumber, I64 *piValue);
static inline B String_ParseOctalI64_Negative(const C **ppszNumber, I64 *piValue);

//Parse '0x', '+0x', '-0x' prefix youself
static inline B String_ParseHexI32_Positive(const C **ppszNumber, I32 *piValue);
static inline B String_ParseHexI32_Negative(const C **ppszNumber, I32 *piValue);
static inline B String_ParseHexI64_Positive(const C **ppszNumber, I64 *piValue);
static inline B String_ParseHexI64_Negative(const C **ppszNumber, I64 *piValue);
////////////////////////////////////////////////////////////////
static inline B String_ParseI32_Positive(const C **ppszNumber, I32 *piValue){
    const C *p = *ppszNumber;
    I32 iValue = *piValue;
    //Max: 2147483647
#define I32_POSITIVE_OVERFLOW_BEFORE_MUL 214748364
#define I32_POSITIVE_OVERFLOW_BEFORE_ADD 2147483640

    while(true){
        U8 uRange = *p - (C)'0';
        if(uRange > 9)
            break;
        ++p;


        if(iValue < I32_POSITIVE_OVERFLOW_BEFORE_MUL)
            iValue = iValue * 10 + uRange;
        else if(iValue > I32_POSITIVE_OVERFLOW_BEFORE_MUL || uRange > 7){
            *ppszNumber = --p;
            *piValue = iValue;
            return true;
        }else
            iValue = I32_POSITIVE_OVERFLOW_BEFORE_ADD + uRange;
    }

    *ppszNumber = p;
    *piValue = iValue;
    return false;
}

static inline B String_ParseI64_Positive(const C **ppszNumber, I64 *piValue){
    const C *p = *ppszNumber;
    I64 iValue = *piValue;
    //Max: 9223372036854775807
#define I64_POSITIVE_OVERFLOW_BEFORE_MUL 922337203685477580LL
#define I64_POSITIVE_OVERFLOW_BEFORE_ADD 9223372036854775800LL

    while(true){
        U8 uRange = *p - (C)'0';
        if(uRange > 9)
            break;
        ++p;


        if(iValue < I64_POSITIVE_OVERFLOW_BEFORE_MUL)
            iValue = iValue * 10 + uRange;
        else if(iValue > I64_POSITIVE_OVERFLOW_BEFORE_MUL || uRange > 7){
            *ppszNumber = --p;
            *piValue = iValue;
            return true;
        }else
            iValue = I64_POSITIVE_OVERFLOW_BEFORE_ADD + uRange;
    }

    *ppszNumber = p;
    *piValue = iValue;
    return false;
}

static inline B String_ParseI32_Negative(const C **ppszNumber, I32 *piValue){
    const C *p = *ppszNumber;
    I32 iValue = *piValue;
    //Min: -2147483648
#define I32_NEGATIVE_OVERFLOW_BEFORE_MUL -214748364
#define I32_NEGATIVE_OVERFLOW_BEFORE_ADD -2147483640

    while(true){
        U8 uRange = *p - (C)'0';
        if(uRange > 9)
            break;
        ++p;


        if(iValue > I32_NEGATIVE_OVERFLOW_BEFORE_MUL)
            iValue = iValue * 10 - uRange;
        else if(iValue < I32_NEGATIVE_OVERFLOW_BEFORE_MUL || uRange > 8){
            *ppszNumber = --p;
            *piValue = iValue;
            return true;
        }else
            iValue = I32_NEGATIVE_OVERFLOW_BEFORE_ADD - uRange;
    }

    *ppszNumber = p;
    *piValue = iValue;
    return false;
}

static inline B String_ParseI64_Negative(const C **ppszNumber, I64 *piValue){
    const C *p = *ppszNumber;
    I64 iValue = *piValue;
    //Min: -9223372036854775808
#define I64_NEGATIVE_OVERFLOW_BEFORE_MUL -922337203685477580LL
#define I64_NEGATIVE_OVERFLOW_BEFORE_ADD -9223372036854775800LL

    while(true){
        U8 uRange = *p - (C)'0';
        if(uRange > 9)
            break;
        ++p;


        if(iValue > I64_NEGATIVE_OVERFLOW_BEFORE_MUL)
            iValue = iValue * 10 - uRange;
        else if(iValue < I64_NEGATIVE_OVERFLOW_BEFORE_MUL || uRange > 8){
            *ppszNumber = --p;
            *piValue = iValue;
            return true;
        }else
            iValue = I64_NEGATIVE_OVERFLOW_BEFORE_ADD - uRange;
    }

    *ppszNumber = p;
    *piValue = iValue;
    return false;
}


static inline B String_ParseBinaryI32_Positive(const C **ppszNumber, I32 *piValue){
    const C *p = *ppszNumber;
    I32 iValue = *piValue;
    //Max: 0b1111111111111111111111111111111
#define BINARY_I32_OVERFLOW_BEFORE_MUL 0x3FFFFFFF

    while(true){
        U8 uRange = *p - (C)'0';
        if(uRange > 1)
            break;
        ++p;

        if(iValue > BINARY_I32_OVERFLOW_BEFORE_MUL){
            *ppszNumber = --p;
            *piValue = iValue;
            return true;
        }

        iValue = (iValue << 1) + uRange;
    }

    *ppszNumber = p;
    *piValue = iValue;
    return false;
}

static inline B String_ParseBinaryI32_Negative(const C **ppszNumber, I32 *piValue){
    const C *p = *ppszNumber;

    I32 iValue = *piValue;
    //Min: -0b10000000000000000000000000000000
#define BINARY_I32_NEGATIVE_OVERFLOW_BEFORE_MUL -0x40000000

    while(true){
        U8 uRange = *p - (C)'0';
        if(uRange > 1)
            break;
        ++p;


        if(iValue < BINARY_I32_NEGATIVE_OVERFLOW_BEFORE_MUL){
            *ppszNumber = --p;
            *piValue = iValue;
            return true;
        }

        iValue = (iValue << 1) - uRange;
    }

    *ppszNumber = p;
    *piValue = iValue;
    return false;
}

static inline B String_ParseBinaryI64_Positive(const C **ppszNumber, I64 *piValue){
    const C *p = *ppszNumber;
    I64 iValue = *piValue;
    //Max: 0b111111111111111111111111111111111111111111111111111111111111111
#define BINARY_I64_OVERFLOW_BEFORE_MUL 0x3FFFFFFFFFFFFFFFLL

    while(true){
        U8 uRange = *p - (C)'0';
        if(uRange > 1)
            break;
        ++p;

        if(iValue > BINARY_I64_OVERFLOW_BEFORE_MUL){
            *ppszNumber = --p;
            *piValue = iValue;
            return true;
        }

        iValue = (iValue << 1) + uRange;
    }

    *ppszNumber = p;
    *piValue = iValue;
    return false;
}

static inline B String_ParseBinaryI64_Negative(const C **ppszNumber, I64 *piValue){
    const C *p = *ppszNumber;
    I64 iValue = *piValue;
    //Min: -0b1000000000000000000000000000000000000000000000000000000000000000
#define BINARY_I64_NEGATIVE_OVERFLOW_BEFORE_MUL -0x4000000000000000

    while(true){
        U8 uRange = *p - (C)'0';
        if(uRange > 1)
            break;
        ++p;

        if(iValue > BINARY_I64_NEGATIVE_OVERFLOW_BEFORE_MUL)
            iValue = (iValue << 1) - uRange;
        else if(iValue < BINARY_I64_NEGATIVE_OVERFLOW_BEFORE_MUL || uRange != 0){
            *ppszNumber = --p;
            *piValue = iValue;
            return true;
        }
        else
            iValue = I64_MIN;
    }

    *ppszNumber = p;
    *piValue = iValue;
    return false;
}

static inline B String_ParseOctalI32_Positive(const C **ppszNumber, I32 *piValue){
    const C *p = *ppszNumber;
    U32 iValue = 0;
    //Max: 0o17777777777
#define OCTAL_I32_POSITIVE_OVERFLOW_BEFORE_MUL 0xFFFFFFF

    while(true){
        U8 uRange = *p - (C)'0';
        if(uRange > 7)
            break;
        ++p;

        if(iValue > OCTAL_I32_POSITIVE_OVERFLOW_BEFORE_MUL){
            *ppszNumber = --p;
            *piValue = iValue;
            return true;
        }

        iValue = (iValue << 3) + uRange;
    }

    *ppszNumber = p;
    *piValue = iValue;
    return false;

}

static inline B String_ParseOctalI32_Negative(const C **ppszNumber, I32 *piValue){
    const C *p = *ppszNumber;
    I32 iValue = 0;
    //Min: -0o20000000000
#define OCTAL_I32_NEGATIVE_OVERFLOW_BEFORE_MUL -0x10000000

    while(true){
        U8 uRange = *p - (C)'0';
        if(uRange > 7)
            break;
        ++p;

        if(iValue > OCTAL_I32_NEGATIVE_OVERFLOW_BEFORE_MUL)
            iValue = (iValue << 3) - uRange;
        else if(iValue < OCTAL_I32_NEGATIVE_OVERFLOW_BEFORE_MUL || uRange != 0){
            *ppszNumber = --p;
            *piValue = iValue;
            return true;
        }
        else
            iValue = I32_MIN;
    }

    *ppszNumber = p;
    *piValue = iValue;
    return false;
}

static inline B String_ParseOctalI64_Positive(const C **ppszNumber, I64 *piValue){
    const C *p = *ppszNumber;
    U64 iValue = 0;
    //Max: 0o777777777777777777777
#define OCTAL_I64_POSITIVE_OVERFLOW_BEFORE_MUL 0xFFFFFFFFFFFFFFFLL

    while(true){
        U8 uRange = *p - (C)'0';
        if(uRange > 7)
            break;
        ++p;

        if(iValue > OCTAL_I64_POSITIVE_OVERFLOW_BEFORE_MUL){
            *ppszNumber = --p;
            *piValue = iValue;
            return true;
        }

        iValue = (iValue << 3) + uRange;
    }

    *ppszNumber = p;
    *piValue = iValue;
    return false;
}

static inline B String_ParseOctalI64_Negative(const C **ppszNumber, I64 *piValue){
    const C *p = *ppszNumber;
    I64 iValue = 0;
    //Min: -0o1000000000000000000000
#define OCTAL_I64_NEGATIVE_OVERFLOW_BEFORE_MUL -0x1000000000000000LL

    while(true){
        U8 uRange = *p - (C)'0';
        if(uRange > 7)
            break;
        ++p;
        if(iValue > OCTAL_I64_NEGATIVE_OVERFLOW_BEFORE_MUL)
            iValue = (iValue << 3) - uRange;
        else if(iValue < OCTAL_I64_NEGATIVE_OVERFLOW_BEFORE_MUL || uRange != 0){
            *ppszNumber = --p;
            *piValue = iValue;
            return true;
        }
        else
            iValue = I64_MIN;
    }

    *ppszNumber = p;
    *piValue = iValue;
    return false;
}

static inline B String_ParseHexI32_Positive(const C **ppszNumber, I32 *piValue){
    const C *p = *ppszNumber;
    I32 iValue = 0;
    //Max: 0x7FFFFFFF
#define HEX_I32_POSITIVE_OVERFLOW_BEFORE_MUL 0x7FFFFFF

    while(true){
        switch(*p){
        default:
            *ppszNumber = p;
            *piValue = iValue;
            return false;
        case CASE_CHAR_DIGIT:
            if(iValue > HEX_I32_POSITIVE_OVERFLOW_BEFORE_MUL){
                *ppszNumber = p;
                *piValue = iValue;
                return true;
            }
            iValue = (iValue << 4) | (*p - '0');
            break;
        case CASE_CHAR_HEX_LETTER_UPPER:
            if(iValue > HEX_I32_POSITIVE_OVERFLOW_BEFORE_MUL){
                *ppszNumber = p;
                *piValue = iValue;
                return true;
            }
            iValue = (iValue << 4) | (10 + (*p - 'A'));
            break;
        case CASE_CHAR_HEX_LETTER_LOWER:
            if(iValue > HEX_I32_POSITIVE_OVERFLOW_BEFORE_MUL){
                *ppszNumber = p;
                *piValue = iValue;
                return true;
            }
            iValue = (iValue << 4) | (10 + (*p - 'a'));
            break;
        }
        ++p;
    }
}

static inline B String_ParseHexI32_Negative(const C **ppszNumber, I32 *piValue){
    const C *p = *ppszNumber;
    I32 iValue = 0;

#define HEX_I32_NEGATIVE_OVERFLOW_BEFORE_MUL -0x8000000

    while(true){
        switch(*p){
        default:
            *ppszNumber = p;
            *piValue = iValue;
            return false;
        case CASE_CHAR_DIGIT:
            if(iValue > HEX_I32_NEGATIVE_OVERFLOW_BEFORE_MUL)
                iValue = (iValue << 4) + ('0' - *p);
            else if(iValue < HEX_I32_NEGATIVE_OVERFLOW_BEFORE_MUL || *p != '0'){
                *ppszNumber = p;
                *piValue = iValue;
                return true;
            }
            else
                iValue = I32_MIN;
            break;
        case CASE_CHAR_HEX_LETTER_UPPER:
            if(iValue > HEX_I32_NEGATIVE_OVERFLOW_BEFORE_MUL)
                iValue = (iValue << 4) + (-10 + ('A' - *p));
            else{
                *ppszNumber = p;
                *piValue = iValue;
                return true;
            }
            break;
        case CASE_CHAR_HEX_LETTER_LOWER:
            if(iValue > HEX_I32_NEGATIVE_OVERFLOW_BEFORE_MUL)
                iValue = (iValue << 4) + (-10 + ('a' - *p));
            else{
                *ppszNumber = p;
                *piValue = iValue;
                return true;
            }
            break;
        }
        ++p;
    }
}

static inline B String_ParseHexI64_Positive(const C **ppszNumber, I64 *piValue){
    const C *p = *ppszNumber;
    I64 iValue = 0;
    //Max: 0x7FFFFFFFFFFFFFFF
#define HEX_I64_POSITIVE_OVERFLOW_BEFORE_MUL 0x7FFFFFFFFFFFFFFLL

    while(true){
        switch(*p){
        default:
            *ppszNumber = p;
            *piValue = iValue;
            return false;
        case CASE_CHAR_DIGIT:
            if(iValue > HEX_I64_POSITIVE_OVERFLOW_BEFORE_MUL){
                *ppszNumber = p;
                *piValue = iValue;
                return true;
            }
            iValue = (iValue << 4) | (*p - '0');
            break;
        case CASE_CHAR_HEX_LETTER_UPPER:
            if(iValue > HEX_I64_POSITIVE_OVERFLOW_BEFORE_MUL){
                *ppszNumber = p;
                *piValue = iValue;
                return true;
            }
            iValue = (iValue << 4) | (10 + (*p - 'A'));
            break;
        case CASE_CHAR_HEX_LETTER_LOWER:
            if(iValue > HEX_I64_POSITIVE_OVERFLOW_BEFORE_MUL){
                *ppszNumber = p;
                *piValue = iValue;
                return true;
            }
            iValue = (iValue << 4) | (10 + (*p - 'a'));
            break;
        }
        ++p;
    }
}

static inline B String_ParseHexI64_Negative(const C **ppszNumber, I64 *piValue){
    const C *p = *ppszNumber;
    I64 iValue = 0;

#define HEX_I64_NEGATIVE_OVERFLOW_BEFORE_MUL -0x800000000000000LL

    while(true){
        switch(*p){
        default:
            *ppszNumber = p;
            *piValue = iValue;
            return false;
        case CASE_CHAR_DIGIT:
            if(iValue > HEX_I64_NEGATIVE_OVERFLOW_BEFORE_MUL)
                iValue = (iValue << 4) + ('0' - *p);
            else if(iValue < HEX_I64_NEGATIVE_OVERFLOW_BEFORE_MUL || *p != '0'){
                *ppszNumber = p;
                *piValue = iValue;
                return true;
            }
            else
                iValue = I64_MIN;
            break;
        case CASE_CHAR_HEX_LETTER_UPPER:
            if(iValue > HEX_I64_NEGATIVE_OVERFLOW_BEFORE_MUL)
                iValue = (iValue << 4) + (-10 + ('A' - *p));
            else{
                *ppszNumber = p;
                *piValue = iValue;
                return true;
            }
            break;
        case CASE_CHAR_HEX_LETTER_LOWER:
            if(iValue > HEX_I64_NEGATIVE_OVERFLOW_BEFORE_MUL)
                iValue = (iValue << 4) + (-10 + ('a' - *p));
            else{
                *ppszNumber = p;
                *piValue = iValue;
                return true;
            }
            break;
        }
        ++p;
    }
}

#endif // STRING2I_H
