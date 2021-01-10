/**
    @file bits.c
    @author Selena Chen (schen53)

    This component will define functions for testing individual bits and setting and clearing
    individual bits in a byte. It's used by the conceal and extract programs.
 */

#include "bits.h"

bool getBit( unsigned char ch, int n )
{
    return ( ch >> n ) & 1;
}

unsigned char putBit( unsigned char ch, int n, bool v )
{
    if ( v ) {
        return ch | ( 1 << n );
    } else {
        return ch & ~( 1 << n );
    }
}
