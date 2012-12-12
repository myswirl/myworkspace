/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// strcpy.c
 * Author:			// ����
 * Date:			// ����
 * Description:		// ģ������
 * Version:			// �汾��Ϣ
 * History:			// ��ʷ�޸ļ�¼
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <stdio.h>


/*
**  Copy the string contained in the second argument to the
** buffer specified by the first argument.
*/
void strcpy1( char *buffer, char const *string )
{
	/*
	** Copy characters until a NUL byte is copied.
	*/
	while( ( *buffer++ = *string++ ) != '\0' )
	{
		;
	}
}

/***********************************************************
* Function:       // main
* Description:    // ������
***********************************************************/
int main( void )
{
	char	des[10]	   = { 0 };
	char	source[10] = "my test";
	printf( "des:%s \nsource:%s\n", des, source );
	strcpy1( des, source );
	printf("--------------after strcpy()-----------\n");
	printf( "des:%s \nsource:%s\n", des, source );
	return 0;
}

/************************************** The End Of File **************************************/
