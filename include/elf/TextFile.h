#ifndef TEXTFILE_H
#define TEXTFILE_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

#include "File.h"

#define MAX_BLOCK_LINES 255
#define MAX_LINE_SIZE 16777215

typedef enum{
    TextFile_ErrorCode_Argument = 1, TextFile_ErrorCode_Create, TextFile_ErrorCode_Open, TextFile_ErrorCode_Verify,
    TextFile_ErrorCode_Read, TextFile_ErrorCode_Write,
    TextFile_ErrorCode_ReadHeader, TextFile_ErrorCode_ReadLine,
    TextFile_ErrorCode_WriteHeader, TextFile_ErrorCode_WriteLine,
    TextFile_ErrorCode_BufferSize
}TextFile_ErrorCode;

typedef enum{
    TextFile_ResultCode_Ok = 0, TextFile_ResultCode_Eof
}TextFile_ResultCode;

//8 bytes
typedef struct{
    U32 nSize: 24;
    U32 nUtf16: 1;
    U32 nReserved: 7;
    U32 nSum;
}TextLine;

//2k bytes
typedef struct{
    U32 nSum: 24;
    U32 nCount: 8;
    U32 nNext;
    TextLine szLines[MAX_BLOCK_LINES];
}TextBlock;

static inline B TextBlock_Full(const TextBlock *ptbBlock){
    return ptbBlock->nCount == MAX_BLOCK_LINES;
}

typedef struct{
    File file;
    TextBlock tbBlock;
}TextFile;

static inline U32 TextFile_VerifyCode(const Byte *pBegin, const Byte *pEnd){
    U8 nSequence = 0;
    U32 nResult = 0;
    const Byte *p = pBegin;
    while(p != pEnd)
        nResult += *p++ ^ nSequence++;
    return nResult;
}

#endif // TEXTFILE_H
