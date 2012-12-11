/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// pthread_example_3.c
 * Author:			// lmy
 * Date:			// 2011��6��5��21:07:16
 * Description:	
 *	���thread�߳�ͨ��return���أ�value_ptr��ָ��ĵ�Ԫ���ŵ���thread�̺߳����ķ���ֵ�� 
 *	���thread�̱߳�����̵߳���pthread_cancel�쳣��ֹ����value_ptr��ָ��ĵ�Ԫ���ŵ��ǳ���PTHREAD_CANCELED�� 
 *	���thread�߳����Լ�����pthread_exit��ֹ�ģ�value_ptr��ָ��ĵ�Ԫ��ŵ��Ǵ���pthread_exit�Ĳ�����
 * Version:			// 1.0
 * History:			// 
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


/***********************************************************
* Function:       // thr_fn1
* Description:    // �̺߳���1
***********************************************************/
void *thr_fn1( void *arg )
{
	printf( "thread 1 returning\n" );
	return (void *)1;
}

/***********************************************************
* Function:       // thr_fn2
* Description:    // �̺߳���2
***********************************************************/
void *thr_fn2( void *arg )
{
	printf( "thread 2 exiting\n" );
	pthread_exit( (void *)2 );
}

/***********************************************************
* Function:       // thr_fn3
* Description:    // �̺߳���3
***********************************************************/
void *thr_fn3( void *arg )
{
	while( 1 )
	{
		printf( "thread 3 writing\n" );
		sleep( 1 );
	}
}

/***********************************************************
* Function:       // main
* Description:    // ������
***********************************************************/
int main( void )
{
	pthread_t	tid;
	void		*tret;
	pthread_create( &tid, NULL, thr_fn1, NULL );
	pthread_join( tid, &tret );		//����Ϊdetach״̬���߳�һ����ֹ�����̻�����ռ�õ�������Դ������������ֹ״̬��
	printf( "thread 1 exit code %d\n", (int)tret );
	
	pthread_create( &tid, NULL, thr_fn2, NULL );
	pthread_join( tid, &tret );
	printf( "thread 2 exit code %d\n", (int)tret );

	pthread_create( &tid, NULL, thr_fn3, NULL );
	sleep( 3 );
	pthread_cancel( tid );
	pthread_join( tid, &tret );
	printf( "thread 3 exit code %d\n", (int)tret );

	return 0;
}

/************************************** The End Of File **************************************/
