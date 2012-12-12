/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// 文件名
 * Author:			// 作者
 * Date:			// 日期
 * Description:		// 模块描述
 * Version:			// 版本信息
 * History:			// 历史修改记录
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include "scull.h"


/***********************************************************
* Function:       // 函数名称
* Description:    // 函数功能、性能等的描述
* Input:          // 1.输入参数1，说明，包括每个参数的作用、取值说明及参数间关系
* Input:          // 2.输入参数2，说明，包括每个参数的作用、取值说明及参数间关系
* Output:         // 1.输出参数1，说明
* Return:         // 函数返回值的说明
* Others:         // 其它说明
***********************************************************/
int main( )
{
	char	buffer1[20]	   = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
	char	buffer2[20]	   = { 0 };
	int		sculltest;
	int		code, i;

	if( ( sculltest = open( "/dev/scull0", O_RDWR ) ) < 0 )
	{
		printf( "open error! \n" );
		exit( 1 );
	}
	printf( "open /dev/scull0 OK.\n" );

	code = 10;
	if( ioctl( sculltest, SCULL_IOCSQUANTUM, &code ) < 0 )
	{
		printf( "ioctl SCULL_IOCSQUANTUM error! \n" );
		exit( 1 );
	}

	printf( "SCULL_IOCSQUANTUM-SCULL_IOCQQUANTUM : scull_quantum=%d \n", ioctl( sculltest, SCULL_IOCQQUANTUM, NULL ) );

	if( ioctl( sculltest, SCULL_IOCTQUANTUM, 6 ) < 0 )
	{
		printf( "ioctl SCULL_IOCTQUANTUM error! \n" );
		exit( 1 );
	}

	if( ioctl( sculltest, SCULL_IOCGQUANTUM, &code ) < 0 )
	{
		printf( "ioctl SCULL_IOCGQUANTUM error! \n" );
		exit( 1 );
	}
	printf( "SCULL_IOCTQUANTUM-SCULL_IOCGQUANTUM : scull_quantum=%d \n", code );

	code = 10;

	if( ioctl( sculltest, SCULL_IOCXQUANTUM, &code ) < 0 )
	{
		printf( "ioctl SCULL_IOCXQUANTUM error! \n" );
		exit( 1 );
	}

	printf( "SCULL_IOCXQUANTUM : scull_quantum=%d --> %d\n", code, ioctl( sculltest, SCULL_IOCQQUANTUM, NULL ) );

	printf( "SCULL_IOCHQUANTUM : scull_quantum=%d --> %d\n", ioctl( sculltest, SCULL_IOCHQUANTUM, 6 ), ioctl( sculltest, SCULL_IOCQQUANTUM, NULL ) );


	code = 2;
	if( ioctl( sculltest, SCULL_IOCSQSET, &code ) < 0 )
	{
		printf( "ioctl SCULL_IOCSQQSET error! \n" );
		exit( 1 );
	}

	printf( "SCULL_IOCSQSET-SCULL_IOCQQSET : scull_qset=%d \n", ioctl( sculltest, SCULL_IOCQQSET, NULL ) );

	if( ioctl( sculltest, SCULL_IOCTQSET, 4 ) < 0 )
	{
		printf( "ioctl SCULL_IOCTQSET error! \n" );
		exit( 1 );
	}

	if( ioctl( sculltest, SCULL_IOCGQSET, &code ) < 0 )
	{
		printf( "ioctl SCULL_IOCGQSET error! \n" );
		exit( 1 );
	}
	printf( "SCULL_IOCTQSET-SCULL_IOCGQSET : scull_qset=%d \n", code );

	code = 2;

	if( ioctl( sculltest, SCULL_IOCXQSET, &code ) < 0 )
	{
		printf( "ioctl SCULL_IOCXQSET error! \n" );
		exit( 1 );
	}

	printf( "SCULL_IOCXQSET : scull_qset=%d --> %d\n", code, ioctl( sculltest, SCULL_IOCQQSET, NULL ) );
	printf( "SCULL_IOCHQSET : scull_qset=%d --> %d\n", ioctl( sculltest, SCULL_IOCHQSET, 4 ), ioctl( sculltest, SCULL_IOCQQSET, NULL ) );
	printf( "before reset : scull_quantum=%d  scull_qset=%d \n", ioctl( sculltest, SCULL_IOCQQUANTUM, NULL ), ioctl( sculltest, SCULL_IOCQQSET, NULL ) );
	close( sculltest );
	printf( "close scull ! \n" );

	if( ( sculltest = open( "/dev/scull0", O_RDWR ) ) < 0 )
	{
		printf( "open error! \n" );
		exit( 1 );
	}
	printf( "reopen /dev/scull0 OK. \n" );

	printf( "reopen : scull_quantum=%d  scull_qset=%d \n", ioctl( sculltest, SCULL_IOCQQUANTUM, NULL ), ioctl( sculltest, SCULL_IOCQQSET, NULL ) );

	for( i = 20; i > 0; i -= code )
	{
		if( ( code = write( sculltest, &buffer1[20 - i], i ) ) != i )
		{
			printf( "write error! code=%d i=%d \n", code, i );
		} else
		{
			printf( "write ok! code=%d \n", code );
		}
	}

	if( ( code = lseek( sculltest, 0, SEEK_SET ) ) != 0 )
	{
		printf( "llseek error! code=%d \n", code );
	}
	printf( "lseek scull  SEEK_SET-->0 ! \n" );

	for( i = 20; i > 0; i -= code )
	{
		if( ( code = read( sculltest, &buffer2[20 - i], i ) ) != i )
		{
			printf( "read error! code=%d i=%d \n", code, i );
		} else
		{
			printf( "read ok! code=%d \n", code );
		}
	}

	for( i = 0; i < 20; i += 5 )
	{
		printf( "[%d]=%d [%d]=%d [%d]=%d [%d]=%d [%d]=%d\n", i, buffer2[i], i + 1, buffer2[i + 1], i + 2, buffer2[i + 2], i + 3, buffer2[i + 3], i + 4, buffer2[i + 4] );
	}

//#if 0
/**********************************tekkaman************************************/
	if( ioctl( sculltest, SCULL_IOCRESET, NULL ) < 0 )
	{
		printf( "ioctl SCULL_IOCRESET error! \n" );
		exit( 1 );
	}
	printf( "SCULL_IOCRESET \n" );

	printf( "after reset : scull_quantum=%d  scull_qset=%d \n", ioctl( sculltest, SCULL_IOCQQUANTUM, NULL ), ioctl( sculltest, SCULL_IOCQQSET, NULL ) );

	close( sculltest );
	printf( "close scull ! \n" );
	if( ( sculltest = open( "/dev/scull0", O_RDWR ) ) < 0 )
	{
		printf( "open error! \n" );
		exit( 1 );
	}
	printf( "reopen scull ! \n" );

	for( i = 20; i > 0; i -= code )
	{
		if( ( code = write( sculltest, &buffer1[20 - i], i ) ) != i )
		{
			printf( "write error! code=%d i=%d \n", code, i );
		} else
		{
			printf( "write ok! code=%d \n", code );
		}
	}

	if( ( code = lseek( sculltest, -10, SEEK_CUR ) ) != 10 )
	{
		printf( "llseek error! code=%d \n", code );
	}
	printf( "lseek scull  SEEK_CUR-10-->10 ! \n" );

	for( i = 10; i > 0; i -= code )
	{
		if( ( code = read( sculltest, &buffer2[10 - i], i ) ) != i )
		{
			printf( "read error! code=%d i=%d \n", code, i );
		} else
		{
			printf( "read ok! code=%d \n", code );
		}
	}

	for( i = 0; i < 10; i += 5 )
	{
		printf( "[%d]=%d [%d]=%d [%d]=%d [%d]=%d [%d]=%d\n", i, buffer2[i], i + 1, buffer2[i + 1], i + 2, buffer2[i + 2], i + 3, buffer2[i + 3], i + 4, buffer2[i + 4] );
	}

	if( ( code = lseek( sculltest, -20, SEEK_END ) ) != 0 )
	{
		printf( "llseek error! code=%d \n", code );
	}
	printf( "lseek scull  SEEK_END-20-->0 ! \n" );

	for( i = 20; i > 0; i -= code )
	{
		if( ( code = read( sculltest, &buffer2[20 - i], i ) ) != i )
		{
			printf( "read error! code=%d i=%d \n", code, i );
		} else
		{
			printf( "read ok! code=%d \n", code );
		}
	}

	for( i = 0; i < 20; i += 5 )
	{
		printf( "[%d]=%d [%d]=%d [%d]=%d [%d]=%d [%d]=%d\n", i, buffer2[i], i + 1, buffer2[i + 1], i + 2, buffer2[i + 2], i + 3, buffer2[i + 3], i + 4, buffer2[i + 4] );
	}
	close( sculltest );
	printf( "close scull ! \n" );

	printf( "\n" );
	exit( 0 );
}
/************************************** The End Of File **************************************/
