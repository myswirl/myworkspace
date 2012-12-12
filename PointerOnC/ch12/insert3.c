/*
** Insert into an ordered, singly linked list.  The arguments are
** a pointer to the first node in the list, and the value to
** insert.
*/
#include <stdlib.h>
#include <stdio.h>
#include "sll_node.h"

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
