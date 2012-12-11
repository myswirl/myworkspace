/*************************************************
Function name: ���ǻ���ʵ���һ��ģ��
Parameter    : ��
Description  : ֱ�ӵ��ø�ģ�弴�ɣ���ѯ��ʽ���ư�������LED������
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


int Wait_key(void);
void key_init(void);
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
Description	 : key�ĳ�ʼ������������K1���ӵ���GPG0
               ��ʼ��GPG0Ϊ����ģʽ
Return		 : void
Argument     : void
Autor & date : Daniel
**************************************************/
void key_init(void) /*Ϊʲô�������û�б����ã�*/
{
   rGPGCON &= ~(3<<0);       //input mode
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
   rGPBDAT &=~(1<<5); 	//LED����������
   delay(1000);
   rGPBDAT |=(1<<5);	//��
   delay(1000);
}
/*************************************************
Function name: Wait_key()
Parameter    : void
Description	 : �����ȴ�����
Return		 : ����1��ʾ�а�������������0��ʾ�ް���
Argument     : void
Autor & date : Daniel
**************************************************/
int Wait_key(void)
{
   if(!(rGPGDAT & 0x1))
   delay(200);
   if(!(rGPGDAT & 0x1)) 	//Ϊʲô��Ҫ�ж�?ȥ������
     {
      while(!(rGPGDAT & 0x1));
      return 1;				//�����ɿ�
     }
    else
      return 0;
}