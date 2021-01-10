/**
    @file wordlist.h
    @author Selena Chen (schen53)

    Contains function prototypes for wordlist.c.
*/

#include <stdbool.h>

/**
    Given a character, it returns true if that character is one that can be
    part of a word.

    @param ch character to be checked.
    @return true if the character if one that can be part of a word, false
            otherwise.
*/
bool wordChar( char ch );

/**
    Given a word from the document, it looks it up in the word list and
    returns one of three values. Either the given word is a proper noun, or
    it's a regular word that's spelled correctly, or it's a word that doesn't
    match anything on the word list. Use the preprocessor to define a named
    constant for each of these possible return values.

    @param word word to be looked up.
    @return named constant for a proper noun, regular word that's spelled
            correctly, or a word that doesn't match anything on the word list.
*/
int lookupWord( char const word[] );

/**
    Given the name of a file, it opens it, reads all the words from it and
    adds any new (unique) words to the in-memory word list. If any error
    conditions occur, this function or a function called by it can print out
    the right error message and terminate the program.

    @param fileName name of a file.
*/
void readWords( char const fileName[] );
