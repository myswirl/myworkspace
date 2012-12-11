/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// fork1.c
 * Author:			// 
 * Date:			// 
 * Description:		// fork������֮��������������һ�Σ�ȴ�������Σ������������ֲ�ͬ�ķ���ֵ
 					   1.�������У�fork�����´������ӽ��̵�PID;
 					   2.���ӽ����У�fork����0;
 					   3.������ִ���fork����һ����ֵ;
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
	//��ʱ����һ������
	pid = fork( );

	//��ʱ�Ѿ�������������ͬʱ����
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
