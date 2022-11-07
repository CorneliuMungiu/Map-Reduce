#include "utils.h"

/**
 * @brief Check alloc.
 * 
 * @param file 
 */
void check_alloc(void *file){
    if(!file){
        printf("Error: Out of memory\r\n");
        exit(-1);
    }
}

/**
 * @brief Raise the number to the power. If the result is greater than INT_MAX, it returns -1
 * 
 * @param number 
 * @param power 
 * @return number ^ power. f the result is greater than INT_MAX, it returns -1
 */
int power(int number, int power){
    long res = 1;
    for(int i = 0; i < power; i++){
        res *= number;
        if(res > INT_MAX)
            return -1;
    }
    return (int)res;
}

/**
 * @brief Binary Search.
 * 
 * @param vec 
 * @param to_find 
 * @param vec_size 
 * @return The index where the number was found
 */
int binarySearch(int *vec, int to_find, int vec_size){
    int low = 0;
    int high = vec_size - 1;
    while(high - low > 1){
        int mid = (high / 2) + (low / 2);
        if(vec[mid] < to_find){
            low = mid + 1;
        }else{
            high = mid;
        }
    }
    if(vec[low] == to_find){
        return low;
    }
    if(vec[high] == to_find){
        return high;
    }
    return -1;
}