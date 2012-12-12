/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// a_queue.c
 * Author:			// 
 * Date:			// 
 * Description:		// �ڶ��ַ�ʽʵ�ֶ��У���̬���飬ʹ�ò���ȫ��������ļ����ж��պ���
 * Version:			// 
 * History:			// 
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include "queue.h"
#include <stdio.h>
#include <assert.h>

#define QUEUE_SIZE	100                 /* Max # of values on the queue */
#define ARRAY_SIZE	( QUEUE_SIZE + 1 )  /* Size of array */


/*
**	The array that holds the values on the queue, and pointers
**	to the front and rear of the queue.
*/
static QUEUE_TYPE	*queue;				//ջ��Ԫ��ָ��
static size_t		front  = 1;
static size_t		rear   = 0;

//	create queue
void create_queue( size_t size )
{
	if( 0 == size)
	{
		printf("Error, queue size is 0!");
		return;
	}
	queue	   = malloc( size * sizeof( QUEUE_TYPE ) );
	assert( queue != NULL );
}
//	destroy_queue(), ���ٶ��У�һ���ͷŸշ�����ڴ�
void destroy_queue( void )
{
	assert( !is_empty( ) );
	free( queue );
	queue = NULL;
	front  = 1;
	rear  = 0;
}
//	insert,������У��Ӷ�β����
void insert( QUEUE_TYPE value )
{
	assert( !is_full( ) );
	rear		   = ( rear + 1 ) % ARRAY_SIZE;
	queue[rear]	   = value;
}

//	delete �Ӷ�ͷɾ��
void delete( void )
{
	assert( !is_empty( ) );
	front = ( front + 1 ) % ARRAY_SIZE;
}

//	first����ͷԪ��
QUEUE_TYPE first( void )
{
	assert( !is_empty( ) );
	return queue[front];
}

//	is_empty
int is_empty( void )
{
	return ( rear + 1 ) % ARRAY_SIZE == front;
}

//	is_full
int is_full( void )
{
	return ( rear + 2 ) % ARRAY_SIZE == front;
}

int main(void)
{

	create_queue( ARRAY_SIZE );
	printf("create queue sucdess, ARRAY_SIZE:%d \n",ARRAY_SIZE);
	
	QUEUE_TYPE a=1;
	QUEUE_TYPE b=2;
	QUEUE_TYPE c=3;
	QUEUE_TYPE ret;

	insert(a);
	insert(b);
	insert(c);

	ret = first();
	delete();
	printf("������:%d \n",ret );

	ret = first();
	delete();
	printf("������:%d \n",ret );
	
	ret = first();
	delete();
	printf("������:%d \n",ret );

	destroy_queue();
	
	return 0;
}
/************************************** The End Of File **************************************/
