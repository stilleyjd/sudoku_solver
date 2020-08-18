#include <stdio.h>
#include "validate.h"
#include "log.h"

int check_area(short board[LEN][LEN], int row_start, int row_end, int col_start, int col_end) {
    int r, c, i;
    int found_values[LEN] = { 0 };
    for (r = row_start; r < row_end; r++) {
        for (c = col_start; c < col_end; c++) {
            i = board[r][c] - 1;

            if (i < 0) {
				continue;  // Move on if no value at this spot
			}
            else {
                found_values[i]++;
            }

            if (found_values[i] > 1) {
                printf("\n\nBoard is Invalid!\n"
                		"  Double value of %d found at row %d, col %d !!!\n", i + 1, r + 1, c + 1);
                return false;
            }
        }
    }

    return true;
}

int exists_no_double_values(short board[LEN][LEN]) {
    // Checks the board to make sure there is only one instance of each number in each row, col, and box
    // If there is an instance of multiple values, sets values to row and col where occurrence found
    int r, c, b;
    bool result = true;

    printf("Checking the board for invalid values...\n");

    // 1st, check all rows
    dbprintf("Checking row: ");
    for (r = 0; r < LEN; r++) {
        dbprintf("%d, ", r + 1);
        result = check_area(board, r, r + 1, 0, LEN);
        if (result == false) {
        	return result;
        }
    }

    // 2nd, check all columns
    dbprintf("\nChecking column: ");
    for (c = 0; c < LEN; c++) {
        dbprintf("%d, ", c + 1);
		result = check_area(board, 0, LEN, c, c + 1);
		if (result == false) {
			return result;
		}
    }

    // 3rd, check all NxN boxes
    dbprintf("\nChecking box: ");
    b = 0;
    for (r = 0; r < LEN - 2; r = r + NUM) {
        for (c = 0; c < LEN - 2; c = c + NUM) {
        	b++;
            dbprintf("%d, ", b);
    		result = check_area(board, r, r + NUM, c, c + NUM);
    		if (result == false) {
    			return result;
    		}
        }
    }
    dbprintf("\n");

    return result;
}
