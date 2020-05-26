// SudokuSolver.cpp : This tool takes in a sudoku board and generates a solution.

#include <iostream>
#include "board_globals.h"
#include "read_and_display.h"
#include "algorithms.h"
#include "validate.h"


int main()
{
    int board[LEN][LEN] = { 0 }; // Initial board state 

    int num_cells_left = LEN * LEN;
    int num_cells_left_prev = num_cells_left;
    int row = 0;
    int col = 0;
    int result = 0;
    int round = 0;
    
    // Setup board
    get_initial_values(board);

    // Check to make sure that each row, col, and box only has 1 - 9 once
    int double_values[2] = { -1, -1 };
    check_for_double_values(board, double_values);
    if (double_values[0] > -1) {
        printf("\nThe board has a double value at %d, %d! Cannot continue!!\n", double_values[0], double_values[1]);
        exit(EXIT_FAILURE);
    }
    
    // Init counters
    for (row = 0; row < LEN; row++) {
        for (col = 0; col < LEN; col++) {
            if (board[row][col] != 0) { num_cells_left--; }
        }
    }

    std::cout << "\n\nStarting!\n";
    printf("Cells completed:   %d\n", LEN*LEN - num_cells_left);
    printf("Cells to complete: %d\n\n", num_cells_left);

    // Main loop
    while (num_cells_left > 0) {
        round++;
        num_cells_left_prev = num_cells_left;

        for (row = 0; row < LEN; row++) {
            for (col = 0; col < LEN; col++) {
                if (board[row][col] == 0) {
                    // printf("\nSearching for results to: row %d, col %d\n", row+1, col+1);
                    result = basic_search(board, row, col);
                    if (result < 0) {
                        printf("\n\n No possible valid result for cell at row %d, column %d.\n   Cannot solve this puzzel!!!\n", 
                            row + 1, col + 1);
                        exit(EXIT_FAILURE);
                    }
                    else if (result != 0) {
                        board[row][col] = result;
                        printf("\nFound result for row %d, col %d: %d\n", row + 1, col + 1, board[row][col]);
                        num_cells_left--;
                    }
                }
            }
        }

        display_board(board);

        printf("Previous number of cells completed: %d\n", 81 - num_cells_left_prev);
        printf("Round %d: number of cells completed: %d\n", round, 81 - num_cells_left);

        // cells_left = 0;

        if (num_cells_left_prev == num_cells_left) {
            printf("No new cells could be solved in the last iteration. Giving up...");
            break; 

            // TODO: Try to randomize the cells with fewest opitons 
            //       to see if that gives a valid solution
        }
    }


    // Finish up
    if (num_cells_left == 0) {
        printf("\nFinished in %d rounds\n\n", round);

        // Check to make sure that each row, col, and box only has 1 - 9 once
        int double_values[2] = { -1, -1 };
        check_for_double_values(board, double_values);

        if (double_values[0] == -1) {
            printf("\nThe solution is valid. Success!!\n");
            exit(EXIT_SUCCESS);
        }
        else {
            printf("\nThe solution had an invalid result at %d, %d!!!\n", double_values[0], double_values[1]);
            exit(EXIT_FAILURE);
        }
    }
    else {
        printf("\n\nFAILED!\n");
        exit(EXIT_FAILURE);
    }
}

