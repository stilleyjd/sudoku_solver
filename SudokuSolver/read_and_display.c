/* This file has functions used to read in initial boards from a file and display the board state
*/


#include <stdio.h>
#include <string.h>
#include "board_globals.h"
#include "puzzles.h"
#include "read_and_display.h"

// #define _CRT_SECURE_NO_DEPRECATE  // For Visual Studio


// NOTE! These are now defined in puzzels.h
// #define FILENAME
// #define PATHNAME

#ifndef PATHNAME
#include <windows.h>
#endif

void print_line(int add_dashes) {
    printf(" ");
    for (int i = 0; i < LEN; i++) {
        printf("----");
        if (add_dashes > 0) {
        	for(int i = 0; i < add_dashes; i++) {
        		printf("-");
        	}
        }
    }
    for (int i = 0; i < NUM-1; i++) {
        printf("-");
    }
    printf("\n");
}

void print_separators(int add_spaces) {
    printf("| ");
    if (add_spaces > 0) {
    	for(int i = 0; i < add_spaces; i++) {
    		printf(" ");
    	}
    }
    for (int i = 0; i < LEN; i++) {
        if (i % NUM == NUM-1) {
            printf("   | ");
        }
        else {
            printf("    ");
        }

        if (add_spaces > 0) {
        	for(int i = 0; i < add_spaces; i++) {
        		printf(" ");
        	}
        }
    }
    printf("\n");
}

void display_board(int board[LEN][LEN]) {

    printf("\nBoard State: \n");
    print_line(0);  // Top box line

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
            print_line(0);  // Bottom section line
        }
        else {
            print_separators(0);
        }
    }
}

void print_spaces(int num) {
	if (num < 1) {
		return;
	}
	for (int i = 0; i < num; i++) {
		printf(" ");
	}
}

void display_candidates(int board[LEN][LEN], int candidates[LEN][LEN][LEN]) {
    int offset = (int) LEN/2;

    printf("\nCandidate State: \n");
    print_line(5);

    for (int row = 0; row < LEN; row++) {
		printf("|"); // Print the left box line
		for (int col = 0; col < LEN; col++) {
			// print candidates if cell not yet solved
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
			// Print solved values
			else {
				print_spaces(offset);
				printf("%d", board[row][col]);
				print_spaces(LEN - 1 - offset);
			}

			if (col % NUM == NUM - 1) { // Separate boxes with a line
				printf("|");
			}
			// else {
			// 	printf(" ");
			// }
		}
		printf("\n"); // Start a new line

		if (row % NUM == NUM - 1) {
			print_line(5);
		}
		else {
			print_separators(5);
		}
	}
}

void display_candidates_in_multiple_rows(int board[LEN][LEN], int candidates[LEN][LEN][LEN]) {
    int ind_start;
    int offset = (int) NUM/2;

    printf("\nCandidate State: \n");
    print_line(1);

    for (int row = 0; row < LEN; row++) {
        ind_start = 0;

    	// Each row gets NUM candidates to print candidates
        for (int cnt = 0; cnt < NUM; cnt++) {
        	if (cnt > 0) {
        		ind_start += NUM;
        	}

            printf("| "); // Print the left box line
			for (int col = 0; col < LEN; col++) {
				// print candidates if cell not yet solved
				if (board[row][col] == 0) {
					for (int ind = ind_start; ind < ind_start+NUM; ind++) {
						if (candidates[row][col][ind] == 1) {
							printf("%d", ind + 1);
						}
						else {
							printf("-");
						}
					}
				}
				// Print known values
				else if (cnt == offset) {
					print_spaces(offset);
					printf("%d", board[row][col]);
					print_spaces(NUM - 1 - offset);
				}
				else {
					print_spaces(NUM);
				}

				// Separate columns with line (if section end) or spaces
				if (col % NUM == NUM - 1) {
					printf(" | ");
				}
				else {
					printf(" ");
				}
			}

	        printf("\n"); // Start a new line
        }

        if (row % NUM == NUM - 1) {
            print_line(1);
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

FILE* build_and_check_path_name(char dir_name[], char file_name[], char path_name[]) {
	FILE* fpt = NULL;

    snprintf(path_name, FILENAME_MAX, "%s\\puzzles\\%s.txt", dir_name, file_name);  // build file path
    fpt = fopen(path_name, "r");

	if (fpt == NULL) {
		printf("Error while opening the file '%s'\n ", path_name);
	}
	return fpt;
}

int get_initial_values(int board[LEN][LEN], int candidates[LEN][LEN][LEN], char file_name[MAX_FILENAME_SIZE]) {
    /* This program populates the initial board values from a .txt file
    The .txt file should have all values in 9 rows and 9 columns
    Empty spaces should be specified by either "-" or "0" (spaces are ignored)
    It returns the number of empty spaces remaining
    */

    char item;
    char input[256];
    // char file_name[25];
    char dir_name[FILENAME_MAX - MAX_FILENAME_SIZE];
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

    // Figure out the dir_name...
	#ifndef PATHNAME // If PATHNAME is not specified above, find automatically
    GetCurrentDirectoryA(FILENAME_MAX - MAX_FILENAME_SIZE, dir_name);
    // Correct path if "Debug" is in the path_name. Replace with "SudokuSolver"
    correct_path_name(dir_name);  // TODO: Figure this out later...
    printf("Corrected file directory: '%s'\n", dir_name);
	#else
    // Workaround...
    strcpy(dir_name, PATHNAME);
    printf("Current working directory: '%s'\n", dir_name;
	#endif // !PATHNAME

    // Then, get the file_name and build a path_name
	#ifdef FILENAME
    strcpy(file_name, FILENAME);  // For development, puzzle name is defined in puzzle.h
	#endif
    // build path_name and check if filename passed in or from puzzle.h is valid
    fp = build_and_check_path_name(dir_name, file_name, path_name);

    while (fp == NULL) {
        printf("Enter the filename of the Sudoku puzzle to solve (without '.txt'): \n");
        fgets(input, 256, stdin);
        sscanf(input, "%s", file_name);

        fp = build_and_check_path_name(dir_name, file_name, path_name);
        if (fp == NULL) {
        	attempts++;
            if (attempts > 10) {
                printf("Too many attempts. Giving up!!\n");
                exit(EXIT_FAILURE);
            } 

            printf("Try again\n");
        }
    }

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
                if (item != EOF) {
                	printf("%c", item);
                }
            }
        }
        // Check if space or divider
        else if (item == ' ' || item == '|') {
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
            if (item == '0' || item == '-' || item == '.' ) {
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
                for (int i = 0; i < LEN; i++) {
                    candidates[row][col][i] = 1;
                }
            }
        }
    }
    return num_empty_cells;
}
