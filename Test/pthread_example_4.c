/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// 文件名
 * Author:			// 作者
 * Date:			// 日期
 * Description:		// 我们创建两个线程，各自把counter增加5000次，
 正常情况下最后counter应该等于10000，但事实上每次运行该程序的结果都不一样，
 有时候数到5000多，有时候数到6000多。 

 * Version:			// 版本信息
 * History:			// 历史修改记录
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define NLOOP 5000
int counter;     //incremented by threads
void *doit( void * );


/***********************************************************
* Function:       // main
***********************************************************/
int main( int argc, char **argv )
{
	pthread_t tidA, tidB;
	pthread_create( &tidA, NULL, &doit, NULL );
	pthread_create( &tidB, NULL, &doit, NULL );     /* wait for both threads to terminate */
	pthread_join( tidA, NULL );         pthread_join( tidB, NULL );          return 0;
}

/***********************************************************
* Function:       // doit()
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
		val = counter;
		printf( "%x: %d\n", (unsigned int)pthread_self( ), val + 1 );
		counter = val + 1;
	}
	return NULL;
}
/************************************** The End Of File **************************************/
