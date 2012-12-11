#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void my_func(int signal_no)
{
	if(signal_no == SIGINT)
	{
		printf("I got SIGINT\n");
	}else if(signal_no == SIGQUIT)
	{
		printf("I got SIGQUIT\n");
	}
}

int main(void)
{
	printf("Waiting for signal SIGINT or SIGQUIT \n");
	//注册信号处理函数
	signal(SIGINT,my_func);
	signal(SIGQUIT,my_func);

	pause();
	return 0;
}

