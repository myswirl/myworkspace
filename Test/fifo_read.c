/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// fifo_read.c
 * Author:			//
 * Date:			//
 * Description:		// IPC��ʽ,�����ܵ�FIFO,����ͬһϵͳ�еĸ����̼�ͨѶ
 * Version:			//
 * History:			//
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define FIFO "/tmp/myfifo"


/***********************************************************
* Function:       // main
***********************************************************/
int main( int argc, char** argv )
{
	char	buf_r[100];
	int		fd;
	int		nread;
	int		readCounter=0;
	//�����ܵ�
	if( ( mkfifo( FIFO, O_CREAT | O_EXCL ) < 0 ) && ( errno != EEXIST ) )
	{
		printf( "cannot creat fifoserver \n" );
	}
	printf( "preparing for reading bytes...\n" );
	memset( buf_r, 0, sizeof( buf_r ) );
	//�򿪹ܵ�
	fd = open( FIFO, O_RDONLY | O_NONBLOCK, 0 );
	if( fd == -1 )
	{
		perror( "open" );
		return -1;
	}
	while( 1 )
	{
		memset( buf_r, 0, sizeof( buf_r ) );
		if( ( nread = read( fd, buf_r, 100 ) ) == -1 )
		{
			if( errno == EAGAIN )
			{
				printf( "no data yet\n" );
			}
		}
		readCounter++;
		printf( "%d times read from FIFO: %s \n", readCounter, buf_r );
		sleep( 1 );
	}
	pause( ); //��ͣ�ȴ��ź�,����ִ�в�����?
	unlink(FIFO);//ɾ���ļ�
	return 0;
}

/************************************** The End Of File **************************************/
