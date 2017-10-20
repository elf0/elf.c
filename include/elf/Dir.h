#ifndef DIR_H
#define DIR_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

#ifdef __linux__
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
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

typedef E8 (*Dir_Entry_Handler)(void *pContext, const C *pName);

inline
static E8 Dir_Create(const C *pPath) {
#ifdef __linux__
  if(mkdir((char*)pPath, 0744) != 0)
    return errno;

  return 0;
#else
#endif
}

inline
static E8 Path_Prepare(const C *pPath) {
#ifdef __linux__
  E8 e = Dir_Create(pPath);
  if(e && e != EEXIST){
    if(e != ENOENT)
      return e;

    // Skip root '/'
    C *p = (C*)pPath + 1;
    while(*p){
      if(*p == '/'){
        *p = 0;
        e = Dir_Create(pPath);
        *p = '/';
        if(e && e != EEXIST)
          return e;
      }
      ++p;
    }
    return Dir_Create(pPath);
  }
  return 0;
#else
#endif
}

inline
static E8 Dir_Remove(const C *pPath) {
#ifdef __linux__
  return rmdir((char*)pPath) != 0;
#else
#endif
}

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
static E8 Dir_List(Dir *pDir, Dir_Entry_Handler handler, void *pContext){
  E8 e;
#ifdef __linux__
  DIR *pDIR = pDir->pDir;
  struct dirent *pEntry;
  while((pEntry = readdir(pDIR))){
    e = handler(pContext, (C*)pEntry->d_name);
    if(e)
      return e;
  }
#else
#endif
  return 0;
}

inline
static E8 Dir_Files(Dir *pDir, Dir_Entry_Handler handler, void *pContext){
  E8 e;
#ifdef __linux__
  DIR *pDIR = pDir->pDir;
  struct dirent *pEntry;
  while((pEntry = readdir(pDIR))){
    if(pEntry->d_type == DT_REG){
      e = handler(pContext, (C*)pEntry->d_name);
      if(e)
        return e;
    }
  }
#else
#endif
  return 0;
}

inline
static E8 Dir_Dirs(Dir *pDir, Dir_Entry_Handler handler, void *pContext){
  E8 e;
#ifdef __linux__
  DIR *pDIR = pDir->pDir;
  struct dirent *pEntry;
  while((pEntry = readdir(pDIR))){
    if(pEntry->d_type == DT_DIR){
      e = handler(pContext, (C*)pEntry->d_name);
      if(e)
        return e;
    }
  }
#else
#endif
  return 0;
}

inline
static E8 Dir_Links(Dir *pDir, Dir_Entry_Handler handler, void *pContext){
  E8 e;
#ifdef __linux__
  DIR *pDIR = pDir->pDir;
  struct dirent *pEntry;
  while((pEntry = readdir(pDIR))){
    if(pEntry->d_type == DT_LNK){
      e = handler(pContext, (C*)pEntry->d_name);
      if(e)
        return e;
    }
  }
#else
#endif
  return 0;
}

inline
static const C *Dir_Entry_Name(const Dir_Entry *pEntry){
#ifdef __linux__
  return (C*)pEntry->pEntry->d_name;
#else
#endif
}

#endif //DIR_H
