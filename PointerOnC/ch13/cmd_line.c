/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// cmd_line.c
 * Author:			//
 * Date:			//
 * Description:		// �����в�������
 * Version:			//
 * History:			//
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <stdio.h>
#define TRUE 1


/***********************************************************
* Function:       // process_standard_input
* Description:    // �����׼���룬�����в����������ļ��� 
***********************************************************/
void process_standard_input( void )
{
	printf( "process_standard_input\n" );
}

/***********************************************************
* Function:       // process_file
* Description:    // �����ļ��������в��������ļ���
* Input:          // 1.file_name:�ļ���
***********************************************************/
void process_file( char *file_name )
{
	printf( "process_file=%s \n", file_name );
}

/*
**	Option flags, default initialization is FALSE.
*/
int option_a, option_b /* etc. */;


/***********************************************************
* Function:       // main
* Description:    // �������������в�������
***********************************************************/
int main( int argc, char **argv )
{
	/*
	** Process option arguments: skip to next argument, and
	** check that it begins with a dash.
	*/
	while( *++argv != NULL && **argv == '-' )
	{
		/*
		** Check the letter after the dash.
		*/
		switch( *++*argv )
		{
			case 'a':
				option_a = TRUE;
				break;

			case 'b':
				option_b = TRUE;
				break;

				/* etc. */
		}
	}


	/*
	** Process file name arguments
	*/
	if( *argv == NULL )
	{
		process_standard_input( );
	} else
	{
		do
		{
			process_file( *argv );
		}
		while( *++argv != NULL );
	}

	return 0;
}

/************************************** The End Of File **************************************/
