#pragma once

#include "board_globals.h"

int sum_values(int L, int x[]);

int check_search_result(int x[LEN]);

void eval_cell(int board[LEN][LEN], int r, int c, int found_values[LEN]);

int basic_search(int board[LEN][LEN], int row, int col);