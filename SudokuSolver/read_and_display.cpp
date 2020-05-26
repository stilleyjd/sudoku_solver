/* This file has functions used to read in initial boards from a file and display the board state
*/

#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <stdio.h>
// #include "read_and_display.h"

/* Global variables
const int N
const int L = N * N;
int board[L][L]
*/


void display_board() {
    printf("\nBoard State: \n\n");
    printf(" -----------------------------------\n");  // Top box line

    for (int row = 0; row < L; row++) {
        printf("| "); // Print the left box line
        for (int col = 0; col < L; col++) {
            if (board[row][col] == 0) {
                printf("-");
            }
            else {
                printf("%d", board[row][col]);
            }
            // Pad with line (if section end) or spaces 
            if (col % N == N - 1) {
                printf(" | ");
            }
            else {
                printf("   ");
            }
        }
        printf("\n"); // Start a new row
        if (row % N == N - 1) {
            printf(" -----------------------------------\n");  // Bottom section line
        }
        else {
            printf("|           |           |           |\n");  // If empty row, just print seperators
        }
    }
}

void get_initial_values() {
    // TODO: Read these in from a file (or image??)
    char item;
    char file_name[10];
    char path_name[150];
    int row = 0;
    int col = 0;
    int val;

    FILE* fp;

    do {
        printf("Enter the filename of the Sudoku puzzel to solve.\n");


        // scanf("%s", file_name);
        strcpy(file_name, "P1.txt");  // For development
        // strcpy(file_name, "P2.txt");  // Need more advanced techniques to solve
        // strcpy(file_name, "P3.txt");  // Need more advanced techniques to solve
        // strcpy(file_name, "P4.txt");  // Need more advanced techniques to solve

        // TODO: Get the current directory during runtime instead of preconfiguring it
        //    https://docs.microsoft.com/sv-se/windows/win32/api/winbase/nf-winbase-getcurrentdirectory?redirectedfrom=MSDN

        snprintf(path_name, sizeof(path_name), "%s%s", "C:\\Users\\Jordan Stilley\\source\\repos\\stilleyjd\\sudoku_solver\\SudokuSolver\\puzzles\\", file_name);
        fp = fopen(path_name, "r");

        if (fp == NULL) {
            printf("Error while opening the file '%s'\n Try again\n", path_name);
        }
    } while (fp == NULL);

    printf("Reading in contnets from: '%s'\n", path_name);
    printf("---------------------------------------------------------------\n");

    item = fgetc(fp);
    while (item != EOF) {
        printf("%c", item);

        // Check if commentted line
        if (item == '/') {
            while (item != '\n' && item != EOF) {
                printf("%c", item);
                item = fgetc(fp);
            }
        }
        // Check if space 
        else if (item == ' ') {
            item = fgetc(fp);
            continue;
        }
        // Check if EOL "\n"
        else if (item == '\n') {
            if (col == L) {
                // If end of line found after 9 columns, reset columns and incremnt the row
                col = 0;
                row++;
            }
            else if (col != 0 && col != L) {
                printf("\nReturn character found when not expected.... At col %s\n", col);
                exit(EXIT_FAILURE);
            }
        }
        // Finally, handle actual board values
        else {
            if (item == '0' || item == '-') {
                // Check if empty value "-" or "0"
                val = 0;
            }
            else {
                // Otherwise, convert to int
                val = item - '0';
            }
            // Check for errors
            if (row >= L || col >= L) {
                printf("\nBoard has too many rows or columns! rows: %d, columns: %d!\n", row + 1, col + 1);
                exit(EXIT_FAILURE);
            }
            if (val < 0 || val > 9) {
                printf("\nInvalid value found: %s!\n", val);
                exit(EXIT_FAILURE);
            }

            board[row][col] = val;
            col++;
        }

        item = fgetc(fp);
    }

    fclose(fp);
    printf("\n---------------------------------------------------------------\n\n");

    display_board();
    printf("\n Is this board correct? Press any key to continue.");
    scanf("%c", &item);
}
