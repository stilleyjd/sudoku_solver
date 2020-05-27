#pragma once

#include "board_globals.h"

int sum_values(int L, int x[]);

int check_search_result(int x[LEN]);

void eval_cell(int board[LEN][LEN], int r, int c, int existing_values[LEN]);

void search_cell(int board[LEN][LEN], int row, int col, int existing_values[LEN]);

int naked_single_search(int board[LEN][LEN]);

int randomized_value_board_search(int board[LEN][LEN]);