#ifndef BINARYSEARCH_H
#define BINARYSEARCH_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

/*
Find key in ascending sorted array
*/

#include "Type.h"

inline
static U8 *U8s_Exists(U32 uSize, U8 *pBegin, U8 *pEnd, U8 uKey){
  U8 *p;
  U8 uValue;
  while(uSize){
    uValue = *(p = pBegin + (uSize >>= 1));
    if(uValue < uKey){
      pBegin = ++p;
      uSize = pEnd - pBegin;
    }
    else if(uValue != uKey)
      pEnd = p;
    else
      return p;
  }
  return 0;
}

inline
static B U8s_Find(U32 uSize, U8 *pBegin, U8 *pEnd, U8 uKey, U8 **ppPostion){
  U8 *p;
  U8 uValue;
  while(uSize){
    uValue = *(p = pBegin + (uSize >>= 1));
    if(uValue < uKey){
      pBegin = ++p;
      uSize = pEnd - pBegin;
    }
    else if(uValue != uKey)
      pEnd = p;
    else{
      *ppPostion = p;
      return 1;
    }
  }
  *ppPostion = pBegin;
  return 0;
}

inline
static U16 *U16s_Exists(U32 uSize, U16 *pBegin, U16 *pEnd, U16 uKey){
  U16 *p;
  U16 uValue;
  while(uSize){
    uValue = *(p = pBegin + (uSize >>= 1));
    if(uValue < uKey){
      pBegin = ++p;
      uSize = pEnd - pBegin;
    }
    else if(uValue != uKey)
      pEnd = p;
    else
      return p;
  }
  return 0;
}

inline
static B U16s_Find(U32 uSize, U16 *pBegin, U16 *pEnd, U16 uKey, U16 **ppPostion){
  U16 *p;
  U16 uValue;
  while(uSize){
    uValue = *(p = pBegin + (uSize >>= 1));
    if(uValue < uKey){
      pBegin = ++p;
      uSize = pEnd - pBegin;
    }
    else if(uValue != uKey)
      pEnd = p;
    else{
      *ppPostion = p;
      return 1;
    }
  }
  *ppPostion = pBegin;
  return 0;
}

inline
static U32 *U32s_Exists(U32 uSize, U32 *pBegin, U32 *pEnd, U32 uKey){
  U32 *p;
  U32 uValue;
  while(uSize){
    uValue = *(p = pBegin + (uSize >>= 1));
    if(uValue < uKey){
      pBegin = ++p;
      uSize = pEnd - pBegin;
    }
    else if(uValue != uKey)
      pEnd = p;
    else
      return p;
  }
  return 0;
}

inline
static B U32s_Find(U32 uSize, U32 *pBegin, U32 *pEnd, U32 uKey, U32 **ppPostion){
  U32 *p;
  U32 uValue;
  while(uSize){
    uValue = *(p = pBegin + (uSize >>= 1));
    if(uValue < uKey){
      pBegin = ++p;
      uSize = pEnd - pBegin;
    }
    else if(uValue != uKey)
      pEnd = p;
    else{
      *ppPostion = p;
      return 1;
    }
  }
  *ppPostion = pBegin;
  return 0;
}

inline
static U64 *U64s_Exists(U32 uSize, U64 *pBegin, U64 *pEnd, U64 uKey){
  U64 *p;
  U64 uValue;
  while(uSize){
    uValue = *(p = pBegin + (uSize >>= 1));
    if(uValue < uKey){
      pBegin = ++p;
      uSize = pEnd - pBegin;
    }
    else if(uValue != uKey)
      pEnd = p;
    else
      return p;
  }
  return 0;
}

inline
static B U64s_Find(U32 uSize, U64 *pBegin, U64 *pEnd, U64 uKey, U64 **ppPostion){
  U64 *p;
  U64 uValue;
  while(uSize){
    uValue = *(p = pBegin + (uSize >>= 1));
    if(uValue < uKey){
      pBegin = ++p;
      uSize = pEnd - pBegin;
    }
    else if(uValue != uKey)
      pEnd = p;
    else{
      *ppPostion = p;
      return 1;
    }
  }
  *ppPostion = pBegin;
  return 0;
}

inline
static I8 *I8s_Exists(U32 uSize, I8 *pBegin, I8 *pEnd, I8 iKey){
  I8 *p;
  I8 iValue;
  while(uSize){
    iValue = *(p = pBegin + (uSize >>= 1));
    if(iValue < iKey){
      pBegin = ++p;
      uSize = pEnd - pBegin;
    }
    else if(iValue != iKey)
      pEnd = p;
    else
      return p;
  }
  return 0;
}

inline
static B I8s_Find(U32 uSize, I8 *pBegin, I8 *pEnd, I8 iKey, I8 **ppPostion){
  I8 *p;
  I8 iValue;
  while(uSize){
    iValue = *(p = pBegin + (uSize >>= 1));
    if(iValue < iKey){
      pBegin = ++p;
      uSize = pEnd - pBegin;
    }
    else if(iValue != iKey)
      pEnd = p;
    else{
      *ppPostion = p;
      return 1;
    }
  }
  *ppPostion = pBegin;
  return 0;
}

inline
static I16 *I16s_Exists(U32 uSize, I16 *pBegin, I16 *pEnd, I16 iKey){
  I16 *p;
  I16 iValue;
  while(uSize){
    iValue = *(p = pBegin + (uSize >>= 1));
    if(iValue < iKey){
      pBegin = ++p;
      uSize = pEnd - pBegin;
    }
    else if(iValue != iKey)
      pEnd = p;
    else
      return p;
  }
  return 0;
}

inline
static B I16s_Find(U32 uSize, I16 *pBegin, I16 *pEnd, I16 iKey, I16 **ppPostion){
  I16 *p;
  I16 iValue;
  while(uSize){
    iValue = *(p = pBegin + (uSize >>= 1));
    if(iValue < iKey){
      pBegin = ++p;
      uSize = pEnd - pBegin;
    }
    else if(iValue != iKey)
      pEnd = p;
    else{
      *ppPostion = p;
      return 1;
    }
  }
  *ppPostion = pBegin;
  return 0;
}

inline
static I32 *I32s_Exists(U32 uSize, I32 *pBegin, I32 *pEnd, I32 iKey){
  I32 *p;
  I32 iValue;
  while(uSize){
    iValue = *(p = pBegin + (uSize >>= 1));
    if(iValue < iKey){
      pBegin = ++p;
      uSize = pEnd - pBegin;
    }
    else if(iValue != iKey)
      pEnd = p;
    else
      return p;
  }
  return 0;
}

inline
static B I32s_Find(U32 uSize, I32 *pBegin, I32 *pEnd, I32 iKey, I32 **ppPostion){
  I32 *p;
  I32 iValue;
  while(uSize){
    iValue = *(p = pBegin + (uSize >>= 1));
    if(iValue < iKey){
      pBegin = ++p;
      uSize = pEnd - pBegin;
    }
    else if(iValue != iKey)
      pEnd = p;
    else{
      *ppPostion = p;
      return 1;
    }
  }
  *ppPostion = pBegin;
  return 0;
}

inline
static I64 *I64s_Exists(U32 uSize, I64 *pBegin, I64 *pEnd, I64 iKey){
  I64 *p;
  I64 iValue;
  while(uSize){
    iValue = *(p = pBegin + (uSize >>= 1));
    if(iValue < iKey){
      pBegin = ++p;
      uSize = pEnd - pBegin;
    }
    else if(iValue != iKey)
      pEnd = p;
    else
      return p;
  }
  return 0;
}

inline
static B I64s_Find(U32 uSize, I64 *pBegin, I64 *pEnd, I64 iKey, I64 **ppPostion){
  I64 *p;
  I64 iValue;
  while(uSize){
    iValue = *(p = pBegin + (uSize >>= 1));
    if(iValue < iKey){
      pBegin = ++p;
      uSize = pEnd - pBegin;
    }
    else if(iValue != iKey)
      pEnd = p;
    else{
      *ppPostion = p;
      return 1;
    }
  }
  *ppPostion = pBegin;
  return 0;
}

inline
static F32 *F32s_Exists(U32 uSize, F32 *pBegin, F32 *pEnd, F32 fKey){
  F32 *p;
  F32 fValue;
  while(uSize){
    fValue = *(p = pBegin + (uSize >>= 1));
    if(fValue < fKey){
      pBegin = ++p;
      uSize = pEnd - pBegin;
    }
    else if(fValue != fKey)
      pEnd = p;
    else
      return p;
  }
  return 0;
}

inline
static B F32s_Find(U32 uSize, F32 *pBegin, F32 *pEnd, F32 fKey, F32 **ppPostion){
  F32 *p;
  F32 fValue;
  while(uSize){
    fValue = *(p = pBegin + (uSize >>= 1));
    if(fValue < fKey){
      pBegin = ++p;
      uSize = pEnd - pBegin;
    }
    else if(fValue != fKey)
      pEnd = p;
    else{
      *ppPostion = p;
      return 1;
    }
  }
  *ppPostion = pBegin;
  return 0;
}

inline
static F64 *F64s_Exists(U32 uSize, F64 *pBegin, F64 *pEnd, F64 fKey){
  F64 *p;
  F64 fValue;
  while(uSize){
    fValue = *(p = pBegin + (uSize >>= 1));
    if(fValue < fKey){
      pBegin = ++p;
      uSize = pEnd - pBegin;
    }
    else if(fValue != fKey)
      pEnd = p;
    else
      return p;
  }
  return 0;
}

inline
static B F64s_Find(U32 uSize, F64 *pBegin, F64 *pEnd, F64 fKey, F64 **ppPostion){
  F64 *p;
  F64 fValue;
  while(uSize){
    fValue = *(p = pBegin + (uSize >>= 1));
    if(fValue < fKey){
      pBegin = ++p;
      uSize = pEnd - pBegin;
    }
    else if(fValue != fKey)
      pEnd = p;
    else{
      *ppPostion = p;
      return 1;
    }
  }
  *ppPostion = pBegin;
  return 0;
}
#endif //BINARYSEARCH_H
