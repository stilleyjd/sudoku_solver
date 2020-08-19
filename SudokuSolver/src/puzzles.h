// #pragma once


// #define PATHNAME "C:\\Users\\Jordan\\git\\sudoku_solver\\SudokuSolver" // Work PC: ELT-0002
// #define PATHNAME "/home/pi/Public/projects/sudoku_solver/SudokuSolver" // Raspberry Pi


/* *********************   9 x 9 Puzzles   ********************* */
//Solved Deterministically
//#define FILENAME "P1"  // Very Easy: Only naked singles
//#define FILENAME "P3"  // Easy: Naked & hidden singles
//#define FILENAME "P4"  // Easy: Naked & hidden singles
//#define FILENAME "P6"  // Easy: Naked & hidden singles
//#define FILENAME "P9"  // Easy: Naked & hidden singles
//#define FILENAME "P16" // Medium: Naked & hidden singles, locked candidates
//#define FILENAME "P2"  // Hard: Naked & hidden singles, locked candidates, and naked pairs
//#define FILENAME "P35" // Hard: Naked & hidden singles, locked candidates, and hidden pairs
//#define FILENAME "P8"  // Hard: Naked & hidden singles, locked candidates, and naked & hidden pairs
//#define FILENAME "P10" // Hard: Naked & hidden singles, locked candidates, and naked triplets
//#define FILENAME "P5"  // Very hard: Naked & hidden singles, locked candidates, naked & hidden pairs & triples, and X-wing
//#define FILENAME "P11" // Extremely hard: Naked & hidden singles & pairs & triples and X-wing & Swordfish(3)
//#define FILENAME "P12" // Extremely hard: Naked & hidden singles, locked candidates, hidden pairs & triples, and Swordfish(3)


//Still need random number
//#define FILENAME "P7"  // Very hard: 1 random +  Naked & hidden singles, locked candidates, and naked & hidden pairs
//#define FILENAME "P13" // Very hard: 1 random + Naked & hidden singles, locked candidates, hidden pairs & triples, and x-wing (3x)
//#define FILENAME "P17" // Very hard: 2 random + Naked & hidden singles, locked candidates, naked pairs, and X-wing
//#define FILENAME "P14" // Extremely hard: 2 random + Naked & hidden singles, locked candidates, hidden pairs, and naked quints(5)
//#define FILENAME "P19" // Extremely hard: 3 random + Naked & hidden singles, locked candidates, naked & hidden pairs & quints(5)
//#define FILENAME "P20" // Extremely hard: 2 random + Naked & hidden singles, locked candidates, and Jellyfish(4)
//#define FILENAME "P23" // Extremely hard: 4 random + Naked & hidden singles, locked candidates, hidden triples, naked quads(4) & quints(5)

// #define FILENAME "P15_Hardest" // Evil!: 5 random + Naked & hidden singles, locked candidates, and naked & hidden pairs, triples & quads(4)
//#define FILENAME "P25" // Evil!: 5 random + Naked & hidden singles, locked candidates, and naked & hidden pairs & triples & quads(4)
//#define FILENAME "P24" // Evil!: 5 random + Naked & hidden singles, locked candidates, and naked & hidden pairs & triples & quads(4) & quints(5)
//#define FILENAME "P18" // Evil!: 6 random + Naked & hidden singles, locked candidates, and naked & hidden pairs & quads(4)
//#define FILENAME "P30" // Evil!: 6 random + Naked & hidden singles, locked candidates, naked pairs & triples & quads(4) & quints(5)
//#define FILENAME "P21" // Evil!: 7 random + Naked & hidden singles, locked candidates, naked & hidden pairs & triples, and Swordfish(3) (2x)
//#define FILENAME "P26" // Evil!: 7 random + Naked & hidden singles, locked candidates, naked & hidden pairs & triples & quads(4), and X-Wing (2x) & Swordfish
//#define FILENAME "P22" // Evil!: 7 random + Naked & hidden singles, locked candidates, naked & hidden pairs & triples & quints(5), and X-Wing
//#define FILENAME "P27" // Evil!: 7 random + Naked & hidden singles, locked candidates, and naked & hidden pairs & triples & quads(4) & quints(5)
//#define FILENAME "P33" // Evil!: 8 random + Naked & hidden singles, locked candidates, hidden pairs, naked pairs & triples & quads(4)
//#define FILENAME "P34" // Evil!: 9 random + Naked & hidden singles, locked candidates, and naked pairs & triples & quads(4)
//#define FILENAME "P31" // Evil!: 9 random + Naked & hidden singles, locked candidates, and naked pairs & triples & quads(4) & quints(5)
//#define FILENAME "P32" // Evil!!: 11 random + Naked & hidden singles, locked candidates, hidden pairs, naked pairs & triples & quads(4), and X-Wing (2x)
//#define FILENAME "P29" // Evil!!: 12 random + Naked & hidden singles, locked candidates, naked pairs & triples & quads(4) & quints(5)


//#define FILENAME "P28"  // Unsolvable!!: even with 5000 retries!

//#define FILENAME "TEST"

// TODO: Add more hard puzzles:
//    http://forum.enjoysudoku.com/the-hardest-sudokus-t4212.html (made it to: 6: dml1 - P27)



/* ********************   16 x 16 Puzzles   ******************** */
//     Also need to change NUM and LEN Board Globals!
//#define FILENAME "P16x1" // Medium: Solved with naked & hidden singles and locked candidates