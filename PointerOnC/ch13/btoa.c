/*
** Take an integer value (unsigned), convert it to characters, and
** print it.  Leading zeros are suppressed.
*/
#include <stdio.h>

void binary_to_ascii( unsigned int value )
{
	unsigned int	quotient;

	quotient = value / 16;
	if( quotient != 0 )
		binary_to_ascii( quotient );
	putchar( "0123456789ABCDEF"[value % 16] );
}

int main(void)
{
	printf("A:0x");
	binary_to_ascii( 'A' ); 
	printf("\n");

	printf("a:0x");
	binary_to_ascii( 'a' ); 
	printf("\n");

	printf("65:0x");
	binary_to_ascii( 65 ); 
	printf("\n");

	return 0;
}

