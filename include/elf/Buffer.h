#ifndef BUFFER_H
#define BUFFER_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

#include <string.h>
#include "Type.h"

typedef struct{
    Byte *pBegin;
    Byte *pEnd;
    Byte *pData;
    Byte *pDataEnd;
}Buffer;

static inline void Buffer_Initialize(Buffer *pbBuffer, Byte *pBegin, Byte *pEnd){
    pbBuffer->pBegin = pBegin;
    pbBuffer->pData = pBegin;
    pbBuffer->pDataEnd = pBegin;
    pbBuffer->pEnd = pEnd;
}

static inline U32 Buffer_Size(Buffer *pbBuffer){
    return pbBuffer->pEnd - pbBuffer->pBegin;
}

static inline void Buffer_Finalize(Buffer *pbBuffer){
    pbBuffer->pBegin = null;
}

static inline B Buffer_IsFinalized(Buffer *pbBuffer){
    return pbBuffer->pBegin == null;
}

static inline Byte *Buffer_Begin(Buffer *pbBuffer){
    return pbBuffer->pBegin;
}

static inline Byte *Buffer_End(Buffer *pbBuffer){
    return pbBuffer->pEnd;
}

static inline B Buffer_HasData(Buffer *pbBuffer){
    return pbBuffer->pData != pbBuffer->pDataEnd;
}

static inline U32 Buffer_DataSize(Buffer *pbBuffer){
    return pbBuffer->pDataEnd - pbBuffer->pData;
}

static inline Byte *Buffer_Data(Buffer *pbBuffer){
    return pbBuffer->pData;
}

static inline Byte *Buffer_DataEnd(Buffer *pbBuffer){
    return pbBuffer->pDataEnd;
}

static inline void Buffer_Clear(Buffer *pbBuffer){
    pbBuffer->pData = pbBuffer->pBegin;
    pbBuffer->pDataEnd = pbBuffer->pBegin;
}

static inline U32 Buffer_TailSize(Buffer *pbBuffer){
    return pbBuffer->pEnd - pbBuffer->pDataEnd;
}

static inline Byte *Buffer_Tail(Buffer *pbBuffer){
    return pbBuffer->pDataEnd;
}

static inline void Buffer_Push(Buffer *pbBuffer, U32 nSize){
    pbBuffer->pDataEnd += nSize;
}

static inline void Buffer_Pop(Buffer *pbBuffer, U32 nSize){
    pbBuffer->pData += nSize;
}

static inline void Buffer_SetData(Buffer *pbBuffer, Byte *pData){
    pbBuffer->pData = pData;
}

static inline void Buffer_MoveData(Buffer *pbBuffer){
    U32 nData = Buffer_DataSize(pbBuffer);
    memmove(pbBuffer->pBegin, pbBuffer->pData, nData);
    pbBuffer->pData = pbBuffer->pBegin;
    pbBuffer->pDataEnd = pbBuffer->pBegin + nData;
}

#endif // BUFFER_H

