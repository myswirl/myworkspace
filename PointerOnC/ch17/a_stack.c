/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// a_stack.c
 * Author:			//
 * Date:			//
 * Description:		// ��ջ����һ��ʵ�ַ�ʽ����̬����
 * Version:			//
 * History:			//
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <stdio.h>
#include <assert.h>
#include "stack.h"

#define STACK_SIZE 100                  // ��ջֵ�������������

//����������, ����Ϊstatic,��ֹ�û�ʹ��Ԥ����ӿ�֮����κεط����ʶ�ջ�е�ֵ��
static STACK_TYPE	stack[STACK_SIZE];  //�洢��ջ��ֵ������
static int			top_element = -1;	//ջ��Ԫ�ص��±�ֵ

// ��ջ
void push( STACK_TYPE value )
{
	assert( !is_full( ) );
	top_element		  += 1;
	stack[top_element] = value;
}

// ��ջ
void pop( void )
{
	assert( !is_empty( ) );
	top_element -= 1;
}

// ����ջ��Ԫ��
STACK_TYPE top( void )
{
	assert( !is_empty( ) );
	return stack[top_element];
}

// �ж϶�ջ�Ƿ�Ϊ��
int is_empty( void )
{
	return top_element == -1;
}

// �ж϶�ջ�Ƿ��Ѿ�����
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
