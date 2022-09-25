#include "omp.h"
#include<iostream>

int main(int argc, char** argv) 
{
    unsigned long long N = atoi(argv[1]);
    unsigned long long sum = 0;
    #pragma omp parallel for schedule(dynamic, 1) reduction(+:sum)
        for(int i = 0; i <= N; ++i)
            sum += i;
    std::cout << "1 + ... + " << N << " = " << sum << std::endl;
}