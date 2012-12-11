/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// fileOperByLinuxApi.c
 * Author:			// lmy
 * Date:			// 2011��5��27��
 * Description:		// Linux����Api�����ļ�����
 * Version:			//
 * History:			//
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#define MAX_BUF_LEN 1024


/***********************************************************
* Function:       // main
* Description:    // ������
***********************************************************/
int main( int argc, char ** argv )
{
	int		from_fd, to_fd;  //�ļ�������
	int		bytes_read, bytes_write;
	char	*ptr;
	char	buffer[MAX_BUF_LEN] = { 0 };

	if( argc != 3 )
	{
		fprintf( stderr, "Error! \nUsage:%s fromfile tofile \n \a", argv[0] );
		exit( 1 );
	}

	//��Դ�ļ�
	if( ( from_fd = open( argv[1], O_RDONLY ) ) == -1 )
	{
		fprintf( stderr, "Error! \nOpen %s Error:%s \n", argv[1], strerror( errno ) );
		exit( 1 );
	}
	//����Ŀ���ļ�
	if( ( to_fd = open( argv[2], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR ) ) == -1 )
	{
		fprintf( stderr, "Error! \nOpen %s Error:%s \n", argv[2], strerror( errno ) );
		exit( 1 );
	}
	//�����Ǿ����ļ���������
	while( (bytes_read = read( from_fd, buffer, MAX_BUF_LEN )) )
	{
		//printf("bytes_read:%d \n", bytes_read);
		if( (bytes_read == -1 ) && (errno != EINTR ))
		{
			printf("Error, read() fail, :%s \n", strerror( errno ) );
			break;
		}else if( bytes_read > 0 )
		{
			ptr = buffer;
			while( (bytes_write = write( to_fd, ptr, bytes_read )) )
			{
				//printf("bytes_write:%d \n", bytes_write);
				if( (bytes_write == -1) && (errno != EINTR) )
				{
					printf("Error, write() fail, :%s \n", strerror( errno ) );
					break;
				}else if( bytes_write == bytes_read )   //д���˱��ζ�ȡ�����ֽ�
				{
					break;
				}else if( bytes_write > 0 )             //ֻд��һ���֣�����д
				{
					ptr			  += bytes_write;
					bytes_read	  -= bytes_write;
				}
			}
			//д��ʱ������������
			if( bytes_write == -1 )
			{
				printf("Error, write() fail, bytes_write=%d.\n", bytes_write);
				break;
			}
		}
	}
	close( from_fd );
	close( to_fd );
	return 0;
}

/************************************** The End Of File **************************************/
