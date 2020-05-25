#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include "vector.h"

typedef struct 
{
  int a;
  int b;
  int c;
} face_t;

typedef struct
{
  vec2_t points[3];
} triangle_t;

#endif