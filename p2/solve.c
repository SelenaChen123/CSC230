/**
    @file solve.c
    @author Selena Chen (schen53)

    Solves a Sudoku puzzle or determines that it cannot be solved.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "validate.h"
#include "solve.h"

bool solve(int n, int size, int grid[size][size])
{
    int blanks[ size * size ][ 2 ];
    int count = 0;
    for ( int rowIndex = 0; rowIndex < size; rowIndex++ ) {
        for ( int columnIndex = 0; columnIndex < size; columnIndex++ ) {
            if ( grid[ rowIndex ][ columnIndex ] == 0 ) {
                blanks[ count ][ 0 ] = rowIndex;
                blanks[ count ][ 1 ] = columnIndex;
                count++;
            }
        }
    }
    
    for ( int blankIndex = 0; blankIndex < count; blankIndex++ ) {
        grid[ blanks[ blankIndex ][ 0 ] ][ blanks[ blankIndex ][ 1 ] ]++;
        while ( !validateRowsWithSpaces( size, grid )
                || !validateColsWithSpaces( size, grid )
                || !validateSquaresWithSpaces( n, size, grid ) ) {
            grid[ blanks[ blankIndex ][ 0 ] ][ blanks[ blankIndex ][ 1 ] ]++;
            bool valid = validateRowsWithSpaces( size, grid )
                            && validateColsWithSpaces( size, grid )
                            && validateSquaresWithSpaces( n, size, grid );
            if ( !valid && grid[ blanks[ blankIndex ][ 0 ] ][ blanks[ blankIndex ][ 1 ] ] >= size ) {
                if ( blankIndex == 0 ) {
                    return false;
                }
                
                grid[ blanks[ blankIndex ][ 0 ] ][ blanks[ blankIndex ][ 1 ] ] = 0;
                blankIndex -= 2;
                break;
            }
        }
        
    }
    return true;
}
