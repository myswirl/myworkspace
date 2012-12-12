/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// l_stack.c
 * Author:			// 
 * Date:			// 
 * Description:		// 第三种堆栈实现方式，单链表，不带有头结点，每次push新建一个节点指向前一个节点。
 * Version:			// 
 * History:			// 
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>

#define FALSE 0


/*
**	Define a structure to hold one value.  The link field will
**	point to the next value on the stack.
*/
typedef struct STACK_NODE {
	STACK_TYPE			value;
	struct STACK_NODE	*next;
} StackNode;


/*
**	A pointer to the topmost node on the stack.
*/
static StackNode *stack;//只需声明栈头，插入和删除都在栈头


//	create_stack
void create_stack( size_t size )
{
}

//	destroy_stack
void destroy_stack( void )
{
	while( !is_empty( ) )
	{
		pop( );
	}
}

//	push
void push( STACK_TYPE value )
{
	StackNode *new_node;

	new_node = malloc( sizeof( StackNode ) );
	assert( new_node != NULL );
	new_node->value	   = value;
	new_node->next	   = stack;
	stack			   = new_node;
}

//	pop
void pop( void )
{
	StackNode *first_node;

	assert( !is_empty( ) );
	first_node = stack;
	stack	   = first_node->next;
	free( first_node );
}

//	top
STACK_TYPE top( void )
{
	assert( !is_empty( ) );
	return stack->value;
}

//	is_empty
int is_empty( void )
{
	return stack == NULL;
}

//	is_full
int is_full( void )
{
	return FALSE;//永远不会满
}

// 主函数
int main( void )
{
	STACK_TYPE	a  = 1;
	STACK_TYPE	b  = 2;
	STACK_TYPE	c  = 3;

	push( a );
	push( b );
	push( c );

	printf( "top:%d \n", top( ) );
	pop( );

	printf( "top:%d \n", top( ) );
	pop( );

	printf( "top:%d \n", top( ) );
	pop( );

	destroy_stack( );

	return 0;
}
/************************************** The End Of File **************************************/
