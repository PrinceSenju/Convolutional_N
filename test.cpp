#include <stdio.h> 
#include <sys/time.h>
#include <unistd.h>
#include "libcorr.h"
#include "libcorrsk.h"

void simple_test() 
{
    struct timeval start_clk, end_clk;
    double elapsed;

    const unsigned Wx = 3;
    const unsigned Hx = 3;
    float X[3*3] = { 0, 1, 2,   3, 4, 5,   6, 7, 8 };

    const unsigned w = 2;
    const unsigned h = 2;
    // float K[5*5] = { Wx, X, Hx};
    float K[2*2] = { 0.0, 1.0,  2.0, 3.0 };

    const unsigned Wy = Wx - w + 1;
    const unsigned Hy = Hx - h + 1;
    float Y[2*2];

    unsigned flop = 0;

    std::cout << "Input matrix: " << std::endl;
    print2d<float>(X, Wx, Hx);
    std::cout << std::endl;

    std::cout << "Kernel: " << std::endl;
    print2d<float>(K, w, h);
    std::cout << std::endl;

    gettimeofday(&start_clk, NULL); /// get the start time

    corr2d<float, float, float>(X, Wx, Hx, K, w, h, Y, &flop);

    gettimeofday(&end_clk, NULL); /// get the end time

    std::cout << "Output matrix: " << std::endl;
    print2d<float>(Y, Wy, Hy);
    std::cout << std::endl;

    //std::cout << "Start time: " << start_clk << std::endl;
    //std::cout << "End time: " << end_clk << std::endl;
    std::cout << "Number of floating-point operations: " << flop << std::endl;    
    elapsed = (end_clk.tv_sec - start_clk.tv_sec) + (end_clk.tv_usec - start_clk.tv_usec) / 1000000.0;
    std::cout << "Elapsed time: " << elapsed << std::endl;    
    std::cout << "FLOPS: " << flop / elapsed << std::endl;
}

void more_complex_test()
{
    struct timeval start_clk, end_clk;
    double elapsed;

    const unsigned Wx = 256;
    const unsigned Hx = 256;
    float *X = new float[Wx * Hx];

    //random2d<float>(X, Wx, Hx);
    seq2d<float>(X, Wx, Hx);  

    const unsigned w = 9;
    const unsigned h = 9;
    float *K = new float[w * h];

    //random2d(K, w, h);
    seq2d(K, w, h);

    const unsigned Pw = (w - 1) * 2; 
    const unsigned Ph = (h - 1) * 2;

    const unsigned Sw = 2;  // stride of 2 for width
    const unsigned Sh = 3;  // stride of 3 for height

    const unsigned Wy = (Wx - w + Pw + Sw) / Sw;
    const unsigned Hy = (Hx - h + Ph + Sh) / Sh;

    float *Y = new float[Wy * Hy];

    unsigned flop = 0;

    std::cout << "Input matrix: " << std::endl;
    //print2d<float>(X, Wx, Hx);
    std::cout << std::endl;

    std::cout << "Kernel: " << std::endl;
    print2d<float>(K, w, h);
    std::cout << std::endl;

    int iters = 1000;

    gettimeofday(&start_clk, NULL); /// get the start time

    for (int i = 0; i < iters; i++)
    corr2d0s_v1<float, float, float>(X, Wx, Hx, K, w, h, Y, Pw, Ph, Sw, Sh, &flop);

    gettimeofday(&end_clk, NULL); /// get the end time

    std::cout << "Output matrix: " << std::endl;
    //print2d<float>(Y, Wy, Hy);
    std::cout << std::endl;

    //std::cout << "Start time: " << start_clk << std::endl;
    //std::cout << "End time: " << end_clk << std::endl;
    std::cout << "Number of floating-point operations: " << flop << std::endl;
    elapsed = (end_clk.tv_sec - start_clk.tv_sec) + (end_clk.tv_usec - start_clk.tv_usec) / 1000000.0;
    std::cout << "Elapsed time per iteration: " << elapsed/iters << std::endl;
    std::cout << "FLOPS: " << flop / elapsed << std::endl;

    delete [] X;
    delete [] K;
    delete [] Y;

}

// MODIFY ME TO USE corrSK
void simple_test_sk() 
{
    struct timeval start_clk, end_clk;
    double elapsed;

    const unsigned Wx = 15;
    const unsigned Hx = 15;
    float *X = new float[Wx * Hx];


    //random2d<float>(X, Wx, Hx);
    seq2d<float>(X, Wx, Hx);  

    const unsigned w = 3;
    const unsigned h = 3;
    float *Kcol = new float[h];
    float *Krow = new float[w];

    seq2d(Kcol, 1, h);
    seq2d(Krow, w, 1);

    const unsigned Hz = Hx - h + 1;
    const unsigned Wz = Wx - w + 1;
    float *Y = new float[Hz * Wz];

    unsigned flop = 0;

    std::cout << "Input matrix: " << std::endl;
    print2d<float>(X, Wx, Hx);
    std::cout << std::endl;

    std::cout << "2 kernels: " << std::endl;
    print2d<float>(Kcol, 1, h);
    print2d<float>(Krow, w, 1);
    std::cout << std::endl;

    gettimeofday(&start_clk, NULL); /// get the start time

    corrSK<float, float, float>(X, Wx, Hx, Krow, Kcol, w, h, Y, &flop);

    gettimeofday(&end_clk, NULL); /// get the end time

    std::cout << "Output matrix: " << std::endl;
    print2d<float>(Y, Wz, Hz);
    std::cout << std::endl;

    //std::cout << "Start time: " << start_clk << std::endl;
    //std::cout << "End time: " << end_clk << std::endl;
    std::cout << "Number of floating-point operations: " << flop << std::endl;    
    elapsed = (end_clk.tv_sec - start_clk.tv_sec) + (end_clk.tv_usec - start_clk.tv_usec) / 1000000.0;
    std::cout << "Elapsed time: " << elapsed << std::endl;    
    std::cout << "FLOPS: " << flop / elapsed << std::endl;
}


void simple_test_2d() 
{
    struct timeval start_clk, end_clk;
    double elapsed;

    const unsigned Wx = 15;
    const unsigned Hx = 15;
    float *X = new float[Wx * Hx];


    //random2d<float>(X, Wx, Hx);
    seq2d<float>(X, Wx, Hx);  

    const unsigned w = 3;
    const unsigned h = 3;
    float *K = new float[h * w];

    seq2d(K, w, h);

    const unsigned Hz = Hx - h + 1;
    const unsigned Wz = Wx - w + 1;
    float *Y = new float[Hz * Wz];

    unsigned flop = 0;

    std::cout << "Input matrix: " << std::endl;
    print2d<float>(X, Wx, Hx);
    std::cout << std::endl;

    std::cout << "Kernel: " << std::endl;
    print2d<float>(K, w, h);
    std::cout << std::endl;

    gettimeofday(&start_clk, NULL); /// get the start time

    corr2d<float, float, float>(X, Wx, Hx, K, w, h, Y, &flop);

    gettimeofday(&end_clk, NULL); /// get the end time

    std::cout << "Output matrix: " << std::endl;
    print2d<float>(Y, Wz, Hz);
    std::cout << std::endl;

    //std::cout << "Start time: " << start_clk << std::endl;
    //std::cout << "End time: " << end_clk << std::endl;
    std::cout << "Number of floating-point operations: " << flop << std::endl;    
    elapsed = (end_clk.tv_sec - start_clk.tv_sec) + (end_clk.tv_usec - start_clk.tv_usec) / 1000000.0;
    std::cout << "Elapsed time: " << elapsed << std::endl;
    std::cout << "FLOPS: " << flop / elapsed << std::endl;
}


void more_complex_test_sk()
{
    // mostly simple_test_sk(), but with code for 0-padding
}


// include header function time and input
int main() {

    std::cout << "2D kernel" << std::endl;
    simple_test_2d();
    std::cout << std::endl;

    std::cout << "Separable kernel" << std::endl;
    simple_test_sk();
    std::cout << std::endl;


    //more_complex_test();

    return 0;
}
