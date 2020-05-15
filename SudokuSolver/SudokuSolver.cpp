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

int board[9][9];

// int all_values[9]


void get_initial_values() {
    // TODO: Read these in from a file (or image??)
    
    //    r  c    From: https://en.wikipedia.org/wiki/Sudoku 
    board[0][0] = 5;
    board[0][1] = 3;
    board[0][4] = 7;
    //board[][] = ;
    //board[][] = ;
    //board[][] = ;
    //board[][] = ;
    //board[][] = ;
    //board[][] = ;
    //board[][] = ;
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



int main()
{
    std::cout << "Hello World!\n";

    get_initial_values();

    for (int row=0; row < 9; row++) {

        for (int col = 0; col < 9; col++) {
            printf("%d ", board[row][col]);
        }

        printf("\n");
    }

}

