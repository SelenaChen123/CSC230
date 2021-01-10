/**
    @file sudoku.c
    @author Selena Chen (schen53)

    Program that tells whether a completed Sudoku puzzle is valid, i.e., it meets the requirements
    given above, or is invalid because it does not meet the requirements. The program will also
    solve a partially completed Sudoku puzzle or determine that it is invalid due to not meeting
    the requirements or because it cannot be solved. In addition to 9 X 9 puzzles, the program will
    work with puzzles whose size is a perfect square ranging from 4 to 16.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "validate.h"
#include "solve.h"

#define MIN_SIZE 4
#define MID_SIZE 9
#define MAX_SIZE 16
#define MIN_N 2
#define MID_N 3
#define MAX_N 4

/**
    Program starting point.

    @return program exit status.
*/
int main()
{
    int n, size;
    int matches = scanf( "%d", &size );
    if ( matches < 1 || ( size != MIN_SIZE && size != MID_SIZE && size != MAX_SIZE ) ) {
        printf( "Invalid size\n" );
        return EXIT_FAILURE;
    }
    if ( size == MIN_SIZE ) {
        n = MIN_N;
    } else if ( size == MID_SIZE ) {
        n = MID_N;
    } else if ( size == MAX_SIZE ) {
        n = MAX_N;
    }
    int grid[ size ][ size ];
    for ( int rowIndex = 0; rowIndex < size; rowIndex++ ) {
        for ( int columnIndex = 0; columnIndex < size; columnIndex++ ) {
            int num;
            int matches = scanf( "%d", &num );
            if ( matches < 1 || num < 0 || num > size ) {
                printf( "Invalid input\n" );
                return EXIT_FAILURE;
            }
            grid[ rowIndex ][ columnIndex ] = num;
        }
    }
    if ( validateRows( size, grid ) && validateCols( size, grid )
        && validateSquares( n, size, grid ) ) {
        printf( "Valid\n" );
    } else if ( validateRowsWithSpaces( size, grid ) && validateColsWithSpaces( size, grid )
                && validateSquaresWithSpaces( n, size, grid ) ) {
        if ( !solve( n, size, grid ) ) {
            printf( "Invalid\n" );
            return EXIT_SUCCESS;
        }
        for ( int rowIndex = 0; rowIndex < size; rowIndex++ ) {
            for ( int columnIndex = 0; columnIndex < size; columnIndex++ ) {
                printf( "%3d", grid[ rowIndex ][ columnIndex ] );
            }
            printf( "\n" );
        }
    } else {
        printf( "Invalid\n" );
    }
    return EXIT_SUCCESS;
}
