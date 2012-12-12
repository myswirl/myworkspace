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
* Description:    // ���뵽��������
* Input:          // 1.linkpΪָ�������һ���ڵ��ָ��
* Input:          // 2.new_valueΪҪ�������ֵ
***********************************************************/
int sll_insert( register Node **linkp, int new_value )
{
	register Node	*current;
	register Node	*new;

	while( ( current = *linkp ) != NULL &&  current->value < new_value )
	{
		linkp = &current->link;
	}

	// Ϊ�½ڵ�����ڴ�
	new = (Node *)malloc( sizeof( Node ) );
	if( new == NULL )
	{
		return FALSE;
	}
	new->value = new_value;

	// �������в����½ڵ㣬������TRUE
	new->link  = current;
	*linkp	   = new;
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
	sll_insert( &root, 4 );
	printf( "--------after insert 4-----------\n" );
	sll_print( root );

	return 0;
}
/************************************** The End Of File **************************************/
