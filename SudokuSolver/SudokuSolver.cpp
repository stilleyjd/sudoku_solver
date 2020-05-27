/* SudokuSolver.cpp : This tool takes in a user-provided sudoku board and generates a solution.
Jordan D. Stilley
Malmö, Sweden
April-June 2020

Note: most of techniques here come from:
    http://www.math.kent.edu/~malexand/Latex/Examples/Article%20Example/YSU_Sudoku.pdf
 - The Naked Single was created by the developer before reading this article, but then renamed to match
 - The randomized search was also independently created by the developer.
*/

#include <iostream>
#include "board_globals.h"
#include "read_and_display.h"
#include "algorithms.h"
#include "validate.h"


int main()
{
    int board[LEN][LEN] = { 0 }; // Initial board state 
    int num_empty_cells = LEN * LEN;
    int num_empty_cells_prev = num_empty_cells;
    int num_cells_found = 0;

    // Copy of intial values in case of failure TODO: Get rid of these if possible!
    int initial_board[LEN][LEN] = { 0 }; // Copy of Initial board state 
    int initial_num_empty_cells = num_empty_cells;

    int round = 0;
    int num_fails = 0;
    int double_values[2] = { -1, -1 };
    
    // Setup board
    num_empty_cells = get_initial_values(board);
    memcpy(initial_board, board, sizeof(initial_board));
    initial_num_empty_cells = num_empty_cells;

    // Check to make sure that each row, col, and box only has 1 - 9 once
    check_for_double_values(board, double_values);
    if (double_values[0] > -1) {
        printf("\nThe board has a double value at %d, %d! Cannot continue!!\n", double_values[0], double_values[1]);
        exit(EXIT_FAILURE);
    }

    std::cout << "\n\nStarting!\n";
    printf("Number of Completed Cells: %d\n", LEN*LEN - num_empty_cells);
    printf("Number of Empty Cells:     %d\n\n", num_empty_cells);

    // Main loop
    while (num_empty_cells > 0) {
        round++;
        num_empty_cells_prev = num_empty_cells;

        // Do one round of simple search first (look where missing value is deterministic)
        num_cells_found = naked_single_search(board);
        if (num_cells_found < 0) { // Something went wrong, so restart
            num_fails++;
            if (num_fails > 50) {
                printf("Failed too many times. Giving up...");
                exit(EXIT_FAILURE);
            }
            // Reset the board and counters
            memcpy(board, initial_board, sizeof(board)); 
            num_empty_cells = initial_num_empty_cells;
            continue;
        }
        num_empty_cells = num_empty_cells - num_cells_found;

        if (num_empty_cells_prev == num_empty_cells) {
            printf("\nNo new cells could be solved in the last iteration. Trying more advanced solutions");
            // Try to randomize the cells with fewest opitons to see if that gives a valid solution
            num_empty_cells = num_empty_cells - randomized_value_board_search(board);
        }

        // Then display the results
        display_board(board);
        printf("Previous number of cells completed: %d\n", 81 - num_empty_cells_prev);
        printf("Round %d: number of cells completed: %d\n\n", round, 81 - num_empty_cells);

    }

    // Finish up
    if (num_empty_cells == 0) {
        printf("\nFinished in %d rounds\n", round);
        printf("\n  Had %d failures\n\n", num_fails);

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

