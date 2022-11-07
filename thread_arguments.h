#ifndef __THREAD_ARGUMENTS__
#define __THREAD_ARGUMENTS__

#include "utils.h"

#define MAX_INPUT_FILE_NAME 255
#define MAPPER 0
#define REDUCER 1

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

void *mapper(void* threads_arg);
void *reducer(void *threads_arg);
void *thread_distribution(void* threads_arg);
void thread_arguments_destroy(Thread_arguments *thread_arguments, int size);
#endif