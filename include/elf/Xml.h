#ifndef XML_H
#define XML_H
//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

//Multi-bytes character in "Name" unsupported

#include "String.h"

//Api
static inline Bool Xml_Parse(void *pContext, Char *pBegin, Char *pEnd);

//You MUST define follow event processing functions:
static inline void Xml_onProcessingInstruction(void *pContext, Char *pBegin, Char *pEnd);
static inline void Xml_onStartTag(void *pContext, Char *pBegin, Char *pEnd);
static inline void Xml_onEndTag(void *pContext, Char *pBegin, Char *pEnd);
static inline void Xml_onAtrributeName(void *pContext,  Char *pTagName, Char *pTagNameEnd, Char *pBegin, Char *pEnd);
static inline void Xml_onAtrributeValue(void *pContext,  Char *pTagName, Char *pTagNameEnd, Char *pBegin, Char *pEnd);
static inline void Xml_onError(void *pContext, Char *pBegin, Char *pEnd, Char *pPosition);

//Internal functions
static inline Char *Xml_ParseTag(void *pContext, Char *pBegin, Char *pEnd);
static inline Char *Xml_ParseStartTag(void *pContext, Char *pBegin, Char *pEnd);
static inline Char *Xml_ParseAttribute(void *pContext,  Char *pTagName, Char *pTagNameEnd, Char *pBegin, Char *pEnd);
static inline Char *Xml_ParseEndTag(void *pContext, Char *pBegin, Char *pEnd);
static inline Char *Xml_ParseProcessingInstruction(void *pContext, Char *pBegin, Char *pEnd);
static inline Bool Xml_IsWhiteSpace(Char c);
static inline Char *Xml_SkipWhiteSpace(Char *p);
static inline Bool Xml_IsNameStartChar(Char c);
static inline Bool Xml_IsNameChar(Char c);
static inline Char *Xml_SkipName(Char *p);

static inline Bool Xml_Parse(void *pContext, Char *pBegin, Char *pEnd){
    Char *p = pBegin;
    *pEnd = '"';
    while((p = Xml_SkipWhiteSpace(p)) != pEnd){
        if(*p != '<'){
            Xml_onError(pContext, pBegin, pEnd, p);
            return false;
        }

        p = Xml_ParseTag(pContext, p, pEnd);
        if(p == null)
            return false;
    }
    return true;
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

static inline Char *Xml_ParseTag(void *pContext, Char *pBegin, Char *pEnd){
    Char *p = pBegin + 1;
    switch(*p){
    case '/':
        return Xml_ParseEndTag(pContext, pBegin, pEnd);
    case '?':
        return Xml_ParseProcessingInstruction(pContext, pBegin, pEnd);
CASE_XML_NAME_START_CHAR:
        return Xml_ParseStartTag(pContext, pBegin, pEnd);
    }
    Xml_onError(pContext, pBegin, pEnd, p);
    return null;
}

static inline Char *Xml_ParseProcessingInstruction(void *pContext, Char *pBegin, Char *pEnd){
    Char cOld = *pEnd;
    *pEnd = '?';
    Char *p = String_SkipUntil(pBegin + 2, '?');
    if(p == pEnd){
        Xml_onError(pContext, pBegin, pEnd, pBegin + 2);
        return null;
    }
    *pEnd = cOld;
    ++p;

    if(*p != '>'){
        Xml_onError(pContext, pBegin, pEnd, p);
        return null;
    }

    Xml_onProcessingInstruction(pContext, pBegin, ++p);
    return p;
}

static inline Char *Xml_ParseStartTag(void *pContext, Char *pBegin, Char *pEnd){
    Char *pName = pBegin + 1;
    Char *pNameEnd = Xml_SkipName(pName + 1);
    Xml_onStartTag(pContext, pName, pNameEnd);

    Char *p = pNameEnd;
    while(Xml_IsWhiteSpace(*p)){
        switch(*++p){
        case '/':
            goto EMTY_CONTENT;
        case '>':
            return ++p;
CASE_XML_NAME_START_CHAR:
            p = Xml_ParseAttribute(pContext, pName, pNameEnd, p, pEnd);
            break;
        default:
            Xml_onError(pContext, pBegin, pEnd, p);
            return null;
        }
    }

    if(*p == '/'){
EMTY_CONTENT:
        ++p;
        Xml_onEndTag(pContext, pName, pNameEnd);
    }

    if(*p != '>'){
        Xml_onError(pContext, pBegin, pEnd, p);
        return null;
    }

    return ++p;
}

static inline Char *Xml_ParseAttribute(void *pContext,  Char *pTagName, Char *pTagNameEnd, Char *pBegin, Char *pEnd){
    Char *pName = pBegin;
    Char *pNameEnd = Xml_SkipName(pName + 1);
    Xml_onAtrributeName(pContext, pTagName, pTagNameEnd, pName, pNameEnd);

    Char *p = pNameEnd;
    if(*p != '='){
        Xml_onError(pContext, pBegin, pEnd, p);
        return null;
    }
    ++p;

    if(*p != '"' || p == pEnd){
        Xml_onError(pContext, pBegin, pEnd, p);
        return null;
    }
    ++p;
    Char *pValue = p;
    p = String_SkipUntil(p, '"');
    if(p == pEnd)
        return null;

    Xml_onAtrributeValue(pContext, pTagName, pTagNameEnd, pValue, p);
    return ++p;
}

static inline Char *Xml_ParseEndTag(void *pContext, Char *pBegin, Char *pEnd){
    Char *pName = pBegin + 2;
    if(!Xml_IsNameStartChar(*pName)){
        Xml_onError(pContext, pBegin, pEnd, pName);
        return null;
    }
    Char *pNameEnd = Xml_SkipName(pName + 1);
    Xml_onEndTag(pContext, pName, pNameEnd);

    Char *p = pNameEnd;
    if(Xml_IsWhiteSpace(*p))
        ++p;
    return *p != '>'? null : ++p;
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

