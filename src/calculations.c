#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "calculations.h"

RowCol get_row_col(int _index, int predefined_cols) {
    if (_index < 0 || predefined_cols < 1) return (RowCol){.row = -1, .col = -1};
    int index = _index + 1;
    int row = ceilf((float)index / (float)predefined_cols) - 1;
    int col_ = index % predefined_cols;
    int col = (col_ == 0 ? predefined_cols : col_) - 1;
    return (RowCol){.row = row, .col = col};
}

char** uv_help(int images_num, int total_cols, double shrink, int *num_lines) {
    int total_rows = images_num / total_cols + (images_num % total_cols > 0);
    double offset_horizontal_unit = (1 - shrink) / 2 / total_cols; // 0.02
    double offset_vertical_unit = (1 - shrink) / 2 / total_rows; // 0.02
    double col_width = 1.0 / total_cols;
    double row_height = 1.0 / total_rows;

    *num_lines = 1 + images_num;
    char **lines = malloc(*num_lines * sizeof(char*));
    if (lines == NULL) {
        printf("Could not allocate uv_help lines\n");
        return NULL;
    }

    lines[0] = malloc(MAX_UV_HELP_LINE_LENGTH * sizeof(char*));
    if (lines[0] == NULL) {
        printf("Could not allocate uv_help lines[0]\n");
        return NULL;
    }

    snprintf(lines[0], MAX_UV_HELP_LINE_LENGTH, "\ntotal_cols: %d, total_rows: %d, col_width: %.6f, row_height: %.6f, offset_horizontal_unit: %.6f, offset_vertical_unit: %.6f\n\n",
             total_cols, total_rows, col_width, row_height, offset_horizontal_unit, offset_vertical_unit);

    for (int i = 0; i < images_num; i++) {
        RowCol row_col = get_row_col(i, total_cols);
        double top_left_x = row_col.col * col_width + offset_horizontal_unit;
        double bottom_left_x = row_col.col * col_width + offset_horizontal_unit;
        double top_right_x = (row_col.col + 1) * col_width - offset_horizontal_unit;
        double bottom_right_x = (row_col.col + 1) * col_width - offset_horizontal_unit;

        double top_left_y = 1 - (row_col.row * row_height + offset_vertical_unit);
        double top_right_y = 1 - (row_col.row * row_height + offset_vertical_unit);
        double bottom_left_y = 1 - ((row_col.row + 1) * row_height - offset_vertical_unit);
        double bottom_right_y = 1 - ((row_col.row + 1) * row_height - offset_vertical_unit);

        lines[i+1] = malloc(MAX_UV_HELP_LINE_LENGTH * sizeof(char*));
        if (lines[i+1] == NULL) {
            printf("Could not allocate uv_help lines[%d]\n", i+1);
            return NULL;
        }

        snprintf(lines[i+1], MAX_UV_HELP_LINE_LENGTH,"Image: %d (row: %d, col: %d) =>\n\ttop_left: (x: %.6f, y: %.6f),\n\ttop_right: (x: %.6f, y: %.6f),\n\tbottom_left: (x: %.6f, y: %.6f),\n\tbottom_right: (x: %.6f, y: %.6f),\n\n",
               i, row_col.row, row_col.col,
               top_left_x, top_left_y, top_right_x, top_right_y, bottom_left_x, bottom_left_y, bottom_right_x, bottom_right_y
        );
    }
    return lines;
}