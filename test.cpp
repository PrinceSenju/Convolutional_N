// ./test -x 5 -y 5 -kw 3 -kh 3 -vs 1 -hs 1 -k 2 -m 1 -c 1 -o -p

#include <stdio.h> 
#include <sys/time.h>
#include <unistd.h>
#include "libcorr.h"
#include "libcorrsk.h"

/*
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

    const unsigned Wx = 20;
    const unsigned Hx = 20;
    float *X = new float[Wx * Hx];


    //random2d<float>(X, Wx, Hx);
    seq2d<float>(X, Wx, Hx);

    const unsigned w = 3;
    const unsigned h = 3;
    float *Kcol = new float[h];
    float *Krow = new float[w];

    seq2d(Kcol, 1, h);
    seq2d(Krow, w, 1);

//    const unsigned Pw = (w - 1) * 2;
//    const unsigned Ph = (h - 1) * 2;


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

    int iter = 1000;


    for (int i = 0; i < iter; i++)
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


void more_complex_test_sk()
{
    // mostly simple_test_sk(), but with code for 0-padding
    struct timeval start_clk, end_clk;
    double elapsed;

    const unsigned Wx = 5;
    const unsigned Hx = 5;
    float *X = new float[Wx * Hx];


    //random2d<float>(X, Wx, Hx);
    seq2d<float>(X, Wx, Hx);  

    const unsigned w = 3;
    const unsigned h = 3;


    float *Kcol = new float[h];
    float *Krow = new float [w];


    seq2d(Kcol, 1, h);
    seq2d(Krow, w, 1);

    const unsigned Pw = (w - 1) * 2;
    const unsigned Ph = (h - 1) * 2;


    const unsigned Hy = Hx - h + Ph + 1;
    const unsigned Wy = Wx - w + Pw + 1;
    float *Y = new float[Hy * Wy];

    unsigned flop = 0;

    std::cout << "Input matrix: " << std::endl;
    print2d<float>(X, Wx, Hx);
    std::cout << std::endl;


    std::cout << " 2 Kernels: " << std::endl;
    print2d<float>(Kcol, 1, h);
    print2d<float>(Krow, w, 1);
    std::cout << std::endl;

    gettimeofday(&start_clk, NULL); /// get the start time

    corrSK0_v1<float, float, float>(X, Wx, Hx, Krow, Kcol, w, h, Y, Ph, Pw, &flop);

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

void more_complex_test_sks()
{
// implement striding


}
*/

static void show_usage(std::string name)
{
    std::cerr << "Usage: " << name << " <options>\n"
              << "Options:\n"
              << "\t-c <count>\tSpecify the number of test iterations\n"
              << "\t-x <width>\tSpecify the input image width\n"
              << "\t-y <height>\tSpecify the input image height\n"
              << "\t-kw <width>\tSpecify the kernel width\n"
              << "\t-kh <height>\tSpecify the kernel height\n"
              << "\t-vs <stride>\tSpecify the vertical striding\n"
              << "\t-hs <stride>\tSpecify the horizontal striding\n"
              << "\t-p \t\tSpecify to use 0 padding\n"
              << "\t-o \t\tSpecify to output results\n"
              << "\t-k <kernel type>\tSpecify the kernel type: 1 for 2d kernel, 2 for separable kernel\n"
              << "\t-m <kernel model>\tSpecify the kernel model: 1 for direct and 2 for 0-pad first"
              << std::endl;
}

struct opts 
{
    unsigned count;
    unsigned Wx, Hx;
    unsigned h, w;
    bool p;
    bool output;
    unsigned Pw, Ph;
    unsigned Sw, Sh;
    unsigned ktype;
    unsigned kver;
};


static void show_options(struct opts opt)
{
    std::cout << "Options:\n"
              << "\t-x " << opt.Wx << "\n"
              << "\t-y " << opt.Hx << "\n"
              << "\t-kw " << opt.w << "\n"
              << "\t-kh " << opt.h << "\n"
              << "\t-vs " << opt.Sh << "\n"
              << "\t-hs " << opt.Sw << "\n"
              << "\t-vp " << opt.Ph << "\n"
              << "\t-hp " << opt.Pw << "\n"
              << "\t-k " << opt.ktype << "\n"
              << "\t-m " << opt.kver << "\n"
              << "\t-c " << opt.count << "\n"
              << "\t-o " << opt.output << "\n"
              << std::endl;
}

void run_test(struct opts opt) 
{
    float *X = new float[opt.Wx * opt.Hx];
    seq2d<float>(X, opt.Wx, opt.Hx);  

    float *K = new float[opt.h * opt.w];
    float *Kcol = new float[opt.h];
    float *Krow = new float [opt.w];
    seq2d(K, opt.w, opt.h);
    seq2d(Kcol, 1, opt.h);
    seq2d(Krow, opt.w, 1);

    const unsigned Wy = (opt.Wx - opt.w + opt.Pw + opt.Sw) / opt.Sw;
    const unsigned Hy = (opt.Hx - opt.h + opt.Ph + opt.Sh) / opt.Sh;
    float *Y = new float[Wy * Hy];

    unsigned flop = 0;

    struct timeval start_clk, end_clk;
    double elapsed;

    gettimeofday(&start_clk, NULL); /// get the start time

    if (opt.ktype == 1) // 2d kernel
    {
         if (opt.kver == 1) {
              for (unsigned i = 0; i < opt.count; i++)
              corr2d0s_v1<float, float, float>(X, opt.Wx, opt.Hx, K, opt.w, opt.h, Y, opt.Pw, opt.Ph, opt.Sw, opt.Sh, &flop);
         } else if (opt.kver == 2) {
              for (unsigned i = 0; i < opt.count; i++)
              corr2d0s_v2<float, float, float>(X, opt.Wx, opt.Hx, K, opt.w, opt.h, Y, opt.Pw, opt.Ph, opt.Sw, opt.Sh, &flop);
         } else {
             std::cerr << "Unknown kernel version " << opt.kver << std::endl;
             return;
         }
    }
    else if (opt.ktype == 2) // separable kernel
    {
         if (opt.kver == 1) {
              for (unsigned i = 0; i < opt.count; i++)
              corrSK0_v1<float, float, float>(X, opt.Wx, opt.Hx, Krow, Kcol, opt.w, opt.h, Y, opt.Pw, opt.Ph, /*opt.Sw, opt.Sh,*/ &flop);
         } else if (opt.kver == 2) {
              for (unsigned i = 0; i < opt.count; i++)
              corrSK0_v2<float, float, float>(X, opt.Wx, opt.Hx, Krow, Kcol, opt.w, opt.h, Y, opt.Pw, opt.Ph, /*opt.Sw, opt.Sh,*/ &flop);
         } else {
             std::cerr << "Unknown kernel version " << opt.kver << std::endl;
             return;
         }
    }
    else {
        std::cerr << "Unknown kernel type " << opt.ktype << std::endl;
        return;
    }

    gettimeofday(&end_clk, NULL); /// get the start time

    if (opt.output) {
       std::cout << "Input matrix: " << std::endl;
       print2d<float>(X, opt.Wx, opt.Hx);
       std::cout << std::endl;

       if (opt.ktype == 1) {
          std::cout << "Kernel: " << std::endl;
          print2d<float>(K, opt.w, opt.h);
          std::cout << std::endl;
       } else if (opt.ktype == 2) {
          std::cout << "2 kernels: " << std::endl;
          print2d<float>(Kcol, 1, opt.h);
          print2d<float>(Krow, opt.w, 1);
          std::cout << std::endl;
       } else {
           std::cerr << "Unknown kernel type " << opt.ktype << std::endl;
           return; 
       }

       std::cout << "Output matrix: " << std::endl;
       print2d<float>(Y, Wy, Hy);
       std::cout << std::endl;
    }

    std::cout << "Number of floating-point operations: " << flop << std::endl;    
    elapsed = (end_clk.tv_sec - start_clk.tv_sec) + (end_clk.tv_usec - start_clk.tv_usec) / 1000000.0;
    std::cout << "Elapsed time: " << elapsed << std::endl;
    std::cout << "FLOPS: " << flop / elapsed << std::endl;

    delete [] X;
    delete [] Y;
    delete [] K;
    delete [] Krow;
    delete [] Kcol;

}


// include header function time and input
int main(int argc, char* argv[]) {

    struct opts opt;

    if (argc < 18) {
        show_usage(argv[0]);
        return 1;
    }

    // default options
    opt.count = 1;
    opt.Wx = opt.Hx = 256;
    opt.h = opt.w = 3;
    opt.p = false;
    opt.output = false;
    opt.Sw = 1; opt.Sh = 1;
    opt.ktype = 1;
    opt.kver = 1;


    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "-x") {
            if (i + 1 < argc) { 
              opt.Wx = std::atoi(argv[++i]);
            } else { 
                std::cerr << "-x option requires one argument." << std::endl;
                return 1;
            }
        }
        if (std::string(argv[i]) == "-y") {
            if (i + 1 < argc) { 
              opt.Hx = std::atoi(argv[++i]);
            } else { 
                std::cerr << "-y option requires one argument." << std::endl;
                return 1;
            }  
        }
        if (std::string(argv[i]) == "-kw") {
            if (i + 1 < argc) { 
              opt.w = std::atoi(argv[++i]);
            } else { 
                std::cerr << "-kw option requires one argument." << std::endl;
                return 1;
            }  
        }
        if (std::string(argv[i]) == "-kh") {
            if (i + 1 < argc) { 
              opt.h = std::atoi(argv[++i]);
            } else { 
                std::cerr << "-kh option requires one argument." << std::endl;
                return 1;
            }  
        }
        if (std::string(argv[i]) == "-vs") {
            if (i + 1 < argc) { 
              opt.Sh = std::atoi(argv[++i]);
            } else { 
                std::cerr << "-vs option requires one argument." << std::endl;
                return 1;
            }  
        }
        if (std::string(argv[i]) == "-hs") {
            if (i + 1 < argc) { 
              opt.Sw = std::atoi(argv[++i]);
            } else { 
                std::cerr << "-hs option requires one argument." << std::endl;
                return 1;
            }  
        }

        if (std::string(argv[i]) == "-p") {
             opt.p = true;
        }
        if (std::string(argv[i]) == "-o") {
             opt.output = true;
        }
        if (std::string(argv[i]) == "-k") {
            if (i + 1 < argc) { 
              opt.ktype = std::atoi(argv[++i]);
            } else { 
                std::cerr << "-k option requires one argument." << std::endl;
                return 1;
            }  
        }
        if (std::string(argv[i]) == "-m") {
            if (i + 1 < argc) { 
              opt.kver = std::atoi(argv[++i]);
            } else { 
                std::cerr << "-m option requires one argument." << std::endl;
                return 1;
            }  
        }
        if (std::string(argv[i]) == "-c") {
            if (i + 1 < argc) { 
              opt.count = std::atoi(argv[++i]);
            } else { 
                std::cerr << "-c option requires one argument." << std::endl;
                return 1;
            }  
        }
    }

    // compute opt.Pw, opt.Ph if -p is true, or set then to 0 if -p is false
    if (opt.p) {
       opt.Pw = (opt.w - 1) * 2; 
       opt.Ph = (opt.h - 1) * 2;
    } else {
       opt.Pw = 0; 
       opt.Ph = 0;
    }


    show_options(opt);

    run_test(opt);

    return 0;
}
