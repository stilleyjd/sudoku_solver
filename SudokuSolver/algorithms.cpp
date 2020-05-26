#include <iostream>
#include "algorithms.h"

int sum_values(int L, int x[]) {
    int sum = 0;
    for (int i = 0; i < L; i++) { sum = sum + x[i]; }
    return sum;
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

void eval_cell(int board[LEN][LEN], int r, int c, int found_values[LEN]) {
    int* value = &board[r][c];
    // printf("Row %d, Col %d, Value %d, Address %d\n", row, i, *value, value);
    if (*value != 0) {
        // printf("Row Search: Found value at row %d, column %d: %d\n", r + 1, c + 1, *value);
        // printf("Col Search: Found value at row %d, column %d: %d\n", r + 1, c + 1, *value);
        // printf("Box Search: Found value at row %d, column %d: %d\n", r + 1, c + 1, *value);
        found_values[*value - 1] = 1; // acount for 0-based indexes...
    }
}

int basic_search(int board[LEN][LEN], int row, int col) {
    // Searches the same row, column and box as the cell at location "row", "col"
    // If there is only one possible result, returns that value

    int r, c;
    int found_values[LEN] = { 0 };
    int result = 0;

    // 1st, look through the same row
    for (c = 0; c < LEN; c++) {
        eval_cell(board, row, c, found_values);
    }

    // 2nd, look through the same column
    for (r = 0; r < LEN; r++) {
        eval_cell(board, r, col, found_values);
    }

    // TODO: Check result here??

    // 3rd, check the 3x3 box it is in
    for (r = (row / NUM) * NUM; r < (row / NUM + 1) * NUM; r++) {
        for (c = (col / NUM) * NUM; c < (col / NUM + 1) * NUM; c++) {
            eval_cell(board, r, c, found_values);
        }
    }

    // Finally, Check if number of found values is 1 less than max (only one solution left)
    result = check_search_result(found_values);

    return result;
}
