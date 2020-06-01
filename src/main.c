#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "triangle.h"
#include "array.h"
#include "matrix.h"

triangle_t *trianglesToRender = NULL;

vec3_t cameraPosition = {0, 0, 0};
mat4_t projectionMatrix;

bool _running = true;
int previousFrameTime = 0;

bool setup()
{
  renderMethod = RENDER_WIRE;
  cullMethod = CULL_BACKFACE;

  _colorBuffer = (uint32_t *)malloc(sizeof(uint32_t) * screenWidth * screenHeight);
  _colorBufferTexture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, screenWidth, screenHeight);

  float fov = M_PI/3; // 180/3 or 60deg
  float aspect = (float)screenHeight / (float)screenWidth;
  float znear = 0.1;
  float zfar = 100.0;
  projectionMatrix = mat4_makePerspective(fov, aspect, znear, zfar);

  //loadObjFileData("./assets/cube.obj");
  load_cube_mesh_data();
}

void handleInput()
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    switch (event.type)
    {
    case SDL_QUIT:
      _running = false;
      break;

    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE)
      {
        _running = false;
      }
      if (event.key.keysym.sym == SDLK_1)
      {
        renderMethod = RENDER_WIRE_VERTEX;
      }
      if (event.key.keysym.sym == SDLK_2)
      {
        renderMethod = RENDER_WIRE;
      }
      if (event.key.keysym.sym == SDLK_3)
      {
        renderMethod = RENDER_FILL_TRIANGLE;
      }
      if (event.key.keysym.sym == SDLK_4)
      {
        renderMethod = RENDER_FILL_TRIANGLE_WIRE;
      }
      if (event.key.keysym.sym == SDLK_c)
      {
        cullMethod = CULL_BACKFACE;
      }
      if (event.key.keysym.sym == SDLK_d)
      {
        cullMethod = CULL_NONE;
      }
      break;

    default:
      break;
    }
  }
}

void update()
{
  int timeToWait = FRAME_LENGHT - (SDL_GetTicks() - previousFrameTime);
  if (timeToWait > 0 && timeToWait <= FRAME_LENGHT)
  {
    SDL_Delay(timeToWait);
  }
  previousFrameTime = SDL_GetTicks();

  trianglesToRender = NULL;

  mesh.rotation.x += 0.01;
  mesh.rotation.y += 0.01;
  mesh.rotation.z += 0.01;
  
  // mesh.scale.x += 0.002;
  // mesh.scale.y += 0.001;

  // mesh.translation.x += 0.01;
  mesh.translation.z = 5;
  
  mat4_t scaleMatrix = mat4_makeScale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
  mat4_t translationMatrix = mat4_makeTranslation(mesh.translation.x, mesh.translation.y, mesh.translation.z);
  mat4_t rotationMatrixX = mat4_makeRotationX(mesh.rotation.x);
  mat4_t rotationMatrixY = mat4_makeRotationY(mesh.rotation.y);
  mat4_t rotationMatrixZ = mat4_makeRotationZ(mesh.rotation.z);

  int numFaces = array_length(mesh.faces);
  for (int i = 0; i < numFaces; ++i)
  {
    face_t meshFace = mesh.faces[i];

    vec3_t faceVertices[3];
    faceVertices[0] = mesh.vertices[meshFace.a - 1];
    faceVertices[1] = mesh.vertices[meshFace.b - 1];
    faceVertices[2] = mesh.vertices[meshFace.c - 1];

    vec4_t transformedVertices[3];
    for (int j = 0; j < 3; ++j)
    {
      vec4_t transformedVertex = vec4_fromVec3(faceVertices[j]);
      
      mat4_t worldMatrix = mat4_identity();
      worldMatrix = mat4_mulMat4(scaleMatrix, worldMatrix);
      worldMatrix = mat4_mulMat4(rotationMatrixX, worldMatrix);
      worldMatrix = mat4_mulMat4(rotationMatrixY, worldMatrix);
      worldMatrix = mat4_mulMat4(rotationMatrixZ, worldMatrix);
      worldMatrix = mat4_mulMat4(translationMatrix, worldMatrix);      

      transformedVertex = mat4_mulVec4(worldMatrix, transformedVertex);
      transformedVertices[j] = transformedVertex;
    }

    if (cullMethod == CULL_BACKFACE)
    {
      //Culling faces
      vec3_t vecA = vec3_fromVec4(transformedVertices[0]);
      vec3_t vecB = vec3_fromVec4(transformedVertices[1]);
      vec3_t vecC = vec3_fromVec4(transformedVertices[2]);

      vec3_t vecAB = vec3Sub(vecB, vecA);
      vec3_t vecAC = vec3Sub(vecC, vecA);
      vecAB = vec3Normalize(vecAB);
      vecAC = vec3Normalize(vecAC);

      vec3_t normal = vec3Cross(vecAB, vecAC);
      normal = vec3Normalize(normal);

      vec3_t cameraRay = vec3Sub(cameraPosition, vecA);

      float dotNormalCamera = vec3Dot(normal, cameraRay);
      if (dotNormalCamera < 0) continue;
    }

    vec4_t projectedPoints[3];

    for (int j = 0; j < 3; ++j)
    {
      projectedPoints[j] = mat4_mulVec4Projection(projectionMatrix, transformedVertices[j]);
      
      //scale into the view
      projectedPoints[j].x *= (screenWidth / 2.0);
      projectedPoints[j].y *= (screenHeight / 2.0);
      
      //translate to the middle of the screen
      projectedPoints[j].x += (screenWidth / 2.0);
      projectedPoints[j].y += (screenHeight / 2.0);
    }

    float avgDepth = (transformedVertices[0].z + transformedVertices[1].z + transformedVertices[2].z) / 3.0;

    triangle_t projectedTriangle = {
        .points = {
            {projectedPoints[0].x, projectedPoints[0].y},
            {projectedPoints[1].x, projectedPoints[1].y},
            {projectedPoints[2].x, projectedPoints[2].y},
        },
        .color = meshFace.color,
        .avgDepth = avgDepth};

    array_push(trianglesToRender, projectedTriangle);
  }

  int numTriangles = array_length(trianglesToRender);
  for (int i = 0; i < numTriangles; ++i)
  {
    for (int j = i; j < numTriangles; ++j)
    {
      if (trianglesToRender[i].avgDepth < trianglesToRender[j].avgDepth)
      {
        triangle_t tmp = trianglesToRender[i];
        trianglesToRender[i] = trianglesToRender[j];
        trianglesToRender[j] = tmp;
      }
    }
  }
}

void render()
{
  SDL_RenderClear(_renderer);

  int numTriangles = array_length(trianglesToRender);
  for (int i = 0; i < numTriangles; ++i)
  {
    triangle_t triangle = trianglesToRender[i];

    if (renderMethod == RENDER_FILL_TRIANGLE || renderMethod == RENDER_FILL_TRIANGLE_WIRE)
    {
      drawFilledTriangle(
          triangle.points[0].x, triangle.points[0].y,
          triangle.points[1].x, triangle.points[1].y,
          triangle.points[2].x, triangle.points[2].y,
          triangle.color);
    }

    if (renderMethod == RENDER_WIRE || renderMethod == RENDER_WIRE_VERTEX || renderMethod == RENDER_FILL_TRIANGLE_WIRE)
    {
      drawTriangle(
          triangle.points[0].x, triangle.points[0].y,
          triangle.points[1].x, triangle.points[1].y,
          triangle.points[2].x, triangle.points[2].y,
          0xffff0000);
    }

    if (renderMethod == RENDER_WIRE_VERTEX)
    {
      drawRect(triangle.points[0].x - 3, triangle.points[0].y - 3, 6, 6, 0xffff0000);
      drawRect(triangle.points[1].x - 3, triangle.points[1].y - 3, 6, 6, 0xffff0000);
      drawRect(triangle.points[2].x - 3, triangle.points[2].y - 3, 6, 6, 0xffff0000);
    }
  }

  array_free(trianglesToRender);
  renderColorBuffer();
  clearColorBuffer(0xff000000);
  SDL_RenderPresent(_renderer);
}

void freeResources()
{
  free(_colorBuffer);
  array_free(mesh.faces);
  array_free(mesh.vertices);
}

int main(int argc, char *argv[])
{
  _running = init();

  setup();

  while (_running)
  {
    handleInput();
    update();
    render();
  }

  freeResources();
  quit();

  return 0;
}