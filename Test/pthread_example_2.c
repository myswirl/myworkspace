/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// 文件名
 * Author:			// lmy
 * Date:			// 2011年6月5日20:08:02
 * Description:		// 模块描述
 * Version:			// 版本信息
 * History:			// 历史修改记录
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


/***********************************************************
* Function:       // thread
* Description:    // 线程函数
***********************************************************/
void thread( void )
{
	int i;
	for( i = 0; i < 3; i++ )
	{
		printf( "This is a pthread.\n" );
	}
}

/***********************************************************
* Function:       // main
* Description:    // 主函数
***********************************************************/
int main( void )
{
	pthread_t	id;
	int			i, ret;
	ret = pthread_create( &id, NULL, (void *)thread, NULL );
	if( ret != 0 )
	{
		printf( "Create pthread error! \n" );
		exit( 1 );
	}
	for( i = 0; i < 3; i++ )
	{
		printf( "This is the main process.\n" );
	}
	pthread_join( id, NULL );
	return ( 0 );
}
/************************************** The End Of File **************************************/
