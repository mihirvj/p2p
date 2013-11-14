#include "data.h"
#include<netdb.h>
#include<sys/ioctl.h>
#include<net/if.h>
#include<string.h>

void connect_to(int sock, char *server_addr, int port);
void read_my_ip(char ip[50]);
