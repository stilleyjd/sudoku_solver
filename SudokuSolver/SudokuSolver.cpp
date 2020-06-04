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
#include <time.h>
#include <cstring>
#include "board_globals.h"
#include "read_and_display.h"
#include "algorithms.h"
#include "validate.h"


int main()
{
    int board[LEN][LEN] = { 0 }; // Initial board state 
    int candidates[LEN][LEN][LEN] = { 0 }; // Possible values for the board

    int num_empty_cells = LEN * LEN;
    int num_empty_cells_prev = num_empty_cells;
    int num_cells_found = 0;
    int num_eliminations = 0;

    int num_times_naked_single = 0;
    int num_times_hidden_single = 0;
    int num_times_locked_candidate = 0;
    int num_times_hidden_pairs = 0;
    int num_times_random = 0;


    // Copy of initial values in case of failure
    // TODO: Get rid of these if possible!
    int initial_board[LEN][LEN] = { 0 }; // Copy of Initial board state
    int initial_candidates[LEN][LEN][LEN] = { 0 };
    int initial_num_empty_cells = num_empty_cells;
    srand((int) time(NULL));   // Init random number gen, should only be called once.

    int round = 0;
    int num_fails = 0;
    int used_random_values = 0;
    int double_values[2] = { -1, -1 };
    
    // Setup board
    num_empty_cells = get_initial_values(board, candidates);

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

    	// Init for next round
        if (used_random_values == 0) {
            // If random values have not been used, then update the board to fall back to.
            memcpy(initial_board, board, sizeof(initial_board));
            memcpy(initial_candidates, candidates, sizeof(initial_candidates));
            initial_num_empty_cells = num_empty_cells;
        }

    	if (round > 0) {
    		// Display the results from last round
    		display_board(board);
    		printf("Previous number of cells completed: %d\n", LEN*LEN - num_empty_cells_prev);
    		printf("Round %d: number of cells completed: %d\n\n", round, LEN * LEN - num_empty_cells);
    	}

        round++;
        num_empty_cells_prev = num_empty_cells;

        do {
            // Do naked single search first (where only one value is missing after looking at row, col, box)
            num_cells_found = naked_single_search(board, candidates);
            num_times_naked_single += num_cells_found;
            num_empty_cells -= num_cells_found;
        } while (num_cells_found > 0);

        if (num_empty_cells == 0) {
            break;
        } else if (num_cells_found < 0) { // If something went wrong (return value is < 0), restart
            num_fails++;
            if (num_fails > 10) {
                printf("Failed too many times. Giving up...\n");
                break;
            }
            else {
                printf("Resetting board back to last deterministic state.\n");
            }
            // Reset the board and counters
            memcpy(board, initial_board, sizeof(board));
            memcpy(candidates, initial_candidates, sizeof(candidates));
            num_empty_cells = initial_num_empty_cells;
            used_random_values = 0;
            continue;
        }

        // Hidden Singles
		printf("\nNo new cells could be solved in the last iteration.\n"
				"    Trying Hidden Singles Search\n");
		num_cells_found = hidden_single_search(board, candidates);
		num_times_hidden_single += num_cells_found;
		num_empty_cells -= num_cells_found;
		if (num_cells_found > 0) {
            continue;
		}

        // TODO: Locked Candidate: article
        // 	  http://www.math.kent.edu/~malexand/Latex/Examples/Article%20Example/YSU_Sudoku.pdf
        // TODO: Omission: https://www.learn-sudoku.com/omission.html
        //     when a candidate are contained inside a single house, pencil marks in other houses can be removed.
        //  Are locked candidate and omission the same??
		printf("\nNo new cells could be solved using Hidden Singles.\n"
				"    Trying a Locked Candidate Search\n");
		num_eliminations = locked_candidate_search(candidates);
		num_times_locked_candidate += num_eliminations;
		if (num_eliminations > 0) {
			continue;
		}

        // TODO: Naked Pairs: article and https://www.learn-sudoku.com/naked-pairs.html

        // TODO: Naked Triplets+ : similar to naked pairs
        //		https://www.learn-sudoku.com/naked-triplets.html

        // TODO: Hidden Pairs: similar to naked pairs  -- Try this before naked pairs!!
        // 		https://www.learn-sudoku.com/hidden-pairs.html
        //  Also, hidden triplets and quads...
		printf("\nNo candidates could be eliminated with previous techniques.\n"
				"    Trying a Naked/Hidden Pairs Search\n");
		num_eliminations = hidden_pairs_search(candidates);
		num_times_hidden_pairs += num_eliminations;

		if (num_eliminations > 0) {
			continue;
		}

        // TODO: More advanced techniques like X-wing
        //   https://www.learn-sudoku.com/advanced-techniques.html

        // Finally, try Random Value search
		printf("\nNo candidates could be eliminated with previous techniques.\n"
				"    Trying randomized solution...\n");
		// Final approach: Try to randomize the cells with fewest opitons to see if that gives a valid solution
		num_cells_found = randomized_value_board_search(board, candidates);
		num_empty_cells -= num_cells_found;
		num_times_random += num_cells_found;

		if (num_cells_found > 0) {
			used_random_values = 1;
		}
		else {
			break;
		}

    }

    // Finish up
    printf("\nFinal Solution: \n");
    display_board(board);

    printf("\nNumber of times the various algorithms were used: \n");
    printf("  Naked Singles:  %d\n", num_times_naked_single);
    printf("  Hidden Singles: %d\n", num_times_hidden_single);
    printf("  Locked Candidate:  %d\n", num_times_locked_candidate);
    printf("  Naked / Hidden Pairs: %d\n", num_times_hidden_pairs);
    printf("  Random Choice:  %d\n", num_times_random);

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

