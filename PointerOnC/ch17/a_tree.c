/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// a_tree.c
 * Author:			//
 * Date:			//
 * Description:		// 二叉搜索树，静态数组实现
 * Version:			//
 * History:			//
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include "tree.h"
#include <assert.h>
#include <stdio.h>

#define TREE_SIZE	100             /* Max # of values in the tree */
#define ARRAY_SIZE	( TREE_SIZE + 1 )

static TREE_TYPE tree[ARRAY_SIZE];  //用于存储树的所有节点


/***********************************************************
* Function:       // left_child
* Description:    // 计算一个节点左孩子的下标
***********************************************************/
static int left_child( int current )
{
	return current * 2;
}

/***********************************************************
* Function:       // right_child
* Description:    // 计算一个节点右孩子的下标
***********************************************************/
static int right_child( int current )
{
	return current * 2 + 1;
}

/***********************************************************
* Function:       // insert
* Description:    // 插入节点
***********************************************************/
void insert( TREE_TYPE value )
{
	int current;


	/*
	** Ensure the value is nonzero, because zero indicates an
	** unused node.
	*/
	assert( value != 0 );


	/*
	** Start with the root node.
	*/
	current = 1;


	/*
	** Go to the proper subtree until we reach a leaf.
	*/
	while( tree[current] != 0 )
	{
		/*
		** Go to the left or right subtree, as appropriate.
		** (And make sure we don't have a duplicate value!)
		*/
		if( value < tree[current] )
		{
			current = left_child( current );
		} else
		{
			assert( value != tree[current] );
			current = right_child( current );
		}
		assert( current < ARRAY_SIZE );
	}

	tree[current] = value;
}

/***********************************************************
* Function:       // find
* Description:    // 查找
***********************************************************/
TREE_TYPE * find( TREE_TYPE value )
{
	int current;


	/*
	** Start with the root node.  Until we find the value,
	** go to the proper subtree.
	*/
	current = 1;

	while( current < ARRAY_SIZE && tree[current] != value )
	{
		/*
		** Go to the left or right subtree, as appropriate.
		*/
		if( value < tree[current] )
		{
			current = left_child( current );
		} else
		{
			current = right_child( current );
		}
	}

	if( current < ARRAY_SIZE )
	{
		return tree + current;
	} else
	{
		return 0;
	}
}
//打印当前节点值
void print_code(TREE_TYPE value)
{
	printf("%d ",value);
}
/*
** do_pre_order_traverse
** 执行一层前序遍历
**	Do one level of a pre-order traverse.  This helper function
**	is needed to save the information of which node we're
**	currently processing; this is not a part of the client's
**	interface.
*/
static void do_pre_order_traverse( int current, void ( *callback )( TREE_TYPE value ) )
{
	if( current < ARRAY_SIZE && tree[current] != 0 )
	{
		callback( tree[current] );
		do_pre_order_traverse( left_child( current ), callback );
		do_pre_order_traverse( right_child( current ), callback );
	}
}

/*
** pre_order_traverse
** 前序遍历二叉搜索树，插入节点顺序为前序遍历结果
*/
void pre_order_traverse( void ( *callback )( TREE_TYPE value ) )
{
	do_pre_order_traverse( 1, callback );
}
//中序遍历二叉搜索树，从小到大的顺序
static void do_in_order_traverse( int current, void ( *callback )( TREE_TYPE value ) )
{
	if( current < ARRAY_SIZE && tree[current] != 0 )
	{
		do_in_order_traverse( left_child( current ), callback );

		callback( tree[current] );
		do_in_order_traverse( right_child( current ), callback );
	}
}
void in_order_traverse( void ( *callback )( TREE_TYPE value ) )
{
	do_in_order_traverse( 1, callback );
}
/***********************************************************
* Function:       // main
* Description:    // 入口主函数
***********************************************************/
int main( void )
{
//	int i;
	TREE_TYPE a=1;
	TREE_TYPE b=2;
	TREE_TYPE c=3;
	TREE_TYPE d=4;

	insert( c );
	insert( a );
	insert( b );
	insert( d );
	/*			
	printf( "Tree array--------------------\n" );
	for( i=0;i < ARRAY_SIZE;i++)
	{
		printf("tree[%02d]=%d \n",i,tree[i]);
	}
	*/
	printf( "----------- Tree array pre_order_traverse() -------------\n" );

	pre_order_traverse(print_code);

	printf("\n");

	printf( "----------- Tree array in_order_traverse() -------------\n" );

	in_order_traverse(print_code);

	printf("\n");
	
	return 0;
}

/************************************** The End Of File **************************************/
