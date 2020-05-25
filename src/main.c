#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "vector.h"

#define NUM_POINTS (9 * 9 * 9)
vec3_t cube[NUM_POINTS];
vec2_t projectedCube[NUM_POINTS];

vec3_t cameraPosition = {0, 0, -5};
vec3_t cubeRotation = {0, 0, 0};

int fovFactor = 512;
bool _running = true;
int previousFrameTime = 0;

bool setup()
{
  _colorBuffer = (uint32_t*) malloc(sizeof(uint32_t) * screenWidth * screenHeight);
  _colorBufferTexture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, screenWidth, screenHeight);

  int pointIndex = 0;
  for(float x = -1; x <= 1; x += 0.25)
  {
    for(float y = -1; y <= 1; y += 0.25)
    {
      for(float z = -1; z <= 1; z += 0.25)
      {
        vec3_t point = {.x = x, .y = y, .z = z};
        cube[pointIndex++] = point;
      }
    }
  }
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

  for(int i = 0; i < NUM_POINTS; ++i)
  {
    vec3_t transformedPoint = vec3RotateX(cube[i], cubeRotation.x);
    transformedPoint = vec3RotateY(transformedPoint, cubeRotation.y);
    transformedPoint = vec3RotateZ(transformedPoint, cubeRotation.z);

    transformedPoint.z -= cameraPosition.z;

    projectedCube[i] = project(transformedPoint);
  }
}

void render()
{
  for(int i=0; i < NUM_POINTS; ++i)
  {
    vec2_t point = projectedCube[i];
    drawRect(point.x + (screenWidth/2), point.y + (screenHeight/2), 4, 4, 0xffffff00);
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