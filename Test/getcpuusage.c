#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

int getcpuusage(void)

{
	int ret = 0;
	
	chdir("/usr/keygoe/temp/");
	
	system("top -n 1  -b > test4.txt");
	
	
	char buffer[4 * 1024  + 1];
	int i   = 0;

	int fd	= open("./test4.txt", O_RDONLY);
	if (fd < 0)
		return;	
	
	
	int len = read(fd, buffer, sizeof(buffer) - 1);
	if (len < 0)
	{
		close(fd);
		return;	
	}

	close(fd);

	char *buf = (char *)buffer;
	
	buf = strstr(buf, "CPU");
	if (NULL == buf)
		return;

	for(i = 0; i < 12; i++)
	{
		while(!isspace(*buf))
		{
			buf++;
		}

		while(isspace(*buf))
		{
			buf++;
		}

	}
 
	

	ret = strtoul(buf, &buf, 0);
	return ret; 

}


int main(void)
{
	while(1)
	{
		int ret = 0;
		ret = getcpuusage();
	
		printf("....%d .......\n", ret);
		sleep(3);
	}
}


