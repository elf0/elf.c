#ifndef WAVREADER_H
#define WAVREADER_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

#include "MMFile.h"

typedef struct{
    U32 uId;
    U32 uContent;
}RIFF_Chunk;

typedef struct{
    U16 uFormat;
    U16 uChannels;
    U32 uSampleRate;
    U32 uBytesPerSecond;
    U16 uAlign;
    U16 uBitsPerSample;
}WAVMeta;

typedef struct{
    RIFF_Chunk rcChunk;
    U32 uFormat;
    RIFF_Chunk rcMetaChunk;
    WAVMeta wmMeta;
    RIFF_Chunk rcSampleChunk;
    Byte szSamples[4];
}WAVHeader;

#define WAVHEADER_SIZE (sizeof(WAVHeader) - 4)

typedef struct{
    MMFile file;
    RIFF_Chunk *prcListChunk;
}WAVReader;

static inline WAVHeader *WAVReader_Header(WAVReader *pReader){
    return (WAVHeader*)pReader->file.pBegin;
}

static inline U32 WAVReader_Seconds(WAVReader *pReader){
    WAVHeader *pHeader = WAVReader_Header(pReader);
    WAVMeta *pMeta = &pHeader->wmMeta;
    return pHeader->rcSampleChunk.uContent / pMeta->uBytesPerSecond;
}

static inline F32 WAVReader_Time(WAVReader *pReader){
    WAVHeader *pHeader = WAVReader_Header(pReader);
    WAVMeta *pMeta = &pHeader->wmMeta;
    return (F32)pHeader->rcSampleChunk.uContent / (F32)pMeta->uBytesPerSecond;
}

static inline U32 WAVReader_Milliseconds(WAVReader *pReader){
    WAVHeader *pHeader = WAVReader_Header(pReader);
    WAVMeta *pMeta = &pHeader->wmMeta;
    return pHeader->rcSampleChunk.uContent * 1000 / pMeta->uBytesPerSecond;
}

static inline E8 WAVReader_Open(WAVReader *pReader, const C *pFileName){
    E8 e = MMFile_OpenForRead((MMFile*)pReader, pFileName);
    if(e)
        return 1;

    U64 uFileSize = pReader->file.file.meta.st_size;
    if(uFileSize < WAVHEADER_SIZE)
        return 2;

    WAVHeader *pHeader = WAVReader_Header(pReader);
    RIFF_Chunk *pChunk = &pHeader->rcChunk;

    if(pChunk->uId != *(U32*)"RIFF")
        return 3;

    if(sizeof(RIFF_Chunk) + pChunk->uContent != uFileSize)
        return 4;

    if(pHeader->uFormat != *(U32*)"WAVE")
        return 5;

    pChunk = &pHeader->rcMetaChunk;
    if(pChunk->uId != *(U32*)"fmt ")
        return 6;

    if(pChunk->uContent != sizeof(WAVMeta))
        return 7;

    WAVMeta *pMeta = &pHeader->wmMeta;

    if(pMeta->uFormat != 1) //PCM
        return 8;

    pChunk = &pHeader->rcSampleChunk;
    if(pChunk->uId != *(U32*)"data")
        return 9;

    I64 iTail = uFileSize - (WAVHEADER_SIZE + pChunk->uContent);
    if(iTail < 0)
        return 10;
    else if(iTail < sizeof(RIFF_Chunk))
        pReader->prcListChunk = NULL;
    else{
        pChunk = (RIFF_Chunk*)((Byte*)pHeader + WAVHEADER_SIZE + pChunk->uContent);
        if(pChunk->uId == *(U32*)"LIST")
            pReader->prcListChunk = pChunk;
        else
            return 11;
    }

    return 0;
}

static inline void WAVReader_Close(WAVReader *pReader){
    MMFile_Close((MMFile*)pReader);
}

#endif // WAVREADER_H
