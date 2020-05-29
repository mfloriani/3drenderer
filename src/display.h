#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <SDL2/SDL.h>

#define FPS 30
#define FRAME_LENGHT 1000 / FPS

enum cullMethod
{
  CULL_NONE,
  CULL_BACKFACE
} cullMethod;

enum renderMethod
{
  RENDER_WIRE,
  RENDER_WIRE_VERTEX,
  RENDER_FILL_TRIANGLE,
  RENDER_FILL_TRIANGLE_WIRE
} renderMethod;

extern int screenWidth;
extern int screenHeight;

extern SDL_Window* _window;
extern SDL_Renderer* _renderer;
extern SDL_Texture* _colorBufferTexture;
extern uint32_t* _colorBuffer;

int init();
void quit();
void renderColorBuffer();
void clearColorBuffer(uint32_t color);
void drawGrid();
void drawRect(int x, int y, int w, int h, uint32_t color);
void drawPixel(int x, int y, uint32_t color);
void drawLine(int x0, int y0, int x1, int y1, uint32_t color);
void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

#endif