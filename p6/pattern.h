/**
    @file pattern.h
    @author Selena Chen (schen53)

    Contains function prototypes for pattern.c.
  */

#ifndef PATTERN_H
#define PATTERN_H

#include <stdbool.h>

//////////////////////////////////////////////////////////////////////
// Superclass for Patterns

/** A short name to use for the Pattern interface. */
typedef struct PatternStruct Pattern;

/**
    Structure used as a superclass/interface for a regular expression
    pattern.  There's a function pointer for an overridable method,
    match(), that reports all the places where this pattern matches a
    given string.  There's also an overridable method for freeing
    resources for the pattern.
  */
struct PatternStruct {
  /**
      Method for matching this pattern against a given string.  For
      each substring str[ begin ] .. str[ end - 1 ] that matches this
      pattern, this function sets table[ begin ][ end ] to true.

      @param pat pointer to the pattern being matched (essentially, a this
                 pointer).
      @param str input string in which we're finding matches.
      @param len length of str.
      @param table ( len + 1 ) x ( len + 1 ) array of booleans that
                   gets filled in with the substrings where this
                   pattern matches the string.
    */
  void (*match)( Pattern *pat, char const *str, int len, bool (*table)[ len + 1 ] );

  /**
      Free memory for this pattern, including any subpatterns it contains.

      @param pat pattern to free.
    */
  void (*destroy)( Pattern *pat );
};

/**
    Makes a pattern for a single, non-special character, like `a` or `5`.

    @param sym symbol this pattern is supposed to match.
    @return dynamically allocated representation for this new pattern.
  */
Pattern *makeLiteralPattern( char sym );

/**
    Makes a pattern for a single occurrence of any character.

    @return dynamically allocated representation for this new pattern.
  */
Pattern *makeAnyCharacterPattern();

/**
    Makes a pattern that will only match things at the beginning of the line.

    @return dynamically allocated representation for this new pattern.
  */
Pattern *makeStartingPattern();

/**
    Makes a pattern that only match things at the end of the line, or used along
    with ^, to make patterns that have to match everything on the line.

    @return dynamically allocated representation for this new pattern.
  */
Pattern *makeEndingPattern();

/**
    Makes a pattern that matches any one character given in the sequence.

    @return dynamically allocated representation for this new pattern.
  */
Pattern *makeCharacterClassPattern();

/**
    Make a pattern for the concatenation of patterns p1 and p2.  It
    should match anything that can be broken into two substrings, s1 and
    s2, where the p1 matches the first part (s1) and p2 matches the
    second part (s2).

    @param p1 subpattern for matching the first part of the string.
    @param p2 subpattern for matching the second part of the string.
    @return dynamically allocated representation for this new pattern.
  */
Pattern *makeConcatenationPattern( Pattern *p1, Pattern *p2 );

/**
    Make a pattern for consecutive occurrences of anything p1 and p2 matches.

    @param p1 subpattern for matching the first part of the string.
    @param p2 subpattern for matching the second part of the string.
    @return dynamically allocated representation for this new pattern.
  */
Pattern *makeAlternationPattern( Pattern *p1, Pattern *p2 );

/**
    Make a pattern for zero or more consecutive occurrences of anything p matches

    @param p pointer to the pattern being matched.
    @return dynamically allocated representation for this new pattern.
  */
Pattern *makeNoneOrMoreCharacterPattern( Pattern *p );

/**
    Make a pattern for one or more consecutive occurrences of anything p matches

    @param p pointer to the pattern being matched.
    @return dynamically allocated representation for this new pattern.
  */
Pattern *makeOneOrMoreCharacterPattern( Pattern *p );

/**
    Make a pattern for zero or one consecutive occurrences of anything p matches

    @param p pointer to the pattern being matched.
    @return dynamically allocated representation for this new pattern.
  */
Pattern *makeNoneOrOneCharacterPattern( Pattern *p );

#endif
