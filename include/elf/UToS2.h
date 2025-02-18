// pTo should have 8 bytes space
inline static C *U8ToS2(C *pTo, U8 u8) {
    C szBuffer[8];
    C *pEnd = szBuffer + 8;
    C *p = pEnd;
    do
        *--p = u8 & 1 | '0';
    while (u8 >>= 1);
    do
        *pTo++ = *p++;
    while (p != pEnd);
    return pTo;
}

// pTo should have 16 bytes space
inline static C *U16ToS2(C *pTo, U16 u16) {
    C szBuffer[16];
    C *pEnd = szBuffer + 16;
    C *p = pEnd;
    do
        *--p = (U8)u16 & 1 | '0';
    while (u16 >>= 1);
    do
        *pTo++ = *p++;
    while (p != pEnd);
    return pTo;
}

// pTo should have 32 bytes space
inline static C *U32ToS2(C *pTo, U32 u32) {
    C szBuffer[32];
    C *pEnd = szBuffer + 32;
    C *p = pEnd;
    do
        *--p = (U8)u32 & 1 | '0';
    while (u32 >>= 1);
    do
        *pTo++ = *p++;
    while (p != pEnd);
    return pTo;
}

// pTo should have 64 bytes space
inline static C *U64ToS2(C *pTo, U64 u64) {
    C szBuffer[64];
    C *pEnd = szBuffer + 64;
    C *p = pEnd;
    do
        *--p = (U8)u64 & 1 | '0';
    while (u64 >>= 1);
    do
        *pTo++ = *p++;
    while (p != pEnd);
    return pTo;
}
