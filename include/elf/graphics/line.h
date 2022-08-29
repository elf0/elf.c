#ifndef DRAW_LINE_H
#define DRAW_LINE_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

#ifndef DRAW_LINE_H
#define DRAW_LINE_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

inline
static void DrawLine1(U8 *pPixels, U32 uPitch, U32 uX0, U32 uY0, U32 uX1, U32 uY1, U8 uValue) {
  I32 dy;
  I32 iPitch;
  if (uY1 < uY0) {
    dy = uY0 - uY1;
    iPitch = -uPitch;
  }
  else {
    dy = uY1 - uY0;
    iPitch = uPitch;
  }

  I32 dx;
  I32 iStep;
  if (uX1 < uX0) {
    dx = uX0 - uX1;
    iStep = -1;
  }
  else {
    dx = uX1 - uX0;
    iStep = 1;
  }

  if (dy > dx) {
    I32 iTmp = dx;
    dx = dy;
    dy = iTmp;

    iTmp = iStep;
    iStep = iPitch;
    iPitch = iTmp;
  }

  I32 d = -dx;
  dx <<= 1;
  dy <<= 1;
  U8 *p = pPixels + uY0 * uPitch + uX0;
  U8 *pLast = pPixels + uY1 * uPitch + uX1;
  while (1) {
    *p = uValue;
    if (p == pLast)
      break;

    p += iStep;
    d += dy;
    if (d >= 0) {
      p += iPitch;
      d -= dx;
    }
  }
}

inline
static void DrawLine2(U8 *pPixels, U32 uPitch, U32 uX0, U32 uY0, U32 uX1, U32 uY1, U16 uColor) {
  I32 dy;
  I32 iPitch;
  if (uY1 < uY0) {
    dy = uY0 - uY1;
    iPitch = -uPitch;
  }
  else {
    dy = uY1 - uY0;
    iPitch = uPitch;
  }

  I32 dx;
  I32 iStep;
  if (uX1 < uX0) {
    dx = uX0 - uX1;
    iStep = -2;
  }
  else {
    dx = uX1 - uX0;
    iStep = 2;
  }

  if (dy > dx) {
    I32 iTmp = dx;
    dx = dy;
    dy = iTmp;

    iTmp = iStep;
    iStep = iPitch;
    iPitch = iTmp;
  }

  I32 d = -dx;
  dx <<= 1;
  dy <<= 1;
  U8 *p = pPixels + uY0 * uPitch + (uX0 << 1);
  U8 *pLast = pPixels + uY1 * uPitch + (uX1 << 1);
  while (1) {
    *(U16*)p = uColor;
    if (p == pLast)
      break;

    p += iStep;
    d += dy;
    if (d >= 0) {
      p += iPitch;
      d -= dx;
    }
  }
}

static void DrawLine3(U8 *pPixels, U32 uPitch, U32 uX0, U32 uY0, U32 uX1, U32 uY1, U8 uValue0, U8 uValue1, U8 uValue2) {
  I32 dy;
  I32 iPitch;
  if (uY1 < uY0) {
    dy = uY0 - uY1;
    iPitch = -uPitch;
  }
  else {
    dy = uY1 - uY0;
    iPitch = uPitch;
  }

  I32 dx;
  I32 iStep;
  if (uX1 < uX0) {
    dx = uX0 - uX1;
    iStep = -3;
  }
  else {
    dx = uX1 - uX0;
    iStep = 3;
  }

  if (dy > dx) {
    I32 iTmp = dx;
    dx = dy;
    dy = iTmp;

    iTmp = iStep;
    iStep = iPitch;
    iPitch = iTmp;
  }

  I32 d = -dx;
  dx <<= 1;
  dy <<= 1;
  U8 *p = pPixels + uY0 * uPitch + uX0 * 3;
  U8 *pLast = pPixels + uY1 * uPitch + uX1 * 3;
  while (1) {
    *p = uValue0;
    p[1] = uValue1;
    p[2] = uValue2;
    if (p == pLast)
      break;

    p += iStep;
    d += dy;
    if (d >= 0) {
      p += iPitch;
      d -= dx;
    }
  }
}

inline
static void DrawLine4(U8 *pPixels, U32 uPitch, U32 uX0, U32 uY0, U32 uX1, U32 uY1, U32 uColor) {
  I32 dy;
  I32 iPitch;
  if (uY1 < uY0) {
    dy = uY0 - uY1;
    iPitch = -uPitch;
  }
  else {
    dy = uY1 - uY0;
    iPitch = uPitch;
  }

  I32 dx;
  I32 iStep;
  if (uX1 < uX0) {
    dx = uX0 - uX1;
    iStep = -4;
  }
  else {
    dx = uX1 - uX0;
    iStep = 4;
  }

  if (dy > dx) {
    I32 iTmp = dx;
    dx = dy;
    dy = iTmp;

    iTmp = iStep;
    iStep = iPitch;
    iPitch = iTmp;
  }

  I32 d = -dx;
  dx <<= 1;
  dy <<= 1;
  U8 *p = pPixels + uY0 * uPitch + (uX0 << 2);
  U8 *pLast = pPixels + uY1 * uPitch + (uX1 << 2);
  while (1) {
    *(U32*)p = uColor;
    if (p == pLast)
      break;

    p += iStep;
    d += dy;
    if (d >= 0) {
      p += iPitch;
      d -= dx;
    }
  }
}

#endif //DRAW_LINE_H

