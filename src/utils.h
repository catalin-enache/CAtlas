#ifndef ATLAS_UTILS_H
#define ATLAS_UTILS_H

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

bool array_insert(void** array, const void* value, size_t pos, size_t *length, size_t elem_size);
int max(int* arr, int size);
int count_char(const char *str, char c);
char** split_string(const char *str_orig, const char *delimiter, int *length);
char *trim(char *s);
int str_concat(char * res, int length, ...);
int replace_first_substring(char **str, const char* old_sub, const char* new_sub);
typedef int int_2x2x2[2][2][2];
typedef int ***int_2_2_2;
typedef size_t ***size_t_2_2_2;
char* print_3d_array_of_size_t_2_2_2(size_t_2_2_2 arr, size_t size1, size_t size2, size_t size3);
char* print_3d_array_of_int_2_2_2(int_2_2_2 arr, size_t size1, size_t size2, size_t size3);
char* print_3d_array_of_int_2x2x2(int_2x2x2 arr, size_t size1, size_t size2, size_t size3);
char* print_3D_array(void ***array, size_t type_size,  const char *type, int is_contiguous_block_of_memory, const char *format_specifier, int size1, int size2, int size3);
void arrayToString(char *output, void *array, int dimensions[], int num_dimensions, int is_contiguous_block_of_memory, const char *type, int type_size, const char *format, int buff_size);
void initialize_multi_dimensional_array(void **pointer, int *dimensions, int dimensions_length, size_t elem_size);
void free_multi_dimensional_array(void **pointer, int *dimensions, int dimensions_length, const char *debug);
#endif //ATLAS_UTILS_H
