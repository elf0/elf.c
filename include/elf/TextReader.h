#ifndef TEXTREADER_H
#define TEXTREADER_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

#include "TextFile.h"

typedef struct{
    TextFile file;
    U64 nBlockOffset;
    U32 nLine;
}TextReader;

static inline B TextReader_ReadBlockHeader(TextReader *pReader);

static inline I32 TextReader_Open(TextReader *pReader, const C *pszPathName){
    if(!File_OpenForRead(&pReader->file.file, pszPathName))
        return -TextFile_ErrorCode_Open;

    pReader->nLine = 0;
    pReader->nBlockOffset = 0;
    pReader->file.file.nOffset = sizeof(TextBlock);

    if(!TextReader_ReadBlockHeader(pReader)){
        File_Close(&pReader->file.file);
        return -TextFile_ErrorCode_ReadHeader;
    }

    TextBlock *ptbBlock = &pReader->file.tbBlock;
    U32 nSum = TextFile_VerifyCode((const Byte*)ptbBlock + 3, (const Byte*)(ptbBlock + 1));

    if(nSum != ptbBlock->nSum){
        File_Close(&pReader->file.file);
        return -TextFile_ErrorCode_Verify;
    }
    return TextFile_ResultCode_Ok;
}

static inline B TextReader_ReadBlockHeader(TextReader *pReader){
    return File_ReadAt(&pReader->file.file, (Byte*)&pReader->file.tbBlock, sizeof(TextBlock), pReader->nBlockOffset) == sizeof(TextBlock);
}

static inline I32 TextReader_ReadLine(TextReader *pReader, C *pBuffer, U32 *pnSize){
    TextBlock *ptbBlock = &pReader->file.tbBlock;
    if(pReader->nLine == ptbBlock->nCount){
        if(ptbBlock->nNext == 0)
            return TextFile_ResultCode_Eof;

        pReader->nBlockOffset += ptbBlock->nNext;
        if(!TextReader_ReadBlockHeader(pReader))
            return -TextFile_ErrorCode_ReadHeader;

        U32 nSum = TextFile_VerifyCode((const Byte*)ptbBlock + 3, (const Byte*)(ptbBlock + 1));

        if(nSum != ptbBlock->nSum)
            return -TextFile_ErrorCode_Verify;

        pReader->nLine = 0;
        pReader->file.file.nOffset = pReader->nBlockOffset + sizeof(TextBlock);
    }

    TextLine *pLine = &ptbBlock->szLines[pReader->nLine];
    if(pLine->nSize > *pnSize)
        return -TextFile_ErrorCode_BufferSize;

    if(File_Read(&pReader->file.file, pBuffer, pLine->nSize) != pLine->nSize)
        return -TextFile_ErrorCode_ReadLine;

    U32 nSum = TextFile_VerifyCode(pBuffer, pBuffer + pLine->nSize);

    if(nSum != pLine->nSum)
        return -TextFile_ErrorCode_Verify;

    ++pReader->nLine;
    *pnSize = pLine->nSize;
    return TextFile_ResultCode_Ok;
}

static inline void TextReader_Close(TextReader *pReader){
    File_Close(&pReader->file.file);
}

#endif // TEXTREADER_H
