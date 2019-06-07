#include <stdio.h>
#include "libcorr.h"

/*
 * input: X - image, Hx, Wx - image size
          K - kernel, h, w - kernel size
 * output: Y - image of size (Wx - w + 1) by (Hx - h + 1)
 why should  we use flop
	figure out what operations can be floating operationsx
 */

int Double::add_count = 0;
void  corr2d(T *X, unsigned Wx, unsigned Hx, T *K, unsigned w, unsigned h, T *Y) //changed int type to T
{
//I think i just need one genneric type     

    unsigned row, col;
    unsigned i, j;

    unsigned Wy = Wx - w + 1;
    unsigned Hy = Hx - h + 1;

    for (row = 0; row < Hy; row++) {
        for (col = 0;  col < Wy; col++) {
            *(Y + row * Wy + col) = 0;
            for (i = 0; i < h; i++) {
                for (j = 0; j < w; j++) {  
                 	flops+=2;
			  *(Y + row *Wy + col) += *(X + (row + i) * Wx + (col + j)) * (*(K + i * w + j));
                }
            }
         }
     }
}

void T print2d(T *X, unsigned Wx, unsigned Hx)
{
   unsigned x, y;

   for (y = 0; y < Hx; y++) {
      for (x = 0; x < Wx; x++) {
          printf("%d ", *(X+y*Wx+x));
      }
      printf("\n");
   }
}

