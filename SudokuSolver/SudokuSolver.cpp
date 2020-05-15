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
    // Otherwise, returns 0 meaning no single result
    if (sum_values(x) == 1) {
        for (int i = 0; i < L; i++)
        {
            if (x[i] != 0) {
                // printf("found one and only one result: %d", i);
                return i+1;  // acount for 0-based indexes...
            }
        }
    }
    // else {
    //     printf("found this many results: %d\n", l);
    // }
    return 0;
}

int search(int row, int col) {
    // Searches the same row, column and box as the cell at location "row", "col"
    // If there is only one possible result, returns that value
    int i;
    int* value;
    int possible_values[L] = { 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    int result = 0;

    // first, look through the same row
    // printf("\nSearching row %d\n", row + 1);
    for (i = 0; i < L; i++) {
        value = &board[row][i];
        // printf("Row %d, Col %d, Value %d, Address %d\n", row, i, *value, value);
        if (*value != 0) {
            possible_values[*value-1] = 0; // acount for 0-based indexes...
        }
    }
    // Check result here??

    // 2nd, look through the same column
    // printf("\nSearching column %d\n", col + 1);
    for (i = 0; i < L; i++) {
        value = &board[i][col];
        if (*value != 0) {
            possible_values[*value-1] = 0; // acount for 0-based indexes...
        }
    }

    // Finally, check the 3x3 box it is in


    // Check if number of possible values is 1 (only one solution left)
    result = check_search_result(possible_values);
    if (result != 0) {
        return result;
    }

    return result;
}

int main()
{
    int cells_left = L * L;
    int cells_left_start = cells_left;
    int row = 0;
    int col = 0;
    
    // Setup board
    std::cout << "Starting!\n";
    get_initial_values();
    display_board();

    // Init counter
    for (row = 0; row < L; row++) {
        for (col = 0; col < L; col++) {
            if (board[row][col] != 0) {  cells_left--; }
        }
    }
    printf("Cells completed:   %d\n", 81-cells_left);
    printf("Cells to complete: %d\n\n", cells_left);

    // Main loop
    while (cells_left > 0) {
        cells_left_start = cells_left;
        // printf("\nTrying with %d cells left\n", cells_left);
        for (row = 0; row < L; row++) {
            for (col = 0; col < L; col++) {
                if (board[row][col] == 0) {
                    // printf("\nSearching for results to: row %d, col %d\n", row+1, col+1);
                    // cells_left--;
                    board[row][col] = search(row, col);
                    if (board[row][col] != 0) {
                        printf("\nFound result for row %d, col %d: %d\n", row + 1, col + 1, board[row][col]);
                        cells_left--;
                    }
                }
            }
        }

        display_board();
        printf("Cells completed:   %d\n", 81 - cells_left);

        if (cells_left_start == cells_left) {
            printf("No new cells could be solved in the last iteration. Giving up...");
            break; 

            // TODO: Try to randomize the cells with fewest opitons 
            //       to see if that gives a valid solution
        }
    }

    // Solution
    if (cells_left == 0) {
        std::cout << "\n\nFinished!\n";
    }
    else {
        std::cout << "\n\nFAILED!\n";
        exit(EXIT_FAILURE);
    }
    
    // display_board();
}

