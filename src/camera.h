#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "vector.h"

typedef struct
{
  vec3_t position;
  vec3_t direction;
  vec3_t forwardVelocity;
  float yawAngle;

} camera_t;

extern camera_t camera;

#endif