/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// fork1.c
 * Author:			// 
 * Date:			// 
 * Description:		// fork的奇妙之处在于它被调用一次，却返回两次，它可能有三种不同的返回值
 					   1.父进程中，fork返回新创建的子进程的PID;
 					   2.在子进程中，fork返回0;
 					   3.如果出现错误，fork返回一个负值;
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
	//此时仅有一个进程
	pid = fork( );

	//此时已经有两个进程在同时运行
	if( pid < 0 )
	{
		printf( "error in fork!" );
	}else if( pid == 0 )
	{
		printf( "I am the child process, ID is %d \n", getpid( ) );
	}else
	{
		printf( "I am the parent process, ID is %d \n", getpid( ) );
	}
	return 0;
}
/************************************** The End Of File **************************************/
