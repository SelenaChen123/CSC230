/**
    @file parse.c
    @author Selena Chen (schen53)

    The parse component parses the text of a regular expression and turns it into a collection
    of Pattern objects that represent it.
  */

#include "parse.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/**
    Return true if the given character is ordinary, if it should just
    match occurrences of itself.  This returns false for metacharacters
    like '*' that control how patterns are matched.

    @param c character that should be evaluated as ordinary or special.
    @return true if c is not special.
  */
static bool ordinary( char c )
{
  // See if c is on our list of special characters.
  if ( strchr( ".^$*?+|()[{", c ) )
    return false;
  return true;
}

/**
    Print the appropriate error message for an invalid pattern and exit
    unsuccessfully.
  */
static void invalidPattern()
{
  fprintf( stderr, "Invalid pattern\n" );
  exit( EXIT_FAILURE );
}

// Forward declaration for a parser function defined below.
static Pattern *parseAlternation( char const *str, int *pos );

/**
    Parse regular expression syntax with the highest precedence,
    individual, ordinary symbols, start and end anchors, character
    classes and patterns surrounded by parentheses.

    @param str string being parsed.
    @param pos pass-by-reference value for the location in str being parsed,
                increased as characters from str are parsed.
    @return dynamically allocated representation of the pattern for the next
            portion of str.
  */
static Pattern *parseAtomicPattern( char const *str, int *pos )
{
  if ( ordinary( str[ *pos ] ) ) {
    return makeLiteralPattern( str[ (*pos)++ ] );
  } else if ( str[ *pos ] == '.' ) {
    (*pos)++;
    return makeAnyCharacterPattern();
  } else if ( str[ *pos ] == '^' ) {
    (*pos)++;
    return makeStartingPattern();
  } else if ( str[ *pos ] == '$' ) {
    (*pos)++;
    return makeEndingPattern();
  } else if ( str[ *pos ] == '[' ) {
    int start = *pos + 1;
    (*pos)++;

    int len = 0;
    while ( str[ *pos ] && str[ *pos ] != ']' ) {
      (*pos)++;
      len++;
    }

    if ( !str[ *pos ] ) {
      invalidPattern();
    }

    char *characters = ( char * ) malloc( sizeof( char ) * ( len + 1 ) );
    for ( int i = start; i < *pos; i++ ) {
      characters[ i - start ] = str[ i ];
    }
    characters[ len ] = '\0';

    (*pos)++;

    return makeCharacterClassPattern( characters );
  } else if ( str[ *pos ] == '(' ) {
    (*pos)++;

    Pattern *p = parseAlternation( str, pos );

    if ( str[ *pos ] != ')' ) {
        invalidPattern();
    }

    (*pos)++;

    return p;
  }

  invalidPattern();
  return NULL; // Just to make the compiler happy.
}

/**
    Parse regular expression syntax with the second-highest precedence,
    a pattern, p, optionally followed by one or more repetition syntax like '*' or '+'.
    If there's no repetition syntax, it just returns the pattern object for p.

    @param str string being parsed.
    @param pos pass-by-reference value for the location in str being parsed,
                increased as characters from str are parsed.
    @return dynamically allocated representation of the pattern for the next
            portion of str.
  */
static Pattern *parseRepetition( char const *str, int *pos )
{
  Pattern *p = parseAtomicPattern( str, pos );

  if ( str[ *pos ] == '*' ) {
    (*pos)++;
    return makeNoneOrMoreCharacterPattern( p );
  } else if ( str[ *pos ] == '+' ) {
    (*pos)++;
    return makeOneOrMoreCharacterPattern( p );
  } else if ( str[ *pos ] == '?' ) {
    (*pos)++;
    return makeNoneOrOneCharacterPattern( p );
  }

  return p;
}

/**
    Parse regular expression syntax with the third-highest precedence,
    one pattern, p, (optionally) followed by additional patterns
    (concatenation).  If there are no additional patterns, it just
    returns the pattern object for p.

    @param str string being parsed.
    @param pos pass-by-reference value for the location in str being parsed,
                increased as characters from str are parsed.
    @return dynamically allocated representation of the pattern for the next
            portion of str.
  */
static Pattern *parseConcatenation( char const *str, int *pos )
{
  // Parse the first pattern.
  Pattern *p1 = parseRepetition( str, pos );
  // While there are additional patterns, parse them.
  while ( str[ *pos ] && str[ *pos ] != '|' && str[ *pos ] != ')' ) {
    Pattern *p2 = parseRepetition( str, pos );

    // And build a concatenation pattern to match the sequence.
    p1 = makeConcatenationPattern( p1, p2 );
  }

  return p1;
}

/**
    Parse regular expression syntax with the lowest precedence, one
    pattern, p, (optionally) followed by additional patterns separated
    by | (alternation).  If there are no additional patterns, it just
    returns the pattern object for p.

    @param str string being parsed.
    @param pos pass-by-reference value for the location in str being
                parsed, increased as characters from str are parsed.
    @return dynamically allocated representation of the pattern for
            the next portion of str.
  */
static Pattern *parseAlternation( char const *str, int *pos )
{
  Pattern *p1 = parseConcatenation( str, pos );

  while ( str[ *pos ] == '|' ) {
      (*pos)++;
      Pattern *p2 = parseConcatenation( str, pos );
      p1 = makeAlternationPattern( p1, p2 );
  }

  return p1;
}

// Documented in the header.
Pattern *parsePattern( char const *str )
{
  // Parse the argument into a tree of pattern objects.
  int pos = 0;
  Pattern *pat = parseAlternation( str, &pos );

  // Complain if this didn't consume the whole pattern.
  if ( str[ pos ] )
    invalidPattern();

  return pat;
}
