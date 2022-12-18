#ifndef LINE_H
#define LINE_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

inline
static void DrawLine1(U8 *pixels, I32 pitch, I32 x0, I32 y0, I32 x1, I32 y1, U8 color) {
  U8 *p = pixels + y0 * pitch + x0;
  U8 *last = pixels + y1 * pitch + x1;

  I32 dy;
  if (y1 < y0) {
    dy = y0 - y1;
    pitch = -pitch;
  }
  else {
    dy = y1 - y0;
  }

  I32 dx;
  I32 step;
  if (x1 < x0) {
    dx = x0 - x1;
    step = -1;
  }
  else {
    dx = x1 - x0;
    step = 1;
  }

  if (dy > dx) {
    I32 tmp = dx;
    dx = dy;
    dy = tmp;

    tmp = step;
    step = pitch;
    pitch = tmp;
  }

  I32 d = -dx;
  dx <<= 1;
  dy <<= 1;

  while (1) {
    *p = color;
    if (p == last)
      break;

    p += step;
    d += dy;
    if (d >= 0) {
      p += pitch;
      d -= dx;
    }
  }
}

inline
static void DrawLine2(U8 *pixels, I32 pitch, I32 x0, I32 y0, I32 x1, I32 y1, U16 color) {
  U8 *p = pixels + y0 * pitch + (x0 << 1);
  U8 *last = pixels + y1 * pitch + (x1 << 1);

  I32 dy;
  if (y1 < y0) {
    dy = y0 - y1;
    pitch = -pitch;
  }
  else {
    dy = y1 - y0;
  }

  I32 dx;
  I32 step;
  if (x1 < x0) {
    dx = x0 - x1;
    step = -2;
  }
  else {
    dx = x1 - x0;
    step = 2;
  }

  if (dy > dx) {
    I32 tmp = dx;
    dx = dy;
    dy = tmp;

    tmp = step;
    step = pitch;
    pitch = tmp;
  }

  I32 d = -dx;
  dx <<= 1;
  dy <<= 1;

  while (1) {
    *(U16*)p = color;
    if (p == last)
      break;

    p += step;
    d += dy;
    if (d >= 0) {
      p += pitch;
      d -= dx;
    }
  }
}

static void DrawLine3(U8 *pixels, I32 pitch, I32 x0, I32 y0, I32 x1, I32 y1, U8 color0, U8 color1, U8 color2) {
  U8 *p = pixels + y0 * pitch + x0 * 3;
  U8 *last = pixels + y1 * pitch + x1 * 3;

  I32 dy;
  if (y1 < y0) {
    dy = y0 - y1;
    pitch = -pitch;
  }
  else {
    dy = y1 - y0;
  }

  I32 dx;
  I32 step;
  if (x1 < x0) {
    dx = x0 - x1;
    step = -3;
  }
  else {
    dx = x1 - x0;
    step = 3;
  }

  if (dy > dx) {
    I32 tmp = dx;
    dx = dy;
    dy = tmp;

    tmp = step;
    step = pitch;
    pitch = tmp;
  }

  I32 d = -dx;
  dx <<= 1;
  dy <<= 1;

  while (1) {
    *p = color0;
    p[1] = color1;
    p[2] = color2;
    if (p == last)
      break;

    p += step;
    d += dy;
    if (d >= 0) {
      p += pitch;
      d -= dx;
    }
  }
}

inline
static void DrawLine4(U8 *pixels, I32 pitch, I32 x0, I32 y0, I32 x1, I32 y1, U32 color) {
  U8 *p = pixels + y0 * pitch + (x0 << 2);
  U8 *last = pixels + y1 * pitch + (x1 << 2);

  I32 dy;
  if (y1 < y0) {
    dy = y0 - y1;
    pitch = -pitch;
  }
  else
    dy = y1 - y0;

  I32 dx;
  I32 step;
  if (x1 < x0) {
    dx = x0 - x1;
    step = -4;
  }
  else {
    dx = x1 - x0;
    step = 4;
  }

  if (dy > dx) {
    I32 tmp = dx;
    dx = dy;
    dy = tmp;

    tmp = step;
    step = pitch;
    pitch = tmp;
  }

  I32 d = -dx;
  dx <<= 1;
  dy <<= 1;

  while (1) {
    *(U32*)p = color;
    if (p == last)
      break;

    p += step;
    d += dy;
    if (d >= 0) {
      p += pitch;
      d -= dx;
    }
  }
}

#endif //LINE_H
