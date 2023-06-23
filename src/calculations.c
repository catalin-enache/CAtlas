
#include <stdio.h>
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