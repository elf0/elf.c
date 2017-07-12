#ifndef DIR_H
#define DIR_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

#ifdef __linux__
#include <errno.h>
#include <dirent.h>
#else
#endif

#include "Type.h"

typedef struct{
#ifdef __linux__
 DIR *pDir;
#else
#endif
}Dir;

typedef struct{
#ifdef __linux__
 struct dirent *pEntry;
#else
#endif
}Dir_Entry;

inline
static E8 Dir_Open(Dir *pDir, const C *pPath){
#ifdef __linux__
 DIR *pD = opendir((char*)pPath);
 if(pD){
  pDir->pDir = pD;
  return 0;
 }
 return errno;
#else
#endif
}

inline
static B Dir_Read(Dir *pDir, Dir_Entry *pEntry){
#ifdef __linux__
 pEntry->pEntry = readdir(pDir->pDir);
 return pEntry->pEntry? 1: 0;
#else
#endif
}

inline
static void Dir_Close(Dir *pDir){
#ifdef __linux__
 closedir(pDir->pDir);
#else
#endif
}

inline
static const C *Dir_Entry_Name(const Dir_Entry *pEntry){
#ifdef __linux__
 return (C*)pEntry->pEntry->d_name;
#else
#endif
}

#endif //DIR_H
