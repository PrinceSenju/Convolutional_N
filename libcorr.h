#ifndef _LIB_CORR_H_
#define _LIB_CORR_H_

template <class T, class U, class V>
void corr2d(T *X, unsigned Wx, unsigned Hx, U *K, unsigned w, unsigned h, V *Y);

template <class T>
void print2d(T *X, unsigned Wx, unsigned Hx);
int flops = 0;
struct Double {
    double val;
    Double(double v): val(v) {}
    static int add_count;
    Double operator+(Double other) {
        ++add_count;
        return Double(val + other.val);
    }
    Double operator*(Double other) {
        ++add_count;
        return Double(val * other.val);
    }
     Double operator+(Double other) {
        ++add_count;
        return Double(val + other.val);
    }
     Double operator/(Double other) {
        ++add_count;
        return Double(val / other.val);
    }
     Double operator-(Double other) {
        ++add_count;
        return Double(val - other.val);
    }
};

#endif

