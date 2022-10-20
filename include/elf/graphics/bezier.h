#ifndef BEZIER_H
#define BEZIER_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

inline static
void DrawQuadricBezier1(U8 *pPixels, I32 iPitch, I32 x0, I32 y0, I32 x1, I32 y1, I32 x2, I32 y2, U8 uColor) {
  I32 iMin = y0;
  if (y1 < iMin)
    iMin = y1;

  if (y2 < iMin)
    iMin = y2;

  I32 iMax = y0;
  if (y1 > iMax)
    iMax = y1;

  if (y2 > iMax)
    iMax = y2;

  I32 n = iMax - iMin;

  iMin = x0;
  if (x1 < iMin)
    iMin = x1;

  if (x2 < iMin)
    iMin = x2;

  iMax = x0;
  if (x1 > iMax)
    iMax = x1;

  if (x2 > iMax)
    iMax = x2;

  I32 nx = iMax - iMin;
  if (nx < n)
    n = nx;

  I32 px0 = x0;
  I32 py0 = y0;
  if (n) {
    I64 n2 = n * n;
    for (I32 t = 1, tr = n - 1; t != n; ++t, --tr) {
      I64 tr2 = tr * tr;
      I64 ttr2 = tr * t << 1;
      I64 t2 = t * t;
      I32 px1 = (tr2 * x0 + ttr2 * x1 + t2 * x2) / n2;
      I32 py1 = (tr2 * y0 + ttr2 * y1 + t2 * y2) / n2;
      DrawLine1(pPixels, iPitch, px0, py0, px1, py1, uColor);
      px0 = px1;
      py0 = py1;
    }
  }
  DrawLine1(pPixels, iPitch, px0, py0, x2, y2, uColor);
}

inline static
void DrawQuadricBezier2(U8 *pPixels, I32 iPitch, I32 x0, I32 y0, I32 x1, I32 y1, I32 x2, I32 y2, U16 uColor) {
  I32 iMin = y0;
  if (y1 < iMin)
    iMin = y1;

  if (y2 < iMin)
    iMin = y2;

  I32 iMax = y0;
  if (y1 > iMax)
    iMax = y1;

  if (y2 > iMax)
    iMax = y2;

  I32 n = iMax - iMin;

  iMin = x0;
  if (x1 < iMin)
    iMin = x1;

  if (x2 < iMin)
    iMin = x2;

  iMax = x0;
  if (x1 > iMax)
    iMax = x1;

  if (x2 > iMax)
    iMax = x2;

  I32 nx = iMax - iMin;
  if (nx < n)
    n = nx;

  I32 px0 = x0;
  I32 py0 = y0;
  if (n) {
    I64 n2 = n * n;
    for (I32 t = 1, tr = n - 1; t != n; ++t, --tr) {
      I64 tr2 = tr * tr;
      I64 ttr2 = tr * t << 1;
      I64 t2 = t * t;
      I32 px1 = (tr2 * x0 + ttr2 * x1 + t2 * x2) / n2;
      I32 py1 = (tr2 * y0 + ttr2 * y1 + t2 * y2) / n2;
      DrawLine2(pPixels, iPitch, px0, py0, px1, py1, uColor);
      px0 = px1;
      py0 = py1;
    }
  }
  DrawLine2(pPixels, iPitch, px0, py0, x2, y2, uColor);
}

inline static
void DrawQuadricBezier3(U8 *pPixels, I32 iPitch, I32 x0, I32 y0, I32 x1, I32 y1, I32 x2, I32 y2, U8 uColor0, U8 uColor1, U8 uColor2) {
  I32 iMin = y0;
  if (y1 < iMin)
    iMin = y1;

  if (y2 < iMin)
    iMin = y2;

  I32 iMax = y0;
  if (y1 > iMax)
    iMax = y1;

  if (y2 > iMax)
    iMax = y2;

  I32 n = iMax - iMin;

  iMin = x0;
  if (x1 < iMin)
    iMin = x1;

  if (x2 < iMin)
    iMin = x2;

  iMax = x0;
  if (x1 > iMax)
    iMax = x1;

  if (x2 > iMax)
    iMax = x2;

  I32 nx = iMax - iMin;
  if (nx < n)
    n = nx;

  I32 px0 = x0;
  I32 py0 = y0;
  if (n) {
    I64 n2 = n * n;
    for (I32 t = 1, tr = n - 1; t != n; ++t, --tr) {
      I64 tr2 = tr * tr;
      I64 ttr2 = tr * t << 1;
      I64 t2 = t * t;
      I32 px1 = (tr2 * x0 + ttr2 * x1 + t2 * x2) / n2;
      I32 py1 = (tr2 * y0 + ttr2 * y1 + t2 * y2) / n2;
      DrawLine3(pPixels, iPitch, px0, py0, px1, py1, uColor0, uColor1, uColor2);
      px0 = px1;
      py0 = py1;
    }
  }
  DrawLine3(pPixels, iPitch, px0, py0, x2, y2, uColor0, uColor1, uColor2);
}

inline static
void DrawQuadricBezier4(U8 *pPixels, I32 iPitch, I32 x0, I32 y0, I32 x1, I32 y1, I32 x2, I32 y2, U32 uColor) {
  I32 iMin = y0;
  if (y1 < iMin)
    iMin = y1;

  if (y2 < iMin)
    iMin = y2;

  I32 iMax = y0;
  if (y1 > iMax)
    iMax = y1;

  if (y2 > iMax)
    iMax = y2;

  I32 n = iMax - iMin;

  iMin = x0;
  if (x1 < iMin)
    iMin = x1;

  if (x2 < iMin)
    iMin = x2;

  iMax = x0;
  if (x1 > iMax)
    iMax = x1;

  if (x2 > iMax)
    iMax = x2;

  I32 nx = iMax - iMin;
  if (nx < n)
    n = nx;

  I32 px0 = x0;
  I32 py0 = y0;
  if (n) {
    I64 n2 = n * n;
    for (I32 t = 1, tr = n - 1; t != n; ++t, --tr) {
      I64 tr2 = tr * tr;
      I64 ttr2 = tr * t << 1;
      I64 t2 = t * t;
      I32 px1 = (tr2 * x0 + ttr2 * x1 + t2 * x2) / n2;
      I32 py1 = (tr2 * y0 + ttr2 * y1 + t2 * y2) / n2;
      DrawLine4(pPixels, iPitch, px0, py0, px1, py1, uColor);
      px0 = px1;
      py0 = py1;
    }
  }
  DrawLine4(pPixels, iPitch, px0, py0, x2, y2, uColor);
}

inline static
void DrawCubicBezier1(U8 *pPixels, I32 iPitch, I32 x0, I32 y0, I32 x1, I32 y1, I32 x2, I32 y2, I32 x3, I32 y3, U8 uColor) {
  I32 iMin = y0;
  if (y1 < iMin)
    iMin = y1;

  if (y2 < iMin)
    iMin = y2;

  if (y3 < iMin)
    iMin = y3;

  I32 iMax = y0;
  if (y1 > iMax)
    iMax = y1;

  if (y2 > iMax)
    iMax = y2;

  if (y3 > iMax)
    iMax = y3;

  I32 n = iMax - iMin;

  iMin = x0;
  if (x1 < iMin)
    iMin = x1;

  if (x2 < iMin)
    iMin = x2;

  if (x3 < iMin)
    iMin = x3;

  iMax = x0;
  if (x1 > iMax)
    iMax = x1;

  if (x2 > iMax)
    iMax = x2;

  if (x3 > iMax)
    iMax = x3;

  I32 nx = iMax - iMin;
  if (nx < n)
    n = nx;

  I32 px0 = x0;
  I32 py0 = y0;
  if (n) {
    I64 n3 = n * n;
    n3 *= n;
    for (I32 t = 1, tr = n - 1; t != n; ++t, --tr) {
      I64 t2 = t * t;
      I64 t3 = t2 * t;
      I64 tr2 = tr * tr;
      I64 tr3 = tr2 * tr;
      I64 c1 = 3 * t * tr2;
      I64 c2 = 3 * t2 * tr;
      I32 px1 = (tr3 * x0 + c1 * x1 + c2 * x2 + t3 * x3) / n3;
      I32 py1 = (tr3 * y0 + c1 * y1 + c2 * y2 + t3 * y3) / n3;
      DrawLine1(pPixels, iPitch, px0, py0, px1, py1, uColor);
      px0 = px1;
      py0 = py1;
    }
  }
  DrawLine1(pPixels, iPitch, px0, py0, x3, y3, uColor);
}

inline static
void DrawCubicBezier2(U8 *pPixels, I32 iPitch, I32 x0, I32 y0, I32 x1, I32 y1, I32 x2, I32 y2, I32 x3, I32 y3, U16 uColor) {
  I32 iMin = y0;
  if (y1 < iMin)
    iMin = y1;

  if (y2 < iMin)
    iMin = y2;

  if (y3 < iMin)
    iMin = y3;

  I32 iMax = y0;
  if (y1 > iMax)
    iMax = y1;

  if (y2 > iMax)
    iMax = y2;

  if (y3 > iMax)
    iMax = y3;

  I32 n = iMax - iMin;

  iMin = x0;
  if (x1 < iMin)
    iMin = x1;

  if (x2 < iMin)
    iMin = x2;

  if (x3 < iMin)
    iMin = x3;

  iMax = x0;
  if (x1 > iMax)
    iMax = x1;

  if (x2 > iMax)
    iMax = x2;

  if (x3 > iMax)
    iMax = x3;

  I32 nx = iMax - iMin;
  if (nx < n)
    n = nx;

  I32 px0 = x0;
  I32 py0 = y0;
  if (n) {
    I64 n3 = n * n;
    n3 *= n;
    for (I32 t = 1, tr = n - 1; t != n; ++t, --tr) {
      I64 t2 = t * t;
      I64 t3 = t2 * t;
      I64 tr2 = tr * tr;
      I64 tr3 = tr2 * tr;
      I64 c1 = 3 * t * tr2;
      I64 c2 = 3 * t2 * tr;
      I32 px1 = (tr3 * x0 + c1 * x1 + c2 * x2 + t3 * x3) / n3;
      I32 py1 = (tr3 * y0 + c1 * y1 + c2 * y2 + t3 * y3) / n3;
      DrawLine2(pPixels, iPitch, px0, py0, px1, py1, uColor);
      px0 = px1;
      py0 = py1;
    }
  }
  DrawLine2(pPixels, iPitch, px0, py0, x3, y3, uColor);
}

inline static
void DrawCubicBezier3(U8 *pPixels, I32 iPitch, I32 x0, I32 y0, I32 x1, I32 y1, I32 x2, I32 y2, I32 x3, I32 y3, U8 uColor0, U8 uColor1, U8 uColor2) {
  I32 iMin = y0;
  if (y1 < iMin)
    iMin = y1;

  if (y2 < iMin)
    iMin = y2;

  if (y3 < iMin)
    iMin = y3;

  I32 iMax = y0;
  if (y1 > iMax)
    iMax = y1;

  if (y2 > iMax)
    iMax = y2;

  if (y3 > iMax)
    iMax = y3;

  I32 n = iMax - iMin;

  iMin = x0;
  if (x1 < iMin)
    iMin = x1;

  if (x2 < iMin)
    iMin = x2;

  if (x3 < iMin)
    iMin = x3;

  iMax = x0;
  if (x1 > iMax)
    iMax = x1;

  if (x2 > iMax)
    iMax = x2;

  if (x3 > iMax)
    iMax = x3;

  I32 nx = iMax - iMin;
  if (nx < n)
    n = nx;

  I32 px0 = x0;
  I32 py0 = y0;
  if (n) {
    I64 n3 = n * n;
    n3 *= n;
    for (I32 t = 1, tr = n - 1; t != n; ++t, --tr) {
      I64 t2 = t * t;
      I64 t3 = t2 * t;
      I64 tr2 = tr * tr;
      I64 tr3 = tr2 * tr;
      I64 c1 = 3 * t * tr2;
      I64 c2 = 3 * t2 * tr;
      I32 px1 = (tr3 * x0 + c1 * x1 + c2 * x2 + t3 * x3) / n3;
      I32 py1 = (tr3 * y0 + c1 * y1 + c2 * y2 + t3 * y3) / n3;
      DrawLine3(pPixels, iPitch, px0, py0, px1, py1, uColor0, uColor1, uColor2);
      px0 = px1;
      py0 = py1;
    }
  }
  DrawLine3(pPixels, iPitch, px0, py0, x3, y3, uColor0, uColor1, uColor2);
}

inline static
void DrawCubicBezier4(U8 *pPixels, I32 iPitch, I32 x0, I32 y0, I32 x1, I32 y1, I32 x2, I32 y2, I32 x3, I32 y3, U32 uColor) {
  I32 iMin = y0;
  if (y1 < iMin)
    iMin = y1;

  if (y2 < iMin)
    iMin = y2;

  if (y3 < iMin)
    iMin = y3;

  I32 iMax = y0;
  if (y1 > iMax)
    iMax = y1;

  if (y2 > iMax)
    iMax = y2;

  if (y3 > iMax)
    iMax = y3;

  I32 n = iMax - iMin;

  iMin = x0;
  if (x1 < iMin)
    iMin = x1;

  if (x2 < iMin)
    iMin = x2;

  if (x3 < iMin)
    iMin = x3;

  iMax = x0;
  if (x1 > iMax)
    iMax = x1;

  if (x2 > iMax)
    iMax = x2;

  if (x3 > iMax)
    iMax = x3;

  I32 nx = iMax - iMin;
  if (nx < n)
    n = nx;

  I32 px0 = x0;
  I32 py0 = y0;
  if (n) {
    I64 n3 = n * n;
    n3 *= n;
    for (I32 t = 1, tr = n - 1; t != n; ++t, --tr) {
      I64 t2 = t * t;
      I64 t3 = t2 * t;
      I64 tr2 = tr * tr;
      I64 tr3 = tr2 * tr;
      I64 c1 = 3 * t * tr2;
      I64 c2 = 3 * t2 * tr;
      I32 px1 = (tr3 * x0 + c1 * x1 + c2 * x2 + t3 * x3) / n3;
      I32 py1 = (tr3 * y0 + c1 * y1 + c2 * y2 + t3 * y3) / n3;
      DrawLine4(pPixels, iPitch, px0, py0, px1, py1, uColor);
      px0 = px1;
      py0 = py1;
    }
  }
  DrawLine4(pPixels, iPitch, px0, py0, x3, y3, uColor);
}

#endif
