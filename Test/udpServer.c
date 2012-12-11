/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// udpServer
 * Author:			// lmy
 * Date:			//
 * Description:		// udp server, socket实现
 * Version:			//
 * History:			//
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT		4000    //服务端口
#define MAX_BUF_SIZE	512     //传输数据最大值


/***********************************************************
* Function:       // main
***********************************************************/
int main( int argc, char *argv[] )
{
	int					sock;
	int					recvLen;
	unsigned int		addrLen;
	char				recvBuffer[MAX_BUF_SIZE];
	char				sendBuffer[MAX_BUF_SIZE];
	struct sockaddr_in	toAddr;                         //sendto中使用的对方地址
	struct sockaddr_in	fromAddr;                       //在recvfrom中使用的对方主机地址

	sock = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );  //服务端创建socket描述符
	if( sock < 0 )
	{
		printf( "socket() fail!\n" );
		exit( 0 );
	}else
	{
		printf( "socket() OK.\n" );
	}
	memset( &fromAddr, 0, sizeof( fromAddr ) );
	fromAddr.sin_family		   = AF_INET;
	fromAddr.sin_addr.s_addr   = htonl( INADDR_ANY );
	fromAddr.sin_port		   = htons( SERVER_PORT );
	if( bind( sock, (struct sockaddr*)&fromAddr, sizeof( fromAddr ) ) < 0 )
	{
		printf( "bind() fail!\n" );
		close( sock );
		exit( 1 );
	}else
	{
		printf( "bind() OK.\n" );
	}
	while( 1 )
	{
		addrLen = sizeof( toAddr );
		memset( recvBuffer, 0, sizeof( recvBuffer ) );
		if( ( recvLen = recvfrom( sock, recvBuffer, 128, 0, (struct sockaddr*)&toAddr, &addrLen ) ) < 0 )
		{
			printf( "recvfrom() fail!\n" );
			close( sock );
			exit( 1 );
		}else
		{
			printf( "recvLen:%d, recvData:%s\n", recvLen, recvBuffer );
		}

		memset( sendBuffer, 0, sizeof( sendBuffer ) );
		strcpy( sendBuffer, "I am server!");
		if( sendto( sock, sendBuffer, sizeof( "I am server!" ), 0, (struct sockaddr*)&toAddr, sizeof( toAddr ) ) < 0 )
		{
			printf( "sendto() fail!\n" );
			close( sock );
			exit( 0 );
		}
		return 0;
	}
}

/************************************** The End Of File **************************************/
