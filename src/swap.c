#include "swap.h"

void intSwap(int* a, int* b)
{
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

void floatSwap(float* a, float* b)
{
  float tmp = *a;
  *a = *b;
  *b = tmp;
}