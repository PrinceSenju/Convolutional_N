#include <stdio.h>
#include "libcorr.h"

void corr2d(int *X, int Wx, int Hx, int *K, int w, int h, int *Y)
{

/* implement me */

}


void print2d(int *X, int Wx, int Hx)
{
   int x, y;

   for (y = 0; y < Hx; y++) {
      for (x = 0; x < Wx; x++) {
          printf("%d ", *(X+y*Wx+x));
      }
      printf("\n");
   }
}

