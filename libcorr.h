#ifndef _LIB_CORR_H_
#define _LIB_CORR_H_

#include <iostream>

template <class T, class U, class V>
void corr2d(T *X, unsigned Wx, unsigned Hx, U *K, unsigned w, unsigned h, V *Y, unsigned *flop);

template <class T>
void print2d(T *X, unsigned Wx, unsigned Hx);

template <class T>
void random2d(T *X, unsigned Wx, unsigned Hx);


#include "libcorr.cpp"

#endif

