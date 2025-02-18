// pEnd - pBegin should be 3
inline static C *U8ToS10(C *pBegin, C *pEnd, U8 u8) {
    C *p = pEnd;
    do
        *--p = u8 % 10 | '0';
    while (u8 /= 10);
    if (p != pBegin) {
        C *pD = pBegin;
        do
            *pD++ = *p++;
        while (p != pEnd);
        return pD;
    }
    return pEnd;
}

// pEnd - pBegin should be 5
inline static C *U16ToS10(C *pBegin, C *pEnd, U16 u16) {
    C *p = pEnd;
    do
        *--p = (U8)(u16 % 10) | '0';
    while (u16 /= 10);
    if (p != pBegin) {
        C *pD = pBegin;
        do
            *pD++ = *p++;
        while (p != pEnd);
        return pD;
    }
    return pEnd;
}

// pEnd - pBegin should be 10
inline static C *U32ToS10(C *pBegin, C *pEnd, U32 u32) {
    C *p = pEnd;
    do
        *--p = (U8)(u32 % 10) | '0';
    while (u32 /= 10);
    if (p != pBegin) {
        C *pD = pBegin;
        do
            *pD++ = *p++;
        while (p != pEnd);
        return pD;
    }
    return pEnd;
}

// pEnd - pBegin should be 20
inline static C *U64ToS10(C *pBegin, C *pEnd, U64 u64) {
    C *p = pEnd;
    do
        *--p = (U8)(u64 % 10) | '0';
    while (u64 /= 10);
    if (p != pBegin) {
        C *pD = pBegin;
        do
            *pD++ = *p++;
        while (p != pEnd);
        return pD;
    }
    return pEnd;
}