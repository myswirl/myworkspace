#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>

int main(int argc, char *argv[])
{
    /* get mac */
    struct ifreq ifreq;
    int sock;
    
    if((sock=socket(AF_INET,SOCK_STREAM,0))<0)
    {
        perror("error");
        return 2;
    }
    strcpy(ifreq.ifr_name,"eth0");
    if(ioctl(sock,SIOCGIFHWADDR,&ifreq)<0)
    {
        perror("error:");
        return 3;
    }
   printf("%02X:%02X:%02X:%02X:%02X:%02X \n",
                (unsigned char)ifreq.ifr_hwaddr.sa_data[0],
                (unsigned char)ifreq.ifr_hwaddr.sa_data[1],
                (unsigned char)ifreq.ifr_hwaddr.sa_data[2],
                (unsigned char)ifreq.ifr_hwaddr.sa_data[3],
                (unsigned char)ifreq.ifr_hwaddr.sa_data[4],
                (unsigned char)ifreq.ifr_hwaddr.sa_data[5]);

  /*   for (i=0; i<6; i++)
        sprintf(mac+2*i, "%02x", (unsigned char)ifreq.ifr_hwaddr.sa_data[i]);
    printf("MAC: %s ", mac);*/
                
    return 0;
}
