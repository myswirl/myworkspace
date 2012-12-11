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
#include <stdlib.h>
#include <pthread.h>
#define NLOOP 5000
int counter;                // incremented by threads
//���Mutex�����Ǿ�̬����ģ�ȫ�ֱ�����static��������Ҳ�����ú궨��PTHREAD_MUTEX_INITIALIZER����ʼ����
//�൱����pthread_mutex_init��ʼ������attr����ΪNULL��
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;
void *doit( void * );


/***********************************************************
* Function:       // main
***********************************************************/
int main( int argc, char **argv )
{
	pthread_t tidA, tidB;
	pthread_create( &tidA, NULL, doit, NULL );
	pthread_create( &tidB, NULL, doit, NULL );     // wait for both threads to terminate
	pthread_join( tidA, NULL );
	pthread_join( tidB, NULL );
	return 0;
}

/***********************************************************
* Function:       // doit
* Description:    // �̺߳���
***********************************************************/
void *doit( void *vptr )
{
	int i, val;
	/*
	 * Each thread fetches, prints, and increments the counter NLOOP times.
	 * The value of the counter should increase monotonically.          */
	for( i = 0; i < NLOOP; i++ )
	{
		pthread_mutex_lock( &counter_mutex );//��ǰ�߳���Ҫ����ȴ�,֪����ȡcounter_mutex
		val = counter;
		printf( "%x: %d\n", (unsigned int)pthread_self( ), val + 1 );
		counter = val + 1;
		pthread_mutex_unlock( &counter_mutex );//�ͷ�Mutex
	}
	return NULL;
}
/************************************** The End Of File **************************************/
