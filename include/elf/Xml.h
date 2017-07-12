#ifndef XML_H
#define XML_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

//Multi-bytes character of "Name" unsupported

#include "String.h"

//Api
#ifdef XML_SINGLE_QUOTE_MARK
    #define XML_QUOTE_MARK '\''
#else
    #define XML_QUOTE_MARK '"'
#endif

enum XmlResult{
    xrOk, xrExpectLess, xrExpectGreater, xrExpectEndTagName, xrExpectAssign, xrExpectStartQuote, xrExpectEndQuote,
    xrInvalidCharAfterLess, xrExpectInterrogation,
    xrInvalidCharAfterWhiteSpace
};

typedef void (*Xml_Handler)(void *pContext, C *pBegin, C *pEnd);
typedef void (*XmlAttribute_Handler)(void *pContext, C *pName, C *pNameEnd, C *pValue, C *pValueEnd);

//pEnd must be writable
static inline XmlResult Xml_Parse(void *pContext, C **ppBegin, C *pEnd,
                                  Xml_Handler onProcessingInstruction, Xml_Handler onStartTag,
                                  XmlAttribute_Handler onAttribute,
                                  Xml_Handler onEndTag, Xml_Handler onContent);

//Internal functions
static inline XmlResult Xml_ParseTag(void *pContext, C **ppBegin, C *pEnd,
                                     Xml_Handler onProcessingInstruction, Xml_Handler onStartTag,
                                     XmlAttribute_Handler onAttribute, Xml_Handler onEndTag);
static inline XmlResult Xml_ParseStartTag(void *pContext, C **ppBegin, C *pEnd,
                                          Xml_Handler onStartTag, XmlAttribute_Handler onAttribute, Xml_Handler onEndTag);
static inline XmlResult Xml_ParseAttribute(void *pContext, C **ppBegin, C *pEnd,
                                           XmlAttribute_Handler onAttribute);
static inline XmlResult Xml_ParseEndTag(void *pContext, C **ppBegin, C *pEnd, Xml_Handler onEndTag);
static inline XmlResult Xml_ParseProcessingInstruction(void *pContext, C **ppBegin, C *pEnd, Xml_Handler onProcessingInstruction);
static inline B Xml_IsWhiteSpace(C c);
static inline C *Xml_SkipWhiteSpace(C *p);
static inline B Xml_IsNameStartChar(C c);
static inline B Xml_IsNameChar(C c);
static inline C *Xml_SkipName(C *p);

static inline XmlResult Xml_Parse(void *pContext, C **ppBegin, C *pEnd,
                                  Xml_Handler onProcessingInstruction, Xml_Handler onStartTag,
                                  XmlAttribute_Handler onAttribute,
                                  Xml_Handler onEndTag, Xml_Handler onContent){
    XmlResult r = xrOk;

    C cOld = *pEnd;
    *pEnd = '"';

    C *pContent;
    C *p = *ppBegin;
    while((p = Xml_SkipWhiteSpace(p)) != pEnd){
        if(*p != '<'){
            pContent = p;
            *pEnd = '<';
            p = String_SkipUntil(p, '<');
            *pEnd = '"';
            onContent(pContext, pContent, p);
            if(p == pEnd)
                break;
        }

        r = Xml_ParseTag(pContext, &p, pEnd, onProcessingInstruction, onStartTag,
                         onAttribute, onEndTag);
        if(r != xrOk)
            break;
    }

    *pEnd = cOld;
    *ppBegin = p;
    return r;
}

static inline B Xml_IsWhiteSpace(C c){
    switch(c){
    case 0x09:case 0x0A:case 0x0D:case 0x20:
        return true;
    }
    return false;
}

static inline C *Xml_SkipWhiteSpace(C *p){
    while(Xml_IsWhiteSpace(*p))
        ++p;
    return p;
}

#define CASE_XML_NAME_START_CHAR \
    case ':':  CASE_CHAR_UPPER: case '_': CASE_CHAR_LOWER: \
    case 0xC0: case 0xC1: case 0xC2: case 0xC3: case 0xC4: case 0xC5: case 0xC6: case 0xC7: \
    case 0xC8: case 0xC9: case 0xCA: case 0xCB: case 0xCC: case 0xCD: case 0xCE: case 0xCF: \
    case 0xD0: case 0xD1: case 0xD2: case 0xD3: case 0xD4: case 0xD5: case 0xD6: \
    case 0xD8: case 0xD9: case 0xDA: case 0xDB: case 0xDC: case 0xDD: case 0xDE: case 0xDF: \
    case 0xE0: case 0xE1: case 0xE2: case 0xE3: case 0xE4: case 0xE5: case 0xE6: case 0xE7: \
    case 0xE8: case 0xE9: case 0xEA: case 0xEB: case 0xEC: case 0xED: case 0xEE: case 0xEF: \
    case 0xF0: case 0xF1: case 0xF2: case 0xF3: case 0xF4: case 0xF5: case 0xF6: \
    case 0xF8: case 0xF9: case 0xFA: case 0xFB: case 0xFC: case 0xFD: case 0xFE: case 0xFF

static inline XmlResult Xml_ParseTag(void *pContext, C **ppBegin, C *pEnd,
                                     Xml_Handler onProcessingInstruction, Xml_Handler onStartTag,
                                     XmlAttribute_Handler onAttribute, Xml_Handler onEndTag){
    C *p = *ppBegin + 1;
    switch(*p){
    case '/':
        return Xml_ParseEndTag(pContext, ppBegin, pEnd, onEndTag);
    case '?':
        return Xml_ParseProcessingInstruction(pContext, ppBegin, pEnd, onProcessingInstruction);
CASE_XML_NAME_START_CHAR:
        return Xml_ParseStartTag(pContext, ppBegin, pEnd, onStartTag, onAttribute, onEndTag);
    }

    return xrInvalidCharAfterLess;
}

static inline XmlResult Xml_ParseProcessingInstruction(void *pContext, C **ppBegin, C *pEnd, Xml_Handler onProcessingInstruction){
    C cOld = *pEnd;
    *pEnd = '?';
    C *p = String_SkipUntil(*ppBegin + 2, '?');
    if(p == pEnd){
        *ppBegin += 2;
        return xrExpectInterrogation;
    }
    *pEnd = cOld;
    ++p;

    if(*p != '>'){
        *ppBegin = p;
        return xrExpectGreater;
    }

    onProcessingInstruction(pContext, *ppBegin, ++p);
    *ppBegin = p;
    return xrOk;
}

static inline XmlResult Xml_ParseStartTag(void *pContext, C **ppBegin, C *pEnd,
                                          Xml_Handler onStartTag, XmlAttribute_Handler onAttribute, Xml_Handler onEndTag){
    XmlResult r = xrOk;

    C *pName = *ppBegin + 1;
    C *pNameEnd = Xml_SkipName(pName + 1);
    onStartTag(pContext, pName, pNameEnd);

    C *p = pNameEnd;
    while(Xml_IsWhiteSpace(*p)){
        switch(*++p){
        case '/':
            goto EMTY_CONTENT;
        case '>':
            *ppBegin = p + 1;
            return xrOk;
CASE_XML_NAME_START_CHAR:
            r = Xml_ParseAttribute(pContext, &p, pEnd, onAttribute);
            if(r != xrOk){
                *ppBegin = p;
                return r;
            }
            break;
        default:
            *ppBegin = p;
            return xrInvalidCharAfterWhiteSpace;
        }
    }

    if(*p == '/'){
EMTY_CONTENT:
        ++p;
        onEndTag(pContext, pName, pNameEnd);
    }

    if(*p != '>'){
        *ppBegin = p;
        return xrExpectGreater;
    }

    *ppBegin = p + 1;
    return xrOk;
}

static inline XmlResult Xml_ParseAttribute(void *pContext, C **ppBegin, C *pEnd, XmlAttribute_Handler onAttribute){
    C *pName = *ppBegin;
    C *pNameEnd = Xml_SkipName(pName + 1);
    C *p = pNameEnd;

    if(*p != '='){
        *ppBegin = p;
        return xrExpectAssign;
    }
    ++p;

    if(*p != XML_QUOTE_MARK || p == pEnd){
        *ppBegin = p;
        return xrExpectStartQuote;
    }
    ++p;
    C *pValue = p;
    p = String_SkipUntil(p, XML_QUOTE_MARK);
    if(p == pEnd){
        *ppBegin = p;
        return xrExpectEndQuote;
    }

    onAttribute(pContext, pName, pNameEnd, pValue, p);
    *ppBegin = p + 1;
    return xrOk;
}

static inline XmlResult Xml_ParseEndTag(void *pContext, C **ppBegin, C *pEnd, Xml_Handler onEndTag){
    C *pName = *ppBegin + 2;
    if(!Xml_IsNameStartChar(*pName)){
        *ppBegin = pName;
        return xrExpectEndTagName;
    }
    C *pNameEnd = Xml_SkipName(pName + 1);
    onEndTag(pContext, pName, pNameEnd);

    C *p = pNameEnd;
    if(Xml_IsWhiteSpace(*p))
        ++p;

    if(*p != '>'){
        *ppBegin = p;
        return xrExpectGreater;
    }

    *ppBegin = p + 1;
    return xrOk;
}

#define CASE_XML_NAME_CHAR \
    CASE_XML_NAME_START_CHAR: case '-': case '.': CASE_CHAR_DIGIT: case 0xB7

static inline B Xml_IsNameStartChar(C c){
    switch(c){
CASE_XML_NAME_START_CHAR:
    return true;
    }
    return false;
}

static inline B Xml_IsNameChar(C c){
    switch(c){
CASE_XML_NAME_CHAR:
    return true;
    }
    return false;
}

static inline C *Xml_SkipName(C *p){
    while(Xml_IsNameChar(*p))
        ++p;
    return p;
}

#endif // XML_H

