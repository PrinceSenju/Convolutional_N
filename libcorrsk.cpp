
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
    	    for(m=0; m < h; m++) {     // kernel cols
                *(Z + row * Wz + col) += Kcol[m] * *(X + (row + m) * Wx + col);
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
    unsigned Wz = Wx;
    unsigned Hz = Hx - h + Ph + 1;

    *flop += Wz * Hz * h;

    V *Z = new V[Hz * Wz];

    unsigned row, col, m;

    for (row = 0; row < Hz; row++) {
        for (col = 0;  col < Wz; col++) {
            *(Z + row * Wz + col) = 0;
    	    for(m=0; m < h; m++) {     // kernel cols
                int Prow = row - Ph/2 + m;
                if (Prow >= 0 && Prow < (int)Hx) 
                   *(Z + row * Wz + col) += Kcol[m] * *(X + Prow * Wx + col);
                else
                   *flop -= 1;
            }
        }
     }

    unsigned Wy = Wx - w + Pw + 1;
    unsigned Hy = Hz;

    *flop += Wy * Hy * w;

    for (row = 0; row < Hy; row++) {
        for (col = 0;  col < Wy; col++) {
            *(Y + row * Wy + col) = 0;
    	    for(m=0; m < w; m++) {     // kernel cols
                int Pcol = col - Pw/2 + m;
                if (Pcol >= 0 && Pcol < (int)Wz)
                   *(Y + row * Wy + col) += Krow[m] * *(Z + row * Wz + Pcol);
                else
                   *flop -= 1;
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
void  corrSK0_v2(T *X, unsigned Wx, unsigned Hx, U *Krow, U *Kcol, unsigned w, unsigned h, V *Y, unsigned Pw, unsigned Ph, unsigned *flop) 
{
    // 0-padding is added and then corrSK is called

    // ./test -x 3 -y 3 -kw 2 -kh 2 -vs 1 -hs 1 -p -o -c 1 -k 2 -m 2
    //  Output matrix: 
    // 0 1 2 0 
    // 3 4 5 0 
    // 6 7 8 0 
    // 0 0 0 0 

    unsigned Wz = Wx + Pw;
    unsigned Hz = Hx + Ph;

    T *Z = new float[Wz * Hz];

    unsigned row, col;

    for (row = 0; row < Hx; row++ ) {  
        for (col = 0; col < Wx; col++ ) {
             *(Z + (row+Ph/2) * Wz + (col+Pw/2)) = *(X + row * Wx + col); 
        }
    }

    corrSK<T, U, V>(Z, Wz, Hz, Krow, Kcol, w, h, Y, flop);

    delete [] Z;
}



////////////////////





/*
 * input: X - image, Hx, Wx - image size
          Krow, Kcol - kernel, h, w - kernel size
          Pw, Ph - padding size
          Sw, Sh - stride size
 * output: Y - image of size ??
           flop - # of floating point operations
 */
template <class T, class U, class V>
void  corrSK0s_v1(T *X, unsigned Wx, unsigned Hx, U *Krow, U *Kcol, unsigned w, unsigned h, V *Y, unsigned Pw, unsigned Ph, unsigned Sw, unsigned Sh, unsigned *flop) 
{
    unsigned Wz = Wx;
    unsigned Hz = (Hx - h + Ph + Sh) / Sh;

    unsigned row, col, m;

    *flop += Wz * Hz * h;

    V *Z = new V[Hz * Wz];

    for (row = 0; row < Hz; row++) {
        for (col = 0;  col < Wz; col++) {
            *(Z + row * Wz + col) = 0;
    	    for(m=0; m < h; m++) {     // kernel cols
                int Prow = Sh * row - Ph/2 + m;
                if (Prow >= 0 && Prow < (int)Hx) 
                   *(Z + row * Wz + col) += Kcol[m] * *(X + Prow * Wx + col);
                else
                   *flop -= 1;
            }
        }
     }

     unsigned Wy = (Wx - w + Pw + Sw) / Sw;
     unsigned Hy = Hz;

    *flop += Wy * Hy * w;

    for (row = 0; row < Hy; row++) {
        for (col = 0;  col < Wy; col++) {
            *(Y + row * Wy + col) = 0;
    	    for(m=0; m < w; m++) {     // kernel cols
                int Pcol = Sw * col - Pw/2 + m;
                if (Pcol >= 0 && Pcol < (int)Wz) 
                   *(Y + row * Wy + col) += Krow[m] * *(Z + row * Wz + Pcol);
                else
                    *flop -= 1;
            }
        }
     }

    delete [] Z;
}


/* striding, but no 0-padding
 * input: X - image, Hx, Wx - image size
          Krow, Kcol - kernel, h, w - kernel size
 * output: Y - image of size (Wx - w + 1) by (Hx - h + 1)
          flop - # of floating point operations
 */
template <class T, class U, class V>
void corrSKs(T *X, unsigned Wx, unsigned Hx, U *Krow, U *Kcol, unsigned w, unsigned h, V *Y, unsigned Sw, unsigned Sh, unsigned *flop)
{
    unsigned Wz = Wx;
    unsigned Hz = (Hx - h + Sh) / Sh;

    unsigned row, col, m;

    *flop += Wz * Hz * h;

    V *Z = new V[Hz * Wz];

    for (row = 0; row < Hz; row++) {
        for (col = 0;  col < Wz; col++) {
            *(Z + row * Wz + col) = 0;
    	    for(m=0; m < h; m++) {     // kernel cols
                int Prow = Sh * row + m;
                *(Z + row * Wz + col) += Kcol[m] * *(X + Prow * Wx + col);
            }
        }
     }

     unsigned Wy = (Wx - w + Sw) / Sw;
     unsigned Hy = Hz;

    *flop += Wy * Hy * w;

    for (row = 0; row < Hy; row++) {
        for (col = 0;  col < Wy; col++) {
            *(Y + row * Wy + col) = 0;
    	    for(m=0; m < w; m++) {     // kernel cols
                int Pcol = Sw * col + m;
                *(Y + row * Wy + col) += Krow[m] * *(Z + row * Wz + Pcol);
            }
        }
     }

    delete [] Z;
}




/*
 * input: X - image, Hx, Wx - image size
          Krow, Kcol - kernel, h, w - kernel size
          Pw, Ph - padding size
          Sw, Sh - stride size
 * output: Y - image of size ??
           flop - # of floating point operations
 */
template <class T, class U, class V>
void  corrSK0s_v2(T *X, unsigned Wx, unsigned Hx, U *Krow, U *Kcol, unsigned w, unsigned h, V *Y, unsigned Pw, unsigned Ph, unsigned Sw, unsigned Sh, unsigned *flop) 
{
    // 0-padding is added and then corrSK is called

    // ./test -x 3 -y 3 -kw 2 -kh 2 -vs 1 -hs 1 -p -o -c 1 -k 2 -m 2
    //  Output matrix: 
    // 0 1 2 0 
    // 3 4 5 0 
    // 6 7 8 0 
    // 0 0 0 0 

    unsigned Wz = Wx + Pw;
    unsigned Hz = Hx + Ph;

    T *Z = new float[Wz * Hz];

    unsigned row, col;

    for (row = 0; row < Hx; row++ ) {  
        for (col = 0; col < Wx; col++ ) {
             *(Z + (row+Ph/2) * Wz + (col+Pw/2)) = *(X + row * Wx + col); 
        }
    }

    corrSKs<T, U, V>(Z, Wz, Hz, Krow, Kcol, w, h, Y, Sw, Sh, flop);

    delete [] Z;
}




