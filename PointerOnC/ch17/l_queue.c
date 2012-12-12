/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// l_queue.c
 * Author:			//
 * Date:			//
 * Description:		// �����ַ�ʽʵ�ֶ��У�����������ͷ��㣬
 * Version:			//
 * History:			//
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include "queue.h"
#include <stdio.h>
#include <assert.h>

//��������нڵ�
typedef struct QUEUE_NODE {
	QUEUE_TYPE			value;
	struct QUEUE_NODE	*next;
} QueueNode;

static QueueNode	*pQueueFront   = NULL;  //��ͷָ��,��ͷɾ��
static QueueNode	*pQueueRear	   = NULL;  //��βָ��,��β����
static size_t		queue_size	   = 0;     //���д�С


/***********************************************************
* Function:       // create_queue
* Description:    // �������У����г�ʼ��
***********************************************************/
void create_queue( size_t size )
{
}

/***********************************************************
* Function:       // destroy_queue
* Description:    // ���ٶ���
***********************************************************/
void destroy_queue( void )
{
	while( !is_empty( ) )
	{
		delete( );
	}
}

/***********************************************************
* Function:       // insert
* Description:    // ���β����Ԫ��
***********************************************************/
void insert( QUEUE_TYPE value )
{
	QueueNode *new_node;
	new_node = malloc( sizeof( QueueNode ) );
	assert( new_node != NULL );
	new_node->value	   = value;
	new_node->next	   = NULL;

	if( is_empty( ) ) //����Ϊ��
	{
		pQueueFront	   = new_node;
		pQueueRear	   = new_node;
	}else
	{
		
		pQueueRear->next = new_node;
		pQueueRear = new_node;
	}
	queue_size++;
}

/***********************************************************
* Function:       // delete
* Description:    // ɾ����ͷԪ��
***********************************************************/
void delete( void )
{
	QueueNode *pTmpNode;
	assert( !is_empty( ) );

	pTmpNode	   = pQueueFront;
	pQueueFront	   = pQueueFront->next;

	free( pTmpNode );
	queue_size--;
}

/***********************************************************
* Function:       // first
* Description:    // ����ͷԪ��
***********************************************************/
QUEUE_TYPE first( void )
{
	assert( !is_empty( ) );
	return pQueueFront->value;
}

/***********************************************************
* Function:       // is_empty
* Description:    // �ж϶����Ƿ�Ϊ��
***********************************************************/
int is_empty( void )
{
	return queue_size == 0;
}

/***********************************************************
* Function:       // is_full
* Description:    // �ж������Ƿ����ˣ�������������������������ڴ�û����
***********************************************************/
int is_full( void )
{
	return 0; //��ԶΪ����
}

/***********************************************************
* Function:       // main
* Description:    // ������
***********************************************************/
int main( void )
{
	QUEUE_TYPE	a  = 1;
	QUEUE_TYPE	b  = 2;
	QUEUE_TYPE	c  = 3;
	QUEUE_TYPE	ret;

	insert( a );
	insert( b );
	insert( c );

	ret = first( );
	delete( );
	printf( "Queue:%d \n", ret );

	ret = first( );
	delete( );
	printf( "Queue:%d \n", ret );

	ret = first( );
	delete( );
	printf( "Queue:%d \n", ret );

	destroy_queue( );

	return 0;
}

/************************************** The End Of File **************************************/
