#include "omp.h"
#include<iostream>

int main(int argc, char** argv) {
    unsigned long long N = atoi(argv[argc - 1]);
    unsigned long long sum;
    #pragma omp parallel for shared(N) schedule(static) reduction(+:sum)
        for(int i = 0; i <= N; ++i)
            sum += i;
    std::cout << "1 + ... + " << N << " = " << sum << std::endl;
}