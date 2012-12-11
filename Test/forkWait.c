/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// forkWait.c
 * Author:			// 
 * Date:			// 
 * Description:		// ���̵ȴ�
 * Version:			// 
 * History:			// 
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>


/***********************************************************
* Function:       // main
***********************************************************/
int main( void )
{
	pid_t pc, pr;
	pc = fork( );
	if( pc == 0 )//�ӽ���
	{
		printf("This is child process with pid :%d \n",getpid() );
		sleep( 8 );
	}else if( pc > 0 )//������
	{
		pr = wait( NULL );
		printf("I catched a child process with pid :%d \n",pr );
	}

	return 0;
}
/************************************** The End Of File **************************************/
