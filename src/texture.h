#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <stdint.h>
#include "upng.h"

typedef struct
{
  float u, v;
} tex2_t;

extern int textureWidth;
extern int textureHeight;

extern upng_t* pngTexture;
extern uint32_t* meshTexture;
extern const uint8_t REDBRICK_TEXTURE[];

void load_png_texture_data(char* filename);

#endif