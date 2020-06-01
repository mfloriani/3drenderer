#include "light.h"

light_t light = {
  .direction = { 0, 0, 1 }
};

uint32_t light_applyIntensity(uint32_t originalColor, float factor)
{
  if(factor < 0 ) factor = 0;
  if(factor > 1) factor = 1;
  uint32_t a = (originalColor & 0xff000000);
  uint32_t r = (originalColor & 0x00ff0000) * factor;
  uint32_t g = (originalColor & 0x0000ff00) * factor;
  uint32_t b = (originalColor & 0x000000ff) * factor;

  uint32_t newColor = a | (r & 0x00ff0000) | (g & 0x0000ff00) | (b & 0x000000ff);
  return newColor;
}