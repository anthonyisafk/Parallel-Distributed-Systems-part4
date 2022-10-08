/**
 * @file: axpy_cilk2.c
 * @brief: Implement axpy: y = a*x + y, where `a` is a scalar.
 * This is the threaded implementation of the algorithm, based on openCilk.
 * @authors: Antonios Antoniou, Anestis Kaimakamidis
 * @emails: aantonii@ece.auth.gr - anestisk@ece.auth.gr
 */ 


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include "util/seq_helpers.h"

#define NUM 1600

void write_csv(float elapsedTime, int size){
    FILE *f;
    char name[10] = "data.csv";
    
    f = fopen(name, "a");
    

    fprintf(f, "cilk, %d, %f\n", size, elapsedTime);
    
    return ;
}

int main (int argc, char** argv)
{
    if (argc < 4) {
        printf("\nUsage: ./axpy_cilk2 <threads> <size> <alpha>\n\n");
        exit(1);
    }

    const int num_threads = atoi(argv[1]);
    const int size = atoi(argv[2]);
    const float alpha = atof(argv[3]);


    __cilkrts_set_param("nworkers", argv[1]);
    __cilkrts_init();

    struct timeval start, end; 

    
  

    int partial_size = size/num_threads;
    float eltime = 0;
    int counter = 0;

    float *x = (float*)malloc(size * sizeof(float));
    float *y = (float*)malloc(size * sizeof(float));


   
    for(int times = 0; times < 10; times++){
        gettimeofday(&start, NULL);
        cilk_for (int i = 0; i < num_threads; i++){

        
             for(int j = i * partial_size; j < (i+1)*partial_size; j++){
                float x_value = 5.6 * rand() / RAND_MAX;
                x[j] = (rand() > RAND_MAX / 2) ? x_value : -1.0 * x_value;
                y[j] = rand() % 3;
            }
            
            cilk_for (int j = 0; j < num_threads; j++){
                int partial_size2 = partial_size/num_threads;
                seq_axpy(x+ j*partial_size2 + i*partial_size, alpha, size/num_threads/num_threads,y + j*partial_size2 + i*partial_size);
            }
        

        

        }
        gettimeofday(&end, NULL);
        eltime += ((end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec)/1000.0;
        counter++;
    }

    free(x);
    free(y);

    
    printf("Time: %f\n", eltime/counter);

    write_csv(eltime/counter, size);

    
    return 0;
}