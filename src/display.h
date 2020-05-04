#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <SDL2/SDL.h>

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

#endif