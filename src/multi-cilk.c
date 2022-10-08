/**
 * @file: multi-cilk.c
 * @brief: Implement axpy: y = a*x + y, where `a` is a scalar.
 * This is the threaded implementation of the algorithm, based on pthreads and openCilk combination.
 * @authors: Antonios Antoniou, Anestis Kaimakamidis
 * @emails: aantonii@ece.auth.gr - anestisk@ece.auth.gr
 */ 


#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include "util/seq_helpers.h"



typedef struct {
    int size;
    float alpha;
    float* x;
    float* y; 
} axpy_args;

void write_csv(float elapsedTime, int size){
    FILE *f;
    char name[10] = "data.csv";
    
    f = fopen(name, "a");
    

    fprintf(f, "multicilk, %d, %f\n", size, elapsedTime);
    
    return;
}


void* axpy_pthread(void* data) {
    axpy_args* args = (axpy_args*)data;
    seq_axpy(args->x, args->alpha, args->size, args->y);
}

int main (int argc, char** argv)
{
    if (argc < 4) {
        printf("\nUsage: ./multicilk <threads> <size> <alpha>\n\n");
        exit(1);
    }

    const int num_threads = atoi(argv[1]);
    const int size = atoi(argv[2]);
    const float alpha = atof(argv[3]);



    __cilkrts_set_param("nworkers", argv[1]);
    __cilkrts_init();

    struct timeval start, end; 


    

    const int partial_size = size / num_threads;
    unsigned long total_bytes = 
        num_threads * sizeof(float) + /* alpha */
        num_threads * sizeof(int) + /* sizes */
        2 * size * sizeof(float); /* the entirety of x and y */
  
    

    float eltime = 0;
    int counter = 0;


    float *x = (float*)malloc(size*sizeof(float));
    float *y = (float*)malloc(size*sizeof(float));
    pthread_t *t = (pthread_t*)malloc(num_threads*num_threads*sizeof(pthread_t));
    for(int times = 0; times < 10; times++){
      gettimeofday(&start, NULL);


      cilk_for (int i = 0; i < num_threads; i++){
        
         
          for(int j = i * partial_size; j < (i+1)*partial_size; j++){
            float x_value = 5.6 * rand() / RAND_MAX;
            x[j] = (rand() > RAND_MAX / 2) ? x_value : -1.0 * x_value;
            y[j] = rand() % 3;
          }
            
      



          axpy_args * args = (axpy_args*)malloc(num_threads*sizeof(axpy_args));
          for(int j = 0; j < num_threads; j++){
            args[j].size = partial_size/num_threads;
            args[j].alpha = alpha;
            args[j].x = x + j*args[j].size + i*partial_size;
            args[j].y = y + j*args[j].size + i*partial_size;
          }
         
          


          for(int j = 0; j < num_threads; j++){
            pthread_create(&t[i*num_threads+j], NULL, axpy_pthread, (void*) &args[j]);
          }

          for(int j = 0; j < num_threads; j++){
            pthread_join(t[i*num_threads+j], NULL);
          }

          free(args);
              
          
      }
      gettimeofday(&end, NULL);
      eltime += ((end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec)/1000;
      counter++;
    }

    printf("Time: %f\n", eltime/counter);
    free(t);
    free(x);
    free(y);


    write_csv(eltime/counter, size);

    return 0;
}

