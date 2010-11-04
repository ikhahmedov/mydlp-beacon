/*
 * file   : mydlp-beacon.c 
 * project: MYDLP autodiscovery daemon 
 * author : Huseyin Ozgur Batur 
 * date   : 2/11/2010
 * */

/* MYDLP autodiscovery protocol:
 *
 * client msg: mydlp 1.0 client mydlpreq
 *
 * server msg: mydlp 1.0 server 1 mydlpoffer IP
 *
 */

#include "mydlp-beacon.h"

#define PRODUCT_NAME "mydlp"
#define VERSION "1.0"
#define CLIENT "client"
#define SERVER "server"
#define REQUEST "mydlpreq"
#define OFFER "mydlpoffer"
#define PRIORITY "1"

#define DEFAULT_PORT 15000
#define MSGBUFSIZE 256

int bsockfd;
int lsockfd;
struct sockaddr_in broadcast_addr;
struct sockaddr_in listen_addr;
int addrlen;
int port = DEFAULT_PORT;
char msgbuf[MSGBUFSIZE];
u_int yes=1; 
char buffer[4][21];
char server_ip[16];

main ()
{
    init_socket(); 
    get_local_ip(server_ip);
    
    while (1) 
    {
        if (get_request_msg())
        {
            send_offer_msg();
        }

        sleep(1);
    }
}


int send_offer_msg()
{
    /* server msg: mydlp 1.0 server 1 mydlpoffer IP */
    sprintf(msgbuf,"%s %s %s %s %s %s", PRODUCT_NAME,
                    VERSION, SERVER, PRIORITY, OFFER, server_ip);
    broadcast(msgbuf);
}

int get_request_msg()
{
    int nbytes=0;
    int wcount=0;

    nbytes=receive();

    /* client msg: mydlp 1.0 client mydlpreq */
    wcount=sscanf(msgbuf, "%20s %20s %20s %20s",buffer[0],
        buffer[1],buffer[2],buffer[3]);
    
    
    /* Check Message Structure */
    if(wcount!=4 || strlen(msgbuf)!=nbytes)
    {
        printf("wcount %d, nbytes %d, msglenth %d\n",
                        wcount,nbytes,(int) strlen(msgbuf));    
        return 0;
    }
    
    if(strcmp(buffer[0],PRODUCT_NAME) != 0 )
    {
        printf("Invalid product name %s\n", buffer[0]);
        return 0;
    }

    if(strcmp(buffer[1],VERSION) != 0 )
    {
        printf("Uncompatible version %s\n", buffer[1]);
        return 0;
    }

    if(strcmp(buffer[2],CLIENT) != 0 )
    {
        if (strcmp(buffer[2],SERVER) != 0)
        {    
            printf("Agent not matched %s\n", buffer[2]);
            return 0;

        } else 
        {
            /* TODO:handle server priority */    
            return 0;

        }
    }

    if(strcmp(buffer[3],REQUEST) != 0 )
    {
        printf("Method not matched %s\n", buffer[3]);
        return 0;
    }
    
    fflush(stdout);
    return 1;
}

int broadcast(char * message)
{   
    int nbytes=0;
    if ((nbytes=sendto(bsockfd,message,strlen(message),0,
           (struct sockaddr *) &broadcast_addr, sizeof(broadcast_addr))) < 0) 
    {
        perror("Failed sendto");
        exit(EXIT_FAILURE);
    }  

    return nbytes;
}

int receive()
{   
    int nbytes=0;
    if ((nbytes=recvfrom(lsockfd,msgbuf,MSGBUFSIZE,0,
       (struct sockaddr *) &listen_addr,&addrlen)) < 0) 
    {
         perror("Failed recvfrom");
         exit(EXIT_FAILURE);
    }
    msgbuf[nbytes]=0;
    return nbytes;
}


int init_socket()
{
    int cnt;   

    /* create ordinary UDP socket */
    if ((bsockfd=socket(AF_INET,SOCK_DGRAM,0)) < 0) 
    {
        perror("Broadcast socket init failed");
        exit(1);
    }
    
    if ((lsockfd=socket(AF_INET,SOCK_DGRAM,0)) < 0) 
    {
        perror("Listening socket init failed");
        exit(1);
    }

    /* allow multiple sockets to use the same PORT number */
    if (setsockopt(lsockfd,SOL_SOCKET,SO_REUSEADDR,
        &yes,sizeof(yes)) < 0) 
    {
           perror("Reusing ADDR failed");
           exit(1);
    }

    const int on = 1;

    /* set broadcast option */
    setsockopt(bsockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
    
    /* set up address */
    memset(&broadcast_addr,0,sizeof(broadcast_addr));
    memset(&listen_addr,0,sizeof(listen_addr));

    broadcast_addr.sin_family=AF_INET;
    listen_addr.sin_family=AF_INET;

    broadcast_addr.sin_addr.s_addr=htonl(INADDR_BROADCAST);
    listen_addr.sin_addr.s_addr=htonl(INADDR_ANY);

    broadcast_addr.sin_port=htons(port);
    listen_addr.sin_port=htons(port);

    addrlen=sizeof(listen_addr);

    /* bind with receive address */
    if (bind(lsockfd,(struct sockaddr *) &listen_addr,
        sizeof(listen_addr)) < 0) 
    {
        perror("Unable to bind listening socket");
        exit(1);
    }
}


