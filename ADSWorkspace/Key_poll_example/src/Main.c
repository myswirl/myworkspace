/*************************************************
Function name: 这是基础实验的一个模版
Parameter    : 无
Description  : 直接调用该模板即可，轮询方式控制按键控制LED亮和灭
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


int Wait_key(void);
void key_init(void);
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
   while(1)
   {
        if(Wait_key())
        //if(!(rGPGDAT & 0x1))
        Led1_run();
   }
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
   rGPGCON &= ~(3<<0);       //input mode
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
   rGPBDAT &=~(1<<5); 	//LED亮还是灭？亮
   delay(1000);
   rGPBDAT |=(1<<5);	//灭
   delay(1000);
}
/*************************************************
Function name: Wait_key()
Parameter    : void
Description	 : 按键等待函数
Return		 : 返回1表示有按键动作，返回0表示无按键
Argument     : void
Autor & date : Daniel
**************************************************/
int Wait_key(void)
{
   if(!(rGPGDAT & 0x1))
   delay(200);
   if(!(rGPGDAT & 0x1)) 	//为什么又要判断?去除抖动
     {
      while(!(rGPGDAT & 0x1));
      return 1;				//按键松开
     }
    else
      return 0;
}