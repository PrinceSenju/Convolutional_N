#ifndef _LIB_CORRSK_H_
#define _LIB_CORRSK_H_

#include <iostream>

// just convolution
template <class T, class U, class V>
void corrSK(T *X, unsigned Wx, unsigned Hx, U *Krow, U *Kcol, unsigned w, unsigned h, V *Y, unsigned *flop);

template <class T>
void random1d(T *X, unsigned Wx);

template <class T>
void seq1d(T *X, unsigned Wx);


#include "libcorrsk.cpp"

#endif

