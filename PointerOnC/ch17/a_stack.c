/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// a_stack.c
 * Author:			//
 * Date:			//
 * Description:		// 堆栈，第一种实现方式，静态数组
 * Version:			//
 * History:			//
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <stdio.h>
#include <assert.h>
#include "stack.h"

#define STACK_SIZE 100                  // 堆栈值数量的最大限制

//作用是隐藏, 声明为static,防止用户使用预定义接口之外的任何地方访问堆栈中的值。
static STACK_TYPE	stack[STACK_SIZE];  //存储堆栈中值的数组
static int			top_element = -1;	//栈顶元素的下标值

// 入栈
void push( STACK_TYPE value )
{
	assert( !is_full( ) );
	top_element		  += 1;
	stack[top_element] = value;
}

// 出栈
void pop( void )
{
	assert( !is_empty( ) );
	top_element -= 1;
}

// 访问栈顶元素
STACK_TYPE top( void )
{
	assert( !is_empty( ) );
	return stack[top_element];
}

// 判断堆栈是否为空
int is_empty( void )
{
	return top_element == -1;
}

// 判断堆栈是否已经满了
int is_full( void )
{
	return top_element == STACK_SIZE - 1;
}

int main(void)
{
	STACK_TYPE a=1;
	STACK_TYPE b=2;
	STACK_TYPE c=3;
	push(a);
	push(b);
	push(c);

	printf("top:%d \n",top());
	pop();

	printf("top:%d \n",top());
	pop();

	printf("top:%d \n",top());
	pop();

	pop();

	return 0;
}
/************************************** The End Of File **************************************/
