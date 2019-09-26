// ./test -x 5 -y 5 -kw 3 -kh 3 -vs 1 -hs 1 -k 2 -m 1 -c 1 -v -p

#include <stdio.h> 
#include <sys/time.h>
#include <unistd.h>
#include "libcorr.h"
#include "libcorrsk.h"


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
              << "\t-v \t\tSpecify to output results\n"
              << "\t-k <kernel type>\tSpecify the kernel type: 1 for 2d kernel, 2 for separable kernel\n"
              << "\t-m <kernel model>\tSpecify the kernel model: 1 for direct and 2 for 0-pad first"
              << "\t-i <multiple input model>\tSpecify the number of input channels\n"
              << "\t-o <multiple output model>\tSpecify the number of output channels\n"
              << std::endl;
}

struct opts 
{
    unsigned count; //test iterations
    unsigned Wx, Hx; //image width, height
    unsigned h, w; //kernel width, height
    bool p; // Specify to use 0 padding
    bool output; // to output results
    unsigned Pw, Ph; // pading in width and height
    unsigned Sw, Sh; //stride in width and height
    unsigned ktype; 
    unsigned kver;
    unsigned Nx; //# of input channels
    unsigned n; //# of output channels
};


static void show_options(struct opts opt)
{
    std::cout << "Options:\n"
              << "\tNumber of iterations: \t-c " << opt.count << "\n"
              << "\tImage size: \t-x " << opt.Wx << "\t-y " << opt.Hx << "\n"
              << "\tKernel size: \t-kw " << opt.w << "\t-kh " << opt.h << "\n"
              << "\tStriding: \t-vs " << opt.Sh << "\t-hs " << opt.Sw << "\n"
              << "\tPadding: \t-vp " << opt.Ph << "\t-hp " << opt.Pw << "\n"
              << "\tKernel Type: \t\t-k " << opt.ktype << ((opt.ktype == 1) ? " - 2D" : " - Separable") << "\n"
              << "\tImplementation style:\t-m " << opt.kver << ((opt.kver== 1) ? " - inline" : " - external") << " 0-padding\n"
              << "\tOutput: \t\t-v " << opt.output << ((opt.output) ? " - enabled" : " - disabled") << "\n"
              << "\tNumber of input channels: \t-i " << opt.Nx << "\n"
              << "\tNumber of output channels: \t-o " << opt.n << "\n"
              << std::endl;
}

static bool get_options(int argc, char* argv[], struct opts &opt)
{
    std::cout << "get_options" << std::endl;
    // default options
    opt.count = 1;
    opt.Wx = opt.Hx = 256;
    opt.h = opt.w = 3;
    opt.p = false;  
    opt.output = false; 
    opt.Sw = 1; opt.Sh = 1; 
    opt.ktype = 1;
    opt.kver = 1;
    opt.Nx = 1;
    opt.n = 1;
    
    // assgin the structure with our inputs
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "-x") {
            if (i + 1 < argc) { 
              opt.Wx = std::atoi(argv[++i]);
            } else { 
                std::cerr << "-x option requires one argument." << std::endl;
                return false;
            }
        }
        if (std::string(argv[i]) == "-y") {
            if (i + 1 < argc) { 
              opt.Hx = std::atoi(argv[++i]);
            } else { 
                std::cerr << "-y option requires one argument." << std::endl;
                return false;
            }  
        }
        if (std::string(argv[i]) == "-kw") {
            if (i + 1 < argc) { 
              opt.w = std::atoi(argv[++i]);
            } else { 
                std::cerr << "-kw option requires one argument." << std::endl;
                return false;
            }  
        }
        if (std::string(argv[i]) == "-kh") {
            if (i + 1 < argc) { 
              opt.h = std::atoi(argv[++i]);
            } else { 
                std::cerr << "-kh option requires one argument." << std::endl;
                return false;
            }  
        }
        if (std::string(argv[i]) == "-vs") {
            if (i + 1 < argc) { 
              opt.Sh = std::atoi(argv[++i]);
            } else { 
                std::cerr << "-vs option requires one argument." << std::endl;
                return false;
            }  
        }
        if (std::string(argv[i]) == "-hs") {
            if (i + 1 < argc) { 
              opt.Sw = std::atoi(argv[++i]);
            } else { 
                std::cerr << "-hs option requires one argument." << std::endl;
                return false;
            }  
        }

        if (std::string(argv[i]) == "-p") {
             opt.p = true;
        }
        if (std::string(argv[i]) == "-v") {
             opt.output = true;
        }
        if (std::string(argv[i]) == "-k") {
            if (i + 1 < argc) { 
              opt.ktype = std::atoi(argv[++i]);
            } else { 
                std::cerr << "-k option requires one argument." << std::endl;
                return false;
            }  
        }
        if (std::string(argv[i]) == "-m") {
            if (i + 1 < argc) { 
              opt.kver = std::atoi(argv[++i]);
            } else { 
                std::cerr << "-m option requires one argument." << std::endl;
                return false;
            }  
        }
        if (std::string(argv[i]) == "-c") {
            if (i + 1 < argc) { 
              opt.count = std::atoi(argv[++i]);
            } else { 
                std::cerr << "-c option requires one argument." << std::endl;
                return false;
            }  
        }
        if (std::string(argv[i]) == "-i") {
            if (i + 1 < argc) { 
              opt.Nx = std::atoi(argv[++i]);
            } else { 
                std::cerr << "-i option requires one argument." << std::endl;
                return false;
            }  
        }
        if (std::string(argv[i]) == "-o") {
            if (i + 1 < argc) { 
              opt.n = std::atoi(argv[++i]);
            } else { 
                std::cerr << "-o option requires one argument." << std::endl;
                return false;
            }  
        }
    }

    // compute opt.Pw, opt.Ph if -p is true, or set then to 0 if -p is false
    if (opt.p) {
       opt.Pw = opt.Wx * opt.Sw + opt.w - opt.Wx - opt.Sw; 
       opt.Ph = opt.Hx * opt.Sh + opt.h - opt.Hx - opt.Sh; 
    } else {
       opt.Pw = 0; 
       opt.Ph = 0;
    }

    return true;
}

void run_test(struct opts opt) 
{
    std::cout << "run_test " << std::endl;
    float *X = new float[opt.Nx * opt.Wx * opt.Hx];
    seq3d<float>(X, opt.Wx, opt.Hx);  

    float *K = new float[opt.n * opt.Nx * opt.h * opt.w];
    float *Kcol = new float[opt. n* opt.Nx * opt.h];
    float *Krow = new float [opt.n * opt.Nx * opt.w];

    // create a 2d array and
    seq3d(K, opt.w, opt.h);
    seq3d(Kcol, 1, opt.h);
    seq3d(Krow, opt.w, 1);
    std::cout << "opt.Wx " << opt.Wx << std::endl;
    std::cout << "opt.w " << opt.w << std::endl;
    std::cout << "opt.Pw " << opt.Pw << std::endl;
    std::cout << "opt.Ph " << opt.Ph << std::endl;
    const unsigned Wy = (opt.Wx - opt.w + opt.Pw + opt.Sw) / opt.Sw;
    const unsigned Hy = (opt.Hx - opt.h + opt.Ph + opt.Sh) / opt.Sh;
    float *Y = new float[opt.n * Wy * Hy]; 
    // we have to change Y when testing with multiple inputs and outputs

    unsigned long long flop = 0;

    struct timeval start_clk, end_clk;
    double elapsed;

    gettimeofday(&start_clk, NULL); /// get the start time

    if (opt.ktype == 1) // 2d kernel
    {
         if (opt.kver == 1) {
              for (unsigned i = 0; i < opt.count; i++)
              corr3d0s_v1<float, float, float>(X, opt.Wx, opt.Hx, K, opt.w, opt.h, Y, opt.Pw, opt.Ph, opt.Sw, opt.Sh, &flop);
         } else if (opt.kver == 2) {
              for (unsigned i = 0; i < opt.count; i++)
              corr3d0s_v2<float, float, float>(X, opt.Wx, opt.Hx, K, opt.w, opt.h, Y, opt.Pw, opt.Ph, opt.Sw, opt.Sh, &flop);
         } else {
             std::cerr << "Unknown kernel version " << opt.kver << std::endl;
             return;
         }
    }
    else if (opt.ktype == 2) // separable kernel
    {
         if (opt.kver == 1) {
              for (unsigned i = 0; i < opt.count; i++)
              corrSK3d0s_v1<float, float, float>(X, opt.Wx, opt.Hx, Krow, Kcol, opt.w, opt.h, Y, opt.Pw, opt.Ph, opt.Sw, opt.Sh, &flop);
         } else if (opt.kver == 2) {
              for (unsigned i = 0; i < opt.count; i++)
              corrSK3d0s_v2<float, float, float>(X, opt.Wx, opt.Hx, Krow, Kcol, opt.w, opt.h, Y, opt.Pw, opt.Ph, opt.Sw, opt.Sh, &flop);
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
       print4d<float>(X, 1, opt.Nx, opt.Wx, opt.Hx);
       std::cout << std::endl;

       if (opt.ktype == 1) {
          std::cout << "Kernel: " << std::endl;
          print4d<float>(K, opt.n, opt.Nx, opt.w, opt.h);
          std::cout << std::endl;
       } else if (opt.ktype == 2) {
          std::cout << "2 kernels: " << std::endl;
          print4d<float>(Kcol, opt.n, opt.Nx, 1, opt.h);
          print4d<float>(Krow, opt.n, opt.Nx, opt.w, 1);
          std::cout << std::endl;
       } else {
           std::cerr << "Unknown kernel type " << opt.ktype << std::endl;
           return; 
       }

       std::cout << "Output matrix: " << std::endl;
       print4d<float>(Y, opt.n, 1, Wy, Hy);
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

    if (argc < 19) {
        show_usage(argv[0]);
        return 1;
    }

    if (get_options(argc, argv, opt))
    {
        show_options(opt);
        run_test(opt);
    }

    return 0;
}
