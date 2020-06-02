#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include <stdint.h>
#include "vector.h"
#include "display.h"
#include "texture.h"

typedef struct 
{
  int a, b, c;
  uint32_t color;
  tex2_t uvA, uvB, uvC;
} face_t;

typedef struct
{
  vec2_t points[3];
  tex2_t texCoords[3];
  uint32_t color;
  float avgDepth;
} triangle_t;

void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void drawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

void drawTexel(
  int x, int y, uint32_t* texture, 
  vec2_t pointA, vec2_t pointB, vec2_t pointC,
  float u0, float v0, float u1, float v1, float u2, float v2
);

void drawTexturedTriangle(
  int x0, int y0, float u0, float v0, 
  int x1, int y1, float u1, float v1, 
  int x2, int y2, float u2, float v2, 
  uint32_t* texture
);

#endif