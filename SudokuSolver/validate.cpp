#include <iostream>
#include "validate.h"

void check_area(int board[LEN][LEN], int row_start, int row_end, int col_start, int col_end, int double_coords[]) {
    int r, c;
    int found_values[LEN] = { 0 };
    int* value;

    for (r = row_start; r < row_end; r++) {
        for (c = col_start; c < col_end; c++) {
            value = &board[r][c];
            // printf("Checking value at row %d, col %d: %d\n", r + 1, c + 1, board[r][c]);

            if (*value > 0) {
                found_values[*value - 1]++;
            }

            if (*value - 1 < 0) {
                return;
            }
            else if (found_values[*value - 1] > 1) {
                printf("\nDouble value of %d found at %d, %d!!!\n", *value, r + 1, c + 1);
                double_coords[0] = r + 1;
                double_coords[1] = c + 1;
                return;
            }
        }
    }
}

void check_for_double_values(int board[LEN][LEN], int double_coords[]) {
    // Checks the board to make sure there is only one instance of each number in each row, col, and box
    // If there is an instance of multiple vaues, sets values to row and col where occurance found
    int r, c;
    int result = 0;

    printf("Checking the board for invalid values...\n");

    // 1st, check all rows
    printf("Checking row: ");
    for (r = 0; r < LEN; r++) {
        printf("%d, ", r + 1);
        check_area(board, r, r + 1, 0, LEN, double_coords);
        if (double_coords[0] + double_coords[1] > -1) { return; }
    }

    // 2nd, check all columns
    printf("\nChecking column: ");
    for (c = 0; c < LEN; c++) {
        printf("%d, ", c + 1);
        check_area(board, 0, LEN, c, c + 1, double_coords);
        if (double_coords[0] + double_coords[1] > -1) { return; }
    }

    // 3rd, check all NxN boxes
    printf("\nChecking boxes: ");
    for (r = 0; r < LEN - 2; r = r + NUM) {
        for (c = 0; c < LEN - 2; c = c + NUM) {
            printf("%d-%dx%d-%d, ", r + 1, r + NUM, c + 1, c + NUM);
            check_area(board, r, r + NUM, c, c + NUM, double_coords);
            if (double_coords[0] + double_coords[1] > -1) { return; }
        }
    }
    printf("\n");
}