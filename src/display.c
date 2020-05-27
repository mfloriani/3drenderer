#include "display.h"
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

int screenWidth = 800;
int screenHeight = 600;

SDL_Window* _window = NULL;
SDL_Renderer* _renderer = NULL;
SDL_Texture* _colorBufferTexture = NULL;
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
      int screenX = x + i;
      int screenY = y + j;
      //_colorBuffer[(screenWidth * (y + j) + (x+i))] = color;
      drawPixel(screenX, screenY, color);
    }
  }
}

void drawPixel(int x, int y, uint32_t color)
{
  if(x < 0 || x > screenWidth || y < 0 || y > screenHeight) return;
  
  _colorBuffer[(screenWidth * y) + x] = color;
}

void drawLine(int x0, int y0, int x1, int y1, uint32_t color)
{
  int deltaX = x1 - x0;
  int deltaY = y1 - y0;

  int sideLength = abs(deltaX) >= abs(deltaY) ? abs(deltaX) : abs(deltaY);

  float xInc = deltaX / (float)sideLength;
  float yInc = deltaY / (float)sideLength;

  float currentX = x0;
  float currentY = y0;

  for(int i = 0; i <= sideLength; ++i)
  {
    drawPixel(round(currentX), round(currentY), color);
    currentX += xInc;
    currentY += yInc;
  }
}

void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
  drawLine(x0, y0, x1, y1, color);
  drawLine(x1, y1, x2, y2, color);
  drawLine(x2, y2, x0, y0, color);
}
