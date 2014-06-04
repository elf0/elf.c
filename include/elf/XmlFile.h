#ifndef XMLFILE_H
#define XMLFILE_H
//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "Xml.h"

//Api
int XmlFile_Parse(void *pContext, const char *pszFileName){
    int fd = open(pszFileName, O_RDONLY);
    if(fd == -1)
        return -1;

    struct stat st;
    if(fstat(fd, &st) == -1){
        close(fd);
        return -2;
    }

    Byte *pBegin = (Byte*)mmap(0, st.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if(pBegin == MAP_FAILED){
        close(fd);
        return -3;
    }

    if(!Xml_Parse(pContext, pBegin, pBegin + st.st_size)){
        munmap(pBegin, st.st_size);
        close(fd);
        return false;
    }

    munmap(pBegin, st.st_size);
    close(fd);
    return 0;
}

#endif // XMLFILE_H
