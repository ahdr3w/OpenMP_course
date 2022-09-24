#include "omp.h"
#include<iostream>

int main() 
{
    int idx = 0;
    #pragma omp parallel
    {
        #pragma omp master
            idx = omp_get_num_threads() - 1;
        bool i = 1;
        int my_id = omp_get_thread_num();
            for(;i;) {
                if (my_id == idx)
                {
                    #pragma omp critical
                    {
                        std::cout << "Hello world! id: " << my_id << std::endl;
                        --idx;
                        i = 0;                        
                    }
                }
            }
        
    }
}