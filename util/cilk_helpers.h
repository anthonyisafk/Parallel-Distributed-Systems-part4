#ifndef CILK_HELPERS_H
#define CILK_HELPERS_H

void cilk_axpy(const float* x, const float alpha, const int size, float* y);
float cilk_sum(const float* x, const float alpha, const int size);

#endif