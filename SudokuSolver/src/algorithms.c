/* 
Code to implement various Sudoku solving techniques

References:
    Davis, Tom. “The Mathematics of Sudoku”. 21 Nov 2007. 15 Feb 2008
        http://www.geometer.org/mathcircles/sudoku.pdf.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "algorithms.h"
#include "log.h"

#define MAX_SET_SIZE 5
#define MAX_FISH_SIZE 4


short sum_inds(short L, short x[]) {
	short i;
	short sum = 0;

    for (i = 0; i < L; i++) {
        sum = sum + x[i]; 
    }
    return sum;
}

//void display_array_values(int len, int array_values[]) {
//    for (int i = 0; i < len; i++) {
//		printf("%d, ", array_values[i]);
//    }
//    printf("\n");
//}

void naked_single_elim_candidate(short board[LEN][LEN], int r, int c, short candidate_values[LEN]) {
    // Eliminate candidate values using the naked singles method.
	short* value = &board[r][c];
    // printf("Row %d, Col %d, Value %d, Address %d\n", row, i, *value, value);
    if (*value != 0) {
        // If there is a value, remove that as a candidate
        candidate_values[*value - 1] = 0; // account for 0-based indexes...
    }
}


int check_search_result(short candidate_values[LEN]) {
    // If only one result, then returns the value of that result
    // If no possible results, then returns -1 (error)
    // Otherwise, returns 0 meaning there are still multiple possible results

	short num_values = sum_inds(LEN, candidate_values);

    if (num_values == 0) {
        // printf("number of found values is 9. No solution possible!!\n");
        return -1;
    }
    else if (num_values == 1) {
        for (int i = 0; i < LEN; i++)
        {
            // If only 1 possible value, the solution must that
            if (candidate_values[i] == 1) {
                return i + 1;  // account for 0-based indexes...
            }
        }
    }
    return 0;
}

int naked_single_search(short board[LEN][LEN], short candidates[LEN][LEN][LEN]) {
    /* Naked Single: a cell for which there exists a unique candidate based on the circumstance
        that its groups contain all the other digits (Davis, 2007) */
	short row = 0;
	short col = 0;
	short r, c;

    int result;
    int num_cells_completed = 0;

    for (row = 0; row < LEN; row++) {
        for (col = 0; col < LEN; col++) {
            if (board[row][col] == 0) {

                // 1st, look through the same row
                for (c = 0; c < LEN; c++) {
                	naked_single_elim_candidate(board, row, c, candidates[row][col]);
                }
                // 2nd, look through the same column
                for (r = 0; r < LEN; r++) {
                	naked_single_elim_candidate(board, r, col, candidates[row][col]);
                }
                // 3rd, check the NUM*NUM box it is in
                for (r = (row / NUM) * NUM; r < (row / NUM + 1) * NUM; r++) {
                    for (c = (col / NUM) * NUM; c < (col / NUM + 1) * NUM; c++) {
                    	naked_single_elim_candidate(board, r, c, candidates[row][col]);
                    }
                }

                // After this search, check if number of found values is 1 less than max (only one solution left)
                result = check_search_result(candidates[row][col]);

                if (result < 0) {
                	dbprintf("\n\n No possible valid result for cell at row %d, column %d.\n   Cannot solve this puzzle!!!\n", row + 1, col + 1);
                    return -1;
                }
                else if (result != 0) {
                    board[row][col] = result;
                    dbprintf("\nFound Naked Single result for row %d, col %d: %d\n", row + 1, col + 1, board[row][col]);
                    num_cells_completed++;
                }
            }
        }
    }
    return num_cells_completed;
}


int hidden_single_group_search(short candidates[LEN][LEN][LEN], int ind,
		int row_start, int row_end, int col_start, int col_end) {
	short c, r;
	short count = 0;

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

int hidden_single_search(short board[LEN][LEN], short candidates[LEN][LEN][LEN]) {
    /* Hidden Single: a cell for which there exists a unique candidate based on the constraint
     *  that no other cell in one of its groups can be that number (Davis, 2007) */
	short row = 0;
	short col = 0;
	short ind;

    int result;
    int num_cells_completed = 0;

    for (row = 0; row < LEN; row++) {
        for (col = 0; col < LEN; col++) {
            if (board[row][col] == 0) {  // If cell is not yet solved
                // Search the cell's groups (the row, column and box of the cell at location "row", "col")
                //    to see if there is a candidate value that only this cell has
                result = 0;
                
                // Loop through all candidate values, checking if they're unique in their row, col, or box
                for (ind = 0; ind < LEN; ind++) {
                    if (candidates[row][col][ind] == 1) {
                        // 1st, look through the same row
                        result = hidden_single_group_search(candidates, ind, row, row + 1, 0, LEN);
                        if (result > 0) {
                            break;
                        }

                        // 2nd, look through the same column
                        result = hidden_single_group_search(candidates, ind, 0, LEN, col, col+1);
                        if (result > 0) {
                            break;
                        }

                        // 3rd, check the 3x3 box it is in
                        result = hidden_single_group_search(candidates, ind,
                            (row / NUM) * NUM, (row / NUM + 1) * NUM, 
                            (col / NUM) * NUM, (col / NUM + 1) * NUM);
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
                    dbprintf("\nFound Hidden Singles result for row %d, col %d: %d\n", row + 1, col + 1, board[row][col]);
                    return num_cells_completed;
                }
            } // if cell not solved
        } // col
    } // row
    return num_cells_completed;
}



// *******************************************************************************************
// The following methods are used to remove candidates (but do not directly determine board values)

int remove_candidate(short candidates[LEN][LEN][LEN], int ind,
		int row_start, int row_end, int col_start, int col_end) {
	int num_removed = 0;

	short r, c;

	for (r = row_start; r < row_end; r++) {
		for (c = col_start; c < col_end; c++) {
			if (candidates[r][c][ind] == 1) {
				candidates[r][c][ind] = 0;
				// printf("  Candidate value of %d was removed from cell at row %d, col %d\n", ind+1, r+1, c+1);
				num_removed++;
			}
		} // col
	} // row

	return num_removed;
}


int locked_candidate_search(short candidates[LEN][LEN][LEN]) {
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

	int num_occurances = 0;

	short num_groups_found = 0;
	short spot_found = 0;
	short num_removed;

	short i, n;
	short r, c;

	short box;
	short row_start[LEN] = { 0 };
	short col_start[LEN] = { 0 };

	for (n = 0; n < LEN; n++) {
		// Determine the row and column of the start of each box (with size of NUM * NUM)
		row_start[n] = (int) (n/NUM) * NUM;
		col_start[n] = (int) (n%NUM) * NUM;
	}

	for (i = 0; i < LEN; i++) {

		// 1st, look through rows and columns in each box (pointing)
		for (box = 0; box < LEN; box++) {

			// TODO: Make a generic function for each search type (if possible?)
			// A: look through rows
			num_groups_found = 0; // reset the number of rows the value was found
 			spot_found = -1;  // Reset where a locked candidate was found (0 is a possible value)
			for (r = row_start[box]; r < row_start[box] + NUM; r++) {
				if (num_groups_found > 1) { break; } // Quit searching if multiple results found
				for (c = col_start[box]; c < col_start[box] + NUM; c++) {
					if (candidates[r][c][i] == 1) {
						// If a candidate found when there already is one, stop looking...
						if (spot_found >= 0) {
							num_groups_found = 2;  // Mark that multiple results found
							break;
						}
						// If a new candidate found, then mark it and check the next group (row)
						spot_found = r;
						num_groups_found = 1;
						break;
					}
				} // column
			} // row
			if (num_groups_found == 1) {
				// If the value was only found in 1 row, eliminate other candidates in that row
				num_removed = 0;
				if (col_start[box] > 0) { // Eliminate in rows to the right
					num_removed = remove_candidate(candidates, i, spot_found, spot_found+1, 0, col_start[box]);
				}
				if (col_start[box]+NUM < LEN) { // Eliminate in rows to the left
					num_removed += remove_candidate(candidates, i, spot_found, spot_found+1, col_start[box]+NUM, LEN);
				}

				if (num_removed > 0) {
					dbprintf("Locked Candidate value of %d was only found in row %d of box %d.\n", i+1, spot_found+1, box+1);
					dbprintf("  This candidate value was removed from other boxes in row %d\n", spot_found+1);
					num_occurances++;
					return num_occurances;
				}
			}

			// B: look through columns
			num_groups_found = 0; // reset the number of columns the value was found
 			spot_found = -1;  // Reset where a locked candidate was found (0 is a possible value)
			for (c = col_start[box]; c < col_start[box] + NUM; c++) {
				if (num_groups_found > 1) { break; } // Quit searching if multiple results found
				for (r = row_start[box]; r < row_start[box] + NUM; r++) {
					if (candidates[r][c][i] == 1) {
						// If a candidate found when there already is one, stop looking...
						if (spot_found >= 0) {
							num_groups_found = 2;  // Mark that multiple results found
							break;
						}
						// If a new candidate found, then mark it and check the next group (col)
						spot_found = c;
						num_groups_found = 1;
						break;
					}
				} // column
			} // row
			if (num_groups_found == 1) {
				// If the value was only found in 1 column, eliminate other candidates in that column
				num_removed = 0;
				if (row_start[box] > 0) { // Eliminate in columns above
					num_removed = remove_candidate(candidates, i, 0, row_start[box], spot_found, spot_found+1);
				}
				if (row_start[box]+NUM < LEN) { // Eliminate in columns below
					num_removed += remove_candidate(candidates, i, row_start[box]+NUM, LEN, spot_found, spot_found+1);
				}

				if (num_removed > 0) {
					dbprintf("Locked Candidate value of %d was only found in column %d of box %d\n", i+1, spot_found+1, box+1);
					dbprintf("  This candidate value was removed from other boxes in column %d\n", spot_found+1);
					num_occurances++;
					return num_occurances;
				}
			}

		} // box


		// 2nd, look through boxes in each row / column (claiming)
		// A: look through rows
		for (r = 0; r < LEN; r++) {
			num_groups_found = 0; // reset the number of rows the value was found
			spot_found = -1;  // Reset where a locked candidate was found (0 is a possible value)
			// Look through boxes in that row
			for (n = 0; n < NUM; n++) {
				if (num_groups_found > 1) { break; } // Quit searching if multiple results found
				for (c = n*NUM; c < (n + 1)*NUM; c++) { // Look through columns in that box
					if (candidates[r][c][i] == 1) {
						// If a candidate found when there already is one, stop looking...
						if (spot_found >= 0) {
							num_groups_found = 2;  // Mark that multiple results found
							break;
						}
						// If a new candidate found, then mark it and check the next group (row)
						spot_found = n;
						num_groups_found = 1;
						break;
					}
				} // columns in box
			} // boxes in row
			if (num_groups_found == 1) {
				// If the value was only found in 1 box, eliminate candidate in other rows in that box
				box = ((int) r/NUM)*NUM + spot_found;
				num_removed = 0;
				if (row_start[box] < r) { // Eliminate in rows above
					num_removed = remove_candidate(candidates, i, row_start[box], r, col_start[box], col_start[box]+NUM);
				}
				if (row_start[box]+NUM > r) { // Eliminate in rows below
					num_removed += remove_candidate(candidates, i, r+1, row_start[box]+NUM, col_start[box], col_start[box]+NUM);
				}

				if (num_removed > 0) {
					dbprintf("Locked Candidate value of %d was only found in box %d of row %d\n", i+1, box+1, r+1);
					dbprintf("  This candidate value was removed from other rows in box %d\n", box+1);
					num_occurances++;
					return num_occurances;
				}
			}
		} // row

		// B: look through columns
		for (c = 0; c < LEN; c++) {
			num_groups_found = 0; // reset the number of rows the value was found
			spot_found = -1;  // Reset where a locked candidate was found (0 is a possible value)
			// Look through boxes in that column
			for (n = 0; n < NUM; n++) {
				if (num_groups_found > 1) { break; } // Quit searching if multiple results found
				for (r = n*NUM; r < (n + 1)*NUM; r++) { // Look through the rows in that box
					if (candidates[r][c][i] == 1) {
						// If a candidate found when there already is one, stop looking...
						if (spot_found >= 0) {
							num_groups_found = 2;  // Mark that multiple results found
							break;
						}
						// If a new candidate found, then mark it and check the next group (col)
						spot_found = n;
						num_groups_found = 1;
						break;
					}
				} // rows in the box
			} // boxes in this column
			if (num_groups_found == 1) {
				// If the value was only found in 1 box, eliminate candidate in other columns in that box
				box = ((int) c/NUM) + spot_found*NUM;
				num_removed = 0;
				if (col_start[box] < c) { // Eliminate in columns to the left
					num_removed = remove_candidate(candidates, i, row_start[box], row_start[box]+NUM, col_start[box], c);
				}
				if (row_start[box]+NUM > c) { // Eliminate in columns to the right
					num_removed += remove_candidate(candidates, i, row_start[box], row_start[box]+NUM, c+1, col_start[box]+NUM);
				}

				if (num_removed > 0) {
					dbprintf("Locked Candidate value of %d was only found in box %d of col %d\n", i+1, box+1, c+1);
					dbprintf("  This candidate value was removed from other columns in box %d\n", box+1);
					num_occurances++;
					return num_occurances;
				}
			}
		} // col

	} // value
	return num_occurances;
}


int process_naked_hidden_combos(short candidates[LEN][LEN][LEN], short set_size,
		short row_start, short row_end, short col_start, short col_end, short combo[]) {

	short sum;
	short r, c, i;
	short m, n;
	int cnt_naked = 0;
	int cnt_hidden = 0;

	bool skip;
	short cells_in_naked_set[2][LEN] = { 0 };
	short cells_in_hidden_set[2][LEN] = { 0 };

	int num_removed = 0;


	// Part A: See if there are sets that meet the naked or hidden criteria.
	// Loop through all cells in house to check if has the combo
	for (r = row_start; r < row_end; r++) {
		for (c = col_start; c < col_end; c++) {
			// Check if values in combo are candidates
			sum = 0;
			for (m = 0; m < set_size; m++) {
				if (candidates[r][c][combo[m]] == 1) {
					sum++;
				}
			}

			if (sum >= 1) {
				// Hidden: If it has any of the values, track as potential part of hidden set
				// if (is_hidden == 1) {
				cells_in_hidden_set[0][cnt_hidden] = r;
				cells_in_hidden_set[1][cnt_hidden] = c;
				// printf("Potential hidden pair: Row: %d, Col: %d\n", r + 1, c + 1);
				cnt_hidden++;
			}
			if (sum >= 2) {  // If only had a sum of 1 would be a naked single...
				// Naked: If only candidate values are in the set testing, it's a potential naked set
				if (sum_inds(LEN, candidates[r][c]) == sum) {
					cells_in_naked_set[0][cnt_naked] = r;
					cells_in_naked_set[1][cnt_naked] = c;
					// printf("Potential naked pair: Row: %d, Col: %d\n", r + 1, c + 1);
					cnt_naked++;
				}
			}

		} // col
	} // row


	// Part B: If there are these sets, remove other candidate values based on set type

	// 1st, Naked Sets (where set_size number of values are the only ones in set_size number of cells)
	//    Ex: naked pair: in a row, there are 2 cells with only 5 & 9. All other 5's and 9's can be eliminated in that row.
	//    http://hodoku.sourceforge.net/en/tech_naked.php
	if (cnt_naked == set_size) { // If set_size number of naked cells found
		// Then eliminate the values in the set from other cells in the same house.
		for (r = row_start; r < row_end; r++) {
			for (c = col_start; c < col_end; c++) {
				// Skip cells it the naked set
				skip = false;
				for (n = 0; n < cnt_naked; n++) {
					if (cells_in_naked_set[0][n] == r && cells_in_naked_set[1][n] == c) {
						skip = true;
						break;
					}
				}
				if (skip == true) { continue; }

				for (m = 0; m < set_size; m++) {
					i = combo[m];
					if (candidates[r][c][i] == 1) {
						candidates[r][c][i] = 0;
						dbprintf("  Candidate value of %d was removed from cell at row %d, col %d\n", i+1, r+1, c+1);
						num_removed++;
					}
				}
			} // col
		} // row

	} else if (cnt_naked > set_size) {
		dbprintf("!! %d candidates found for a naked set of size %d !!!\n", cnt_naked, set_size);
		return -1;
	}

	if (num_removed > 0) {
		#ifdef PRINT_DEBUG
		printf("Naked Set values were found: ");
		for (m = 0; m < set_size-1; m++) {
			printf("%d & ", combo[m] + 1);
		}
		printf("%d\n", combo[set_size-1] + 1);
		#endif
		return 1;
	}

	// 2nd, Hidden Sets (where set_size number of values only exist in set_size cells)
		//    Ex: hidden pair: in a col, there are only 2 cells with 4 & 8. All other candidates in those 2 cells can be removed.
		//    http://hodoku.sourceforge.net/en/tech_hidden.php
	if (cnt_hidden == set_size) { // If set_size number of hidden cells found

		// Then eliminate any other candidates values from these cells
		for (n = 0; n < cnt_hidden; n++) {
			for (i = 0; i < LEN; i++) {
				// Skip values in the current combo
				skip = false;
				for (m = 0; m < set_size; m++) {
					if (i == combo[m]) {
						skip = true;
						break;
					}
				}
				if (skip == true) { continue; }

				// Otherwise, remove the value
				r = cells_in_hidden_set[0][n];
				c = cells_in_hidden_set[1][n];
				if (candidates[r][c][i] == 1) {
					candidates[r][c][i] = 0;
					dbprintf("  Candidate value of %d was removed from cell at row %d, col %d\n", i+1, r+1, c+1);
					num_removed++;
				}
			} // col
		} // row
	}

	if (num_removed > 0) {
		#ifdef PRINT_DEBUG
		printf("Hidden Set values were found: ");
		for (m = 0; m < set_size-1; m++) {
			printf("%d & ", combo[m] + 1);
		}
		printf("%d\n", combo[set_size-1] + 1);
		#endif
		return 2;
	}

	return 0;
}

int naked_and_hidden_for_house(short candidates[LEN][LEN][LEN], int set_size,
		int row_start, int row_end, int col_start, int col_end) {
	/*
	 * Performs the naked set and hidden pairs search for the house defined by
	 * 		row_start, row_end, col_start, and col_end
	 *
	 * 	If the naked pairs search finds something, returns a 1.
	 * 	If the hidden pairs search finds something, returns a 2.
	 * 		Otherwise, returns 0.
	 */

	short i, r, c;
	short a, b, d, e;
	short num = 0, sum = 0;
	short values[LEN] = { 0 };
	short combo[MAX_SET_SIZE] = { 0 };

	int result;


	if (set_size > MAX_SET_SIZE) {
		printf("Invalid set size: %d!!", set_size);
		return -1;
	}

	if ((row_end - row_start) * (col_end - col_start) > LEN) {
		printf("Invalid house size!!");
		return -1;
	}

	// Initial step: Find values that are candidates in multiple cells, and put them in an array to look though next
	for (i = 0; i < LEN; i++) {
		sum = 0;
		for (r = row_start; r < row_end; r++) {
			for (c = col_start; c < col_end; c++) {
				if (candidates[r][c][i] == 1) {
					sum++;
				}
			} // col
		} // row
		if (sum >= 2) {
			values[num] = i;
			num++;
		}
	}
	// Quit if number of values is less than set_size
	if (num < set_size) {
		return 0;
	}

	// Otherwise, loop through all possible unique combination of values
	if (set_size == 2) {
		for (a = 0; a < num-1; a++) {
			for (b = a+1; b < num; b++) {
				combo[0] = values[a];
				combo[1] = values[b];
				result = process_naked_hidden_combos(candidates, set_size,
						row_start, row_end, col_start, col_end, combo);
				if (result > 0) {
					return result;
				}
			}
		}
	} else if (set_size == 3) {
		for (a = 0; a < num-2; a++) {
			for (b = a+1; b < num-1; b++) {
				for (c = b+1; c < num; c++) {
					combo[0] = values[a];
					combo[1] = values[b];
					combo[2] = values[c];
					result = process_naked_hidden_combos(candidates, set_size,
							row_start, row_end, col_start, col_end, combo);
					if (result > 0) {
						return result;
					}
				}
			}
		}
	} else if (set_size == 4) {
		for (a = 0; a < num-3; a++) {
			for (b = a+1; b < num-2; b++) {
				for (c = b+1; c < num-1; c++) {
					for (d = c+1; d < num; d++) {
						combo[0] = values[a];
						combo[1] = values[b];
						combo[2] = values[c];
						combo[3] = values[d];
						result = process_naked_hidden_combos(candidates, set_size,
								row_start, row_end, col_start, col_end, combo);
						if (result > 0) {
							return result;
						}
					}
				}
			}
		}
	} else if (set_size == 5) {
		for (a = 0; a < num-4; a++) {
			for (b = a+1; b < num-3; b++) {
				for (c = b+1; c < num-2; c++) {
					for (d = c+1; d < num-1; d++) {
						for (e = d+1; e < num; e++) {
							combo[0] = values[a];
							combo[1] = values[b];
							combo[2] = values[c];
							combo[3] = values[d];
							combo[4] = values[e];
							result = process_naked_hidden_combos(candidates, set_size,
									row_start, row_end, col_start, col_end, combo);
							if (result > 0) {
								return result;
							}
						}
					}
				}
			}
		}
	}

	return 0;
}

void naked_hidden_sets_search(short candidates[LEN][LEN][LEN], int set_size, int* num_naked, int* num_hidden) {
	/*
	 * Naked Pairs:
 	 *   This method of elimination pertains to the situation in which two numbers
 	 *   are candidates in exactly two cells of a given group. Consequently, those two numbers
 	 *   are eliminated as candidates in all other cells within the group (Davis, 2007).
 	 * see: http://www.math.kent.edu/~malexand/Latex/Examples/Article%20Example/YSU_Sudoku.pdf
 	 *   and https://www.learn-sudoku.com/naked-pairs.html
 	 *
	 * Hidden Pair:
	 *   It occurs when two pencil marks appear in exactly two cells within the same house (row, column, or block).
	 *   However, the pair is not "Naked" - it is buried (or hidden) among other pencil marks
	 * source: https://www.learn-sudoku.com/hidden-pairs.html
	 *
	 * This function uses the naked and hidden sets techniques to remove candidates from some cells
	 * set_size: the size of hidden sets to look for. 2 = pairs, 3 = triplets, etc.
	 * return: how many times the naked and hidden sets techniques found values to remove
	 *
	 */

	int result = 0;
	short house;  // the row, column, or box being tested
	short row_start, col_start;

	*num_naked = 0;
	*num_hidden = 0;

	// Check that set size is one this function can handle
	if (set_size < 2 || set_size > 5) {
		printf("** Invalid set_size: %d!! **\n", set_size);
		return;
	}

	// A: Search in rows
	for (house = 0; house < LEN; house++) {
		result = naked_and_hidden_for_house(candidates, set_size, house, house + 1, 0, LEN);
		if (result == 1) {
			*num_naked += 1;
			dbprintf("  These values were removed as candidates from other cells in row %d\n\n", house + 1);
		} else if (result == 2) {
			*num_hidden += 1;
			dbprintf("  All other candidates were removed from the cells with these values in row %d\n\n", house + 1);
		}

		 if (result > 0) {
		 	return;
		 }
	}

	// B: Search in columns
	for (house = 0; house < LEN; house++) {
		result = naked_and_hidden_for_house(candidates, set_size, 0, LEN, house, house + 1);
		if (result == 1) {
			*num_naked += 1;
			dbprintf("  These values were removed as candidates from other cells in column %d\n\n", house + 1);
		} else if (result == 2) {
			*num_hidden += 1;
			dbprintf("  All other candidates were removed from the cells with these values in column %d\n\n", house + 1);
		}

		 if (result > 0) {
		 	return;
		 }
	}

	// C: Search in boxes
	for (house = 0; house < LEN; house++) {
		row_start = (int) (house / NUM) * NUM;
		col_start = (int) (house % NUM) * NUM;

		result = naked_and_hidden_for_house(candidates, set_size, row_start, row_start+NUM, col_start, col_start+NUM);
		if (result == 1) {
			*num_naked += 1;
			dbprintf("  These values were removed as candidates from other cells in box %d\n\n", house + 1);
		} else if (result == 2) {
			*num_hidden += 1;
			dbprintf("  All other candidates were removed from the cells with these values in box %d\n\n", house + 1);
		}

		 if (result > 0) {
		 	return;
		 }
	}

	return;
}


int fish_search_for_combo(short candidates[LEN][LEN][LEN], int fish_size, short combo[]) {
	short i, m, n;
	short c, r;
	short vect = 0; // 0: rows, 1: columns
	enum {row, col};
#ifdef PRINT_DEBUG
	char vector[2][8] = {"rows", "columns"};
	char fish_type[3][10] = {"X-Wing", "Swordfish", "Jellyfish"};
#endif
	short num, sum;
	short spots[MAX_FISH_SIZE] = { 0 };
	bool potential_fish;
	bool skip;

	int num_removed = 0;
	int num_occurances = 0;

	// Loop through all numbers
	for (i = 0; i < LEN; i++ ) {
		// check both by all vectors (rows & columns)
		for (vect = 0; vect < 2; vect++) {
			num = 0;
			potential_fish = true;
			num_removed = 0;

			// check every column/row to see if it had candidates for spots
			for (m=0; m < LEN; m++) {
				sum = 0;
				for (n = 0; n < fish_size; n++) {
					if (vect == row) { // if checking rows
						sum += candidates[combo[n]][m][i];
					} else { // if checking columns
						sum += candidates[m][combo[n]][i];
					}
				}

				// if only only 1 row has this candidate, then cannot be a fish
				if (sum == 1) {
					potential_fish = false;
					break;
				}
				// if there are at least 2 candidates, add as potential spot for this combo
				else if (sum >= 2) {  // && sum <= fish_size) {
					if (num >= fish_size) {
						potential_fish = false;  // If too many columns with this combo, stop checking
						break;
					}
					spots[num] = m;
					num++;
				}
			}
			// If not a fish OR the number of spots not fish size, then move on
			if (potential_fish == false || num != fish_size) {
				continue;
			}

			// Make sure all vectors in combo have this value
			for (n = 0; n < fish_size; n++) {  // each vector (row/col)
				sum = 0;
				for (m = 0; m < fish_size; m++) {  // spots
					if (vect == row) { // if checking rows
						sum += candidates[combo[n]][spots[m]][i];
					} else {
						sum += candidates[spots[m]][combo[n]][i];
					}
				}
				if (sum < 2) {  // If a combo row is lacking this value, not a valid fish...
					potential_fish = false;
					break;
				}
			}
			if (potential_fish == false) {
				continue;
			}

			// If still a potential fish, eliminate candidates in other rows based on spots (if any)
			for (n = 0; n < LEN; n++) {  // vectors
				// skip row / column if in combo
				skip = false;
				for (m = 0; m < fish_size; m++) {
					if (n == combo[m]) {
						skip = true;
						break;
					}
				}
				if (skip == true) { continue; }

				// Otherwise, see if this value is a candidate at spots in the row
				for (m = 0; m < fish_size; m++) {
					if (vect == row) { // if checking by rows
						c = spots[m];
						if (candidates[n][c][i] == 1) {
							candidates[n][c][i] = 0;
							dbprintf("  Candidate value of %d was removed from cell at row %d, col %d\n", i+1, n+1, c+1);
							num_removed++;
						}
					} else { // if checking by columns
						r = spots[m];
						if (candidates[r][n][i] == 1) {
							candidates[r][n][i] = 0;
							dbprintf("  Candidate value of %d was removed from cell at row %d, col %d\n", i+1, r+1, n+1);
							num_removed++;
						}
					}
				} // spot
			} // vectors

			if (num_removed > 0) {
				#ifdef PRINT_DEBUG
				printf("%s found for value of %d in %s: ", fish_type[fish_size-2], i+1, vector[vect]);
				for (n = 0; n < fish_size-1; n++){
					printf("%d, ", combo[n] + 1);
				}
				printf("%d; %s: ", combo[fish_size-1] + 1, vector[-1*(vect-1)]);
				for (n = 0; n < fish_size-1; n++){
					printf("%d, ", spots[n] + 1);
				}
				printf("%d.\n", spots[fish_size-1] + 1);
				#endif
				num_occurances++;
				return num_occurances;
			} // if some removed

		} // vectors
	} // i

	return num_occurances;
}

int basic_fish_search(short candidates[LEN][LEN][LEN], int fish_size) {
	// fish_size:
	// 2: X-wings: Look for rectangles of matches:
	//    https://www.learn-sudoku.com/advanced-techniques.html
	// 3: Swordfish (Like X-wing, but looking for same value in 2-3 spots in 3 rows/columns):
	//    https://www.learn-sudoku.com/swordfish.html
	// 4: Jellyfish (Like Swordfish, but looking for same value in 2-4 spots in 4 rows/columns):
	//    http://nanpre.adg5.com/tec_en14.html

	// Note: this algorithm assumes searches with smaller sizes have already been tried...

	int num_occurances = 0;
	short w = 0, x = 0, y = 0, z = 0;
	short combo[MAX_FISH_SIZE] = { 0 };

	if (fish_size > MAX_FISH_SIZE) {
		printf("Fish size of %d is not supported!! Max size is: %d...", fish_size, MAX_FISH_SIZE);
	}

	// Loop through of all possible combinations of rows/columns to check if a fish
	if (fish_size == 2) {
		for (w = 0; w < LEN-1; w++) {
			for (x = w+1; x < LEN; x++) {
				combo[0] = w;
				combo[1] = x;
				num_occurances += fish_search_for_combo(candidates, fish_size, combo);
				if (num_occurances > 0) {
					return num_occurances;
				}
			}
		}
	}
	else if (fish_size == 3) {
		for (w = 0; w < LEN-2; w++) {
			for (x = w+1; x < LEN-1; x++) {
				for (y = x+1; y < LEN; y++) {
					combo[0] = w;
					combo[1] = x;
					combo[2] = y;
					num_occurances += fish_search_for_combo(candidates, fish_size, combo);
					if (num_occurances > 0) {
						return num_occurances;
					}
				}
			}
		}
	}
	else if (fish_size == 4) {
		for (w = 0; w < LEN-3; w++) {
			for (x = w+1; x < LEN-2; x++) {
				for (y = x+1; y < LEN-1; y++) {
					for (z = y+1; z < LEN; z++) {
						combo[0] = w;
						combo[1] = x;
						combo[2] = y;
						combo[3] = z;
						num_occurances += fish_search_for_combo(candidates, fish_size, combo);
						if (num_occurances > 0) {
							return num_occurances;
						}
					}
				}
			}
		}
	}

	return num_occurances;
}

// *******************************************************************************************


int randomized_value_board_search(short board[LEN][LEN], short candidates[LEN][LEN][LEN]) {
	// Finally, if all else fails, this technique will just pick a value from the cell with fewest candidates
	short row = 0;
	short col = 0;
    short num_missing = 0;

    int result = 0;
    int num_cells_completed = 0;

    // Values for the cell with fewest unknown values
    short row_min = -1;
    short col_min = -1;
    short num_missing_min = LEN;

    short candidate_values_min[LEN] = { 0 };
    int rand_val;
    
    for (row = 0; row < LEN; row++) {
        for (col = 0; col < LEN; col++) {
            if (board[row][col] == 0) {  // TODO: check board values or candidate values?
                // If fewer values are missing in this cell than the current candidate, update values 
                num_missing = sum_inds(LEN, candidates[row][col]);
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
		#ifdef PRINT_DEBUG
        printf("\nSelecting a random result for row %d, col %d.\n Possible values are: ", row_min + 1, col_min + 1);
        for (short idx = 0; idx < LEN; idx++) {
            if (candidate_values_min[idx] == 1) {
                printf("%d, ", idx + 1);
            }
        }
		#endif

        while (result == 0) {
            rand_val = rand() % LEN; // Then gen a random value and see if it's a valid value
            // TODO: Instead of a random number, use the first possible value
            //    Then increment based on the number of failures (if passed in as input)
            if (candidate_values_min[rand_val] == 1) {
                result = rand_val + 1;
            }
        }
        dbprintf("\nSelected a result of %d\n\n", result);
        board[row_min][col_min] = result;
        // NOTE: don't clear the candidate values, since it's good to keep track of these for the future
        num_cells_completed++;
    }
    else {
        printf("\nNo cells could be found to evaluate for random values. Something went wrong!!!\n");
    }

    return num_cells_completed;
}
