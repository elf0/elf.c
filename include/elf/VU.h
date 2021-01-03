#ifndef VU_H
#define VU_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

inline
static U8 VU_U32NeedBytes(U32 u32) {
  if (u32 < 0x80)
    return 1;
  if (u32 < 0x4000)
    return 2;
  if (u32 < 0x200000)
    return 3;
  return u32 < 0x10000000? 4 : 5;
}

inline
static U8 VU_U63NeedBytes(U64 u63) {
  if (u63 < 0x80ULL)
    return 1;
  if (u63 < 0x4000ULL)
    return 2;
  if (u63 < 0x200000ULL)
    return 3;
  if (u63 < 0x10000000ULL)
    return 4;
  if (u63 < 0x800000000ULL)
    return 5;
  if (u63 < 0x40000000000ULL)
    return 6;
  if (u63 < 0x2000000000000ULL)
    return 7;
  return u63 < 0x100000000000000ULL? 8 : 9;
}

inline
static U16 VU_Read14(const U8 **ppVU) {
  const U8 *pVU = *ppVU;
  U16 u14 = *pVU++;
  if (u14 > 0x7F) {
    u14 &= 0x7F;
    u14 <<= 7;
    u14 |= *pVU++;
  }
  *ppVU = pVU;
  return u14;
}

inline
static U32 VU_Read21(const U8 **ppVU) {
  const U8 *pVU = *ppVU;
  U32 u21 = *pVU++;
  if (u21 > 0x7F) {
    u21 &= 0x7F;
    u21 <<= 7;
    U8 u7 = *pVU++;
    if (u7 > 0x7F) {
      u7 &= 0x7F;
      u21 |= u7;
      u21 <<= 7;
      u7 = *pVU++;
    }
    u21 |= u7;
  }
  *ppVU = pVU;
  return u21;
}

inline
static U32 VU_Read28(const U8 **ppVU) {
  const U8 *pVU = *ppVU;
  U32 u28 = *pVU++;
  if (u28 > 0x7F) {
    u28 &= 0x7F;
NEXT:
    u28 <<= 7;
    U8 u7 = *pVU++;
    if (u7 > 0x7F) {
      u28 |= u7 & 0x7F;
      goto NEXT;
    }
    else
      u28 |= u7;
  }
  *ppVU = pVU;
  return u28;
}

inline
static U64 VU_Read63(const U8 **ppVU) {
  const U8 *pVU = *ppVU;
  U64 u63 = *pVU++;
  if (u63 > 0x7F) {
    u63 &= 0x7F;
NEXT:
    u63 <<= 7;
    U8 u7 = *pVU++;
    if (u7 > 0x7F) {
      u63 |= u7 & 0x7F;
      goto NEXT;
    }
    else
      u63 |= u7;
  }
  *ppVU = pVU;
  return u63;
}

inline
static U8 *VU_FromU14(U8 *pVU, U16 u14) {
  if (u14 > 0x7F)
    *pVU++ = (u14 >> 7) | 0x80;
  *pVU++ = u14 & 0x7F;
  return pVU;
}

inline
static U8 *VU_FromU21(U8 *pVU, U32 u21) {
  if (u21 > 0x7F) {
    if (u21 > 0x3FFF)
      *pVU++ = (u21 >> 14) | 0x80;
    *pVU++ = (u21 >> 7) | 0x80;
  }
  *pVU++ = u21 & 0x7F;
  return pVU;
}

inline
static U8 *VU_FromU28(U8 *pVU, U32 u28) {
  if (u28 > 0x7F) {
    if (u28 > 0x3FFF) {
      if (u28 > 0x1FFFFF)
        *pVU++ = (u28 >> 21) | 0x80;
      *pVU++ = (u28 >> 14) | 0x80;
    }
    *pVU++ = (u28 >> 7) | 0x80;
  }
  *pVU++ = u28 & 0x7F;
  return pVU;
}

inline
static U8 *VU_FromU35(U8 *pVU, U64 u35) {
  if (u35 > 0x7FULL) {
    if (u35 > 0x3FFFULL) {
      if (u35 > 0x1FFFFFULL) {
        if (u35 > 0xFFFFFFFULL)
          *pVU++ = (u35 >> 28) | 0x80;
        *pVU++ = (u35 >> 21) | 0x80;
      }
      *pVU++ = (u35 >> 14) | 0x80;
    }
    *pVU++ = (u35 >> 7) | 0x80;
  }
  *pVU++ = u35 & 0x7F;
  return pVU;
}

inline
static U8 *VU_FromU42(U8 *pVU, U64 u42) {
  if (u42 > 0x7FULL) {
    if (u42 > 0x3FFFULL) {
      if (u42 > 0x1FFFFFULL) {
        if (u42 > 0xFFFFFFFULL) {
          if (u42 > 0x7FFFFFFFFULL)
            *pVU++ = (u42 >> 35) | 0x80;
          *pVU++ = (u42 >> 28) | 0x80;
        }
        *pVU++ = (u42 >> 21) | 0x80;
      }
      *pVU++ = (u42 >> 14) | 0x80;
    }
    *pVU++ = (u42 >> 7) | 0x80;
  }
  *pVU++ = u42 & 0x7F;
  return pVU;
}

inline
static U8 *VU_FromU49(U8 *pVU, U64 u49) {
  if (u49 > 0x7FULL) {
    if (u49 > 0x3FFFULL) {
      if (u49 > 0x1FFFFFULL) {
        if (u49 > 0xFFFFFFFULL) {
          if (u49 > 0x7FFFFFFFFULL) {
            if (u49 > 0x3FFFFFFFFFFULL)
              *pVU++ = (u49 >> 42) | 0x80;
            *pVU++ = (u49 >> 35) | 0x80;
          }
          *pVU++ = (u49 >> 28) | 0x80;
        }
        *pVU++ = (u49 >> 21) | 0x80;
      }
      *pVU++ = (u49 >> 14) | 0x80;
    }
    *pVU++ = (u49 >> 7) | 0x80;
  }
  *pVU++ = u49 & 0x7F;
  return pVU;
}

inline
static U8 *VU_FromU56(U8 *pVU, U64 u56) {
  if (u56 > 0x7FULL) {
    if (u56 > 0x3FFFULL) {
      if (u56 > 0x1FFFFFULL) {
        if (u56 > 0xFFFFFFFULL) {
          if (u56 > 0x7FFFFFFFFULL) {
            if (u56 > 0x3FFFFFFFFFFULL) {
              if (u56 > 0x1FFFFFFFFFFFFULL)
                *pVU++ = (u56 >> 49) | 0x80;
              *pVU++ = (u56 >> 42) | 0x80;
            }
            *pVU++ = (u56 >> 35) | 0x80;
          }
          *pVU++ = (u56 >> 28) | 0x80;
        }
        *pVU++ = (u56 >> 21) | 0x80;
      }
      *pVU++ = (u56 >> 14) | 0x80;
    }
    *pVU++ = (u56 >> 7) | 0x80;
  }
  *pVU++ = u56 & 0x7F;
  return pVU;
}

inline
static U8 *VU_FromU63(U8 *pVU, U64 u63) {
  if (u63 > 0x7FULL) {
    if (u63 > 0x3FFFULL) {
      if (u63 > 0x1FFFFFULL) {
        if (u63 > 0xFFFFFFFULL) {
          if (u63 > 0x7FFFFFFFFULL) {
            if (u63 > 0x3FFFFFFFFFFULL) {
              if (u63 > 0x1FFFFFFFFFFFFULL) {
                if (u63 > 0xFFFFFFFFFFFFFFULL)
                  *pVU++ = (u63 >> 56) | 0x80;
                *pVU++ = (u63 >> 49) | 0x80;
              }
              *pVU++ = (u63 >> 42) | 0x80;
            }
            *pVU++ = (u63 >> 35) | 0x80;
          }
          *pVU++ = (u63 >> 28) | 0x80;
        }
        *pVU++ = (u63 >> 21) | 0x80;
      }
      *pVU++ = (u63 >> 14) | 0x80;
    }
    *pVU++ = (u63 >> 7) | 0x80;
  }
  *pVU++ = u63 & 0x7F;
  return pVU;
}

#endif //VU_H
