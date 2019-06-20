
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

    for(j=0; j < cols; ++j)          // columns
    {
	for(i=0; i < rows; ++i)     // rows
	{
	  sum = 0;                 // init to 0 before sum
    	  for(m=0; m < kcol; ++m)     // kernel rows
          {
       	     mm = kcol - 1 - m;      // row index of flipped kernel
     
            y[i] += x[i - j] * h[j]; // convolve: multiply and accumulate

            }
        }
    }


// implement me
// find center position of kernel (half of kernel size)

for(i=0; i < rows; ++i)              // rows
{
    for(j=0; j < cols; ++j)          // columns
    {
        sum = 0;                     // init to 0 before sum

        for(m=0; m < kRows; ++m)     // kernel rows
        {
            mm = kRows - 1 - m;      // row index of flipped kernel

            x[i] += y[i - j] * h[j]; // convolve: multiply and accumulate

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


