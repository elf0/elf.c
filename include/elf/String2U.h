#ifndef STRING2U_H
#define STRING2U_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

//String to unsigned integer

//From 'pBegin' to 'pEnd' MUST be a valid number string.
static inline U8 String_ToU8(const C *pBegin, const C *pEnd);
static inline U16 String_ToU16(const C *pBegin, const C *pEnd);
static inline U32 String_ToU32(const C *pBegin, const C *pEnd);
static inline U64 String_ToU64(const C *pBegin, const C *pEnd);

static inline E8 String_ParseU8(const C **ppszNumber, U8 *puValue);
static inline E8 String_ParseU16(const C **ppszNumber, U16 *puValue);
static inline E8 String_ParseU32(const C **ppszNumber, U32 *puValue);
static inline E8 String_ParseU64(const C **ppszNumber, U64 *puValue);

//Parse '0b' prefix youself
static inline E8 String_ParseBinaryU32(const C **ppszNumber, U32 *puValue);
static inline E8 String_ParseBinaryU64(const C **ppszNumber, U64 *puValue);

//Parse '0o' prefix youself
static inline E8 String_ParseOctalU32(const C **ppszNumber, U32 *puValue);
static inline E8 String_ParseOctalU64(const C **ppszNumber, U64 *puValue);

//Parse '0x' prefix youself
static inline E8 String_ParseHexU32(const C **ppszNumber, U32 *puValue);
static inline E8 String_ParseHexU64(const C **ppszNumber, U64 *puValue);
static inline E8 String_ParseUHexU64(const C **ppszNumber, U64 *puValue);

static inline E8 String_ParseIp(const C **ppIp, U32 *puIp);
////////////////////////////////////////////////////////////////
static inline U8 String_ToU8(const C *pBegin, const C *pEnd){
    const C *p = pBegin;
    U8 uValue = *p++ - '0';

    while(p != pEnd)
        uValue = 10 * uValue + (*p++ - '0');

    return uValue;
}

static inline U16 String_ToU16(const C *pBegin, const C *pEnd){
    const C *p = pBegin;
    U16 uValue = *p++ - '0';

    while(p != pEnd)
        uValue = 10 * uValue + (*p++ - '0');

    return uValue;
}

static inline U32 String_ToU32(const C *pBegin, const C *pEnd){
    const C *p = pBegin;
    U32 uValue = *p++ - '0';

    while(p != pEnd)
        uValue = 10 * uValue + (*p++ - '0');

    return uValue;
}

static inline U64 String_ToU64(const C *pBegin, const C *pEnd){
    const C *p = pBegin;
    U64 uValue = *p++ - '0';

    while(p != pEnd)
        uValue = 10 * uValue + (*p++ - '0');


    return uValue;
}

static inline E8 String_ParseU8(const C **ppszNumber, U8 *puValue){
    const C *p = *ppszNumber;
    U8 uValue = *puValue;
    //Max: 255
#define U8_OVERFLOW_BEFORE_MUL 25U
#define U8_OVERFLOW_BEFORE_ADD 250U

    while(1){
        U8 uRange = *p - (C)'0';
        if(uRange > 9)
            break;


        if(uValue < U8_OVERFLOW_BEFORE_MUL)
            uValue = uValue * 10 + uRange;
        else if(uValue > U8_OVERFLOW_BEFORE_MUL || uRange > 5){
            *ppszNumber = p;
            *puValue = uValue;
            return 1;
        }else
            uValue = U8_OVERFLOW_BEFORE_ADD + uRange;

        ++p;
    }

    *ppszNumber = p;
    *puValue = uValue;
    return 0;
}

static inline E8 String_ParseU16(const C **ppszNumber, U16 *puValue){
    const C *p = *ppszNumber;
    U16 uValue = *puValue;
    //Max: 65535
#define U16_OVERFLOW_BEFORE_MUL 6553U
#define U16_OVERFLOW_BEFORE_ADD 65530U

    while(1){
        U8 uRange = *p - (C)'0';
        if(uRange > 9)
            break;


        if(uValue < U16_OVERFLOW_BEFORE_MUL)
            uValue = uValue * 10 + uRange;
        else if(uValue > U16_OVERFLOW_BEFORE_MUL || uRange > 5){
            *ppszNumber = p;
            *puValue = uValue;
            return 1;
        }else
            uValue = U16_OVERFLOW_BEFORE_ADD + uRange;

        ++p;
    }

    *ppszNumber = p;
    *puValue = uValue;
    return 0;
}

static inline E8 String_ParseU32(const C **ppszNumber, U32 *puValue){
    const C *p = *ppszNumber;
    U32 uValue = *puValue;
    //Max: 4294967295
#define U32_OVERFLOW_BEFORE_MUL 429496729U
#define U32_OVERFLOW_BEFORE_ADD 4294967290U

    while(1){
        U8 uRange = *p - (C)'0';
        if(uRange > 9)
            break;


        if(uValue < U32_OVERFLOW_BEFORE_MUL)
            uValue = uValue * 10 + uRange;
        else if(uValue > U32_OVERFLOW_BEFORE_MUL || uRange > 5){
            *ppszNumber = p;
            *puValue = uValue;
            return 1;
        }else
            uValue = U32_OVERFLOW_BEFORE_ADD + uRange;

        ++p;
    }

    *ppszNumber = p;
    *puValue = uValue;
    return 0;
}

static inline E8 String_ParseU64(const C **ppszNumber, U64 *puValue){
    const C *p = *ppszNumber;
    U64 uValue = *puValue;

    //Max: 18446744073709551615
#define U64_OVERFLOW_BEFORE_MUL 1844674407370955161ULL
#define U64_OVERFLOW_BEFORE_ADD 18446744073709551610ULL

    while(1){
        U8 uRange = *p - (C)'0';
        if(uRange > 9)
            break;

        if(uValue < U64_OVERFLOW_BEFORE_MUL)
            uValue = uValue * 10 + uRange;
        else if(uValue > U64_OVERFLOW_BEFORE_MUL || uRange > 5){
            *ppszNumber = p;
            *puValue = uValue;
            return 1;
        }else
            uValue = U64_OVERFLOW_BEFORE_ADD + uRange;

        ++p;
    }

    *ppszNumber = p;
    *puValue = uValue;
    return 0;
}

static inline E8 String_ParseBinaryU32(const C **ppszNumber, U32 *puValue){
    const C *p = *ppszNumber;
    U32 uValue = *puValue;
    //Max: 0b11111111111111111111111111111111
#define BINARY_U32_OVERFLOW_BEFORE_MUL 0x7FFFFFFFU

    while(1){
        U8 uRange = *p - (C)'0';
        if(uRange > 1)
            break;

        if(uValue > BINARY_U32_OVERFLOW_BEFORE_MUL){
            *ppszNumber = p;
            *puValue = uValue;
            return 1;
        }

        uValue = (uValue << 1) + uRange;

        ++p;
    }

    *ppszNumber = p;
    *puValue = uValue;
    return 0;
}

static inline E8 String_ParseBinaryU64(const C **ppszNumber, U64 *puValue){
    const C *p = *ppszNumber;
    U64 uValue = *puValue;
    //Max: 0b1111111111111111111111111111111111111111111111111111111111111111
#define BINARY_U64_OVERFLOW_BEFORE_MUL 0x7FFFFFFFFFFFFFFFULL

    while(1){
        U8 uRange = *p - (C)'0';
        if(uRange > 1)
            break;

        if(uValue > BINARY_U64_OVERFLOW_BEFORE_MUL){
            *ppszNumber = p;
            *puValue = uValue;
            return 1;
        }

        uValue = (uValue << 1) + uRange;

        ++p;
    }

    *ppszNumber = p;
    *puValue = uValue;
    return 0;
}

static inline E8 String_ParseOctalU32(const C **ppszNumber, U32 *puValue){
    const C *p = *ppszNumber;
    U32 uValue = *puValue;
    //Max: 0o37777777777
#define OCTAL_U32_OVERFLOW_BEFORE_MUL 03777777777U

    while(1){
        U8 uRange = *p - (C)'0';
        if(uRange > 7)
            break;

        if(uValue > OCTAL_U32_OVERFLOW_BEFORE_MUL){
            *ppszNumber = p;
            *puValue = uValue;
            return 1;
        }

        uValue = (uValue << 3) + uRange;

        ++p;
    }

    *ppszNumber = p;
    *puValue = uValue;
    return 0;
}

static inline E8 String_ParseOctalU64(const C **ppszNumber, U64 *puValue){
    const C *p = *ppszNumber;
    U64 uValue = *puValue;
    //Max: 0o1777777777777777777777
#define OCTAL_U64_OVERFLOW_BEFORE_MUL 0177777777777777777777ULL

    while(1){
        U8 uRange = *p - (C)'0';
        if(uRange > 7)
            break;

        if(uValue > OCTAL_U64_OVERFLOW_BEFORE_MUL){
            *ppszNumber = p;
            *puValue = uValue;
            return 1;
        }

        uValue = (uValue << 3) + uRange;

        ++p;
    }

    *ppszNumber = p;
    *puValue = uValue;
    return 0;
}

//Max: 0xFFFFFFFF
#define HEX_U32_OVERFLOW_BEFORE_MUL 0xFFFFFFFU

static inline E8 String_ParseHexU32(const C **ppszNumber, U32 *puValue){
    const C *p = *ppszNumber;
    U32 uValue = *puValue;
    B bOverflow = 0;
    U8 uRange;
    while(1){
      uRange = *p - (C)'0';
      if(uRange < 10){
        if(uValue > HEX_U32_OVERFLOW_BEFORE_MUL){
          bOverflow = 1;
          break;
        }
        else
          uValue = uValue << 4 | uRange;
      }
      else{
        uRange -= 17;
        if(uRange < 6){
          if(uValue > HEX_U32_OVERFLOW_BEFORE_MUL){
            bOverflow = 1;
            break;
          }
          else
            uValue = uValue << 4 | (uRange + 10);
        }
        else{
          uRange -= 32;
          if(uRange > 5)
            break;

          if(uValue > HEX_U32_OVERFLOW_BEFORE_MUL){
            bOverflow = 1;
            break;
          }
          else
            uValue = uValue << 4 | (uRange + 10);
        }
      }

      ++p;
    }

    *ppszNumber = p;
    *puValue = uValue;
    return bOverflow;
}

//Max: 0xFFFFFFFFFFFFFFFF
#define HEX_U64_OVERFLOW_BEFORE_MUL 0xFFFFFFFFFFFFFFFULL

static inline E8 String_ParseHexU64(const C **ppszNumber, U64 *puValue){
    const C *p = *ppszNumber;
    U64 uValue = *puValue;
    B bOverflow = 0;
    U8 uRange;
    while(1){
      uRange = *p - (C)'0';
      if(uRange < 10){
        if(uValue > HEX_U64_OVERFLOW_BEFORE_MUL){
          bOverflow = 1;
          break;
        }
        else
          uValue = uValue << 4 | uRange;
      }
      else{
        uRange -= 17;
        if(uRange < 6){
          if(uValue > HEX_U64_OVERFLOW_BEFORE_MUL){
            bOverflow = 1;
            break;
          }
          else
            uValue = uValue << 4 | (uRange + 10);
        }
        else{
          uRange -= 32;
          if(uRange > 5)
            break;

          if(uValue > HEX_U64_OVERFLOW_BEFORE_MUL){
            bOverflow = 1;
            break;
          }
          else
            uValue = uValue << 4 | (uRange + 10);
        }
      }

      ++p;
    }

    *ppszNumber = p;
    *puValue = uValue;
    return bOverflow;
}

static inline E8 String_ParseUHexU64(const C **ppszNumber, U64 *puValue){
    const C *p = *ppszNumber;
    U64 uValue = *puValue;
    B bOverflow = 0;
    U8 uRange;
    while(1){
      uRange = *p - (C)'0';
      if(uRange < 10){
        if(uValue > HEX_U64_OVERFLOW_BEFORE_MUL){
          bOverflow = 1;
          break;
        }
        else
          uValue = uValue << 4 | uRange;
      }
      else{
        uRange -= 17;
        if(uRange > 5)
          break;

        if(uValue > HEX_U64_OVERFLOW_BEFORE_MUL){
          bOverflow = 1;
          break;
        }
        else
          uValue = uValue << 4 | (uRange + 10);
      }

      ++p;
    }

    *ppszNumber = p;
    *puValue = uValue;
    return bOverflow;
}

static inline E8 String_ParseIp(const C **ppIp, U32 *puIp){
    const C *p = *ppIp;

    if(C_IsNotDigit(*p))
        return 1;

    union{
        U32 uIp;
        U8 szIp[4];
    }ip;

    ip.uIp = 0;

    if(String_ParseU8(&p, &ip.szIp[0])){
        *ppIp = p;
        return 1;
    }

    if(*p != '.'){
        *ppIp = p;
        return 1;
    }
    ++p;

    if(C_IsNotDigit(*p)){
        *ppIp = p;
        return 1;
    }

    if(String_ParseU8(&p, &ip.szIp[1])){
        *ppIp = p;
        return 1;
    }

    if(*p != '.'){
        *ppIp = p;
        return 1;
    }
    ++p;

    if(C_IsNotDigit(*p)){
        *ppIp = p;
        return 1;
    }

    if(String_ParseU8(&p, &ip.szIp[2])){
        *ppIp = p;
        return 1;
    }

    if(*p != '.'){
        *ppIp = p;
        return 1;
    }
    ++p;

    if(C_IsNotDigit(*p)){
        *ppIp = p;
        return 1;
    }

    if(String_ParseU8(&p, &ip.szIp[3])){
        *ppIp = p;
        return 1;
    }

    *ppIp = p;
    *puIp = ip.uIp;
    return 0;
}

#endif // STRING2U_H
