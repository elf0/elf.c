#ifndef BINARYSEARCH_H
#define BINARYSEARCH_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

//All uValues must not be 0!

Bool U8Array_BinarySearch(U8 szValues[], U32 uValues, U8 uValue, U8 **ppPosition){
  U8 *pBegin = szValues, *pEnd = pBegin + uValues, *pMid;
  U32 i;
  while(i = pEnd - pBegin >> 1){
    pMid = pBegin + i;
    if(uValue > *pMid)
      pBegin = pMid;
    else if(uValue < *pMid)
      pEnd = pMid;
    else{
      *ppPosition = pMid;
      return true;
    }
  }

  *ppPosition = pBegin;
  return uValue == *pBegin;
}

Bool U16Array_BinarySearch(U16 szValues[], U32 uValues, U16 uValue, U16 **ppPosition){
  U16 *pBegin = szValues, *pEnd = pBegin + uValues, *pMid;
  U32 i;
  while(i = pEnd - pBegin >> 1){
    pMid = pBegin + i;
    if(uValue > *pMid)
      pBegin = pMid;
    else if(uValue < *pMid)
      pEnd = pMid;
    else{
      *ppPosition = pMid;
      return true;
    }
  }

  *ppPosition = pBegin;
  return uValue == *pBegin;
}

Bool U32Array_BinarySearch(U32 szValues[], U32 uValues, U32 uValue, U32 **ppPosition){
  U32 *pBegin = szValues, *pEnd = pBegin + uValues, *pMid;
  U32 i;
  while(i = pEnd - pBegin >> 1){
    pMid = pBegin + i;
    if(uValue > *pMid)
      pBegin = pMid;
    else if(uValue < *pMid)
      pEnd = pMid;
    else{
      *ppPosition = pMid;
      return true;
    }
  }

  *ppPosition = pBegin;
  return uValue == *pBegin;
}

Bool U64Array_BinarySearch(U64 szValues[], U32 uValues, U64 uValue, U64 **ppPosition){
  U64 *pBegin = szValues, *pEnd = pBegin + uValues, *pMid;
  U32 i;
  while(i = pEnd - pBegin >> 1){
    pMid = pBegin + i;
    if(uValue > *pMid)
      pBegin = pMid;
    else if(uValue < *pMid)
      pEnd = pMid;
    else{
      *ppPosition = pMid;
      return true;
    }
  }

  *ppPosition = pBegin;
  return uValue == *pBegin;
}

Bool I8Array_BinarySearch(I8 szValues[], U32 uValues, I8 uValue, I8 **ppPosition){
  I8 *pBegin = szValues, *pEnd = pBegin + uValues, *pMid;
  U32 i;
  while(i = pEnd - pBegin >> 1){
    pMid = pBegin + i;
    if(uValue > *pMid)
      pBegin = pMid;
    else if(uValue < *pMid)
      pEnd = pMid;
    else{
      *ppPosition = pMid;
      return true;
    }
  }

  *ppPosition = pBegin;
  return uValue == *pBegin;
}

Bool I16Array_BinarySearch(I16 szValues[], U32 uValues, I16 iValue, I16 **ppPosition){
  I16 *pBegin = szValues, *pEnd = pBegin + uValues, *pMid;
  U32 i;
  while(i = pEnd - pBegin >> 1){
    pMid = pBegin + i;
    if(iValue > *pMid)
      pBegin = pMid;
    else if(iValue < *pMid)
      pEnd = pMid;
    else{
      *ppPosition = pMid;
      return true;
    }
  }

  *ppPosition = pBegin;
  return iValue == *pBegin;
}

Bool I32Array_BinarySearch(I32 szValues[], U32 uValues, I32 iValue, I32 **ppPosition){
  I32 *pBegin = szValues, *pEnd = pBegin + uValues, *pMid;
  U32 i;
  while(i = pEnd - pBegin >> 1){
    pMid = pBegin + i;
    if(iValue > *pMid)
      pBegin = pMid;
    else if(iValue < *pMid)
      pEnd = pMid;
    else{
      *ppPosition = pMid;
      return true;
    }
  }

  *ppPosition = pBegin;
  return iValue == *pBegin;
}

Bool I64Array_BinarySearch(I64 szValues[], U32 uValues, I64 iValue, I64 **ppPosition){
  I64 *pBegin = szValues, *pEnd = pBegin + uValues, *pMid;
  U32 i;
  while(i = pEnd - pBegin >> 1){
    pMid = pBegin + i;
    if(iValue > *pMid)
      pBegin = pMid;
    else if(iValue < *pMid)
      pEnd = pMid;
    else{
      *ppPosition = pMid;
      return true;
    }
  }

  *ppPosition = pBegin;
  return iValue == *pBegin;
}

Bool F32Array_BinarySearch(F32 szValues[], U32 uValues, F32 fValue, F32 **ppPosition){
  F32 *pBegin = szValues, *pEnd = pBegin + uValues, *pMid;
  U32 i;
  while(i = pEnd - pBegin >> 1){
    pMid = pBegin + i;
    if(fValue > *pMid)
      pBegin = pMid;
    else if(fValue < *pMid)
      pEnd = pMid;
    else{
      *ppPosition = pMid;
      return true;
    }
  }

  *ppPosition = pBegin;
  return fValue == *pBegin;
}

Bool F64Array_BinarySearch(F64 szValues[], U32 uValues, F64 fValue, F64 **ppPosition){
  F64 *pBegin = szValues, *pEnd = pBegin + uValues, *pMid;
  U32 i;
  while(i = pEnd - pBegin >> 1){
    pMid = pBegin + i;
    if(fValue > *pMid)
      pBegin = pMid;
    else if(fValue < *pMid)
      pEnd = pMid;
    else{
      *ppPosition = pMid;
      return true;
    }
  }

  *ppPosition = pBegin;
  return fValue == *pBegin;
}
#endif //BINARYSEARCH_H
