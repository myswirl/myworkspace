/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// l_queue.c
 * Author:			//
 * Date:			//
 * Description:		// 第三种方式实现队列，单链表，不带头结点，
 * Version:			//
 * History:			//
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include "queue.h"
#include <stdio.h>
#include <assert.h>

//单链表队列节点
typedef struct QUEUE_NODE {
	QUEUE_TYPE			value;
	struct QUEUE_NODE	*next;
} QueueNode;

static QueueNode	*pQueueFront   = NULL;  //队头指针,队头删除
static QueueNode	*pQueueRear	   = NULL;  //队尾指针,队尾插入
static size_t		queue_size	   = 0;     //队列大小


/***********************************************************
* Function:       // create_queue
* Description:    // 创建队列，队列初始化
***********************************************************/
void create_queue( size_t size )
{
}

/***********************************************************
* Function:       // destroy_queue
* Description:    // 销毁队列
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
* Description:    // 向队尾插入元素
***********************************************************/
void insert( QUEUE_TYPE value )
{
	QueueNode *new_node;
	new_node = malloc( sizeof( QueueNode ) );
	assert( new_node != NULL );
	new_node->value	   = value;
	new_node->next	   = NULL;

	if( is_empty( ) ) //队列为空
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
* Description:    // 删除队头元素
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
* Description:    // 队列头元素
***********************************************************/
QUEUE_TYPE first( void )
{
	assert( !is_empty( ) );
	return pQueueFront->value;
}

/***********************************************************
* Function:       // is_empty
* Description:    // 判断队列是否为空
***********************************************************/
int is_empty( void )
{
	return queue_size == 0;
}

/***********************************************************
* Function:       // is_full
* Description:    // 判读队列是否满了，单链表不存在满的情况，除非内存没有了
***********************************************************/
int is_full( void )
{
	return 0; //永远为不满
}

/***********************************************************
* Function:       // main
* Description:    // 主函数
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
