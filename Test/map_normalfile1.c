/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// map_normalfile1.c
 * Author:			//
 * Date:			//
 * Description:		// IPC, 共享内存
 * Version:			//
 * History:			//
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>


typedef struct {
	char	name[4];
	int		age;
}people;


/***********************************************************
* Function:       // main
* Description:    // 主函数
***********************************************************/
int main( int argc, char** argv ) // map a normal file as shared mem:
{
	int		fd, i;
	people	*p_map;
	char	temp;

	fd = open( argv[1], O_CREAT | O_RDWR | O_TRUNC, 00777 );
	lseek( fd, sizeof( people ) * 5 - 1, SEEK_SET );
	write( fd, "", 1 );

	p_map = (people*)mmap( NULL, sizeof( people ) * 10, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0 );
	close( fd );
	temp = 'a';
	for( i = 0; i < 10; i++ )
	{
		temp += 1;
		memcpy( ( *( p_map + i ) ).name, &temp, 2 );
		( *( p_map + i ) ).age = 20 + i;
	}
	printf( " initialize over \n " );
	sleep( 10 );
	munmap( p_map, sizeof( people ) * 10 );
	printf( "umap ok \n" );
	return 0;
}

/************************************** The End Of File **************************************/
