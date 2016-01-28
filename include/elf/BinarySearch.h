#ifndef BINARYSEARCH_H
#define BINARYSEARCH_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

//All uValues must not be 0!

Bool U8Array_BinarySearch(U8 *pBegin, U8 *pEnd, U8 uValue, U8 **ppPosition){
  U8 *pB = pBegin, *pE = pEnd, *pMid;
  U32 i;
  while(i = pE - pB >> 1){
    pMid = pB + i;
    if(uValue > *pMid)
      pB = pMid;
    else if(uValue < *pMid)
      pE = pMid;
    else{
      *ppPosition = pMid;
      return true;
    }
  }

  *ppPosition = pB;
  return uValue == *pB;
}

Bool U16Array_BinarySearch(U16 *pBegin, U16 *pEnd, U16 uValue, U16 **ppPosition){
  U16 *pB = pBegin, *pE = pEnd, *pMid;
  U32 i;
  while(i = pE - pB >> 1){
    pMid = pB + i;
    if(uValue > *pMid)
      pB = pMid;
    else if(uValue < *pMid)
      pE = pMid;
    else{
      *ppPosition = pMid;
      return true;
    }
  }

  *ppPosition = pB;
  return uValue == *pB;
}

Bool U32Array_BinarySearch(U32 *pBegin, U32 *pEnd, U32 uValue, U32 **ppPosition){
  U32 *pB = pBegin, *pE = pEnd, *pMid;
  U32 i;
  while(i = pE - pB >> 1){
    pMid = pB + i;
    if(uValue > *pMid)
      pB = pMid;
    else if(uValue < *pMid)
      pE = pMid;
    else{
      *ppPosition = pMid;
      return true;
    }
  }

  *ppPosition = pB;
  return uValue == *pB;
}

Bool U64Array_BinarySearch(U64 *pBegin, U64 *pEnd, U64 uValue, U64 **ppPosition){
  U64 *pB = pBegin, *pE = pEnd, *pMid;
  U32 i;
  while(i = pE - pB >> 1){
    pMid = pB + i;
    if(uValue > *pMid)
      pB = pMid;
    else if(uValue < *pMid)
      pE = pMid;
    else{
      *ppPosition = pMid;
      return true;
    }
  }

  *ppPosition = pB;
  return uValue == *pB;
}

Bool I8Array_BinarySearch(I8 *pBegin, I8 *pEnd, I8 uValue, I8 **ppPosition){
  I8 *pB = pBegin, *pE = pEnd, *pMid;
  U32 i;
  while(i = pE - pB >> 1){
    pMid = pB + i;
    if(uValue > *pMid)
      pB = pMid;
    else if(uValue < *pMid)
      pE = pMid;
    else{
      *ppPosition = pMid;
      return true;
    }
  }

  *ppPosition = pB;
  return uValue == *pB;
}

Bool I16Array_BinarySearch(I16 *pBegin, I16 *pEnd, I16 iValue, I16 **ppPosition){
  I16 *pB = pBegin, *pE = pEnd, *pMid;
  U32 i;
  while(i = pE - pB >> 1){
    pMid = pB + i;
    if(iValue > *pMid)
      pB = pMid;
    else if(iValue < *pMid)
      pE = pMid;
    else{
      *ppPosition = pMid;
      return true;
    }
  }

  *ppPosition = pB;
  return iValue == *pB;
}

Bool I32Array_BinarySearch(I32 *pBegin, I32 *pEnd, I32 iValue, I32 **ppPosition){
  I32 *pB = pBegin, *pE = pEnd, *pMid;
  U32 i;
  while(i = pE - pB >> 1){
    pMid = pB + i;
    if(iValue > *pMid)
      pB = pMid;
    else if(iValue < *pMid)
      pE = pMid;
    else{
      *ppPosition = pMid;
      return true;
    }
  }

  *ppPosition = pB;
  return iValue == *pB;
}

Bool I64Array_BinarySearch(I64 *pBegin, I64 *pEnd, I64 iValue, I64 **ppPosition){
  I64 *pB = pBegin, *pE = pEnd, *pMid;
  U32 i;
  while(i = pE - pB >> 1){
    pMid = pB + i;
    if(iValue > *pMid)
      pB = pMid;
    else if(iValue < *pMid)
      pE = pMid;
    else{
      *ppPosition = pMid;
      return true;
    }
  }

  *ppPosition = pB;
  return iValue == *pB;
}

Bool F32Array_BinarySearch(F32 *pBegin, F32 *pEnd, F32 fValue, F32 **ppPosition){
  F32 *pB = pBegin, *pE = pEnd, *pMid;
  U32 i;
  while(i = pE - pB >> 1){
    pMid = pB + i;
    if(fValue > *pMid)
      pB = pMid;
    else if(fValue < *pMid)
      pE = pMid;
    else{
      *ppPosition = pMid;
      return true;
    }
  }

  *ppPosition = pB;
  return fValue == *pB;
}

Bool F64Array_BinarySearch(F64 *pBegin, F64 *pEnd, F64 fValue, F64 **ppPosition){
  F64 *pB = pBegin, *pE = pEnd, *pMid;
  U32 i;
  while(i = pE - pB >> 1){
    pMid = pB + i;
    if(fValue > *pMid)
      pB = pMid;
    else if(fValue < *pMid)
      pE = pMid;
    else{
      *ppPosition = pMid;
      return true;
    }
  }

  *ppPosition = pB;
  return fValue == *pB;
}
#endif //BINARYSEARCH_H
