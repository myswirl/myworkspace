/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// MemoryAssign.c
 * Author:			//
 * Date:			//
 * Description:		// ��ʾC�����ڴ��������������:���ݶΡ�����Ρ�BSS�Ρ�ջ����
 * Version:			//
 * History:			//
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int	g_A	   = 10;            //�����
int			g_B	   = 20;            //���ݶ�
static int	g_C	   = 30;            //���ݶ�
static int	g_D;                    //BSS��
int			g_E;                    //BSS��
char		*p1;					//BSS��


/***********************************************************
* Function:       // main
***********************************************************/
int main( void )
{
	int			local_A;            //ջ
	static int	local_C = 0;        //���ݶ�
	static int	local_D;	        //���ݶ�
	
	char		*p3 = "123456";     //123456�ڴ���Σ�p3��ջ��

	p1 = (char *)malloc( 10 );      //�ѣ����������10�ֽڵ������ڶ���
	strcpy( p1, "123456" );         //123456{post.content}���ڳ����������������ܻὫ����p3��ָ�� ��"123456"�Ż���һ��
	printf("\n");
	printf( "�����,ȫ�ֳ�ʼ������, ֻ��const,  g_A,     addr:%p\n", &g_A);
	printf("\n");
	printf( "���ݶ�,ȫ�ֱ���,       ��ʼ��      g_B,     addr:%p\n", &g_B);
	printf( "���ݶ�,��̬ȫ�ֱ���,   ��ʼ��,     g_C,     addr:%p\n", &g_C);
	printf("\n");
	printf( "BSS��, ȫ�ֱ���,       δ��ʼ��    g_E,     addr:%p\n", &g_E );	
	printf( "BSS��, ��̬ȫ�ֱ���,   δ��ʼ��,   g_D,     addr:%p\n", &g_D );
	printf( "BSS��, ��̬�ֲ�����,   ��ʼ��,     local_C, addr:%p\n", &local_C);
	printf( "BSS��, ��̬�ֲ�����,   δ��ʼ��,   local_D, addr:%p\n", &local_D);
	printf("\n");
	printf( "ջ,    �ֲ�����,                   local_A, addr:%p\n", &local_A );
	printf("\n");
	printf( "��,    malloc�����ڴ�,             p1,      addr:%p\n", p1 );

	return 0;
}

/************************************** The End Of File **************************************/
