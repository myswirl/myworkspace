/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// pipe_readtest.c
 * Author:			//
 * Date:			//
 * Description:		// IPC���ܵ��������ܵ�����������Ե��ϵ���̼�ͨ�š�
 * Version:			//
 * History:			//
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
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
	char	r_buf[100];
	char	w_buf[4];
	char	* p_wbuf;
	int		r_num;

	memset( r_buf, 0, sizeof( r_buf ) );
	memset( w_buf, 0, sizeof( r_buf ) );
	p_wbuf = w_buf;
	if( pipe( pipe_fd ) < 0 )
	{
		printf( "pipe create error\n" );
		return -1;
	}

	if( ( pid = fork( ) ) == 0 )    //�����ӽ���
	{
		printf( "sub process\n" );
		close( pipe_fd[1] );
		printf( "sub process, close fd[1] over, fd[1] is pipe write part\n" );
		sleep( 3 );                 //ȷ�������̹ر�д��
		r_num = read( pipe_fd[0], r_buf, 100 );
		printf( "sub prcess: read num is %d, the data read from the pipe is %d\n", r_num, atoi( r_buf ) );

		close( pipe_fd[0] );
		printf( "sub process, close fd[0] over, fd[0] is pipe read part\n" );
		printf( "sub process exit\n" );
		exit( 0 );                  //�����������˳�
	}else if( pid > 0 )             //���븸����
	{
		printf( "parent process\n" );
		close( pipe_fd[0] );        //�ӽ��̹رն���
		printf( "parent process, close fd[0] over, fd[0] is pipe read part\n" );
		strcpy( w_buf, "1234" );
		if( write( pipe_fd[1], w_buf, 4 ) != -1 )
		{
			printf( "parent process, parent write over\n" );
		}
		close( pipe_fd[1] );        //write
		printf( "parent process, parent close fd[1] over, fd[1] is pipe write part\n" );
		sleep( 10 );
	}
	printf( "parent process exit \n" );
	return 0;                       
}

/**************************************************
 * ������������
 * parent write over
 * parent close fd[1] over
 * read num is 4   the data read from the pipe is 111
 * ���ӽ��ۣ�
 * �ܵ�д�˹رպ�д������ݽ�һֱ���ڣ�ֱ������Ϊֹ.
 ****************************************************/
