/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// insert2.c
 * Author:			//
 * Date:			//
 * Description:		// 插入单链表，第二次尝试
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


/***********************************************************
* Function:       // sll_insert
* Description:    // 插入到单链表
* Input:          // 1.rootp:一个指向链表根指针的指针
* Input:          // 2.new_value:一个需要插入的新值
***********************************************************/
int sll_insert( Node **rootp, int new_value )
{
	Node	*current;
	Node	*previous;
	Node	*new;

	current	   = *rootp;
	previous   = NULL;

	while( current != NULL && current->value < new_value )
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

	new->link = current;
	if( previous == NULL )
	{
		*rootp = new;
	} else
	{
		previous->link = new;
	}
	return TRUE;
}

/***********************************************************
* Function:       // sll_print
* Description:    // 打印单链表数据
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
* Description:    // 主函数
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
	sll_insert( &root, 4 );
	printf( "--------after insert 4-----------\n" );
	sll_print( root );

	return 0;
}

/************************************** The End Of File **************************************/
