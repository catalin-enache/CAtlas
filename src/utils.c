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
            sprintf(buffer, "%.6f", arr[i][j]);
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

char* print_min_max_for_each_band(VipsImage *in) {
    double **bands_min_max = get_min_max_for_each_band(in);
    if (bands_min_max == NULL) {
        printf("Could not get min max for each band.\n");
        return NULL;
    }
    char *res = print_2d_array_of_double(bands_min_max, in->Bands, 2);
    if (res == NULL) {
        printf("Could not print min max for each band.\n");
        return NULL;
    }
    for (int i = 0; i < in->Bands; i++) {
        free(bands_min_max[i]);
    }
    free(bands_min_max);
    return res;
}