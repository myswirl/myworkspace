/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// search.c
 * Author:			// 作者
 * Date:			// 日期
 * Description:		// 模块描述
 * Version:			// 版本信息
 * History:			// 历史修改记录
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "node.h"

#define FALSE	0
#define TRUE	1


/***********************************************************
* Function:       // sll_insert
* Description:    // 插入到有序单链表
* Input:          // 1.linkp为指向链表第一个节点的指针
* Input:          // 2.new_value为要插入的新值
***********************************************************/
int sll_insert( register Node **linkp, int new_value )
{
	register Node	*current;
	register Node	*new;

	while( ( current = *linkp ) != NULL &&  current->value < new_value )
	{
		linkp = &current->link;
	}

	// 为新节点分配内存
	new = (Node *)malloc( sizeof( Node ) );
	if( new == NULL )
	{
		return FALSE;
	}
	new->value = new_value;

	// 在链表中插入新节点，并返回TRUE
	new->link  = current;
	*linkp	   = new;
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
* Function:       // compare_ints
* Description:    // 比较两个整形数据的函数
***********************************************************/
int compare_ints( void const *a, void const *b )
{
	if( *(int *)a == *(int *)b )
	{
		return 0;
	}else
	{
		return 1;
	}
}

/***********************************************************
* Function:       // search_list
* Description:    // 在一个单链表中查找指定的值的函数
* Input:          // 1.node为指向链表第一个节点的指针
* 				  // 2.value为指向我们需要查找的值的指针
* 			      // 3.compare为一个函数指针
***********************************************************/
Node * search_list( Node *node, void const *value,
                    int ( *compare )( void const *, void const * ) )
{
	while( node != NULL )
	{
		if( compare( &node->value, value ) == 0 )
		{
			break;
		}
		node = node->link;
	}
	return node;
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
	Node 	*desired_node;	//查找到的节点
	int		desired_value = 10;
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

	//指针函数，回调函数，查找int
	desired_node = search_list(root, &desired_value, compare_ints);
	printf("desired_node=%d\n",desired_node->value);
	return 0;
}

/************************************** The End Of File **************************************/
