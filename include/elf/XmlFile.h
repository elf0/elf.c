#ifndef XMLFILE_H
#define XMLFILE_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "Xml.h"

//Api
typedef void (*XmlFile_ErrorHandler)(void *pContext, XmlResult xrError, size_t nOffset);
int XmlFile_Parse(void *pContext, const C *pszFileName, Xml_Handler onProcessingInstruction, Xml_Handler onStartTag,
                  XmlAttribute_Handler onAttribute, Xml_Handler onEndTag, Xml_Handler onContent, XmlFile_ErrorHandler onError){
    int fd = open((const char*)pszFileName, O_RDONLY);
    if(fd == -1)
        return -1;

    struct stat st;
    if(fstat(fd, &st) == -1){
        close(fd);
        return -2;
    }

    if(st.st_size == 0)
        return 0;

    Byte *pBegin = (Byte*)mmap(0, st.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if(pBegin == MAP_FAILED){
        close(fd);
        return -3;
    }

    Byte *pEnd = pBegin + st.st_size - 1;
    C cEnd = *pEnd;

    Byte *p = pBegin;
    XmlResult r = Xml_Parse(pContext, &p, pEnd, onProcessingInstruction, onStartTag, onAttribute, onEndTag, onContent);

    munmap(pBegin, st.st_size);
    close(fd);

    if((r == xrOk || (r == xrExpectGreater && p == pEnd && cEnd == '>')))
        return 0;

    onError(pContext, r, p - pBegin);
    return -4;
}

#endif // XMLFILE_H
