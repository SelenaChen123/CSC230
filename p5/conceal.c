/**
    @file conceal.c
    @author Selena Chen (schen53)

    This component will implement the main function of the conceal program. It will be responsible
    for handling the command-line arguments, reading the image and message files, hiding bits of
    the message in the image and writing out the resulting image file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bits.h"
#include "image.h"

#define ARG_NUM 5
#define IMAGE_ARG 2
#define OUTPUT_ARG 3

/**
    Program starting point.

    @param argc number of command line arguments.
    @param argv command line arguments.
    @return program exit status.
 */
int main( int argc, char *argv[] )
{
    if ( argc != ARG_NUM ) {
        fprintf( stderr, "usage: conceal <input-message> <input-image> <output-image> <bits>\n" );
        exit( EXIT_FAILURE );
    }
    int userNumBits = atoi( argv[ argc - 1 ] );
    if ( userNumBits < 1 || userNumBits > BITS_PER_BYTE ) {
        fprintf( stderr, "Invalid number of bits\n" );
        exit( EXIT_FAILURE );
    }
    Image *image = readImage( argv[ IMAGE_ARG ] );
    FILE *src = fopen( argv[ 1 ], "r" );
    if ( !src ) {
        fclose( src );
        perror( argv[ 1 ] );
        exit( EXIT_FAILURE );
    }
    int len = ( image->rows * image->cols * PIXEL_WIDTH * userNumBits ) / BITS_PER_BYTE;
    if ( len == 0 ) {
        fclose( src );
        fprintf( stderr, "Invalid number of bits\n" );
        exit( EXIT_FAILURE );
    }
    int pos = 0;
    char message[ len + 1 ];
    for ( int i = 0; i < len + 1; i++ ) {
        message[ i ] = '\0';
    }
    while ( strlen( message ) <= len && fscanf( src, "%c", &message[ pos ] ) == 1 ) {
        pos++;
    }
    if ( fscanf( src, "%*c" ) != EOF ) {
        fclose( src );
        fprintf( stderr, "Invalid number of bits\n" );
        exit( EXIT_FAILURE );
    }
    fclose( src );
    while ( pos != len + 1 ) {
        message[ pos++ ] = '\0';
    }
    int mPos = 0;
    int bPos = 0;
    for ( int i = 0; i < image->rows * image->cols * PIXEL_WIDTH; i++ ) {
        for ( int j = 0; j < userNumBits; j++ ) {
            image->color[ i ] = putBit( image->color[ i ], j, getBit( message[ mPos ], bPos++ ) );
            if ( bPos == BITS_PER_BYTE ) {
                bPos = 0;
                mPos++;
            }
        }
    }
    writeImage( image, argv[ OUTPUT_ARG ] );
    freeImage( image );
}
