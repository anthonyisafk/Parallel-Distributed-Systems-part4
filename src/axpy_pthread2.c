/**
 * @file: axpy_pthread2.c
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

typedef struct {
    int size;
    float* x;
    float* y; 
    int id;
    int num_threads;
    float alpha;
} thread_args;


void write_csv(float elapsedTime, int size){
    FILE *f;
    char name[10] = "data.csv";
    
    f = fopen(name, "a");
    
    //headers
    if(size == 500000) fprintf(f, "method, size, time\n");
    fprintf(f, "pthread, %d, %f\n", size, elapsedTime);
    
    return ;
}



void* axpy_pthread(void* data) {
    axpy_args* args = (axpy_args*)data;
    seq_axpy(args->x, args->alpha, args->size, args->y);
}

void * produce(void * data){
    thread_args* args = (thread_args*)data;
    pthread_t threads[args->num_threads];
    int i = args->id;
    int partial_size = args->size;

    for(int j = i * partial_size; j < (i+1)*partial_size; j++){
        float x_value = 5.6 * rand() / RAND_MAX;
        args->x[j] = (rand() > RAND_MAX / 2) ? x_value : -1.0 * x_value;
        args->y[j] = rand() % 3;
    }

    axpy_args *args1 = (axpy_args*)malloc(args->num_threads*sizeof(axpy_args));
    int partial_size2 = partial_size/args->num_threads;
    for (int j = 0 ; j < args->num_threads; j++) {
        args1[j].alpha = args->alpha;
        args1[j].size = partial_size2;
        args1[j].x = args->x + i * partial_size + j*partial_size2;
        args1[j].y = args->y + i * partial_size + j*partial_size2;
    }

    for (int j = 0 ; j < args->num_threads; j++) {
        pthread_create(&threads[j], NULL, axpy_pthread, (void *) &args1[j]);
    }

    for (int j = 0 ; j < args->num_threads; j++) {
        pthread_join(threads[j], NULL);
    }

    return NULL;


}

int main(int argc, char** argv) {
    if (argc < 4) {
        printf("\nUsage: ./axpy_pthread2 <threads> <size> <alpha>\n\n");
        exit(1);
    }

    const int num_threads = atoi(argv[1]);
    const int size = atoi(argv[2]);
    const float alpha = atof(argv[3]);

    struct timeval start, end; 

    printf("Initializing arrays...\n");
    float* x = (float*)malloc(size * sizeof(float));
    float* y = (float*)malloc(size * sizeof(float));
    
    printf("Initialized arrays.\n");

    const int partial_size = size / num_threads;
    
    pthread_t threads[num_threads];
    float eltime = 0;
    int counter = 0;

    for(int times = 0; times < 10; times++){
        gettimeofday(&start, NULL);
        thread_args *data = (thread_args*)malloc(num_threads*sizeof(thread_args));
        for (int i = 0; i < num_threads; i++){
            data[i].x = x;
            data[i].y = y;
            data[i].id = i;
            data[i].alpha = alpha;
            data[i].size = partial_size;
            data[i].num_threads = num_threads;
        }

        
        for(int i = 0; i < num_threads; i++){
            pthread_create(&threads[i], NULL, produce, (void *) &data[i]);
        }

        for(int i = 0; i < num_threads; i++){
            pthread_join(threads[i], NULL);
        }
        gettimeofday(&end, NULL);
        eltime += ((end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec)/1000.0;
        counter++;
    }

    printf("Time: %f\n", eltime/counter);

    write_csv(eltime/counter, size);

    return 0;
}