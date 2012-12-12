/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// a_queue.c
 * Author:			// 
 * Date:			// 
 * Description:		// 第一种方式实现队列，静态数组，使用不完全填满数组的技巧判读空和满
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
static QUEUE_TYPE	queue[ARRAY_SIZE];
static size_t		front  = 1;
static size_t		rear   = 0;


//	insert,插入队列，从队尾插入
void insert( QUEUE_TYPE value )
{
	assert( !is_full( ) );
	rear		   = ( rear + 1 ) % ARRAY_SIZE;
	queue[rear]	   = value;
}

//	delete 从队头删除
void delete( void )
{
	assert( !is_empty( ) );
	front = ( front + 1 ) % ARRAY_SIZE;
}

//	first，队头元素
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
	QUEUE_TYPE a=1;
	QUEUE_TYPE b=2;
	QUEUE_TYPE c=3;
	QUEUE_TYPE ret;

	insert(a);
	insert(b);
	insert(c);

	ret = first();
	delete();
	printf("出队列:%d \n",ret );

	ret = first();
	delete();
	printf("出队列:%d \n",ret );
	
	ret = first();
	delete();
	printf("出队列:%d \n",ret );
	
	return 0;
}
/************************************** The End Of File **************************************/
