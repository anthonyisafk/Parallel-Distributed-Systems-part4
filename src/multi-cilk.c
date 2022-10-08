#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include "util/seq_helpers.h"


#define QUEUESIZE 1000



void *producer (void *args);
void *consumer (void *args);



typedef struct{
  void (*work)(const float* x, const float alpha, const int size, float* y);
  float *x;
  float *y;
  float alpha;
  int size;
} workFunction;

typedef struct {
    int size;
    float alpha;
    float* x;
    float* y; 
} axpy_args;

typedef struct {
  workFunction buf[QUEUESIZE];
  int tid;
  int counter;
  long head, tail;
  int full, empty;
  pthread_mutex_t *mut;
  pthread_cond_t *notFull, *notEmpty;
} queue;

typedef struct{
    queue *q;
    int size;
    float alpha;
    float *x;
    float *y;
} t_work;

queue *queueInit (void);
void queueDelete (queue *q);
void queueAdd (queue *q, workFunction in);
void queueDel (queue *q, workFunction *out);
void *print_message(void *arg);
void write_csv(double *elapsedTime, int id);


void* axpy_pthread(void* data) {
    axpy_args* args = (axpy_args*)data;
    seq_axpy(args->x, args->alpha, args->size, args->y);
}

int main (int argc, char** argv)
{
    queue *fifo;
    
        
    fifo = queueInit ();
        
    if (fifo ==  NULL) {
        fprintf (stderr, "main: Queue Init failed.\n");
        exit (1);
    }


    
    const int num_threads = atoi(argv[1]);
    const int size = atoi(argv[2]);
    const float alpha = atof(argv[3]);

    pthread_t *con;
    con = (pthread_t*)malloc(num_threads*num_threads/2*sizeof(pthread_t));
    //t = (pthread_t*)malloc(num_threads*num_threads*sizeof(pthread_t));

    __cilkrts_set_param("nworkers", argv[1]);
    __cilkrts_init();

    struct timeval start, end; 


    printf("Initializing arrays...\n");
    
  
    printf("Initialized arrays.\n");


    

    const int partial_size = size / num_threads;
    unsigned long total_bytes = 
        num_threads * sizeof(float) + /* alpha */
        num_threads * sizeof(int) + /* sizes */
        2 * size * sizeof(float); /* the entirety of x and y */
    
    t_work * args = (t_work*)malloc(sizeof(t_work));
    args->size = size/num_threads/2;
    args->alpha = alpha;
    args->q = fifo;
    

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

    free(args);
    printf("Time: %f\n", eltime/counter);
    free(con);
    free(t);
    free(x);
    free(y);

    queueDelete (fifo);


    return 0;
}

void *producer (void *a)
{
    queue *fifo;


    int size = ((t_work *)a)->size;
    float alpha = ((t_work *)a)->alpha;

    float* x = (float*)malloc(size * sizeof(float));
    float* y = (float*)malloc(size * sizeof(float));
    fifo = ((t_work *)a)->q;

    
    for (int j = 0; j < size; j++) {
      float x_value = 5.6 * rand() / RAND_MAX;
      x[j] = (rand() > RAND_MAX / 2) ? x_value : -1.0 * x_value;
      y[j] = rand() % 3;
    }
   
  

    

    pthread_mutex_lock (fifo->mut);
    while (fifo->full) {
        printf ("producer: queue FULL.\n");
        pthread_cond_wait (fifo->notFull, fifo->mut);
    }

    workFunction b;
    b.size = size;
    b.alpha = alpha;
    b.x = x;
    b.y = y;
    b.work = &seq_axpy;
    
    queueAdd (fifo, b);
    pthread_mutex_unlock (fifo->mut);
    pthread_cond_signal (fifo->notEmpty);

    return (NULL);
}

void *consumer (void *q)
{
  queue *fifo;
  workFunction d;
  int i;
  

  fifo = (queue*)q;


  
  pthread_mutex_lock (fifo->mut);
  while (fifo->empty) {
      
      pthread_cond_wait (fifo->notEmpty, fifo->mut);
    }
  
  queueDel (fifo, &d);

  fifo->counter += 1;
    
  d.work(d.x, d.alpha, d.size, d.y);  
  pthread_mutex_unlock (fifo->mut);
  pthread_cond_signal (fifo->notFull);

   free(d.x);
   free(d.y);
  
  return (NULL);
}

/*
  typedef struct {
  int buf[QUEUESIZE];
  long head, tail;
  int full, empty;
  pthread_mutex_t *mut;
  pthread_cond_t *notFull, *notEmpty;
  } queue;
*/

queue *queueInit (void)
{
  queue *q;

  q = (queue *)malloc (sizeof (queue));
  if (q == NULL) return (NULL);

  q->empty = 1;
  q->full = 0;
  q->head = 0;
  q->tail = 0;
  q->counter = 0;
  q->mut = (pthread_mutex_t *) malloc (sizeof (pthread_mutex_t));
  pthread_mutex_init (q->mut, NULL);
  q->notFull = (pthread_cond_t *) malloc (sizeof (pthread_cond_t));
  pthread_cond_init (q->notFull, NULL);
  q->notEmpty = (pthread_cond_t *) malloc (sizeof (pthread_cond_t));
  pthread_cond_init (q->notEmpty, NULL);
    
  return (q);
}

void queueDelete (queue *q)
{
  pthread_mutex_destroy (q->mut);
  free (q->mut);
  pthread_cond_destroy (q->notFull);
  free (q->notFull);
  pthread_cond_destroy (q->notEmpty);
  free (q->notEmpty);
  free (q);
}

void queueAdd (queue *q, workFunction in)
{
  q->buf[q->tail] = in;
  q->tail++;
  if (q->tail == QUEUESIZE)
    q->tail = 0;
  if (q->tail == q->head)
    q->full = 1;
  q->empty = 0;

  return;
}

void queueDel (queue *q, workFunction *out)
{
  *out = q->buf[q->head];

  q->head++;
  if (q->head == QUEUESIZE)
    q->head = 0;
  if (q->head == q->tail)
    q->empty = 1;
  q->full = 0;

  return;
}

void *print_message(void *arg){
    int* message = (int*) arg;
    printf("Hello from item %d\n", *message);
}

