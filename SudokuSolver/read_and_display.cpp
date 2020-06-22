/* This file has functions used to read in initial boards from a file and display the board state
*/

#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "board_globals.h"
#include "read_and_display.h"


// NOTE! These are now defined in board_globals.h
// #define FILENAME
// #define PATHNAME

#ifndef PATHNAME
#include <windows.h>
#endif

void print_line() {
    printf(" ");
    for (int i = 0; i < LEN; i++) {
        printf("----");
    }
    for (int i = 0; i < NUM-1; i++) {
        printf("-");
    }
    printf("\n");
}

void print_separators() {
    printf("| ");
    for (int i = 0; i < LEN; i++) {
        if (i % NUM == NUM-1) {
            printf("   | ");
        }
        else {
            printf("    ");
        }
    }
    printf("\n");
}

void display_board(int board[LEN][LEN]) {

    printf("\nBoard State: \n");
    print_line();  // Top box line

    for (int row = 0; row < LEN; row++) {
        printf("| "); // Print the left box line
        for (int col = 0; col < LEN; col++) {
            if (board[row][col] == 0) {
                printf(" - ");
            }
            else {
                printf("%2d ", board[row][col]);
            }
            // Pad with line (if section end) or spaces 
            if (col % NUM == NUM - 1) {
                printf("| ");
            }
            else {
                printf(" ");
            }
        }
        printf("\n"); // Start a new row

        // Add line (if section end) or spaces and separators
        if (row % NUM == NUM - 1) {
            print_line();  // Bottom section line
        }
        else {
            print_separators();
        }
    }
}

void display_candidates(int board[LEN][LEN], int candidates[LEN][LEN][LEN]) {
    printf("\nCandidate State: \n");
    printf(" -----------------------------------------------------------------------------------------\n");  // Top box line

    for (int row = 0; row < LEN; row++) {
        printf("| "); // Print the left box line
        for (int col = 0; col < LEN; col++) {
            if (board[row][col] == 0) {
                for (int ind = 0; ind < LEN; ind++) {
                    if (candidates[row][col][ind] == 1) {
                        printf("%d", ind + 1);
                    }
                    else {
                        printf("-");
                    }
                }
            }
            else {
                printf("    %d    ", board[row][col]);
            }
            // Pad with line (if section end) or spaces
            if (col % NUM == NUM - 1) {
                printf("|");
            }
            else {
                printf(" ");
            }
        }
        printf("\n"); // Start a new row
        if (row % NUM == NUM - 1) {
            printf(" -----------------------------------------------------------------------------------------\n");  // Bottom section line
        }
        else {
            printf("|                              |                             |                             |\n");  // If empty row, just print separators
        }
    }
}


void correct_path_name(char buff[]) {
    // Correct path if "Debug" is in the path_name. Replace with "SudokuSolver"
    int i;
    int j;
    char new_word[] = "SudokuSolver";
    char* loc;

    //    First, find if "Debug" starts // TODO: Do this with pointers instead??
    loc = strstr(buff, "Debug");
    if (loc != NULL) {
        // Find index at beginning of old word
        i = strlen(buff);
        while (i > 0 && buff[i] != '\\') {
            // printf("%c", buff[i]);
            i--;
        }
        i++;

        // Then replace text starting at that index
        for (j = 0; j < (int) sizeof(new_word); j++) {
            buff[i] = new_word[j];
            i++;
        }
        // Then place end character
        buff[i++] = '\0';
    }

}

int get_initial_values(int board[LEN][LEN], int candidates[LEN][LEN][LEN]) {
    /* This program populates the initial board values from a .txt file
    The .txt file should have all values in 9 rows and 9 columns
    Empty spaces should be specified by either "-" or "0" (spaces are ignored)
    It returns the number of empty spaces remaining
    */

    char item;
    char file_name[25];
    char dir_name[FILENAME_MAX - sizeof(file_name)];
    char path_name[FILENAME_MAX];
    int attempts = 0;
    int row = 0;
    int col = 0;
    int val;
    int val2;
    int num_empty_cells = 0;

    FILE* fp;

    // Get the current directory during runtime
    //    https://docs.microsoft.com/sv-se/windows/win32/api/winbase/nf-winbase-getcurrentdirectory?redirectedfrom=MSDN
    //    http://www.cplusplus.com/forum/windows/187372/ 

	#ifndef PATHNAME // If PATHNAME is not specified above, find automatically
    GetCurrentDirectoryA(FILENAME_MAX - sizeof(file_name), dir_name);
    // Correct path if "Debug" is in the path_name. Replace with "SudokuSolver"
    correct_path_name(dir_name);  // TODO: Figure this out later...
    printf("Corrected file dir: '%s'\n", dir_name);
	#else
    // Workaround...
    strcpy(dir_name, PATHNAME);
    printf("Current working dir: '%s'\n", dir_name;
	#endif // !PATHNAME

    do {
        #ifndef FILENAME // If filename is not specified above, ask the user
        printf("Enter the filename of the Sudoku puzzle to solve.\n");
        scanf("%s", file_name);
        #else
        strcpy(file_name, FILENAME);  // For development
        #endif // !FILENAME

        snprintf(path_name, sizeof(path_name), "%s\\puzzles\\%s", dir_name, file_name);  // build file path
        fp = fopen(path_name, "r");

        attempts++;
        if (fp == NULL) {
            printf("Error while opening the file '%s'\n ", path_name);

            if (attempts > 10) {
                printf("Too many attempts. Giving up!!\n");
                exit(EXIT_FAILURE);
            } 
            else {
                printf("Try again\n");
            }
        }

    } while (fp == NULL);

    // Read these in the values from the file 
    printf("\nReading in contents from: '%s'\n", path_name);
    printf("---------------------------------------------------------------\n");

    item = fgetc(fp);
    while (item != EOF) {
        printf("%c", item);

        // Check if commented line
        if (item == '/') {
            while (item != '\n' && item != EOF) {
                item = fgetc(fp);
                printf("%c", item);
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
                // If end of line found after LEN (9) columns, reset columns and incremnt the row
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

                // For boards with LEN > 9, check if next value is int (a double-digit number)
                if (LEN > 9) {
                    item = fgetc(fp);

                    val2 = item - '0';
                    if (val2 >= 0 && val2 < 9) {
                        printf("%c", item);
                        val = val * 10 + val2;
                    }
                    else { // If not, then move the fp back one char and move on...
                        fseek(fp, -1L, SEEK_CUR);
                    }
                }
            }
            // Check for errors
            if (row >= LEN || col >= LEN) {
                printf("\nBoard has too many rows or columns! rows: %d, columns: %d!\n", row + 1, col + 1);
                exit(EXIT_FAILURE);
            }
            if (val < 0 || val > LEN) {
                printf("\nInvalid value found: %d!\n", val);
                exit(EXIT_FAILURE);
            }

            board[row][col] = val;
            // printf("'%d'", val);
            col++;
        }

        item = fgetc(fp);
    }

    fclose(fp);
    printf("\n---------------------------------------------------------------\n\n");

    display_board(board);
	#ifndef FILENAME
    printf("\n Is this board correct? Press any key to continue.");
    scanf("%c", &item);
	#endif

    // Init counter
    for (int row = 0; row < LEN; row++) {
        for (int col = 0; col < LEN; col++) {
            if (board[row][col] == 0) { 
                num_empty_cells++; 
                // candidates[row][col] = round(pow(2, LEN)) - 1
                // memset(candidates[row][col], 1, sizeof(candidates[row][col])); -- didn't work...
                for (int i = 0; i < LEN; i++) {
                    candidates[row][col][i] = 1;
                }
            }
        }
    }
    return num_empty_cells;
}
