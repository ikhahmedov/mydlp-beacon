/*
 *sender.c 
 *
 *Test tool: Broadcasts mydlp client mydlpreq messages continuously
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <stdlib.h>

#define HELLO_PORT 15000

main(int argc, char *argv[])
{
    struct sockaddr_in addr;
    int fd, cnt;
    struct ip_mreq mreq;
    char *message="mydlp 1.0 client mydlpreq";
    int ccount=0;

    struct ifreq ifr; 

//    char szBroadcast[255];     

    /* create an ordinary UDP socket */
    if ((fd=socket(AF_INET,SOCK_DGRAM,0)) < 0) 
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

/*    if (0 == ioctl(fd, SIOCGIFBRDADDR, &ifr))
    {
        strcpy(szBroadcast, inet_ntoa(((struct sockaddr_in *)&ifr.ifr_broadaddr)->sin_addr));
    }*/


    /* set up destination address */
    memset(&addr,0,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=INADDR_BROADCAST;
    addr.sin_port=htons(HELLO_PORT);
    
    const int on = 1;
    setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));

    while (1) 
    {
        if ((ccount=sendto(fd,message,strlen(message),0,(struct sockaddr *) &addr,
            sizeof(addr))) < 0) 
            {
                perror("sendto");
                exit(EXIT_FAILURE);
            }
       printf("sent count=%d, message count=%lu\n",ccount,sizeof(message));
       fflush(stdout);
       sleep(1);
    }
}

