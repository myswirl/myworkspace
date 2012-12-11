/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// fifo_write.c
 * Author:			//
 * Date:			//
 * Description:		// IPC方式，有名管道，FIFO,用于同一系统中进程间通讯
 * Version:			//
 * History:			//
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define FIFO_SERVER "/tmp/myfifo"


/***********************************************************
* Function:       // main
***********************************************************/
int main( int argc, char** argv )
{
	int		fd;
	char	w_buf[100];
	int		nwrite;
	//打开管道
	fd = open( FIFO_SERVER, O_WRONLY | O_NONBLOCK, 0 );
	if( fd == -1 )
	{
		printf( "open() %s fail! \n", FIFO_SERVER );
		return -1;
	}
	if( argc == 1 )
	{
		printf( "please send something!\n" );
		return -1;
	}

	strcpy( w_buf, argv[1] );
	//向管道写数据
	if( ( nwrite = write( fd, w_buf, 100 ) ) == -1 )
	{
		if( errno == EAGAIN )
		{
			printf( "the FIFO has not been read yet, please try later\n" );
		}
	}else
	{
		printf( "write %s to FIFO\n", w_buf );
	}
	return 0;
}

/************************************** The End Of File **************************************/
