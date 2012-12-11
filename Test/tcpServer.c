/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// tcpServer.c
 * Author:			// lmy
 * Date:			// 
 * Description:		// socketͨѶ��tcp server
 * Version:			// 
 * History:			// 
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#define SERVPORT	3333                    //tcp server�Ķ˿�
#define BACKLOG		10                      //���ͬʱ����������


/***********************************************************
* Function:       // main
***********************************************************/
int main( void )
{
	int					sockfd, client_fd;  //sock_fd������socket��client_fd�����ݴ���socket
	struct sockaddr_in	my_addr;            //������ַ��Ϣ
	struct sockaddr_in	remote_addr;        //�ͻ��˵�ַ��Ϣ
	socklen_t			sin_size = sizeof( struct sockaddr_in );
	if( ( sockfd = socket( AF_INET, SOCK_STREAM, 0 ) ) == -1 )
	{
		perror( "socket() error!" );
		exit( 1 );
	}else
	{
		printf( "socket() OK!\n" );
	}
	my_addr.sin_family		   = AF_INET;
	my_addr.sin_port		   = htons( SERVPORT );
	my_addr.sin_addr.s_addr	   = INADDR_ANY;
	bzero( &( my_addr.sin_zero ), 8 );
	if( bind( sockfd, (struct sockaddr *)&my_addr, sizeof( struct sockaddr ) ) == -1 )
	{
		perror( "Error, bind() fail!" );
		exit( 1 );
	}else
	{
		printf( "bind() OK!\n" );
	}
	if( listen( sockfd, BACKLOG ) == -1 )
	{
		perror( "listen error!" );
		exit( 1 );
	}else
	{
		printf( "listen success!---wait client connect\n" );
	}
	while( 1 )
	{
		if( ( client_fd = accept( sockfd, (struct sockaddr *)&remote_addr, &sin_size ) ) == -1 )
		{
			perror( "accept error!" );
			continue;
		}
		printf( "received a connection from %s\n", inet_ntoa( remote_addr.sin_addr ) );
		if( 0 == fork( ) )  //�ӽ��̴����
		{
			if( send( client_fd, "Hello, you are connected!\n", 26, 0 ) == -1 )
			{
				perror( "send error!" );
			}
			close( client_fd );
			return 0;
		}
		close( client_fd );
	}
}
/************************************** The End Of File **************************************/
