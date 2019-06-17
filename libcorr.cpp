
/*
 * input: X - image, Hx, Wx - image size
          K - kernel, h, w - kernel size
 * output: Y - image of size (Wx - w + 1) by (Hx - h + 1)
           flop - # of floating point operations
 */

template <class T, class U, class V>
void corr2d(T *X, unsigned Wx, unsigned Hx, U *K, unsigned w, unsigned h, V *Y, unsigned *flop) 
{
    unsigned row, col;
    unsigned i, j;

    unsigned Wy = Wx - w + 1;
    unsigned Hy = Hx - h + 1;

    *flop += Wy * Hy * w * h;

    for (row = 0; row < Hy; row++) {
        for (col = 0;  col < Wy; col++) {
            *(Y + row * Wy + col) = 0;
            for (i = 0; i < h; i++) {
                for (j = 0; j < w; j++) {  
		        *(Y + row * Wy + col) += *(X + (row + i) * Wx + (col + j)) * (*(K + i * w + j));
                }
            }
         }
     }
}

/*
 * input: X - image, Hx, Wx - image size
          K - kernel, h, w - kernel size
          Sw, Sh - stride size
 * output: Y - image of size (Wx - w + 1) by (Hx - h + 1)
           flop - # of floating point operations
 */
template <class T, class U, class V>
void corr2ds(T *X, unsigned Wx, unsigned Hx, U *K, unsigned w, unsigned h, V *Y, unsigned Pw, unsigned Ph, unsigned Sw, unsigned Sh, unsigned *flop) 
{
    unsigned row, col;
    unsigned i, j;

    unsigned Wy = Wx - w + 1;
    unsigned Hy = Hx - h + 1;

    *flop += Wy * Hy * w * h;

    for (row = 0; row < Hy; row++) {
        for (col = 0;  col < Wy; col++) {
            *(Y + row * Wy + col) = 0;
            for (i = 0; i < h; i++) {
                for (j = 0; j < w; j++) {  
// modify me to add striding 
		        *(Y + row * Wy + col) += *(X + (row + i) * Wx + (col + j)) * (*(K + i * w + j));
                }
            }
         }
     }
}

/*
 * input: X - image, Hx, Wx - image size
          K - kernel, h, w - kernel size
          Pw, Ph - padding size
 * output: Y - image of size (Wx - w + 1) by (Hx - h + 1)
           flop - # of floating point operations
 */
template <class T, class U, class V>
void  corr2d0_v1(T *X, unsigned Wx, unsigned Hx, U *K, unsigned w, unsigned h, V *Y, unsigned Pw, unsigned Ph, unsigned *flop) 
{
    unsigned row, col;
    unsigned i, j;

    unsigned Wy = Wx - w + Pw + 1;
    unsigned Hy = Hx - h + Ph + 1;

    *flop += Wy * Hy * w * h; // think about 0-padding

    for (row = 0; row < Hy; row++) {
        for (col = 0;  col < Wy; col++) {
            *(Y + row * Wy + col) = 0;
            for (i = 0; i < h; i++) {
                for (j = 0; j < w; j++) {
                        int Prow = row - Ph/2 + i;
                        int Pcol = col - Pw/2 + j;
                        if (Prow >= 0 && Pcol >=0 && Prow < Hx && Pcol < Wx)
			        *(Y + row *Wy + col) += *(X + (Prow) * Wx + (Pcol)) * (*(K + i * w + j));
                }
            }
         }
     }
}


/*
 * input: X - image, Hx, Wx - image size
          K - kernel, h, w - kernel size
          Pw, Ph - padding size
          Sw, Sh - stride size
 * output: Y - image of size (Wx - w + 1) by (Hx - h + 1)
           flop - # of floating point operations
 */
template <class T, class U, class V>
void  corr2d0s_v1(T *X, unsigned Wx, unsigned Hx, U *K, unsigned w, unsigned h, V *Y, unsigned Pw, unsigned Ph, unsigned Sw, unsigned Sh, unsigned *flop) 
{
    unsigned row, col;
    unsigned i, j;

    unsigned Wy = (Wx - w + Pw + Sw) / Sw;
    unsigned Hy = (Hx - h + Ph + Sh) / Sh;

    *flop += Wy * Hy * w * h; // think about 0-padding

    for (row = 0; row < Hy; row++) {
        for (col = 0;  col < Wy; col++) {
            *(Y + row * Wy + col) = 0;
            for (i = 0; i < h; i++) {
                for (j = 0; j < w; j++) {
                        int Prow = Sh * row - Ph/2 + i;
                        int Pcol = Sw * col - Pw/2 + j;
                        if (Prow >= 0 && Pcol >=0 && Prow < Hx && Pcol < Wx)
			        *(Y + row *Wy + col) += *(X + (Prow) * Wx + (Pcol)) * (*(K + i * w + j));
                }
            }
         }
     }
}


template <class T, class U, class V>
void corr2d0_v2(T *X, unsigned Wx, unsigned Hx, U *K, unsigned w, unsigned h, V *Y, unsigned Pw, unsigned Ph, unsigned *flop) 
{
    unsigned Wz = Wx + Pw;
    unsigned Hz = Hx + Ph;

    T *Z = new float[Wz * Hz];

    unsigned row, col;

    for (row = 0; row < Hx; row++ ) {  
        for (col = 0; col < Wx; col++ ) {
             *(Z + (row+Ph/2) * Wz + (col+Pw/2)) = *(X + row * Wx + col); 
        }
    }

    corr2d<T, U, V>(Z, Wz, Hz, K, w, h, Y, flop);

    delete [] Z;
}

template <class T, class U, class V>
void corr2d0s_v2(T *X, unsigned Wx, unsigned Hx, U *K, unsigned w, unsigned h, V *Y, unsigned Pw, unsigned Ph, unsigned Sw, unsigned Sh, unsigned *flop)
{
    unsigned Wz = Wx + Pw;
    unsigned Hz = Hx + Ph;

    T *Z = new float[Wz * Hz];

    unsigned row, col;

    for (row = 0; row < Hx; row++ ) {  
        for (col = 0; col < Wx; col++ ) {
             *(Z + (row+Ph/2) * Wz + (col+Pw/2)) = *(X + row * Wx + col); 
        }
    }

    corr2ds<T, U, V>(Z, Wz, Hz, K, w, h, Y, Pw, Ph, Sw, Sh, flop);

    delete [] Z;
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
   for (unsigned y = 0; y < Hx; y++) {
     for (unsigned x = 0; x < Wx; x++) { 
        *(X+y*Wx+x) =  rand() % 256; 
     }
   }
}

template <class T>
void seq2d(T *X, unsigned Wx, unsigned Hx)
{
   T val = 0;

   for (unsigned y = 0; y < Hx; y++) {
     for (unsigned x = 0; x < Wx; x++) { 
        *(X+y*Wx+x) =  val++; 
     }
   }
}

/*

// should I create another array to supplement the padding I took out?
template <class T, class U, class V>
void  corr2d0s_v1(T *X, unsigned Wx, unsigned Hx, U *K, unsigned w, unsigned h, V *Y, unsigned Sw, unsigned Sh, unsigned *flop)
{
    unsigned row, col;
    unsigned i, j;

    unsigned Wy = (Wx - w + Sw - 1) / Sw;
    unsigned Hy = (Hx - h + Sh - 1) / Sh;

    *flop += Wy * Hy * w * h; // think about 0-padding

    for (row = 0; row < Hy; row++) {
        for (col = 0;  col < Wy; col++) {
            *(Y + row * Wy + col) = 0;
            for (i = 0; i < h; i++) {
                for (j = 0; j < w; j++) {
                        int Prow = Sh * row - Wx/2 + i;
                        int Pcol = Sw * col - Hx/2 + j;
                        if (Prow >= 0 && Pcol >=0 && Prow < Hx && Pcol < Wx)
                                *(Y + row *Wy + col) += *(X + (Prow) * Wx + (Pcol)) * (*(K + i * w + j));
                }
            }
         }
     }
}
Needs cleaning and approval from errors
template <class T, class U, class V>
void corr2ds(T *X, unsigned Wx, unsigned Hx, U *K, unsigned w, unsigned h, V *Y, unsigned Sw, unsigned Sh, unsigned *flop)
{
    unsigned row, col;
    unsigned i, j;
    unsigned Wy = (Wx + Sw  + w - 1 ) / Sw;
    unsigned Hy = (Hx + Sh + h - 1) / Sh;

    *flop += Wy * Hy * w * h;

    for (row = 0; row < Hy; row++) {
        for (col = 0;  col < Wy; col++) {
            *(Y + row * Wy + col) = 0;
            for (i = 0; i < h; i++) {
                for (j = 0; j < w; j++) {
                        int Prow = Sh * row - Ph/2 + i;
                        int Pcol = Sw * col - Pw/2 + j;

// modify me to add striding
                                *(Y + row *Wy + col) += *(X + (Prow) * Wx + (Pcol)) * (*(K + i * w + j));

                }
            }
         }
     }
}






//add padding and stride
template <class T, class U, class V>
void corr2d0s_v2(T *X, unsigned Wx, unsigned Hx, U *K, unsigned w, unsigned h, V *Y, unsigned Pw, unsigned Ph, unsigned Sw, unsigned Sh, unsigned *flop)
{
    unsigned Wz = (Wx - w + Pw + Sw) / Sw;
    unsigned Hz= (Hx - h + Ph + Sh) / Sh;


    T *Z = new float[Wz * Hz];

    unsigned row, col, prow, pcol;

    for (row = 0; row < Hx; row++ ) {
        for (col = 0; col < Wx; col++ ) {
                        prow = Sh * row - Ph/2 + i;
                        pcol = Sw * col - Pw/2 + j;
                        if (Prow >= 0 && Pcol >=0 && Prow < Hx && Pcol < Wx)

             *(Z + (row+Ph/2) * Wz + (col+Pw/2)) = *(X + row * Wx + col);


        }
    }

    corr2d<T, U, V>(Z, Wz, Hz, K, w, h, Y, flop);

    delete [] Z;
}








template <class T, class U, class V>
void  corr2d0s_v1(T *X, unsigned Wx, unsigned Hx, U *K, unsigned w, unsigned h, V *Y, unsigned Pw, unsigned Ph, unsigned Sw, unsigned Sh, unsigned *flop)
{
    unsigned row, col;
    unsigned i, j;

    unsigned Wy = (Wx - w + Pw + Sw) / Sw;
    unsigned Hy = (Hx - h + Ph + Sh) / Sh;

    *flop += Wy * Hy * w * h; // think about 0-padding

    for (row = 0; row < Hy; row++) {
        for (col = 0;  col < Wy; col++) {
            *(Y + row * Wy + col) = 0;
            for (i = 0; i < h; i++) {
                for (j = 0; j < w; j++) {
                        int Prow = Sh * row - Ph/2 + i;
                        int Pcol = Sw * col - Pw/2 + j;
                        if (Prow >= 0 && Pcol >=0 && Prow < Hx && Pcol < Wx)
                                *(Y + row *Wy + col) += *(X + (Prow) * Wx + (Pcol)) * (*(K + i * w + j));
                }
            }
         }
     }
}
*/

