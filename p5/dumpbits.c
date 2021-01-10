/** Helpful program to report the value in every byte of stdin, in binary. */
#include <stdio.h>
void printBits( unsigned char ch )
{
  // Start printing bits from the high-order one.
  for ( int i = 7; i >= 0; i-- )
    printf( "%c", ch & ( 0x01 << i ) ? '1' : '0' );
}

int main()
{
  // Normally, standard input will be in text mode, so it may not handle
  // binary input well. This should fix that.
  freopen( NULL, "rb", stdin );

  int idx = 0;
  int ch;

  // Read the contents of input.
  while ( ( ch = getchar() ) != EOF ) {
    // Print byte index, followed by its binary representation.
    printf( "%04x ", idx );
    printBits( ch );
    printf( "\n" );
    idx++;
  }
}
