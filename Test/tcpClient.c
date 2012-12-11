/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// tcpClient.c
 * Author:			// lmy
 * Date:			// 2011年11月22日21:16:53
 * Description:		// socket通讯，tcp client
 * Version:			//
 * History:			//
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define SERVPORT		3333    //要连接的socket服务的端口号
#define MAX_DATA_SIZE	100     //传输数据的最大长度


/***********************************************************
* Function:       // main
***********************************************************/
int main( int argc, char **argv )
{
	int					sockfd, recvbytes;
	char				buf[MAX_DATA_SIZE];
	struct hostent		*host;
	struct sockaddr_in	serv_addr;

	memset(buf, 0, sizeof(buf));
	if( argc < 2 )
	{
		fprintf( stderr, "Please enter the server's hostname!\n" );
		exit( 1 );
	}

	if( ( host = gethostbyname( argv[1] ) ) == NULL )
	{
		perror( "gethostbyname() error!" );
		exit( 1 );
	}

	if( ( sockfd = socket( AF_INET, SOCK_STREAM, 0 ) ) == -1 )
	{
		perror( "create socket error!" );
		exit( 1 );
	}

	serv_addr.sin_family   = AF_INET;
	serv_addr.sin_port	   = htons( SERVPORT );
	serv_addr.sin_addr	   = *( (struct in_addr *)host->h_addr );
	bzero( &( serv_addr.sin_zero ), 8 );
	if( connect( sockfd, (struct sockaddr *)&serv_addr, sizeof( struct sockaddr ) ) == -1 )
	{
		perror( "connect() error!" );
		exit( 1 );
	}

	if( ( recvbytes = recv( sockfd, buf, MAX_DATA_SIZE, 0 ) ) == -1 )
	{
		perror( "recv() error!" );
		exit( 1 );
	}
	buf[recvbytes] = '\0';
	printf( "Received: %s", buf );
	close( sockfd );
	return 0;
}

/************************************** The End Of File **************************************/
