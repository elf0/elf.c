#ifndef ERROR_H
#define ERROR_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

typedef enum{
    ErrorCode_Error = 1, ErrorCode_Argument, ErrorCode_IllegalValue, ErrorCode_Size,
    ErrorCode_Verify, ErrorCode_Full, ErrorCode_Format,
    ErrorCode_BufferSize, ErrorCode_DataSize,
    //File
    ErrorCode_File_Create, ErrorCode_File_Open, ErrorCode_File_Access, ErrorCode_File_NotFound,
    ErrorCode_File_Read, ErrorCode_File_ReadMeta,
    ErrorCode_File_Write, ErrorCode_File_WriteMeta,
    //Database
    ErrorCode_Database_Error, ErrorCode_Database_NotOpen, ErrorCode_Database_KeyNotFound,
}ErrorCode;

#endif // ERROR_H

