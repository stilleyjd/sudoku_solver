#pragma once

#include "board_globals.h"

void check_area(int board[LEN][LEN], int row_start, int row_end, int col_start, int col_end, int double_coords[]);

void check_for_double_values(int board[LEN][LEN], int double_coords[]);
