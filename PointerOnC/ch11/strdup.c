/*
** Make a copy of a string in dynamically allocated memory.  Note: 
** caller is responsible for checking whether the memory was
** allocated!  This allows the caller to respond to an error in
** any way they wish.
** ��̬�����ڴ棬����һ���ַ�����һ�ݿ�����ע���鷵��ֵ��
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * strdup( char const *string )
{
	char	*new_string;

	/*
	** Ask for enough memory to hold the string and its
	** terminating NUL byte.
	*/
	new_string = malloc( strlen( string ) + 1 );

	/*
	** If we got the memory, copy the string.
	*/
	if( new_string != NULL )
		strcpy( new_string, string );

	return new_string;
}

int main(void)
{
	char *message = "hello world!";
	if ( strdup(message) == NULL)
	{
		printf("Out of Memory \n");
	}else
	{
		printf("%s \n", strdup(message) );
	}
	return 0;
}

