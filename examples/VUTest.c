#include <stdio.h>
#include "VU15.h"
#include "VU30.h"
#include "VU61.h"

int main(int argc, char *argv[]){
  Byte szVU[8];
  Byte szData[2] = {0xFF, 0xFF};

  VU15_Sum(szVU, szData, szData + 2);
  U16 r16;
  VU15_ToU16(szVU, &r16);
  if(r16 != 0x1FE){
    fprintf(stderr, "%X\n", r16);
    return 1;
  }

  VU30_Sum(szVU, szData, szData + 2);
  U32 r32;
  VU30_ToU32(szVU, &r32);
  if(r32 != 0x1FE){
    fprintf(stderr, "%X\n", r32);
    return 1;
  }

  VU61_Sum(szVU, szData, szData + 2);
  U64 r64;
  VU61_ToU64(szVU, &r64);
  if(r64 != 0x1FE){
    fprintf(stderr, "%llX\n", r64);
    return 1;
  }

  U16 u16;
  for(u16 = 0; u16 < 0x80; u16 = (u16 << 1) + 1){
    //    fprintf(stderr, "%X\n", u16);

    VU15_FromU7(szVU, u16);
    VU15_ToU16(szVU, &r16);
    if(u16 != r16){
      fprintf(stderr, "%X != %X\n", u16, r16);
      return 1;
    }
  }

  for(u16 = 0; u16 < 0x100; u16 = (u16 << 1) + 1){
    //    fprintf(stderr, "%X\n", u16);

    VU15_FromU8(szVU, u16);
    VU15_ToU16(szVU, &r16);
    if(u16 != r16){
      fprintf(stderr, "%X != %X\n", u16, r16);
      return 1;
    }
  }

  for(u16 = 0; u16 < 0x8000; u16 = (u16 << 1) + 1){
    //    fprintf(stderr, "%X\n", u16);

    VU15_FromU15(szVU, u16);
    VU15_ToU16(szVU, &r16);
    if(u16 != r16){
      fprintf(stderr, "%X != %X\n", u16, r16);
      return 1;
    }
  }

  U32 u32;
  for(u32 = 0; u32 < 0x40ULL; u32 = (u32 << 1) + 1){
    //    fprintf(stderr, "%X\n", u32);

    VU30_FromU6(szVU, u32);
    VU30_ToU32(szVU, &r32);
    if(u32 != r32){
      fprintf(stderr, "%X != %X\n", u32, r32);
      return 1;
    }
  }

  for(u32 = 0; u32 < 0x100ULL; u32 = (u32 << 1) + 1){
    //    fprintf(stderr, "%X\n", u32);

    VU30_FromU8(szVU, u32);
    VU30_ToU32(szVU, &r32);
    if(u32 != r32){
      fprintf(stderr, "%X != %X\n", u32, r32);
      return 1;
    }
  }

  for(u32 = 0; u32 < 0x10000ULL; u32 = (u32 << 1) + 1){
    //    fprintf(stderr, "%X\n", u32);

    VU30_FromU16(szVU, u32);
    VU30_ToU32(szVU, &r32);
    if(u32 != r32){
      fprintf(stderr, "%X != %X\n", u32, r32);
      return 1;
    }
  }

  for(u32 = 0; u32 < 0x40000000; u32 = (u32 << 1) + 1){
    //    fprintf(stderr, "%X\n", u32);

    VU30_FromU30(szVU, u32);
    VU30_ToU32(szVU, &r32);
    if(u32 != r32){
      fprintf(stderr, "%X != %X\n", u32, r32);
      return 1;
    }
  }

  U64 u64;
  for(u64 = 0; u64 < 0x20ULL; u64 = (u64 << 1) + 1){
    //    fprintf(stderr, "%llX\n", u64);

    VU61_FromU5(szVU, u64);
    VU61_ToU64(szVU, &r64);
    if(u64 != r64){
      fprintf(stderr, "%llX != %llX\n", u64, r64);
      return 1;
    }
  }

  for(u64 = 0; u64 < 0x100ULL; u64 = (u64 << 1) + 1){
    //    fprintf(stderr, "%llX\n", u64);

    VU61_FromU8(szVU, u64);
    VU61_ToU64(szVU, &r64);
    if(u64 != r64){
      fprintf(stderr, "%llX != %llX\n", u64, r64);
      return 1;
    }
  }

  for(u64 = 0; u64 < 0x10000ULL; u64 = (u64 << 1) + 1){
    //    fprintf(stderr, "%llX\n", u64);

    VU61_FromU16(szVU, u64);
    VU61_ToU64(szVU, &r64);
    if(u64 != r64){
      fprintf(stderr, "%llX != %llX\n", u64, r64);
      return 1;
    }
  }

  for(u64 = 0; u64 < 0x100000000ULL; u64 = (u64 << 1) + 1){
    //    fprintf(stderr, "%llX\n", u64);

    VU61_FromU32(szVU, u64);
    VU61_ToU64(szVU, &r64);
    if(u64 != r64){
      fprintf(stderr, "%llX != %llX\n", u64, r64);
      return 1;
    }
  }

  for(u64 = 0; u64 < 0x2000000000000000ULL; u64 = (u64 << 1) + 1){
    //    fprintf(stderr, "%llX\n", u64);

    VU61_FromU61(szVU, u64);
    VU61_ToU64(szVU, &r64);
    if(u64 != r64){
      fprintf(stderr, "%llX != %llX\n", u64, r64);
      return 1;
    }
  }
  return 0;
}
