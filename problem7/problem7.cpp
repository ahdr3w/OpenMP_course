#include "omp.h"
#include <iostream>

using namespace std;

#define INIT_VAL 100
#define CURANT 0.25

float** initLayer (int N, int M)
{
    float** T = new float*[M];
    for (int i = 0; i < M; ++i)
        T[i] = new float[N];
    return T;
}

int main(int argc, char**argv)
{
    int M         = atoi(argv[1]);
    int N         = atoi(argv[2]);
    float epsilon = atof(argv[3]);
        
    int counter = 0;

    float** T_1 = initLayer(N, M);
    float** T_2 = initLayer(N, M);

    int n_threads = omp_get_max_threads();

    float* max  = new float[n_threads];
    float* min  = new float[n_threads];
    bool* check = new bool[n_threads];

    float max_diff = epsilon + 1;

    double t1 = omp_get_wtime();

    for (int j = 0; j < N; ++j)
        T_1[0][j] = INIT_VAL;
    for (int i = 1; i < M; ++i)
    {
        T_1[i][0]     = INIT_VAL;
        T_1[i][N - 1] = INIT_VAL;
        for (int j = 1; j < N - 1; ++j)
            T_1[i][j] = 0;
    }

    #pragma omp parallel shared(T_1,       \
                                T_2,       \
                                N, M,      \
                                epsilon,   \
                                n_threads, \
                                max,       \
                                min,       \
                                max_diff) 
    {
        #pragma omp single
        {
            for (;max_diff > epsilon;)
            {
                max_diff = 0;

                for (int i = 0; i < n_threads; ++i)
                {
                    max[i] = 0;
                    min[i] = INIT_VAL;
                    check[i] = false;
                }
                
                for (int i = 0; i < M; ++i)
                {
                    #pragma omp task
                    {
                        int thread_n = omp_get_thread_num();
                        for (int j = 0; j < N; ++j)
                        {

                            if ((i > 0) && (i < M - 1) && (j > 0) && (j < N - 1))
                                T_2[i][j] = T_1[i][j] + CURANT * (T_1[i][j + 1] + \
                                                                  T_1[i][j - 1] + \
                                                                  T_1[i + 1][j] + \
                                                                  T_1[i - 1][j] - \
                                                                  4 * T_1[i][j]);
                            if ((i == 0) && (j > 0) && (j < N - 1))
                                T_2[i][j] = T_1[i][j] + CURANT * (T_1[i][j + 1] + \
                                                                  T_1[i][j - 1] + \
                                                                  T_1[i + 1][j] - \
                                                                  3 * T_1[i][j]);
                            if ((i == M - 1) && (j > 0) && (j < N - 1))
                                T_2[i][j] = T_1[i][j] + CURANT * (T_1[i][j + 1] + \
                                                                  T_1[i][j - 1] + \
                                                                  T_1[i - 1][j] - \
                                                                  3 * T_1[i][j]);
                            if ((j == 0) && (i > 0) && (i < M - 1))
                                T_2[i][j] = T_1[i][j] + CURANT * (T_1[i][j + 1] + \
                                                                  T_1[i + 1][j] + \
                                                                  T_1[i - 1][j] - \
                                                                  3 * T_1[i][j]);
                            if ((j == N - 1) && (i > 0) && (i < M - 1))
                                T_2[i][j] = T_1[i][j] + CURANT * (T_1[i][j - 1] + \
                                                                  T_1[i + 1][j] + \
                                                                  T_1[i - 1][j] - \
                                                                  3 * T_1[i][j]);
                            if ((j == 0) && (i == 0))
                                T_2[i][j] = T_1[i][j] + CURANT * (T_1[i][j + 1] + \
                                                                  T_1[i + 1][j] - \
                                                                  2 * T_1[i][j]);
                            if ((j == 0) && (i == M - 1))
                                T_2[i][j] = T_1[i][j] + CURANT * (T_1[i][j + 1] + \
                                                                  T_1[i - 1][j] - \
                                                                  2 * T_1[i][j]);
                            if ((j == N - 1) && (i == 0))
                                T_2[i][j] = T_1[i][j] + CURANT * (T_1[i][j - 1] + \
                                                                  T_1[i + 1][j] - \
                                                                  2 * T_1[i][j]);
                            if ((j == N - 1) && (i == M - 1))
                                T_2[i][j] = T_1[i][j] + CURANT * (T_1[i][j - 1] + \
                                                                  T_1[i - 1][j] - \
                                                                  2 * T_1[i][j]);
                            if (T_2[i][j] > max[thread_n])
                                max[thread_n] = T_2[i][j];
                            if (T_2[i][j] < min[thread_n])
                                min[thread_n] = T_2[i][j];
                            check[thread_n] = true;
                        }
                    }
                }
                #pragma omp taskwait

                float** temp = T_1;
                T_1 = T_2;
                T_2 = temp;
            
                ++counter;
                
                for (int k = 0; k < n_threads; ++k)
                    if (((max[k] - min[k]) > max_diff) && check[k])
                        max_diff = max[k] - min[k];
                        
                printf("Iteration %d: Max diff = %f\n", counter, max_diff);
            }
        }       
    }

    cout << "Steady temperature: " << T_2[0][0] << " +- " << epsilon << endl;
    cout << "Time of work: " << omp_get_wtime() - t1 << endl;
    for (int i = 0; i < M; ++i) 
        delete[] T_1[i], T_2[i]; 
    delete[] T_1, T_2, max, min, check;
}