// pTo should have 3 bytes space
inline static C *U8ToS10(C *pTo, U8 u8) {
    C szBuffer[3];
    C *pEnd = szBuffer + 3;
    C *p = pEnd;
    do
        *--p = u8 % 10 | '0';
    while (u8 /= 10);
    do
        *pTo++ = *p++;
    while (p != pEnd);
    return pTo;
}

// pTo should have 5 bytes space
inline static C *U16ToS10(C *pTo, U16 u16) {
    C szBuffer[5];
    C *pEnd = szBuffer + 5;
    C *p = pEnd;
    do
        *--p = (U8)(u16 % 10) | '0';
    while (u16 /= 10);
    do
        *pTo++ = *p++;
    while (p != pEnd);
    return pTo;
}

// pTo should have 10 bytes space
inline static C *U32ToS10(C *pTo, U32 u32) {
    C szBuffer[10];
    C *pEnd = szBuffer + 10;
    C *p = pEnd;
    do
        *--p = (U8)(u32 % 10) | '0';
    while (u32 /= 10);
    do
        *pTo++ = *p++;
    while (p != pEnd);
    return pTo;
}

// pTo should have 20 bytes space
inline static C *U64ToS10(C *pTo, U64 u64) {
    C szBuffer[20];
    C *pEnd = szBuffer + 20;
    C *p = pEnd;
    do
        *--p = (U8)(u64 % 10) | '0';
    while (u64 /= 10);
    do
        *pTo++ = *p++;
    while (p != pEnd);
    return pTo;
}
