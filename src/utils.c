#include "utils.h"

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


char* print_2d_array_of_double(double **arr, int arr_size, int arr_elem_size) {
    int d_size = 40; // arbitrary size of formatted double including surrounding chars (commas, parenthesis)
    char *res = (char*)malloc(arr_size * arr_elem_size * d_size * sizeof(char));
    if (res == NULL) {
        printf("Could not allocate memory for printing 2d array of doubles.\n");
        return NULL;
    }

    res[0] = '\0';
    strcpy(res, "[");
    for(int i = 0; i < arr_size; i++) {
        strcat(res, "[");
        for (int j = 0; j < arr_elem_size; j++) {
            char buffer[30];
            sprintf(buffer, "%.8f", arr[i][j]);
            strcat(res, buffer);
            if (j < arr_elem_size - 1) {
                strcat(res, ", ");
            }
        }
        strcat(res, "]");
    }
    strcat(res, "]");
    res[strlen(res)] = '\0';

    return res;
}
