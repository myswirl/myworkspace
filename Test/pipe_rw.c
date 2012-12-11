/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// pipe_rw.c
 * Author:			//
 * Date:			//
 * Description:		// IPC��ʽ,�����ܵ�pipe,����������Ե��ϵ����ͨѶ��
 * Version:			//
 * History:			//
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

/***********************************************************
* Function:       // main
***********************************************************/
int main( void )
{
	int		pipe_fd[2];
	pid_t	pid;
	char	buf_r[100];
	//char	* p_wbuf;
	int		r_num;
	memset( buf_r, 0, sizeof( buf_r ) );
	//�����ܵ�
	if( pipe( pipe_fd ) < 0 )
	{
		printf( "pipe create error\n" );
		return -1;
	}
	//�����ӽ���
	if( ( pid = fork( ) ) == 0 ) //�ӽ���
	{
		printf( "\n" );
		close( pipe_fd[1] );
		sleep( 2 );
		if( ( r_num = read( pipe_fd[0], buf_r, 100 ) ) > 0 )
		{
			printf( "%d numbers read from the pipe is %s \n", r_num, buf_r );
		}
		close( pipe_fd[0] );
		return 0;
	}else if( pid > 0 )//������
	{
		close( pipe_fd[0] );
		if( write( pipe_fd[1], "Hello", 5 ) != -1 )
		{
			printf( "parent write 1st Hello!\n" );
		}

		if( write( pipe_fd[1], "Hello", 5 ) != -1 )
		{
			printf( "parent write 2nd Hello!\n" );
		}
		close( pipe_fd[1] );
		sleep( 3 );
		waitpid( pid, NULL, 0 ); //�ȴ��ӽ��̽���
		return 0;
	}

	return 0;
}

/************************************** The End Of File **************************************/
