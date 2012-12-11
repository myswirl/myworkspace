/*************************************************
Function name: �ж�ʵ�֣���������LED
Parameter    : ��
Description  : 
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

int g_KeyInterrupt_Counter=0;//�жϴ���
void key_init(void);
static void __irq key_handler(void);
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
Function name: Main
Parameter    : void
Description	 : �����ܺ���
Return		 : void
Argument     : void
Autor & date :
**************************************************/
void Main(void)
{	
   Led1_init();

   key_init();

   while( 1);//��������ѭ������֤���򲻻��˳����жϴ����߳��ܹ��������ж�
  
}	
/*************************************************
Function name: key_init()
Parameter    : void
Description	 : key�ĳ�ʼ������������K1���ӵ���GPG0
               ��ʼ��GPG0Ϊ����ģʽ
Return		 : void
Argument     : void
Autor & date : Daniel
**************************************************/
void key_init(void) /*Ϊʲô�������û�б����ã�*/
{
   	rGPGCON &= ~(3<<0);     //input mode
   	rGPGCON |= (0x2<<0);	//?
   	
   	rEXTINT1 &= ~(0xf<<0);	//���ã�EINT8 LowLevel�����жϣ��������ر�
   	rEINTPEND |= (1<<8);	//���EINT8�������ж�
   	rEINTMASK &= ~(1<<8);	//����EINT8�ж�
   	
   	//����ISR
   	pISR_EINT8_23 =(U32)key_handler;
   	EnableIrq(BIT_EINT8_23);
   		  
   
}
//�����жϴ�����
static void __irq key_handler(void)
{
	//�ж��Ƿ񰴼�K1�������ж�
	if(rINTPND == BIT_EINT8_23)
	{
		g_KeyInterrupt_Counter++;
		
		ClearPending(BIT_EINT8_23);
		
		if( rEINTPEND &(1<<8))
		{
			rEINTPEND |= 1<<8;//����ж�
		}
		
		Led1_run();
	}
}
/*************************************************
Function name: Led1_init()
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
Function name: Led1_run()
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
