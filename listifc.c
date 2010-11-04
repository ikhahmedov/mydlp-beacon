#include "listifc.h"

int get_local_ip(char *ip_addr)
{

    char          buf[1024];
    struct ifconf ifc;
    struct ifreq *ifr;
    int           sck;
    int           nInterfaces;
    int           i;
    char           ifname[4];

/* Get a socket handle. */
    sck = socket(AF_INET, SOCK_DGRAM, 0);
    if(sck < 0)
    {
        perror("socket");
        return 1;
    }

/* Query available interfaces. */
    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = buf;
    if(ioctl(sck, SIOCGIFCONF, &ifc) < 0)
    {
        perror("ioctl(SIOCGIFCONF)");
        return 1;
    }

/* Iterate through the list of interfaces. */
    ifr         = ifc.ifc_req;
    nInterfaces = ifc.ifc_len / sizeof(struct ifreq);
    for(i = 0; i < nInterfaces; i++)
    {
        struct ifreq *item = &ifr[i];

    /* Return the first local IP of host */
        if(is_local_addr(inet_ntoa(((struct sockaddr_in *)
                                         &item->ifr_addr)->sin_addr)))
        {
               
            if(strlen(item->ifr_name) >= 4) 
            { 
                strncpy(ifname, item->ifr_name, 3);
                ifname[3]=0;    
             
                if(strcmp(ifname,"eth")==0){
                    strcpy(ip_addr,inet_ntoa(
                        ((struct sockaddr_in *)
                             &item->ifr_addr)->sin_addr));
                    return 1; 
               }
           }
        }

    /* Get the MAC address 
        if(ioctl(sck, SIOCGIFHWADDR, item) < 0)
        {
            perror("ioctl(SIOCGIFHWADDR)");
            return 1;
        }

        if(ioctl(sck, SIOCGIFBRDADDR, item) >= 0)
            printf(", BROADCAST %s", inet_ntoa(((struct sockaddr_in *)
            &item->ifr_broadaddr)->sin_addr));
    */
    }

        return 0;
}

int is_local_addr(char *addr)
{
    return (is_addr_btw("10.0.0.0", addr, "10.255.255.255") ||
            is_addr_btw("172.16.0.0", addr, "172.31.255.255") ||
            is_addr_btw("192.168.0.0", addr, "192.168.255.255"));
}

int is_addr_btw(char* min, char* mid, char* max)
{

    struct in_addr mid_addr;    
    struct in_addr min_addr;
    struct in_addr max_addr;

    inet_aton(min,&min_addr); 
    inet_aton(mid,&mid_addr);  
    inet_aton(max,&max_addr);  

    if(ntohl(min_addr.s_addr) < ntohl(mid_addr.s_addr) &&
       ntohl(mid_addr.s_addr) < ntohl(max_addr.s_addr))
    {
        return 1;
    }

    return 0;                
}

/* test main
int main(void)
{
    char temp[100];
    get_local_ip(temp);
    printf("local ip: %s\n",temp);
}
*/
