/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// sll_node.h
 * Author:			// 
 * Date:			// 
 * Description:		// 单链表
 * Version:			// 
 * History:			// 
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/

//单链表节点定义，数据整型
typedef struct NODE {
	struct NODE *link;
	int			value;
} Node;
	
/************************************** The End Of File **************************************/
