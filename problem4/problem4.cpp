#include "omp.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
const int N_1 = 500;
const int M_1 = 1000;
const int N_2 = 1000;
const int M_2 = 700;

int** init_matrix(int N, int M) 
{
    int** matrix = new int*[N];
    for (int i = 0; i < N; i++)
    {
        matrix[i] = new int[M];
        for (int j = 0; j < M; j++)
            matrix[i][j] = rand() % 100;
    }
    return matrix;
}
int** init_zeros(int N, int M) 
{
    int** matrix = new int*[N];
    for (int i = 0; i < N; i++)
    {
        matrix[i] = new int[M];
        for (int j = 0; j < M; j++)
            matrix[i][j] = 0;
    }
    return matrix;
}

int main() 
{
    srand(42);
    if (M_1 != N_2)
        return -1;
    int** A = init_matrix(N_1, M_1);
    int** B = init_matrix(N_2, M_2);
    int** C = init_zeros(N_1, M_2);
    int sum = 0;
    #pragma omp prallel shared(A, B, C, sum)
    for (int k = 0; k < M_2; k++)    
        
        for (int i = 0; i < N_1; i++) 
        {
            
            #pragma omp for reduction(+:sum)
            for (int j = 0; j < M_1; j++)
                sum += A[i][j] * B[j][k];  
            C[i][k] = sum;
        }
}