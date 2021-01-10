/**
    @file solve.h
    @author Selena Chen (schen53)

    Contains function protoypes for solve.c.
*/

#include <stdbool.h>

/**
    Solves a Sudoku puzzle or determines that it cannot be solved.

    @param n size of the subgrids.
    @param size size f the grid.
    @param grid grid to be solved.
    @return true if the Sudoku puzzle can be solved, false otherwise.
*/
bool solve(int n, int size, int grid[size][size]);
