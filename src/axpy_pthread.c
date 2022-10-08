/**
 * @file: axpy_pthread.c
 * @brief: Implement axpy: y = a*x + y, where `a` is a scalar.
 * This is the threaded implementation of the algorithm, based on pthreads.
 * @authors: Antonios Antoniou, Anestis Kaimakamidis
 * @emails: aantonii@ece.auth.gr - anestisk@ece.auth.gr
 */ 

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <sys/time.h>
#include <pthread.h>

#include "util/seq_helpers.h"

typedef struct {
    int size;
    float alpha;
    float* x;
    float* y; 
} axpy_args;

void* axpy_pthread(void* data) {
    axpy_args* args = (axpy_args*)data;
    seq_axpy(args->x, args->alpha, args->size, args->y);
}

void write_csv(float elapsedTime, int threads, int size){
    FILE *f;
    char name[10] = "data2.csv";
    
    f = fopen(name, "a");
    
    if(threads == 1 && size == 1000000) fprintf(f, "method, threads, time\n");
    fprintf(f, "pthread, %d, %d, %f\n", size, threads, elapsedTime);
    
    return;
}


int main(int argc, char** argv) {
    if (argc < 4) {
        printf("\nUsage: ./axpy_pthread <threads> <size> <alpha>\n\n");
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

    const int partial_size = size / num_threads;
    unsigned long total_bytes = 
        num_threads * sizeof(float) + /* alpha */
        num_threads * sizeof(int) + /* sizes */
        2 * size * sizeof(float); /* the entirety of x and y */
    axpy_args* args = (axpy_args*)malloc(total_bytes);

    for (int i = 0 ; i < num_threads; i++) {
        args[i].alpha = alpha;
        args[i].size = (i != num_threads - 1) ? partial_size : size - i * partial_size;
        args[i].x = x + i * partial_size;
        args[i].y = y + i * partial_size;
    }

    struct timeval stop, start;
    pthread_t threads[num_threads];
    gettimeofday(&start, NULL);

    printf("\n >>> Performing axpy threads = %d, size = %d, alpha = %f ...\n", num_threads, size, alpha);
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, axpy_pthread, (void*) &args[i]);
    }
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    gettimeofday(&stop, NULL);
    printf(" >>> Done.\n\n");

    float timediff = ((stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec)/1000.0;
    printf(
        "Axpy: threads = %d, size = %d, alpha = %f, ELAPSED TIME[us] = %f\n\n",
        num_threads, size, alpha, timediff
    );

    write_csv(timediff, num_threads, size);

    return 0;
}