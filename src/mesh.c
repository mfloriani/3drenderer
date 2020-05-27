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