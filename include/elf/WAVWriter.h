#ifndef WAVWRITER_H
#define WAVWRITER_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

#include "File.h"
#include "WAVFile.h"

typedef struct{
    File file;
    WAVHeader header;
    U64 uOffset;
}WAVWriter;

static inline E8 WAVWriter_Create(WAVWriter *pWriter, const C *pFileName, U16 uChannels, U16 uBitsPerSample, U32 uSampleRate){
    E8 e = !File_Create((File*)pWriter, pFileName);
    if(e)
        return 1;

    WAVHeader *pHeader = &pWriter->header;
    pHeader->rcChunk .uId= *(U32*)"RIFF";
    pHeader->rcChunk .uContent = WAVHEADER_SIZE;
    pHeader->uFormat = *(U32*)"WAVE";
    pHeader->rcMetaChunk .uId= *(U32*)"fmt ";
    pHeader->rcMetaChunk .uContent= sizeof(WAVMeta);

    WAVMeta *pMeta = &pHeader->wmMeta;
    pMeta->uFormat = 1;
    pMeta->uChannels = uChannels;
    pMeta->uSampleRate = uSampleRate;
    pMeta->uBytesPerSecond = (uChannels * uSampleRate * (uBitsPerSample >> 3)  + 0x3) & (~0x3);
    pMeta->uAlign = 4;
    pMeta->uBitsPerSample = uBitsPerSample;

    pHeader->rcSampleChunk .uId= *(U32*)"data";
    pHeader->rcSampleChunk .uContent = 0;
    pWriter->uOffset = WAVHEADER_SIZE;

    return 0;
}

static inline E8 WAVWriter_Open(WAVWriter *pWriter, const C *pFileName){
    File *pFile = (File*)pWriter;
    E8 e = !File_Open(pFile, pFileName);
    if(e)
        return 1;

    if(!File_ReadMeta(pFile)){
        File_Close(pFile);
        return 2;
    }

    U64 uFileSize = pWriter->file.meta.st_size;
    if(uFileSize < WAVHEADER_SIZE){
        File_Close(pFile);
        return 3;
    }

    WAVHeader *pHeader = &pWriter->header;

    if(File_Read(pFile, (Byte*)pHeader, WAVHEADER_SIZE) != WAVHEADER_SIZE){
        File_Close(pFile);
        return 4;
    }

    RIFF_Chunk *pChunk = &pHeader->rcChunk;

    if(pChunk->uId != *(U32*)"RIFF"){
        File_Close(pFile);
        return 5;
    }

    if(sizeof(RIFF_Chunk) + pChunk->uContent != uFileSize){
        File_Close(pFile);
        return 6;
    }

    if(pHeader->uFormat != *(U32*)"WAVE"){
        File_Close(pFile);
        return 7;
    }

    pChunk = &pHeader->rcMetaChunk;
    if(pChunk->uId != *(U32*)"fmt "){
        File_Close(pFile);
        return 8;
    }

    if(pChunk->uContent != sizeof(WAVMeta)){
        File_Close(pFile);
        return 9;
    }

    WAVMeta *pMeta = &pHeader->wmMeta;

    if(pMeta->uFormat != 1){ //PCM
        File_Close(pFile);
        return 10;
    }

    pChunk = &pHeader->rcSampleChunk;
    if(pChunk->uId != *(U32*)"data"){
        File_Close(pFile);
        return 11;
    }

    pWriter->uOffset = WAVHEADER_SIZE + pChunk->uContent;
    if(pWriter->uOffset > uFileSize){
        File_Close(pFile);
        return 12;
    }

    return 0;
}

static inline void WAVWriter_Close(WAVWriter *pWriter){
    File_Close((File*)pWriter);
}

static inline void WAVWriter_AdjustHeader(WAVWriter *pWriter){
    WAVHeader *pHeader = &pWriter->header;
    RIFF_Chunk *pChunk = &pHeader->rcSampleChunk;
    if(pWriter->uOffset != WAVHEADER_SIZE + pChunk->uContent){
        pHeader->rcChunk .uContent = pWriter->uOffset - sizeof(RIFF_Chunk);
        pChunk->uContent = pWriter->uOffset - WAVHEADER_SIZE;
    }
}

static inline E8 WAVWriter_WriteHeader(WAVWriter *pWriter){
    E8 e = File_WriteTo((File*)pWriter, 0, (Byte*)&pWriter->header, WAVHEADER_SIZE) != WAVHEADER_SIZE;
    return e;
}

static inline E8 WAVWriter_Append(WAVWriter *pWriter, Byte *pSamples, U32 uSamples){
    WAVHeader *pHeader = &pWriter->header;
    WAVMeta *pMeta = &pHeader->wmMeta;
    U32 uBytesPerSample = WAVMeta_BytesPerSample(pMeta);
    U32 uBytes = uSamples * uBytesPerSample;
    if(File_WriteTo((File*)pWriter, pWriter->uOffset, pSamples, uBytes) != uBytes)
        return  1;
    pWriter->uOffset += uBytes;
    return 0;
}

static inline E8 WAVWriter_Modify(WAVWriter *pWriter, U32 uIndex, Byte *pSamples, U32 uSamples){
    WAVHeader *pHeader = &pWriter->header;
    WAVMeta *pMeta = &pHeader->wmMeta;
    U32 uBytesPerSample = WAVMeta_BytesPerSample(pMeta);
    U64 uOffset = WAVHEADER_SIZE + uIndex * uBytesPerSample;
    U32 uBytes = uSamples * uBytesPerSample;
    if(uOffset + uBytes > pWriter->uOffset)
        return 1;

    if(File_WriteTo((File*)pWriter, uOffset, pSamples, uBytes) != uBytes)
        return  2;

    return 0;
}

#endif // WAVWRITER_H
