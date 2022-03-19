#ifndef SEQ_HELPERS_H
#define SEQ_HELPERS_H

#include <stdlib.h>
#include <stdio.h>

#include "seq_helpers.h"

void seq_axpy(const float* x, const float alpha, const int size, float* y) {
    for (int i = 0; i < size; i++) {
        y[i] += alpha * x[i];
    }
}

float seq_sum(const float* x, const float alpha, const int size) {
    float result = 0;
    for (int i = 0; i < size; i++) {
        result += alpha * x[i];
    }

    return result;
}

void print_vector(const float* vec, const int size) {
    for (int i = 0; i < size; i++) {
        printf(" %f ", vec[i]);
    }
    printf("\n\n");
}

#endif