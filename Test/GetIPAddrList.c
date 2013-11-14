/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// GetIPAddrList.c
 * Author:			//
 * Date:			//
 * Description:		// 获取Linux系统IP地址列表
 * Version:			//
 * History:			//
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h> 
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <errno.h>
int GetLocalIPAddress( char* s8CardName, char *s8IPAddress )
{
	struct ifreq		ifr;
	int				sock_get_ip = -1;
	struct sockaddr_in	*ip;

	sock_get_ip = socket( AF_INET, SOCK_STREAM, 0 );

	if( sock_get_ip >= 0 )
	{
		strcpy( ifr.ifr_name, s8CardName );
		if( ioctl( sock_get_ip, SIOCGIFADDR, &ifr ) >= 0 )
		{
			ip = (struct sockaddr_in *)&ifr.ifr_addr; //获取ipv4地址
			strcpy( s8IPAddress, inet_ntoa( ip->sin_addr ) );
		}else
		{
			printf(  "ioctl SIOCGIFADDR error!%s", strerror( errno ) );
		}
		close( sock_get_ip );
	}else
	{
		printf(  "socket create fail...GetLocalIp!, %s", strerror( errno ) );
		return -1;
	}
}


//根据网卡设备名称获取MAC值
int GetMAC( char *ifa_name, char *MAC )
{
	/* get mac */
	struct ifreq	ifreq;
	int				sock;

	if( ( sock = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 )
	{
		perror( "error" );
		return -1;
	}
	strcpy( ifreq.ifr_name, ifa_name );
	if( ioctl( sock, SIOCGIFHWADDR, &ifreq ) < 0 )
	{
		perror( "error:" );
		return -2;
	}
	sprintf( MAC, "%02X:%02X:%02X:%02X:%02X:%02X ",
	         (unsigned char)ifreq.ifr_hwaddr.sa_data[0],
	         (unsigned char)ifreq.ifr_hwaddr.sa_data[1],
	         (unsigned char)ifreq.ifr_hwaddr.sa_data[2],
	         (unsigned char)ifreq.ifr_hwaddr.sa_data[3],
	         (unsigned char)ifreq.ifr_hwaddr.sa_data[4],
	         (unsigned char)ifreq.ifr_hwaddr.sa_data[5] );
	return 0;
}

/***********************************************************
* Function:       // main
* Description:    //
* Input:          //
* Input:          //
* Output:         //
* Return:         //
* Others:         //
***********************************************************/
int main( void )
{
	struct ifaddrs	* ifAddrStruct = NULL;
	void			* tmpAddrPtr   = NULL;
	char			mac[NI_MAXHOST];
	char			tempIP[50];

	if( getifaddrs( &ifAddrStruct ) == -1 )
	{
		perror( "getifaddrs" );
		exit( EXIT_FAILURE );
	}

	while( ifAddrStruct != NULL )
	{
		if( ifAddrStruct->ifa_addr == NULL )
		{
			ifAddrStruct = ifAddrStruct->ifa_next;
			continue;
		}
		if( ifAddrStruct->ifa_addr->sa_family == AF_INET )          // check it is IP4
		{                                                           // is a valid IP4 Address
			tmpAddrPtr = &( (struct sockaddr_in *)ifAddrStruct->ifa_addr )->sin_addr;
			char addressBuffer[INET_ADDRSTRLEN];
			inet_ntop( AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN );
			GetMAC( ifAddrStruct->ifa_name, mac );//获取MAC地址
			printf( "%5s IP Address: %15s, MAC:%s\n", ifAddrStruct->ifa_name, addressBuffer, mac );
		} else if( ifAddrStruct->ifa_addr->sa_family == AF_INET6 )  // check it is IP6
		{                                                           // is a valid IP6 Address
			tmpAddrPtr = &( (struct sockaddr_in *)ifAddrStruct->ifa_addr )->sin_addr;
			char addressBuffer[INET6_ADDRSTRLEN];
			inet_ntop( AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN );
			GetMAC( ifAddrStruct->ifa_name, mac );//获取MAC地址
			printf( "%5s IP Address: %15s, MAC:%s\n", ifAddrStruct->ifa_name, addressBuffer,mac );
		}
		ifAddrStruct = ifAddrStruct->ifa_next;
	}
	
	freeifaddrs(ifAddrStruct); //一定要释放  
while(1)
	{
	sleep(1);
	GetLocalIPAddress("eth0", tempIP ); //获取本机IP
	printf( "IP Address:%s \n", tempIP );
	}
	return 0;
}

/************************************** The End Of File **************************************/
