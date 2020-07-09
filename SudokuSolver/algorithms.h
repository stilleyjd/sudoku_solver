#pragma once

#include "board_globals.h"

int naked_single_search(short board[LEN][LEN], short candidates[LEN][LEN][LEN]);

int hidden_single_search(short board[LEN][LEN], short candidates[LEN][LEN][LEN]);

int locked_candidate_search(short candidates[LEN][LEN][LEN]);

void naked_hidden_sets_search(short candidates[LEN][LEN][LEN], int set_size, int* num_naked, int* num_hidden);

int basic_fish_search(short candidates[LEN][LEN][LEN], int fish_size);

int randomized_value_board_search(short board[LEN][LEN], short candidates[LEN][LEN][LEN]);
