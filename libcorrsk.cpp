
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
    unsigned row, col;
    unsigned m;

    unsigned Wz = Wx;
    unsigned Hz = Hx - h + 1;

    *flop += Wz * Hz * h;

    V *Z = new V[Hz * Wz];

    for (row = 0; row < Hz; row++) {
        for (col = 0;  col < Wz; col++) {
            *(Z + row * Wz + col) = 0;
    	    for(m=0; m < h; m++) {     // kernel rows
                *(X + row * Wz + col) += Kcol[m] * *(Z + (row + m) * Wx + col);
            }
        }
     }

    // step 2: apply row convolution: output <- tmp x row_kernel
    unsigned Wy = Hx - w + 1;
    unsigned Hy = Hx - h + 1;

    *flop += Wy * Hy * w;

    for (row = 0; row < Hy; row++) {
        for (col = 0;  col < Wy; col++) {
             *(Y + row * Wy + col) = 0;
             for(m=0; m < w; ++m) {     // kernel rows
                 *(Y + row * Wy + col) += Krow[m] * *(Z + row * Wz + col+m);
             }
         }
    }

    delete [] Z;
}


/*
 * input: X - image, Hx, Wx - image size
          Krow, Kcol - kernel, h, w - kernel size
          Pw, Ph - padding size
 * output: Y - image of size ??
           flop - # of floating point operations
 */
template <class T, class U, class V>
void  corrSK0_v1(T *X, unsigned Wx, unsigned Hx, U *Krow, U *Kcol, unsigned w, unsigned h, V *Y, unsigned Pw, unsigned Ph, unsigned *flop) 
{

unsigned Wz = Wx + Pw;
unsigned Hz = Hx + Ph;

  //  unsigned Wz = Wx - w + Pw + 1;
  //  unsigned Hz = Hx - h + Ph + 1;

    *flop += Wz * Hz * h;

    V *Z = new V[Hz * Wz];

    unsigned row, col;

    for (row = 0; row < Hx; row++ ) {
        for (col = 0; col < Wx; col++ ) {
             *(Z + (row+Ph/2) * Wz + (col+Pw/2)) = *(X + row * Wx + col);
        }
    }

    unsigned Wy = Wx - w + Pw;
    unsigned Hy = Hx - h + Ph;

    *flop += Wy * Hy * w;

    for (row = 0; row < Hx; row++ ) {
        for (col = 0; col < Wx; col++ ) {
             *(Y + (row+Ph/2) * Hx + (col+Pw/2)) = *(Z + row * Hz + col);
        }
    }
print2d<float>(Z, Wz, Hz);

    corrSK<T, U, V>(Z, Wz, Hz, Krow, Kcol, w, h, Y, flop);

    delete [] Z;




/*
    // 0-padding is embedded in the calculation
    unsigned row, col;
    unsigned m;

    unsigned Wz = Wx - w + Pw + 1;
    unsigned Hz = Hx - h + Ph + 1;

    *flop += Wz * Hz * h;

    V *Z = new V[Hz * Wz];

    for (row = 0; row < Hz; row++) {
        for (col = 0;  col < Wz; col++) {
            *(Z + row * Wz + col) = 0;
    	    for(m=0; m < h; m++) {     // kernel rows
		 int Prow = row - Ph/2 + m;
	         int Pcol = col - Pw/2 + 0;
         if (Prow >= 0 && Pcol >=0 && Prow < Hx && Pcol < Wx)
          *(Z + row *Wz + col) += *(X + (Prow) * Wx + (Pcol)) * (*(Kcol + m * 1 + 0));
            }
        }
     }

    // step 2: apply row convolution: output <- tmp x row_kernel
    unsigned Wy = Hx - w + Pw + 1;
    unsigned Hy = Hx - h + Pw + 1;

    *flop += Wy * Hy * w;

    for (row = 0; row < Hy; row++) {
        for (col = 0;  col < Wy; col++) {
             *(Y + row * Wy + col) = 0;
             for(m=0; m < w; ++m) {
	    int Prow = row - Ph/2 + 0;
             int Pcol = col - Pw/2 + m;
	       if (Prow >= 0 && Pcol >=0 && Prow < Hx && Pcol < Wx)
                 *(Y + row * Wy + col) += Krow[m] * *(Z + Prow * Wz + Pcol);

             }
         }
    }

    delete [] Z;

*/
}



/*
 * input: X - image, Hx, Wx - image size
          Krow, Kcol - kernel, h, w - kernel size
          Pw, Ph - padding size
 * output: Y - image of size ??
           flop - # of floating point operations
 */
template <class T, class U, class V>
void  corrSK0_v2(T *X, unsigned Wx, unsigned Hx, U *Krow, U *Kcol, unsigned w, unsigned h, V *Y, unsigned Pw, unsigned Ph, unsigned *flop) 
{
    // 0-padding is added and then corrSK is called


}



