#pragma once

#include "board_globals.h"

int naked_single_search(int board[LEN][LEN], int candidates[LEN][LEN][LEN]);

int hidden_single_search(int board[LEN][LEN], int candidates[LEN][LEN][LEN]);

int locked_candidate_search(int candidates[LEN][LEN][LEN]);

int naked_hidden_sets_search(int candidates[LEN][LEN][LEN], int set_size);

int randomized_value_board_search(int board[LEN][LEN], int candidates[LEN][LEN][LEN]);
