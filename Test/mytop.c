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
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define NCPUSTATES 5

static long cp_time[NCPUSTATES];
static long cp_old[NCPUSTATES];
static long cp_diff[NCPUSTATES];
int			cpu_states[NCPUSTATES];
char		buffer[4096 + 1];


/***********************************************************
* Function:       // 函数名称
* Description:    // 函数功能、性能等的描述
* Input:          // 1.输入参数1，说明，包括每个参数的作用、取值说明及参数间关系
* Input:          // 2.输入参数2，说明，包括每个参数的作用、取值说明及参数间关系
* Output:         // 1.输出参数1，说明
* Return:         // 函数返回值的说明
* Others:         // 其它说明
***********************************************************/
static inline char *
skip_token( const char *p )
{
	while( isspace( *p ) )
	{
		p++;
	}
	while( *p && !isspace( *p ) )
	{
		p++;
	}
	return (char *)p;
}

long percentages( cnt, out, new, old, diffs )
int cnt;
int				*out;
register long	*new;
register long	*old;
long			*diffs;

{
	register int	i;
	register long	change;
	register long	total_change;
	register long	*dp;
	long			half_total;

/* initialization */
	total_change   = 0;
	dp			   = diffs;

/* calculate changes for each state and the overall change */
	for( i = 0; i < cnt; i++ )
	{
		if( ( change = *new - *old ) < 0 )
		{
/* this only happens when the counter wraps */
			change = (int)
			         ( (unsigned long)*new - (unsigned long)*old );
		}
		total_change  += ( *dp++ = change );
		*old++		   = *new++;
	}

/* avoid divide by zero potential */
	if( total_change == 0 )
	{
		total_change = 1;
	}

/* calculate percentages based on overall change, rounding up */
	half_total = total_change / 2l;
	half_total = 0;
	for( i = 0; i < cnt; i++ )
	{
//printf("dd %ld %ld\n",(*diffs* 1000 + half_total),total_change);
		*out++ = (int)( ( *diffs++ *1000 + half_total ) / total_change );
	}

/* return the total in case the caller wants to use it */
	return ( total_change );
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
int main( )
{
	int		fd, len;
	char	*p;
	int		i;

	for( i = 0; i < 4; i++ )
	{
		cpu_states[i]  = 0;
		cp_diff[i]	   = 0;
	}

	while( 1 )
	{
		fd	   = open( "/proc/stat", O_RDONLY );
		len	   = read( fd, buffer, sizeof( buffer ) - 1 );
		close( fd );
		buffer[len] = '\0';

		p		   = skip_token( buffer ); /* "cpu" */
		cp_time[0] = strtoul( p, &p, 0 );
		cp_time[1] = strtoul( p, &p, 0 );
		cp_time[2] = strtoul( p, &p, 0 );
		cp_time[3] = strtoul( p, &p, 0 );
		cp_time[4] = strtoul( p, &p, 0 );
		printf("f %ld %ld %ld %ld\n",cp_time[0],cp_time[1],cp_time[2],cp_time[3]);
		percentages( NCPUSTATES, cpu_states, cp_time, cp_old, cp_diff );

		printf( "cpu used:%4.1f nice:%4.1f sys:%4.1f idle:%4.1f iowait:%4.1f\n", cpu_states[0] / 10.0, cpu_states[1] / 10.0, cpu_states[2] / 10.0, cpu_states[3] / 10.0, cpu_states[4] / 10.0 );

		sleep( 3 );
	}
}
/************************************** The End Of File **************************************/
