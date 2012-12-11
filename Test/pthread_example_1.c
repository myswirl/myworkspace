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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
pthread_t ntid;


/***********************************************************
* Function:       // printids
* Description:    // 打印线程信息函数
***********************************************************/
void printids( const char * s )
{
	pid_t		pid;
	pthread_t	tid;
	pid	   = getpid( );         //获得当前进程的id
	tid	   = pthread_self( );   //获得当前线程的id
	printf( "%s pid:%u, tid:%u (0x%x)\n",
	        s,
	        ( unsigned int )pid,
	        ( unsigned int )tid,
	        ( unsigned int )tid );
}

/***********************************************************
* Function:       // thr_fn
* Description:    // 线程函数
***********************************************************/
void *thr_fn( void * arg )
{
	printids( arg );
	return NULL;
}

/***********************************************************
* Function:       // main
* Description:    // 主函数
***********************************************************/
int main( void )
{
	int err;
	err = pthread_create( &ntid, NULL, thr_fn, "new thread: " );
	if( err != 0 )
	{
		fprintf( stderr, "can't create thread: %s\n", strerror( err ) );
		exit( 1 );
	}
	printids( "main thread:" );
	sleep( 1 );//权宜之计，即使主线程等待1秒，内核也不一定会调度新创建的线程执行
	return 0;
}

/************************************** The End Of File **************************************/
