/* This file has functions used to read in initial boards from a file and display the board state
*/

#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <stdio.h>
#include <windows.h>
// #include "board_globals.h"
#include "read_and_display.h"

#define FILENAME "P1.txt" // For inital development of simple solver
// #define FILENAME "P2.txt" // Need more advanced techniques to solve
// #define FILENAME "P3.txt" // Need more advanced techniques to solve
// #define FILENAME "P4.txt" // Need more advanced techniques to solve


void display_board(int board[LEN][LEN]) {
    printf("\nBoard State: \n\n");
    printf(" -----------------------------------\n");  // Top box line

    for (int row = 0; row < LEN; row++) {
        printf("| "); // Print the left box line
        for (int col = 0; col < LEN; col++) {
            if (board[row][col] == 0) {
                printf("-");
            }
            else {
                printf("%d", board[row][col]);
            }
            // Pad with line (if section end) or spaces 
            if (col % NUM == NUM - 1) {
                printf(" | ");
            }
            else {
                printf("   ");
            }
        }
        printf("\n"); // Start a new row
        if (row % NUM == NUM - 1) {
            printf(" -----------------------------------\n");  // Bottom section line
        }
        else {
            printf("|           |           |           |\n");  // If empty row, just print seperators
        }
    }
}

void get_initial_values(int board[LEN][LEN]) {
    // TODO: Read these in from a file (or image??)
    char item;
    char file_name[10];
    char path_name[FILENAME_MAX];
    char buff[FILENAME_MAX];
    int attempts = 0;
    int row = 0;
    int col = 0;
    int val;

    FILE* fp;


    // Get the current directory during runtime
    //    https://docs.microsoft.com/sv-se/windows/win32/api/winbase/nf-winbase-getcurrentdirectory?redirectedfrom=MSDN
    //    http://www.cplusplus.com/forum/windows/187372/ 
    GetCurrentDirectoryA(FILENAME_MAX, buff);
    printf("Current working dir: %s\n", buff);

    do {
        printf("Enter the filename of the Sudoku puzzel to solve.\n");

        #ifndef FILENAME
        scanf("%s", file_name);
        #else
        strcpy(file_name, FILENAME);  // For development
        #endif // !FILENAME

        snprintf(path_name, sizeof(path_name), "%s\\puzzles\\%s", buff, file_name);
        fp = fopen(path_name, "r");

        attempts++;
        if (attempts > 10) {
            printf("Error while opening the file '%s'\n Too many attempts. Giving up!!\n", path_name);
            exit(EXIT_FAILURE);
        } else if (fp == NULL) {
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
            if (col == LEN) {
                // If end of line found after 9 columns, reset columns and incremnt the row
                col = 0;
                row++;
            }
            else if (col != 0 && col != LEN) {
                printf("\nReturn character found when not expected.... At col %d\n", col);
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
            if (row >= LEN || col >= LEN) {
                printf("\nBoard has too many rows or columns! rows: %d, columns: %d!\n", row + 1, col + 1);
                exit(EXIT_FAILURE);
            }
            if (val < 0 || val > 9) {
                printf("\nInvalid value found: %d!\n", val);
                exit(EXIT_FAILURE);
            }

            board[row][col] = val;
            col++;
        }

        item = fgetc(fp);
    }

    fclose(fp);
    printf("\n---------------------------------------------------------------\n\n");

    display_board(board);
    printf("\n Is this board correct? Press any key to continue.");
    scanf("%c", &item);
}
