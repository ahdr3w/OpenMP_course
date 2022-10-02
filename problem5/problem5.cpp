#include "omp.h"
#include <iostream>
#include <math.h>
double coeff(int n, double prev_coeff)
{
    return prev_coeff * (0.5 - n + 1) / n * (-1);
}

int main(int argc, char** argv) {
    double cff = 1;
    double x = 0.5;
    double pi = 0.5;
    for (int i = 1; i < 20; ++i) 
    {
        x *= 0.25;
        cff = coeff(i, cff);
        pi += cff * x / (2*i + 1);
    }
    pi = (pi - sqrt(3) / 8) * 12;
    printf("Newton's linear (most effective) algorithm: pi = %.14f, n = 20, time of work = 0 :) \n", pi); 
                               //precision - 14 signes after dot, n = 20. 
                              //So there is no reason to use parallel algorithm,
                              //thanks to Newton 

    unsigned long long n = atoi(argv[1]);//number of segments
    pi = 0;
    double h = 1. / n; //integration step
    
    double t1 = omp_get_wtime();
    #pragma omp parallel firstprivate(h, n)
    {    
        #pragma omp for schedule(dynamic, 1000) reduction(+:pi)
        for (unsigned long long i = 0; i < n; ++i) 
        {
            pi += h / (h * i * h * i + 1); // integrating the arctangent derivative from 0 to 1 gives quarter of pi
        }
    }
    pi *= 4;
    printf("pi = %.14f \n", pi);
    printf("Time of work: %f \n", omp_get_wtime() - t1);
}