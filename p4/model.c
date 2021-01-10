/**
   @file model.c
   @author Selena Chen (schen53)

   This component defines the representation for events of interest during the
   contest. The other components use this to keep up with the problems, the
   contestants and the attempts to solve problems.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model.h"

/** Initial capacity of an array. */
#define INITIAL_CAPACITY 3

Problem *makeProblem( char const *id, char const *name )
{
    Problem *problem = (Problem *) malloc( sizeof( Problem ) );
    strcpy( problem->id, id );
    strcpy( problem->name, name );
    problem->aCount = 0;
    problem->sCount = 0;
    return problem;
}

void freeProblem( Problem *problem )
{
    free( problem );
}

Contestant *makeContestant( char const *id, char const *name )
{
    Contestant *contestant = (Contestant *) malloc( sizeof( Contestant ) );
    strcpy( contestant->id, id );
    strcpy( contestant->name, name );
    contestant->aCount = 0;
    contestant->aCap = INITIAL_CAPACITY;
    contestant->sCount = 0;
    contestant->penalty = 0;
    contestant->aList = (Attempt *) malloc( contestant->aCap * sizeof( Attempt ) );
    return contestant;
}

void freeContestant( Contestant *contestant )
{
    free( contestant->aList );
    free( contestant );
}

Contest *makeContest()
{
    Contest *contest = (Contest *) malloc( sizeof( Contest ) );
    contest->pCount = 0;
    contest->pCap = INITIAL_CAPACITY;
    contest->pList = (Problem **) malloc( contest->pCap * sizeof( Problem * ) );
    contest->cCount = 0;
    contest->cCap = INITIAL_CAPACITY;
    contest->cList = (Contestant **) malloc( contest->cCap * sizeof( Contestant * ) );
    return contest;
}

void freeContest( Contest *contest )
{
    for ( int i = 0; i < contest->pCount; i++ ) {
        freeProblem( contest->pList[ i ] );
    }
    free( contest->pList );
    for ( int i = 0; i < contest->cCount; i++ ) {
        freeContestant( contest->cList[ i ] );
    }
    free( contest->cList );
    free( contest );
}

Problem *findProblem( Contest *contest, char const *id )
{
    for ( int i = 0; i < contest->pCount; i++ ) {
        if ( strcmp( id, contest->pList[ i ]->id ) == 0 ) {
            return contest->pList[ i ];
        }
    }
    return NULL;
}

Contestant *findContestant( Contest *contest, char const *id )
{
    for ( int i = 0; i < contest->cCount; i++ ) {
        if ( strcmp( id, contest->cList[ i ]->id ) == 0 ) {
            return contest->cList[ i ];
        }
    }
    return NULL;
}
