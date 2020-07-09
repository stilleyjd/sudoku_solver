/* This file has functions used to read in initial boards from a file and display the board state
*/


#include <stdio.h>
#include <string.h>
#include "board_globals.h"
#include "puzzles.h"
#include "read_and_display.h"
#include "log.h"

// #define _CRT_SECURE_NO_DEPRECATE  // For Visual Studio


// NOTE! These are now defined in puzzels.h
// #define FILENAME
// #define PATHNAME

#ifndef PATHNAME
#include <windows.h>
#endif

void gen_line(char char_array[], int add_dashes) {
	char_array[0] = '\0';   // ensures the memory is an empty string

	strcat(char_array, " ");

    for (int i = 0; i < LEN; i++) {
    	strcat(char_array, "----");
        if (add_dashes > 0) {
        	for(int i = 0; i < add_dashes; i++) {
        		// printf("-");
            	strcat(char_array, "-");
        	}
        }
    }
    for (int i = 0; i < NUM-1; i++) {
    	strcat(char_array, "-");
    }

    strcat(char_array, "\n");
    return;
}

void gen_separators(char char_array[], int add_spaces) {
	char_array[0] = '\0';   // ensures the memory is an empty string

    strcat(char_array, "| ");
    if (add_spaces > 0) {
    	for(int i = 0; i < add_spaces; i++) {
    		strcat(char_array, " ");
    	}
    }
    for (int i = 0; i < LEN; i++) {
        if (i % NUM == NUM-1) {
            strcat(char_array, "   | ");
        }
        else {
            strcat(char_array, "    ");
        }

        if (add_spaces > 0) {
        	for(int i = 0; i < add_spaces; i++) {
        		strcat(char_array, " ");
        	}
        }
    }

    strcat(char_array, "\n");
    return;
}

void gen_row_string(short board[LEN][LEN], int row, char char_array[]) {

	char buffer[5];
	int col;

	char_array[0] = '\0';   // ensures the memory is an empty string

    strcat(char_array, "| ");  // Print the left box line

    for (col = 0; col < LEN; col++) {

        if (board[row][col] == 0) {
            strcat(char_array, " - ");
        }
        else {
        	buffer[0] = '\0';   // ensures the memory is an empty string
            sprintf(buffer, "%2d ", board[row][col]);
            strcat(char_array, buffer);
        }
        // Pad with line (if end of box) or spaces
        if (col % NUM == NUM - 1) {
            strcat(char_array, "| ");
        }
        else {
            strcat(char_array, " ");
        }
    }

    strcat(char_array, "\n");
    return;
}


void display_board(short board[LEN][LEN]) {
    char char_array[100];
	char_array[0] = '\0';   // ensures the memory is an empty string
	int row;

    // printf("\nBoard State: \n");
	gen_line(char_array, 0);
    printf("%s", char_array);

    for (row = 0; row < LEN; row++) {
    	gen_row_string(board, row, char_array);
        printf("%s", char_array);

        // Add box separators
        if (row % NUM == NUM - 1) {
        	gen_line(char_array, 0);  // Box bottom line
            printf("%s", char_array);
        }
        else {
        	gen_separators(char_array, 0);  // Box side lines
            printf("%s", char_array);
        }
    }
    printf("\n");
}

void print_spaces(int num) {
	if (num < 1) {
		return;
	}
	for (int i = 0; i < num; i++) {
		printf(" ");
	}
}

void display_candidates(short board[LEN][LEN], short candidates[LEN][LEN][LEN]) {
    int offset = (int) LEN/2;
    char char_array[100];

    printf("\nCandidate State: \n");
	gen_line(char_array, 5);
    printf("%s", char_array);

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
			gen_line(char_array, 5);
		    printf("%s", char_array);
		}
		else {
//			print_separators(5);
        	gen_separators(char_array, 5);
            printf("%s", char_array);
		}
	}
}

//void display_candidates_in_multiple_rows(int board[LEN][LEN], int candidates[LEN][LEN][LEN]) {
//    int ind_start;
//    int offset = (int) NUM/2;
//
//    printf("\nCandidate State: \n");
//    print_line(1);
//	gen_line(char_array, 1);
//    printf("%s\n", char_array);
//
//    for (int row = 0; row < LEN; row++) {
//        ind_start = 0;
//
//    	// Each row gets NUM candidates to print candidates
//        for (int cnt = 0; cnt < NUM; cnt++) {
//        	if (cnt > 0) {
//        		ind_start += NUM;
//        	}
//
//            printf("| "); // Print the left box line
//			for (int col = 0; col < LEN; col++) {
//				// print candidates if cell not yet solved
//				if (board[row][col] == 0) {
//					for (int ind = ind_start; ind < ind_start+NUM; ind++) {
//						if (candidates[row][col][ind] == 1) {
//							printf("%d", ind + 1);
//						}
//						else {
//							printf("-");
//						}
//					}
//				}
//				// Print known values
//				else if (cnt == offset) {
//					print_spaces(offset);
//					printf("%d", board[row][col]);
//					print_spaces(NUM - 1 - offset);
//				}
//				else {
//					print_spaces(NUM);
//				}
//
//				// Separate columns with line (if section end) or spaces
//				if (col % NUM == NUM - 1) {
//					printf(" | ");
//				}
//				else {
//					printf(" ");
//				}
//			}
//
//	        printf("\n"); // Start a new line
//        }
//
//        if (row % NUM == NUM - 1) {
//            print_line(1);
//        }
//    }
//}


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

void get_current_working_dir(char dir_name[FILENAME_MAX - MAX_FILENAME_SIZE]) {
    // Get the current directory during runtime
    //    https://docs.microsoft.com/sv-se/windows/win32/api/winbase/nf-winbase-getcurrentdirectory?redirectedfrom=MSDN
    //    http://www.cplusplus.com/forum/windows/187372/

    // Figure out the dir_name...
	#ifndef PATHNAME // If PATHNAME is not specified above, find automatically
	GetCurrentDirectoryA(FILENAME_MAX - MAX_FILENAME_SIZE, dir_name);
    // Correct path if "Debug" is in the path_name. Replace with "SudokuSolver"
    correct_path_name(dir_name);  // TODO: Figure this out later...
    dbprintf("Corrected file directory: '%s'\n", dir_name);

    #else
    // Workaround...
    strcpy(dir_name, PATHNAME);
    dbprintf("Current working directory: '%s'\n", dir_name;

	#endif

}

FILE* build_and_check_path_name(char dir_name[], char file_name[], char path_name[]) {
	FILE* fpt = NULL;

    snprintf(path_name, FILENAME_MAX, "%s\\puzzles\\%s.txt", dir_name, file_name);  // build file path
    fpt = fopen(path_name, "r");

	if (fpt == NULL) {
		printf("Error while opening the file \"%s\" \n ", path_name);
	}
	return fpt;
}

int get_initial_values(short board[LEN][LEN], short candidates[LEN][LEN][LEN], char file_name[MAX_FILENAME_SIZE]) {
    /* This program populates the initial board values from a .txt file
    The .txt file should have all values in 9 rows and 9 columns
    Empty spaces should be specified by either "-" or "0" (spaces are ignored)
    It returns the number of empty spaces remaining
    */

    char item;
    char input[256];
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
    get_current_working_dir(dir_name);

    // For development, puzzle name is defined in puzzle.h
	#ifdef FILENAME
    strcpy(file_name, FILENAME);
	#endif

    // build path_name and check if filename (passed in or from puzzle.h) is valid
    fp = build_and_check_path_name(dir_name, file_name, path_name);

    // If the filename is not valid, have user try to enter again
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

    // Then, read in the values from the file
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

    // Init empty cell counter and candidate board
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


int save_board_to_file(short board[LEN][LEN], char file_name[MAX_FILENAME_SIZE]) {

	char dir_name[FILENAME_MAX - MAX_FILENAME_SIZE];
	char path_name[FILENAME_MAX];
    char char_array[100];
	char_array[0] = '\0';   // ensures the memory is an empty string
	int row;

	FILE* fp;

	// build the path_name and open file handler for writing
	get_current_working_dir(dir_name);
    snprintf(path_name, FILENAME_MAX, "%s\\puzzle_solutions\\%s-solution.txt", dir_name, file_name);
    fp = fopen(path_name, "w");

    if (fp == NULL) {
    	printf("Error while opening the file \"%s\" \n ", path_name);
    	return false;
    }

    // printf("\nBoard State: \n");
	gen_line(char_array, 0);
	fputs(char_array, fp);

    for (row = 0; row < LEN; row++) {
    	gen_row_string(board, row, char_array);
//        printf("%s", char_array);
        fputs(char_array, fp);

        // Add box separators
        if (row % NUM == NUM - 1) {
        	gen_line(char_array, 0);  // Box bottom line
//            printf("%s", char_array);
            fputs(char_array, fp);
        }
        else {
        	gen_separators(char_array, 0);  // Box side lines
//            printf("%s", char_array);
            fputs(char_array, fp);
        }
    }
//    printf("\n");
    fputs("\n", fp);
    fclose(fp);

    printf("Saved puzzle results to:\n  \"%s\" \n ", path_name);
	return true;
}
