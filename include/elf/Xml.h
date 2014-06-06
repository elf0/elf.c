#ifndef XML_H
#define XML_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

//Multi-bytes character of "Name" unsupported

#include "String.h"

enum XmlResult{
    xrOk, xrExpectLess, xrExpectGreater, xrExpectEndTagName, xrExpectAssign, xrExpectStartQuote, xrExpectEndQuote,
    xrInvalidCharAfterLess, xrExpectInterrogation,
    xrInvalidCharAfterWhiteSpace
};

//Api
//pEnd must be writable
static inline XmlResult Xml_Parse(void *pContext, Char **ppBegin, Char *pEnd);

//You MUST define follow event processing functions:
static inline void Xml_onProcessingInstruction(void *pContext, Char *pBegin, Char *pEnd);
static inline void Xml_onStartTag(void *pContext, Char *pBegin, Char *pEnd);
static inline void Xml_onEndTag(void *pContext, Char *pBegin, Char *pEnd);
static inline void Xml_onAtrribute(void *pContext, Char *pTagName, Char *pTagNameEnd, Char *pName, Char *pNameEnd, Char *pValue, Char *pValueEnd);

//Internal functions
static inline XmlResult Xml_ParseTag(void *pContext, Char **ppBegin, Char *pEnd);
static inline XmlResult Xml_ParseStartTag(void *pContext, Char **ppBegin, Char *pEnd);
static inline XmlResult Xml_ParseAttribute(void *pContext,  Char *pTagName, Char *pTagNameEnd, Char **ppBegin, Char *pEnd);
static inline XmlResult Xml_ParseEndTag(void *pContext, Char **ppBegin, Char *pEnd);
static inline XmlResult Xml_ParseProcessingInstruction(void *pContext, Char **ppBegin, Char *pEnd);
static inline Bool Xml_IsWhiteSpace(Char c);
static inline Char *Xml_SkipWhiteSpace(Char *p);
static inline Bool Xml_IsNameStartChar(Char c);
static inline Bool Xml_IsNameChar(Char c);
static inline Char *Xml_SkipName(Char *p);

static inline XmlResult Xml_Parse(void *pContext, Char **ppBegin, Char *pEnd){
    XmlResult r = xrOk;

    Char cOld = *pEnd;
    *pEnd = '"';

    Char *p = *ppBegin;
    while((p = Xml_SkipWhiteSpace(p)) != pEnd){
        if(*p != '<'){
            r = xrExpectLess;
            break;
        }

        r = Xml_ParseTag(pContext, &p, pEnd);
        if(r != xrOk)
            break;
    }

    *pEnd = cOld;
    *ppBegin = p;
    return r;
}

static inline Bool Xml_IsWhiteSpace(Char c){
    switch(c){
    case 0x09:case 0x0A:case 0x0D:case 0x20:
        return true;
    }
    return false;
}

static inline Char *Xml_SkipWhiteSpace(Char *p){
    while(Xml_IsWhiteSpace(*p))
        ++p;
    return p;
}

//	NameStartChar	   ::=   	":" | [A-Z] | "_" | [a-z] | [#xC0-#xD6] | [#xD8-#xF6]
//    | [#xF8-#x2FF] | [#x370-#x37D] | [#x37F-#x1FFF] | [#x200C-#x200D]
//    | [#x2070-#x218F] | [#x2C00-#x2FEF] | [#x3001-#xD7FF] | [#xF900-#xFDCF]
//    | [#xFDF0-#xFFFD] | [#x10000-#xEFFFF]

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

static inline XmlResult Xml_ParseTag(void *pContext, Char **ppBegin, Char *pEnd){
    Char *p = *ppBegin + 1;
    switch(*p){
    case '/':
        return Xml_ParseEndTag(pContext, ppBegin, pEnd);
    case '?':
        return Xml_ParseProcessingInstruction(pContext, ppBegin, pEnd);
CASE_XML_NAME_START_CHAR:
        return Xml_ParseStartTag(pContext, ppBegin, pEnd);
    }

    return xrInvalidCharAfterLess;
}

static inline XmlResult Xml_ParseProcessingInstruction(void *pContext, Char **ppBegin, Char *pEnd){
    Char cOld = *pEnd;
    *pEnd = '?';
    Char *p = String_SkipUntil(*ppBegin + 2, '?');
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

    Xml_onProcessingInstruction(pContext, *ppBegin, ++p);
    *ppBegin = p;
    return xrOk;
}

static inline XmlResult Xml_ParseStartTag(void *pContext, Char **ppBegin, Char *pEnd){
    XmlResult r = xrOk;

    Char *pName = *ppBegin + 1;
    Char *pNameEnd = Xml_SkipName(pName + 1);
    Xml_onStartTag(pContext, pName, pNameEnd);

    Char *p = pNameEnd;
    while(Xml_IsWhiteSpace(*p)){
        switch(*++p){
        case '/':
            goto EMTY_CONTENT;
        case '>':
            *ppBegin = p + 1;
            return xrOk;
CASE_XML_NAME_START_CHAR:
            r = Xml_ParseAttribute(pContext, pName, pNameEnd, &p, pEnd);
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
        Xml_onEndTag(pContext, pName, pNameEnd);
    }

    if(*p != '>'){
        *ppBegin = p;
        return xrExpectGreater;
    }

    *ppBegin = p + 1;
    return xrOk;
}

static inline XmlResult Xml_ParseAttribute(void *pContext,  Char *pTagName, Char *pTagNameEnd, Char **ppBegin, Char *pEnd){
    Char *pName = *ppBegin;
    Char *pNameEnd = Xml_SkipName(pName + 1);

    Char *p = pNameEnd;
    if(*p != '='){
        *ppBegin = p;
        return xrExpectAssign;
    }
    ++p;

    if(*p != '"' || p == pEnd){
        *ppBegin = p;
        return xrExpectStartQuote;
    }
    ++p;
    Char *pValue = p;
    p = String_SkipUntil(p, '"');
    if(p == pEnd){
        *ppBegin = p;
        return xrExpectEndQuote;
    }

    Xml_onAtrribute(pContext, pTagName, pTagNameEnd, pName, pNameEnd, pValue, p);
    *ppBegin = p + 1;
    return xrOk;
}

static inline XmlResult Xml_ParseEndTag(void *pContext, Char **ppBegin, Char *pEnd){
    Char *pName = *ppBegin + 2;
    if(!Xml_IsNameStartChar(*pName)){
        *ppBegin = pName;
        return xrExpectEndTagName;
    }
    Char *pNameEnd = Xml_SkipName(pName + 1);
    Xml_onEndTag(pContext, pName, pNameEnd);

    Char *p = pNameEnd;
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

static inline Bool Xml_IsNameStartChar(Char c){
    switch(c){
CASE_XML_NAME_START_CHAR:
    return true;
    }
    return false;
}

static inline Bool Xml_IsNameChar(Char c){
    switch(c){
CASE_XML_NAME_CHAR:
    return true;
    }
    return false;
}

static inline Char *Xml_SkipName(Char *p){
    while(Xml_IsNameChar(*p))
        ++p;
    return p;
}

#endif // XML_H

