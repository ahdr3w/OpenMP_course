#include "omp.h"
#include <iostream>
#include <stdlib.h>

const int N_1 = 1000; // A[N_1 x M_1]
const int M_1 = 2000; //
const int N_2 = 2000; // B[N_2 x M_2]
const int M_2 = 1500; //

int** init_matrix(int N, int M) // init of random marix A and B
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
int** init_zeros(int N, int M) // init of matrix C[N_1 x M_2]
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
void del(int** matrix, int N) {
    for (int i = 0; i < N; ++i)
        delete[] matrix[i];
    delete[] matrix;
}

int main() 
{
    srand(42);
    if (M_1 != N_2)
        return -1;
    int** A = init_matrix(N_1, M_1);
    int** B = init_matrix(N_2, M_2);
    int** C = init_zeros(N_1, M_2);
    double t1 = omp_get_wtime();
    // linear algorithm needs 13.5s
    #pragma omp parallel for shared(A, B, C) schedule(dynamic) //3.7s working
    for (int k = 0; k < M_2; k++) //walking on columns of matrix B
        for (int i = 0; i < N_1; i++) //for each column of matrix B walking on lines of matrix A
        {
            int sum = 0;
            //#pragma omp shared(sum)
            //#pragma omp parallel for schedule(dynamic) reduction(+:sum) // ~158s working
            for (int j = 0; j < M_1; j++)   //calculating [i, k] element's value of matrix C
                sum += A[i][j] * B[j][k];  
            C[i][k] = sum;
        }
    
    std::cout << "Work time: " << omp_get_wtime() - t1 << std::endl;
    del(A, N_1);
    del(B, N_2);
    del(C, N_1);
}