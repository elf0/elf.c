#ifndef WAVFILE_H
#define WAVFILE_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

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

#define WAVHEADER_SIZE sizeof(WAVHeader)

typedef struct{
    WAVHeader header;
    Byte szSamples[4];
}WAVFile;


#endif // WAVFILE_H
