/**
   @file model.h
   @author Selena Chen (schen53)

   Contains function prototypes for model.c.
 */

#include <stdbool.h>

/** Maximum length of a person or problem unique id. */
#define MAX_ID 16

/** Maximum length of a person or problem name. */
#define MAX_NAME 40

/** Representation for a problem in the contest. */
typedef struct {
  /** Unique ID for this problem, containing no whitespace. */
  char id[ MAX_ID + 1 ];

  /** Name of this problem (maybe not unique, and maybe with whitespace). */
  char name[ MAX_NAME + 1 ];

  /** Number of attempts. */
  int aCount;

  /** Number of successful attempts. */
  int sCount;
} Problem;

/** Record for an attempt to solve a problem. */
typedef struct {
  /** Problem this attempt was for. */
  Problem *problem;

  /** Was it a successful attempt. */
  bool solved;
} Attempt;

/** Representation for a person or a team in the competition. */
typedef struct {
  /** Unique ID for this person (containing no whitespace). */
  char id[ MAX_ID + 1 ];

  /** Name for this person (maybe not unique and maybe whitespace). */
  char name[ MAX_NAME + 1 ];

  /** Number of attempts. */
  int aCount;

  /** List of all attempts. */
  Attempt *aList;

  /** Capacity of the aList array. */
  int aCap;

  /** Number of successful attempts. */
  int sCount;

  /** Total number of penalty points. */
  int penalty;
} Contestant;

/** Representation for the whole contest, containing a resizable list of problmes
    and a resizable list of contestants. */
typedef struct {
  /** Number of problems. */
  int pCount;

  /** List of all problems. */
  Problem **pList;

  /** Capacity of the pList array. */
  int pCap;

  /*/ Number of contestants. */
  int cCount;

  /** List of all the contestants. */
  Contestant **cList;

  /** Capacity of the current cList array. */
  int cCap;
} Contest;

/**
   This dynamically allocates an instance of Problem, initializes its fields and
   returns a pointer to it. It copies the given name and id fields into the new
   struct's fields.

   @param id unique problem id.
   @param name problem name.
   @return dyanamically allocated instance of a Problem.
 */
Problem *makeProblem( char const *id, char const *name );

/**
   This frees the memory used for the given problem.

   @param problem Problem to be freed.
 */
void freeProblem( Problem *problem );

/**
   This dynamically allocates an instance of Contestant, initializes its fields and
   returns a pointer to it. It copies the given name and id fields into the struct's
   fields, and, since Contestant contains a resizable array, this function has to
   initialize that.

   @param id unique contestant id.
   @param name contestant name.
   @return dyanamically allocated instance of a Contestant.
 */
Contestant *makeContestant( char const *id, char const *name );

/**
   This frees the memory used for the given contestant. This includes freeing the
   memory for the resizable array it contains.

   @param contestant Contestant to be freed.
 */
void freeContestant( Contestant *contestant );

/**
   This dynamically allocates an instance of Contest, initializes its fields and
   returns a pointer to it. Since Contest contains two resizable arrays, this
   function has to initialize both of them.

   @return dyanamically allocated instance of a Contest.
 */
Contest *makeContest();

/**
   This frees the memory used for the given contest. This includes freeing all the
   problems and contestants on its two lists. Of course, this function can use the
   functions above to help with this.

   @param contest Contest to be freed.
 */
void freeContest( Contest *contest );

/**
   Given a contest and a problem ID, this function returns a pointer to the problem
   with that ID, or NULL if it doesn't exist.

   @param contest contest associated with the problem to be found.
   @param id id associated with the problem to be found.
   @return problem associated with the given contest and id.
 */
Problem *findProblem( Contest *contest, char const *id );

/**
   Given a contest and a contestant ID, this function returns a pointer to the
   contestant with that ID, or NULL if it doesn't exist.

   @param contest contest associated with the contestant to be found.
   @param id id associated with the contestant to be found.
   @return contestant associated with the given contest and id.
 */
Contestant *findContestant( Contest *contest, char const *id );
