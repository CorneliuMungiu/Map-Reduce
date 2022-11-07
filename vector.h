#ifndef __VECTOR__
#define __VECTOR__
#include "utils.h"

typedef struct vector{
    int *vec;
    int size;
}Vector;

Vector* return_power_table(int nr_reducer);
void vector_destroy(Vector *power_table, int size);
void get_index_from_power_table(Vector* power_table, int nr_reducer, int number, Vector *res);
void merge_Vector(Vector *vec1, Vector *vec2, int nr_reducer);
void add_elem_in_list(Vector *vec, int number);
#endif

