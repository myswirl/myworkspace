/*
** Mystery function
**
**	The argument is a value in the range 0 through 100.
*/
#include <stdio.h>

void mystery( int n )
{
	printf("%03d ",n );
	n += 5;
	n /= 10;
	printf( "%s\n", "**********" + 10 - n );
}

int main(void)
{
	mystery( 5 );
	mystery( 10 );
	mystery( 15 );
	mystery( 20 );
	mystery( 25 );
	mystery( 30 );
	mystery( 35 );	
	mystery( 40 );
	mystery( 45 );
	mystery( 50 );
	mystery( 55 );
	mystery( 60 );
	mystery( 65 );	
	mystery( 70 );
	mystery( 75 );
	mystery( 80 );
	mystery( 85 );
	mystery( 90 );
	mystery( 95 );
	mystery( 100 );
	return 0;
}

