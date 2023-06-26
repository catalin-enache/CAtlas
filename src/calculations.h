
#ifndef ATLAS_CALCULATIONS_H
#define ATLAS_CALCULATIONS_H

#define MAX_UV_HELP_LINE_LENGTH 512

typedef struct {
    int row;
    int col;
} RowCol;

RowCol get_row_col(int index, int predefined_cols);

char** uv_help(int images_num, int total_cols, double shrink, int *num_lines);

#endif //ATLAS_CALCULATIONS_H
