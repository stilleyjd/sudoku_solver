/* 
Code to implment various Sudoku solver techniques 

Refences: 
    Davis, Tom. “The Mathematics of Sudoku”. 21 Nov 2007. 15 Feb 2008
        http://www.geometer.org/mathcircles/sudoku.pdf.
*/

#include <iostream>
#include <time.h>
#include <stdlib.h>
#include "algorithms.h"
#include "read_and_display.h"

int sum_values(int L, int x[]) {
    int sum = 0;
    for (int i = 0; i < L; i++) { sum = sum + x[i]; }
    return sum;
}

void eval_cell(int board[LEN][LEN], int r, int c, int existing_values[LEN]) {
    int* value = &board[r][c];
    // printf("Row %d, Col %d, Value %d, Address %d\n", row, i, *value, value);
    if (*value != 0) {
        existing_values[*value - 1] = 1; // acount for 0-based indexes...
    }
}

void search_cell(int board[LEN][LEN], int row, int col, int existing_values[LEN]) {
    // Searches the same row, column and box as the cell at location "row", "col"
    // Puts the results into the existing_values array (1 means a value is found)
    int r, c;

    // 1st, look through the same row
    for (c = 0; c < LEN; c++) {
        eval_cell(board, row, c, existing_values);
    }

    // 2nd, look through the same column
    for (r = 0; r < LEN; r++) {
        eval_cell(board, r, col, existing_values);
    }

    // 3rd, check the 3x3 box it is in
    for (r = (row / NUM) * NUM; r < (row / NUM + 1) * NUM; r++) {
        for (c = (col / NUM) * NUM; c < (col / NUM + 1) * NUM; c++) {
            eval_cell(board, r, c, existing_values);
        }
    }
}

int check_search_result(int x[LEN]) {
    // If only one result, then returns the value of that result
    // If no possible results, then returns -1 (error)
    // Otherwise, returns 0 meaning there are still multiple possible results

    int num_values = sum_values(LEN, x);

    if (num_values >= LEN) {
        // printf("number of found values is 9. No solution possible!!\n");
        return -1;
    }
    else if (num_values == LEN - 1) {
        for (int i = 0; i < LEN; i++)
        {
            // If only 1 unfound value, the solution must be the value that wasn't found
            if (x[i] == 0) {
                return i + 1;  // acount for 0-based indexes...
            }
        }
    }

    return 0;
}


int naked_single_search(int board[LEN][LEN]) {
    /* Naked Single: a cell for which there exists a unique candidate based on the circumstance
    that its groups contain all the other digits (Davis, 2007) */
    int row = 0;
    int col = 0;
    int existing_values[LEN] = { 0 };
    int result;
    int num_cells_completed = 0;

    for (row = 0; row < LEN; row++) {
        for (col = 0; col < LEN; col++) {
            if (board[row][col] == 0) {
                // reset the existing values array and then search this cell
                memset(existing_values, 0, sizeof(existing_values));

                // printf("\nSearching for results to: row %d, col %d\n", row+1, col+1);
                search_cell(board, row, col, existing_values);
                // For a basic search, just check if number of found values is 1 less than max (only one solution left)
                result = check_search_result(existing_values);

                if (result < 0) {
                    printf("\n\n No possible valid result for cell at row %d, column %d.\n   Cannot solve this puzzel!!!\n",
                        row + 1, col + 1);
                    return -1;
                }
                else if (result != 0) {
                    board[row][col] = result;
                    printf("\nFound result for row %d, col %d: %d\n", row + 1, col + 1, board[row][col]);
                    num_cells_completed++;
                }
            }
        }
    }
    return num_cells_completed;
}

int randomized_value_board_search(int board[LEN][LEN]) {
    int row = 0;
    int col = 0;
    int existing_values[LEN] = { 0 };
    int num_missing = 0;
    int result = 0;
    int num_cells_completed = 0;

    // Values for the cell with fewest unknown values
    int row_min = -1;
    int col_min = -1;
    int num_missing_min = LEN;
    int values_min[LEN] = { 0 };
    int rand_val = 0;
    
    for (row = 0; row < LEN; row++) {
        for (col = 0; col < LEN; col++) {
            if (board[row][col] == 0) {
                // reset the existing values array and then search this cell
                memset(existing_values, 0, sizeof(existing_values));

                // printf("\nSearching for results to: row %d, col %d\n", row+1, col+1);
                search_cell(board, row, col, existing_values);

                // ***
                // TODO: Combine this part into the simple search... and then randomize if no new values found
                // ***

                // Then, if fewer cells are missing than the fewest found, update values 
                num_missing = LEN - sum_values(LEN, existing_values);
                if (num_missing < num_missing_min) {
                    num_missing_min = num_missing;
                    row_min = row;
                    col_min = col;
                    memcpy(values_min, existing_values, sizeof(values_min));
                }
                // if only 2 values are missing, quit, since this is the smallest value possible.
                if (num_missing <= 2) {
                    break;
                }
            }
        } // col
    } // row

    // Finally, pick a value for the cell with fewest possible values (if one found)
    if (row_min >= 0) {
        printf("\nSelecting a random result for row %d, col %d.\n Possible values are: ", row_min + 1, col_min + 1);
        for (int idx = 0; idx < LEN; idx++) {
            if (values_min[idx] == 0) {
                printf("%d, ", idx + 1);
            }
        }

        srand(time(NULL));   // Init random number gen, should only be called once.
        while (result == 0) {
            rand_val = rand() % LEN; // Then gen a random value and see if it's a valid value
            if (values_min[rand_val] == 0) {
                result = rand_val + 1;
            }
        }

        printf("\nSelected a result of %d\n\n", result);
        board[row_min][col_min] = result;
        num_cells_completed++;
    }
    else {
        printf("\nNo cells could be found to evaluate for random values. Something went wrong!!!\n");
    }

    return num_cells_completed;
}