#ifndef ERROR_H
#define ERROR_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

typedef enum{
    error_Error = 1, error_Argument, error_TimeOut, error_IllegalValue, error_Size,
    error_Verify, error_Full, error_Format, error_Version,
    error_BufferSize, error_DataSize,
    //File
    error_File_Create, error_File_Open, error_File_Access, error_File_NotFound,
    error_File_Read, error_File_ReadMeta,
    error_File_Write, error_File_WriteMeta,
    error_File_Format, error_File_Verify,
    //Packet
    error_Packet_Format, error_Packet_Verify,
    //Database
    error_Database_Error, error_Database_NotOpen, error_Database_KeyNotFound,
}Error;

#endif // ERROR_H

