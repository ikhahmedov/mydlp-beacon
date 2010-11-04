/*
 * receiver.c 
 * Test tool MyDLP: prints all received broadcasts
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define HELLO_PORT 15000
#define HELLO_GROUP "225.0.0.37"
#define MSGBUFSIZE 256

main(int argc, char *argv[])
{
     struct sockaddr_in addr;
     int fd, nbytes,addrlen;
     struct ip_mreq mreq;
     char msgbuf[MSGBUFSIZE];
     u_int yes=1;  

     /* create what looks like an ordinary UDP socket */
     if ((fd=socket(AF_INET,SOCK_DGRAM,0)) < 0) {
      perror("socket");
      exit(EXIT_FAILURE);
     }


    /* allow multiple sockets to use the same PORT number */
    if (setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes)) < 0) {
       perror("Reusing ADDR failed");
       exit(EXIT_FAILURE);
       }

     /* set up address */
     memset(&addr,0,sizeof(addr));
     addr.sin_family=AF_INET;
     addr.sin_addr.s_addr=htonl(INADDR_ANY);
     addr.sin_port=htons(HELLO_PORT);
     
     /* bind to receive address */
     if (bind(fd,(struct sockaddr *) &addr,sizeof(addr)) < 0) {
      perror("bind");
      exit(EXIT_FAILURE);
     }
     
     mreq.imr_multiaddr.s_addr=inet_addr(HELLO_GROUP);
     mreq.imr_interface.s_addr=htonl(INADDR_ANY);
     if (setsockopt(fd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq)) < 0) {
      perror("setsockopt");
      exit(EXIT_FAILURE);
     }

     /* read-print loop */
     while (1) {
      addrlen=sizeof(addr);
      if ((nbytes=recvfrom(fd,msgbuf,MSGBUFSIZE,0,
                   (struct sockaddr *) &addr,&addrlen)) < 0) {
           perror("recvfrom");
           exit(EXIT_FAILURE);
      }
      msgbuf[nbytes]=0;
      puts(msgbuf);
     }
}

