// pEnd - pBegin should be 3
inline static char *U8ToSD(char *pBegin, char *pEnd, U8 u8) {
    char *p = pEnd;
    do
        *--p = u8 % 10 + '0';
    while (u8 /= 10);
    if (p != pBegin) {
        char *pD = pBegin;
        do
            *pD++ = *p++;
        while (p != pEnd);
        return pD;
    }
    return pEnd;
}

// pEnd - pBegin should be 5
inline static char *U16ToSD(char *pBegin, char *pEnd, U16 u16) {
    char *p = pEnd;
    do
        *--p = u16 % 10 + '0';
    while (u16 /= 10);
    if (p != pBegin) {
        char *pD = pBegin;
        do
            *pD++ = *p++;
        while (p != pEnd);
        return pD;
    }
    return pEnd;
}

// pEnd - pBegin should be 10
inline static char *U32ToSD(char *pBegin, char *pEnd, U32 u32) {
    char *p = pEnd;
    do
        *--p = u32 % 10 + '0';
    while (u32 /= 10);
    if (p != pBegin) {
        char *pD = pBegin;
        do
            *pD++ = *p++;
        while (p != pEnd);
        return pD;
    }
    return pEnd;
}

// pEnd - pBegin should be 20
inline static char *U64ToSD(char *pBegin, char *pEnd, U64 u64) {
    char *p = pEnd;
    do
        *--p = u64 % 10 + '0';
    while (u64 /= 10);
    if (p != pBegin) {
        char *pD = pBegin;
        do
            *pD++ = *p++;
        while (p != pEnd);
        return pD;
    }
    return pEnd;
}
