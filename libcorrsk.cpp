
/* 
 * input: X - image, Hx, Wx - image size
          Krow, Kcol - kernel, h, w - kernel size
 * output: Y - image of size (Wx - w + 1) by (Hx - h + 1)
           flop - # of floating point operations
 */

template <class T, class U, class V>
void corrSK(T *X, unsigned Wx, unsigned Hx, U *Krow, U *Kcol, unsigned w, unsigned h, V *Y, unsigned *flop)
{
// step 1: apply column convolution: tmp <- input x col_kernel
// step 2: apply row convolution: output <- tmp x row_kernel

    unsigned row, col, krows, kcol, mm;
    unsigned i, j, m;

    unsigned Wz = Wx;
    unsigned Hz = Hx - h + 1;

//    *flop += Wz * Hz * w * h;
	V *Z = new V[Hz * Wz];

       for (row = 0; row < Hz; row++) {
        for (col = 0;  col < Wz; col++) {
            *(Z + row * Wz + col) = 0;
    	  for(m=0; m < kcol; ++m)     // kernel rows
          {
            *(Z + row * Hz + col) += Kcol[m] * *(X + (row + m) * Hx + col);

            }
        }
    }
print2d(Z, Wz, Hz);
/* implement me


        for (col = 0;  col < Wz; col++) {
         for (row = 0; row < Hz; row++) {
            *(Z + row * Wz + col) = 0;
    	  for(m=0; m < krow; ++m)     // kernel rows
          {
            *(Z + col * Hz + row) += Krow[m] * *(X + (col + m) * Hx + row);

            }
        }
    }

*/
}


template <class T>
void random1d(T *X, unsigned Wx)
{
// implement me
}

template <class T>
void seq1d(T *X, unsigned Wx)
{
// implement me
}


