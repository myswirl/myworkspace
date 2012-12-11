/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// msgQueue.c
 * Author:			//
 * Date:			//
 * Description:		// IPC方式，消息队列
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

//消息队列结构体
struct msg_buf
{
	int		mtype;      //类型
	char	data[256];  //内容
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

	key = ftok( "/tmp/2", 'a' );                                        //文件名转化为键值
	printf( "key = [%x]\n", key );
	msgid = msgget( key, IPC_CREAT | 0666 );                            //返回key的描述符，如果没有key则创建
	if( msgid == -1 )
	{
		printf( "Create message queue key error!\n" );
		return -1;
	}
	printf( "msgid:%d,msgget() OK.\n", msgid );

	msgbuf.mtype = getpid( );                                           //类型，用本进程的进程号
	strcpy( msgbuf.data, "test message" );          //内容
	ret = msgsnd( msgid, &msgbuf, sizeof( msgbuf.data ), IPC_NOWAIT );  //向消息队列发送数据，长度不包括消息队列类型
	if( ret == -1 )
	{
		printf( "Send message error!" );
		return -1;
	}
	printf( "msgsnd() OK." );

	memset( &msgbuf, 0, sizeof( msgbuf ) );
	ret = msgrcv( msgid, &msgbuf, sizeof( msgbuf.data ), getpid( ), IPC_NOWAIT );//接收
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
