#include <ncurses.h>
typedef struct _win_border_struct 
{
	chtype ls, rs, ts, bs,tl, tr, bl, br;
}WIN_BORDER;

typedef struct _WIN_struct 
{
	int startx, starty;
	int height, width;
	WIN_BORDER border;
}WIN;
void init_win_params(WIN *p_win);
void print_win_params(WIN *p_win);
void create_box(WIN *win, int isCreate);
int main(int argc, char *argv[])
{
	WIN win;
	int ch;
	initscr(); /* 初始化并进入curses 模式*/
	start_color(); /* 开启彩色显示功能*/
	cbreak(); /* 行缓冲禁止，传递所有控制信息*/
	keypad(stdscr, TRUE); /*程序需要使用F1 功能键*/
	noecho();
	init_pair(1, COLOR_CYAN, COLOR_BLACK);
	/* 以下是初始化窗口参数*/
	init_win_params(&win);
	print_win_params(&win);
	attron(COLOR_PAIR(1));
	//printw("Press F1 to exit");
	refresh();
	attroff(COLOR_PAIR(1));
	create_box(&win, TRUE);
	while((ch = getch()) != 'q')
	{ 
		switch(ch)
		{ 
		case KEY_LEFT:
			create_box(&win, FALSE);
			--win.startx;
			create_box(&win, TRUE);
			break;
		case KEY_RIGHT:
			create_box(&win, FALSE);
			++win.startx;
			create_box(&win, TRUE);
			break;
		case KEY_UP:
			create_box(&win, FALSE);
			--win.starty;
			create_box(&win, TRUE);
			break;
		case KEY_DOWN:
			create_box(&win, FALSE);
			++win.starty;
			create_box(&win, TRUE);
			break;
		}
	}
	endwin(); /*结束curses 模式*/
	return 0;
}
void init_win_params(WIN *p_win)
{
	p_win->height = LINES-3;
	p_win->width = COLS-3;
	p_win->starty = 1;//(LINES - p_win->height)/2;
	p_win->startx = 1;//(COLS - p_win->width)/2;
	p_win->border.ls = '|';
	p_win->border.rs = '|';
	p_win->border.ts = '-';
	p_win->border.bs = '-';
	p_win->border.tl = '+';
	p_win->border.tr = '+';
	p_win->border.bl = '+';
	p_win->border.br = '+';
}
void print_win_params(WIN *p_win)
{

	mvprintw(4, 4, "startx=%d starty=%d width=%d height=%d  LINES=%d COLS=%d \n", p_win->startx, p_win->starty,
		p_win->width, p_win->height,LINES,COLS);
	refresh();
}
void create_box(WIN *p_win, int isCreate)
{ 
	int i, j;
	int x, y, w, h;
	x = p_win->startx;
	y = p_win->starty;
	w = p_win->width;
	h = p_win->height;
	if(isCreate == TRUE)
	{ 
		mvaddch(y, x, p_win->border.tl);
		mvaddch(y, x + w, p_win->border.tr);
		mvaddch(y + h, x, p_win->border.bl);
		mvaddch(y + h, x + w, p_win->border.br);
		mvhline(y, x + 1, p_win->border.ts, w - 1);
		mvhline(y + h, x + 1, p_win->border.bs, w - 1);
		mvvline(y + 1, x, p_win->border.ls, h - 1);
		mvvline(y + 1, x + w, p_win->border.rs, h - 1);
	}	else
	{
		for(j = y; j <= y + h; ++j)
		{
			for(i = x; i <= x + w; ++i)
			{
				mvaddch(j, i, ' ');
			}
		}
	}
	refresh();
}

