#ifndef __THREAD_ARGUMENTS__
#define __THREAD_ARGUMENTS__

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_INPUT_FILE_NAME 255
#define MAPPER 0
#define REDUCER 1

typedef struct vector{
    int *vec;
    int size;
}Vector;

typedef struct thread_arguments{
    int id;
    int type;
    FILE *input_file;
    Vector* power_arr;
    Vector* power_table;
    int *nr_mapper;
    int *nr_reducer;
    char* input_file_name;
    pthread_mutex_t *mutex;
    pthread_barrier_t *barrier;
    struct thread_arguments* threads_arg;
}Thread_arguments;

void *f(void* threads_arg);
void get_index_from_power_table(Vector* power_table, int nr_reducer, int number, Vector *res);

#endif