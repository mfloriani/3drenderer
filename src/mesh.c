#include "mesh.h"
#include "array.h"
#include <stdio.h>
#include <string.h>

mesh_t mesh = {
  .vertices = NULL,
  .faces = NULL,
  .rotation = {0,0,0},
  .scale = { 1.0, 1.0, 1.0 },
  .translation = {0,0,0}
};

void loadObjFileData(char* filename)
{
  FILE* file = fopen(filename, "r");
  
  char line[1024];
  tex2_t* texcoords = NULL;

  while(fgets(line, 1024, file))
  {
    if(strncmp(line, "v ", 2) == 0)
    {
      vec3_t vertex;
      sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
      array_push(mesh.vertices, vertex);
    }

    if(strncmp(line, "vt ", 3) == 0)
    {
      tex2_t texcoord;
      sscanf(line, "vt %f %f", &texcoord.u, &texcoord.v);
      array_push(texcoords, texcoord);
    }

    if(strncmp(line, "f ", 2) == 0)
    {
      int vertexIndeces[3];
      int textureIndeces[3];
      int normalIndeces[3];
      sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", 
        &vertexIndeces[0], &textureIndeces[0], &normalIndeces[0],
        &vertexIndeces[1], &textureIndeces[1], &normalIndeces[1],
        &vertexIndeces[2], &textureIndeces[2], &normalIndeces[2]
      );

      face_t face = {
        .a = vertexIndeces[0] - 1,
        .b = vertexIndeces[1] - 1,
        .c = vertexIndeces[2] - 1,
        .uvA = texcoords[ textureIndeces[0] - 1 ],
        .uvB = texcoords[ textureIndeces[1] - 1],
        .uvC = texcoords[ textureIndeces[2] - 1 ],
        .color = 0xffffffff
      };
      array_push(mesh.faces, face);
    }
  }
  array_free(texcoords);
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
    { .a = 1, .b = 2, .c = 3, .color = 0xffffffff, .uvA = {0,0}, .uvB = {0,1}, .uvC = {1,1} },
    { .a = 1, .b = 3, .c = 4, .color = 0xffffffff, .uvA = {0,0}, .uvB = {1,1}, .uvC = {1,0} },
    // right
    { .a = 4, .b = 3, .c = 5, .color = 0xffffffff, .uvA = {0,0}, .uvB = {0,1}, .uvC = {1,1} },
    { .a = 4, .b = 5, .c = 6, .color = 0xffffffff, .uvA = {0,0}, .uvB = {1,1}, .uvC = {1,0} },
    // back
    { .a = 6, .b = 5, .c = 7, .color = 0xffffffff, .uvA = {0,0}, .uvB = {0,1}, .uvC = {1,1} },
    { .a = 6, .b = 7, .c = 8, .color = 0xffffffff, .uvA = {0,0}, .uvB = {1,1}, .uvC = {1,0} },
    // left
    { .a = 8, .b = 7, .c = 2, .color = 0xffffffff, .uvA = {0,0}, .uvB = {0,1}, .uvC = {1,1} },
    { .a = 8, .b = 2, .c = 1, .color = 0xffffffff, .uvA = {0,0}, .uvB = {1,1}, .uvC = {1,0} },
    // top
    { .a = 2, .b = 7, .c = 5, .color = 0xffffffff, .uvA = {0,0}, .uvB = {0,1}, .uvC = {1,1} },
    { .a = 2, .b = 5, .c = 3, .color = 0xffffffff, .uvA = {0,0}, .uvB = {1,1}, .uvC = {1,0} },
    // bottom
    { .a = 6, .b = 8, .c = 1, .color = 0xffffffff, .uvA = {0,0}, .uvB = {0,1}, .uvC = {1,1} },
    { .a = 6, .b = 1, .c = 4, .color = 0xffffffff, .uvA = {0,0}, .uvB = {1,1}, .uvC = {1,0} },
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