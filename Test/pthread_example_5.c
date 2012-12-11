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
#include <stdlib.h>
#include <pthread.h>
#define NLOOP 5000
int counter;                // incremented by threads
//如果Mutex变量是静态分配的（全局变量或static变量），也可以用宏定义PTHREAD_MUTEX_INITIALIZER来初始化，
//相当于用pthread_mutex_init初始化并且attr参数为NULL。
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;
void *doit( void * );


/***********************************************************
* Function:       // main
***********************************************************/
int main( int argc, char **argv )
{
	pthread_t tidA, tidB;
	pthread_create( &tidA, NULL, doit, NULL );
	pthread_create( &tidB, NULL, doit, NULL );     // wait for both threads to terminate
	pthread_join( tidA, NULL );
	pthread_join( tidB, NULL );
	return 0;
}

/***********************************************************
* Function:       // doit
* Description:    // 线程函数
***********************************************************/
void *doit( void *vptr )
{
	int i, val;
	/*
	 * Each thread fetches, prints, and increments the counter NLOOP times.
	 * The value of the counter should increase monotonically.          */
	for( i = 0; i < NLOOP; i++ )
	{
		pthread_mutex_lock( &counter_mutex );//当前线程需要挂起等待,知道获取counter_mutex
		val = counter;
		printf( "%x: %d\n", (unsigned int)pthread_self( ), val + 1 );
		counter = val + 1;
		pthread_mutex_unlock( &counter_mutex );//释放Mutex
	}
	return NULL;
}
/************************************** The End Of File **************************************/
