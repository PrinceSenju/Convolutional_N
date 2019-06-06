#ifndef _LIB_CORR_H_
#define _LIB_CORR_H_

void corr2d(int *X, int Wx, int Hx, int *K, int w, int h, int *Y);
{
	int x, y;
	int i, j;

	int Wy = Wx - w + 1;
	int Hy = Hx - h + 1;

	for (y = 0; y < Hy; y++){
	 for (x = 0;  x < Wy; x++){
	  *(Y + y * Wy + x) = 0;
		for (i = 0; i < h; i++){
		 for (j = 0; j < w; j++){
		  //implement this code
		 // *(y + y *wy + x) += *(x + y *wx + x)
		// Y[x, y] += x[x + j, y + i] * k[j, i];

void print2d(int *X, int Wx, int Hx);


#endif
/*
#include <stdio.h>
#include "libcorr.h"

void corr2d(int *X, int Wx, int Hx, int *K, int w, int h, int *Y)
{

        int col, row;
        // for y
        int i, j;
        // for x

        int Wy = Wx - w + 1;
        int Hy = Hx - h + 1;

        for (row = 0; row < Hy; row++){
         for (col = 0;  col < Wy; col+){
          *(Y + row * Wy + col) = 0;
                for (i = 0; i < h; i++){
                 for (j = 0; j < w; j++){
                  
                  *(Y + row *Wy + col) += *(X + (row+i) *Wx + col + j) * (*(K + i * Wx + j))
                 
                 }
                }
         }
        }

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
*/
