/**
    @file ugrep.c
    @author Selena Chen (schen53)

    This component contains the main() function. It's responsible for handling command-line
    arguments, using the parser to build a representation for the regular expression, reading
    input lines, using the Pattern representation to find places where the pattern matchs part
    of an input line, and printing out the lines with the matches highlighted in red.
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pattern.h"
#include "parse.h"

// On the command line, which argument is the pattern.
#define PAT_ARG 1

// On the command line, which argument is the input file.
#define FILE_ARG 2

// ASCII code for the ESC character.
#define ESC 27

// Minimum number of command linearguments.
#define MIN_ARGS 2

// Maximum character length of a line.
#define MAX_LINE_LENGTH 100

/**
    Entry point for the program, parses command-line arguments, builds
    the pattern and then tests it against lines of input.

    @param argc number of command-line arguments.
    @param argv list of command-line arguments.
    @return exit status for the program.
  */
int main( int argc, char *argv[] )
{
  const char RED[] = { ESC, '[', '3', '1', 'm', '\0' };
  const char DEFAULT[] = { ESC, '[', '0', 'm', '\0' };

  if ( argc < MIN_ARGS || argc > MIN_ARGS + 1 ) {
    fprintf( stderr, "usage: ugrep <pattern> [input-file.txt]\n" );
    exit( EXIT_FAILURE );
  }

  FILE *fp = NULL;

  if ( argc == MIN_ARGS ) {
    fp = stdin;
  } else {
    fp = fopen( argv[ FILE_ARG ], "r" );
  }
  if ( !fp ) {
    fprintf( stderr, "Can't open input file: %s\n", argv[ FILE_ARG ] );
    exit( EXIT_FAILURE );
  }

  char string[ MAX_LINE_LENGTH ];
  while ( fscanf( fp, "%100[^\n]", string ) == 1 ) {
    int length = strlen( string );

    char ch;
    if ( fscanf( fp, "%c", &ch ) > 0 && length == MAX_LINE_LENGTH ) {
      fprintf( stderr, "Input line too long\n" );
      fclose( fp );
      exit( EXIT_FAILURE );
    }

    char discard = ch;
    while ( discard != '\n' ) {
      discard = fgetc( fp );
    }

    char *patternString = argv[ PAT_ARG ];
    Pattern *pattern = parsePattern( patternString );

    bool (*table)[ length + 1 ]  = calloc( ( length + 1 ) * ( length + 1 ), sizeof( bool ) );
    pattern->match( pattern, string, length, table );

    bool foundMatch = false;
    for ( int begin = 0; begin <= length; begin++ ) {
      for ( int end = begin; end <= length; end++ ) {
        if ( table[ begin ][ end ] ) {
          foundMatch = true;
          break;
        }
      }
      if ( foundMatch ) {
        break;
      }
    }

    if ( foundMatch ) {
      for ( int begin = 0; begin <= length; begin++ ) {
        for ( int end = length; end >= begin; end-- ) {
          if ( table[ begin ][ end ] ) {
            printf( RED );
            for ( int i = begin; i < end; i++ ) {
              printf( "%c", string[ i ] );
              begin = end;
            }
            printf( DEFAULT );
          }
        }
        if ( string[ begin ] != '\0' ) {
          printf( "%c", string[ begin ] );
        }
      }
      printf( "\n" );
    }

    free( table );
    pattern->destroy( pattern );
  }

  fclose( fp );

  return EXIT_SUCCESS;
}
