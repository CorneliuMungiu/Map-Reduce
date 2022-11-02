#include "thread_arguments.h"

void *f(void* threads_arg){
    Thread_arguments *thread_arg = (Thread_arguments *)threads_arg;

    if(thread_arg->type == MAPPER){
        while(!feof(thread_arg->input_file)){
            char* buffer = malloc(MAX_INPUT_FILE_NAME * sizeof(char));
            pthread_mutex_lock(thread_arg->mutex);
            fscanf(thread_arg->input_file, "%s", buffer);
            pthread_mutex_unlock(thread_arg->mutex);
            printf("thread: %d string: %s\n",thread_arg->id, buffer);
            free(buffer);
        }

    }
}