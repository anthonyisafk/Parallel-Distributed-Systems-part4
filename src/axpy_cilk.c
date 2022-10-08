/**
 * @file: axpy_cilk.c
 * @brief: Implement axpy: y = a*x + y, where `a` is a scalar.
 * This is the threaded implementation of the algorithm, based on openCilk.
 * @authors: Antonios Antoniou, Anestis Kaimakamidis
 * @emails: aantonii@ece.auth.gr - anestisk@ece.auth.gr
 */ 

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <sys/time.h>

#include "util/cilk_helpers.h"
#include "util/seq_helpers.h"

void write_csv(float elapsedTime, int threads){
    FILE *f;
    char name[10] = "data2.csv";
    
    f = fopen(name, "a");
    

    fprintf(f, "cilk, %d, %fus\n", threads, elapsedTime);
    
    return;
}


int main(int argc, char** argv) {
    if (argc < 4) {
        printf("\nUsage: ./axpy_cilk <threads> <size> <alpha>\n\n");
        exit(1);
    }

    const int num_threads = atoi(argv[1]);
    const int size = atoi(argv[2]);
    const float alpha = atof(argv[3]);

    printf("Initializing arrays...\n");
    float* x = (float*)malloc(size * sizeof(float));
    float* y = (float*)malloc(size * sizeof(float));
    for (int i = 0; i < size; i++) {
        float x_value = 5.6 * rand() / RAND_MAX;
        x[i] = (rand() > RAND_MAX / 2) ? x_value : -1.0 * x_value;
        y[i] = rand() % 3;
    }
    printf("Initialized arrays.\n");

    struct timeval stop, start;
    gettimeofday(&start, NULL);

    printf("\n >>> Performing axpy threads = %d, size = %d, alpha = %f ...\n", num_threads, size, alpha);
    __cilkrts_set_param("nworkers", argv[1]);
    __cilkrts_init();
    cilk_axpy(x, alpha, size, y);
    gettimeofday(&stop, NULL);
    printf(" >>> Done.\n\n");

    float timediff = ((stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);
    printf(
        "Axpy: threads = %d, size = %d, alpha = %f, ELAPSED TIME[us] = %f\n\n",
        num_threads, size, alpha, timediff
    );

    write_csv(timediff, num_threads);

    return 0;
}