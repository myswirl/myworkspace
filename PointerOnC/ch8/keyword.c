/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// keyword.c
 * Author:			// 
 * Date:			// 
 * Description:		// 
 * Version:			// 
 * History:			// 
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <stdio.h>
#include <string.h>

char const *keyword[] = {
	"do",
	"for",
	"if",
	"register",
	"return",
	"switch",
	"while",
	"else",
	"then",
	NULL
};


/*
** Determine whether the argument matches any of the words in
** a list of keywords, and return the index to the one it matches.
** If no match is found, return the value -1.
*/
int lookup_keyword( char const * const desired_word,
                    char const *keyword_table[], int const size )
{
	char const **kwp;
	//For each word in the table ...
	for( kwp = keyword_table; kwp < keyword_table + size; kwp++ )
	{
		/*
		** If this word matches the one we're looking for,
		** return its position in the table.
		*/
		if( strcmp( desired_word, *kwp ) == 0 )
		{
			return kwp - keyword_table;
		}
	}
	// Not found.
	return -1;
}

/***********************************************************
* Function:       // main
* Description:    // Ö÷º¯Êý
***********************************************************/
int main( void )
{
	int ret = -1;
	ret = lookup_keyword( "do", keyword, sizeof( keyword ) / sizeof( keyword[0] ) );
	printf( "lookup do ,ret=%d \n", ret );
	return 0;
}
/************************************** The End Of File **************************************/
