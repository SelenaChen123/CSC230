/**
    @file image.h
    @author Selena Chen (schen53)

    Header for the image component, with a representation for an Image
    and functions for reading and writing images in PPM format.
 */

#ifndef _IMAGE_H_
#define _IMAGE_H_

/** PPM file format string. */
#define FORMAT "P6"

/** Maximum color value. */
#define MAX_COLOR 255

/** Number of intensity values per pixel. */
#define PIXEL_WIDTH 3

/** Representation for image file data. */
typedef struct {
  /** number of rows. */
  int rows;
  
  /** pixels per row. */
  int cols;
  
  /** Dynamically allocated pixel data, rows * cols pixels, each with
      3 intensities (RGB), stored in row-major order. */
  unsigned char *color;
} Image;

/**
    This function dynamically allocates an instance of Image and populates it based on the given
    PPM file. If it encounters problems with the format of the image file, it prints an appropriate
    error message and terminates the program.

    @param filename name of the file to read image information from.
    @return dynamically allocated instance of Image populated with the information from the given
            PPM file.
 */
Image *readImage(char const *filename);

/**
    This function writes out the given image in Raw PPM format to a file with the given name. It
    will print an error message and terminate the program if the given file can't be opened.

    @param image Image to be written.
    @param filename name of the file to write Image to.
 */
void writeImage( Image *image, char const *filename );

/**
    This function frees all the memory associated with an image representation, including the pixel
    data and the Image struct itself.

    @param image Image to be freed.
 */
void freeImage( Image *image );

#endif
