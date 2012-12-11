/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// fileSizeOperByC.c
 * Author:			// lmy
 * Date:			// 
 * Description:		// �ļ���������ȡ�ļ���С��C Lib, Linux��Windowsͨ�á�
 * Version:			// 
 * History:			// 
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <stdio.h>
long filesize( FILE *stream );


/***********************************************************
* Function:       // main
***********************************************************/
int main( void )
{
	FILE *stream;
	stream = fopen( "MYFILE.txt", "w+" );
	fprintf( stream, "This is a test, get the length of file." );
	printf( "Filesize of MYFILE.txt is %ld bytes\n", filesize( stream ) );
	fclose( stream );
	return 0;
}

/***********************************************************
* Function:       // filesize
* Description:    // ��ȡ�ļ���С
* Input:          // 1.stream
* Return:         // �����ļ��ֽ���
***********************************************************/
long filesize( FILE *stream )
{
	long curpos, length;
	curpos = ftell( stream );
	fseek( stream, 0L, SEEK_END );  //current position move to the end of the file
	length = ftell( stream );       //get the file size
	fseek( stream, curpos, SEEK_SET );
	return length;
}
/************************************** The End Of File **************************************/
