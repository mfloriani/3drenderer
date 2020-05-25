#ifndef __MESH_H__
#define __MESH_H__

#include "vector.h"
#include "triangle.h"

#define N_MESH_VERTICES 8
extern vec3_t meshVertices[N_MESH_VERTICES];

#define N_MESH_FACES 6 * 2
extern face_t meshFaces[N_MESH_FACES];

#endif