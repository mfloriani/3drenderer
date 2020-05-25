#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "triangle.h"

triangle_t trianglesToRender[N_MESH_FACES];

vec3_t cameraPosition = {0, 0, -5};
vec3_t cubeRotation = {0, 0, 0};

int fovFactor = 512;
bool _running = true;
int previousFrameTime = 0;

bool setup()
{
  _colorBuffer = (uint32_t*) malloc(sizeof(uint32_t) * screenWidth * screenHeight);
  _colorBufferTexture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, screenWidth, screenHeight);
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

  cubeRotation.x += 0.01;
  cubeRotation.y += 0.01;
  cubeRotation.z += 0.01;

  for(int i = 0; i < N_MESH_FACES; ++i)
  {
    face_t meshFace = meshFaces[i];

    vec3_t faceVertices[3];
    faceVertices[0] = meshVertices[meshFace.a - 1];
    faceVertices[1] = meshVertices[meshFace.b - 1];
    faceVertices[2] = meshVertices[meshFace.c - 1];

    triangle_t projectedTriangle;

    for(int j = 0; j < 3; ++j)
    {
      vec3_t transformedVertex = faceVertices[j];

      transformedVertex = vec3RotateX(transformedVertex, cubeRotation.x);
      transformedVertex = vec3RotateY(transformedVertex, cubeRotation.y);
      transformedVertex = vec3RotateZ(transformedVertex, cubeRotation.z);

      transformedVertex.z -= cameraPosition.z;

      vec2_t projectedPoint = project(transformedVertex);
      projectedPoint.x += (screenWidth/2);
      projectedPoint.y += (screenHeight/2);

      projectedTriangle.points[j] = projectedPoint;
    }

    trianglesToRender[i] = projectedTriangle;
  }
}

void render()
{
   for(int i = 0; i < N_MESH_FACES; ++i)
   {
     triangle_t triangle = trianglesToRender[i];
     drawRect(triangle.points[0].x, triangle.points[0].y, 3, 3, 0xffffff00);
     drawRect(triangle.points[1].x, triangle.points[1].y, 3, 3, 0xffffff00);
     drawRect(triangle.points[2].x, triangle.points[2].y, 3, 3, 0xffffff00);
   }

  renderColorBuffer();
  clearColorBuffer(0xff000000);

  SDL_RenderPresent(_renderer);
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
  
  quit();

  return 0;
}