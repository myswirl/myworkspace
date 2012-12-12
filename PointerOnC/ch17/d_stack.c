/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// d_stack.c
 * Author:			// 
 * Date:			// 
 * Description:		// �ڶ��ַ�ʽʵ�ֶ�ջ����̬��������
 *					��ջ�ĳ����ڴ�����ջ�ĺ���������ʱ�������ú�����������������֮ǰ���á�
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


/*
**	The array that holds the values on the stack, and a pointer
**	to the topmost value on the stack.
*/
static STACK_TYPE	*stack;				//ջ��Ԫ��ָ��
static size_t		stack_size;			//ָ����ջ��С
static int			top_element = -1;

//	create_stack
void create_stack( size_t size )
{
	if( 0 == size)
	{
		printf("Error, stack size is 0!");
		return;
	}
	stack	   = malloc( stack_size * sizeof( STACK_TYPE ) );
	assert( stack != NULL );
	stack_size = size;
}

//	destroy_stack,���ٶ�ջ�ͷŸշ�����ڴ�
void destroy_stack( void )
{
	assert( stack_size > 0 );
	stack_size = 0;
	free( stack );
	stack = NULL;
}

//	push
void push( STACK_TYPE value )
{
	assert( !is_full( ) );
	top_element		  += 1;
	stack[top_element] = value;
}

//	pop
void pop( void )
{
	assert( !is_empty( ) );
	top_element -= 1;
}

//	top
STACK_TYPE top( void )
{
	assert( !is_empty( ) );
	return stack[top_element];
}

//	is_empty
int is_empty( void )
{
	assert( stack_size > 0 );
	return top_element == -1;
}

//	is_full
int is_full( void )
{
	assert( stack_size > 0 );
	return top_element == stack_size - 1;
}

int main(void)
{
	STACK_TYPE a=1;
	STACK_TYPE b=2;
	STACK_TYPE c=3;

	create_stack( 100 );
	printf("create stack success, size is 100 \n");
	
	push(a);
	push(b);
	push(c);

	printf("top:%d \n",top());
	pop();

	printf("top:%d \n",top());
	pop();

	printf("top:%d \n",top());
	pop();

	destroy_stack();

	return 0;
}
/************************************** The End Of File **************************************/
