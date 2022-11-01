#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX_INPUT_FILE_NAME 30

typedef struct thread_arguments{
    int id;
    int type;
    char *input_files_name;
}thread_arguments, *thread_arg;

int power(int number, int power){
    long long res = 1;
    // printf("%lld\n",res);
    // printf("%d %d\n", number,power);
    for(int i = 0; i < power; i++){
        res *= number;
        if(res > INT_MAX)
            return -1;
        // printf("%lld\n",res);
    }
    return res;
}

void return_power_table(int ***power_table, int nr_reducer){

    *power_table = (int**)malloc(nr_reducer * sizeof(int*));
    if(!(*power_table)){
        printf("Error: Out of memory\r\n");
        exit(-1);
    }

    for(int i = 0; i < nr_reducer; i++){
        long long number = 2;
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

    int **power_table;
    printf("A\n");
    return_power_table(&power_table, *nr_reducer);

    for(int i = 0; i < *nr_reducer; i++){
        int j = 0;
        while(power_table[i][j] != -1){
            printf("%d ", power_table[i][j]);
            j++;
        }
        printf("\nWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW\n");
    }
    
    

    printf("%d %d %s\n", *nr_mapper, *nr_reducer, input_files_name);

    return 0;
}
