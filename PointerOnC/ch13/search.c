/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// search.c
 * Author:			// ����
 * Date:			// ����
 * Description:		// ģ������
 * Version:			// �汾��Ϣ
 * History:			// ��ʷ�޸ļ�¼
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
* Function:       // compare_ints
* Description:    // �Ƚ������������ݵĺ���
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
* Description:    // ��һ���������в���ָ����ֵ�ĺ���
* Input:          // 1.nodeΪָ�������һ���ڵ��ָ��
* 				  // 2.valueΪָ��������Ҫ���ҵ�ֵ��ָ��
* 			      // 3.compareΪһ������ָ��
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
* Description:    // ������
***********************************************************/
int main( )
{
	printf( "*********insert1.c , single linked list ***********\n" );
	Node	* root;
	Node	a, b, c;
	Node 	*desired_node;	//���ҵ��Ľڵ�
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

	//ָ�뺯�����ص�����������int
	desired_node = search_list(root, &desired_value, compare_ints);
	printf("desired_node=%d\n",desired_node->value);
	return 0;
}

/************************************** The End Of File **************************************/
