/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// udpClient.c
 * Author:			// lmy
 * Date:			//
 * Description:		// udp client，socket实现
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


/***********************************************************
* Function:       // main
***********************************************************/
int main( int argc, char *argv[] )
{
	if( argc < 2 )
	{
		printf( "Error!\n" );
		printf( "Usage: %s senddata.\n", argv[0] );
		exit( 0 );
	}
	int					sock;
	struct sockaddr_in	toAddr;     //sendto中使用的对方地址
	struct sockaddr_in	fromAddr;   //在recvfrom中使用的对方主机地址

	unsigned int		fromLen;
	char				recvBuffer[128];
	sock = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
	if( sock < 0 )
	{
		printf( "socket() fail!\n" );
		exit( 1 );
	}else
	{
		printf( "socket() OK.\n" );
	}
	memset( &toAddr, 0, sizeof( toAddr ) );
	toAddr.sin_family	   = AF_INET;
	toAddr.sin_addr.s_addr = inet_addr( "127.0.0.1" );
	toAddr.sin_port		   = htons( 4000 );
	if( sendto( sock, argv[1], strlen( argv[1] ), 0, (struct sockaddr*)&toAddr, sizeof( toAddr ) ) != strlen( argv[1] ) )
	{
		printf( "sendto() fail!\n" );
		close( sock );
		exit( 1 );
	}
	fromLen = sizeof( fromAddr );
	if( recvfrom( sock, recvBuffer, 128, 0, (struct sockaddr*)&fromAddr, &fromLen ) < 0 )
	{
		printf( "recvfrom() fail!\n" );
		close( sock );
		exit( 1 );
	}
	printf( "recvfrom() result:%s\r\n", recvBuffer );
	close( sock );
	return 0;
}

/************************************** The End Of File **************************************/
