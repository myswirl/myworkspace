/*************************************************
Function name: 中断实现，按键点亮LED
Parameter    : 无
Description  : 
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

int g_KeyInterrupt_Counter=0;//中断次数
void key_init(void);
static void __irq key_handler(void);
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
Function name: Main
Parameter    : void
Description	 : 主功能函数
Return		 : void
Argument     : void
Autor & date :
**************************************************/
void Main(void)
{	
   Led1_init();

   key_init();

   while( 1);//程序处于死循环，保证程序不会退出，中断处理线程能够处理按键中断
  
}	
/*************************************************
Function name: key_init()
Parameter    : void
Description	 : key的初始化函数，其中K1连接的是GPG0
               初始化GPG0为输入模式
Return		 : void
Argument     : void
Autor & date : Daniel
**************************************************/
void key_init(void) /*为什么这个函数没有被调用？*/
{
   	rGPGCON &= ~(3<<0);     //input mode
   	rGPGCON |= (0x2<<0);	//?
   	
   	rEXTINT1 &= ~(0xf<<0);	//设置，EINT8 LowLevel产生中断，过滤器关闭
   	rEINTPEND |= (1<<8);	//清除EINT8产生的中断
   	rEINTMASK &= ~(1<<8);	//允许EINT8中断
   	
   	//设置ISR
   	pISR_EINT8_23 =(U32)key_handler;
   	EnableIrq(BIT_EINT8_23);
   		  
   
}
//按键中断处理函数
static void __irq key_handler(void)
{
	//判断是否按键K1产生的中断
	if(rINTPND == BIT_EINT8_23)
	{
		g_KeyInterrupt_Counter++;
		
		ClearPending(BIT_EINT8_23);
		
		if( rEINTPEND &(1<<8))
		{
			rEINTPEND |= 1<<8;//清除中断
		}
		
		Led1_run();
	}
}
/*************************************************
Function name: Led1_init()
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
Function name: Led1_run()
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
