/* 
Code to implement various Sudoku solving techniques

References:
    Davis, Tom. �The Mathematics of Sudoku�. 21 Nov 2007. 15 Feb 2008
        http://www.geometer.org/mathcircles/sudoku.pdf.
*/

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <cstring>
#include "algorithms.h"
#include "read_and_display.h"

int sum_ints(int L, int x[]) {
    int sum = 0;
    for (int i = 0; i < L; i++) { 
        sum = sum + x[i]; 
    }
    return sum;
}

/*
void expand_candidate(int candidate_bin, int candidate_array[]) {
    int residual = candidate_bin;
    int i;

    // reset candidate to 0s
    memset(candidate_array, 0, sizeof(candidate_array));

    if (candidate_bin == 0) {
        return;
    }

    // Then populate based on binary value
    // values 1  2  3  4  5  6  7  8  9
    // array {1, 1, 1, 1, 1, 1, 1, 1, 1}
    // binary 111111111 = 511 (max value)
    // printf("Candidate values are: ");
    for (i = 0; i < LEN; i++) {
        candidate_array[i] = residual % 2;
        residual = round(residual / 2);
        // if (candidate_array[i] == 1) {
        //     printf("%d ", i + 1);
        // }
    }
    // printf("\n");
}

int collapse_candidate(int candidate_array[]) {
    int candidate_bin = 0;
    int i;

    for (i = 0; i < LEN; i++) {
        if (candidate_array[i] == 1) {
            candidate_bin = candidate_bin + round(pow(2, i));
        }
    }
    return candidate_bin;
}
*/

void display_candidates(int candidate_values[]) {
    for (int i = 0; i < LEN; i++) {
        if (candidate_values[i] > 0) {
            printf("%d, ", i + 1);
        }
    }
    printf("\n");
}

void naked_single_elim_candidate(int board[LEN][LEN], int r, int c, int candidate_values[LEN]) {
    // Eliminate candidate values using the naked singles method.
    int* value = &board[r][c];
    // printf("Row %d, Col %d, Value %d, Address %d\n", row, i, *value, value);
    if (*value != 0) {
        // If there is a value, remove that as a candidate
        candidate_values[*value - 1] = 0; // account for 0-based indexes...
    }
}


int check_search_result(int candidate_values[LEN]) {
    // If only one result, then returns the value of that result
    // If no possible results, then returns -1 (error)
    // Otherwise, returns 0 meaning there are still multiple possible results

    int num_values = sum_ints(LEN, candidate_values);

    if (num_values == 0) {
        // printf("number of found values is 9. No solution possible!!\n");
        return -1;
    }
    else if (num_values == 1) {
        for (int i = 0; i < LEN; i++)
        {
            // If only 1 possible value, the solution must that
            if (candidate_values[i] == 1) {
                // printf("Found a result of: %d ", i + 1);
                return i + 1;  // account for 0-based indexes...
            }
        }
    }
    return 0;
}

int naked_single_search(int board[LEN][LEN], int candidates[LEN][LEN][LEN]) {
    /* Naked Single: a cell for which there exists a unique candidate based on the circumstance
        that its groups contain all the other digits (Davis, 2007) */
    int row = 0;
    int col = 0;
    int r, c;
    int result;
    int num_cells_completed = 0;

    for (row = 0; row < LEN; row++) {
        for (col = 0; col < LEN; col++) {
            if (board[row][col] == 0) {
                // printf("\Naked Single search for results to: row %d, col %d\n", row+1, col+1);

                // 1st, look through the same row
                for (c = 0; c < LEN; c++) {
                	naked_single_elim_candidate(board, row, c, candidates[row][col]);
                }

                // 2nd, look through the same column
                for (r = 0; r < LEN; r++) {
                	naked_single_elim_candidate(board, r, col, candidates[row][col]);
                }

                // 3rd, check the 3x3 box it is in
                for (r = (row / NUM) * NUM; r < (row / NUM + 1) * NUM; r++) {
                    for (c = (col / NUM) * NUM; c < (col / NUM + 1) * NUM; c++) {
                    	naked_single_elim_candidate(board, r, c, candidates[row][col]);
                    }
                }

                // After this search, check if number of found values is 1 less than max (only one solution left)
                result = check_search_result(candidates[row][col]);

                if (result < 0) {
                    printf("\n\n No possible valid result for cell at row %d, column %d.\n   Cannot solve this puzzel!!!\n",
                        row + 1, col + 1);
                    return -1;
                }
                else if (result != 0) {
                    board[row][col] = result;
                    // candidates[row][col] = 0;
                    printf("\nFound Naked Single result for row %d, col %d: %d\n", row + 1, col + 1, board[row][col]);
                    num_cells_completed++;
                }
            }
        }
    }
    return num_cells_completed;
}


int hidden_single_group_search(int ind, int row_start, int row_end, int col_start, int col_end,
		int candidates[LEN][LEN][LEN]) {
    int c, r;
	int count = 0;

    for (r = row_start; r < row_end; r++) {
        for (c = col_start; c < col_end; c++) {
            if (candidates[r][c][ind] == 1) {
                count++;
            }
        }
    }
    // If only one cell has this value, then it must be the winner :)
    if (count == 1) {
    	return ind + 1; // Account for 0-based index...
    }
    return 0;
}

int hidden_single_search(int board[LEN][LEN], int candidates[LEN][LEN][LEN]) {
    /* Hidden Single: a cell for which there exists a unique candidate based on the constraint
     *  that no other cell in one of its groups can be that number (Davis, 2007) */
    int row = 0;
    int col = 0;
    int ind;

    int result;
    int num_cells_completed = 0;

    for (row = 0; row < LEN; row++) {
        for (col = 0; col < LEN; col++) {
            if (board[row][col] == 0) {  // If cell is not yet solved
                // Search the cell's groups (the row, column and box of the cell at location "row", "col")
                //    to see if there is a candidate value that only this cell has

               //  printf("Hidden Single search for results to: row %d, col %d\n", row+1, col+1);
                result = 0;
                
                // Loop through all candidate values, checking if they're unique in their row, col, or box
                for (ind = 0; ind < LEN; ind++) {
                    if (candidates[row][col][ind] == 1) {
                        // 1st, look through the same row
                        result = hidden_single_group_search(ind, row, row + 1, 0, LEN, candidates);
                        if (result > 0) {
                            break;
                        }

                        // 2nd, look through the same column
                        result = hidden_single_group_search(ind, 0, LEN, col, col+1, candidates);
                        if (result > 0) {
                            break;
                        }

                        // 3rd, check the 3x3 box it is in
                        result = hidden_single_group_search(ind,
                            (row / NUM) * NUM, (row / NUM + 1) * NUM, 
                            (col / NUM) * NUM, (col / NUM + 1) * NUM, candidates);
                        if (result > 0) {
                            break;
                        }
                    } // if ind is a candidate value
                } // ind

                // If successful, recored the result and exit 
                if (result != 0) {
                    board[row][col] = result;
                    num_cells_completed++;
                    // Remove candidates from this cell
                    memset(candidates[row][col], 0, sizeof(candidates[row][col]));

                    printf("\nFound Hidden Singles result for row %d, col %d: %d\n", row + 1, col + 1, board[row][col]);
                    return num_cells_completed;
                }
            } // if cell not solved
        } // col
    } // row
    return num_cells_completed;
}

// *******************************************************************************************
// The following methods are used to remove candidates (but do not directly determine board values)

int remove_candidate(int candidates[LEN][LEN][LEN], int ind,
		int row_start, int row_end, int col_start, int col_end) {
	int num_removed = 0;
	int r, c;

	for (r = row_start; r < row_end; r++) {
		for (c = col_start; c < col_end; c++) {
			if (candidates[r][c][ind] == 1) {
				candidates[r][c][ind] = 0;
				num_removed++;
				printf("Candidate value of %d was removed from cell at row %d, col %d", ind+1, r+1, c+1);
			}
		} // col
	} // row

	return num_removed;
}

int locked_candidate_search(int candidates[LEN][LEN][LEN]) {
	/*
	 * A method of elimination for which a number within a box is restricted to a specific
	 * 	  row or column and therefore can be excluded from the remaining cells in the
	 * 	  corresponding row or column outside of the selected box
	 * OR:
	 * A method of elimination for which a number within a row or column is restricted
	 * 	  to a specific box and therefore can be excluded from the remaining cells within
	 * 	  the box.  (Davis, 2007)
	 * see: http://www.math.kent.edu/~malexand/Latex/Examples/Article%20Example/YSU_Sudoku.pdf
	 * 	 Omission: https://www.learn-sudoku.com/omission.html
     *   http://www.angusj.com/sudoku/hints.php
   	 *   http://hodoku.sourceforge.net/en/tech_intersections.php
	 *
	 * This function uses the locked candidate technique to remove candidates from some cells
	 * 2 sub types:
	 * 	  Pointing: If in a block all candidates of a certain digit are confined to a row or column,
	 * 		that digit cannot appear outside of that block in that row or column.
	 * 	  Claiming: If in a row (or column) all candidates of a certain digit are confined to one block,
	 * 	    that candidate that be eliminated from all other cells in that block.
	 * return: how many times the locked candidate found values to remove
	 */

	int num_times_used = 0;
	int box;
	int row_start[LEN] = {0, 0, 0, 3, 3, 3, 6, 6, 6};
	int col_start[LEN] = {0, 3, 6, 0, 3, 6, 0, 3, 6};
	int found_in_group = 0;
	int num_groups = 0;  // TODO: Rename better
	int num_removed;
	int i;
	int r, c;

	// 1st, look through rows in each box (pointing)
	for (box = 0; r < LEN; box++) {
		for (i = 0; i < LEN; i++) {
			num_groups = 0; // reset the number of candidate rows...
			for (r = row_start[box]; r < row_start[box] + NUM; r++) {
				found_in_group = 0;  // reset if candidate was found in that row
				for (c = col_start[box]; c < col_start[box] + NUM; c++) {
					if (candidates[r][c][i] == 1) {
						// DO something here!
						if (found_in_group == 0) {
							num_groups++;
							found_in_group++;
						}
					}
				} // column
			} // row
			if (num_groups == 1) {
				// If the value was only found in 1 row
				// TODO: Eliminate other candidates in that row
				printf("Candidate value of %d was only found in row %d of box %d", i+1, r+1, box+1);
				printf("Eliminating other candidates of %d in row %d", i+1, r+1);
				// Eliminate in rows to the right
				if (col_start[box] < LEN-NUM) {
					num_removed = remove_candidate(candidates, i, r, r+1, 0, col_start[box]);
				}
				// Eliminate in rows to the left
				if (col_start[box] > 0) {
					num_removed = remove_candidate(candidates, i, r, r+1, col_start[box] + 3, LEN);
				}
				if (num_removed > 0) {
					num_times_used++;
				}

			}
		} // value
	} // box


	// 2nd, look through columns in each box (pointing)

	// 3rd, look through 3x3 boxes (claiming)


	return num_times_used;
}

// int naked_pairs_search(int candidates[LEN][LEN][LEN]) {
// 	/* Naked Pairs:
// 	 * This method of elimination pertains to the situation in which two numbers
// 	 *   are candidates in exactly two cells of a given group. Consequently, those two numbers
// 	 *   are eliminated as candidates in all other cells within the group (Davis, 2007).
// 	 * see: http://www.math.kent.edu/~malexand/Latex/Examples/Article%20Example/YSU_Sudoku.pdf
// 	 *   and https://www.learn-sudoku.com/naked-pairs.html
// 	 *
// 	 * This function uses the naked pairs technique to remove candidates from some cells
// 	 * return: how many times the naked pairs technique found values to remove
// 	 */
//
// }

int hidden_sets_search(int candidates[LEN][LEN][LEN], int set_size) {
	/*
	 * A Hidden Pair is basically just a �buried� Naked Pair. (so try this first?)
	 *   It occurs when two pencil marks appear in exactly two cells within the same house (row, column, or block).
	 *   However, the pair is not "Naked" - it is buried (or hidden) among other pencil marks
	 * source: https://www.learn-sudoku.com/hidden-pairs.html
	 *
	 * This function uses the hidden pairs technique to remove candidates from some cells
	 * set_size: the size of hidden sets to look for. 2 = hidden pairs, 3 = hidden triplets, etc.
	 * return: how many times the hidden sets technique found values to remove
	 *
	 */

	int num_times_used = 0;


	return num_times_used;
}
// *******************************************************************************************

// Finally, if all else fails, this technique will just pick a value from the cell with fewest candidates
int randomized_value_board_search(int board[LEN][LEN], int candidates[LEN][LEN][LEN]) {
    int row = 0;
    int col = 0;
    int num_missing = 0;
    int result = 0;
    int num_cells_completed = 0;

    // Values for the cell with fewest unknown values
    int row_min = -1;
    int col_min = -1;
    int num_missing_min = LEN;
    int candidate_values_min[LEN] = { 0 };
    int rand_val = 0;
    
    for (row = 0; row < LEN; row++) {
        for (col = 0; col < LEN; col++) {
            if (board[row][col] == 0) {  // TODO: check board values or candidate values?
                // If fewer values are missing in this cell than the current candidate, update values 
                num_missing = sum_ints(LEN, candidates[row][col]);
                if (num_missing < num_missing_min) {
                    num_missing_min = num_missing;
                    row_min = row;
                    col_min = col;
                    memcpy(candidate_values_min, candidates[row][col], sizeof(candidate_values_min));
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
            if (candidate_values_min[idx] == 1) {
                printf("%d, ", idx + 1);
            }
        }

        while (result == 0) {
            rand_val = rand() % LEN; // Then gen a random value and see if it's a valid value
            // TODO: Instead of a random number, use the first possible value
            //    Then increment based on the number of failures (if passed in as input)
            if (candidate_values_min[rand_val] == 1) {
                result = rand_val + 1;
            }
        }

        printf("\nSelected a result of %d\n\n", result);
        board[row_min][col_min] = result;
        // NOTE: don't clear the candidate values, since it's good to keep track of these for the future
        num_cells_completed++;
    }
    else {
        printf("\nNo cells could be found to evaluate for random values. Something went wrong!!!\n");
    }

    return num_cells_completed;
}
