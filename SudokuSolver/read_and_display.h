/* This file has functions used to read in initial boards from a file and display the board state
*/

#include "board_globals.h"

void display_board(int board[LEN][LEN]);

int get_initial_values(int board[LEN][LEN], int candidates[LEN][LEN][LEN]);

void display_candidates(int board[LEN][LEN], int candidates[LEN][LEN][LEN]);
