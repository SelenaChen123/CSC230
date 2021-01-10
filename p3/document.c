/**
    @file document.c
    @author Selena Chen (schen53)

    Contains support for reading the document, using the wordlist component
    to check the spelling of individual words and printing out the document
    in the right format.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "wordlist.h"
#include "document.h"

/** Represents a proper noun from the word list. */
#define PROPER_NOUN 1

/** Represents a regular word from the word list. */
#define REGULAR_WORD 2

/** Represents a word not found in the word list. */
#define NOT_FOUND 3

/** Max number of words that can be in the word list. */
#define MAX_WORDS 40000

/** Max number of characters in a word. */
#define MAX_WORD_LENGTH 12

/** Max number of characters in a line. */
#define MAX_LINE_LENGTH 75

/** ASCII code for the ESC character. */
#define ESC 27

/** ASCII difference between capital and lowercase letters. */
#define ASCII_OFFSET 32

/**
    Given a line of text from the document, this function finds each word in
    the line, checks it against the word list and prints the line, highlighted
    as necessary. The two flags passed to this function tell it about whether
    the input line had to be truncated when it was read. If it did, the ellipsis
    flag tells the function to print out an ellipsis at the end of the line. If
    the chopped flag is true, it tells the function that the last word on the
    line was chopped and shouldn't be spellchecked.

    @param line line of text from the document.
    @param ellipsis whether or not to print out an ellipsis at the end of the line.
    @param chopped whether or not the last word on the line should be spellchecked.
*/
static void checkLine( char const line[], bool ellipsis, bool chopped )
{
    const char RED[] = { ESC, '[', '3', '1', 'm', '\0' };
    const char BLUE[] = { ESC, '[', '3', '4', 'm', '\0' };
    const char DEFAULT[] = { ESC, '[', '0', 'm', '\0' };

    int length = strlen( line );
    int start = 0;
    int end = 0;
    for ( int charIndex = 0; charIndex <= length; charIndex++ ) {
        if ( !wordChar( line[ charIndex ] ) ) {
            end = charIndex;
            int wordLength = end - start;
            char word[ wordLength + 1 ];
            for ( int wordIndex = start; wordIndex < end; wordIndex++ ) {
                word[ wordIndex - start ] = line[ wordIndex ];
            }
            word[ wordLength ] = '\0';
            start = end + 1;
            if ( end == strlen( line ) && chopped ) {
                printf( "%s", word );
            } else {
                int lookup;
                if ( strlen( word ) > 0 ) {
                    lookup = lookupWord( word );
                    if ( lookup == PROPER_NOUN ) {
                        printf( "%s%s%s", BLUE, word, DEFAULT );
                    } else if ( lookup == NOT_FOUND ) {
                        char lowercase[ strlen( word ) + 1 ];
                        for ( int index = 0; index < strlen( word ); index++ ) {
                            lowercase[ index ] = ( word[ index ] >= 'A'
                            && word[ index ] <= 'Z' ) ? word[ index ] + ASCII_OFFSET
                            : word[ index ];
                        }
                        lowercase[ strlen( word ) ] = '\0';
                        if ( lookupWord( lowercase ) == REGULAR_WORD ) {
                            printf( "%s", word );
                        } else {
                            printf( "%s%s%s", RED, word, DEFAULT );
                        }
                        for ( int index = 0; index < strlen( lowercase ) + 1; index++ ) {
                            lowercase[ index ] = '\0';
                        }
                    } else {
                        printf( "%s", word );
                    }
                }
            }
            if ( end != length ) {
                printf( "%c", line[ charIndex ] );
            }
            for ( int index = 0; index < MAX_WORD_LENGTH + 1; index++ ) {
                word[ index ] = '\0';
            }
        }
    }
    if ( ellipsis ) {
        printf( "..." );
    }
}

void checkDocument( char const fileName[] )
{
    FILE *fp = fopen( fileName, "r" );
    if ( !fp ) {
        fprintf( stderr, "Can't open file: %s\n", fileName );
        exit( EXIT_FAILURE );
    }
    char line[ MAX_LINE_LENGTH + 1 ];
    bool ellipsis = false;
    bool chopped = false;
    char discarded;
    while ( fscanf( fp, "%75[^\n]%c", line, &discarded ) == 2
            || fscanf( fp, "%c", &discarded ) == 1 ) {
        int length = strlen( line );
        if ( length == 0 ) {
            printf( "\n" );
        } else {
            if ( wordChar( line[ length - 1 ] ) && wordChar( discarded ) ) {
                chopped = true;
            }
            if ( length == MAX_LINE_LENGTH && discarded != '\n' ) {
                ellipsis = true;
            }
            if ( discarded != '\n' ) {
                fscanf( fp, "%*[^\n]%c", &discarded );
            }
            checkLine( line, ellipsis, chopped );
            ellipsis = false;
            chopped = false;
            if ( discarded == '\n' ) {
                printf( "%c", discarded );
            }
        }
        for ( int charIndex = 0; charIndex < MAX_LINE_LENGTH; charIndex++ ) {
            line[ charIndex ] = '\0';
        }
    }
    fclose( fp );
}
