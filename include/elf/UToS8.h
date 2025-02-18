// pTo should have 3 bytes space
inline static C *U8ToS8(C *pTo, U8 u8) {
    C szBuffer[3];
    C *pEnd = szBuffer + 3;
    C *p = pEnd;
    do
        *--p = u8 & 7 | '0';
    while (u8 >>= 3);
    do
        *pTo++ = *p++;
    while (p != pEnd);
    return pTo;
}

// pTo should have 6 bytes space
inline static C *U16ToS8(C *pTo, U16 u16) {
    C szBuffer[6];
    C *pEnd = szBuffer + 6;
    C *p = pEnd;
    do
        *--p = (U8)u16 & 7 | '0';
    while (u16 >>= 3);
    do
        *pTo++ = *p++;
    while (p != pEnd);
    return pTo;
}

// pTo should have 11 bytes space
inline static C *U32ToS8(C *pTo, U32 u32) {
    C szBuffer[11];
    C *pEnd = szBuffer + 11;
    C *p = pEnd;
    do
        *--p = (U8)u32 & 7 | '0';
    while (u32 >>= 3);
    do
        *pTo++ = *p++;
    while (p != pEnd);
    return pTo;
}

// pTo should have 22 bytes space
inline static C *U64ToS8(C *pTo, U64 u64) {
    C szBuffer[22];
    C *pEnd = szBuffer + 22;
    C *p = pEnd;
    do
        *--p = (U8)u64 & 7 | '0';
    while (u64 >>= 3);
    do
        *pTo++ = *p++;
    while (p != pEnd);
    return pTo;
}
