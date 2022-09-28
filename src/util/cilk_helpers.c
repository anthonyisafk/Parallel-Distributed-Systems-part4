#ifndef CILK_HELPERS_H
#define CILK_HELPERS_H

#include <stdio.h>
#include <stdlib.h>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>

#include "cilk_helpers.h"

void cilk_axpy(const float* x, const float alpha, const int size, float* y) {
    cilk_for (int i = 0; i < size; i++) {
        y[i] += alpha * x[i];
    }
}

float cilk_sum(const float* x, const float alpha, const int size) {
    float result = 0;
    cilk_for (int i = 0; i < size; i++) {
        result += alpha * x[i];
    }

    return result;
}

#endif