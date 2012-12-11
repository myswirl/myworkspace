#include <stdio.h>
#include <unistd.h>
#define MAX_PATH_LEN 256	//最大路径长度
int main(void)
{
	char buf[MAX_PATH_LEN]={0};
	getcwd(buf,sizeof(buf));
	printf("The current working path is: %s\n", buf);
	return 0;
}

