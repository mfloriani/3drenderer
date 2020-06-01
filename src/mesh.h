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
  vec3_t  scale;
  vec3_t  translation;
} mesh_t;

extern mesh_t mesh;

//TODO: remove this block of code
#define N_CUBE_VERTICES 8
#define N_CUBE_FACES (6 * 2) // 6 cube faces, 2 triangles per face
extern vec3_t cube_vertices[N_CUBE_VERTICES];
extern face_t cube_faces[N_CUBE_FACES];
void load_cube_mesh_data(void);


#endif