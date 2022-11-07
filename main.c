#include "utils.h"

int main(int argc, char **argv){

    int *nr_mapper = (int*)calloc(1,sizeof(int));
    check_alloc(nr_mapper);
    *nr_mapper = atoi(argv[1]);

    int *nr_reducer = (int*)calloc(1,sizeof(int));
    check_alloc(nr_reducer);
    *nr_reducer = atoi(argv[2]);

    char* input_files_name = (char*)calloc(MAX_INPUT_FILE_NAME , sizeof(char));
    check_alloc(input_files_name);
    strcpy(input_files_name, argv[3]);
    FILE* input_file = fopen(input_files_name,"r");

    char *buffer = calloc(MAX_INPUT_FILE_NAME , sizeof(int));
    check_alloc(buffer);
    fscanf(input_file,"%s",buffer);
    free(buffer);
    check_alloc(input_file);

    Vector *power_table = return_power_table(*nr_reducer);

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

    Thread_arguments* threads_arg = (Thread_arguments*)calloc(cores , sizeof(Thread_arguments));
    check_alloc(threads_arg);

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
        threads_arg[i].mutex = &mutex;
        threads_arg[i].barrier = &barrier;
        threads_arg[i].threads_arg = threads_arg;
        threads_arg[i].input_file_name = calloc(MAX_INPUT_FILE_NAME , sizeof(char));
        check_alloc(threads_arg[i].input_file_name);
        r = pthread_create(&threads[i],NULL,thread_distribution,&threads_arg[i]);
        if (r) {
		    printf("Error creating thread %d\n", i);
		    exit(-1);
	    }
    }

    for(int i = 0; i < cores; i++){
        r = pthread_join(threads[i],&status);
        if (r) {
	  		printf("Error waiting for thread %d\n", i);
	  		exit(-1);
		}
    }

    vector_destroy(power_table,*nr_reducer);
    thread_arguments_destroy(threads_arg,cores);

    free(nr_mapper);
    free(nr_reducer);
    free(input_files_name);

    fclose(input_file);
    pthread_mutex_destroy(&mutex);
    pthread_barrier_destroy(&barrier);
    pthread_exit(NULL);

    return 0;
}
