/*************************************************
Function name: 这是基础实验的一个模版
Parameter    : 无
Description  : 做基础实验，直接调用该模板即可
Return	     : 无
Argument     : 无
Autor & date : Daniel
**************************************************/
#define	GLOBAL_CLK		1
#include <stdlib.h>
#include <string.h>
#include "def.h"
#include "option.h"
#include "2440addr.h"
#include "2440lib.h"
#include "2440slib.h"
#include "mmu.h"
#include "profile.h"
#include "memtest.h"

void Led1_run(void);
void Led1_init(void);
/*************************************************
Function name: delay
Parameter    : times
Description	 : 延时函数
Return		 : void
Argument     : void
Autor & date :
**************************************************/
void delay(int times)
{
    int i,j;
    for(i=0;i<times;i++)
       for(j=0;j<400;j++);
}


/*************************************************
Function name: beep_init()
Parameter    : void
Description	 : Led1的初始化函数，其中Led1连接的是GPB5
               初始化GPB5为输出模式
Return		 : void
Argument     : void
Autor & date : Daniel
**************************************************/
void Led1_init(void)
{
   rGPBCON &= ~(3<<10);
   rGPBCON |= (1<<10);
}
/*************************************************
Function name: beep_run()
Parameter    : void
Description	 : beep的初始化函数，其中beep连接的是GPB0
               初始化GPB0为输出模式
Return		 : void
Argument     : void
Autor & date : Daniel
**************************************************/
void Led1_run(void)
{
   rGPBDAT &=~(1<<5); /*LED亮还是灭？*/
   delay(1000);
   rGPBDAT |=(1<<5);
   delay(1000);
}
//定时器中断处理函数
static void __irq IRQ_Timer0_Handle(void)
{
	Led1_run();
	ClearPending(BIT_TIMER0);
}
//定时器0初始化
void Timer0_init(void)
{
	
	rTCFG0 = 49;			//PCLK/(49+1)
	rTCFG1 = 0x03;			//(1M/16分频)=62500Hz
	rTCNTB0 = 62500/2;		//0.5s
	rTCMPB0 = 0;
	
	rTCON = 0x09;			//启动定时器，
	
	ClearPending(BIT_TIMER0);
	pISR_TIMER0 = (U32)IRQ_Timer0_Handle;
	EnableIrq(BIT_TIMER0);
}

static void cal_cpu_bus_clk(void)
{
	static U32 cpu_freq;
	static U32 UPLL;
	U32 val;
	U8 m,p,s;
	
	val = rMPLLCON;
	m = (val>>12)&0xff;
	p = (val>>4)&0x3f;
	s = val&3;
	
	FCLK = ((m+8)*(FIN/100)*2)/((p+2)*(1<<s))*100;
	
	val = rCLKDIVN;
	m = (val>>1)&3;
	p = val&1;
	val = rCAMDIVN;
	s = val>>8;
	
	switch(m)
	{
	case 0:
		HCLK = FCLK;
		break;
	
	case 1:
		HCLK = FCLK>>1;
		break;
		
	case 2:
		if(s&2)
		{
			HCLK = FCLK>>3;
		}else
		{
			HCLK = FCLK>>2;
		}
		break;
	case 3:
		if(s&1)
		{
			HCLK = FCLK/6;
		}else
		{
			HCLK = FCLK/3;
		}
		break;
	
	}
	
	if(p)
	{
		PCLK = HCLK>>1;
	}else
	{
		PCLK = HCLK;
	}
	
	if(s&0x10)
	{
		cpu_freq = HCLK;
	}else
	{
		cpu_freq = FCLK;
	}
	
	val = rUPLLCON;
	m = (val>>12)&0xff;
	p = (val>.4)&0x3f;
	s = val&3;
	
	UPLL = ((m+8)*FIN)/((p+2)*(1<<s));
	UCLK = (rCLKDIVN&8)?(UPLL>>1):UPLL;
	
	
	
	
	
}
//系统时钟设置
void Set_Clk(void)
{
	int i;
	U8 key;
	U32 mpll_val =0;
	i=2;
	
	switch(i)
	{
	case 0://200
		key = 12;
		mpll_val = (92<<12)|(4<<4)|(1);
		break;
	
	case 1://300
		key = 13;
		mpll_val = (67<<12)|(1<<4)|(1);
		break;
	case 2:
		key = 14;
		mpll_val = (92<<12)|(1<<4)|(1);
		break;
	case 3://440
		key = 14;
		mpll_val = (102<<12)|(1<<4)|(1);
		break;
	default:
		key = 14;
		mpll_val = (92<<12)|(1<<4)|(1);
		break;
	
	}
	
	ChangeMPllValue((mpll_val>>12)&0xff,(mpll_val>>4)&0x3f,mpll_val&3);
	ChangeClockDivider(key,12);
	cal_cpu_bus_clk();
	
}
/*************************************************
Function name: Main
Parameter    : void
Description	 : 主功能函数
Return		 : void
Argument     : void
Autor & date :
**************************************************/
void Main(void)
{	
	//系统时钟设置
	Set_Clk();
	
	//LED1初始化
	Led1_init();
   	
   	Timer0_init();
   	
}	