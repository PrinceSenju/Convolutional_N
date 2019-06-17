#ifndef _LIB_CORR_H_
#define _LIB_CORR_H_

#include <iostream>

// just convolution
template <class T, class U, class V>
void corr2d(T *X, unsigned Wx, unsigned Hx, U *K, unsigned w, unsigned h, V *Y, unsigned *flop);

// convolution + stride (knowing padding size)
template <class T, class U, class V>
void corr2ds(T *X, unsigned Wx, unsigned Hx, U *K, unsigned w, unsigned h, V *Y, unsigned Pw, unsigned Ph, unsigned Sw, unsigned Sh, unsigned *flop);

// 0-padding no stride V1
template <class T, class U, class V>
void  corr2d0_v1(T *X, unsigned Wx, unsigned Hx, U *K, unsigned w, unsigned h, V *Y, unsigned Pw, unsigned Ph, unsigned *flop);

// 0-padding no stride V2
template <class T, class U, class V>
void  corr2d0_v2(T *X, unsigned Wx, unsigned Hx, U *K, unsigned w, unsigned h, V *Y, unsigned Pw, unsigned Ph, unsigned *flop);

// 0-padding + stride V1
template <class T, class U, class V>
void  corr2d0s_v1(T *X, unsigned Wx, unsigned Hx, U *K, unsigned w, unsigned h, V *Y, unsigned Pw, unsigned Ph, unsigned Sw, unsigned Sh, unsigned *flop);

// 0-padding + stride V2
template <class T, class U, class V>
void corr2d0s_v2(T *X, unsigned Wx, unsigned Hx, U *K, unsigned w, unsigned h, V *Y, unsigned Pw, unsigned Ph, unsigned Sw, unsigned Sh, unsigned *flop);

template <class T>
void print2d(T *X, unsigned Wx, unsigned Hx);

template <class T>
void random2d(T *X, unsigned Wx, unsigned Hx);

template <class T>
void seq2d(T *X, unsigned Wx, unsigned Hx);


#include "libcorr.cpp"

#endif

