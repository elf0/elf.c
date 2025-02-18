// szChars: "0123456789ABCDEF" or "0123456789abcdef"

// pTo should have 2 bytes space
inline static C *U8ToS16(C *pTo, U8 u8, const C szChars[16]) {
    C szBuffer[2];
    C *pEnd = szBuffer + 2;
    C *p = pEnd;
    *--p = szChars[u8 & 0xF];
    if (u8 >>= 4)
        *--p = szChars[u8];

    *pTo++ = *p++;
    if (p != pEnd)
        *pTo++ = *p;

    return pTo;
}

// pTo should have 4 bytes space
inline static C *U16ToS16(C *pTo, U16 u16, const C szChars[16]) {
    C szBuffer[4];
    C *pEnd = szBuffer + 4;
    C *p = pEnd;
    do
        *--p = szChars[(U8)u16 & 0xF];
    while (u16 >>= 4);
    do
        *pTo++ = *p++;
    while (p != pEnd);
    return pTo;
}

// pTo should have 8 bytes space
inline static C *U32ToS16(C *pTo, U32 u32, const C szChars[16]) {
    C szBuffer[8];
    C *pEnd = szBuffer + 8;
    C *p = pEnd;
    do
        *--p = szChars[(U8)u32 & 0xF];
    while (u32 >>= 4);
    do
        *pTo++ = *p++;
    while (p != pEnd);
    return pTo;
}

// pTo should have 16 bytes space
inline static C *U64ToS16(C *pTo, U64 u64, const C szChars[16]) {
    C szBuffer[16];
    C *pEnd = szBuffer + 16;
    C *p = pEnd;
    do
        *--p = szChars[(U8)u64 & 0xF];
    while (u64 >>= 4);
    do
        *pTo++ = *p++;
    while (p != pEnd);
    return pTo;
}
