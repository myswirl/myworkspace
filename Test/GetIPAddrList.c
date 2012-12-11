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
void getLocalIPAddress( char *s8IPAddress )
{
#ifdef WIN32


/*	DJ_S8     szHostname[100];
   HOSTENT   *pHostEnt;
    in_addr   inAddr;
    gethostname(   szHostname,   sizeof(   szHostname   ));
    pHostEnt   =   gethostbyname(   szHostname   );
    memcpy(&inAddr.S_un,   pHostEnt->h_addr_list[0],   pHostEnt->h_length);
    strcpy(s8IPAddress,inet_ntoa(inAddr));*/
#else
	struct ifconf	conf;
	struct ifreq	*ifr;
	char			buff[BUFSIZ];
	int				num;
	int				i;

	int				s = socket( PF_INET, SOCK_DGRAM, 0 );
	conf.ifc_len   = BUFSIZ;
	conf.ifc_buf   = buff;

	ioctl( s, SIOCGIFCONF, &conf );
	num	   = conf.ifc_len / sizeof( struct ifreq );
	ifr	   = conf.ifc_req;

	//printf("num=%d\n",num);

	for( i = 0; i < num; i++ )
	{
		struct sockaddr_in *sin = (struct sockaddr_in *)( &ifr->ifr_addr );

		ioctl( s, SIOCGIFFLAGS, ifr );
		printf("sin->sin_addr =%X\n",sin->sin_addr.s_addr);
		if( ( ( ifr->ifr_flags & IFF_LOOPBACK ) == 0 ) && ( ifr->ifr_flags & IFF_UP ) )
		{
			strcpy( s8IPAddress, inet_ntoa( sin->sin_addr ) );
			printf("s8IPAddress=%s,sin_addr =%X \n",s8IPAddress,sin->sin_addr.s_addr);
			break;
		}
		ifr++;
	}
#endif
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

	getLocalIPAddress( tempIP ); //获取本机IP
	printf( "IP Address:%s \n", tempIP );

	return 0;
}

/************************************** The End Of File **************************************/
