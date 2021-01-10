/**
   @file command.c
   @author Selena Chen (schen53)

   This component is responsible for parsing and performing user commands. It uses
   the model component.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model.h"

/** Max character length of an id. */
#define MAX_ID_LENGTH 16

/** Max character length of a name. */
#define MAX_NAME_LENGTH 40

/** Max character length of a command. */
#define MAX_CMD_LENGTH 11

/** Value of a penalty for an attempt. */
#define PENALTY_AMT 20

/**
   Orders problems by their difficulty rating.

   @param problem1 first problem to be ordered.
   @param problem2 second problem to be ordered.
   @return -1 if problem1 comes before problem2, 1 if problem1 comes after problem2,
            or 0 if problem1 is equal to problem2.
 */
static int pComp( const void *problem1, const void *problem2 )
{
    Problem const *p1 = *( (Problem **) problem1 );
    Problem const *p2 = *( (Problem **) problem2 );
    if ( p1->sCount > p2->sCount ) {
        return -1;
    } else if ( p1->sCount < p2->sCount ) {
        return 1;
    } else {
        if ( p1->aCount < p2->aCount ) {
            return -1;
        } else if ( p1->aCount > p2->aCount ) {
            return 1;
        } else {
            return strcmp( p1->id, p2->id );
        }
    }
}

/**
   Orders contestants by their difficulty rating.

   @param contestant1 first contestant to be ordered.
   @param contestant2 second contestant to be ordered.
   @return -1 if contestant1 comes before contestant2, 1 if contestant1 comes after.
            contestant2, or 0 if contestant1 is equal to contestant2.
 */
static int cComp( const void *contestant1, const void *contestant2 )
{
    Contestant const *c1 = *( (Contestant **) contestant1 );
    Contestant const *c2 = *( (Contestant **) contestant2 );
    if ( c1->sCount > c2->sCount ) {
        return -1;
    } else if ( c1->sCount < c2->sCount ) {
        return 1;
    } else {
        if ( c1->penalty < c2->penalty ) {
            return -1;
        } else if ( c1->penalty > c2->penalty ) {
            return 1;
        } else {
            return strcmp( c1->id, c2->id );
        }
    }
}

/**
   Helper function to list all problems.

   @param problem problem to be tested.
   @param data extra information the test function needs in order to do its job.
   @return true when testing problems.
 */
static bool problemsTest( Problem *problem, void *data ) {
    return true;
}

/**
   Helper function to list all solved problems.

   @param problem problem to be tested.
   @param data extra information the test function needs in order to do its job.
   @return true if the problem is solved, false otherwise.
 */
static bool solvedTest( Problem *problem, void *data ) {
    Contestant *contestant = (Contestant *) data;
    for ( int i = 0; i < contestant->aCount; i++ ) {
        if ( strcmp( problem->id, contestant->aList[ i ].problem->id ) == 0
            && contestant->aList[ i ].solved ) {
            return true;
        }
    }
    return false;
}

/**
   Helper function to list all unsolved problems.

   @param problem problem to be tested.
   @param data extra information the test function needs in order to do its job.
   @return false if the problem is solved, true otherwise
 */
static bool unsolvedTest( Problem *problem, void *data ) {
    Contestant *contestant = (Contestant *) data;
    for ( int i = 0; i < contestant->aCount; i++ ) {
        if ( strcmp( problem->id, contestant->aList[ i ].problem->id ) == 0
            && contestant->aList[ i ].solved ) {
            return false;
        }
    }
    return true;
}

/**
   This function is for listing problems. It's used to implement the list problems,
   list solved and list unsolved commands. The first parameter is the contest data
   structure. The next parameter is a pointer to a function (test) that you'll use
   to decide which problems to report, and the last parameter (data) is a pointer to
   any additional data the test function might need to do its job.

   @param contest contest associated with the problems to be listed.
   @param test function that decides which problems to report.
   @param data additional data the test function might need to do its job.
 */
static void listProblems( Contest *contest, bool (*test)( Problem *, void * ), void *data )
{
    for ( int i = 0; i < contest->pCount; i++ ) {
        Problem *problem = contest->pList[ i ];
        if ( test( problem, data ) ) {
            printf( "\n%-16s %-40s %9d %9d", problem->id, problem->name, problem->sCount,
                    problem->aCount );
        }
    }
}

bool processCommand( Contest *contest )
{
    static int cmdNum = 1;
    char cmd[ MAX_CMD_LENGTH + 1 ];
    printf( "%d> ", cmdNum );
    cmdNum++;
    if ( scanf( " %10[^ \n]", cmd ) == 1 ) {
        if ( strcmp( "problem", cmd ) == 0 ) {
            char id[ MAX_ID_LENGTH + 1 ];
            char name[ MAX_NAME_LENGTH + 1 ];
            int ch;
            if ( scanf( " %16[^ \n]", id ) != 1 || ( ch = getchar() ) != ' ' ) {
                scanf( "%*[^\n]" );
                return false;
            }
            ungetc( ch, stdin );
            if ( scanf( " %40[^\n]", name ) != 1 || ( ch = getchar() ) != '\n' ) {
                scanf( "%*[^\n]" );
                return false;
            }
            ungetc( ch, stdin );
            if ( findProblem( contest, id ) ) {
                scanf( "%*[^\n]" );
                return false;
            }
            if ( contest->pCount + 1 >= contest->pCap ) {
                contest->pCap *= 2;
                contest->pList = (Problem **) realloc( contest->pList,
                                    contest->pCap * sizeof( Problem * ) );
            }
            contest->pList[ contest->pCount ] = makeProblem( id, name );
            contest->pCount++;
        } else if ( strcmp( "contestant", cmd ) == 0 ) {
            char id[ MAX_ID_LENGTH + 1 ];
            char name[ MAX_NAME_LENGTH + 1 ];
            int ch;
            if ( scanf( " %16[^ \n]", id ) != 1 || ( ch = getchar() ) != ' ' ) {
                scanf( "%*[^\n]" );
                return false;
            }
            ungetc( ch, stdin );
            if ( scanf( " %40[^\n]", name ) != 1 || ( ch = getchar() ) != '\n' ) {
                scanf( "%*[^\n]" );
                return false;
            }
            ungetc( ch, stdin );
            if ( findContestant( contest, id ) ) {
                scanf( "%*[^\n]" );
                return false;
            }
            if ( contest->cCount + 1 >= contest->cCap ) {
                contest->cCap *= 2;
                contest->cList = (Contestant **) realloc( contest->cList,
                                    contest->cCap * sizeof( Contestant * ) );
            }
            contest->cList[ contest->cCount ] = makeContestant( id, name );
            contest->cCount++;
        } else if ( strcmp( "attempt", cmd ) == 0 ) {
            char contestantID[ MAX_NAME_LENGTH + 1 ];
            char problemID[ MAX_NAME_LENGTH + 1 ];
            int ch;
            if ( scanf( " %16[^ \n]", contestantID ) != 1 || ( ch = getchar() ) != ' ' ) {
                scanf( "%*[^\n]" );
                return false;
            }
            ungetc( ch, stdin );
            if ( scanf( " %40[^\n]", problemID ) != 1 ) {
                scanf( "%*[^\n]" );
                return false;
            }
            Contestant *contestant = findContestant( contest, contestantID );
            Problem *problem = findProblem( contest, problemID );
            if ( contestant->aCount + 1 >= contestant->aCap ) {
                contestant->aCap *= 2;
                contestant->aList = (Attempt *) realloc( contestant->aList,
                                        contestant->aCap * sizeof( Attempt ) );
            }
            int count = contestant->aCount;
            for ( int i = 0; i < count; i++ ) {
                if ( strcmp( problem->id, contestant->aList[ i ].problem->id ) == 0
                    && contestant->aList[ i ].solved ) {
                    return true;
                }
            }
            contestant->aList[ contestant->aCount ].problem = problem;
            contestant->aList[ contestant->aCount ].solved = false;
            contestant->aCount++;
            problem->aCount++;
        } else if ( strcmp( "solved", cmd ) == 0 ) {
            char contestantID[ MAX_NAME_LENGTH + 1 ];
            char problemID[ MAX_NAME_LENGTH + 1 ];
            int ch;
            if ( ( scanf( " %16[^ \n]", contestantID ) != 1 ) || ( ( ch = getchar() ) != ' ' ) ) {
                scanf( "%*[^\n]" );
                return false;
            }
            ungetc( ch, stdin );
            if ( scanf( " %40[^\n]", problemID ) != 1 ) {
                scanf( "%*[^\n]" );
                return false;
            }
            Contestant *contestant = findContestant( contest, contestantID );
            Problem *problem = findProblem( contest, problemID );
            if ( contestant->aCount + 1 >= contestant->aCap ) {
                contestant->aCap *= 2;
                contestant->aList = (Attempt *) realloc( contestant->aList,
                                        contestant->aCap * sizeof( Attempt ) );
            }
            int penaltyCount = 0;
            for ( int i = 0; i < contestant->aCount; i++ ) {
                if ( strcmp( problem->id, contestant->aList[ i ].problem->id ) == 0
                    && !contestant->aList[ i ].solved ) {
                    penaltyCount++;
                }
            }
            for ( int i = 0; i < penaltyCount; i++ ) {
                contestant->penalty += PENALTY_AMT;
            }
            int count = contestant->aCount;
            for ( int i = 0; i < count; i++ ) {
                if ( strcmp( problem->id, contestant->aList[ i ].problem->id ) == 0
                    && contestant->aList[ i ].solved ) {
                    return true;
                }
            }
            contestant->aList[ contestant->aCount ].problem = problem;
            contestant->aList[ contestant->aCount ].solved = true;
            contestant->aCount++;
            contestant->sCount++;
            problem->aCount++;
            problem->sCount++;
        } else if ( strcmp( "list", cmd ) == 0 ) {
            if ( scanf( " %11[^ \n]", cmd ) != 1 ) {
                scanf( "%*[^\n]" );
                return false;
            }
            if ( strcmp( "problems", cmd ) == 0 ) {
                printf( "\n%-16s %-40s %9s %9s", "ID", "Name", "Solutions", "Attempts" );
                if ( contest->pCount > 0 ) {
                    qsort( contest->pList, contest->pCount, sizeof( Problem * ), pComp );
                    listProblems( contest, problemsTest, NULL );
                }
            } else if ( strcmp( "contestants", cmd ) == 0 ) {
                printf( "\n%-16s %-40s %9s %9s", "ID", "Name", "Solved", "Penalty" );
                if ( contest->cCount > 0 ) {
                    qsort( contest->cList, contest->cCount, sizeof( Contestant * ), cComp );
                    for ( int i = 0; i < contest->cCount; i++ ) {
                        Contestant *contestant = contest->cList[ i ];
                        printf( "\n%-16s %-40s %9d %9d", contestant->id, contestant->name,
                                contestant->sCount, contestant->penalty );
                    }
                }
            } else if ( strcmp( "solved", cmd ) == 0 ) {
                printf( "\n%-16s %-40s %9s %9s", "ID", "Name", "Solutions", "Attempts" );
                char id[ MAX_ID_LENGTH + 1 ];
                if ( scanf( " %16[^ \n]", id ) != 1 ) {
                    scanf( "%*[^\n]" );
                    return false;
                }
                Contestant *contestant = findContestant( contest, id );
                qsort( contest->pList, contest->pCount, sizeof( contest->pList[ 0 ] ), pComp );
                listProblems( contest, solvedTest, contestant );
            } else if ( strcmp( "unsolved", cmd ) == 0 ) {
                printf( "\n%-16s %-40s %9s %9s", "ID", "Name", "Solutions", "Attempts" );
                char id[ MAX_ID_LENGTH + 1 ];
                if ( scanf( " %16[^ \n]", id ) != 1 ) {
                    return true;
                }
                Contestant *contestant = findContestant( contest, id );
                qsort( contest->pList, contest->pCount, sizeof( contest->pList[ 0 ] ), pComp );
                listProblems( contest, unsolvedTest, contestant );
            } else {
                scanf( "%*[^\n]" );
                return false;
            }
        } else if ( strcmp( "quit", cmd ) == 0 ) {
            printf( "\n" );
            freeContest( contest );
            exit( EXIT_SUCCESS );
        } else {
            scanf( "%*[^\n]" );
            return false;
        }
    }
    return true;
}
