#include "omp.h"
#include <iostream>
#include <math.h>
double coeff(int n, double prev_coeff) //func is used in pi_linear
{
    return prev_coeff * (0.5 - n + 1) / n * (-1);
}
void pi_linear()
{
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
}

void pi_parallel(unsigned long long n)
{
    double pi = 0;
    double h = 1. / n; //integration step
    
    
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
}

void exp_parallel(unsigned long long n)
{
    double exp = 0;
    double val = 1;
    #pragma omp parallel firstprivate(val)
    {    
        #pragma omp for schedule(dynamic, 1000) reduction(+: exp)
        for (int i = 0; i < n; i++)
        {
            for (int j = 1; j <= i; ++j) //calculating i!
                val /= j;
            exp += val; // 1/1! + 1/2! + ... + 1/(n-1)!
            val = 1;
        }
    }   
    printf("exp = %.13f\n", exp);
}


int main(int argc, char** argv) {
    unsigned long long n1 = atoi(argv[1]);//number of segments(for pi)
    unsigned long long n2 = atoi(argv[2]);//number of iterations(for exp)
    pi_linear();

    double t1 = omp_get_wtime();
    pi_parallel(n1);
    printf("Time of work on calculating pi: %f \n", omp_get_wtime() - t1);

    double t2 = omp_get_wtime();
    exp_parallel(n2);
    printf("Time of work on calculating exp: %f \n", omp_get_wtime() - t2);
}