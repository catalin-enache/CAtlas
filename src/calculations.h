
#ifndef ATLAS_CALCULATIONS_H
#define ATLAS_CALCULATIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "utils.h"
#include "shared_types.h"

#define MAX_UV_HELP_LINE_LENGTH 512

typedef struct {
    double topLeft[2];
    double topRight[2];
    double bottomLeft[2];
    double bottomRight[2];
} UVCorners;

UVCorners* get_uv_corners_arr(size_t** input_sizes, int images_num, int _total_cols, double shrink, bool debug_uv_help);
UVCorners* get_abs_uv_corners_arr(AbsolutePositioningEntry * array, int array_length,  int canvas_width, int canvas_height, double shrink, bool debug_uv_help);
char** print_UV_help(UVCorners *uvCornersArr, int images_num, int _total_cols, int *num_lines);

#endif //ATLAS_CALCULATIONS_H
