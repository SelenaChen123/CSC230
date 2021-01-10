/**
    @file image.c
    @author Selena Chen (schen53)

    This component contains functions to read and write images in Raw PPM format. It's used by the
    conceal and extract programs.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image.h"

#define HEADER_ARGS 4
#define STR_LENGTH 2

Image *readImage(char const *filename)
{
    FILE *fp = fopen( filename, "rb" );
    if ( !fp ) {
        perror( filename );
        exit( EXIT_FAILURE );
    }
    char p6[ STR_LENGTH ];
    int width, height;
    int pixelMax;
    if ( fscanf( fp, "%s %d %d %d%*c", p6, &width, &height, &pixelMax ) != HEADER_ARGS
        || strncmp( FORMAT, p6, STR_LENGTH ) != 0
        || width < 0 || height < 0 || pixelMax != MAX_COLOR ) {
        fclose( fp );
        fprintf( stderr, "Invalid image file\n" );
        exit( EXIT_FAILURE );
    }
    Image *image = (Image *) malloc( sizeof( Image ) );
    image->rows = height;
    image->cols = width;
    image->color = (unsigned char *) malloc( PIXEL_WIDTH * width * height
                                            * sizeof( unsigned char ) );
    for ( int i = 0; i < PIXEL_WIDTH * width * height; i++ ) {
        if ( fread( &image->color[ i ], sizeof( unsigned char ), 1, fp ) != 1 ) {
            fclose( fp );
            freeImage( image );
            fprintf( stderr, "Invalid image file\n" );
            exit( EXIT_FAILURE );
        }
    }
    fclose( fp );
    return image;
}

void writeImage( Image *image, char const *filename )
{
    FILE *fp = fopen( filename, "wb" );
    if ( !fp ) {
        perror( filename );
        exit( EXIT_FAILURE );
    }
    fprintf( fp, "P6\n%d %d\n255\n", image->cols, image->rows );
    fwrite( image->color, sizeof( unsigned char ), image->rows * image->cols * PIXEL_WIDTH, fp );
    fclose( fp );
}

void freeImage( Image *image )
{
    free( image->color );
    free( image );
}
