/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// insert1.c
 * Author:			// 
 * Date:			// 
 * Description:		// �ڱ�ͷ�ͱ�β��������
 * Version:			// 
 * History:			// 
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <stdlib.h>
#include <stdio.h>
#include "sll_node.h"

#define FALSE	0
#define TRUE	1
//���뵽һ������ĵ�����currentΪָ�������һ���ڵ��ָ�룬new_value��Ҫ�����ֵ
int sll_insert( Node *current, int new_value )
{
	Node	*previous;
	Node	*new;

	while( current->value < new_value )
	{
		previous   = current;
		current	   = current->link;
	}

	new = (Node *)malloc( sizeof( Node ) );
	if( new == NULL )
	{
		return FALSE;
	}
	new->value = new_value;

	new->link	   = current;
	previous->link = new;
	return TRUE;
}

/***********************************************************
* Function:       // sll_print
* Description:    // ��ӡ����������
***********************************************************/
void sll_print( Node * root )
{
	int num = 0;
	if( root == NULL )
	{
		printf( "root is NULL !\n" );
		return;
	}
	while( root->link != NULL )
	{
		num++;
		printf( "%d:%d \n", num, root->value );
		root = root->link;
	}
	num++;
	printf( "%d:%d \n", num, root->value );
}

/***********************************************************
* Function:       // main
* Description:    // ������
***********************************************************/
int main( )
{
	printf( "*********insert1.c , single linked list ***********\n" );
	Node	* root;
	Node	a, b, c;
	root	   = &a;
	a.link	   = &b;
	a.value	   = 5;

	b.link	   = &c;
	b.value	   = 10;

	c.link	   = NULL;
	c.value	   = 15;

	sll_print( root );
	sll_insert( root, 4);
	printf("--------after insert 4-----------\n");
	sll_print( root );

	return 0;
}
/************************************** The End Of File **************************************/
