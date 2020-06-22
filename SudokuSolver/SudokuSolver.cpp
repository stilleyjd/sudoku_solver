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


struct BoardStats {
    int num_empty_cells = LEN * LEN;
    int num_times_naked_single = 0;
    int num_times_hidden_single = 0;
    int num_times_locked_candidate = 0;
    int num_times_hidden_pairs = 0;
    int num_times_random = 0;
};


int main()
{
    int board[LEN][LEN] = { 0 }; // Initial board state
    int candidates[LEN][LEN][LEN] = { 0 }; // Possible values for the board
    struct BoardStats board_stats;

    int round = 0;
    int num_cells_found = 0;
    int num_cells_completed = 0;
    int num_eliminations = 0;
    int num_fails = 0;
    int double_values[2] = { -1, -1 };

    // Copy of initial values in case of failure (the last deterministic state of the board)
    // TODO: Get rid of these if possible!
    int det_board[LEN][LEN] = { 0 };
    int det_candidates[LEN][LEN][LEN] = { 0 };
    struct BoardStats det_board_stats;

    // Prepare for random values
    int used_random_values = 0;
    srand((int) time(NULL));   // Initialize the random number generator, should only be called once.
    

    // Setup board
    board_stats.num_empty_cells = get_initial_values(board, candidates);

    // Check to make sure that each row, col, and box only has 1 - 9 once
    check_for_double_values(board, double_values);
    if (double_values[0] > -1) {
        printf("\nThe board has a double value at %d, %d! Cannot continue!!\n", double_values[0], double_values[1]);
        exit(EXIT_FAILURE);
    }

    std::cout << "\n\nStarting!\n";
    num_cells_completed = LEN*LEN - board_stats.num_empty_cells;
    printf("Number of Completed Cells: %d\n", num_cells_completed);
    printf("Number of Remaining Cells: %d\n\n", board_stats.num_empty_cells);

    // Main loop
    while (board_stats.num_empty_cells > 0) {

    	// Finish up the previous round
    	if (round > 0) {
    		// Display the results from last round
            if (LEN < 10) {
                display_candidates(board, candidates);
            }
    		printf("Previous number of cells completed: %d\n", num_cells_completed);
            num_cells_completed = LEN*LEN - board_stats.num_empty_cells; // Update number completed for next round
    		printf("Round %d: number of cells completed: %d\n\n", round, num_cells_completed);
    	}

    	// Prepare for the next round
    	round++;
        if (used_random_values == 0) {
            // If random values have not been used, then update the board to fall back to.
            memcpy(det_board, board, sizeof(det_board));
            memcpy(det_candidates, candidates, sizeof(det_candidates));
            det_board_stats = board_stats;
        }

        do {
            // Do naked single search first (where only one value is missing after looking at row, col, box)
            num_cells_found = naked_single_search(board, candidates);
            board_stats.num_times_naked_single += num_cells_found;
            board_stats.num_empty_cells -= num_cells_found;
        } while (num_cells_found > 0);

        // Check board conditions before moving on
        if (board_stats.num_empty_cells == 0) {
            break;  // If nothing else to solve, quit
        } else if (num_cells_found < 0) { // If something went wrong (no possible solution for a cell)
            num_fails++;

            if (used_random_values == 0) {
            	// If failed when not using random values, something is really wrong!  Quit and fix!!!
                printf("Failed without using random values, only deterministic means!!!.\n");
                printf("    Something went really wrong! You need to fix your code!\n");
                break;
            } else if (num_fails > 20) {
                printf("Failed too many times. Giving up...\n");
                break;
            }
            else {
                printf("Resetting board back to last deterministic state.\n");
            }

            // Reset the board and counters, then start over
            memcpy(board, det_board, sizeof(board));
            memcpy(candidates, det_candidates, sizeof(candidates));
            board_stats = det_board_stats;
            used_random_values = 0;
            continue;
        }

        // Hidden Singles
		printf("\nNo new cells could be solved in the last iteration.\n"
				"    Trying Hidden Singles Search\n");
		num_cells_found = hidden_single_search(board, candidates);
		board_stats.num_times_hidden_single += num_cells_found;
		board_stats.num_empty_cells -= num_cells_found;
		if (num_cells_found > 0) {
            continue;  // if hidden singles worked, go back to naked singles
		}

        // If made it this far, then display candidate values for next algorithms (and if puzzle is not too big...)
        if (LEN < 10) {
            display_candidates(board, candidates);
        }

        // Locked Candidate
        // TODO: Quit once one elimination has been made??
		printf("\nNo new cells could be solved using Hidden Singles.\n"
				"    Trying a Locked Candidate Search\n");
		num_eliminations = locked_candidate_search(candidates);
		board_stats.num_times_locked_candidate += num_eliminations;
		if (num_eliminations > 0) {
			continue;  // If locked candidates did something, give previous techniques another chance
		}

        // TODO: Naked/Hidden Sets. Includes:
	    //	Naked Pairs: article and https://www.learn-sudoku.com/naked-pairs.html
        // 	Naked Triplets+ : similar to naked pairs: https://www.learn-sudoku.com/naked-triplets.html
        // 	Hidden Pairs: similar to naked pairs
		//    -- Try this before naked pairs, as naked pairs is a sub-set of this!
        // 		https://www.learn-sudoku.com/hidden-pairs.html
        //  	Also, can extend to hidden triplets and quadruplets ...
		printf("\nNo candidates could be eliminated with previous techniques.\n"
				"    Trying a Naked/Hidden Pairs Search\n");
		num_eliminations = hidden_sets_search(candidates, 2);
		board_stats.num_times_hidden_pairs += num_eliminations;
		if (num_eliminations > 0) {
			continue; // If hidden sets did something, give previous techniques another chance
		}
		// TODO: Try with larger set size


        // TODO??: More advanced techniques like X-wing
        //   https://www.learn-sudoku.com/advanced-techniques.html


        // Finally, try a Brute Force technique
		printf("\nNo candidates could be eliminated with previous techniques.\n"
				"    Trying randomized solution for a cell...\n");
		// TODO: Make this more systematic than random (like step through candidates instead)
		num_cells_found = randomized_value_board_search(board, candidates);
		board_stats.num_empty_cells -= num_cells_found;
		board_stats.num_times_random += num_cells_found;

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
    printf("  Naked Singles:  %d\n", board_stats.num_times_naked_single);
    printf("  Hidden Singles: %d\n", board_stats.num_times_hidden_single);
    printf("  Locked Candidate:  %d\n", board_stats.num_times_locked_candidate);
    printf("  Naked / Hidden Pairs: %d\n", board_stats.num_times_hidden_pairs);
    printf("  Random Choice:  %d\n", board_stats.num_times_random);

    if (board_stats.num_empty_cells == 0) {
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

