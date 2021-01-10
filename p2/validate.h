/**
    @file validate.h
    @author Selena Chen (schen53)

    Contains function prototypes for validate.c.
*/

#include <stdbool.h>

/**
    Returns true if every row in the grid contains exactly one digit in the range 1 to size.

    @param size size of the grid.
    @param grid grid of digits to be validated.
    @return true if every row in the grid contains exactly one digit in the range 1 to size, false
            otherwise.
*/
bool validateRows(int size, int grid[size][size]);

/**
    Returns true if every column in the grid contains exactly one digit in the range 1 to size.

    @param size size of the grid.
    @param grid grid of digits to be validated.
    @return true if every column in the grid contains exactly one digit in the range 1 to size,
            false otherwise.
*/
bool validateCols(int size, int grid[size][size]);

/**
    Returns true if every n X n square (subgrid) in the grid contains exactly one digit in the
    range 1 to size.

    @param n size of the subgrids.
    @param size of the grid.
    @param grid grid of digits to be validated.
    @return true if every n X n square (subgrid) in the grid contains exactly one digit in the
            range 1 to size, false otherwise.
*/
bool validateSquares(int n, int size, int grid[size][size]);

/**
    Returns true if every row in the grid contains no more than one digit in the range 1 to size.

    @param size size of the grid.
    @param grid grid of digits to be validated.
    @return true if every row in the grid contains no more than one digit in the range 1 to size,
            false otherwise.
*/
bool validateRowsWithSpaces(int size, int grid[size][size]);

/**
    Returns true if every column in the grid contains no more than one digit in the range 1 to size.

    @param size size of the grid.
    @param grid grid of digits to be validated.
    @return true if every column in the grid contains no more than one digit in the range 1 to size,
            false otherwise.
*/
bool validateColsWithSpaces(int size, int grid[size][size]);

/**
    Returns true if every n X n square (subgrid) in the grid contains no more than one digit in the
    range 1 to size.

    @param n size of the subgrids.
    @param size size of the grid.
    @param grid grid of digits to be validated.
    @return true if every n X n square (subgrid) in the grid contains no more than one digit in the
            range 1 to size, false otherwise.
*/
bool validateSquaresWithSpaces(int n, int size, int grid[size][size]);
