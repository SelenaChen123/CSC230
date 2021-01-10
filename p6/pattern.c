/**
    @file pattern.c
    @author Selena Chen (schen53)

    The pattern component implements the inheritance hierarchy used for regular expressions.
    It defines the abstract superclass for a Pattern as well as different concrete subclasses
    for represent various parts of regular expression syntax. The starter includes a partial
    implementaton for the pattern component. It defines the pattern superclass, along with a
    few functions that serve as methods for the class. It also includes concrete implementations
    of pattern for matching ordinary symbols (LiteralPattern) and for matching concatenated
    patterns (BinaryPattern).
  */

#include "pattern.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
    A simple function that can be used to free the memory for any
    pattern that doesn't allocate any additional memory other than the
    struct used to represent it (e.g., if it doesn't contain any
    sub-patterns).  It's like a default implementation of the destroy
    method.

    @param pat pattern to free memory for.
  */
static void destroySimplePattern( Pattern *pat )
{
  // If we don't need fields that are specific to the sub-type, we can just
  // free the block of memory where the object is stored.
  free( pat );
}

/**
    Type of pattern used to represent a single, ordinary symbol,
    like 'a' or '5'.
  */
typedef struct {
  // Fields from our superclass.
  void (*match)( Pattern *pat, char const *str, int len, bool (*table)[ len + 1 ] );
  void (*destroy)( Pattern *pat );

  /** Symbol this pattern is supposed to match. */
  char sym;
} LiteralPattern;

/**
    Overridden match() method for a LiteralPattern.

    @param pat pointer to the pattern being matched (essentially, a this
                pointer).
    @param str input string in which we're finding matches.
    @param len length of str.
    @param table ( len + 1 ) x ( len + 1 ) array of booleans that
                  gets filled in with the substrings where this
                  pattern matches the string.
  */
static void matchLiteralPattern( Pattern *pat, char const *str, int len,
                                 bool (*table)[ len + 1 ] )
{
  // Cast down to the struct type pat really points to.
  LiteralPattern *this = (LiteralPattern *) pat;

  // Find all occurreces of the symbol we're supposed to match, and
  // mark them in the match table as matching, 1-character substrings.
  for ( int i = 0; i < len; i++ )
    if ( str[ i ] == this->sym )
      table[ i ][ i + 1 ] = true;
}

// Documented in the header.
Pattern *makeLiteralPattern( char sym )
{
  // Make an instance of LiteralPattern, and fill in its state.
  LiteralPattern *this = (LiteralPattern *) malloc( sizeof( LiteralPattern ) );

  this->match = matchLiteralPattern;
  this->destroy = destroySimplePattern;
  this->sym = sym;

  return (Pattern *) this;
}

/**
    Type of pattern used to represent a single occurrence of any character.
  */
typedef struct {
  // Fields from our superclass.
  void (*match)( Pattern *pat, char const *str, int len, bool (*table)[ len + 1 ] );
  void (*destroy)( Pattern *pat );
} AnyCharacterPattern;

/**
    Overridden match() method for a AnyCharacterPattern.

    @param pat pointer to the pattern being matched (essentially, a this
                pointer).
    @param str input string in which we're finding matches.
    @param len length of str.
    @param table ( len + 1 ) x ( len + 1 ) array of booleans that
                  gets filled in with the substrings where this
                  pattern matches the string.
  */
static void matchAnyCharacterPattern( Pattern *pat, char const *str, int len,
                                      bool (*table)[ len + 1 ] )
{
  for ( int i = 0; i < len; i++ ) {
    table[ i ][ i + 1 ] = true;
  }
}

// Documented in the header.
Pattern *makeAnyCharacterPattern()
{
  AnyCharacterPattern *this = (AnyCharacterPattern *) malloc( sizeof( AnyCharacterPattern ) );

  this->match = matchAnyCharacterPattern;
  this->destroy = destroySimplePattern;
  
  return (Pattern *) this;
}

/**
    Type of pattern used to represent things at the beginning of the line.
  */
typedef struct {
  // Fields from our superclass.
  void (*match)( Pattern *pat, char const *str, int len, bool (*table)[ len + 1 ] );
  void (*destroy)( Pattern *pat );
} StartingPattern;

/**
    Overridden match() method for a StartingPattern.

    @param pat pointer to the pattern being matched (essentially, a this
                pointer).
    @param str input string in which we're finding matches.
    @param len length of str.
    @param table ( len + 1 ) x ( len + 1 ) array of booleans that
                  gets filled in with the substrings where this
                  pattern matches the string.
  */
static void matchStartingPattern( Pattern *pat, char const *str, int len,
                                  bool (*table)[ len + 1 ] )
{
  table[ 0 ][ 0 ] = true;
}

// Documented in the header.
Pattern *makeStartingPattern()
{
  StartingPattern *this = (StartingPattern *) malloc( sizeof( StartingPattern ) );

  this->match = matchStartingPattern;
  this->destroy = destroySimplePattern;
  
  return (Pattern *) this;
}

/**
    Type of pattern used to represent things at the end of the line.
  */
typedef struct {
  // Fields from our superclass.
  void (*match)( Pattern *pat, char const *str, int len, bool (*table)[ len + 1 ] );
  void (*destroy)( Pattern *pat );
} EndingPattern;

/**
    Overridden match() method for an EndingPattern.

    @param pat pointer to the pattern being matched (essentially, a this
                pointer).
    @param str input string in which we're finding matches.
    @param len length of str.
    @param table ( len + 1 ) x ( len + 1 ) array of booleans that
                  gets filled in with the substrings where this
                  pattern matches the string.
  */
static void matchEndingPattern( Pattern *pat, char const *str, int len,
                                bool (*table)[ len + 1 ] )
{
  table[ len ][ len ] = true;
}

// Documented in the header.
Pattern *makeEndingPattern()
{
  EndingPattern *this = (EndingPattern *) malloc( sizeof( EndingPattern ) );

  this->match = matchEndingPattern;
  this->destroy = destroySimplePattern;
  
  return (Pattern *) this;
}

/**
    Type of pattern used to represent any one character given in the sequence.
  */
typedef struct {
  // Fields from our superclass.
  void (*match)( Pattern *pat, char const *str, int len, bool (*table)[ len + 1 ] );
  void (*destroy)( Pattern *pat );

  /** Symbols this pattern is supposed to match. */
  char *characters;
} CharacterClassPattern;

/**
    Free memory for this pattern, including any subpatterns it contains.

    @param pat pattern to free.
  */
static void destroyCharacterClassPattern( Pattern *pat )
{
  CharacterClassPattern *this = (CharacterClassPattern *) pat;
  free( this->characters );
  free( pat );
}

/**
    Overridden match() method for a CharacterClassPattern.

    @param pat pointer to the pattern being matched (essentially, a this
                pointer).
    @param str input string in which we're finding matches.
    @param len length of str.
    @param table ( len + 1 ) x ( len + 1 ) array of booleans that
                  gets filled in with the substrings where this
                  pattern matches the string.
  */
static void matchCharacterClassPattern( Pattern *pat, char const *str, int len,
                                        bool (*table)[ len + 1 ] )
{
  CharacterClassPattern *this = (CharacterClassPattern *) pat;

  for ( int i = 0; i < len; i++ ) {
    if ( strchr( this->characters, str[ i ] ) ) {
      table[ i ][ i + 1 ] = true;
    }
  }
}

// Documented in the header.
Pattern *makeCharacterClassPattern( char *str )
{
  CharacterClassPattern *this = (CharacterClassPattern *) malloc( sizeof( CharacterClassPattern ) );

  this->match = matchCharacterClassPattern;
  this->destroy = destroyCharacterClassPattern;
  this->characters = str;
  
  return (Pattern *) this;
}

/**
    Representation for a type of pattern that contains two sub-patterns
    (e.g., concatenation).  This representation could be used by more
    than one type of pattern, as long as it uses a pointer to a
    different match() function.
  */
typedef struct {
  // Fields from our superclass.
  void (*match)( Pattern *pat, char const *str, int len, bool (*table)[ len + 1 ] );
  void (*destroy)( Pattern *pat );

  // Pointers to the two sub-patterns.
  Pattern *p1, *p2;
} BinaryPattern;

/**
    Free memory for this pattern, including any subpatterns it contains.

    @param pat pattern to free.
  */
static void destroyBinaryPattern( Pattern *pat )
{
  // Cast down to the struct type pat really points to.
  BinaryPattern *this = (BinaryPattern *) pat;

  // Free our two sub-patterns.
  this->p1->destroy( this->p1 );
  this->p2->destroy( this->p2 );
  // Free the struct representing this object.
  free( this );
}

/**
    Overridden match() method for a BinaryPattern used to handle concatenation.

    @param pat pointer to the pattern being matched (essentially, a this
                pointer).
    @param str input string in which we're finding matches.
    @param len length of str.
    @param table ( len + 1 ) x ( len + 1 ) array of booleans that
                  gets filled in with the substrings where this
                  pattern matches the string.
  */
static void matchConcatenationPattern( Pattern *pat, char const *str, int len,
                                       bool (*table)[ len + 1 ] )
{
  // Cast down to the struct type pat really points to.
  BinaryPattern *this = (BinaryPattern *) pat;

  // Two more tables for matching sub-expressions.
  bool (*tbl1)[ len + 1 ]  = calloc( ( len + 1 ) * ( len + 1 ), sizeof( bool ) );
  bool (*tbl2)[ len + 1 ]  = calloc( ( len + 1 ) * ( len + 1 ), sizeof( bool ) );

  //  Let our two sub-patterns figure out everywhere they match.
  this->p1->match( this->p1, str, len, tbl1 );
  this->p2->match( this->p2, str, len, tbl2 );

  // Then, based on their matches, look for all places where their
  // concatenaton matches.  Check all substrings of the input string.
  for ( int begin = 0; begin <= len; begin++ )
    for ( int end = begin; end <= len; end++ ) {

      // For the [ begin, end ) range, check all places where it could
      // be split into two substrings, the first matching p1 and the second
      // matching p2.
      for ( int k = begin; k <= end; k++ )
        if ( tbl1[ begin ][ k ] && tbl2[ k ][ end ] )
          table[ begin ][ end ] = true;
    }

  free( tbl1 );
  free( tbl2 );
}

// Documented in the header.
Pattern *makeConcatenationPattern( Pattern *p1, Pattern *p2 )
{
  // Make an instance of Binary pattern and fill in its fields.
  BinaryPattern *this = (BinaryPattern *) malloc( sizeof( BinaryPattern ) );
  this->p1 = p1;
  this->p2 = p2;

  this->match = matchConcatenationPattern;
  this->destroy = destroyBinaryPattern;

  return (Pattern *) this;
}

/**
    Type of pattern used to represent consecutive occurrences of matches.
  */
typedef struct {
  // Fields from our superclass.
  void (*match)( Pattern *pat, char const *str, int len, bool (*table)[ len + 1 ] );
  void (*destroy)( Pattern *pat );

  // Pointers to the two sub-patterns.
  Pattern *p1, *p2;
} AlternationPattern;

/**
    Free memory for this pattern, including any subpatterns it contains.

    @param pat pattern to free.
  */
static void destroyAlternationPattern( Pattern *pat )
{
  AlternationPattern *this = (AlternationPattern *) pat;

  this->p1->destroy( this->p1 );
  this->p2->destroy( this->p2 );
  
  free( this );
}

/**
    Overridden match() method for an AlternationPattern.

    @param pat pointer to the pattern being matched (essentially, a this
                pointer).
    @param str input string in which we're finding matches.
    @param len length of str.
    @param table ( len + 1 ) x ( len + 1 ) array of booleans that
                  gets filled in with the substrings where this
                  pattern matches the string.
  */
static void matchAlternationPattern( Pattern *pat, char const *str, int len,
                                     bool (*table)[ len + 1 ] )
{
  AlternationPattern *this = (AlternationPattern *) pat;

  bool (*tbl1)[ len + 1 ]  = calloc( ( len + 1 ) * ( len + 1 ), sizeof( bool ) );
  bool (*tbl2)[ len + 1 ]  = calloc( ( len + 1 ) * ( len + 1 ), sizeof( bool ) );

  this->p1->match( this->p1, str, len, tbl1 );
  this->p2->match( this->p2, str, len, tbl2 );

  for ( int begin = 0; begin <= len; begin++ ) {
    for ( int end = 0; end <= len; end++ ) {
      table[ begin ][ end ] = tbl1[ begin ][ end ] || tbl2[ begin ][ end ];
    }
  }

  free( tbl1 );
  free( tbl2 );
}

// Documented in the header.
Pattern *makeAlternationPattern( Pattern *p1, Pattern *p2 )
{
  AlternationPattern *this = (AlternationPattern *) malloc( sizeof( AlternationPattern ) );

  this->match = matchAlternationPattern;
  this->destroy = destroyAlternationPattern;
  this->p1 = p1;
  this->p2 = p2;
  
  return (Pattern *) this;
}

/**
    Type of pattern used to represent zero or more consecutive occurrences of matches.
  */
typedef struct {
  // Fields from our superclass.
  void (*match)( Pattern *pat, char const *str, int len, bool (*table)[ len + 1 ] );
  void (*destroy)( Pattern *pat );

  // Pattern to be matched.
  Pattern *pattern;
} NoneOrMoreCharacterPattern;

/**
    Free memory for this pattern, including any subpatterns it contains.

    @param pat pattern to free.
  */
static void destroyNoneOrMoreCharacterPattern( Pattern *pat )
{
  NoneOrMoreCharacterPattern *this = (NoneOrMoreCharacterPattern *) pat;
  this->pattern->destroy( this->pattern );
  free( this );
}

/**
    Overridden match() method for a NoneOrMoreCharacterPattern.

    @param pat pointer to the pattern being matched (essentially, a this
                pointer).
    @param str input string in which we're finding matches.
    @param len length of str.
    @param table ( len + 1 ) x ( len + 1 ) array of booleans that
                  gets filled in with the substrings where this
                  pattern matches the string.
  */
static void matchNoneOrMoreCharacterPattern( Pattern *pat, char const *str, int len,
                                             bool (*table)[ len + 1 ] )
{
  NoneOrMoreCharacterPattern *this = (NoneOrMoreCharacterPattern *) pat;

  bool (*tbl)[ len + 1 ] = calloc( ( len + 1 ) * ( len + 1 ), sizeof( bool ) );
  this->pattern->match( this->pattern, str, len, tbl );
  this->pattern->match( this->pattern, str, len, table );

  for ( int begin = 0; begin <= len; begin++ ) {
      for ( int end = 0; end <= len; end++ ) {
          if ( tbl[ begin ][ end ] ) {
              int length = end - begin;
              int count = 1;
              while ( end + length * count <= len &&
                      tbl[ begin + length * count ][ end + length * count ] ) {
                  count++;
              }
              for ( int i = 0; i < count; i++ ) {
                  for ( int j = begin; j <= begin + length * i; j += length ) {
                      table[ j ][ end + length * i ] = true;
                  }
              }
          }
      }
  }

  for ( int i = 0; i <= len; i++ ) {
      table[ i ][ i ] = true;
  }

  free( tbl );
}

// Documented in the header.
Pattern *makeNoneOrMoreCharacterPattern( Pattern *pat )
{
  NoneOrMoreCharacterPattern *this = (NoneOrMoreCharacterPattern *)
                                      malloc( sizeof( NoneOrMoreCharacterPattern ) );
  this->pattern = pat;
  this->match = matchNoneOrMoreCharacterPattern;
  this->destroy = destroyNoneOrMoreCharacterPattern;
  return (Pattern *) this;
}

/**
    Type of pattern used to represent one or more consecutive occurrences of matches.
  */
typedef struct {
  // Fields from our superclass.
  void (*match)( Pattern *pat, char const *str, int len, bool (*table)[ len + 1 ] );
  void (*destroy)( Pattern *pat );

  // Pattern to be matched.
  Pattern *pattern;
} OneOrMoreCharacterPattern;

/**
    Free memory for this pattern, including any subpatterns it contains.

    @param pat pattern to free.
  */
static void destroyOneOrMoreCharacterPattern( Pattern *pat )
{
    OneOrMoreCharacterPattern *this = (OneOrMoreCharacterPattern *) pat;
    this->pattern->destroy( this->pattern );
    free( this );
}

/**
    Overridden match() method for a OneOrMoreCharacterPattern.

    @param pat pointer to the pattern being matched (essentially, a this
                pointer).
    @param str input string in which we're finding matches.
    @param len length of str.
    @param table ( len + 1 ) x ( len + 1 ) array of booleans that
                  gets filled in with the substrings where this
                  pattern matches the string.
  */
static void matchOneOrMoreCharacterPattern( Pattern *pat, char const *str, int len,
                                            bool (*table)[ len + 1 ] )
{
  OneOrMoreCharacterPattern *this = (OneOrMoreCharacterPattern *) pat;

  bool (*tbl)[ len + 1 ] = calloc( ( len + 1 ) * ( len + 1 ), sizeof( bool ) );
  this->pattern->match( this->pattern, str, len, tbl );
  this->pattern->match( this->pattern, str, len, table );

  for ( int begin = 0; begin <= len; begin++ ) {
      for ( int end = 0; end <= len; end++ ) {
          if ( tbl[ begin ][ end ] ) {
              int length = end - begin;
              int count = 1;
              while ( end + length * count <= len &&
                      tbl[ begin + length * count ][ end + length * count ] ) {
                  count++;
              }
              for ( int i = 0; i < count; i++ ) {
                  for ( int j = begin; j <= begin + length * i; j += length ) {
                      table[ j ][ end + length * i ] = true;
                  }
              }
          }
      }
  }

  free( tbl );
}

// Documented in the header.
Pattern *makeOneOrMoreCharacterPattern( Pattern *pat )
{
  OneOrMoreCharacterPattern *this = (OneOrMoreCharacterPattern *)
                                      malloc( sizeof( OneOrMoreCharacterPattern ) );
  this->pattern = pat;
  this->match = matchOneOrMoreCharacterPattern;
  this->destroy = destroyOneOrMoreCharacterPattern;
  return (Pattern *) this;
}

/**
    Type of pattern used to represent none or one consecutive occurrences of matches.
  */
typedef struct {
  // Fields from our superclass.
  void (*match)( Pattern *pat, char const *str, int len, bool (*table)[ len + 1 ] );
  void (*destroy)( Pattern *pat );

  // Pattern to be matched.
  Pattern *pattern;
} NoneOrOneCharacterPattern;

/**
    Free memory for this pattern, including any subpatterns it contains.

    @param pat pattern to free.
  */
static void destroyNoneOrOneCharacterPattern( Pattern *pat )
{
    NoneOrOneCharacterPattern *this = (NoneOrOneCharacterPattern *) pat;
    this->pattern->destroy( this->pattern );
    free( this );
}

/**
    Overridden match() method for a NoneOrOneCharacterPattern.

    @param pat pointer to the pattern being matched (essentially, a this
                pointer).
    @param str input string in which we're finding matches.
    @param len length of str.
    @param table ( len + 1 ) x ( len + 1 ) array of booleans that
                  gets filled in with the substrings where this
                  pattern matches the string.
  */
static void matchNoneOrOneCharacterPattern( Pattern *pat, char const *str, int len,
                                            bool (*table)[ len + 1 ] )
{
  NoneOrOneCharacterPattern *this = (NoneOrOneCharacterPattern *) pat;
  this->pattern->match( this->pattern, str, len, table );
  for ( int i = 0; i <= len; i++ ) {
      table[ i ][ i ] = true;
  }
}

// Documented in the header.
Pattern *makeNoneOrOneCharacterPattern( Pattern *pat )
{
  NoneOrOneCharacterPattern *this = (NoneOrOneCharacterPattern *)
                                      malloc( sizeof( NoneOrOneCharacterPattern ) );
  this->pattern = pat;
  this->match = matchNoneOrOneCharacterPattern;
  this->destroy = destroyNoneOrOneCharacterPattern;
  return (Pattern *) this;
}
