#include "thread_arguments.h"

int power(int number, int power){
    long res = 1;
    for(int i = 0; i < power; i++){
        res *= number;
        if(res > INT_MAX)
            return -1;
    }
    return (int)res;
}

void return_power_table(int ***power_table, int nr_reducer){

    *power_table = (int**)malloc(nr_reducer * sizeof(int*));
    if(!(*power_table)){
        printf("Error: Out of memory\r\n");
        exit(-1);
    }

    for(int i = 0; i < nr_reducer; i++){
        int number = 2;
        int tmp = 0;
        int vec_size = 1;
        do{
            tmp = power(number, i + 2);
            (*power_table)[i] = realloc((*power_table)[i],sizeof(int) * (vec_size++));
            if(!(*power_table)[i]){
                printf("Error: Out of memory\r\n");
                exit(-1);
            }
            (*power_table)[i][number - 2] = tmp;
            number++;
        }while(tmp != -1);
        (*power_table)[i] = realloc((*power_table)[i],sizeof(int) * (vec_size));
        if(!(*power_table)[i]){
                printf("Error: Out of memory\r\n");
                exit(-1);
        }
        (*power_table)[i][number - 2] = -1;
    }
}

void power_table_destroy(int ***power_table, int nr_reducer){
    for(int i = 0; i < nr_reducer; i++){
        free((*power_table)[i]);
        (*power_table)[i] = NULL;
    }
    free(*power_table);
    *power_table = NULL;
}


int main(int argc, char **argv){

    int *nr_mapper = (int*)malloc(sizeof(int));
    if(!nr_mapper){
        printf("Error: Out of memory\r\n");
        exit(-1);
    }
    *nr_mapper = atoi(argv[1]);

    int *nr_reducer = (int*)malloc(sizeof(int));
    if(!nr_reducer){
        printf("Error: Out of memory\r\n");
        exit(-1);
    }
    *nr_reducer = atoi(argv[2]);

    char* input_files_name = (char*)malloc(MAX_INPUT_FILE_NAME * sizeof(char));
    if(!input_files_name){
        printf("Error: Out of memory\r\n");
        exit(-1);
    }
    strcpy(input_files_name, argv[3]);
    FILE* input_file = fopen(input_files_name,"r");
    if(!input_file){
        printf("Error: Can't open file\r\n");
        exit(-1);
    }

    int **power_table;
    return_power_table(&power_table, *nr_reducer);

    long cores = (*nr_mapper) + (*nr_reducer);
    pthread_t threads[cores];
    int r;
    void *status;

    pthread_mutex_t mutex;
    r = pthread_mutex_init(&mutex,NULL);
    if(r != 0){
        printf("Error: Can't init pthread mutex\r\n");
        exit(-1);
    }

    pthread_barrier_t barrier;
    r = pthread_barrier_init(&barrier,NULL,cores);
    if(r != 0){
        printf("Error: Can't init pthread barrier\r\n");
        exit(-1);
    }

    Thread_arguments* threads_arg = (Thread_arguments*)malloc(cores * sizeof(Thread_arguments));
    if(!threads_arg){
        printf("Error: Out of memory\r\n");
        exit(-1);
    }

    for(int i = 0; i < cores; i++){
        threads_arg[i].id = i;
        if(i < (*nr_mapper))
            threads_arg[i].type = 0;
        else
            threads_arg[i].type = 1;

        threads_arg[i].input_file = input_file;
        threads_arg[i].nr_mapper = nr_mapper;
        threads_arg[i].nr_reducer = nr_reducer;
        threads_arg[i].power_table = power_table;
        threads_arg[i].power_arr = (int**)malloc((*nr_reducer) * sizeof(int*));
        threads_arg[i].mutex = &mutex;
        r = pthread_create(&threads[i],NULL,f,&threads_arg[i]);
        if (r) {
		    printf("Eroare la crearea thread-ului %d\n", i);
		    exit(-1);
	    }
    }

    for(int i = 0; i < cores; i++){
        r = pthread_join(threads[i],&status);
        if (r) {
	  		printf("Eroare la asteptarea thread-ului %d\n", i);
	  		exit(-1);
		}
    }

    free(nr_mapper);
    free(nr_reducer);
    free(input_files_name);
    free(threads_arg);
    power_table_destroy(&power_table, *nr_reducer);

    fclose(input_file);
    pthread_mutex_destroy(&mutex);
    pthread_barrier_destroy(&barrier);
    pthread_exit(NULL);

    return 0;
}
