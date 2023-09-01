#include <stddef.h>
#include "calculations.h"

size_t get_rows_height(size_t *output, size_t*** rows_with_sizes, int rows_num, int cols_num) {
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
    arrayToString(rows_height_info, (void *)output, (int[]){rows_num}, 1, 0, "size_t", sizeof(size_t), "%zu", 8);
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
    size_t max_height = 0;
    for (int col = 0; col < cols_num; col++) {
        size_t col_height = 0;
        for (int row = 0; row < rows_num; row++) {
            col_height += rows_with_sizes[row][col][1];
        }
        if (col_height > max_height) max_height = col_height;
    }
    printf("atlas_height: %zu\n", max_height);
    return max_height;
}

// double*** output
void get_rows_with_sizes_as_percentage(void* array, size_t*** rows_with_sizes, size_t* rows_height, size_t atlas_width, size_t atlas_height, int rows_num, int cols_num, double shrink) {
    double (*output)[cols_num][4] = (double (*)[cols_num][4])array;
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
    arrayToString(rows_with_sizes_as_percentage_info, array, (int[]){rows_num, cols_num, 4}, 3, 1 , "double", sizeof(double), "%.6f", 8);
    printf("%s\n", rows_with_sizes_as_percentage_info);
}

UVCorners * get_uv_corners_arr(int images_num, size_t** input_sizes, int _total_cols, double shrink) {
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
    arrayToString(rows_with_sizes_info, (void *)rows_with_sizes, (int[]){total_rows, total_cols, 2}, 3, 0, "size_t", sizeof(size_t), "%zu", 8);
    printf("rows_with_sizes: %s\n", rows_with_sizes_info);

    size_t* rows_height = (size_t*)malloc(total_rows * sizeof(size_t));
    get_rows_height(rows_height, rows_with_sizes, total_rows, total_cols);

    size_t atlas_width = get_atlas_width(rows_with_sizes, total_rows, total_cols);
    size_t atlas_height = get_atlas_height(rows_with_sizes, total_rows, total_cols);
    printf("rows_with_sizes_as_percentage:             ");
    double rows_with_sizes_as_percentage[total_rows][total_cols][4];
    get_rows_with_sizes_as_percentage(rows_with_sizes_as_percentage, rows_with_sizes, rows_height, atlas_width, atlas_height, total_rows, total_cols, 1);
    printf("rows_with_sizes_as_percentage_with_shrink: ");
    double rows_with_sizes_as_percentage_with_shrink[total_rows][total_cols][4];
    get_rows_with_sizes_as_percentage(rows_with_sizes_as_percentage_with_shrink, rows_with_sizes, rows_height, atlas_width, atlas_height, total_rows, total_cols, shrink);

    printf("\ntotal_cols: %d, total_rows: %d, atlas_width: %zu, atlas_height: %zu\n\n",
             total_cols, total_rows, atlas_width, atlas_height);

    UVCorners *uvCornersArr = malloc(images_num * sizeof(UVCorners));
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
    return uvCornersArr;
}

char** print_UV_help(UVCorners *uvCornersArr, int images_num, int _total_cols, int *num_lines) {
    int total_rows = images_num / _total_cols + (images_num % _total_cols > 0);
    int total_cols = images_num < _total_cols ? images_num : _total_cols;

    *num_lines = 1 + images_num;
    char **lines = malloc(*num_lines * sizeof(char*));
    if (lines == NULL) {printf("Could not allocate uv_help lines\n"); return NULL; }

    lines[0] = malloc(MAX_UV_HELP_LINE_LENGTH * sizeof(char*));
    if (lines[0] == NULL) {printf("Could not allocate uv_help lines[0]\n"); return NULL; }

    snprintf(lines[0], MAX_UV_HELP_LINE_LENGTH, "\ntotal_cols: %d, total_rows: %d\n\n",
             total_cols, total_rows);

    for (int i = 0; i < images_num; i++) {
        int curr_row = i / _total_cols;
        int curr_col = i % _total_cols;
        UVCorners imageUVCorners = uvCornersArr[i];
        lines[i+1] = malloc(MAX_UV_HELP_LINE_LENGTH * sizeof(char*));
        if (lines[i+1] == NULL) { printf("Could not allocate uv_help lines[%d]\n", i+1); return NULL; }
        snprintf(lines[i+1], MAX_UV_HELP_LINE_LENGTH,"Image: %d (row: %d, col: %d) =>\n\ttop_left: (x: %.6f, y: %.6f),\n\ttop_right: (x: %.6f, y: %.6f),\n\tbottom_left: (x: %.6f, y: %.6f),\n\tbottom_right: (x: %.6f, y: %.6f),\n\n",
                 i, curr_row, curr_col,
                 imageUVCorners.topLeft[0], imageUVCorners.topLeft[1], imageUVCorners.topRight[0], imageUVCorners.topRight[1], imageUVCorners.bottomLeft[0], imageUVCorners.bottomLeft[1], imageUVCorners.bottomRight[0], imageUVCorners.bottomRight[1]
        );
    }
    return lines;
}