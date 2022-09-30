#include "omp.h"
#include <iostream>

#define ARRAY_SIZE 100000

int operation(int a, int b, int c) 
{   
    unsigned long long var = 0;
    if (a % 3 == 0) 
    {   
        var = a / 3;
        return var * b * c;
    }  
     if (b % 3 == 0) 
    {   
        var = b / 3;
        return var * a * c;
    }  
     if (c % 3 == 0) 
    {   
        var = c / 3;
        return var * a * b;
    }  
}

int main() 
{
    unsigned long long* array  = new unsigned long long[ARRAY_SIZE];
    unsigned long long* array2 = new unsigned long long[ARRAY_SIZE];
    for (int i = 0; i < ARRAY_SIZE; i++)
        array[i] = i;
   array2[0] = array[0];
   array2[ARRAY_SIZE - 1] = array[ARRAY_SIZE - 1];
   #pragma omp parallel shared(array, array2)
    {
        int my_id = omp_get_thread_num();
        #pragma omp for schedule(dynamic)
        for(int i = 1; i < ARRAY_SIZE; ++i) 
        {
           array2[i] = operation(array[i-1],array[i], array[i+1]);
           //printf("thread %d changed value %llu to %llu \n", my_id, array[i], array2[i]); 
        } 
    } 
    
    return 0;

}