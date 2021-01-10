/**
   @file contest.c
   @author Selena Chen (schen53)
   This is the top-level main component. It uses the other two components to
   represent the contest and respond to user commands.
 */

#include <stdio.h>
#include <stdlib.h>
#include "model.h"
#include "command.h"

/**
   Program starting point.
   @return program exit status.
 */
int main()
{
    Contest *contest = makeContest();
    while ( !feof( stdin ) ) {
        if ( !processCommand( contest ) ) {
            printf( "\nInvalid command" );
        }
        if ( !feof( stdin ) ) {
            printf( "\n" );
        }
    }
    freeContest( contest );
    return EXIT_SUCCESS;
}
