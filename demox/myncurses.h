/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// �ļ���
 * Author:			// ����
 * Date:			// ����
 * Description:		// ģ������
 * Version:			// �汾��Ϣ
 * History:			// ��ʷ�޸ļ�¼
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#ifndef __MY_NCURSES_H__
#define __MY_NCURSES_H__

#include <ncurses.h>

#define WIN_HEIGHT	2
#define WIN_WIDTH	79
#define	MAX_TRK_NUM_PERPAGE 24 //ÿҳͨ����

extern int									g_total_page_num; //��ҳ��
extern int									g_cur_page; //��ǰҳ
extern int									g_cur_trk; //��ǰͨ��,��Χ(0, TRK_NUM_PERPAGE)

typedef struct _win_border_struct {
	chtype ls, rs, ts, bs,
	       tl, tr, bl, br;
}WIN_BORDER;

typedef struct _WIN_struct {
	int			startx, starty;
	int			height, width;
	WIN_BORDER	border;
}WIN;

void init_win_params( WIN *p_win ,int height, int width, int starty,int startx);//��ʼ�����ڲ���


void print_win_params( WIN *p_win );//��ӡ��ǰ���ڲ���


void printmsg_in_win( WIN *p_win ,char *msg,int color);//�����д�ӡ��Ϣ


void create_box( WIN *win, bool flag );//��ʾ����

void mainWork(void);//���������

void print_curpage_trk( void );

void draw_main(TRUNK_STRUCT* pOneTrunk);

#endif
/************************************** The End Of File **************************************/

