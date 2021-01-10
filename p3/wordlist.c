/**
    @file wordlist.c
    @author Selena Chen (schen53)

    Provides support for reading word list files, storing and managing the
    in-memory word list and checking given words against the list.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "wordlist.h"

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

/** ASCII difference between capital and lowercase letters. */
#define ASCII_OFFSET 32

/** List of used used to decide if words are spelled right. */
static char wordList[ MAX_WORDS ][ MAX_WORD_LENGTH + 1 ];

/** Size of the word list. */
static int size;

bool wordChar( char ch )
{
    if ( ( ch >= 'a' && ch <= 'z' ) || ( ch >= 'A' && ch <= 'Z' ) || ch == '\'' ) {
        return true;
    }
    return false;
}

int lookupWord( char const word[] )
{
    int length = strlen( word );
    for ( int charIndex = 0; charIndex < length; charIndex++ ) {
        if ( !wordChar( word[ charIndex ] ) ) {
            return NOT_FOUND;
        }
    }
    bool couldBeProper = false;
    for ( int charIndex = 0; charIndex < length; charIndex++ ) {
        char ch = word[ charIndex ];
        if ( ch >= 'A' && ch <= 'Z' ) {
            couldBeProper = true;
            break;
        }
    }
    if ( couldBeProper ) {
        for ( int wordIndex = 0; wordIndex < size; wordIndex++ ) {
            if ( length != strlen( wordList[ wordIndex ] ) ) {
                continue;
            }
            for ( int charIndex = 0; charIndex < strlen( word ); charIndex++ ) {
                if ( word[ charIndex ] != wordList[ wordIndex ][ charIndex ] ) {
                    break;
                } else if ( charIndex == length - 1 ) {
                    return PROPER_NOUN;
                }
            }
        }
    } else {
        for ( int wordIndex = 0; wordIndex < size; wordIndex++ ) {
            if ( length != strlen( wordList[ wordIndex ] ) ) {
                continue;
            }
            for ( int charIndex = 0; charIndex < length; charIndex++ ) {
                char ch = word[ charIndex ];
                ch = ( ch >= 'A' && ch <= 'Z' ) ? ch + ASCII_OFFSET : ch;
                if ( ch != wordList[ wordIndex ][ charIndex ] ) {
                    break;
                } else if ( charIndex == length - 1 ) {
                    return REGULAR_WORD;
                }
            }
        }
    }
    return NOT_FOUND;
}

void readWords( char const fileName[] )
{
    FILE *fp = fopen( fileName, "r" );
    if ( !fp ) {
        fprintf( stderr, "Can't open file: %s\n", fileName );
        exit( EXIT_FAILURE );
    }
    int lineNum = 1;
    int count = 0;
    char word[ MAX_WORD_LENGTH + 1 ];
    char ch = fgetc( fp );
    while ( ch != EOF ) {
        count++;
        if ( !wordChar( ch ) && ch != '\n' ) {
            fprintf( stderr, "Invalid word at %s:%d\n", fileName, lineNum);
            exit( EXIT_FAILURE );
        } else if ( wordChar( ch ) ) {
            word[ count - 1 ] = ch;
        } else if ( ch == '\n' && count > 1 ) {
            if ( count > MAX_WORD_LENGTH + 1 ) {
                fprintf( stderr, "Invalid word at %s:%d\n", fileName, lineNum);
                exit( EXIT_FAILURE );
            }
            word[ count - 1 ] = '\0';
            if ( lookupWord( word ) == NOT_FOUND ) {
                if ( size + 1 > MAX_WORDS ) {
                    fprintf( stderr, "Too many words\n" );
                    exit( EXIT_FAILURE );
                }
                for ( int index = 0; index < strlen( word ); index++ ) {
                    wordList[ size ][ index ] = word[ index ];
                }
                wordList[ size ][ strlen( word ) ] = '\0';
                size++;
            }
            count = 0;
            lineNum++;
            for ( int index = 0; index < MAX_WORD_LENGTH; index++ ) {
                word[ index ] = '\0';
            }
        }
        ch = fgetc( fp );
    }
    fclose( fp );
}
