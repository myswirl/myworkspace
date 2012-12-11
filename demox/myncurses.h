/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// 文件名
 * Author:			// 作者
 * Date:			// 日期
 * Description:		// 模块描述
 * Version:			// 版本信息
 * History:			// 历史修改记录
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#ifndef __MY_NCURSES_H__
#define __MY_NCURSES_H__

#include <ncurses.h>

#define WIN_HEIGHT	2
#define WIN_WIDTH	79
#define	MAX_TRK_NUM_PERPAGE 24 //每页通道数

extern int									g_total_page_num; //总页数
extern int									g_cur_page; //当前页
extern int									g_cur_trk; //当前通道,范围(0, TRK_NUM_PERPAGE)

typedef struct _win_border_struct {
	chtype ls, rs, ts, bs,
	       tl, tr, bl, br;
}WIN_BORDER;

typedef struct _WIN_struct {
	int			startx, starty;
	int			height, width;
	WIN_BORDER	border;
}WIN;

void init_win_params( WIN *p_win ,int height, int width, int starty,int startx);//初始化窗口参数


void print_win_params( WIN *p_win );//打印当前窗口参数


void printmsg_in_win( WIN *p_win ,char *msg,int color);//窗口中打印消息


void create_box( WIN *win, bool flag );//显示窗口

void mainWork(void);//程序主框架

void print_curpage_trk( void );

void draw_main(TRUNK_STRUCT* pOneTrunk);

#endif
/************************************** The End Of File **************************************/

