#ifndef ATLAS_UTILS_H
#define ATLAS_UTILS_H

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <vips/vips.h>

#include "img_manip.h"
char *trim(char *s);
char* print_2d_array_of_double(double **arr, int arr_size, int arr_elem_size);
char* print_min_max_for_each_band(VipsImage *in);
#endif //ATLAS_UTILS_H
