#include "thread_arguments.h"

void get_index_from_power_table(int **power_table, int nr_reducer, int number, Vector *res){
    if(number == 1){
        for(int i = 0; i < nr_reducer; i++){
            res[i].vec = realloc(res[i].vec,(++res[i].size) * sizeof(int));
            if(!res[i].vec){
                printf("Error: Out of memory\r\n");
                exit(-1);
            }
            res[i].vec[res[i].size - 1] = number;
        }
    }
    for(int i = 0; i < nr_reducer; i++){
        int j = 0;
        while(j != -1 && power_table[i][j] <= number){
            if(power_table[i][j] == number){
                res[i].vec = realloc(res[i].vec,(++res[i].size) * sizeof(int));
                if(!res[i].vec){
                    printf("Error: Out of memory\r\n");
                    exit(-1);
                }
                res[i].vec[res[i].size - 1] = number;
                break;
            }
            j++;
        }
    }
}

void merge_Vector(Vector *vec1, Vector *vec2, int nr_reducer){
    for(int i = 0; i < nr_reducer; i++){
        for(int j = 0; j < vec2[i].size; j++){
            vec1[i].vec = realloc(vec1[i].vec,(++vec1[i].size) * sizeof(int));
            vec1[i].vec[vec1[i].size - 1] = vec2[i].vec[j];
        }
    }
}

void *mapper(void* threads_arg){
    
    Thread_arguments *thread_arg = (Thread_arguments *)threads_arg;
    Vector *vec_aux = calloc(*(thread_arg->nr_reducer) , sizeof(Vector));
    if(!vec_aux){
        printf("Error: Out of memory\r\n");
        exit(-1);
    }
    for(int i = 0; i < *(thread_arg->nr_reducer); i++){
        vec_aux[i].size = 0;
    }
    FILE *input_file = fopen(thread_arg->input_file_name, "r");
    if (!input_file)
        return NULL;
    char* buffer = calloc(MAX_INPUT_FILE_NAME , sizeof(char));
    if(!buffer){
        printf("Error: Out of memory\r\n");
        exit(-1);
    }
    fscanf(input_file,"%s",buffer);
    int nr_of_rows = atoi(buffer);
    int aux;
    for(int i = 0; i < nr_of_rows; i++){
        fscanf(input_file,"%s",buffer);
        aux = atoi(buffer);
        printf("thread %d file %s\n", thread_arg->id, thread_arg->input_file_name);
        get_index_from_power_table(thread_arg->power_table, *(thread_arg->nr_reducer), aux, vec_aux);
    }

    if(thread_arg->power_arr == NULL){
        thread_arg->power_arr = vec_aux;

        // printf("%s\n",thread_arg->input_file_name);
        // for(int j = 0; j < *thread_arg->nr_reducer; j++){
        //     for(int k = 0; k < thread_arg->power_arr[j].size; k++){
        //         printf("%d ",thread_arg->power_arr[j].vec[k]);
        //     }
        //     printf("\n");
        // }
    }else{
        merge_Vector(thread_arg->power_arr,vec_aux,*thread_arg->nr_reducer);
        // for(int j = 0; j < *thread_arg->nr_reducer; j++){
        //     for(int k = 0; k < thread_arg->power_arr[j].size; k++){
        //         printf("%d ",thread_arg->power_arr[j].vec[k]);
        //     }
        //     printf("\n");
        // }
    }
    //printf("A citit fisierul %s\n",thread_arg->input_file_name);
    free(buffer);
    fclose(input_file);
    return NULL;

}


void *f(void* threads_arg){
    Thread_arguments *thread_arg = (Thread_arguments *)threads_arg;
    if(thread_arg->type == MAPPER){
        char* buffer = calloc(MAX_INPUT_FILE_NAME , sizeof(char));
        if (!buffer){
            printf("Error: Out of memory\r\n");
            exit(-1);
        }
        while(!feof(thread_arg->input_file)){
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
    pthread_barrier_wait(thread_arg->barrier);
    if(thread_arg->type == REDUCER){
        
    }
    return NULL;
}

    