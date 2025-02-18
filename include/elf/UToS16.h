// szChars: "0123456789ABCDEF" or "0123456789abcdef"

// pEnd - pBegin should be 2
inline static C *U8ToS16(C *pBegin, C *pEnd, U8 u8, const C szChars[16]) {
    C *p = pEnd;
    *--p = szChars[u8 & 0xF];
    if (u8 >>= 4) {
        *--p = szChars[u8];
        return pEnd;
    }
    *pBegin = *p;
    return p;
}

// pEnd - pBegin should be 4
inline static C *U16ToS16(C *pBegin, C *pEnd, U16 u16, const C szChars[16]) {
    C *p = pEnd;
    do
        *--p = szChars[(U8)u16 & 0xF];
    while (u16 >>= 4);
    if (p != pBegin) {
        C *pD = pBegin;
        do
            *pD++ = *p++;
        while (p != pEnd);
        return pD;
    }
    return pEnd;
}

// pEnd - pBegin should be 8
inline static C *U32ToS16(C *pBegin, C *pEnd, U32 u32, const C szChars[16]) {
    C *p = pEnd;
    do
        *--p = szChars[(U8)u32 & 0xF];
    while (u32 >>= 4);
    if (p != pBegin) {
        C *pD = pBegin;
        do
            *pD++ = *p++;
        while (p != pEnd);
        return pD;
    }
    return pEnd;
}

// pEnd - pBegin should be 16
inline static C *U64ToS16(C *pBegin, C *pEnd, U64 u64, const C szChars[16]) {
    C *p = pEnd;
    do
        *--p = szChars[(U8)u64 & 0xF];
    while (u64 >>= 4);
    if (p != pBegin) {
        C *pD = pBegin;
        do
            *pD++ = *p++;
        while (p != pEnd);
        return pD;
    }
    return pEnd;
}