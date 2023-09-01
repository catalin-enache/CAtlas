#ifndef ATLAS_UTILS_H
#define ATLAS_UTILS_H

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

int max(int* arr, int size);
char *trim(char *s);
int str_concat(char * res, int length, ...);
typedef int int_2x2x2[2][2][2];
typedef int ***int_2_2_2;
typedef size_t ***size_t_2_2_2;
char* print_3d_array_of_size_t_2_2_2(size_t_2_2_2 arr, size_t size1, size_t size2, size_t size3);
char* print_3d_array_of_int_2_2_2(int_2_2_2 arr, size_t size1, size_t size2, size_t size3);
char* print_3d_array_of_int_2x2x2(int_2x2x2 arr, size_t size1, size_t size2, size_t size3);
char* print_3D_array(void ***array, size_t type_size,  const char *type, int is_contiguous_block_of_memory, const char *format_specifier, int size1, int size2, int size3);
void arrayToString(char *output, void *array, int dimensions[], int num_dimensions, int is_contiguous_block_of_memory, const char *type, int type_size, const char *format, int buff_size);
#endif //ATLAS_UTILS_H
