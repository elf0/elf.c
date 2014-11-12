#ifndef ERROR_H
#define ERROR_H

enum ErrorCode{
    ErrorCode_Error = 1, ErrorCode_Argument, ErrorCode_Size, ErrorCode_Verify, ErrorCode_Full, ErrorCode_Format,
    ErrorCode_BufferSize, ErrorCode_DataSize,
    //File
    ErrorCode_File_Open, ErrorCode_File_Access, ErrorCode_File_NotFound,
    ErrorCode_File_Read, ErrorCode_File_ReadMeta,
    ErrorCode_File_Write, ErrorCode_File_WriteMeta,
};

#endif // ERROR_H
