/*
#include <stdio.h>
#include "libcorr.h"

int main()
{

   int X[6] = { 1, 2, 3,    4, 5, 6 };

   print2d(X, 3, 2); 




    return 0;
}




*/
#include <stdio.h> 
#include <time.h>
#include "libcorr.h"
// include header function time and input
 int main(void) //type with main {

    clock_t start_clk = clock(); //declaeration of function
    /*
        put any code here
    */
    int X[6] = {1, 2, 3,   4, 5, 6};

    print2d(X, 3, 2);

    printf(" Test Rub for code and more ");
    //print function for test
    printf("Processing time of program: %lg sec.\n", \
    //prints output
    (clock() - start_clk) / (long double) CLOCKS_PER_SEC);
    //time function with subtraction and division to convert to a long int
    return 0;
    // gives back output of time function
}
