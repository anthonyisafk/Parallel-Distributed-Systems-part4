/**
 * @file: axpy_seq.c
 * @brief: Implement axpy: y = a*x + y, where `a` is a scalar.
 * This is the sequential implementation of the algorithm.
 * @authors: Antonios Antoniou, Anestis Kaimakamidis
 * @emails: aantonii@ece.auth.gr - anestisk@ece.auth.gr
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>

void print_vector(const float* vec, const int size) {
    for (int i = 0; i < size; i++) {
        printf(" %f ", vec[i]);
    }
    printf("\n\n");
}

void seq_axpy(const float* x, const float alpha, const int size, float* y) {
    for (int i = 0; i < size; i++) {
        y[i] += alpha * x[i];
    }
}

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("\nUsage: ./axpy_seq <size> <alpha>\n\n");
        exit(1);
    }

    const int size = atoi(argv[1]);
    const float alpha = atof(argv[2]);

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

    printf("\n >>> Performing axpy size = %d, alpha = %f ...\n", size, alpha);
    seq_axpy(x, alpha, size, y);
    gettimeofday(&stop, NULL);
    printf(" >>> Done.\n\n");

    uint timediff = (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
    printf("Axpy: size = %d, alpha = %f, ELAPSED TIME[us] = %d\n\n", size, alpha, timediff);
}