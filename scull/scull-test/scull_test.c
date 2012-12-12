/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// scull_test.c
 * Author:			// ����
 * Date:			// ����
 * Description:		// ģ������
 * Version:			// �汾��Ϣ
 * Function List:	// ��Ҫ�������书��
 *     1. -------
 * History:			// ��ʷ�޸ļ�¼
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>


/***********************************************************
* Function:       // main
* Description:    // �������ܡ����ܵȵ�����
* Input:          // �������˵��������ÿ�����������á�ȡֵ˵�����������ϵ
*       1.----
* Output:         // �����������˵��
*       1.----
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
int main( )
{
	char	buffer1[20]    = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
	char	buffer2[20]    = { 0 };
	int	sculltest;
	int	code, i;

	sculltest = open( "/dev/scull0", O_WRONLY ); //���豸scull0
	if( sculltest == -1)
	{
		printf( "Error,open /dev/scull0 ! sculltest=%d \n", sculltest );
		return 0;
	}
	for( i = 20; i > 0; i -= code )
	{
		if( ( code = write( sculltest, &buffer1[20 - i], i ) ) != i )
		{
			printf( "write error! code=%d \n", code );
		} else
		{
			printf( "write ok! code=%d \n", code );
		}
	}

	close( sculltest );

	sculltest = open( "/dev/scull0", O_RDONLY );

	for( i = 20; i > 0; i -= code )
	{
		if( ( code = read( sculltest, &buffer2[20 - i], i ) ) != i )
		{
			printf( "read error! code=%d \n", code );
		} else
		{
			printf( "read ok! code=%d \n", code );
		}
	}

	for( i = 0; i < 20; i++ )
	{
		printf( "[%2d]=%d\n", i, buffer2[i] );
	}

	printf( "\n" );

	close( sculltest );
	exit( 0 );
}

/************************************** The End Of File **************************************/

