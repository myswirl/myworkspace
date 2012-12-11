/*************************************************
Function name: ���ǻ���ʵ���һ��ģ��
Parameter    : ��
Description  : ������ʵ�飬ֱ�ӵ��ø�ģ�弴��
Return	     : ��
Argument     : ��
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
Description	 : ��ʱ����
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
Description	 : Led1�ĳ�ʼ������������Led1���ӵ���GPB5
               ��ʼ��GPB5Ϊ���ģʽ
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
Description	 : beep�ĳ�ʼ������������beep���ӵ���GPB0
               ��ʼ��GPB0Ϊ���ģʽ
Return		 : void
Argument     : void
Autor & date : Daniel
**************************************************/
void Led1_run(void)
{
   rGPBDAT &=~(1<<5); /*LED��������*/
   delay(1000);
   rGPBDAT |=(1<<5);
   delay(1000);
}
//��ʱ���жϴ�����
static void __irq IRQ_Timer0_Handle(void)
{
	Led1_run();
	ClearPending(BIT_TIMER0);
}
//��ʱ��0��ʼ��
void Timer0_init(void)
{
	
	rTCFG0 = 49;			//PCLK/(49+1)
	rTCFG1 = 0x03;			//(1M/16��Ƶ)=62500Hz
	rTCNTB0 = 62500/2;		//0.5s
	rTCMPB0 = 0;
	
	rTCON = 0x09;			//������ʱ����
	
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
//ϵͳʱ������
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
Description	 : �����ܺ���
Return		 : void
Argument     : void
Autor & date :
**************************************************/
void Main(void)
{	
	//ϵͳʱ������
	Set_Clk();
	
	//LED1��ʼ��
	Led1_init();
   	
   	Timer0_init();
   	
}	