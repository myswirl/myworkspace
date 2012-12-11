/*************************************************
Function name: 这是基础实验的一个模版
Parameter    : 无
Description	 : 做基础实验，直接调用该模板即可
Return		 : 无
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


void Led_port_init(void);	//初始化函数
void Disp_run(void);
void Disp_num(int data);

#define LED1_ON   ~(1<<5)	//第五个bit置为0，低电平，点亮
#define LED2_ON   ~(1<<6)
#define LED3_ON   ~(1<<7)
#define LED4_ON   ~(1<<8)

#define LED1_OFF   (1<<5)
#define LED2_OFF   (1<<6)
#define LED3_OFF   (1<<7)
#define LED4_OFF   (1<<8)
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
    int i;
    for(;times>0;times--)
      for(i=0;i<400;i++);
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
    int i;
    //设置GPB5-GPB8为输出端口，将GPB5置为01
    rGPBCON &= ~((3<<10)|(3<<12)|(3<<14)|(3<<16));
    rGPBCON |= (1<<10)|(1<<12)|(1<<14)|(1<<16);
    
    //Disp_run();  
    rGPBDAT = rGPBDAT|(LED1_OFF)|(LED2_OFF)|(LED3_OFF)|(LED4_OFF);//全灭
   
     rGPBDAT = rGPBDAT&(LED1_ON);//LED1亮
     delay(1000);
     rGPBDAT = rGPBDAT|(LED1_OFF)|(LED2_OFF)|(LED3_OFF)|(LED4_OFF);//全灭
     delay(1000);
     rGPBDAT = rGPBDAT&(LED2_ON);//LED2亮
     delay(1000);
     rGPBDAT = rGPBDAT|(LED1_OFF)|(LED2_OFF)|(LED3_OFF)|(LED4_OFF);//全灭
     delay(1000);
     rGPBDAT = rGPBDAT&(LED3_ON);//LED3亮
     delay(1000);
     rGPBDAT = rGPBDAT|(LED1_OFF)|(LED2_OFF)|(LED3_OFF)|(LED4_OFF);//全灭
     delay(1000);
     rGPBDAT = rGPBDAT&(LED4_ON);//LED4亮
     delay(1000);
     rGPBDAT = rGPBDAT|(LED1_OFF)|(LED2_OFF)|(LED3_OFF)|(LED4_OFF);//全灭
     delay(1000);
   
	for(i=0;i<16;i++)
	{
	  Disp_num(i);
	  delay(1000);
	}
}	

/*************************************************
Function name: Led_port_init
Parameter    : void
Description	 : mini2440开发板有4个引脚连接了LED发光二极管，分别是GPB5--LED1，GPB6—LED2
               GPB7--LED3，GPB8--LED4，该函数的作用是对其个引脚进行初始化。
Return		 : void
Argument     : void
Autor & date : Daniel
**************************************************/   
void Led_port_init(void)
{
	//设置GPB5-GPB8为输出端口，将GPB5置为01
   rGPBCON &= ~((3<<10)|(3<<12)|(3<<14)|(3<<16));
   rGPBCON |= (1<<10)|(1<<12)|(1<<14)|(1<<16);
   
    //rGPBCON &= ~(3<<0);
    //rGPBCON |= (2<<0);
}

/*************************************************
Function name: Disp_run()
Parameter    : void
Description	 : mini2440开发板有4个引脚连接了LED发光二极管，该函数实现了流水灯
Return		 : void
Argument     : void
Autor & date : Daniel
**************************************************/   
void Disp_run(void)
{
    int i=30;
    rGPBDAT = rGPBDAT|(LED1_OFF)|(LED2_OFF)|(LED3_OFF)|(LED4_OFF);
    while(i--)
    {
     rGPBDAT = rGPBDAT&(LED1_ON);
     delay(1000);
     rGPBDAT = rGPBDAT|(LED1_OFF)|(LED2_OFF)|(LED3_OFF)|(LED4_OFF);
     delay(1000);
     rGPBDAT = rGPBDAT&(LED2_ON);
     delay(1000);
     rGPBDAT = rGPBDAT|(LED1_OFF)|(LED2_OFF)|(LED3_OFF)|(LED4_OFF);
     delay(1000);
     rGPBDAT = rGPBDAT&(LED3_ON);
     delay(1000);
     rGPBDAT = rGPBDAT|(LED1_OFF)|(LED2_OFF)|(LED3_OFF)|(LED4_OFF);
     delay(1000);
     rGPBDAT = rGPBDAT&(LED4_ON);
     delay(1000);
     rGPBDAT = rGPBDAT|(LED1_OFF)|(LED2_OFF)|(LED3_OFF)|(LED4_OFF);
     delay(1000);
    }
}

/*************************************************
Function name: Disp_num()
Parameter    : void
Description	 : mini2440开发板有4个引脚连接了LED发光二极管，该函数实现了显示0x0--0xF等十六进制数
Return		 : void
Argument     : void
Autor & date : Daniel
**************************************************/   
void Disp_num(int data)
{
  if(data & 0x08)   rGPBDAT = rGPBDAT&(LED4_ON);
     else           rGPBDAT = rGPBDAT|(LED4_OFF);
  if(data & 0x04)   rGPBDAT = rGPBDAT&(LED3_ON);
     else           rGPBDAT = rGPBDAT|(LED3_OFF);
  if(data & 0x02)   rGPBDAT = rGPBDAT&(LED2_ON);
     else           rGPBDAT = rGPBDAT|(LED2_OFF);
  if(data & 0x01)   rGPBDAT = rGPBDAT&(LED1_ON);
     else           rGPBDAT = rGPBDAT|(LED1_OFF);
    
}