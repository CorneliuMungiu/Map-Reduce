#include "utils.h"

/**
 * @brief Function reads from the input files and adds the perfect powers in the corresponding lists.
 * 
 * @param threads_arg 
 * @return 
 */
void *mapper(void* threads_arg){
    
    Thread_arguments *thread_arg = (Thread_arguments *)threads_arg;
    Vector *vec_aux = calloc(*(thread_arg->nr_reducer) , sizeof(Vector));
    check_alloc(vec_aux);

    for(int i = 0; i < *(thread_arg->nr_reducer); i++){
        vec_aux[i].size = 0;
    }
    FILE *input_file = fopen(thread_arg->input_file_name, "r");
    if (!input_file)
        return NULL;

    char* buffer = calloc(MAX_INPUT_FILE_NAME , sizeof(char));
    check_alloc(buffer);
    fscanf(input_file,"%s",buffer);
    int nr_of_rows = atoi(buffer);
    int aux;
    for(int i = 0; i < nr_of_rows; i++){
        fscanf(input_file,"%s",buffer);
        aux = atoi(buffer);
        //Look for the number in the table of perfect powers
        get_index_from_power_table(thread_arg->power_table, *(thread_arg->nr_reducer), aux, vec_aux);
    }

    //Since a thread can read several files, the data from the following files will be added to the same list.
    if(thread_arg->power_arr == NULL){
        thread_arg->power_arr = vec_aux;
    }else{
        merge_Vector(thread_arg->power_arr,vec_aux,*thread_arg->nr_reducer);
        vector_destroy(vec_aux,*(thread_arg->nr_reducer));
    }
    free(buffer);
    fclose(input_file);
    return NULL;

}

/**
 * @brief The function counts the perfect powers (without duplicates) and writes them to the file
 * 
 * @param threads_arg 
 * @return 
 */
void *reducer(void *threads_arg){
    Thread_arguments *thread_arg = (Thread_arguments *)threads_arg;
    int power = thread_arg->id - *thread_arg->nr_mapper;
    Vector aux;
    for(int i = 0; i < *thread_arg->nr_mapper; i++){
        if(thread_arg->threads_arg[i].power_arr == NULL)
            continue;
        for(int j = 0; j < thread_arg->threads_arg[i].power_arr[power].size; j++){
            add_elem_in_list(&aux,thread_arg->threads_arg[i].power_arr[power].vec[j]);
        }
    }

    char *file_name = calloc(MAX_INPUT_FILE_NAME,sizeof(char));
    check_alloc(file_name);
    sprintf(file_name,"out%d.txt",power + 2);
    FILE *file = fopen(file_name,"w");
    fprintf(file,"%d",aux.size);
    free(file_name);
    fclose(file);
    return NULL;
}

/**
 * @brief Call the mapper then the reduction.
 * 
 * @param threads_arg 
 * @return 
 */
void *thread_distribution(void* threads_arg){
    Thread_arguments *thread_arg = (Thread_arguments *)threads_arg;
    if(thread_arg->type == MAPPER){
        char* buffer = calloc(MAX_INPUT_FILE_NAME , sizeof(char));
        check_alloc(buffer);

        while(!feof(thread_arg->input_file)){
            //A mutex is placed when reading from the file so that several 
            //threads do not read from the same file at the same time.
            pthread_mutex_lock(thread_arg->mutex);
            fscanf(thread_arg->input_file, "%s", buffer);
            pthread_mutex_unlock(thread_arg->mutex);
            
            strcpy(thread_arg->input_file_name,buffer);
            mapper(thread_arg);

            
            if(strcmp(buffer,"") == 0){
                continue;
            }
        }
        free(buffer);
    }
    //Waiting for complete reading from the file.
    pthread_barrier_wait(thread_arg->barrier);
    if(thread_arg->type == REDUCER){
        reducer(thread_arg);
    }
    return NULL;
}

/**
 * @brief Frees memory from thread_arguments.
 * 
 * @param thread_arguments 
 * @param size 
 */
void thread_arguments_destroy(Thread_arguments *thread_arguments, int size){
    for(int i = 0; i < size; i++){
        thread_arguments[i].input_file = NULL;
        vector_destroy(thread_arguments[i].power_arr,*(thread_arguments[i].nr_reducer));
        thread_arguments[i].power_arr = NULL;
        thread_arguments[i].power_table = NULL;
        thread_arguments[i].nr_mapper = NULL;
        thread_arguments[i].nr_reducer = NULL;
        free(thread_arguments[i].input_file_name);
        thread_arguments[i].mutex = NULL;
        thread_arguments[i].barrier = NULL;
        thread_arguments[i].threads_arg = NULL;
    }
    free(thread_arguments);
    thread_arguments = NULL;
}

    