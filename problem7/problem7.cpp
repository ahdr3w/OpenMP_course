#include "omp.h"
#include <iostream>

using namespace std;

#define INIT_VAL 100
#define CURANT 0.25 //we would prefer maximum possible value, 
                    //but if value increased, scheme would be unstable

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
        
    int counter = 0; //number of iteration 

    float** T_1 = initLayer(N, M);
    float** T_2 = initLayer(N, M); //time layers

    int n_threads = omp_get_max_threads();

    //creating arrays where we're gonna put max and min values of temperature of each thread.
    //Each thread will work with its own cells, it will enter max and min values from its subset
    //into the arrays according to thread number
    float* max  = new float[n_threads];
    float* min  = new float[n_threads];

    bool* check = new bool[n_threads]; //we're gonna check if particular thread was doing some tasks or not
                                                     
    float max_diff = epsilon + 1; //difference between the hotest and the coldest cells on a particular layer

    double t1 = omp_get_wtime();

    //init initial conditions
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

                //init temporary values
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
                            //each if describes particular position of a cell (central, coner and so on) 
                            
                            //cetral cells boder with 4 other cels
                            if ((i > 0) && (i < M - 1) && (j > 0) && (j < N - 1))
                                T_2[i][j] = T_1[i][j] + CURANT * (T_1[i][j + 1] + \
                                                                  T_1[i][j - 1] + \
                                                                  T_1[i + 1][j] + \
                                                                  T_1[i - 1][j] - \
                                                                  4 * T_1[i][j]);
                            //side cells boder with 3 other cells
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
                            
                            //coner cells boder only with to other cells
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
                            //each thread calculats values in many cells in variuos places
                            //also he remembers max and min values
                            if (T_2[i][j] > max[thread_n])
                                max[thread_n] = T_2[i][j];
                            if (T_2[i][j] < min[thread_n])
                                min[thread_n] = T_2[i][j];
                            check[thread_n] = true; //proof that thread was doing smth.
                                                    //without this array thread which was not doing any tasks
                                                    //would ruin calculations later with its fictional vulues in max and min arrays
                        }
                    }
                }
                #pragma omp taskwait

                float** temp = T_1; //swaping layers 
                T_1 = T_2;          
                T_2 = temp;
            
                ++counter;
                //now main thread checks work of ohter threads and finds max and min values from all set
                for (int k = 0; k < n_threads; ++k)
                    if (((max[k] - min[k]) > max_diff) && check[k]) // if check[0]=0 then we will not
                                                                    // take into accaunt its values
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