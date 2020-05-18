// SudokuSolver.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

#include <iostream>

const int L = 9;
int board[L][L];


void get_initial_values() {
    // TODO: Read these in from a file (or image??)
    
    //    r  c    From: https://en.wikipedia.org/wiki/Sudoku 
    board[0][0] = 5;
    board[0][1] = 3;
    board[0][4] = 7;
    board[1][0] = 6;
    board[1][3] = 1;
    board[1][4] = 9;
    board[1][5] = 5;
    board[2][1] = 9;
    board[2][2] = 8;
    board[2][7] = 6;
    board[3][0] = 8;
    board[3][4] = 6;
    board[3][8] = 3;
    board[4][0] = 4;
    board[4][3] = 8;
    board[4][5] = 3;
    board[4][8] = 1;
    board[5][0] = 7;
    board[5][4] = 2;
    board[5][8] = 6;
    board[6][1] = 6;
    board[6][6] = 2;
    board[6][7] = 8;
    board[7][3] = 4;
    board[7][4] = 1;
    board[7][5] = 9;
    board[7][8] = 5;
    board[8][4] = 8;
    board[8][7] = 7;
    board[8][8] = 9;
    //board[][] = ;
    //board[][] = ;
    //board[][] = ;
    //board[][] = ;
    //board[][] = ;
    //board[][] = ;
    //board[][] = ;
    //board[][] = ;
    //board[][] = ;
}


void display_board() {
    std::cout << "Board State: \n\n";

    printf(" -----------------------------------\n");
    for (int row = 0; row < L; row++) {
        printf("| ");
        for (int col = 0; col < L; col++) {
            if (board[row][col] == 0) {
                printf("-");
            }
            else {
                printf("%d", board[row][col]);
            }
            // Pad with line (if section end) or spaces 
            if (col % 3 == 2) {
                printf(" | ");
            }
            else {
                printf("   ");
            }
        }
        printf("\n"); // Start a new row
        if (row % 3 == 2) {
            printf(" -----------------------------------\n");
        }
        else {
            printf("|           |           |           |\n");
        }
    }
}

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
    // Otherwise, returns 0 meaning there are still multiple results
    int num_values = sum_values(x);
    
    if (num_values == 0) {
        printf("number of possible values is 0. No solution possible!!\n");
        return -1;
    }
    else if (num_values == 1) {
        for (int i = 0; i < L; i++)
        {
            if (x[i] != 0) {
                return i+1;  // acount for 0-based indexes...
            }
        }
    }
     
    return 0;
}

int search(int row, int col) {
    // Searches the same row, column and box as the cell at location "row", "col"
    // If there is only one possible result, returns that value
    int r, c;
    int* value;
    int possible_values[L];
    int result = 0;

    for (r = 0; r < L; r++) {
        possible_values[r] = 1;
    }

    // first, look through the same row
    r = row;
    for (c = 0; c < L; c++) {
        value = &board[r][c];
        // printf("Row %d, Col %d, Value %d, Address %d\n", row, i, *value, value);
        if (*value != 0) {
            // printf("Row Search: Found value at row %d, column %d: %d\n", r + 1, c + 1, *value);
            possible_values[*value-1] = 0; // acount for 0-based indexes...
        }
    }
    // Check result here??

    // 2nd, look through the same column
    c = col;
    for (r = 0; r < L; r++) {
        value = &board[r][c];
        if (*value != 0) {
            // printf("Col Search: Found value at row %d, column %d: %d\n", r + 1, c + 1, *value);
            possible_values[*value-1] = 0; // acount for 0-based indexes...
        }
    }

    // 3rd, check the 3x3 box it is in
    int row_min = (row/3)*3;
    int row_max = (row/3 + 1)*3;
    int col_min = (col/3)*3;
    int col_max = (col/3 + 1)*3;

    for (r = row_min; r < row_max; r++) {
        for (c = col_min; c < col_max; c++) {
            value = &board[r][c];
            if (*value != 0) {
                // printf("Box Search: Found value at row %d, column %d: %d\n", r + 1, c + 1, *value);
                possible_values[*value - 1] = 0; // acount for 0-based indexes...
            }
        }
    }

    // Finally, Check if number of possible values is 1 (only one solution left)
    result = check_search_result(possible_values);
    // if (result != 0) {
    //     return result;
    // }

    return result;
}

int main()
{
    int cells_left = L * L;
    int cells_left_start = cells_left;
    int row = 0;
    int col = 0;
    int result = 0;
    int round = 0;
    
    // Setup board
    std::cout << "Starting!\n";
    get_initial_values();
    display_board();

    // Init counters
    for (row = 0; row < L; row++) {
        for (col = 0; col < L; col++) {
            if (board[row][col] != 0) {  cells_left--; }
        }
    }
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
                        printf("\n\n No possible results for cell at row %d, column %d.\n   Cannot solve this puzzel!!!\n");
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

    // TODO: Add some sort of checker to make sure that each row, col, and box only has 1-9 once

    // Solution
    if (cells_left == 0) {
        printf("\n\nFinished in %d rounds!\n", round);
    }
    else {
        printf("\n\nFAILED!\n");
        exit(EXIT_FAILURE);
    }
    
    // display_board();
}

