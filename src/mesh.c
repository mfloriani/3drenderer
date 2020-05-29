#include "mesh.h"
#include "array.h"
#include <stdio.h>
#include <string.h>

mesh_t mesh = {
  .vertices = NULL,
  .faces = NULL,
  .rotation = {0,0,0}
};

void loadObjFileData(char* filename)
{
  FILE* file = fopen(filename, "r");
  
  char line[1024];
  while(fgets(line, 1024, file))
  {
    if(strncmp(line, "v ", 2) == 0)
    {
      vec3_t vertex;
      sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
      array_push(mesh.vertices, vertex);
    }
    if(strncmp(line, "f ", 2) == 0)
    {
      int vertextIndeces[3];
      int textureIndeces[3];
      int normalIndeces[3];
      sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", 
        &vertextIndeces[0], &textureIndeces[0], &normalIndeces[0],
        &vertextIndeces[1], &textureIndeces[1], &normalIndeces[1],
        &vertextIndeces[2], &textureIndeces[2], &normalIndeces[2]
      );

      face_t face;
      face.a = vertextIndeces[0];
      face.b = vertextIndeces[1];
      face.c = vertextIndeces[2];
      array_push(mesh.faces, face);
    }
  }
}

vec3_t cube_vertices[N_CUBE_VERTICES] = {
    { .x = -1, .y = -1, .z = -1 }, // 1
    { .x = -1, .y =  1, .z = -1 }, // 2
    { .x =  1, .y =  1, .z = -1 }, // 3
    { .x =  1, .y = -1, .z = -1 }, // 4
    { .x =  1, .y =  1, .z =  1 }, // 5
    { .x =  1, .y = -1, .z =  1 }, // 6
    { .x = -1, .y =  1, .z =  1 }, // 7
    { .x = -1, .y = -1, .z =  1 }  // 8
};

face_t cube_faces[N_CUBE_FACES] = {
    // front
    { .a = 1, .b = 2, .c = 3, .color = 0xffff0000 },
    { .a = 1, .b = 3, .c = 4, .color = 0xffff0000 },
    // right
    { .a = 4, .b = 3, .c = 5, .color = 0xff00ff00 },
    { .a = 4, .b = 5, .c = 6, .color = 0xff00ff00 },
    // back
    { .a = 6, .b = 5, .c = 7, .color = 0xff0000ff },
    { .a = 6, .b = 7, .c = 8, .color = 0xff0000ff },
    // left
    { .a = 8, .b = 7, .c = 2, .color = 0xffffff00 },
    { .a = 8, .b = 2, .c = 1, .color = 0xffffff00 },
    // top
    { .a = 2, .b = 7, .c = 5, .color = 0xffffffff },
    { .a = 2, .b = 5, .c = 3, .color = 0xffffffff },
    // bottom
    { .a = 6, .b = 8, .c = 1, .color = 0xff0ffff0 },
    { .a = 6, .b = 1, .c = 4, .color = 0xff0ffff0 },
};

void load_cube_mesh_data(void) {
    for (int i = 0; i < N_CUBE_VERTICES; i++) {
        vec3_t cube_vertex = cube_vertices[i];
        array_push(mesh.vertices, cube_vertex);
    }
    for (int i = 0; i < N_CUBE_FACES; i++) {
        face_t cube_face = cube_faces[i];
        array_push(mesh.faces, cube_face);
    }
}