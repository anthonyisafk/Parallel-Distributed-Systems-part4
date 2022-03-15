/**
 * @file: sum_seq.c
 * @brief: Implement the sum of a vector a * X[N], where `a` is a scalar.
 * This is the sequential implementation of the algorithm.
 * @authors: Antonios Antoniou, Anestis Kaimakamidis
 * @emails: aantonii@ece.auth.gr - anestisk@ece.auth.gr
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void print_vector(const float* vec, const int size) {
    for (int i = 0; i < size; i++) {
        printf(" %f ", vec[i]);
    }
    printf("\n\n");
}

float seq_sum(const float* x, const float alpha, const int size) {
    float result = 0;
    for (int i = 0; i < size; i++) {
        result += alpha * x[i];
    }

    return result;
}

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("\nUsage: ./sum_seq <size> <alpha>\n\n");
        exit(1);
    }

    const int size = atoi(argv[1]);
    const float alpha = atof(argv[2]);

    printf("Initializing array...\n");
    float* x = (float*)malloc(size * sizeof(float));
    float* y = (float*)malloc(size * sizeof(float));
    for (int i = 0; i < size; i++) {
        float x_value = 5.6 * rand() / RAND_MAX;
        x[i] = (rand() > RAND_MAX / 2) ? x_value : -1.0 * x_value;
    }
    printf("Initialized array.\n");

    printf("\n >>> Performing sum, size = %d, alpha = %f ...\n", size, alpha);
    seq_sum(x, alpha, size);
    printf(" >>> Done.\n\n");
}