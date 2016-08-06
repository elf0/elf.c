#include "Type.h"
#include "DStack.h"

#define PATH_INCREASE_SIZE 256
#define PATH_INCREASE_STACK_SIZE 8
#define PATH_DELIMITER '/'

typedef struct{
  C *pBegin;
  C *pEnd;
  C *pBufferEnd;
  DStack stack;
}Path;

static inline E8 Path_Initialize(Path *pPath, U16 uBytes, U8 uDeep){
  C *pBuffer = malloc(uBytes);
  if(!pBuffer)
    return 1;

  E8 e = DStack_Initialize(&pPath->stack, uDeep);
  if(e){
    free(pBuffer);
    return 2;
  }

  pPath->pEnd = pPath->pBegin = pBuffer;
  pPath->pBufferEnd = pBuffer + uBytes;
  return 0;
}

static inline void Path_Finalize(Path *pPath){
  DStack_Finalize(&pPath->stack);
  free(pPath->pBegin);
}

static inline void Path_SetRoot(Path *pPath, const C *pRoot, U32 uRoot){
  Byte *pBegin = pPath->pBegin;
  memcpy(pBegin, pRoot, uRoot);
  pPath->pEnd = pBegin + uRoot;
}

static inline const C *Path_Begin(Path *pPath){
  return pPath->pBegin;
}

static inline C *Path_End(Path *pPath){
  return pPath->pEnd;
}

static inline U32 Path_Size(Path *pPath){
  return pPath->pEnd - pPath->pBegin;
}

static inline E8 Path_Increase(Path *pPath, U32 uBytes){
  Byte *pBegin = pPath->pBegin;
  U32 uData = pPath->pEnd - pBegin;
  U32 uBuffer = pPath->pBufferEnd - pBegin + uBytes;

  Byte *pBuffer = (Byte*)malloc(uBuffer);
  if(!pBuffer)
    return 1;

  memcpy(pBuffer, pBegin, uData);
  free(pBegin);

  pPath->pBegin = pBuffer;
  pPath->pEnd = pBuffer + uData;
  pPath->pBufferEnd = pBuffer + uBuffer;
  return 0;
}

static inline E8 Path_Prepare(Path *pPath, U8 uName){
  if(DStack_Full(&pPath->stack)){
    if(DStack_Increase(&pPath->stack, PATH_INCREASE_STACK_SIZE))
      return 1;
  }

  if((pPath->pBufferEnd - pPath->pEnd) < (1 + uName))
    return Path_Increase(pPath, PATH_INCREASE_SIZE);

  return 0;
}

static inline void Path_Push(Path *pPath, const C *pName, U8 uName){
  C *p = pPath->pEnd;
  *p++ = PATH_DELIMITER;
  memcpy(p, pName, uName);
  pPath->pEnd = p + uName;

  *DStack_Push(&pPath->stack) = uName;
}

static inline B Path_CanPop(Path *pPath){
  return DStack_NotEmpty(&pPath->stack);
}

static inline void Path_Pop(Path *pPath){
  pPath->pEnd -= *DStack_Pop(&pPath->stack) + 1;
}
