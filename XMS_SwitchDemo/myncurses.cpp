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
#include <unistd.h>

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"

#include "XMS_SwitchDemo_Sub.h"
#include "XMS_SwitchDemo_Event.h"
#include "XMS_SwitchDemo_String.h"

#include "myncurses.h"
#include "MyLog.h"

WIN									g_win_title; //标题窗口
WIN									g_win_head; //头窗口
WIN									g_win_count; //统计窗口
WIN									g_win_Trk[MAX_TRK_NUM_PERPAGE]; //16个通道窗口

int									g_total_page_num; //总页数
int									g_cur_page; //当前页
int									g_cur_trk; //当前通道,范围(0, TRK_NUM_PERPAGE)
int									TRK_NUM_PERPAGE = 8; //每页通道数

extern int							g_iTotalTrunk; //总通道数
extern TYPE_XMS_DSP_DEVICE_RES_DEMO AllDeviceRes[MAX_DSP_MODULE_NUMBER_OF_XMS];
extern TYPE_CHANNEL_MAP_TABLE		MapTable_Trunk[MAX_TRUNK_NUM_IN_THIS_DEMO];
extern CMyLog						gLog;
extern int							g_getdev;

extern int							g_iTotalTrunk;
extern int							g_iTotalTrunkOpened;
extern int							g_iTotalVoice;
extern int							g_iTotalVoiceOpened;
extern int							g_iTotalVoiceFree;
extern int							g_iTotalUser;
extern int							g_iTotalUserOpened;
extern int							g_iTotalModule;

/***********************************************************
* Function:       // 函数名称
* Description:    // 函数功能、性能等的描述
* Input:          // 1.输入参数1，说明，包括每个参数的作用、取值说明及参数间关系
* Input:          // 2.输入参数2，说明，包括每个参数的作用、取值说明及参数间关系
* Output:         // 1.输出参数1，说明
* Return:         // 函数返回值的说明
* Others:         // 其它说明
***********************************************************/
void init_win_params( WIN *p_win, int height, int width, int starty, int startx )
{
	p_win->height  = height;
	p_win->width   = width;
	p_win->starty  = starty; //( LINES - p_win->height ) / 2
	p_win->startx  = startx; //( COLS - p_win->width ) / 2

	p_win->border.ls   = '|';
	p_win->border.rs   = '|';
	p_win->border.ts   = '-';
	p_win->border.bs   = '-';
	p_win->border.tl   = '+';
	p_win->border.tr   = '+';
	p_win->border.bl   = '+';
	p_win->border.br   = '+';
}

/***********************************************************
* Function:       // 函数名称
* Description:    // 函数功能、性能等的描述
* Input:          // 1.输入参数1，说明，包括每个参数的作用、取值说明及参数间关系
* Input:          // 2.输入参数2，说明，包括每个参数的作用、取值说明及参数间关系
* Output:         // 1.输出参数1，说明
* Return:         // 函数返回值的说明
* Others:         // 其它说明
***********************************************************/
void print_win_params( WIN *p_win )
{
	attron( COLOR_PAIR( 1 ) );
	mvprintw( p_win->starty + 1, p_win->startx + 1, "x:%d y:%d width:%d height:%d LINES:%d COLS:%d ",
	          p_win->startx, p_win->starty, p_win->width, p_win->height, LINES, COLS );
	refresh( );
	attroff( COLOR_PAIR( 1 ) );
}

/***********************************************************
* Function:       // 函数名称
* Description:    // 函数功能、性能等的描述
* Input:          // 1.输入参数1，说明，包括每个参数的作用、取值说明及参数间关系
* Input:          // 2.输入参数2，说明，包括每个参数的作用、取值说明及参数间关系
* Output:         // 1.输出参数1，说明
* Return:         // 函数返回值的说明
* Others:         // 其它说明
***********************************************************/
void printmsg_in_win( WIN *p_win, char *msg, int color )
{
	int i;
	if( msg == NULL )
	{
		return;
	}
	for( i = 0; i < p_win->width - 1; ++i )
	{
		mvaddch( p_win->starty + 1, p_win->startx + 1 + i, ' ' );
	}
	attron( COLOR_PAIR( color ) );
	mvprintw( p_win->starty + 1, p_win->startx+1, " %s", msg );
	refresh( );
	attroff( COLOR_PAIR( color ) );
}

/***********************************************************
* Function:       // 函数名称
* Description:    // 函数功能、性能等的描述
* Input:          // 1.输入参数1，说明，包括每个参数的作用、取值说明及参数间关系
* Input:          // 2.输入参数2，说明，包括每个参数的作用、取值说明及参数间关系
* Output:         // 1.输出参数1，说明
* Return:         // 函数返回值的说明
* Others:         // 其它说明
***********************************************************/
void print_curpage_trk( void )
{
	int			i;
	int			modID;
	int			chnID;
	TRUNK_STRUCT* pOneTrunk = NULL;
	if( g_cur_page < 1 || g_cur_page > g_iTotalTrunk )
	{
		return;
	}
	for( i = 0; i < TRK_NUM_PERPAGE; i++ )
	{
		modID	   = MapTable_Trunk[( g_cur_page - 1 ) * TRK_NUM_PERPAGE + i].m_s8ModuleID;
		chnID	   = MapTable_Trunk[( g_cur_page - 1 ) * TRK_NUM_PERPAGE + i].m_s16ChannelID;
		pOneTrunk  = &AllDeviceRes[modID].pTrunk[chnID];
		draw_main( pOneTrunk );
	}
}

/***********************************************************
* Function:       // 函数名称
* Description:    // 函数功能、性能等的描述
* Input:          // 1.输入参数1，说明，包括每个参数的作用、取值说明及参数间关系
* Input:          // 2.输入参数2，说明，包括每个参数的作用、取值说明及参数间关系
* Output:         // 1.输出参数1，说明
* Return:         // 函数返回值的说明
* Others:         // 其它说明
***********************************************************/
void create_box( WIN *p_win, bool flag )
{
	int i, j;
	int x, y, w, h;

	x  = p_win->startx;
	y  = p_win->starty;
	w  = p_win->width;
	h  = p_win->height;

	if( flag == TRUE )
	{
		mvaddch( y, x, p_win->border.tl );
		mvaddch( y, x + w, p_win->border.tr );
		mvaddch( y + h, x, p_win->border.bl );
		mvaddch( y + h, x + w, p_win->border.br );
		mvhline( y, x + 1, p_win->border.ts, w - 1 );
		mvhline( y + h, x + 1, p_win->border.bs, w - 1 );
		mvvline( y + 1, x, p_win->border.ls, h - 1 );
		mvvline( y + 1, x + w, p_win->border.rs, h - 1 );
	}else
	{
		for( j = y; j <= y + h; ++j )
		{
			for( i = x; i <= x + w; ++i )
			{
				mvaddch( j, i, ' ' );
			}
		}
	}

	refresh( );
}

/***********************************************************
* Function:       //
* Description:    //
* Input:          //
* Input:          //
* Output:         //
* Return:         //
* Others:         //
***********************************************************/
void mainWork( void )
{
	int		i;
	int		ch;
	int		drawflag	   = 0;
	char	tmpStr[200]	   = { 0 };

	initscr( ); // Start curses mode
	start_color( ); // Start the color functionality
	cbreak( ); // Line buffering disabled, Pass on everty thing to me
	keypad( stdscr, TRUE ); // I need that nifty F1
	noecho( );
	init_pair( 1, COLOR_CYAN, COLOR_BLACK );
	init_pair( 2, COLOR_GREEN, COLOR_BLACK );
	init_pair( 3, COLOR_RED, COLOR_BLACK );

	// Initialize the window parameters
	init_win_params( &g_win_title, WIN_HEIGHT, WIN_WIDTH, 0, 0 );
	create_box( &g_win_title, TRUE );
	
	init_win_params( &g_win_count, WIN_HEIGHT, WIN_WIDTH, 2, 0 );
	create_box( &g_win_count, TRUE );

	init_win_params( &g_win_head, WIN_HEIGHT, WIN_WIDTH, 4, 0 );
	create_box( &g_win_head, TRUE );
	
	for( i = 0; i < TRK_NUM_PERPAGE; i++ )
	{
		init_win_params( &g_win_Trk[i], WIN_HEIGHT, WIN_WIDTH, 6 + i * 2, 0 );
		create_box( &g_win_Trk[i], TRUE );
	}

	sprintf( tmpStr, "%-3s %-8s %-7s %-12s %-9s %-7s %-7s %-7s %-7s", "ID", "Mod,Ch", "Type", "LineState", "State", "Called", "Calling", "DTMF", "VocInfo" );
	printmsg_in_win( &g_win_head, tmpStr, 3 );

	g_cur_page = 1; //当前页
	g_cur_trk  = 0; //当前通道,范围(0, TRK_NUM_PERPAGE)

	for(;; )
	{
		if( g_getdev == 0 )
		{
			usleep( 1000 );
			continue;
		}
		g_total_page_num = g_iTotalTrunk / TRK_NUM_PERPAGE; //总页数
		sprintf( tmpStr, "%-53s[X:%d,Y:%d] Page(%02d/%02d)", "SwitchDemo: 'q':Exit, 'Up & Down':Change page.", COLS, LINES, g_cur_page, g_total_page_num );
		printmsg_in_win( &g_win_title, tmpStr, 2 );

		sprintf( tmpStr, "Trk/Open:(%03d/%03d) User/Open:(%03d/%03d) Voc/Open:(%03d/%03d)    VocFree:%03d", 
			g_iTotalTrunk,g_iTotalTrunkOpened,g_iTotalUser,g_iTotalUserOpened,g_iTotalVoice,g_iTotalVoiceOpened,g_iTotalVoiceFree);
		printmsg_in_win( &g_win_count, tmpStr, 2 );

		if( drawflag == 1 )
		{
			print_curpage_trk( );
			g_cur_trk  = TRK_NUM_PERPAGE - 1;
			drawflag   = 0;
		}

		if( ( ch = getch( ) ) == ERR )
		{
			ch = 0;
			usleep( 1000 );
		}

		drawflag = 1;
		gLog.WriteLog( LEVEL_WARN, "ch:%d", ch );
		switch( ch )
		{
			case 'q': endwin( ); exit(0);
			case KEY_UP:
			case KEY_LEFT:
			case KEY_PPAGE:
				if( g_cur_page > 1 )
				{
					g_cur_page--;
				}else
				{
					g_cur_page = 1;
				}
				break;
			case KEY_DOWN:
			case KEY_RIGHT:
			case KEY_NPAGE:
				if( g_cur_page >= g_total_page_num )
				{
					g_cur_page = g_total_page_num;
				}else
				{
					g_cur_page++;
				}
				break;
			case 27:
			case KEY_HOME:
				g_cur_page = 1;
				break;
			case 52:
			case KEY_END:
				g_cur_page = g_total_page_num;
				break;
			default:
				drawflag = 0;
				break;
		}
	}
	return;
}

/***********************************************************
* Function:       // 函数名称
* Description:    // 函数功能、性能等的描述
* Input:          // 1.输入参数1，说明，包括每个参数的作用、取值说明及参数间关系
* Input:          // 2.输入参数2，说明，包括每个参数的作用、取值说明及参数间关系
* Output:         // 1.输出参数1，说明
* Return:         // 函数返回值的说明
* Others:         // 其它说明
***********************************************************/
void draw_main( TRUNK_STRUCT* pOneTrunk )
{
	char	tmpStr[256] = { 0 };
	char	buf[100];
	memset( buf, 0, sizeof( buf ) );

	if( pOneTrunk == NULL )
	{
		return;
	}
	if( ( pOneTrunk->iSeqID < ( g_cur_page - 1 ) * TRK_NUM_PERPAGE ) || ( pOneTrunk->iSeqID > ( g_cur_page * TRK_NUM_PERPAGE - 1 ) ) )
	{
		return; //不在当前页,不需要显示打印
	}
	GetString_LineState( buf, pOneTrunk->iLineState );

	sprintf( tmpStr, "%03d (%02d,%03d) %-7s %-12s %-9s %-7s %-7s %-7s (%02d,%03d)",
	         pOneTrunk->iSeqID, pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID,
	         GetString_DeviceSub( pOneTrunk->deviceID.m_s16DeviceSub ), buf,
	         GetString_TrunkState( pOneTrunk->State ), pOneTrunk->CalledCode, pOneTrunk->CallerCode, pOneTrunk->DtmfBuf,
	         pOneTrunk->VocDevID.m_s8ModuleID, pOneTrunk->VocDevID.m_s16ChannelID );
	printmsg_in_win( &g_win_Trk[pOneTrunk->iSeqID % TRK_NUM_PERPAGE], tmpStr, 1 );
	g_cur_trk = ( pOneTrunk->iSeqID % TRK_NUM_PERPAGE );
	//gLog.WriteLog( LEVEL_DEBUG, "draw_main() g_cur_trk:%d, g_cur_page:%d", g_cur_trk, g_cur_page );
}
void draw_count(void)
{
	char tmpStr[200];
	sprintf( tmpStr, "Trk/Open:(%03d/%03d) User/Open:(%03d/%03d) Voc/Open:(%03d/%03d)    VocFree:%03d", 
			g_iTotalTrunk,g_iTotalTrunkOpened,g_iTotalUser,g_iTotalUserOpened,g_iTotalVoice,g_iTotalVoiceOpened,g_iTotalVoiceFree);
	printmsg_in_win( &g_win_count, tmpStr, 2 );
}
/************************************** The End Of File **************************************/

