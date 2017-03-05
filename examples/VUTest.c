#include <stdio.h>
#include "VU15.h"
#include "VU30.h"
#include "VU61.h"

int main(int argc, char *argv[]){
  Byte szVU[8];
  Byte *pVU = szVU;

  U16 u16, r16;
  for(u16 = 0; u16 < 0x8000; u16 = (u16 << 1) + 1){
//    fprintf(stderr, "%X\n", u16);

    VU15_FromU15(pVU, u16);
    VU15_ToU16(pVU, &r16);
    if(u16 != r16){
      fprintf(stderr, "%X != %X\n", u16, r16);
      break;
    }
  }

  U32 u32, r32;
  for(u32 = 0; u32 < 0x40000000; u32 = (u32 << 1) + 1){
//    fprintf(stderr, "%X\n", u32);

    VU30_FromU30(pVU, u32);
    VU30_ToU32(pVU, &r32);
    if(u32 != r32){
      fprintf(stderr, "%X != %X\n", u32, r32);
      break;
    }
  }

  U64 u64, r64;
  for(u64 = 0; u64 < 0x2000000000000000ULL; u64 = (u64 << 1) + 1){
//    fprintf(stderr, "%llX\n", u64);

    VU61_FromU61(pVU, u64);
    VU61_ToU64(pVU, &r64);
    if(u64 != r64){
      fprintf(stderr, "%llX != %llX\n", u64, r64);
      break;
    }
  }
  return 0;
}
