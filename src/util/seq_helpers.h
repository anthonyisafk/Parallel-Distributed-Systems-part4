#ifndef SEQ_HELPERS_H
#define SEQ_HELPERS_H

void print_vector(const float* vec, const int size);
void seq_axpy(const float* x, const float alpha, const int size, float* y);
float seq_sum(const float* x, const float alpha, const int size);

#endif