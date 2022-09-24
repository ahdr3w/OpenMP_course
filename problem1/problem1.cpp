#include "omp.h"
#include <iostream>
int main() {
    #pragma omp parallel 
    {
        #pragma omp critical
        {
            int my_id = omp_get_thread_num();
            std::cout << "Hello world! id: " << my_id << std::endl;
        }
    }
}