/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// msgQueue.c
 * Author:			//
 * Date:			//
 * Description:		// IPC��ʽ����Ϣ����
 * Version:			//
 * History:			//
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <sys/types.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//��Ϣ���нṹ��
struct msg_buf
{
	int		mtype;      //����
	char	data[256];  //����
};


/***********************************************************
* Function:       // main
***********************************************************/
int main( void )
{
	key_t			key;
	int				msgid;
	int				ret;
	struct msg_buf	msgbuf;

	key = ftok( "/tmp/2", 'a' );                                        //�ļ���ת��Ϊ��ֵ
	printf( "key = [%x]\n", key );
	msgid = msgget( key, IPC_CREAT | 0666 );                            //����key�������������û��key�򴴽�
	if( msgid == -1 )
	{
		printf( "Create message queue key error!\n" );
		return -1;
	}
	printf( "msgid:%d,msgget() OK.\n", msgid );

	msgbuf.mtype = getpid( );                                           //���ͣ��ñ����̵Ľ��̺�
	strcpy( msgbuf.data, "test message" );          //����
	ret = msgsnd( msgid, &msgbuf, sizeof( msgbuf.data ), IPC_NOWAIT );  //����Ϣ���з������ݣ����Ȳ�������Ϣ��������
	if( ret == -1 )
	{
		printf( "Send message error!" );
		return -1;
	}
	printf( "msgsnd() OK." );

	memset( &msgbuf, 0, sizeof( msgbuf ) );
	ret = msgrcv( msgid, &msgbuf, sizeof( msgbuf.data ), getpid( ), IPC_NOWAIT );//����
	if( ret == -1 )
	{
		printf("recv message queue error!\n");
		return -1;
	}
	printf("msgrcv() OK.\n");
	printf("recv msg:[%s]\n", msgbuf.data);
	return 0;
}

/************************************** The End Of File **************************************/
