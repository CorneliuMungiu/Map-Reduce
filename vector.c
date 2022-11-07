#include "utils.h"

/**
 * @brief Create a list of vectors that store all numbers raised to powers starting from 
 * 2 up to nr_reducer + 2 so that the number raised to that power is not greater than INT_MAX.
 * 
 * @param nr_reducer The number of power vector lists.
 * @return Power Table.
 */
Vector* return_power_table(int nr_reducer){
    Vector *res = calloc(nr_reducer,sizeof(Vector));
    check_alloc(res);

    for(int i = 0; i < nr_reducer; i++){
        res[i].size = 0;
    }
    int tmp = 0;
    for(int i = 0; i < nr_reducer; i++){
        int number = 2;
        tmp = 0;
        //Raise the number to power until it becomes greater than INT_MAX and add it to the list.
        while(1){
            tmp = power(number,i + 2);
            if(tmp == -1)
                break;
            res[i].vec = realloc(res[i].vec,sizeof(int) * (++res[i].size));
            check_alloc(res[i].vec);
            res[i].vec[res[i].size - 1] = tmp;
            number++;
        }
    }
    return res;
}

/**
 * @brief The function looks for the perfect power of a number and adds the number to the list of powers.
 * 
 * @param power_table 
 * @param nr_reducer 
 * @param number The number sought.
 * @param res The vector of perfect powers.
 */
void get_index_from_power_table(Vector* power_table, int nr_reducer, int number, Vector *res){
    if(number == 1){
        for(int i = 0; i < nr_reducer; i++){
            res[i].vec = realloc(res[i].vec,(++res[i].size) * sizeof(int));
            check_alloc(res[i].vec);
            res[i].vec[res[i].size - 1] = number;
        }
    }
    for(int i = 0; i < nr_reducer; i++){
        int aux = binarySearch(power_table[i].vec,number,power_table[i].size);
        if(aux != -1){
            res[i].vec = realloc(res[i].vec,(++res[i].size) * sizeof(int));
            check_alloc(res[i].vec);
            res[i].vec[res[i].size - 1] = number;
        }
    }
}

/**
 * @brief Merge vec1 with vec2 in vec1
 * 
 * @param vec1 
 * @param vec2 
 * @param nr_reducer 
 */
void merge_Vector(Vector *vec1, Vector *vec2, int nr_reducer){
    for(int i = 0; i < nr_reducer; i++){
        for(int j = 0; j < vec2[i].size; j++){
            vec1[i].vec = realloc(vec1[i].vec,(++vec1[i].size) * sizeof(int));
            check_alloc(vec1[i].vec);
            vec1[i].vec[vec1[i].size - 1] = vec2[i].vec[j];
        }
    }
}

/**
 * @brief Add an element to the list of a vector.
 * 
 * @param vec 
 * @param number 
 */
void add_elem_in_list(Vector *vec, int number){
    if(vec->vec == NULL){
        vec->vec = calloc(1,sizeof(int));
        check_alloc(vec->vec);
        vec->vec[0] = number;
        vec->size = 1;
        return;
    }
    for(int i = 0; i < vec->size; i++){
        if(number == vec->vec[i])
            return;
    }
    vec->vec = realloc(vec->vec,(++vec->size) * sizeof(int));
    check_alloc(vec->vec);
    vec->vec[vec->size - 1] = number;
}

/**
 * @brief Frees memory from a vector.
 * 
 * @param power_table 
 * @param size 
 */
void vector_destroy(Vector *power_table, int size){
    for(int i = 0; i < size; i++){
        if(power_table == NULL)
            break;
        if(power_table[i].vec != NULL)
            free(power_table[i].vec);
    }
    free(power_table);
    power_table = NULL;
    
}