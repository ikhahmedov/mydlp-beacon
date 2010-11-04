/*
 * file   : autodiscover.h
 * project: MYDLP auto discovery daemon 
 * author : Huseyin Ozgur Batur 
 * date   : 2/11/2010
 * */

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

#include "listifc.h"

int init_socket();
int receive();
int broadcast();
int send_offer_msg();
int get_request_msg();
