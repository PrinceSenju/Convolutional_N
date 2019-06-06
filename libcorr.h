#ifndef _LIB_CORR_H_
#define _LIB_CORR_H_

template <class T, class U, class V>
void corr2d(T *X, unsigned Wx, unsigned Hx, U *K, unsigned w, unsigned h, V *Y);

template <class T>
void print2d(T *X, unsigned Wx, unsigned Hx);


#endif

