/**
    @file validate.c
    @author Selena Chen (schen53)

    Responsible for validating the rows, columns, and squares of a puzzle to determine if the Sudoku
    rules have been violated.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "validate.h"

bool validateRows(int size, int grid[size][size])
{
    for ( int rowIndex = 0; rowIndex < size; rowIndex++ ) {
        for ( int digit = 1; digit <= size; digit++ ) {
            for ( int columnIndex = 0; columnIndex < size; columnIndex++ ) {
                if ( grid[ rowIndex ][ columnIndex ] == digit ) {
                    break;
                } else if ( columnIndex == size - 1 ){
                    return false;
                }
            }
        }
    }
    return true;
}

bool validateCols(int size, int grid[size][size])
{
    for ( int columnIndex = 0; columnIndex < size; columnIndex++ ) {
        for ( int digit = 1; digit <= size; digit++ ) {
            for ( int rowIndex = 0; rowIndex < size; rowIndex++ ) {
                if ( grid[ rowIndex ][ columnIndex ] == digit ) {
                    break;
                } else if ( rowIndex == size - 1 ){
                    return false;
                }
            }
        }
    }
    return true;
}

bool validateSquares(int n, int size, int grid[size][size])
{
    for ( int subgridRowIndex = 0; subgridRowIndex < size; subgridRowIndex += n ) {
        for ( int subgridColumnIndex = 0; subgridColumnIndex < size; subgridColumnIndex += n ) {
            for (int digit = 1; digit <= size; digit++ ) {
            bool found = false;
                for ( int rowIndex = subgridRowIndex; rowIndex < subgridRowIndex + n ;
                        rowIndex++ ) {
                    for ( int columnIndex = subgridColumnIndex;
                            columnIndex < subgridColumnIndex + n;
                            columnIndex++ ) {
                        if ( grid[ rowIndex ][ columnIndex ] == digit ) {
                            found = true;
                            break;
                        } else if ( rowIndex == subgridRowIndex + n - 1
                                    && columnIndex == subgridColumnIndex + n - 1 ) {
                            return false;
                        }
                    }
                    if ( found ) {
                        break;
                    }
                }
            }
        }
    }
    return true;
}

bool validateRowsWithSpaces(int size, int grid[size][size])
{
    for ( int rowIndex = 0; rowIndex < size; rowIndex++ ) {
        for ( int digit = 1; digit <= size; digit++ ) {
            int count = 0;
            for ( int columnIndex = 0; columnIndex < size; columnIndex++ ) {
                if ( grid[ rowIndex ][ columnIndex ] == digit ) {
                    count++;
                }
                if ( count > 1 ) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool validateColsWithSpaces(int size, int grid[size][size])
{
     for ( int columnIndex = 0; columnIndex < size; columnIndex++ ) {
        for ( int digit = 1; digit <= size; digit++ ) {
            int count = 0;
            for ( int rowIndex = 0; rowIndex < size; rowIndex++ ) {
                if ( grid[ rowIndex ][ columnIndex ] == digit ) {
                    count++;
                } 
                if ( count > 1 ) {
                    return false;
                }
            }
        }
    }
    return true;  
}

bool validateSquaresWithSpaces(int n, int size, int grid[size][size])
{
    for ( int subgridRowIndex = 0; subgridRowIndex < size; subgridRowIndex += n ) {
        for ( int subgridColumnIndex = 0; subgridColumnIndex < size; subgridColumnIndex += n ) {
            for (int digit = 1; digit <= size; digit++ ) {
                int count = 0;
                for ( int rowIndex = subgridRowIndex; rowIndex < subgridRowIndex + n ;
                        rowIndex++ ) {
                    for ( int columnIndex = subgridColumnIndex;
                            columnIndex < subgridColumnIndex + n;
                            columnIndex++ ) {
                        if ( grid[ rowIndex ][ columnIndex ] == digit ) {
                            count++;
                        }
                    }
                    if ( count > 1 ) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}
