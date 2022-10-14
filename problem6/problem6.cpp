//Parallel realisation of "resheto Eratosvena"

#include "omp.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv) 
{
    int N = atoi(argv[1]);
    int *a = new int[N + 1];
    int result = 0;
    for (int i = 2; i < N + 1; i++)
        a[i] = 1;
    a[0] = 0;
    a[1] = 0; // a = [0,0, 1, ... , 1]
    int n_threads = 0;
    double t1 = omp_get_wtime();
    #pragma omp parallel shared(a, N, n_threads)
    {   
        n_threads = omp_get_num_threads();
        #pragma omp single
        {
            for (int p = 2; p * p < N + 1; p++) //cannot be performed in parallel
            {
                if (a[p]) //if a[p]=0 it cannot be prime
                #pragma omp task //the only parallel directive callable inside single section
                {
                    if (a[p]) //check again if smth changed, 
                              //it increases performance in to times when N is big 
                        for (int j = p*p; j < N + 1; j += p)
                            a[j] = 0;
                }
            }
        #pragma omp taskwait
        } 
        //counting amount of prime numbers after all tasks being completed
        #pragma omp for schedule(static) reduction(+: result)
        for (int i = 0; i < N+1; i++)
            result += a[i];
    }
    
    cout << "Number of threads: " << n_threads << endl;
    cout << "Amount of prime numbers from 1 to " << N << ": " << result << endl;
    cout << "Time of work: " << omp_get_wtime() - t1 << endl;
}