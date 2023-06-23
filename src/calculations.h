
#ifndef ATLAS_CALCULATIONS_H
#define ATLAS_CALCULATIONS_H

typedef struct {
    int row;
    int col;
} RowCol;

RowCol get_row_col(int index, int predefined_cols);

#endif //ATLAS_CALCULATIONS_H
