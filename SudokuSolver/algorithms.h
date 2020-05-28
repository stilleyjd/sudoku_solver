#pragma once

#include "board_globals.h"

int sum_ints(int L, int x[]);

// void expand_candidate(int candidate_bin, int candidate_array[]);
// int collapse_candidate(int candidate_array[]);

// int check_search_result(int x[LEN]);

// void eval_cell(int board[LEN][LEN], int r, int c, int candidate_values[LEN]);

// void naked_single_cell(int board[LEN][LEN], int row, int col, int candidate_values[LEN]);

int naked_single_search(int board[LEN][LEN], int candidates[LEN][LEN][LEN]);

int randomized_value_board_search(int board[LEN][LEN], int candidates[LEN][LEN][LEN]);