#ifndef ATLAS_UTILS_H
#define ATLAS_UTILS_H

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#include "img_manip.h"
char *trim(char *s);
int str_concat(char * res, int length, ...);
char* print_2d_array_of_double(double **arr, int arr_size, int arr_elem_size);
#endif //ATLAS_UTILS_H
