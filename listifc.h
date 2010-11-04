#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>

int get_local_ip(char *ip_addr);
int is_local_addr(char *addr);
int is_addr_btw(char* min, char* mid, char* max);
