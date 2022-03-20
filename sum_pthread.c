/**
 * @file: sum_pthread.c
 * @brief: Implement the sum of a vector a * X[N], where `a` is a scalar.
 * This is the threaded implementation of the algorithm, based on pthreads.
 * @authors: Antonios Antoniou, Anestis Kaimakamidis
 * @emails: aantonii@ece.auth.gr - anestisk@ece.auth.gr
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>
#include <pthread.h>

#include "util/seq_helpers.h"

// The object to be passed to an individual thread via `pthread_create`.
// `size` will be the size of the pointers the thread has to process,
// NOT the original one.
typedef struct {
    int size;
    float alpha;
    float* x;
    float result;
} sum_args;

void* sum_pthread(void* data) {
    sum_args* args = (sum_args*)data;
    args->result = seq_sum(args->x, args->alpha, args->size);
}

int main(int argc, char** argv) {
    if (argc < 4) {
        printf("\nUsage: ./sum_seq <threads> <size> <alpha>\n\n");
        exit(1);
    }

    const int num_threads = atoi(argv[1]);
    const int size = atoi(argv[2]);
    const float alpha = atof(argv[3]);

    printf("Initializing array...\n");
    float* x = (float*)malloc(size * sizeof(float));
    for (int i = 0; i < size; i++) {
        float x_value = 5.6 * rand() / RAND_MAX;
        x[i] = (rand() > RAND_MAX / 2) ? x_value : -1.0 * x_value;
    }
    printf("Initialized array.\n");

    // -------------------- TESTING --------------------
    // float actual_result = seq_sum(x, alpha, size);

    const int partial_size = size / num_threads;
    unsigned long total_bytes = 
        2 * num_threads * sizeof(float) + /* result and alpha */
        num_threads * sizeof(int) + /* sizes */
        size * sizeof(float); /* the entirety of x */
    sum_args* args = (sum_args*)malloc(total_bytes);

    for (int i = 0 ; i < num_threads; i++) {
        args[i].alpha = alpha;
        args[i].size = (i != num_threads - 1) ? partial_size : size - i * partial_size;
        args[i].x = (float*)malloc(args[i].size * sizeof(float));

        for (int j = 0; j < args[i].size; j++) {
            args[i].x[j] = x[i * partial_size + j];
        }
    }
    free(x);
    
    struct timeval stop, start;
    gettimeofday(&start, NULL);

    printf("\n >>> Performing sum, threads = %d, size = %d, alpha = %f ...\n", num_threads, size, alpha);
    pthread_t threads[num_threads];
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, sum_pthread, (void*) &args[i]);
    }
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    gettimeofday(&stop, NULL);
    printf(" >>> Done.\n\n");

    uint timediff = (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
    printf(
        "Sum: threads = %d, size = %d, alpha = %f, ELAPSED TIME[us] = %d\n\n",
        num_threads, size, alpha, timediff
    );

    float total_sum = 0;
    for (int i = 0; i < num_threads; i++) {
        total_sum += args[i].result;
    }
    // printf("Results: threaded = %f, sequential = %f\n", total_sum, actual_result);

    return 0;
}