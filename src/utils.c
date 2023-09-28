#include <stddef.h>
#include "utils.h"

bool array_insert(void** array, const void* value, size_t pos, size_t *length, size_t elem_size) {
    // printf("array_insert length: %d, elem_size: %zu, value: %s\n", *length, elem_size, *(char **)value);
    if (pos > *length) return false;  // Invalid position

    void *new_array = realloc(*array, elem_size * (*length + 1));
    if (!new_array) return false;  // Memory allocation failed

    void *insert_position = (char *)new_array + pos * elem_size;
    void *next_position = (char *)new_array + (pos + 1) * elem_size;
    // Shift memory from the position to the end
    memmove(next_position, insert_position, elem_size * (*length - pos));
    // Copy new value to the given position
    memcpy(insert_position, value, elem_size);

    (*length)++;
    *array = new_array;
    return true;
}

int max(int* arr, int size) {
    int max = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] > max) max = arr[i];
    }
    return max;
}

int count_char(const char *str, char c) {
    int count = 0;
    while (*str) {
        if (*str == c) {
            count++;
        }
        str++;
    }
    return count;
}

char** split_string(const char *str_orig, const char *delimiter, int *length) {
    char *str = strdup(str_orig);

    char *token = strtok(str, delimiter);
    char **tokens = NULL;
    int i = 0;
    while (token != NULL) {
        tokens = (char **)realloc(tokens, sizeof(char *) * (i + 1));
        if (tokens == NULL) { printf("Could not allocate memory for splitting string.\n"); return NULL; }
        tokens[i] = strdup(token);
        token = strtok(NULL, delimiter);
        i++;
    }
    tokens = (char **)realloc(tokens, sizeof(char *) * (i + 1));
    tokens[i] = NULL;
    if (length != NULL) { *length = i; }

    free(str);

    return tokens;
}

// https://stackoverflow.com/questions/656542/trim-a-string-in-c
char *trim(char *s) {
    char *ptr;
    if (!s)
        return NULL;   // handle NULL string
    if (!*s)
        return s;      // handle empty string
    for (ptr = s + strlen(s) - 1; (ptr >= s) && isspace(*ptr); --ptr);
    ptr[1] = '\0';
    return s;
}

int str_concat(char * res, int length, ...) {
    // variadic args flow >>
    va_list args;
    va_start(args, length);
    res[0] = '\0';
    char * next = NULL;
    int total_length = 0;
    while ((next = va_arg(args, char*)) != NULL) {
        int next_length = strlen(next);
        if (total_length + next_length + 1 > length) {
            printf("String concatenation '%s' would exceed the maximum length of %d.\n", next, length);
            return -1;
        }
        strncat(res, next, length - total_length - 1);
        total_length += next_length;
    }
    va_end(args);
    // << variadic args flow
    return total_length;
}

// From ChatGPT
int replace_first_substring(char **str, const char* old_sub, const char* new_sub) {
    char* occurrence = strstr(*str, old_sub);
    if (!occurrence) {
        // Old substring not found.
        return 0;  // No replacement occurred
    }

    int len_str = strlen(*str);
    int len_old = strlen(old_sub);
    int len_new = strlen(new_sub);
    int new_length = len_str - len_old + len_new;

    char* new_str = (char*)malloc(new_length + 1);  // +1 for the null terminator
    if (!new_str) {
        // Memory allocation failed.
        return -1;  // Indicates a memory error
    }

    // Copy the part of the original string before the old_sub into the new string.
    strncpy(new_str, *str, occurrence - *str);
    new_str[occurrence - *str] = '\0';  // Add a null terminator to the string.

    // Append the replacement substring.
    strcat(new_str, new_sub);

    // Append the rest of the original string after the old_sub.
    strcat(new_str, occurrence + len_old);

    free(*str);  // Free the original string's memory.

    *str = new_str;  // Point the original string pointer to the new memory.

    return 1;  // Replacement was successful
}


#define DEFINE_PRINT_3D_ARRAY_FUNC(type, format_specifier) \
char* print_3d_array_of_##type(type arr, size_t size1, size_t size2, size_t size3) { \
    int d_size = 50; /* arbitrary size of formatted double including surrounding chars (commas, parenthesis) */ \
    char *res = (char*)malloc(size1 * size2 * size3 * d_size * sizeof(char)); \
    if (res == NULL) { \
        printf("Could not allocate memory for printing 3d array of ints.\n"); \
        return NULL; \
    } \
    res[0] = '\0'; \
    strcpy(res, "["); \
    for(int i = 0; i < size1; i++) { \
        strcat(res, "["); \
        for (int j = 0; j < size2; j++) { \
            strcat(res, "["); \
            for (int k = 0; k < size3; k++) { \
                char buffer[30]; \
                sprintf(buffer, format_specifier, arr[i][j][k]); \
                strcat(res, buffer); \
                if (k < size3 - 1) { \
                    strcat(res, ", "); \
                } \
            } \
            strcat(res, "]"); \
            if (j < size2 - 1) { \
                strcat(res, ", "); \
            } \
        } \
        strcat(res, "]"); \
        if (i < size1 - 1) { \
            strcat(res, ", "); \
        } \
    } \
    strcat(res, "]"); \
    res[strlen(res)] = '\0'; \
    return res; \
}

// Generate functions for size_t and int
DEFINE_PRINT_3D_ARRAY_FUNC(size_t_2_2_2, "%zu")
DEFINE_PRINT_3D_ARRAY_FUNC(int_2_2_2, "%d")
DEFINE_PRINT_3D_ARRAY_FUNC(int_2x2x2, "%d")

/*
print_3d_array_of_int_2_2_2(heapInt3DArr, 2, 2, 2);
print_3d_array_of_int_2x2x2(stackInt3dArr, 2, 2, 2);
*/

// print_3D_array((void ***)arrSizeTStack, sizeof(size_t), "size_t", 1, "%zu", rows, cols, data_length); // for 3d array of size_t allocated on stack
// print_3D_array((void ***)arrSizeTHeap, sizeof(size_t), "size_t", 0, "%zu", rows, cols, data_length); // for 3d array of size_t allocated on heap
// print_3D_array((void ***)arrStack, 64, "string", 1, "%s", rows, cols, data_length); // for 3d array of strings allocated on stack (64 is the str length)
// print_3D_array((void ***)arrHeap, sizeof(char*), "string", 0, "%s", rows, cols, data_length); // for 3d array of strings allocated on heap
char* print_3D_array(void ***array, size_t type_size, const char *type, int is_contiguous_block_of_memory, const char *format_specifier, int size1, int size2, int size3) {
    size_t d_size = type_size + 30; // arbitrary size of formatted double including surrounding chars (commas, parenthesis)
    char *res = (char*)malloc(size1 * size2 * size3 * d_size * sizeof(char));
    if (res == NULL) { printf("Could not allocate memory for printing 3d array.\n"); return NULL; }

    res[0] = '\0';
    strcpy(res, "[");
    for(int i = 0; i < size1; i++) {
        strcat(res, "[");
        for(int j = 0; j < size2; j++) {
            strcat(res, "[");
            for(int k = 0; k < size3; k++) {
                char buffer[30];
                // Offset calculation to navigate through memory
                void *element = NULL;
                // [[[1,2],[3,4]],[[5,6],[7,8]]]
                if (is_contiguous_block_of_memory) {
                    // Stack memory case
                    // Pointer Arithmetic Version:
                    element = (char *)array + i * size2 * size3 * type_size + j * size3 * type_size + k * type_size; // (alternate)
                    // Type Casting Version:
                    // the array is cast to a 3d array pointer
                    // element = &((char (*)[size2][size3][type_size]) array)[i][j][k];
                    // element = &((char (*)[size2][size3 * type_size]) array)[i][j][k * type_size];
                    // element = ((char (*)[size2][size3][type_size]) array)[i][j] + k; // when i changes it is offsetting inside the 3d array pointer with the size of [size2][size3][type_size]. That's why the first dimension (size1) is not needed.
                    // element = ((char (*)[size2][size3 * type_size]) array)[i][j] + k * type_size;
                    // element = (char*)(((char (*)[size2][size3 * type_size]) array)[i]) + j * size3 * type_size + k * type_size;
                    // The following works in C but not in C++
                    // char (*arrPtr)[size2][size3][type_size] = (char (*)[size2][size3][type_size]) array;
                    // element = &arrPtr[i][j][k];
                } else {
                    // Heap memory case
                    // array[i] has precedence, we cast (char **) on array[i]
                    // element = ((char **)array[i])[j] + k * type_size;
                    element = &((char **)array[i])[j][k * type_size];
                }

                if (strcmp(type, "int") == 0) {
                    sprintf(buffer, format_specifier, *(int *)element);
                } else if (strcmp(type, "long") == 0) {
                    sprintf(buffer, format_specifier, *(long *)element);
                } else if (strcmp(type, "size_t") == 0) {
                    sprintf(buffer, format_specifier, *(size_t *)element);
                } else if (strcmp(type, "float") == 0) {
                    sprintf(buffer, format_specifier, *(float *)element);
                } else if (strcmp(type, "double") == 0) {
                    sprintf(buffer, format_specifier, *(double *)element);
                } else if (strcmp(type, "char") == 0) {
                    sprintf(buffer, format_specifier, *(char *)element);
                } else if (strcmp(type, "string") == 0) {
                    if (is_contiguous_block_of_memory) {
                        sprintf(buffer, format_specifier, (char *)element); // pointer to string {"stackStr", "stackStr"}
                    } else {
                        sprintf(buffer, format_specifier, *(char **)element); // dereferenced pointer to string pointer on heap {*{heapStr},*{heapStr}}
                    }
                } else {
                    printf(buffer, "Unsupported type: %s\n", format_specifier);
                    return NULL;
                }
                strcat(res, buffer);
                if (k < size3 - 1) {
                    strcat(res, ", ");
                }
            }
            strcat(res, "]");
            if (j < size2 - 1) {
                strcat(res, ", ");
            }
        }
        strcat(res, "]");
        if (i < size1 - 1) {
            strcat(res, ", ");
        }
    }
    strcat(res, "]");
    res[strlen(res)] = '\0';

    return res;
}

void _arrayToString(char *output, void *array, int dimensions[], int num_dimensions, int curr_dimension, int *index, int is_contiguous_block_of_memory, const char *type, int type_size, const char *format, int buff_size) {
    strcat(output, "[");
    for (int i = 0; i < dimensions[curr_dimension]; i++) {
        if (i > 0) {
            strcat(output, ", ");
        }

        if (curr_dimension == num_dimensions - 1) { // last dimension / innermost dimension
            char *buffer = (char *)malloc(buff_size * sizeof(char));
            void *element;

            if (is_contiguous_block_of_memory) {
                element = ((char *)array) + (*index) * type_size;
            } else {
                if (strcmp(type, "string") == 0) {
                    element = *((char **)array + i); // pointer to string pointer
                } else {
                    element = ((char *)array + i * type_size); // pointer to data
                }
            }

            if (strcmp(type, "string") == 0) {
                snprintf(buffer, buff_size, format, element);
            } else {
                if (strcmp(type, "int") == 0) {
                    snprintf(buffer, buff_size, format, *(int *)element);
                } else if(strcmp(type, "long") == 0) {
                    snprintf(buffer, buff_size, format, *(long *)element);
                } else if(strcmp(type, "size_t") == 0) {
                    snprintf(buffer, buff_size, format, *(size_t *)element);
                } else if(strcmp(type, "float") == 0) {
                    snprintf(buffer, buff_size, format, *(float *)element);
                } else if(strcmp(type, "double") == 0) {
                    snprintf(buffer, buff_size, format, *(double *)element);
                } else if(strcmp(type, "char") == 0) {
                    snprintf(buffer, buff_size, format, *(char *)element);
                } else {
                    snprintf(buffer, buff_size, "%s ?", type);
                }
                buffer[buff_size - 1] = '\0';
            }

            strcat(output, buffer);
            free(buffer);
            *index = (*index + 1); // only used when array is contiguous block of memory, useless otherwise
        } else {
            if (is_contiguous_block_of_memory) {
                // call unchanged, just increase dimension
                _arrayToString(output, array, dimensions, num_dimensions, curr_dimension + 1, index, is_contiguous_block_of_memory, type, type_size, format, buff_size);
            } else {
                // for every i in current dimension, jump into next/inner dimension
                _arrayToString(output, (void *)((void **)array)[i], dimensions, num_dimensions, curr_dimension + 1, index,  is_contiguous_block_of_memory, type, type_size, format, buff_size);
            }
        }
    }
    strcat(output, "]");
}

void arrayToString(char *output, void *array, int dimensions[], int num_dimensions, int is_contiguous_block_of_memory, const char *type, int type_size, const char *format, int buff_size) {
    int index = 0;
    int curr_dimension = 0;
    _arrayToString(output, array, dimensions, num_dimensions, curr_dimension, &index, is_contiguous_block_of_memory, type, type_size, format, buff_size);
}

// this works but is hardcoded to 1 - 4 dimensions
void initialize_multi_dimensional_array_(void **pointer, int *dimensions, int dimensions_length, size_t elem_size) {
    if (dimensions_length < 1 || dimensions_length > 4) {
        // This approach is designed only for 1D, 2D, and 3D arrays.
        return;
    }

    if (dimensions_length == 1) {
        *pointer = (char *)malloc(dimensions[0] * elem_size);
    }

    if (dimensions_length == 2) {
        *pointer = (char **)malloc(dimensions[0] * sizeof(char*));
        for (int i = 0; i < dimensions[0]; i++) {
            ((char **)(*pointer))[i] = (char *)malloc(dimensions[1] * elem_size);
        }
    }

    if (dimensions_length == 3) {
        *pointer = (char *)malloc(dimensions[0] * sizeof(char *));
        for (int i = 0; i < dimensions[0]; i++) {
            ((char **)*pointer)[i] = (char *)malloc(dimensions[1] * sizeof(char *));
            for (int j = 0; j < dimensions[1]; j++) {
                ((char ***)*pointer)[i][j] = (char *)malloc(dimensions[2] * elem_size);
            }
        }
    }


    if (dimensions_length == 4) {
        *pointer = (char *)malloc(dimensions[0] * sizeof(char *));
        for (int i = 0; i < dimensions[0]; i++) {
            ((char **)*pointer)[i] = (char *)malloc(dimensions[1] * sizeof(char *));
            for (int j = 0; j < dimensions[1]; j++) {
                ((char ***)*pointer)[i][j] = (char *)malloc(dimensions[2] * sizeof(char *));
                for (int k = 0; k < dimensions[2]; k++) {
                    ((char ****)*pointer)[i][j][k] = (char *)malloc(dimensions[3] * elem_size);
                }
            }
        }
    }
}

char *initialize_dimension(int *dimensions, int current_dimension, int dimensions_length, size_t elem_size) {
    // Base case: if the last dimension, allocate memory for actual elements
    if (current_dimension == dimensions_length - 1) {
        return (char*)malloc(dimensions[current_dimension] * elem_size);
    }

    // Otherwise, allocate memory for pointers to the next dimension
    char **arr = (char **)malloc(dimensions[current_dimension] * sizeof(char*));
    if (!arr) return NULL;

    for (int i = 0; i < dimensions[current_dimension]; i++) {
        arr[i] = initialize_dimension(dimensions, current_dimension + 1, dimensions_length, elem_size);
    }

    return (char *)arr;
}

void initialize_multi_dimensional_array(void **pointer, int *dimensions, int dimensions_length, size_t elem_size) {
    *pointer = initialize_dimension(dimensions, 0, dimensions_length, elem_size);
}

