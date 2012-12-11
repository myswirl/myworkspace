/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// �ļ���
 * Author:			// ����
 * Date:			// ����
 * Description:		// ģ������
 * Version:			// �汾��Ϣ
 * History:			// ��ʷ�޸ļ�¼
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include  <stdio.h>
#include  <stdlib.h>
#include  <ctype.h>
#include  <string.h>
#include  <fcntl.h>
#include  <unistd.h>
#include  <errno.h>
#include  <arpa/inet.h>
#include  <net/if.h>
#include  <stropts.h>

typedef struct
{
	char			name[20];
	unsigned int	user;
	unsigned int	nice;
	unsigned int	system;
	unsigned int	idle;
	unsigned int	iowait;
	unsigned int	hardirq;
	unsigned int	softirq;
}CPU_OCCUPY;

int get_CPUActiveInfo( char* pCPUName, CPU_OCCUPY *pOccu );


int getmemstate( void );


int getnetcardspeed( void );


/***********************************************************
* Function:       // get_CPUActiveInfo
* Description:    // ��ȡCPU���Ϣ
* Input:          // 1.pCPUName:CPU����
* Output:         // 1.pOccu:CPU���Ϣ�ṹ��ָ��
* Return:         //
***********************************************************/
int get_CPUActiveInfo( char* pCPUName, CPU_OCCUPY *pOccu )
{
	FILE	*fd;
	char	buff[1024];

	if( NULL == pCPUName || NULL == pOccu )
	{
		printf( "Error, get_CPUActiveInfo() input param is wrong! pCPUName:%p, pOccu:%p.\n", pCPUName, pOccu );
		return -1;  //�����������
	}

	fd = fopen( "/proc/stat", "r" );
	if( fd == NULL )
	{
		printf( "Error, get_CPUActiveInfo() read file %s fail! err_msg:%s.\n", "/proc/stat", strerror( errno ) );
		return -2;  //��ȡ�ļ�����
	}

	memset( buff, 0, sizeof( buff ) );

	while( fgets( buff, sizeof( buff ), fd ) != NULL )
	{
		memset( pOccu, 0, sizeof( CPU_OCCUPY ) );

		sscanf( buff, "%s %u %u %u %u %u %u %u", &pOccu->name, &pOccu->user, &pOccu->nice, &pOccu->system, &pOccu->idle, &pOccu->iowait, &pOccu->hardirq, &pOccu->softirq );

		//printf( "%s %u %u %u %u %u %u %u\n", pOccu->name, pOccu->user, pOccu->nice, pOccu->system, pOccu->idle, pOccu->iowait, pOccu->hardirq, pOccu->softirq );

		if( strcmp( pCPUName, pOccu->name ) == 0 )
		{
			//printf( "get right cpu info, %s \n",pCPUName);
			fclose( fd );
			return 0;
		}
	}

	fclose( fd );
	printf( "Error, get_CPUActiveInfo() not find cpu %s! \n", pCPUName );
	return -3;
}

/***********************************************************
* Function:       // getmemstate
* Description:    // ��ȡ�ڴ�ʹ�����
***********************************************************/
int getmemstate( void )
{
	int				fd, len;
	int				i;
	char			buffer[4 * 1024 + 1];
	char			*buf;
	char			str[6][30] = { "MemTotal", "MemFree", "Buffers", "Cached", "VmallocTotal", "VmallocUsed" };
	unsigned int	ret[6];
	int				MemUsed;    // MB
	int				MemUsage;   // MemUsage %

	fd	   = open( "/proc/meminfo", O_RDONLY );
	len	   = read( fd, buffer, sizeof( buffer ) - 1 );
	close( fd );

	for( i = 0; i < 6; i++ )
	{
		buf = (char *)buffer;
		char *str1 = str[i];
		buf = strstr( buf, str1 );
		if( NULL == buf )
		{
			return -1;
		}
		while( !isspace( *buf ) )
		{
			buf++;
		}
		while( isspace( *buf ) )
		{
			buf++;
		}
		ret[i] = strtoul( buf, &buf, 0 ) / 1024;
	}
	MemUsed	   = ret[0] - ret[1] - ret[2] - ret[3]; //MemUsed = MemTotal - MemFree - Buffers - Cached
	MemUsage   = MemUsed * 100 / ret[0];

	printf( "-------------MemoryInfo-------------\n" );
	for( i = 0; i < 6; i++ )
	{
		printf( "%12s: %4d MByte \n", str[i], ret[i] );
	}
	printf( "%12s: %4d MByte \n", "MemUsed", MemUsed );
	printf( "%12s: %d%% \n\n", "MemUsage", MemUsage );

	return 1;
}

/***********************************************************
* Function:       // getnetcardspeed
* Description:    // ��ȡ��������
***********************************************************/
int getnetcardspeed( char *devname )
{
#ifndef SIOCETHTOOL
#define SIOCETHTOOL 0x8946
#endif
#ifndef ARRAY_SIZE
#define ARRAY_SIZE( x ) ( sizeof( x ) / sizeof( ( x )[0] ) )
#endif

/* CMDs currently supported */
#define ETHTOOL_GSET	0x00000001  /* Get settings. */
#define ETHTOOL_SSET	0x00000002  /* Set settings. */

/* hack, so we may include kernel's ethtool.h */
//typedef unsigned long long __u64;
	typedef __uint32_t	__u32;      /* ditto */
	typedef __uint16_t	__u16;      /* ditto */
	typedef __uint8_t	__u8;       /* ditto */

/* The forced speed, 10Mb, 100Mb, gigabit, 2.5Gb, 10GbE. */
#define SPEED_10	10
#define SPEED_100	100
#define SPEED_1000	1000
#define SPEED_2500	2500
#define SPEED_10000 10000
#define NETCARD		"eth1"

/* This should work for both 32 and 64 bit userland. */
	struct ethtool_cmd {
		__u32	cmd;
		__u32	supported;      /* Features this interface supports */
		__u32	advertising;    /* Features this interface advertises */
		__u16	speed;          /* The forced speed, 10Mb, 100Mb, gigabit */
		__u8	duplex;         /* Duplex, half or full */
		__u8	port;           /* Which connector port */
		__u8	phy_address;
		__u8	transceiver;    /* Which transceiver to use */
		__u8	autoneg;        /* Enable or disable autonegotiation */
		__u32	maxtxpkt;       /* Tx pkts before generating tx int */
		__u32	maxrxpkt;       /* Rx pkts before generating rx int */
		__u32	reserved[4];
	};

	struct ifreq	ifr, *ifrp; // �ӿ�����ṹ
	int				fd;         // to  access socket  ͨ��socket���������� �ļ���������fd

	/* Setup our control structures. */
	memset( &ifr, 0, sizeof( ifr ) );
	strcpy( ifr.ifr_name, devname );

	/* Open control socket. */
	fd = socket( AF_INET, SOCK_DGRAM, 0 );
	if( fd < 0 )
	{
		perror( "Cannot get control socket" );
		return 70;
	}

	int					err;
	struct ethtool_cmd	ep;
	//fprintf(stdout, "Settings for %s:/n", devname);

	ep.cmd		   = ETHTOOL_GSET;                      // ethtool-copy.h:380:#define ETHTOOL_GSET         0x00000001 /* Get settings. */
	ifr.ifr_data   = (caddr_t)&ep;                      //   caddr_t ��void���ͣ�����仰��ʲô��˼
	err			   = ioctl( fd, SIOCETHTOOL, &ifr );    //  int ioctl(int handle, int cmd,[int *argdx, int argcx]);
	if( err != 0 )                                      // ��������˳�;
	{
		printf( "getnetcardspeed(),  devname:%s, ioctl is erro .\n", devname );
		return -1;
	}

	// ===========  ��� �����ٶȣ�============
	printf( "%s Speed: %dMbps \n", devname, ep.speed );
	return ep.speed;
}

/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          // 1.�������1��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input:          // 2.�������2��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.�������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
unsigned int  getnetcardrate( void )
{
#define NETCARD "eth0"

	int						fd, len;
	char					buffer[4 * 1024 + 1];
	char					buffer1[4 * 1024 + 1];
	char					*buf;
	int						i = 0;
	unsigned long long int	bytes[4];

	fd = open( "/proc/net/dev", O_RDONLY );
	if( fd < 0 )
	{
		printf( "Error, open fd /proc/net/dev fail!\n" );
		return -1;
	}

	len = read( fd, buffer, sizeof( buffer ) - 1 );
	if( len < 0 )
	{
		printf( "Error, read fd /proc/net/de fail!\n" );
		close( fd );
		return -1;
	}
	close( fd );

	buf = (char *)buffer;

	buf	   = strstr( buf, NETCARD );
	buf	   = strstr( buf, ":" );
	buf++;
//	char *buf1 = buf;
//	printf("%s\n", buf);
	if( !isspace( *buf ) )
	{
		bytes[0] = strtoul( buf, &buf, 0 );
	}else
	{
		if( NULL == buf )
		{
			printf( "Error, strstr error!\n" );
			return -1;
		}

		while( !isspace( *buf ) )
		{
			buf++;
		}

		while( isspace( *buf ) )
		{
			buf++;
		}

		bytes[0] = strtoul( buf, &buf, 0 ); //old receive bytes
	}
	for( i = 0; i < 8; i++ )
	{
		while( !isspace( *buf ) )
		{
			buf++;
		}

		while( isspace( *buf ) )
		{
			buf++;
		}
	}

	bytes[1] = strtoul( buf, &buf, 0 ); //old receive bytes

	sleep( 1 );

	/*open /proc/net/dev file again   read the value of receive bytes and transmit bytes*/

	int fd1 = open( "/proc/net/dev", O_RDONLY );

	if( fd1 < 0 )
	{
		printf( "Error, open fd1 /proc/net/dev fail!\n" );
		return -1;
	}

	len = read( fd1, buffer1, sizeof( buffer1 ) - 1 );
	if( len < 0 )
	{
		printf( "Error, read fd1 /proc/net/dev fail!\n" );
		close( fd1 );
		return -1;
	}

	close( fd1 );

	buf = (char *)buffer1;

	buf	   = strstr( buf, NETCARD );
	buf	   = strstr( buf, ":" );
	buf++;
	if( !isspace( *buf ) )
	{
		bytes[2] = strtoul( buf, &buf, 0 );
	}else
	{
		if( NULL == buf )
		{
			printf( "Error, buf is NULL !\n" );
			return -1;
		}

		while( !isspace( *buf ) )
		{
			buf++;
		}

		while( isspace( *buf ) )
		{
			buf++;
		}

		bytes[2] = strtoul( buf, &buf, 0 ); //new receive bytes
	}
	for( i = 0; i < 8; i++ )
	{
		while( !isspace( *buf ) )
		{
			buf++;
		}

		while( isspace( *buf ) )
		{
			buf++;
		}
	}

	bytes[3] = strtoul( buf, &buf, 0 );                     //new transmit bytes

	unsigned int rate = 0;

	rate = ( bytes[3] + bytes[2] - bytes[1] - bytes[0] );   //MB
	printf( "rate is: %ld, bytes[0]:%ld, bytes[1]:%ld, bytes[2]:%ld, bytes[3]:%ld.\n", rate, bytes[0], bytes[1], bytes[2], bytes[3] );
	return rate;
}

/***********************************************************
* Function:       // main
* Description:    // ������
***********************************************************/
int main( )
{
	unsigned int	cpu_num;
	unsigned int	uTotalOccupy, uIdleOccupy, uUsedOccupy;
	CPU_OCCUPY		ocpu;
	CPU_OCCUPY		ncpu;

	cpu_num = sysconf( _SC_NPROCESSORS_ONLN );

	while( 1 )
	{
		get_CPUActiveInfo( "cpu0", &ocpu );
		sleep( 3 );
		get_CPUActiveInfo( "cpu0", &ncpu );

		uTotalOccupy = ( ncpu.idle + ncpu.nice + ncpu.system + ncpu.user + ncpu.iowait + ncpu.hardirq + ncpu.softirq )
		               - ( ocpu.idle + ocpu.nice + ocpu.system + ocpu.user + ocpu.iowait + ocpu.hardirq + ocpu.softirq );

		if( uTotalOccupy == 0 )
		{
			return 0;
		}
		uUsedOccupy = ( ncpu.nice + ncpu.system + ncpu.user + ncpu.iowait + ncpu.hardirq + ncpu.softirq )
		              - ( ocpu.nice + ocpu.system + ocpu.user + ocpu.iowait + ocpu.hardirq + ocpu.softirq );

		uIdleOccupy = ncpu.idle - ocpu.idle;
		printf( "cpu, usage is %d \n", ( uTotalOccupy - uIdleOccupy ) * 100 / uTotalOccupy );

		getmemstate( );

		getnetcardspeed( );

		float rate=0;
		rate = (float)getnetcardrate()/1000000;
		printf("net rate is %3.6fMBps", getnetcardrate());
		
	}
	return 0;
}

/************************************** The End Of File **************************************/
