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

Vector* return_power_table(int nr_reducer){
    Vector *res = calloc(nr_reducer,sizeof(Vector));
    for(int i = 0; i < nr_reducer; i++){
        res[i].size = 0;
    }
    int tmp = 0;
    for(int i = 0; i < nr_reducer; i++){
        int number = 2;
        int tmp = 0;
        while(1){
            tmp = power(number,i + 2);
            if(tmp == -1)
                break;
            res[i].vec = realloc(res[i].vec,sizeof(int) * (++res[i].size));
            res[i].vec[res[i].size - 1] = tmp;
            number++;
        }
    }
    return res;
}

void power_table_destroy(int ***power_table, int nr_reducer){
    for(int i = 0; i < nr_reducer; i++){
        free((*power_table)[i]);
        // (*power_table)[i] = NULL;
    }
    free(*power_table);
    *power_table = NULL;
}


int main(int argc, char **argv){

    int *nr_mapper = (int*)calloc(1,sizeof(int));
    if(!nr_mapper){
        printf("Error: Out of memory\r\n");
        exit(-1);
    }
    *nr_mapper = atoi(argv[1]);

    int *nr_reducer = (int*)calloc(1,sizeof(int));
    if(!nr_reducer){
        printf("Error: Out of memory\r\n");
        exit(-1);
    }
    *nr_reducer = atoi(argv[2]);

    char* input_files_name = (char*)calloc(MAX_INPUT_FILE_NAME , sizeof(char));
    if(!input_files_name){
        printf("Error: Out of memory\r\n");
        exit(-1);
    }
    strcpy(input_files_name, argv[3]);
    FILE* input_file = fopen(input_files_name,"r");

    char *buffer = calloc(MAX_INPUT_FILE_NAME , sizeof(int));
    fscanf(input_file,"%s",buffer);
    free(buffer);
    
    if(!input_file){
        printf("Error: Can't open file\r\n");
        exit(-1);
    }

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
        threads_arg[i].mutex = &mutex;
        threads_arg[i].barrier = &barrier;
        threads_arg[i].threads_arg = threads_arg;
        threads_arg[i].input_file_name = calloc(MAX_INPUT_FILE_NAME , sizeof(char));
        if(!threads_arg[i].input_file_name){
            printf("Error: Out of memory\r\n");
            exit(-1);
        }
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

    fclose(input_file);
    pthread_mutex_destroy(&mutex);
    pthread_barrier_destroy(&barrier);
    pthread_exit(NULL);

    return 0;
}
