/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// pthread_example_3.c
 * Author:			// lmy
 * Date:			// 2011年6月5日21:07:16
 * Description:	
 *	如果thread线程通过return返回，value_ptr所指向的单元里存放的是thread线程函数的返回值。 
 *	如果thread线程被别的线程调用pthread_cancel异常终止掉，value_ptr所指向的单元里存放的是常数PTHREAD_CANCELED。 
 *	如果thread线程是自己调用pthread_exit终止的，value_ptr所指向的单元存放的是传给pthread_exit的参数。
 * Version:			// 1.0
 * History:			// 
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


/***********************************************************
* Function:       // thr_fn1
* Description:    // 线程函数1
***********************************************************/
void *thr_fn1( void *arg )
{
	printf( "thread 1 returning\n" );
	return (void *)1;
}

/***********************************************************
* Function:       // thr_fn2
* Description:    // 线程函数2
***********************************************************/
void *thr_fn2( void *arg )
{
	printf( "thread 2 exiting\n" );
	pthread_exit( (void *)2 );
}

/***********************************************************
* Function:       // thr_fn3
* Description:    // 线程函数3
***********************************************************/
void *thr_fn3( void *arg )
{
	while( 1 )
	{
		printf( "thread 3 writing\n" );
		sleep( 1 );
	}
}

/***********************************************************
* Function:       // main
* Description:    // 主函数
***********************************************************/
int main( void )
{
	pthread_t	tid;
	void		*tret;
	pthread_create( &tid, NULL, thr_fn1, NULL );
	pthread_join( tid, &tret );		//被置为detach状态的线程一旦终止就立刻回收它占用的所有资源，而不保留终止状态。
	printf( "thread 1 exit code %d\n", (int)tret );
	
	pthread_create( &tid, NULL, thr_fn2, NULL );
	pthread_join( tid, &tret );
	printf( "thread 2 exit code %d\n", (int)tret );

	pthread_create( &tid, NULL, thr_fn3, NULL );
	sleep( 3 );
	pthread_cancel( tid );
	pthread_join( tid, &tret );
	printf( "thread 3 exit code %d\n", (int)tret );

	return 0;
}

/************************************** The End Of File **************************************/
