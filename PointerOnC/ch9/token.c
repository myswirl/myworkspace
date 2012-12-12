/*
** Extract whitespace-delimited tokens from a character array and
** print them one per line.
*/
#include <stdio.h>
#include <string.h>


/***********************************************************
* Function:       // print_tokens
* Description:    // ≤È’“±Íº«token
***********************************************************/
void print_tokens( char *line )
{
	static char whitespace[] = " \t\f\r\v\n";
	char		*token;

	for( token = strtok( line, whitespace ); token != NULL; token = strtok( NULL, whitespace ) )
	{
		printf( "Next token is %s\n", token );
	}
}

int main(void)
{
	char str[100]="the world is my world.";
	print_tokens( str );
	return 0;
}

/************************************** The End Of File **************************************/
