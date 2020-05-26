// SudokuSolver.cpp : This tool takes in a sudoku board and generates a solution.

#include <iostream>
#include "read_and_display.h"


// Global variables
const int N = 3;   // Minor board dimension
const int L = N*N; // Major board length
int board[L][L] = { 0 };  // Board state


int sum_values(int x[L]) {
    int sum = 0;
    for (int i = 0; i < L; i++)
    {
        sum = sum + x[i];
    }
    return sum;
}

int check_search_result(int x[L]) {
    // If only one result, then returns the value of that result
    // If no possible results, then returns -1 (error)
    // Otherwise, returns 0 meaning there are still multiple possible results

    int num_values = sum_values(x);
    
    if (num_values >= L) {
        // printf("number of found values is 9. No solution possible!!\n");
        return -1;
    }
    else if (num_values == L-1) {
        for (int i = 0; i < L; i++)
        {   
            // If only 1 unfound value, the solution must be the value that wasn't found
            if (x[i] == 0) {
                return i+1;  // acount for 0-based indexes...
            }
        }
    }
     
    return 0;
}

void eval_cell(int r, int c, int found_values[L]) {
    int* value = &board[r][c];
    // printf("Row %d, Col %d, Value %d, Address %d\n", row, i, *value, value);
    if (*value != 0) {
        // printf("Row Search: Found value at row %d, column %d: %d\n", r + 1, c + 1, *value);
        // printf("Col Search: Found value at row %d, column %d: %d\n", r + 1, c + 1, *value);
        // printf("Box Search: Found value at row %d, column %d: %d\n", r + 1, c + 1, *value);
        found_values[*value - 1] = 1; // acount for 0-based indexes...
    }
}

int search(int row, int col) {
    // Searches the same row, column and box as the cell at location "row", "col"
    // If there is only one possible result, returns that value

    int r, c;
    int found_values[L] = { 0 };
    int result = 0;

    // 1st, look through the same row
    for (c = 0; c < L; c++) {
        eval_cell(row, c, found_values);
    }

    // 2nd, look through the same column
    for (r = 0; r < L; r++) {
        eval_cell(r, col, found_values);
    }

    // TODO: Check result here??

    // 3rd, check the 3x3 box it is in
    for (r = (row/N)*N; r < (row/N + 1)*N; r++) {
        for (c = (col/N)*N; c < (col/N + 1)*N; c++) {
            eval_cell(r, c, found_values);
        }
    }

    // Finally, Check if number of found values is 1 less than max (only one solution left)
    result = check_search_result(found_values);

    return result;
}


void check_area(int row_start, int row_end, int col_start, int col_end, int double_coords[]) {
    int r, c;
    int found_values[L] = { 0 };
    int* value;

    for (r = row_start; r < row_end; r++) {
        for (c = col_start; c < col_end; c++) {
            value = &board[r][c];
            // printf("Checking value at row %d, col %d: %d\n", r + 1, c + 1, board[r][c]);

            if (*value > 0) {
                found_values[*value - 1]++;
            }

            if (found_values[*value - 1] > 1) {
                printf("\nDouble value of %d found at %d, %d!!!\n", *value, r + 1, c + 1);
                double_coords[0] = r + 1;
                double_coords[1] = c + 1;
                return;
            }
        }
    }
}

void check_for_double_values(int double_coords[]) {
    // Checks the board to make sure there is only one instance of each number in each row, col, and box
    // If there is an instance of multiple vaues, sets values to row and col where occurance found
    int r, c;
    int result = 0;

    printf("Checking the board for invalid values...\n");

    // 1st, check all rows
    printf("Checking row: ");
    for (r = 0; r < L; r++) {
        printf("%d, ", r + 1);
        check_area(r, r + 1, 0, L, double_coords);
        if (double_coords[0] + double_coords[1] > -1) { return; }
    }

    // 2nd, check all columns
    printf("\nChecking column: ");
    for (c = 0; c < L; c++) {
        printf("%d, ", c + 1);
        check_area(0, L, c, c + 1, double_coords);
        if (double_coords[0] + double_coords[1] > -1) { return; }
    }

    // 3rd, check all NxN boxes
    printf("\nChecking boxes: ");
    for (r = 0; r < L - 2; r = r + N) {
        for (c = 0; c < L - 2; c = c + N) {
            printf("%d-%dx%d-%d, ", r + 1, r + N, c + 1, c + N);
            check_area(r, r + N, c, c + N, double_coords);
            if (double_coords[0] + double_coords[1] > -1) { return; }
        }
    }
    printf("\n");
}


int main()
{
    // int success = 0;
    int cells_left = L * L;
    int cells_left_start = cells_left;
    int row = 0;
    int col = 0;
    int result = 0;
    int round = 0;
    
    // Setup board
    get_initial_values();

    // Check to make sure that each row, col, and box only has 1 - 9 once
    int double_values[2] = { -1, -1 };
    check_for_double_values(double_values);
    if (double_values[0] > -1) {
        printf("\nThe board has a double value at %d, %d! Cannot continue!!\n", double_values[0], double_values[1]);
        exit(EXIT_FAILURE);
    }
    
    // Init counters
    for (row = 0; row < L; row++) {
        for (col = 0; col < L; col++) {
            if (board[row][col] != 0) {  cells_left--; }
        }
    }

    std::cout << "\n\nStarting!\n";
    printf("Cells completed:   %d\n", 81-cells_left);
    printf("Cells to complete: %d\n\n", cells_left);

    // Main loop
    while (cells_left > 0) {
        round++;
        cells_left_start = cells_left;

        for (row = 0; row < L; row++) {
            for (col = 0; col < L; col++) {
                if (board[row][col] == 0) {
                    // printf("\nSearching for results to: row %d, col %d\n", row+1, col+1);
                    result = search(row, col);
                    if (result < 0) {
                        printf("\n\n No possible valid result for cell at row %d, column %d.\n   Cannot solve this puzzel!!!\n", 
                            row + 1, col + 1);
                        exit(EXIT_FAILURE);
                    }
                    else if (result != 0) {
                        board[row][col] = result;
                        printf("\nFound result for row %d, col %d: %d\n", row + 1, col + 1, board[row][col]);
                        cells_left--;
                    }
                }
            }
        }

        display_board();
        printf("Previous number of cells completed: %d\n", 81 - cells_left_start);
        printf("Round %d: number of cells completed: %d\n", round, 81 - cells_left);

        // cells_left = 0;

        if (cells_left_start == cells_left) {
            printf("No new cells could be solved in the last iteration. Giving up...");
            break; 

            // TODO: Try to randomize the cells with fewest opitons 
            //       to see if that gives a valid solution
        }
    }


    // Finish up
    if (cells_left == 0) {
        printf("\nFinished in %d rounds\n\n", round);

        // Check to make sure that each row, col, and box only has 1 - 9 once
        int double_values[2] = { -1, -1 };
        check_for_double_values(double_values);

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

