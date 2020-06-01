#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "vector.h"
#include <stdint.h>

typedef struct
{
  vec3_t direction;
} light_t;

extern light_t light;

uint32_t light_applyIntensity(uint32_t originalColor, float factor);

#endif