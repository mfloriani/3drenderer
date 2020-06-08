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
  vec4_t points[3];
  tex2_t texCoords[3];
  uint32_t color;
} triangle_t;

void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

void drawFilledTriangle(
  int x0, int y0, float z0, float w0, 
  int x1, int y1, float z1, float w1, 
  int x2, int y2, float z2, float w2, 
  uint32_t color
);

void drawTexel(
  int x, int y, uint32_t* texture, 
  vec4_t pointA, vec4_t pointB, vec4_t pointC,
  tex2_t uvA, tex2_t uvB, tex2_t uvC
);

void drawTexturedTriangle(
  int x0, int y0, float z0, float w0, float u0, float v0, 
  int x1, int y1, float z1, float w1, float u1, float v1, 
  int x2, int y2, float z2, float w2, float u2, float v2, 
  uint32_t* texture
);

#endif