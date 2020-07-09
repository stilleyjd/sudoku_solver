/* SudokuSolver.cpp : This tool takes in a user-provided sudoku board and generates a solution.
Jordan D. Stilley
Malmö, Sweden
April-June 2020

Note: some of techniques here come from:
    http://www.math.kent.edu/~malexand/Latex/Examples/Article%20Example/YSU_Sudoku.pdf
 - The Naked Single was created by the developer before reading this article, but then renamed to match
 - The brute force (randomized value) search was also independently created by the developer.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "board_globals.h"
#include "log.h"
#include "read_and_display.h"
#include "algorithms.h"
#include "validate.h"

const int max_retries = 2000;

struct BoardStats {
	short num_empty_cells;
	short naked_single;
	short hidden_single;
	short locked_candidate;
	short naked_pairs;
	short hidden_pairs;
	short naked_triples;
	short hidden_triples;
	short naked_quads;
	short hidden_quads;
	short naked_quints;
	short hidden_quints;
	short x_wing;
    short swordfish;
    short jellyfish;
    short random;
};


int main (int argc, char *argv[]) {

	char file_name[MAX_FILENAME_SIZE] = "Not_yet_entered";

	short board[LEN][LEN] = { 0 }; // Initial board state
	short candidates[LEN][LEN][LEN] = { 0 }; // Possible values for the board
    struct BoardStats board_stats = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    int round = 0;
    int num_fails = 0;

    short num_cells_found = 0;
    short num_cells_completed = 0;
    short num_eliminations = 0;

    int num_naked = 0;
    int num_hidden = 0;

    short difficulty = 0;

    // Copy of initial values in case of failure (the last deterministic state of the board)
    // TODO: Get rid of these if possible. (if can get rid of brute force technique)
    short det_board[LEN][LEN] = { 0 };
    short det_candidates[LEN][LEN][LEN] = { 0 };
    struct BoardStats det_board_stats;

    // Prepare for random values
    bool used_random_values = false;
    srand((int) time(NULL));   // Initialize the random number generator, should only be called once.
    

    // Check if user provided a filename
    if (argc == 2) {  // Assume parameter is the filename if given
    	strcpy(file_name, argv[1]);
    	dbprintf("User provided file name: %s\n", file_name);
    }

    // Setup board
    board_stats.num_empty_cells = get_initial_values(board, candidates, file_name);

    // Check to make sure that each row, col, and box only has 1 - 9 once
    if (exists_no_double_values(board) == false) {
        printf("Cannot continue due to invalid board!!\n");
        exit(EXIT_FAILURE);
    }

    num_cells_completed = LEN*LEN - board_stats.num_empty_cells;
    printf("\nNumber of Completed Cells: %d\n", num_cells_completed);
    printf("Number of Remaining Cells: %d\n", board_stats.num_empty_cells);
    printf("\n--------------------------------------------------------------------------\n");
    printf("\nStarting...\n\n");

    // Main loop
    while (board_stats.num_empty_cells > 0) {

		#ifdef PRINT_DEBUG
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
		#endif

    	// Prepare for the next round
    	round++;
        if (used_random_values == false) {
            // If random values have not been used, then update the board to fall back to.
            memcpy(det_board, board, sizeof(det_board));
            memcpy(det_candidates, candidates, sizeof(det_candidates));
            det_board_stats = board_stats;
        }

        do {
            // Do naked single search first (where only one value is missing after looking at row, col, box)
            num_cells_found = naked_single_search(board, candidates);
            board_stats.naked_single += num_cells_found;
            board_stats.num_empty_cells -= num_cells_found;
        } while (num_cells_found > 0);

        // Check board conditions before moving on
        if (board_stats.num_empty_cells == 0) {
            break;  // If nothing else to solve, quit
        }
        else if (num_cells_found < 0) { // If something went wrong (no possible solution for a cell)
            num_fails++;

            if (used_random_values == false) {
            	// If failed when not using random values, something is really wrong!  Quit and fix!!!
                printf("Failed without using random values, only deterministic means!!!.\n");
                printf("    Something went really wrong! You need to fix your code!\n");
                break;
            } else if (num_fails > max_retries) {
                printf("Failed too many times. Giving up...\n");
                break;
            }

            dbprintf("Resetting board back to last deterministic state.\n");

            // Reset the board and counters, then start over
            memcpy(board, det_board, sizeof(board));
            memcpy(candidates, det_candidates, sizeof(candidates));
            board_stats = det_board_stats;
            used_random_values = false;
            continue;
        }


        /* ************************  More advanced Techniques ************************ */
        // Hidden Singles
        dbprintf("\nNo new cells could be solved in the last iteration.\n"
        		"    Trying Hidden Singles Search\n");
		num_cells_found = hidden_single_search(board, candidates);
		board_stats.hidden_single += num_cells_found;
		board_stats.num_empty_cells -= num_cells_found;
		if (num_cells_found > 0) {
            continue;  // if hidden singles worked, go back to naked singles
		}

        // Locked Candidate
		dbprintf("\nNo new cells could be solved using Hidden Singles.\n"
				"    Trying a Locked Candidate Search...\n");
		num_eliminations = locked_candidate_search(candidates);
		board_stats.locked_candidate += num_eliminations;
		if (num_eliminations > 0) {
			continue;  // If locked candidates did something, give previous techniques another chance
		}

		#ifdef PRINT_DEBUG
        // If made it this far, then display candidate values for next algorithms (and if puzzle is not too big...)
        if (LEN < 10) {
            display_candidates(board, candidates);
        }
		#endif


        // Naked/Hidden Sets. Includes:
	    //	Naked Pairs: article and https://www.learn-sudoku.com/naked-pairs.html
		//  Hidden Pairs: https://www.learn-sudoku.com/hidden-pairs.html
        // 	Naked Triplets+ : similar to naked pairs: https://www.learn-sudoku.com/naked-triplets.html
		num_naked = 0;
		num_hidden = 0;

        // Pairs
		dbprintf("    Trying a Naked/Hidden Pairs Search...\n");
		naked_hidden_sets_search(candidates, 2, &num_naked, &num_hidden);
		board_stats.naked_pairs += num_naked;
		board_stats.hidden_pairs += num_hidden;
		if (num_naked + num_hidden > 0) {
			continue; // If naked / hidden pairs did something, give previous techniques another chance
		}
		// Triples
		dbprintf("    Trying a Naked/Hidden Triples Search...\n");
		naked_hidden_sets_search(candidates, 3, &num_naked, &num_hidden);
		board_stats.naked_triples += num_naked;
		board_stats.hidden_triples += num_hidden;
		if (num_naked + num_hidden > 0) {
			continue; // If naked / hidden triplets did something, give previous techniques another chance
		}


		// Fish Searches
        // X-Wing
		dbprintf("    Trying a X-Wing Search...\n");
		num_eliminations = basic_fish_search(candidates, 2);
		board_stats.x_wing += num_eliminations;
		if (num_eliminations > 0) {
			continue;  // If this search did something, give previous techniques another chance
		}
	    // Swordfish (Like X-wing, but looking for same value in 2-3 spots in 3 rows/columns):
		//    https://www.learn-sudoku.com/swordfish.html
		dbprintf("    Trying a Swordfish Search...\n");
		num_eliminations = basic_fish_search(candidates, 3);
		board_stats.swordfish += num_eliminations;
		if (num_eliminations > 0) {
			continue;  // If this search did something, give previous techniques another chance
		}

		// More Naked/Hidden Sets (once that help less often...)
		// Quads
		dbprintf("    Trying a Naked/Hidden Quadruples Search...\n");
		naked_hidden_sets_search(candidates, 4, &num_naked, &num_hidden);
		board_stats.naked_quads += num_naked;
		board_stats.hidden_quads += num_hidden;
		if (num_naked + num_hidden > 0) {
			continue; // If naked / hidden quads did something, give previous techniques another chance
		}
		// Quints
		dbprintf("    Trying a Naked/Hidden Quintuples Search...\n");
		naked_hidden_sets_search(candidates, 5, &num_naked, &num_hidden);
		board_stats.naked_quints += num_naked;
		board_stats.hidden_quints += num_hidden;
		if (num_naked + num_hidden > 0) {
			continue; // If naked / hidden quints did something, give previous techniques another chance
		}


		// Jellyfish (Like X-Wing/Swordfish, but looking for same value in 2-4 spots in 4 rows/columns):
		//    http://nanpre.adg5.com/tec_en14.html
		dbprintf("    Trying a Jellyfish Search...\n");
		num_eliminations = basic_fish_search(candidates, 4);
		board_stats.jellyfish += num_eliminations;
		if (num_eliminations > 0) {
			continue;  // If this search did something, give previous techniques another chance
		}

//		display_candidates(board, candidates);
	    // TODO: More advanced techniques like XY Wing
		//   https://www.learn-sudoku.com/advanced-techniques.html
//		// #ifdef PRINT_DEBUG
//		printf("    Trying a X-Wing Search...\n");
//		// #endif
//		num_eliminations = x_wing_search(candidates);
//		board_stats.x_wing += num_eliminations;
//		if (num_eliminations > 0) {
//			display_candidates(board, candidates);
//			continue;  // If this search did something, give previous techniques another chance
//		}

        // Finally, try a Brute Force technique
		dbprintf("\nNo candidates could be eliminated with previous techniques.\n"
				"    Trying randomized solution for a cell...\n");
		// TODO: Make this more systematic than random (like step through candidates instead)?
		num_cells_found = randomized_value_board_search(board, candidates);
		board_stats.num_empty_cells -= num_cells_found;
		board_stats.random += num_cells_found;

		if (num_cells_found > 0) {
			used_random_values = true;
		}
		else {
			// If not even this worked, give up...
			printf("Not even random value gen could work. Giving up...\n");
			break;
		}

    }


    /* ****************************   Finish up  **************************** */
    printf("\nFinished.\n");

    if (board_stats.num_empty_cells == 0) {
		// Check to make sure that each row, col, and box only has 1 - 9 once
		if (exists_no_double_values(board) == true) {
			printf("\nThe solution is valid. Success!!\n");
		}
		else {
			printf("\nThe solution is not valid. Failure!!\n");
		    display_board(board);
			exit(EXIT_FAILURE);
		}
	}

    printf("\nFinal Solution for Puzzle \"%s\":\n", file_name);
    display_board(board);

    //  TODO:  Save board to a file based off of the file_name
    save_board_to_file(board, file_name);

	printf("\nTook %d rounds\n", round);
	if (num_fails == 0) {
		printf("  Completed with no retries \n");
	}
	else if (num_fails == 1) {
		printf("  Had 1 retry \n");
	}
	else {
		printf("  Had %d retries \n", num_fails);
	}

    printf("\nNumber of times the various algorithms were used: \n");
    printf("  Naked Singles:  %d\n", board_stats.naked_single);
    if (board_stats.hidden_single > 0) {
		printf("  Hidden Singles: %d\n", board_stats.hidden_single);
		difficulty = 1;
	}
    if (board_stats.locked_candidate > 0) {
        printf("  Locked Candidate:  %d\n", board_stats.locked_candidate);
        difficulty = 2;
    }
    if (board_stats.naked_pairs + board_stats.hidden_pairs > 0) {
		printf("  Naked Pairs: %d\n", board_stats.naked_pairs);
		printf("  Hidden Pairs: %d\n", board_stats.hidden_pairs);
        difficulty = 3;
    }
    if (board_stats.naked_triples + board_stats.hidden_triples > 0) {
		printf("  Naked Triples: %d\n", board_stats.naked_triples);
		printf("  Hidden Triples: %d\n", board_stats.hidden_triples);
        difficulty = 4;
    }
    if (board_stats.naked_quads + board_stats.hidden_quads > 0) {
		printf("  Naked Quadruples: %d\n", board_stats.naked_quads);
		printf("  Hidden Quadruples: %d\n", board_stats.hidden_quads);
        difficulty = 5;
    }
    if (board_stats.naked_quints + board_stats.hidden_quints > 0) {
		printf("  Naked Quintuples: %d\n", board_stats.naked_quints);
		printf("  Hidden Quintuples: %d\n", board_stats.hidden_quints);
        difficulty = 5;
    }
    if (board_stats.x_wing > 0) {
		printf("  X-Wing: %d\n", board_stats.x_wing);
        difficulty = 4;
    }
    if (board_stats.swordfish > 0) {
		printf("  Swordfish: %d\n", board_stats.swordfish);
        difficulty = 5;
    }
    if (board_stats.jellyfish > 0) {
		printf("  Jellyfish: %d\n", board_stats.jellyfish);
        difficulty = 5;
    }
    if (board_stats.random > 0) {
    	printf("  Random Choice:  %d\n", board_stats.random);
        difficulty = 6;
    }

    printf("\nDiffucluty of Board: ");
    switch (difficulty) {
		case 0 : printf ("Very Easy \n");
			break;
		case 1 : printf ("Easy \n");
			break;
		case 2 : printf ("Medium \n");
			break;
		case 3 : printf ("Hard \n");
			break;
		case 4 : printf ("Very Hard \n");
			break;
		case 5 : printf ("Extremely Hard \n");
			break;
		case 6 : printf ("Evil \n");
			break;
		default : printf ("Unknown!! \n");
    }


    if (board_stats.num_empty_cells > 0) {
        printf("\n\nFailed to complete the board!\n");
        exit(EXIT_FAILURE);
    }

	// If made it this far, then exit successfully!
    exit(EXIT_SUCCESS);
}

