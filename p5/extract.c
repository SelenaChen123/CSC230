/**
    @file extract.c
    @author Selena Chen (schen53)

    This component will implement the main function of the extract program. It will be responsible
    for handling the command-line arguments and extracting the message characters from the input
    image.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bits.h"
#include "image.h"

#define ARG_NUM 4
#define OUTPUT_ARG 2

/**
    Program starting point.

    @param argc number of command line arguments.
    @param argv command line arguments.
    @return program exit status.
 */
int main( int argc, char *argv[] )
{
    if ( argc != ARG_NUM ) {
        fprintf( stderr, "usage: extract <input-image> <output-message> <bits>\n" );
        exit( EXIT_FAILURE );
    }
    int userNumBits = atoi( argv[ argc - 1 ] );
    if ( userNumBits < 1 || userNumBits > BITS_PER_BYTE ) {
        fprintf( stderr, "Invalid number of bits\n" );
        exit( EXIT_FAILURE );
    }
    Image *image = readImage( argv[ 1 ] );
    FILE *dest = fopen( argv[ OUTPUT_ARG ], "w" );
    if ( !dest ) {
        freeImage( image );
        perror( argv[ OUTPUT_ARG ] );
        exit( EXIT_FAILURE );
    }
    int len = ( image->rows * image->cols * PIXEL_WIDTH * userNumBits ) / BITS_PER_BYTE;
    char message[ len + 1 ];
    for ( int i = 0; i < len + 1; i++ ) {
        message[ i ] = '\0';
    }
    int mPos = 0;
    int bPos = 0;
    for ( int i = 0; i < image->rows * image->cols * PIXEL_WIDTH; i++ ) {
        for ( int j = 0; j < userNumBits; j++ ) {
            message[ mPos ] = putBit( message[ mPos ], bPos++, getBit( image->color[ i ], j ) );
            if ( bPos == BITS_PER_BYTE ) {
                if ( message[ mPos ] == '\0' ) {
                    goto done;
                }
                bPos = 0;
                mPos++;
            }
        }
    }
    if ( strlen( message ) == len ) {
        message[ mPos ] = '\0';
    }
done:
    for ( int i = 0; i < strlen( message ); i++ ) {
        fprintf( dest, "%c", message[ i ] );
    }
    freeImage( image );
    fclose( dest );
    return EXIT_SUCCESS;
}
