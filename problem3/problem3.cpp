#include "omp.h"
#include <iostream>

#define ARRAY_SIZE 100000

int operation(int a, int b, int c) 
{
        return a % 3 == 0 ? (a / 3) * b * c : (b % 3 == 0 ? a * (b / 3) * c : a * b * (c / 3));
}

int main() 
{
    int* array = new int[ARRAY_SIZE];
    for (int i = 0; i < ARRAY_SIZE; i++)
        array[i] = i;
    /*  
    #pragma omp parallel for schedule(dynamic, 100)
        {
             for (int i = 1; i < ARRAY_LENGTH - 1; ++i)   
                array[i] = operation(i - 1, i, i + 1); <--------------EZE WAY
        }
    */
   int init_var = array[0];
   int flag1 = 0, flag2 = 0;
   int num_threads = 1;
   #pragma omp parallel shared(array, init_var, num_threads)
    {
        int var = 0;
        int my_id = omp_get_thread_num();
        num_threads = omp_get_num_threads();
        #pragma omp for schedule(static, 1)
        for(int i = 1; i < ARRAY_SIZE; ++i) 
        {
            if (my_id > 0)
                var = operation(array[i - 1], array[i], array[i + 1]);
            else    
                var = operation(init_var, array[i], array[i + 1]);
            
            if (my_id == num_threads - 1)
                init_var = array[i];
            int idx1 = 1;
            int idx2 = 1;
            while(idx1) 
            {
                if (flag2 % num_threads != 0)
                    idx1 = 0;
            }
            #pragma omp critical
                ++flag1;
            
            while(idx2) 
            {
                if (flag2 % num_threads != 0)
                    idx2 = 0;
            }
            #pragma omp critical      
                ++flag2;

            array[i] = var;
            std::cout << 0;
        } 
    }

}