#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

int screenWidth = 800;
int screenHeight = 600;

SDL_Window* _window = NULL;
SDL_Renderer* _renderer = NULL;
SDL_Texture* _colorBufferTexture = NULL;

bool _running = true;
uint32_t* _colorBuffer = NULL;

int init()
{
  if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
  {
    SDL_Log("Failed to initialize SDL %s", SDL_GetError());
    return false;
  }

  SDL_DisplayMode displayMode;
  SDL_GetCurrentDisplayMode(0, &displayMode);
  screenWidth = displayMode.w;
  screenHeight = displayMode.h;

  _window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_BORDERLESS);
  if(!_window)
  {
    SDL_Log("Failed to create window %s", SDL_GetError());
    return false;
  }

  _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
  if(!_renderer)
  {
    SDL_Log("Failed to create renderer %s", SDL_GetError());
    return false;
  }

  return true;
}

void quit()
{
  SDL_DestroyTexture(_colorBufferTexture);
  free(_colorBuffer);
  SDL_DestroyRenderer(_renderer);
  SDL_DestroyWindow(_window);
  SDL_Quit();
}

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

void renderColorBuffer()
{
  SDL_UpdateTexture(_colorBufferTexture, NULL, _colorBuffer, screenWidth * sizeof(uint32_t));
  SDL_RenderCopy(_renderer, _colorBufferTexture, NULL, NULL);
}

void clearColorBuffer(uint32_t color)
{
  for(int y = 0; y < screenHeight; ++y)
  {
    for(int x = 0; x < screenWidth; ++x)
    {
      _colorBuffer[(screenWidth * y) + x] = color;
    }
  }
}

void drawGrid()
{
  for(int y = 0; y < screenHeight; ++y)
  {
    for(int x = 0; x < screenWidth; ++x)
    {
      if((y % 10) == 0 || (x % 10) == 0)
      {
        int pixel = (screenWidth * y) + x;
        _colorBuffer[pixel] = 0xff111111;
      }
    }
  }
}

void drawRect(int x, int y, int w, int h, uint32_t color)
{
  for(int j = 0; j < h; ++j )
  {
    for(int i = 0; i < w; ++i )
    {
      _colorBuffer[(screenWidth * (y + j) + (x+i))] = color;
    }
  }
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