#include "mesh.h"

vec3_t meshVertices[N_MESH_VERTICES] = {
  { -1, -1, -1 }, //1
  { -1,  1, -1 }, //2
  {  1,  1, -1 }, //3
  {  1, -1, -1 }, //4
  {  1,  1,  1 }, //5
  {  1, -1,  1 }, //6
  { -1,  1,  1 }, //7
  { -1, -1,  1 }, //8
};

face_t meshFaces[N_MESH_FACES] = {
  { 1, 2, 3 },//front
  { 1, 3, 4 },
  { 4, 3, 5 },//right
  { 4, 5, 6 },
  { 6, 5, 7 },//back
  { 6, 7, 8 },
  { 8, 7, 2 },//left
  { 8, 2, 1 },
  { 2, 7, 5 },//top
  { 2, 5, 3 },
  { 6, 8, 1 },//bottom
  { 6, 1, 4 },
};