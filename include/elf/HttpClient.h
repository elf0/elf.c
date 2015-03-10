#ifndef HTTPCLIENT
#define HTTPCLIENT

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

//Note: User MUST define macro "HTTPCLIENT_BASE" before this file

#define HTTP_VERSION_1_0

#include "Type.h"
#include "File.h"

#define HTTP_RECEIVE_BUFFER_SIZE 65535
#define HTTP_MAX_REASON_PHRASE_SIZE 255
#define HTTP_MAX_FIELD_LINE_SIZE 255
#define HTTP_MAX_CHUNK_SIZE_LENGTH 8

enum ResultOfHttpParsing{
    rohpOk,
    rohpNeedMoreData,
    rohpBadFormat,
    rohpCarriageReturnExpected,
    rohpDotExpected,
    rohpSpaceExpected,
    rohpColonExpected,
    rohpNumberExpected,
    rohpInvalidMajorVersion,
    rohpInvalidMinorVersion,
    rohpInvalidStatusCode,
    rohpReasonPhraseTooLong,
    rohpFieldLineTooLong,
    rohpChunkSizeTooLong,
};

typedef struct HttpClient HttpClient;
typedef int (*EventHandler)(HttpClient *pClient);
typedef int (*DataHandler)(HttpClient *pClient, const Char *pBegin, const Char *pEnd);
typedef int (*FieldHandler)(HttpClient *pClient, const Char *pName, const Char *pNameEnd, const Char *pValue, const Char *pValueEnd);
//Api
static inline void HttpClient_Initialize(HttpClient *pClient, DataHandler onVersion,
                                         DataHandler onStatusCode,
                                         DataHandler onReasonPhrase,
                                         FieldHandler onField,
                                         EventHandler onHeaderComplete,
                                         DataHandler onBody,
                                         EventHandler onComplete);

static inline ResultOfHttpParsing HttpClient_Parse(HttpClient *pClient, const Char *pBegin, Char *pEnd);

typedef ResultOfHttpParsing (*ParseFunction)(HttpClient *pClient, const Char *pBegin, Char *pEnd);

struct HttpClient{
    HTTPCLIENT_BASE base;
    ParseFunction Parse;
    //user must initialize Handlers;
    DataHandler onVersion;
    DataHandler onStatusCode;
    DataHandler onReasonPhrase;
    FieldHandler onField;
    EventHandler onHeaderComplete;
    DataHandler onBody;
    EventHandler onComplete;
    File file;
    U32 nVersionMajor: 1;
    U32 nVersionMinor: 1;
    U32 nStatusCode: 9;
    U32 bContentLength: 1;
    U32 bContentRange: 1;
    U32 bChunkedBody: 1;
    U32 bLastModified: 1;
    U32 nChunkSize;
    U64 nContentLength;
    U64 nFileSize;
    U64 nReceived;
    Byte *pDataEnd;
    Byte szBuffer[HTTP_RECEIVE_BUFFER_SIZE];
};

//Internal
static ResultOfHttpParsing HttpClient_ParseHttpVersion(HttpClient *pClient, const Char *pBegin, Char *pEnd);
static inline ResultOfHttpParsing HttpClient_ParseStatusCode(HttpClient *pClient, const Char *pBegin, Char *pEnd);
static inline ResultOfHttpParsing HttpClient_ParseReasonPhrase(HttpClient *pClient, const Char *pBegin, Char *pEnd);
static inline ResultOfHttpParsing HttpClient_ParseFieldLine(HttpClient *pClient, const Char *pBegin, Char *pEnd);
static ResultOfHttpParsing HttpClient_ParseMessageBody(HttpClient *pClient, const Char *pBegin, Char *pEnd);
static ResultOfHttpParsing HttpClient_ParseChunk(HttpClient *pClient, const Char *pBegin, Char *pEnd);
static ResultOfHttpParsing HttpClient_ParseChunkedData(HttpClient *pClient, const Char *pBegin, Char *pEnd);
static inline Bool ParseFieldContent(HttpClient *pClient, const Char *pName, const Char *pNameEnd, const Char *pValue, const Char *pValueEnd);

static inline void HttpClient_Initialize(HttpClient *pClient, DataHandler onVersion,
                                         DataHandler onStatusCode,
                                         DataHandler onReasonPhrase,
                                         FieldHandler onField,
                                         EventHandler onHeaderComplete,
                                         DataHandler onBody,
                                         EventHandler onComplete){
    U32 nBegin = offsetof(HttpClient, file);
    U32 nSize = offsetof(HttpClient, pDataEnd) - nBegin;
    memset((Byte*)pClient + nBegin, 0, nSize);
    pClient->Parse = HttpClient_ParseHttpVersion;
    pClient->onVersion = onVersion;
    pClient->onStatusCode = onStatusCode;
    pClient->onReasonPhrase = onReasonPhrase;
    pClient->onField = onField;
    pClient->onHeaderComplete = onHeaderComplete;
    pClient->onBody = onBody;
    pClient->onComplete = onComplete;
    pClient->pDataEnd = pClient->szBuffer;
    pClient->file.fd = -1;
}

static inline ResultOfHttpParsing HttpClient_Parse(HttpClient *pClient, const Char *pBegin, Char *pEnd){
    *pEnd = '\n';
    return pClient->Parse(pClient, pBegin, pEnd);
}

static inline Char *HttpClient_ReadLine(const Char *pBegin, Char *pEnd){
    return (Char*)String_SkipUntil(pBegin, '\n');
}

static ResultOfHttpParsing HttpClient_ParseHttpVersion(HttpClient *pClient, const Char *pBegin, Char *pEnd){
    U32 nSize = pEnd - pBegin;
    if(nSize < 4){
        if(nSize && *pBegin != 'H')
            return rohpBadFormat;
        return rohpNeedMoreData;
    }

    if(!String_Equal4(pBegin, (const Char*)"HTTP"))
        return rohpBadFormat;

    const Char *p = pBegin + 4;
    if(*p != '/'){
        if(p != pEnd)
            return rohpBadFormat;
        else
            return rohpNeedMoreData;
    }
    ++p;

    //Parse major version
    U32 nInteger = 0;
    const Char *pString = p;
    p = String_ParseU32(p, &nInteger);
    if(p == pEnd)
        return rohpNeedMoreData;
    if(p == pString)
        return rohpNumberExpected;
    if(nInteger != 1)
        return rohpInvalidMajorVersion;
    pClient->nVersionMajor = nInteger;

    if(*p != '.')
        return rohpDotExpected;
    ++p;

    //Parse minor version
    nInteger = 0;
    pString = p;
    p = String_ParseU32(p, &nInteger);
    if(p == pEnd)
        return rohpNeedMoreData;
    if(p == pString)
        return rohpNumberExpected;
    if(nInteger > 1)
        return rohpInvalidMinorVersion;
    pClient->nVersionMinor = nInteger;

    if(*p != 0x20)
        return rohpSpaceExpected;
    pClient->onVersion(pClient, pBegin + 5, p);
    ++p;

    pClient->Parse = HttpClient_ParseStatusCode;
    return HttpClient_ParseStatusCode(pClient, p, pEnd);
}

static inline ResultOfHttpParsing HttpClient_ParseStatusCode(HttpClient *pClient, const Char *pBegin, Char *pEnd){
    U32 nInteger = 0;
    const Char *p = String_ParseU32(pBegin, &nInteger);
    if(p == pEnd){
        U32 nSize = p - pBegin;
        memmove(pClient->szBuffer, pBegin, nSize);
        pClient->pDataEnd = pClient->szBuffer + nSize;
        return rohpNeedMoreData;
    }
    if(p == pBegin)
        return rohpNumberExpected;
    if(nInteger < 100 || nInteger > 505)
        return rohpInvalidStatusCode;
    pClient->nStatusCode = nInteger;

    if(*p != 0x20)
        return rohpSpaceExpected;

    pClient->onStatusCode(pClient, pBegin, p);
    ++p;

    pClient->Parse = HttpClient_ParseReasonPhrase;
    return HttpClient_ParseReasonPhrase(pClient, p, pEnd);
}

static inline ResultOfHttpParsing HttpClient_ParseReasonPhrase(HttpClient *pClient, const Char *pBegin, Char *pEnd){
    const Char *p = HttpClient_ReadLine(pBegin, pEnd);
    U32 nSize = p - pBegin;
    if(p == pEnd){
        if(nSize > (HTTP_MAX_REASON_PHRASE_SIZE + 1))
            return rohpReasonPhraseTooLong;

        memmove(pClient->szBuffer, pBegin, nSize);
        pClient->pDataEnd = pClient->szBuffer + nSize;
        return rohpNeedMoreData;
    }

    if(nSize == 0 || p[-1] != 0x0D)
        return rohpCarriageReturnExpected;

    pClient->onReasonPhrase(pClient, pBegin, p - 1);
    ++p;

    pClient->Parse = HttpClient_ParseFieldLine;
    return HttpClient_ParseFieldLine(pClient, p, pEnd);
}

static inline ResultOfHttpParsing HttpClient_ParseFieldLine(HttpClient *pClient, const Char *pBegin, Char *pEnd){
    Char *pLineEnd = HttpClient_ReadLine(pBegin, pEnd);
    U32 nSize = pLineEnd - pBegin;
    if(pLineEnd == pEnd){
        if(nSize > (HTTP_MAX_FIELD_LINE_SIZE + 1))
            return rohpFieldLineTooLong;

        memmove(pClient->szBuffer, pBegin, nSize);
        pClient->pDataEnd = pClient->szBuffer + nSize;
        return rohpNeedMoreData;
    }

    *pLineEnd = ':';
    const Char *p = String_SkipUntil(pBegin, ':');
    if(p == pLineEnd){
        if(nSize != 1 || *pBegin != 0x0D)
            return rohpColonExpected;

        if(pClient->nFileSize == 0)
            pClient->nFileSize = pClient->nContentLength;

        pClient->onHeaderComplete(pClient);

        if(pClient->bChunkedBody)
            pClient->Parse = HttpClient_ParseChunk;
        else
            pClient->Parse = HttpClient_ParseMessageBody;

        return pClient->Parse(pClient, pLineEnd + 1, pEnd);
    }
    const Char *pNameEnd = p;
    ++p;

    if(*p == 0x20)
        ++p;

    const Char *pValue = p;
    p = pLineEnd - 1;
    if(*p != 0x0D)
        return rohpCarriageReturnExpected;
    const Char *pValueEnd = p;

    //Parse field content
    ParseFieldContent(pClient, pBegin, pNameEnd, pValue, pValueEnd);

    pClient->onField(pClient, pBegin, pNameEnd, pValue, pValueEnd);
    return HttpClient_ParseFieldLine(pClient, pLineEnd + 1, pEnd);
}

static ResultOfHttpParsing HttpClient_ParseChunk(HttpClient *pClient, const Char *pBegin, Char *pEnd){
    Char *pLineEnd = HttpClient_ReadLine(pBegin, pEnd);
    U32 nSize = pLineEnd - pBegin;
    if(pLineEnd == pEnd){
        if(nSize > HTTP_MAX_CHUNK_SIZE_LENGTH)
            return rohpChunkSizeTooLong;

        memmove(pClient->szBuffer, pBegin, nSize);
        pClient->pDataEnd = pClient->szBuffer + nSize;
        return rohpNeedMoreData;
    }

    const Char *p = String_ParseHexU32(pBegin, &pClient->nChunkSize);
    if(p == pEnd){
        return rohpBadFormat;
    }

    if(pLineEnd[-1] != 0x0D)
        return rohpCarriageReturnExpected;

    if(pClient->nChunkSize == 0){
        pClient->onComplete(pClient);
        return rohpOk;
    }
    pClient->Parse = HttpClient_ParseChunkedData;
    return HttpClient_ParseChunkedData(pClient, pLineEnd + 1, pEnd);
}

static ResultOfHttpParsing HttpClient_ParseMessageBody(HttpClient *pClient, const Char *pBegin, Char *pEnd){
    U32 nSize = pEnd - pBegin;

    if(nSize > pClient->nContentLength)
        nSize = pClient->nContentLength;

    pClient->nReceived += nSize;
    if(pClient->nFileSize  == 0)
        pClient->nFileSize = pClient->nReceived;

    if(pClient->nContentLength){
        pClient->nContentLength -= nSize;
        pClient->onBody(pClient, pBegin, pBegin + nSize);
        if(pClient->nContentLength == 0)
            pClient->onComplete(pClient);
    }
    else
        pClient->onBody(pClient, pBegin, pBegin + nSize);

    pClient->pDataEnd = pClient->szBuffer;
    return rohpOk;
}

static ResultOfHttpParsing HttpClient_ParseChunkedData(HttpClient *pClient, const Char *pBegin, Char *pEnd){
    if(pBegin == pEnd){
        pClient->pDataEnd = pClient->szBuffer;
        return rohpOk;
    }

    U32 nSize = pEnd - pBegin;
    if(nSize > pClient->nChunkSize)
        nSize = pClient->nChunkSize;

    const Char *pDataEnd = pBegin + nSize;
    pClient->nReceived += nSize;
    pClient->nContentLength = pClient->nReceived;
    pClient->onBody(pClient, pBegin, pDataEnd);
    pClient->nChunkSize -= nSize;
    if(pClient->nChunkSize == 0){
        Char *pLineEnd = HttpClient_ReadLine(pDataEnd, pEnd);
        if(pLineEnd == pEnd){
            pClient->pDataEnd = pClient->szBuffer;
            return rohpOk;
        }

        nSize = pLineEnd - pDataEnd;
        if(nSize != 1 || pLineEnd[-1] != 0x0D)
            return rohpCarriageReturnExpected;

        pClient->Parse = HttpClient_ParseChunk;
        return HttpClient_ParseChunk(pClient, pLineEnd + 1, pEnd);
    }

    pClient->pDataEnd = pClient->szBuffer;
    return rohpOk;
}

static inline Bool ParseFieldContent(HttpClient *pClient, const Char *pName, const Char *pNameEnd, const Char *pValue, const Char *pValueEnd){
    U32 nNameSize = pNameEnd - pName;
    switch(nNameSize){
    case 13:
        if(String_Equal8(pName, (const Char *)"Content-") && String_Equal4(&pName[8], (const Char *)"Rang") && pName[12] == 'e'){
            if(String_Equal6(pValue, (const Char *)"byte", (const Char *)"s ")){
                const Char *p = pValue + 6;
                U64 nBeginOffset = 0;
                p = String_ParseU64(p, &nBeginOffset);
                if(*p == '-'){
                    ++p;
                    U64 nEndOffset = 0;
                    p = String_ParseU64(p, &nEndOffset);
                    if(*p == '/'){
                        ++p;
                        p = String_ParseU64(p, &pClient->nFileSize);
                        pClient->bContentRange = 1;
                    }
                }
            }
        }
        break;
    case 14:
        if(String_Equal8(pName, (const Char *)"Content-") && String_Equal6(&pName[8], (const Char *)"Leng", (const Char *)"th")){
            const Char *p = String_ParseU64(pValue, &pClient->nContentLength);
            if(p == pValue)
                return rohpNumberExpected;
            pClient->bContentLength = 1;
        }
        break;
    case 17:
        if(String_Equal8(pName, (const Char *)"Transfer") && String_Equal8(&pName[8], (const Char *)"-Encodin") && pName[16] == 'g'){
            if(String_Equal8(pValue, (const Char *)"chunked\r")){
                pClient->bChunkedBody = 1;
            }
        }
        break;
    }
    return true;
}

#endif // HTTPCLIENT
