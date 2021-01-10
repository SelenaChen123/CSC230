/**
    @file spellcheck.c
    @author Selena Chen (schen53)

    Calls functions in the other components to load the word lists and to
    spellcheck the document.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "wordlist.h"
#include "document.h"

/**
    Program starting point.

    @param argc number of command line arguments.
    @param argv command line arguments.
    @return program exit status.
 */
int main( int argc, char *argv[] )
{
    if ( argc < 2 ) {
        fprintf( stderr, "usage: spellcheck <word-file>* <document-file>\n" );
        exit( EXIT_FAILURE );
    }
    for ( int argIndex = 0; argIndex < argc; argIndex++ ) {
        if ( argc > 2 && argIndex > 0 && argIndex < argc - 1 ) {
            readWords( argv[ argIndex ] );
        }
    }
    FILE *fp = fopen( argv[ argc - 1 ], "r" );
    if ( !fp ) {
        fprintf( stderr, "Can't open file: %s\n", argv[ argc - 1 ] );
        exit( EXIT_FAILURE );
    }
    checkDocument( argv[ argc - 1 ] );
    fclose( fp );
    return EXIT_SUCCESS;
}
