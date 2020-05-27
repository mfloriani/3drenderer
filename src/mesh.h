#ifndef __MESH_H__
#define __MESH_H__

#include "vector.h"
#include "triangle.h"

void loadObjFileData(char* filename);

typedef struct
{
  vec3_t* vertices;
  face_t* faces;
  vec3_t  rotation;
} mesh_t;

extern mesh_t mesh;

#endif