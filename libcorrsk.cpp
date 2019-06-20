
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

            unsigned rows, cols, kcol, krows, sum, mm;
	    unsigned i, j, m;
	   



    *flop += cols * rows * kcol * h;

    for(j=0; j < cols; ++j)          // columns
    {
	for(i=0; i < rows; ++i)     // rows
	{
	  sum = 0;                 // init to 0 before sum
    	  for(m=0; m < kcol; ++m)     // kernel rows
          {
       	     mm = kcol - 1 - m;      // row index of flipped kernel
     
       int     y[i] += int x[i - j] * int  h[j]; // convolve: multiply and accumulate
		*flop += cols * rows * kcol * h;
            }
        }
    }

// implement me

for(i=0; i < rows; ++i)              // rows
{
    for(j=0; j < cols; ++j)          // columns
    {
        sum = 0;                     // init to 0 before sum

        for(m=0; m < krows; ++m)     // kernel rows
        {
            mm = krows - 1 - m;      // row index of flipped kernel

     int       x[i] += int y[i - j] * int  h[j]; // convolve: multiply and accumulate

            }
        }
    }

       
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


