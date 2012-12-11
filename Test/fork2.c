/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// fork1.c
 * Author:			// 
 * Date:			// 
 * Description:		// fork创建子进程，共享代码，拷贝数据
 					子进程的数据空间、堆栈空间都会从父进程得到一个拷贝，而不是共享。
 * Version:			// 
 * History:			// 
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

/***********************************************************
* Function:       // main
***********************************************************/
int main( void )
{
	pid_t pid;
	int count = 0;
	//此时仅有一个进程
	pid = fork( );

	count++;
	
	//此时已经有两个进程在同时运行
	printf( "count = %d \n", count );
	
	return 0;
}
/************************************** The End Of File **************************************/
