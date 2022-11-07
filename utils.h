#ifndef __UTILS__
#define __UTILS__

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>
#include "vector.h"
#include "thread_arguments.h"

void check_alloc(void *file);
int power(int number, int power);
int binarySearch(int *vec, int to_find, int vec_size);
#endif