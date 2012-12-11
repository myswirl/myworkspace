/*
fork 与 vfork
1.fork:子进程拷贝父进程的数据段
  vfork:子进程与父进程共享数据段
2.fork:父、子进程的执行次序不确定
  vfork:子进程先运行、父进程后运行
*/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

/***********************************************************
* Function:       // main
***********************************************************/
int main( void )
{
	pid_t pid;
	int count = 0;
	//此时仅有一个进程
	pid = vfork( );

	count++;
	
	//此时已经有两个进程在同时运行
	printf( "count = %d \n", count );
	
	return 0;
}
