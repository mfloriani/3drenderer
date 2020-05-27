#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "triangle.h"
#include "array.h"

triangle_t* trianglesToRender = NULL;

vec3_t cameraPosition = {0, 0, -5};

int fovFactor = 512;
bool _running = true;
int previousFrameTime = 0;

bool setup()
{
  _colorBuffer = (uint32_t*) malloc(sizeof(uint32_t) * screenWidth * screenHeight);
  _colorBufferTexture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, screenWidth, screenHeight);

  loadObjFileData("./assets/f22.obj");
}

void handleInput()
{
  SDL_Event event;
  while(SDL_PollEvent(&event))
  {
    switch (event.type)
    {
    case SDL_QUIT:
      _running = false;
      break;

    case SDL_KEYDOWN:
      if(event.key.keysym.sym == SDLK_ESCAPE)
      {
        _running = false;
      }
      break;
    
    default:
      break;
    }
  }
}

vec2_t project(vec3_t point)
{
  vec2_t projectedPoint = { 
    (point.x * fovFactor) / point.z, 
    (point.y * fovFactor) / point.z 
  };
  return projectedPoint;
}

void update()
{
  int timeToWait = FRAME_LENGHT - (SDL_GetTicks() - previousFrameTime);
  if(timeToWait > 0 && timeToWait <= FRAME_LENGHT)
  {
    SDL_Delay(timeToWait);
  }
  previousFrameTime = SDL_GetTicks();

  trianglesToRender = NULL;

  mesh.rotation.x += 0.01;
  mesh.rotation.y += 0.01;
  mesh.rotation.z += 0.01;

  int numFaces = array_length(mesh.faces);
  for(int i = 0; i < numFaces; ++i)
  {
    face_t meshFace = mesh.faces[i];

    vec3_t faceVertices[3];
    faceVertices[0] = mesh.vertices[meshFace.a - 1];
    faceVertices[1] = mesh.vertices[meshFace.b - 1];
    faceVertices[2] = mesh.vertices[meshFace.c - 1];

    triangle_t projectedTriangle;

    for(int j = 0; j < 3; ++j)
    {
      vec3_t transformedVertex = faceVertices[j];

      transformedVertex = vec3RotateX(transformedVertex, mesh.rotation.x);
      transformedVertex = vec3RotateY(transformedVertex, mesh.rotation.y);
      transformedVertex = vec3RotateZ(transformedVertex, mesh.rotation.z);

      transformedVertex.z -= cameraPosition.z;

      vec2_t projectedPoint = project(transformedVertex);
      projectedPoint.x += (screenWidth/2);
      projectedPoint.y += (screenHeight/2);

      projectedTriangle.points[j] = projectedPoint;
    }

    array_push(trianglesToRender, projectedTriangle);
  }
}

void render()
{
  int numTriangles = array_length(trianglesToRender);
   for(int i = 0; i < numTriangles; ++i)
   {
     triangle_t triangle = trianglesToRender[i];

     drawRect(triangle.points[0].x, triangle.points[0].y, 3, 3, 0xffffff00);
     drawRect(triangle.points[1].x, triangle.points[1].y, 3, 3, 0xffffff00);
     drawRect(triangle.points[2].x, triangle.points[2].y, 3, 3, 0xffffff00);

     drawTriangle(
       triangle.points[0].x, triangle.points[0].y,
       triangle.points[1].x, triangle.points[1].y,
       triangle.points[2].x, triangle.points[2].y,
       0xffffff00
     );
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

int main(int argc, char * argv [])
{
  _running = init();

  setup();

  while(_running)
  {
    handleInput();
    update();
    render();
  }
  
  freeResources();
  quit();

  return 0;
}