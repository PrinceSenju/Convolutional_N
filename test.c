#include <stdio.h> 
#include <time.h>
#include <unistd.h>
#include "libcorr.h"

// include header function time and input
int main() {

    clock_t start_clk, end_clk;

    const unsigned Wx = 3;
    const unsigned Hx = 3;
    int X[3*3] = { 0, 1, 2,   3, 4, 5,   6, 7, 8 };

    const unsigned w = 2;
    const unsigned h = 2;
    int K[2*2] = { 0, 1,  2, 3 };

    const unsigned Wy = Wx - w + 1;
    const unsigned Hy = Hx - h + 1;
    int Y[2*2];

    printf("Input matrix: \n");
    print2d(X, Wx, Hx);
    printf("\n");

    printf("Kernel: \n");
    print2d(K, w, h);
    printf("\n");

    start_clk = clock(); /* get the start time */

    corr2d(X, Wx, Hx, K, w, h, Y);

    end_clk = clock(); /* get the end time */

    printf("Output matrix: \n");
    print2d(Y, Wy, Hy);
    printf("\n");

    printf("Start time: %li\n", start_clk);
    printf("End time: %li\n", end_clk);

    printf("Processing time of program: %LE sec.\n", (end_clk - start_clk) / (long double) CLOCKS_PER_SEC);

    return 0;
}
