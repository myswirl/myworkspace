/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// �ļ���
 * Author:			// ����
 * Date:			// ����
 * Description:		// ģ������
 * Version:			// �汾��Ϣ
 * History:			// ��ʷ�޸ļ�¼
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
pthread_t ntid;


/***********************************************************
* Function:       // printids
* Description:    // ��ӡ�߳���Ϣ����
***********************************************************/
void printids( const char * s )
{
	pid_t		pid;
	pthread_t	tid;
	pid	   = getpid( );         //��õ�ǰ���̵�id
	tid	   = pthread_self( );   //��õ�ǰ�̵߳�id
	printf( "%s pid:%u, tid:%u (0x%x)\n",
	        s,
	        ( unsigned int )pid,
	        ( unsigned int )tid,
	        ( unsigned int )tid );
}

/***********************************************************
* Function:       // thr_fn
* Description:    // �̺߳���
***********************************************************/
void *thr_fn( void * arg )
{
	printids( arg );
	return NULL;
}

/***********************************************************
* Function:       // main
* Description:    // ������
***********************************************************/
int main( void )
{
	int err;
	err = pthread_create( &ntid, NULL, thr_fn, "new thread: " );
	if( err != 0 )
	{
		fprintf( stderr, "can't create thread: %s\n", strerror( err ) );
		exit( 1 );
	}
	printids( "main thread:" );
	sleep( 1 );//Ȩ��֮�ƣ���ʹ���̵߳ȴ�1�룬�ں�Ҳ��һ��������´������߳�ִ��
	return 0;
}

/************************************** The End Of File **************************************/
