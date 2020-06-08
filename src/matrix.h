#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "vector.h"

typedef struct
{
  float m[4][4];
} mat4_t;


vec4_t mat4_mulVec4(mat4_t m, vec4_t v);
mat4_t mat4_mulMat4(mat4_t a, mat4_t b);
mat4_t mat4_identity();
mat4_t mat4_makeScale(float sx, float sy, float sz);
mat4_t mat4_makeTranslation(float tx, float ty, float tz);
mat4_t mat4_makeRotationX(float angle);
mat4_t mat4_makeRotationY(float angle);
mat4_t mat4_makeRotationZ(float angle);
mat4_t mat4_makePerspective(float fov, float aspect, float znear, float zfar);
vec4_t mat4_mulVec4Projection(mat4_t projectionMatrix, vec4_t v);
mat4_t mat4_lookAt(vec3_t eye, vec3_t target, vec3_t up);

#endif