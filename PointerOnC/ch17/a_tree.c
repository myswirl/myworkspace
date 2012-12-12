/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// a_tree.c
 * Author:			//
 * Date:			//
 * Description:		// ��������������̬����ʵ��
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

static TREE_TYPE tree[ARRAY_SIZE];  //���ڴ洢�������нڵ�


/***********************************************************
* Function:       // left_child
* Description:    // ����һ���ڵ����ӵ��±�
***********************************************************/
static int left_child( int current )
{
	return current * 2;
}

/***********************************************************
* Function:       // right_child
* Description:    // ����һ���ڵ��Һ��ӵ��±�
***********************************************************/
static int right_child( int current )
{
	return current * 2 + 1;
}

/***********************************************************
* Function:       // insert
* Description:    // ����ڵ�
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
* Description:    // ����
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
//��ӡ��ǰ�ڵ�ֵ
void print_code(TREE_TYPE value)
{
	printf("%d ",value);
}
/*
** do_pre_order_traverse
** ִ��һ��ǰ�����
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
** ǰ���������������������ڵ�˳��Ϊǰ��������
*/
void pre_order_traverse( void ( *callback )( TREE_TYPE value ) )
{
	do_pre_order_traverse( 1, callback );
}
//���������������������С�����˳��
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
* Description:    // ���������
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
