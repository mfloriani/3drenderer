#include "triangle.h"

void intSwap(int* a, int* b)
{
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

void fillFlatBottomTriangle(int x0, int y0, int x1, int y1, int mX, int mY, uint32_t color)
{
  float invSlope1 = (float)(x1 - x0) / (y1 - y0);
  float invSlope2 = (float)(mX - x0) / (mY - y0);

  float xStart = x0;
  float xEnd = x0;

  for(int y = y0; y <= mY; ++y)
  {
    drawLine(xStart, y, xEnd, y, color);
    xStart += invSlope1;
    xEnd += invSlope2;
  }
}

void fillFlatTopTriangle(int x0, int y0, int mX, int mY, int x2, int y2, uint32_t color)
{
  float invSlope1 = (float)(x2- x0) / (y2 - y0);
  float invSlope2 = (float)(x2 - mX) / (y2 - mY);

  float xStart = x2;
  float xEnd = x2;

  for(int y = y2; y >= y0; --y)
  {
    drawLine(xStart, y, xEnd, y, color);
    xStart -= invSlope1;
    xEnd -= invSlope2;
  }
}

void drawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
  if(y0 > y1)
  {
    intSwap(&y0, &y1);
    intSwap(&x0, &x1);
  }

  if(y1 > y2)
  {
    intSwap(&y1, &y2);
    intSwap(&x1, &x2);
  }

  if(y0 > y1)
  {
    intSwap(&y0, &y1);
    intSwap(&x0, &x1);
  }

  if(y1 == y2)
  {
    fillFlatBottomTriangle(x0, y0, x1, y1, x2, y2, color);
  }
  else if (y0 == y1)
  {
    fillFlatTopTriangle(x0, y0, x1, y1, x2, y2, color);
  }
  else
  {
    int mY = y1;
    int mX = (((x2 - x0) * (y1 - y0)) / (y2 - y0)) + x0;

    fillFlatBottomTriangle(x0, y0, x1, y1, mX, mY, color);
    fillFlatTopTriangle(x1, y1, mX, mY, x2, y2, color);
  }

}