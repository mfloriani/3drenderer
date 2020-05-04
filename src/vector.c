#include "vector.h"
#include <math.h>

vec3_t vec3RotateX(vec3_t v, float angle)
{
  vec3_t rotatedVector = {
    v.x,
    v.y * cos(angle) - v.z * sin(angle),
    v.y * sin(angle) + v.z * cos(angle),
  };
  return rotatedVector;
}
vec3_t vec3RotateY(vec3_t v, float angle)
{
  vec3_t rotatedVector = {
    v.x * cos(angle) - v.z * sin(angle),
    v.y,
    v.x * sin(angle) + v.z * cos(angle),
  };
  return rotatedVector;
}
vec3_t vec3RotateZ(vec3_t v, float angle)
{
  vec3_t rotatedVector = {
    v.x * cos(angle) - v.y * sin(angle),
    v.x * sin(angle) + v.y * cos(angle),
    v.z
  };
  return rotatedVector;
}