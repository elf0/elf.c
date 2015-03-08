#ifndef HTTPCLIENT
#define HTTPCLIENT

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#define HTTP_VERSION_1_0

#include "Type.h"
#include "File.h"

#define HTTP_RECEIVE_BUFFER_SIZE 65535
#define HTTP_MIN_STATUS_LINE_SIZE 14
#define HTTP_MAX_REASON_PHRASE_SIZE 255
#define HTTP_MAX_FIELD_LINE_SIZE 255
#define HTTP_MAX_CHUNK_SIZE_LENGTH 8

enum ResultOfHttpParsing{
    rohpOk,
    rohpNeedMoreData,
    rohpBadFormat,
    rohpCarriageReturnExpected,
    rohpSpaceExpected,
    rohpColonExpected,
    rohpNumberExpected,
    rohpInvalidStatusCode,
    rohpStatusLineTooLong,
    rohpFieldLineTooLong,
    rohpFieldNameTooLong,
    rohpFieldValueTooLong,
    rohpChunkSizeTooLong,
};

typedef struct HttpClient HttpClient;
typedef ResultOfHttpParsing (*ParseFunction)(HttpClient *pClient, const Char *pBegin, Char *pEnd);
typedef int (*DataHandler)(HttpClient *pClient, const Char *pBegin, const Char *pEnd);
typedef int (*FieldHandler)(HttpClient *pClient, const Char *pName, const Char *pNameEnd, const Char *pValue, const Char *pValueEnd);

struct HttpClient{
    HTTPCLIENT_BASE base;
    ParseFunction Parse;
    U32 bChunkedBody: 1;
    U32 nChunkSize;
    U64 nContentLength;
    File file;
    //user must initialize Handlers;
    DataHandler onVersion;
    DataHandler onStatusCode;
    DataHandler onReasonPhrase;
    FieldHandler onField;
    DataHandler onBody;
    DataHandler onComplete;

    Byte *pDataEnd;
    Byte szBuffer[HTTP_RECEIVE_BUFFER_SIZE];
};

static ResultOfHttpParsing HttpClient_ParseStatusLine(HttpClient *pClient, const Char *pBegin, Char *pEnd);
static inline ResultOfHttpParsing HttpClient_ParseFieldLine(HttpClient *pClient, const Char *pBegin, Char *pEnd);
static ResultOfHttpParsing HttpClient_ParseMessageBody(HttpClient *pClient, const Char *pBegin, Char *pEnd);
static ResultOfHttpParsing HttpClient_ParseChunk(HttpClient *pClient, const Char *pBegin, Char *pEnd);
static ResultOfHttpParsing HttpClient_ParseChunkedData(HttpClient *pClient, const Char *pBegin, Char *pEnd);

static inline void HttpClient_Initialize(HttpClient *pClient, DataHandler onVersion,
                                         DataHandler onStatusCode,
                                         DataHandler onReasonPhrase,
                                         FieldHandler onField,
                                         DataHandler onBody,
                                         DataHandler onComplete){
    pClient->Parse = HttpClient_ParseStatusLine;
    pClient->bChunkedBody = 0;
    pClient->nChunkSize = 0;
    pClient->nContentLength = 0;
    pClient->file.fd = -1;
    pClient->onVersion = onVersion;
    pClient->onStatusCode = onStatusCode;
    pClient->onReasonPhrase = onReasonPhrase;
    pClient->onField = onField;
    pClient->onBody = onBody;
    pClient->onComplete = onComplete;
    pClient->pDataEnd = pClient->szBuffer;
}

static inline Char *HttpClient_ReadLine(const Char *pBegin, Char *pEnd){
    return (Char*)String_SkipUntil(pBegin, '\n');
}

static ResultOfHttpParsing HttpClient_ParseStatusLine(HttpClient *pClient, const Char *pBegin, Char *pEnd){
    const Char *pLineEnd = HttpClient_ReadLine(pBegin, pEnd);
    U32 nSize = pLineEnd - pBegin;
    if(pLineEnd == pEnd){
        if(nSize > (HTTP_MIN_STATUS_LINE_SIZE + HTTP_MAX_REASON_PHRASE_SIZE))
            return rohpStatusLineTooLong;
        return rohpNeedMoreData;
    }

    if(nSize < HTTP_MIN_STATUS_LINE_SIZE ||
            !(String_Equal8(pBegin, (const Char*)"HTTP/1.1")
          #ifdef HTTP_VERSION_1_0
              || String_Equal8(pBegin, (const Char*)"HTTP/1.0")
          #endif
              ))
        return rohpBadFormat;

    const Char *p = pBegin + 8;
    if(*p != 0x20)
        return rohpSpaceExpected;
    pClient->onVersion(pClient, pBegin, p);
    ++p;

    const Char *pString = p;
    p = String_SkipDigit(pString);
    if((p - pString) != 3)
        return rohpInvalidStatusCode;
    if(*p != 0x20)
        return rohpSpaceExpected;
    pClient->onStatusCode(pClient, pString, p);
    ++p;

    pString = p;
    p = pLineEnd - 1;
    if(*p != 0x0D)
        return rohpCarriageReturnExpected;
    pClient->onReasonPhrase(pClient, pString, p);
    p = pLineEnd + 1;


    pClient->Parse = HttpClient_ParseFieldLine;
    return HttpClient_ParseFieldLine(pClient, p, pEnd);
}

static inline ResultOfHttpParsing HttpClient_ParseFieldLine(HttpClient *pClient, const Char *pBegin, Char *pEnd){
    Char *pLineEnd = HttpClient_ReadLine(pBegin, pEnd);
    U32 nSize = pLineEnd - pBegin;
    if(pLineEnd == pEnd){
        if(nSize > HTTP_MAX_FIELD_LINE_SIZE)
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
    U32 nNameSize = pNameEnd - pBegin;
    switch(nNameSize){
    case 14:
        if(String_Equal8(pBegin, (const Char *)"Content-") && String_Equal6(&pBegin[8], (const Char *)"Leng", (const Char *)"th")){
            p = String_SkipDigit(pValue);
            if(p == pValue)
                return rohpNumberExpected;
            pClient->nContentLength = String_ToU64(pValue, p);
        }
        break;
    case 17:
        if(String_Equal8(pBegin, (const Char *)"Transfer") && String_Equal8(&pBegin[8], (const Char *)"-Encodin") && pBegin[16] == 'g'){
            if(String_Equal8(pValue, (const Char *)"chunked\r")){
                pClient->bChunkedBody = 1;
            }
        }
        break;
    }

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
        pClient->onComplete(pClient, null, null);
        pClient->pDataEnd = pClient->szBuffer;
        pClient->Parse = HttpClient_ParseStatusLine;
        return rohpOk;
    }
    pClient->Parse = HttpClient_ParseChunkedData;
    return HttpClient_ParseChunkedData(pClient, pLineEnd + 1, pEnd);
}

static ResultOfHttpParsing HttpClient_ParseMessageBody(HttpClient *pClient, const Char *pBegin, Char *pEnd){
    U32 nSize = pEnd - pBegin;

    if(nSize > pClient->nContentLength)
        nSize = pClient->nContentLength;

    pClient->onBody(pClient, pBegin, pBegin + nSize);
    pClient->nContentLength -= nSize;
    if(pClient->nContentLength == 0)
        pClient->onComplete(pClient, null, null);
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

#endif // HTTPCLIENT

