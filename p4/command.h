/**
   @file command.h
   @author Selena Chen (schen53)

   Contains function prototypes for command.c.
 */

/**
   This function reads a user command from standard input and performs that
   command, updating or using the given contest instance as necessary. If the user
   enters an invalid command, this function returns false.

   @param contest contest to be updated.
   @return true if the command is processed successfully, false otherwise.
 */
bool processCommand( Contest *contest );
