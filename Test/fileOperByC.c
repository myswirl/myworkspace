/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// fileOperByC.c
 * Author:			// lmy
 * Date:			// 2011��5��27��
 * Description:		// Linux��C�����ļ���������
 * Version:			// 
 * History:			// 
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <stdio.h>
#define LENGTH 100


/***********************************************************
* Function:       // main
* Description:    // ����������
***********************************************************/
int main( )
{
	FILE	*fd;
	char	str[LENGTH];

	fd = fopen( "hello.txt", "w+" );
	if( fd )
	{
		fputs( "Hello World", fd );
		fclose( fd );
	}
	fd = fopen( "hello.txt", "r" );
	fgets( str, LENGTH, fd );
	printf( "%s\n", str );
	fclose( fd );
	return 0;
}
/************************************** The End Of File **************************************/
