#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "display.h"

bool _running = true;

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

void update()
{

}

void render()
{
  SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
  SDL_RenderClear(_renderer);

  //drawGrid();
  drawRect(100, 300, 100, 300, 0xffffff00);

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