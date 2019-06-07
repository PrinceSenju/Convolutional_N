
/*
 * input: X - image, Hx, Wx - image size
          K - kernel, h, w - kernel size
 * output: Y - image of size (Wx - w + 1) by (Hx - h + 1)
 why should  we use flop
	figure out what operations can be floating operationsx
 */

template <class T, class U, class V>
void  corr2d(T *X, unsigned Wx, unsigned Hx, T *K, unsigned w, unsigned h, T *Y, unsigned *flop) 
{
    unsigned row, col;
    unsigned i, j;

    unsigned Wy = Wx - w + 1;
    unsigned Hy = Hx - h + 1;

    *flop = Wy * Hy * w * h;

    for (row = 0; row < Hy; row++) {
        for (col = 0;  col < Wy; col++) {
            *(Y + row * Wy + col) = 0;
            for (i = 0; i < h; i++) {
                for (j = 0; j < w; j++) {  
                 	//flops+=2;
			        *(Y + row *Wy + col) += *(X + (row + i) * Wx + (col + j)) * (*(K + i * w + j));
                }
            }
         }
     }
}

template <class T>
void print2d(T *X, unsigned Wx, unsigned Hx)
{
   unsigned x, y;

   for (y = 0; y < Hx; y++) {
      for (x = 0; x < Wx; x++) {
            std::cout << *(X+y*Wx+x) << " ";
      }
      std::cout << std::endl;
   }
}

template <class T>
void random2d(T *X, unsigned Wx, unsigned Hx)
{
// @implement me: fill in array with random numbers

   for (int y = 0; y < Hx; y++) {
     for (int x = 0; x < Wx; x++) { 
        *(X+y*Wx+x) = 1 + rand() % 300; 
        cout << *(X+y*Wx+x);
}
     }
/* WHERE WOULD I IMPLEMENT THIS? IN THE HEADER OR ?
   }#include <iostream>

using namespace std;
#include <iomanip>
using std::setw;
#include <cstdlib> 
*/
