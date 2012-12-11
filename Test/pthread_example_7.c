/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// 
 * Author:			// lmy
 * Date:			// 
 * Description:		// 
 * Version:			// 
 * History:			// 
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#define NUM 5
int		queue[NUM];
sem_t	blank_number, product_number;


/***********************************************************
* Function:       // producer
* Description:    // �������߳�
***********************************************************/
void *producer( void *arg )
{
	int p = 0;
	while( 1 )
	{
		sem_wait( &blank_number );
		queue[p] = rand( ) % 1000 + 1;
		printf( "Produce %d\n", queue[p] );
		sem_post( &product_number );
		p = ( p + 1 ) % NUM;
		sleep( rand( ) % 5 );
	}
}

/***********************************************************
* Function:       // consumer
* Description:    // �������߳�
***********************************************************/
void *consumer( void *arg )
{
	int c = 0;
	while( 1 )
	{
		sem_wait( &product_number );//�����Դ��ʹsemaphore��ֵ��1,�������sem_wait()ʱsemaphore��ֵ�Ѿ��� 0�������ȴ�
		printf( "Consume %d\n", queue[c] );
		queue[c] = 0;
		sem_post( &blank_number );//����sem_post()�����ͷ���Դ��ʹsemaphore��ֵ��1��ͬʱ���ѹ���ȴ����̡߳�
		c = ( c + 1 ) % NUM;
		sleep( rand( ) % 5 );
	}
}

/***********************************************************
* Function:       // main
***********************************************************/
int main( int argc, char *argv[] )
{
	pthread_t pid, cid;
	sem_init( &blank_number, 0, NUM );
	sem_init( &product_number, 0, 0 );
	pthread_create( &pid, NULL, producer, NULL );
	pthread_create( &cid, NULL, consumer, NULL );
	pthread_join( pid, NULL );
	pthread_join( cid, NULL );
	sem_destroy( &blank_number );
	sem_destroy( &product_number );
	return 0;
}

/************************************** The End Of File **************************************/
