#ifndef WAVREADER_H
#define WAVREADER_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

#include "MMFile.h"
#include "WAVFile.h"

typedef struct{
    MMFile file;
    RIFF_Chunk *prcListChunk;
}WAVReader;

static inline WAVFile *WAVReader_File(WAVReader *pReader){
    return (WAVFile*)pReader->file.pBegin;
}

static inline U32 WAVReader_Seconds(WAVReader *pReader){
    WAVHeader *pHeader = (WAVHeader*)WAVReader_File(pReader);
    WAVMeta *pMeta = &pHeader->wmMeta;
    return pHeader->rcSampleChunk.uContent / pMeta->uBytesPerSecond;
}

static inline F32 WAVReader_Time(WAVReader *pReader){
    WAVHeader *pHeader = (WAVHeader*)WAVReader_File(pReader);
    WAVMeta *pMeta = &pHeader->wmMeta;
    return (F32)pHeader->rcSampleChunk.uContent / (F32)pMeta->uBytesPerSecond;
}

static inline U32 WAVReader_Milliseconds(WAVReader *pReader){
    WAVHeader *pHeader = (WAVHeader*)WAVReader_File(pReader);
    WAVMeta *pMeta = &pHeader->wmMeta;
    return pHeader->rcSampleChunk.uContent * 1000 / pMeta->uBytesPerSecond;
}

static inline E8 WAVReader_Open(WAVReader *pReader, const C *pFileName){
    E8 e = MMFile_OpenForRead((MMFile*)pReader, pFileName);
    if(e)
        return 1;

    U64 uFileSize = pReader->file.file.meta.st_size;
    if(uFileSize < WAVHEADER_SIZE){
        MMFile_Close((MMFile*)pReader);
        return 2;
    }

    WAVHeader *pHeader = (WAVHeader*)WAVReader_File(pReader);
    RIFF_Chunk *pChunk = &pHeader->rcChunk;

    if(pChunk->uId != *(U32*)"RIFF"){
        MMFile_Close((MMFile*)pReader);
        return 3;
    }

    if(sizeof(RIFF_Chunk) + pChunk->uContent != uFileSize){
        MMFile_Close((MMFile*)pReader);
        return 4;
    }

    if(pHeader->uFormat != *(U32*)"WAVE"){
        MMFile_Close((MMFile*)pReader);
        return 5;
    }

    pChunk = &pHeader->rcMetaChunk;
    if(pChunk->uId != *(U32*)"fmt "){
        MMFile_Close((MMFile*)pReader);
        return 6;
    }

    if(pChunk->uContent != sizeof(WAVMeta)){
        MMFile_Close((MMFile*)pReader);
        return 7;
    }

    WAVMeta *pMeta = &pHeader->wmMeta;

    if(pMeta->uFormat != 1){ //PCM
        MMFile_Close((MMFile*)pReader);
        return 8;
    }

    pChunk = &pHeader->rcSampleChunk;
    if(pChunk->uId != *(U32*)"data"){
        MMFile_Close((MMFile*)pReader);
        return 9;
    }

    I64 iTail = uFileSize - (WAVHEADER_SIZE + pChunk->uContent);
    if(iTail < 0){
        MMFile_Close((MMFile*)pReader);
        return 10;
    }
    else if(iTail < sizeof(RIFF_Chunk))
        pReader->prcListChunk = NULL;
    else{
        pChunk = (RIFF_Chunk*)((Byte*)pHeader + WAVHEADER_SIZE + pChunk->uContent);
        if(pChunk->uId == *(U32*)"LIST")
            pReader->prcListChunk = pChunk;
        else{
            MMFile_Close((MMFile*)pReader);
            return 11;
        }
    }

    return 0;
}

static inline void WAVReader_Close(WAVReader *pReader){
    MMFile_Close((MMFile*)pReader);
}

#endif // WAVREADER_H
