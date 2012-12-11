/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// shareMemroy.c
 * Author:			//
 * Date:			//
 * Description:		// IPC��ʽ�������ڴ�,���̼乲�����ݵ�һ�����ķ�����
 * Version:			//
 * History:			//
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

#define PERM				S_IRUSR | S_IWUSR
#define MAX_SHAREMEMROY_LEN 1024 //�����ڴ�ռ��С


/***********************************************************
* Function:       // main
***********************************************************/
int main( int argc, char ** argv )
{
	int		shmid;
	char	*parent_addr, *child_addr;
	if( argc != 2 )
	{
		fprintf( stderr, "Usage:%s str\n\a", argv[0] );
		return -1;
	}
	//���������ڴ�
	if( ( shmid = shmget( IPC_PRIVATE, MAX_SHAREMEMROY_LEN, PERM ) ) == -1 )
	{
		fprintf( stderr, "Creat Share Memory Error:%s\n\a", strerror( errno ) );
		return -1;
	}
	//�����ӽ���
	if( fork( ) ) //������
	{
		parent_addr = shmat( shmid, 0, 0 );
		memset( parent_addr, 0, MAX_SHAREMEMROY_LEN );
		strncpy( parent_addr, argv[1], MAX_SHAREMEMROY_LEN );
		printf( "Parent process, PID:%d, init share memory data: %s\n", getpid( ), parent_addr );
		wait( NULL );
		exit( 0 );
	}else
	{
		sleep( 1 );
		child_addr = shmat( shmid, 0, 0 );
		printf( "Child Process, PID:%d, get share memory data: %s\n", getpid( ), child_addr );
		exit( 0 );
	}
}

/************************************** The End Of File **************************************/
