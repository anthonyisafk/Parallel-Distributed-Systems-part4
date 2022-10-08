#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include "util/seq_helpers.h"

#define NUM 1600

int main (int argc, char** argv)
{

    const int num_threads = atoi(argv[1]);
    const int size = atoi(argv[2]);
    const float alpha = atof(argv[3]);


    __cilkrts_set_param("nworkers", argv[1]);
    __cilkrts_init();

    struct timeval start, end; 

    
  

    int partial_size = size/num_threads/num_threads;
    float eltime = 0;
    int counter = 0;

    float **x = (float**)malloc(num_threads * sizeof(float*));
    float **y = (float**)malloc(num_threads * sizeof(float*));

    for(int j = 0; j < num_threads; j++){
        x[j] = (float*)malloc(size/num_threads/num_threads*sizeof(float));
        y[j] = (float*)malloc(size/num_threads/num_threads*sizeof(float));
    }

   
    for(int times = 0; times < 10; times++){
        gettimeofday(&start, NULL);
        cilk_for (int i = 0; i < num_threads; i++){


            for(int k = 0; k < num_threads; k++){
                for (int j = 0; j < size/num_threads/num_threads; j++) {
                    float x_value = 5.6 * rand() / RAND_MAX;   
                    x[k][j] = (rand() > RAND_MAX / 2) ? x_value : -1.0 * x_value;
                    y[k][j] = rand() % 3;
                }
            }
            
            cilk_for (int j = 0; j < num_threads; j++){
            
                seq_axpy(x[j], alpha, size/num_threads/num_threads, y[j]);
            }
        

        

        }
        gettimeofday(&end, NULL);
        eltime += ((end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec)/1000.0;
        counter++;
    }

    free(x);
    free(y);

    
    printf("Time: %f\n", eltime/counter);

    
    return 0;
}