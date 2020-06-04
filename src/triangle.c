#include "triangle.h"
#include "swap.h"
#include <stdlib.h>

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

void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
  drawLine(x0, y0, x1, y1, color);
  drawLine(x1, y1, x2, y2, color);
  drawLine(x2, y2, x0, y0, color);
}

vec3_t barycentricWeights(vec2_t a, vec2_t b, vec2_t c, vec2_t p)
{
  vec2_t ab = vec2Sub(b, a);
  vec2_t bc = vec2Sub(c, b);
  vec2_t ac = vec2Sub(c, a);
  vec2_t ap = vec2Sub(p, a);
  vec2_t bp = vec2Sub(p, b);

  float abcTriangleArea = ab.x * ac.y - ab.y * ac.x;
  float alpha = (bc.x * bp.y - bp.x * bc.y) / abcTriangleArea;
  float beta = (ap.x * ac.y - ac.x * ap.y) / abcTriangleArea;
  float gamma = 1 - alpha - beta;

  vec3_t weights = { alpha, beta, gamma };
  return weights;
}

void drawTexel(
  int x, int y, uint32_t* texture, 
  vec4_t pointA, vec4_t pointB, vec4_t pointC,
  tex2_t uvA, tex2_t uvB, tex2_t uvC
)
{
  vec2_t p = { x, y };
  vec2_t a = vec2_fromVec4(pointA);
  vec2_t b = vec2_fromVec4(pointB);
  vec2_t c = vec2_fromVec4(pointC);

  vec3_t weights = barycentricWeights(a, b, c, p);
  
  float alpha = weights.x;
  float beta = weights.y;
  float gamma = weights.z;

  float interpolatedU;
  float interpolatedV;
  float interpolatedReciprocalW;

  interpolatedU = (uvA.u / pointA.w) * alpha + (uvB.u / pointB.w) * beta + (uvC.u / pointC.w) * gamma;
  interpolatedV = (uvA.v / pointA.w) * alpha + (uvB.v / pointB.w) * beta + (uvC.v / pointC.w) * gamma;

  interpolatedReciprocalW = (1 / pointA.w) * alpha + (1 / pointB.w) * beta + (1 / pointC.w) * gamma;

  interpolatedU /= interpolatedReciprocalW;
  interpolatedV /= interpolatedReciprocalW;

  int texX = abs((int)(interpolatedU * textureWidth));
  int texY = abs((int)(interpolatedV * textureHeight));

  drawPixel(x, y, texture[(textureWidth * texY) + texX]);
}

void drawTexturedTriangle(
  int x0, int y0, float z0, float w0, float u0, float v0, 
  int x1, int y1, float z1, float w1, float u1, float v1, 
  int x2, int y2, float z2, float w2, float u2, float v2, 
  uint32_t* texture
)
{
  if(y0 > y1)
  {
    intSwap(&y0, &y1);
    intSwap(&x0, &x1);
    floatSwap(&z0, &z1);
    floatSwap(&w0, &w1);
    floatSwap(&u0, &u1);
    floatSwap(&v0, &v1);
  }
  if(y1 > y2)
  {
    intSwap(&y1, &y2);
    intSwap(&x1, &x2);
    floatSwap(&z1, &z2);
    floatSwap(&w1, &w2);
    floatSwap(&u1, &u2);
    floatSwap(&v1, &v2);
  }
  if(y0 > y1)
  {
    intSwap(&y0, &y1);
    intSwap(&x0, &x1);
    floatSwap(&z0, &z1);
    floatSwap(&w0, &w1);
    floatSwap(&u0, &u1);
    floatSwap(&v0, &v1);
  }

  vec4_t pointA = { x0, y0, z0, w0 };
  vec4_t pointB = { x1, y1, z1, w1 };
  vec4_t pointC = { x2, y2, z2, w2 };

  tex2_t uvA = { u0, v0 };
  tex2_t uvB = { u1, v1 };
  tex2_t uvC = { u2, v2 };

  float invSlope1 = 0;
  float invSlope2 = 0;

  if(y1 - y0 != 0) invSlope1 = (float)(x1 - x0) / (y1 - y0);
  if(y2 - y0 != 0) invSlope2 = (float)(x2 - x0) / (y2 - y0);

  if(y1 - y0 != 0) 
  {
    for(int y = y0; y <= y1; ++y)
    {
      int xStart = x1 + (y -y1) * invSlope1;
      int xEnd = x0 + (y -y0) * invSlope2;

      if(xEnd < xStart) intSwap(&xEnd, &xStart);

      for(int x = xStart; x < xEnd; ++x)
      {
        drawTexel(x, y, texture, pointA, pointB, pointC, uvA, uvB, uvC);
      }
    }
  }

  invSlope1 = 0;
  invSlope2 = 0;

  if(y2 - y1 != 0) invSlope1 = (float)(x2 - x1) / (y2 - y1);
  if(y2 - y0 != 0) invSlope2 = (float)(x2 - x0) / (y2 - y0);

  if(y2 - y1 != 0) 
  {
    for(int y = y1; y <= y2; ++y)
    {
      int xStart = x1 + (y -y1) * invSlope1;
      int xEnd = x0 + (y -y0) * invSlope2;

      if(xEnd < xStart) intSwap(&xEnd, &xStart);

      for(int x = xStart; x < xEnd; ++x)
      {
        drawTexel(x, y, texture, pointA, pointB, pointC, uvA, uvB, uvC);
      }
    }
  }


}