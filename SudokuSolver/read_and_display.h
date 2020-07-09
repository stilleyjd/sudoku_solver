/* This file has functions used to read in initial boards from a file and display the board state
*/

#include "board_globals.h"

void display_board(short board[LEN][LEN]);

void display_candidates(short board[LEN][LEN], short candidates[LEN][LEN][LEN]);

int get_initial_values(short board[LEN][LEN], short candidates[LEN][LEN][LEN], char file_name[MAX_FILENAME_SIZE]);

int save_board_to_file(short board[LEN][LEN], char file_name[MAX_FILENAME_SIZE]);
