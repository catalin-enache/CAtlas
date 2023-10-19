#include <stddef.h>
#include "calculations.h"

void get_rows_height(size_t *output, size_t*** rows_with_sizes, int rows_num, int cols_num) {
    for (int row = 0; row < rows_num; row++) {
        size_t row_height = 0;
        for (int col = 0; col < cols_num; col++) {
            if (rows_with_sizes[row][col][1] > row_height) {
                row_height = rows_with_sizes[row][col][1];
            };
        }
        output[row] = row_height;
    }
    char rows_height_info[1000] = "";
    int dimensions[] = {rows_num};
    arrayToString(rows_height_info, (void *)output, dimensions, 1, 0, "size_t", sizeof(size_t), "%zu", 8);
    printf("rows_height: %s\n", rows_height_info);
}

size_t get_atlas_width(size_t*** rows_with_sizes, int rows_num, int cols_num) {
    size_t max_width = 0;
    for (int row = 0; row < rows_num; row++) {
        size_t row_width = 0;
        for (int col = 0; col < cols_num; col++) {
            row_width += rows_with_sizes[row][col][0];
        }
        if (row_width > max_width) max_width = row_width;
    }
    printf("atlas_width: %zu\n", max_width);
    return max_width;
}

size_t get_atlas_height(size_t*** rows_with_sizes, int rows_num, int cols_num) {
    size_t atlas_height = 0;
    for (int row = 0; row < rows_num; row++) {
        size_t row_height = 0;
        for (int col = 0; col < cols_num; col++) {
            size_t col_height = rows_with_sizes[row][col][1];
            if (col_height > row_height) row_height = col_height;
        }
        atlas_height += row_height;
    }
    printf("atlas_height: %zu\n", atlas_height);
    return atlas_height;
}

double *** get_rows_with_sizes_as_percentage(size_t*** rows_with_sizes, size_t* rows_height, size_t atlas_width, size_t atlas_height, int rows_num, int cols_num, double shrink, bool debug_uv_help) {
    // If the initialised array be passed in, this would work in C but not in C++ (VLAs - Variable Length Arrays) are not supported in C++
    // double (*output)[cols_num][4] = (double (*)[cols_num][4])array; // C (not working in C++ due to VLAs)
    // double ***output = (double ***)array; // C++
    // Anyways, we're initializing it internally and returning it.
    double ***output = NULL;

    int _dimensions[] = {rows_num, cols_num, 4};
    initialize_multi_dimensional_array((void **)&output, _dimensions, 3, sizeof(double));

    for (int row = 0; row < rows_num; row++) {
        for (int col = 0; col < cols_num; col++) {
            double width = (double)rows_with_sizes[row][col][0] / (double)atlas_width;
            double height = (double)rows_with_sizes[row][col][1] / (double)atlas_height;
            double x = 0;
            double y = 0;
            for (int i = 0; i < col; i++) {
                x += (double)rows_with_sizes[row][i][0] / (double)atlas_width;
            }
            for (int i = 0; i < row; i++) {
                y += (double)rows_height[i] / (double)atlas_height;
            }
            output[row][col][0] = x;
            output[row][col][1] = y;
            output[row][col][2] = width;
            output[row][col][3] = height;

            double offset_horizontal_unit = width * ((1 - shrink) / 2); // width * 0.04 when shrink is 0.92
            double offset_vertical_unit = height * ((1 - shrink) / 2); // width * 0.04 when shrink is 0.92
            output[row][col][0] += offset_horizontal_unit;
            output[row][col][1] += offset_vertical_unit;
            output[row][col][2] -= offset_horizontal_unit * 2;
            output[row][col][3] -= offset_vertical_unit * 2;
        }
    }
    char rows_with_sizes_as_percentage_info[1000] = "";
    int dimensions[] = {rows_num, cols_num, 4};
    arrayToString(rows_with_sizes_as_percentage_info, output, dimensions, 3, 0 , "double", sizeof(double), "%.6f", 8);
    if (debug_uv_help) printf("%s\n", rows_with_sizes_as_percentage_info);
    return output;
}

UVCorners * get_uv_corners_arr(size_t** input_sizes, int images_num, int _total_cols, double shrink, bool debug_uv_help) {
    // [ [ [1,2], [1,2] ], [ [1,2], [1,2] ] ]
    int total_rows = images_num / _total_cols + (images_num % _total_cols > 0);
    int total_cols = images_num < _total_cols ? images_num : _total_cols;
    size_t*** rows_with_sizes = (size_t***)malloc(total_rows * sizeof(size_t**)); // allocate memory for rows array
    if (rows_with_sizes == NULL) { printf("Could not allocate rows_with_sizes\n"); return NULL; }

    int last_row = -1;
    for (int img = 0; img < images_num; img++) {
        int curr_row = img / _total_cols;
        int curr_col = img % _total_cols;
        if (curr_row > last_row) {
            rows_with_sizes[curr_row] = (size_t**)malloc(total_cols * sizeof(size_t*)); // allocate memory for columns array in current row
            if (rows_with_sizes[curr_row] == NULL) { printf("Could not allocate rows_with_sizes[%d]\n", curr_row); return NULL; }
            for (int col = 0; col < total_cols; col++) {
                rows_with_sizes[curr_row][col] = (size_t*)malloc(2 * sizeof(size_t)); // allocate memory for w/h array for current col in current row
                if (rows_with_sizes[curr_row][col] == NULL) { printf("Could not allocate rows_with_sizes[%d][%d]\n", curr_row, col); return NULL; }
                rows_with_sizes[curr_row][col][0] = 0;
                rows_with_sizes[curr_row][col][1] = 0;
            }
            // populate first column in curr_row with input sizes
            rows_with_sizes[curr_row][curr_col][0] = input_sizes[img][0];
            rows_with_sizes[curr_row][curr_col][1] = input_sizes[img][1];
            last_row = curr_row;
        } else {
            // populate other columns in curr_row with input sizes
            rows_with_sizes[last_row][curr_col][0] = input_sizes[img][0];
            rows_with_sizes[last_row][curr_col][1] = input_sizes[img][1];
        }
        // printf("input_sizes[%d] = [%zu, %zu]\n", img, input_sizes[img][0], input_sizes[img][1]);
    }

    char rows_with_sizes_info[1000] = "";
    int _dimensions[] = {total_rows, total_cols, 2};
    arrayToString(rows_with_sizes_info, (void *)rows_with_sizes, _dimensions, 3, 0, "size_t", sizeof(size_t), "%zu", 8);
    printf("rows_with_sizes: %s\n", rows_with_sizes_info);

    size_t* rows_height = (size_t*)malloc(total_rows * sizeof(size_t));
    get_rows_height(rows_height, rows_with_sizes, total_rows, total_cols);

    size_t atlas_width = get_atlas_width(rows_with_sizes, total_rows, total_cols);
    size_t atlas_height = get_atlas_height(rows_with_sizes, total_rows, total_cols);

    if (debug_uv_help) printf("rows_with_sizes_as_percentage:             ");
    double *** rows_with_sizes_as_percentage = get_rows_with_sizes_as_percentage(rows_with_sizes, rows_height, atlas_width, atlas_height, total_rows, total_cols, 1, debug_uv_help);

    if (debug_uv_help) printf("rows_with_sizes_as_percentage_with_shrink: ");
    double *** rows_with_sizes_as_percentage_with_shrink = get_rows_with_sizes_as_percentage(rows_with_sizes, rows_height, atlas_width, atlas_height, total_rows, total_cols, shrink, debug_uv_help);

    printf("\ntotal_cols: %d, total_rows: %d, atlas_width: %zu, atlas_height: %zu\n\n",
             total_cols, total_rows, atlas_width, atlas_height);

    UVCorners *uvCornersArr = (UVCorners *)malloc(images_num * sizeof(UVCorners));
    if (uvCornersArr == NULL) {printf("Could not allocate uvCornersArr\n"); return NULL; }
    for (int i = 0; i < images_num; i++) {
        int curr_row = i / _total_cols;
        int curr_col = i % _total_cols;
        double *dimensions = rows_with_sizes_as_percentage_with_shrink[curr_row][curr_col];

        double top_left_x = dimensions[0];
        double bottom_left_x = dimensions[0];
        double top_right_x = dimensions[0] + dimensions[2];
        double bottom_right_x = dimensions[0] + dimensions[2];

        double top_left_y = dimensions[1];
        double top_right_y = dimensions[1];
        double bottom_left_y = dimensions[1] + dimensions[3];
        double bottom_right_y = dimensions[1] + dimensions[3];

        uvCornersArr[i] = (UVCorners) {
                .topLeft = {top_left_x, top_left_y},
                .topRight = {top_right_x, top_right_y},
                .bottomLeft = {bottom_left_x, bottom_left_y},
                .bottomRight = {bottom_right_x, bottom_right_y}
        };

    }
    int rows_with_sizes_dimensions[] = {total_rows, total_cols, 4};
    free_multi_dimensional_array((void **)&rows_with_sizes_as_percentage, rows_with_sizes_dimensions, 3, "");
    free_multi_dimensional_array((void **)&rows_with_sizes_as_percentage_with_shrink, rows_with_sizes_dimensions, 3, "");
    free_multi_dimensional_array((void **)&rows_with_sizes, rows_with_sizes_dimensions, 3, "");
    free(rows_height);

    return uvCornersArr;
}

UVCorners* get_abs_uv_corners_arr(AbsolutePositioningEntry * array, int array_length, int canvas_width, int canvas_height, double shrink, bool debug_uv_help) {
    UVCorners *uvCornersArr = (UVCorners *)malloc(array_length * sizeof(UVCorners));
    if (uvCornersArr == NULL) {printf("Could not allocate uvCornersArr\n"); return NULL; }

    for (int i = 0; i < array_length; i++) {
        AbsolutePositioningEntry entry = array[i];
        double width = (double)entry.width / (double)canvas_width;
        double height = (double)entry.height / (double)canvas_height;
        double x = (double)entry.offset_x / (double)canvas_width;
        double y = (double)entry.offset_y / (double)canvas_height;

        double offset_horizontal_unit = width * ((1 - shrink) / 2); // width * 0.04 when shrink is 0.92
        double offset_vertical_unit = height * ((1 - shrink) / 2); // width * 0.04 when shrink is 0.92
        x += offset_horizontal_unit;
        y += offset_vertical_unit;
        width -= offset_horizontal_unit * 2;
        height -= offset_vertical_unit * 2;

        uvCornersArr[i] = (UVCorners) {
                .topLeft = {x, y},
                .topRight = {x + width, y},
                .bottomLeft = {x, y + height},
                .bottomRight = {x + width, y + height}
        };
    }
    return uvCornersArr;
}

char** print_UV_help(UVCorners *uvCornersArr, int images_num, int _total_cols, int *num_lines) {
    // this function is reused between using cols or using absolute positioning
    // when using cols, _total_cols is not 0, when using absolute positioning, _total_cols is 0
    int total_rows = _total_cols ? (images_num / _total_cols + (images_num % _total_cols > 0)) : 0;
    int total_cols = _total_cols ? (images_num < _total_cols ? images_num : _total_cols) : 0;

    *num_lines = 1 + images_num;
    char **lines = (char**)malloc(*num_lines * sizeof(char*));
    if (lines == NULL) {printf("Could not allocate uv_help lines\n"); return NULL; }

    lines[0] = (char*)malloc(MAX_UV_HELP_LINE_LENGTH * sizeof(char));
    if (lines[0] == NULL) {printf("Could not allocate uv_help lines[0]\n"); return NULL; }

    if (_total_cols) {
        snprintf(lines[0], MAX_UV_HELP_LINE_LENGTH, "\ntotal_cols: %d, total_rows: %d\n\n",
                 total_cols, total_rows);
    } else {
        snprintf(lines[0], MAX_UV_HELP_LINE_LENGTH, "\n");
    }


    for (int i = 0; i < images_num; i++) {
        int curr_row = _total_cols ? i / _total_cols : 0;
        int curr_col = _total_cols ? i % _total_cols : 0;
        UVCorners imageUVCorners = uvCornersArr[i];
        lines[i+1] = (char *)malloc(MAX_UV_HELP_LINE_LENGTH * sizeof(char));
        if (lines[i+1] == NULL) { printf("Could not allocate uv_help lines[%d]\n", i+1); return NULL; }
        if (_total_cols) {
            snprintf(lines[i+1], MAX_UV_HELP_LINE_LENGTH,"Image: %d (row: %d, col: %d) =>\n\ttop_left: (x: %.6f, y: %.6f),\n\ttop_right: (x: %.6f, y: %.6f),\n\tbottom_left: (x: %.6f, y: %.6f),\n\tbottom_right: (x: %.6f, y: %.6f),\n\n",
                     i, curr_row, curr_col,
                     imageUVCorners.topLeft[0], imageUVCorners.topLeft[1], imageUVCorners.topRight[0], imageUVCorners.topRight[1], imageUVCorners.bottomLeft[0], imageUVCorners.bottomLeft[1], imageUVCorners.bottomRight[0], imageUVCorners.bottomRight[1]
            );
        } else {
            snprintf(lines[i+1], MAX_UV_HELP_LINE_LENGTH,"Image: %d =>\n\ttop_left: (x: %.6f, y: %.6f),\n\ttop_right: (x: %.6f, y: %.6f),\n\tbottom_left: (x: %.6f, y: %.6f),\n\tbottom_right: (x: %.6f, y: %.6f),\n\n",
                     i,
                     imageUVCorners.topLeft[0], imageUVCorners.topLeft[1], imageUVCorners.topRight[0], imageUVCorners.topRight[1], imageUVCorners.bottomLeft[0], imageUVCorners.bottomLeft[1], imageUVCorners.bottomRight[0], imageUVCorners.bottomRight[1]
            );
        }

    }
    return lines;
}

